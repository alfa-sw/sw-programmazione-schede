""" A module to program Alfa PIC based boards using USB based bootloader.

Introduction to memory layout with 24 bit PIC ucontrollers
==========================================================

(These are just some remarks, don't condemn me to the stake if something is
wrong :-) )

Following is the first part of the Memory Table, exported from MPLAB IPE.

00000         FFFFFF    FFFFFF    002E14    002E14    ........ ........
00008         002E14    002E14    002E14    002E14    ........ ........
00010         002E14    002E14    002E14    002E14    ........ ........

    Address  ----+
     00008       |
                 v
       +----+----+----+----+
       |0x14|0x2E|0x00|<00>|
       +----+----+----+----+
 byte:    0    1    2    3

Memory is organized in groups of 4 bytes: the first 3 bytes contains real data
saved in the memory and is shown in the table; the last byte is called
*phantom byte*. Any hex binary representation of a program baked by microchip
compiler contains a "0" every 3 bytes.

There are only pair addresses: first group has address 0, second has address 2
and so on. (another possibility is that an address contains 2 bytes each -
let's check)

You can see that (calling a_b the address on a binary file; a_p the address
according to PIC24 architecture):

a_b = 2 * a_p

** USB commands always uses the addresses expressed in PIC24 architecture,
   but data is sent with phantom bytes -> double the address to seek the
   binary array and just copy the piece of program from hex to the
   message buffer. **

Communication to bootloader
===========================

Updating of a program is managed by the bootloader. When powered up, the
bootloader starts the application or puts itself in update mode.
Earlier versions of the bootloader switch to update mode simply when USB
is attached to a PC; newer versions allows to switch to update mode
by means of RS232 commands. After receiving the command, a flag is set, then,
if some conditions are met, there is a jump to the bootloader program.

Updating happens via USB commands to MAB/MMT. If the node to update
is a slave, MAB/MMT acts as a relay:
                                                 +-------+
                                          +----> | PUMP1 |
  +----+             +-----+              |      +-------+
  | PC | <-- usb --> | MAB | <--- rs485 --+----> | PUMPn |
  +----+             +-----+                     +-------+

When MAB goes to update mode, it sends a broadcast message to RS485, in order
to put all the slaves to update modality too.

Each USB message sent to bootloader has the following structure:

  +--------+------------------+
  | CMD_ID |     Payload      |
  | <byte> | <array of bytes> |
  +--------+------------------+

CMD_ID is a byte to identify command, Payload is a array of bytes which
size differs depending on the command. USB Messages to MAB have not a
fixed size - it is not needed to add trailing zeros.

Some messages - GET_DATA, QUERY - provide an answer, which size depends
on the command id - it is always the same with a given command id and
you have to strip away trailing zeros.

In order to select the proper node, each of them has its own *device ID*.
MAB has always a *device ID* = 0xFF. The booloader selects which node to
update by setting a parameter of QUERY message.

Switching to update mode
========================

Newer versions of bootloaders allows to switch to update mode when an
application is running. If correctly initialized, class AlfaFirmwareLoader
talks to the application using serial port to jump to boot.

1. check if the board is already in update mode, trying to communicate
   using USB; if so, finish OK, otherwise:
2. wait for the machine status parameter *status_level* to reach values
   0x06 (ALARM) or 0x04 (STANDBY) with a timeout of 180 secs.
   If the timeout happens, finish FAIL, otherwise:
3. send the command ENTER_DIAGNOSTIC and wait for status_level to
   have the value 0x07
4. monitor for 5 seconds the status_level to make sure it holds
   the value 0x07 - there is the possibility it changes to 0x06 and
   in this case go to 3 for max 3 times. If more, finish FAIL; otherwise,
5. take slaves configuration, fw and boot versions via proper commands;
6. send command JUMP_TO_APPLICATION and wait for status_level to go to 0x09
   for a timeout of 5 seconds
7. wait for 5 seconds, then check USB again
8. if USB is not available, still use serial port to read from application
   machine status any error code from the parameter *error_code*.

Caveats
=======

- In order to check if node is in update mode, you have to send a command and
  check the response in addition to successfully initialize the USB.
  It could happen that USB subsystem is initialized but not handled correctly
  by the firmware. For this reason we always send QUERY with device id 0.
- Sometimes firmware does not respond to commands. To work around this problem
  AlfaFirmwareLoader provides the possibility to retry the same command
  multiple times - a good value for cmdRetries parameter is 5.
- You should verify if the memory has been correctly programmed - seldom
  something goes wrong and verify fails and a new erase-programming-verify cycle
  should be performed.

"""

import asyncio

import usb.core
import usb.util
import struct
import logging

import time
import zipfile
from io import BytesIO
import yaml

from hexutils import HexUtils
from typing import NoReturn

from mab_serial_lib import SerialDriver, Protocol


class USBManager:
    """ This class implements the USB communication with bootloader """

    PASSWORD_QUERY = [0x82, 0x14, 0x2A, 0x5D, 0x6F, 0x9A, 0x25, 0x01]
    USB_ID_VENDOR = 0x04d8
    USB_ID_PRODUCT = 0xe89b
    DATA_ATTACHMENT_LEN = 56

    CMD_ID_QUERY = 0x2
    CMD_ID_ERASE = 0x4
    CMD_ID_PROGRAM = 0x5
    CMD_ID_PROGRAM_COMPLETE = 0x6
    CMD_ID_VERIFY = 0x7
    CMD_ID_BOOT_FW_VERSION_REQUEST = 0x0A
    CMD_ID_JUMP_TO_APPLICATION = 0x09
    CMD_ID_RESET_BOOT_MMT = 0x0B

    def repetible(method):
        def decorated(self, *args, **kwargs):
            if self.cmd_retries == 0:
                try:
                    return method(self, *args, **kwargs)
                except BaseException:
                    raise
            for i in range(1, self.cmd_retries + 1):
                try:
                    return method(self, *args, **kwargs)
                except Exception as e:
                    logging.warning(f"Tentative #{i} failed - exception: {e}")
                    if i >= self.cmd_retries:
                        logging.warning(f"Failed.")
                        raise
        return decorated

    def __init__(self, deviceId, cmdRetries, cmdTimeout):
        self.cmd_retries = cmdRetries
        self.cmd_timeout = cmdTimeout
        self._usb_init()
        self.deviceId = deviceId

    def disconnect(self):
        logging.debug("disconnecting USB")
        usb.util.dispose_resources(self.dev)

    def _usb_init(self):
        """ USB initialization """

        self.dev = usb.core.find(idVendor=self.USB_ID_VENDOR,
                                 idProduct=self.USB_ID_PRODUCT)
        if self.dev is None:
            raise RuntimeError("USB device not found")

        try:
            # needed in linux because there is a kernel driver that
            # take possession of our device
            self.dev.detach_kernel_driver(0)
        except BaseException:
            logging.info("failed to detach kernel driver")

        # following stuff is more or less a copy&paste
        # from PyUsb tutorial

        self.dev.set_configuration()

        cfg = self.dev.get_active_configuration()
        intf = cfg[(0, 0)]

        self.ep_out = usb.util.find_descriptor(
            intf,
            custom_match=lambda e:
            usb.util.endpoint_direction(e.bEndpointAddress) ==
            usb.util.ENDPOINT_OUT)

        self.ep_in = usb.util.find_descriptor(
            intf,
            custom_match=lambda e:
            usb.util.endpoint_direction(e.bEndpointAddress) ==
            usb.util.ENDPOINT_IN)

        assert self.ep_out is not None
        assert self.ep_in is not None

    def _send_usb_message(self, data, timeout=None):
        """ send a message to USB endpoint """

        data_to_send = list(data)
        # TODO use lazy evaluation here
        logging.debug("Writing data: {}".format(
            " ".join(["%02X" % int(b) for b in bytes(data_to_send)])
        ))

        timeout = timeout if not None else self.cmd_timeout
        ret = self.dev.write(self.ep_out, data_to_send, timeout)
        if ret != len(data_to_send):
            raise RuntimeError(
                "Returning value {} from write operation is not "
                "the expected one {}".format(
                    ret, len(data_to_send)))

    def _read_usb_message(self, length=64, timeout=None):
        """ receive a message from USB endpoint

        Note that communication will hang if the length is not the right one -
        this value depends on the command. """

        timeout = timeout if not None else self.cmd_timeout
        ret = self.dev.read(self.ep_in, length, timeout)

        # TODO use lazy evaluation here
        logging.debug("Read data: {}".format(
            " ".join(["%02X" % int(b) for b in bytes(ret)])
        ))
        return ret

    @repetible
    def QUERY(self, altDeviceId=None, timeout=None):
        """ Command to obtain information about the memory layout - starting
        address and length of the application memory. It is also used to
        check for erasing operation finish and to setup the following
        operations (e.g. to setup communication with a slave node, this is
        the only command taking the device ID as argument!)

        :return: a tuple:
          1. starting address of application memory
          2. length of application memory, i.e. number of available addresses
        """

        # Command message layout:
        #
        # +--------+------------+----------+
        # | CMD_ID |  PASSWORD  | DeviceID |
        # | <byte> |  <array>   | <byte>   |
        # +--------+------------+----------+
        #
        # Answer message layout:
        #
        # +--------+----------------+---------+-----------------+--------+--------+
        # | CMD_ID | BytesPerPacket | MemType | StartingAddress | Length | Marker |
        # | <byte> |     <byte>     | <byte>  |    <uint32>     |<uint32>| <byte> |
        # |  [2]   |      [2]       |  [1]    |       ?         |   ?    | [0xFF] |
        # +--------+----------------+---------+-----------------+--------+--------+
        # +--------------------------+-----------+-----------+-----------+-----------+
        # | BOOTLOADER_PROTO_VERSION | VER_MAJOR | VER_MINOR | VER_PATCH |  BOOT STS |
        # |        <byte>            |  <byte>   |  <byte>   |  <byte>   |  <byte>   |
        # |        [0/1]             |     ?     |   ?       |   ?       |   ?       |
        # +--------------------------+-----------+-----------+-----------+-----------+

        if altDeviceId is None:
            deviceId = self.deviceId
        else:
            deviceId = altDeviceId

        fmt = "<B{}sB".format(len(self.PASSWORD_QUERY))
        data = struct.pack(fmt, self.CMD_ID_QUERY,
                           bytes(self.PASSWORD_QUERY), deviceId)
        self._send_usb_message(data)
        buff = self._read_usb_message(64, timeout=5000)[:18]
        cmd_id, bytesPerPacket, bytesPerAddress, memoryType,  \
            address1, lenght1, type2, proto_ver, \
            ver_major, ver_minor, ver_patch, boot_status = \
            struct.unpack("<BBBBLLBBBBBB", buff)

        if proto_ver > 0:
            ver = (ver_major, ver_minor, ver_patch)
        else:
            ver = None

        try:
            assert cmd_id == self.CMD_ID_QUERY
            assert bytesPerPacket == self.DATA_ATTACHMENT_LEN
            assert bytesPerAddress == 2
            assert memoryType == 1
            assert type2 == 0xFF
            assert proto_ver == 0 or proto_ver == 1
        except BaseException:
            RuntimeError("Invalid data in QUERY response")

        if boot_status > 0:
            logging.warning(f"Reported boot status is {boot_status}")

        return (address1, lenght1, proto_ver, ver, boot_status)

    @repetible
    def ERASE(self) -> NoReturn:
        """ Erase the application memory. Do not return anything. """

        # Command message contains just
        #
        # No answer - uses QUERY message to synchronize.
        #

        data = struct.pack("<B", self.CMD_ID_ERASE)
        self._send_usb_message(data)

        # erase command does not produce any answer - send QUERY and wait
        # for the answer from this command
        self.QUERY(timeout=5000)

    def PROGRAM(self, address: int, chunk: bytes) -> NoReturn:
        """ Write a piece of memory. No return.

        :parameter address: starting address
        :parameter chunk: the array bytes to program

        """

        # Command message with *fixed size of 64 bytes* and layout:
        #
        # +--------+------------+----------------+----------+------+
        # | CMD_ID |  ADDRESS   | BytesPerPacket | Leading  | Data |
        # | <byte> |  <uint32>  |    <byte>      | zeros    |      |
        # +--------+------------+----------------+----------+------+
        #
        # No answer provided.

        if len(chunk) > self.DATA_ATTACHMENT_LEN:
            raise ValueError("too much bytes on the PROGRAM message")

        if len(chunk) < 58:
            # align to right
            array = [0] * (58 - len(chunk)) + list(chunk)

        data = struct.pack("<BLB58s", self.CMD_ID_PROGRAM, address, len(chunk),
                           bytes(array))
        self._send_usb_message(data)

    @repetible
    def PROGRAM_COMPLETE(self) -> NoReturn:
        """ Send the bootloader to signal that programming is complete.
        No return."""

        # Note that in some docs report some parameters but it is better
        # to use it just as a signal of program complete to bootloader.
        # No answer provided.

        data = struct.pack("<B", self.CMD_ID_PROGRAM_COMPLETE)
        self._send_usb_message(data)

    def GET_DATA(self, address: int, length: int) -> bytes:
        """ Read a piece of memory.

        :parameter address: starting address
        :parameter length: number of bytes to read

        :return: the chunk of bytes

        """

        # Command message layout:
        #
        # +--------+------------+----------+
        # | CMD_ID |  ADDRESS   | LENGTH   |
        # | <byte> |  <array>   | <byte>   |
        # +--------+------------+----------+
        #
        # Answer message layout:
        #
        # +--------+------------+----------------+----------------+
        # | CMD_ID |  ADDRESS   | BytesPerPacket | Trailing zeros |
        # | <byte> |  <uint32>  |     <byte>     |                |
        # +--------+------------+----------------+----------------+

        if length > self.DATA_ATTACHMENT_LEN:
            raise ValueError("too much bytes on the GET_DATA message")

        data = struct.pack("<BLB", self.CMD_ID_VERIFY, address, length)

        self._send_usb_message(data)
        buff = self._read_usb_message(64)

        cmd_id, address, bytesPerPacket, array = \
            struct.unpack(f"<BLB58s", buff)

        try:
            assert cmd_id == self.CMD_ID_VERIFY
            assert bytesPerPacket <= 58
        except BaseException:
            RuntimeError("Invalid data in GET_DATA response")

        return array[58 - bytesPerPacket:]

    @repetible
    def BOOT_FW_VERSION_REQUEST(self) -> tuple:
        data = struct.pack("<BB", self.CMD_ID_BOOT_FW_VERSION_REQUEST,
                           self.deviceId)
        self._send_usb_message(data)
        buff = self._read_usb_message(4)
        cmd_id, version_major, version_minor, version_patch = \
            struct.unpack("<BBBB", buff)

        try:
            assert cmd_id == self.CMD_ID_BOOT_FW_VERSION_REQUEST
        except BaseException:
            RuntimeError("Invalid data in BOOT_FW_VERSION_REQUEST response")

        return (version_major, version_minor, version_patch)

    @repetible
    def JUMP_TO_APPLICATION(self) -> NoReturn:
        data = struct.pack("<B", self.CMD_ID_JUMP_TO_APPLICATION)
        self._send_usb_message(data)

        # the command does not reply anything
        return

        # ~ buff = self._read_usb_message(1)
        # ~ cmd_id = struct.unpack("<B", buff)

        # ~ try:
        # ~    assert cmd_id == self.CMD_ID_BOOT_FW_VERSION_REQUEST
        # ~ except:
        #~    RuntimeError("Invalid data in JUMP_TO_APPLICATION response")

    @repetible
    def RESET_BOOT_MMT(self) -> NoReturn:
        data = struct.pack("<B", self.CMD_ID_RESET_BOOT_MMT)
        self._send_usb_message(data)

        # the command does not reply anything
        return

        buff = self._read_usb_message(1)
        cmd_id = struct.unpack("<B", buff)

        try:
            assert cmd_id == self.CMD_ID_RESET_BOOT_MMT
        except BaseException:
            RuntimeError("Invalid data in JUMP_TO_APPLICATION response")


class AlfaFirmwareLoader:
    """ Memory management of PIC24 based boards using USB protocol."""

    def __init__(self, deviceId=0xff, pollingMode=False, serialMode=False,
                 serialPort='/dev/ttyUSB0', pollingInterval=10, cmdRetries=0,
                 cmdTimeout=5000):
        """
        Instantiate an object of this class.
        Note: it is possible to select either the polling and serial strategies,
        but caller should not select them together, since they refers to
        different use cases.

        :parameter deviceId: id of the device to talk to
        :parameter pollingMode: boolean to enable the polling strategy
        :parameter serialMode: boolean to enable the serial/remote strategy
        :parameter serialPort: serial device filename
        :parameter pollingInterval: polling time in seconds
        """

        self.fw_versions = None
        self.boot_versions = None
        self.slaves_configuration = None

        usb_args = {
            "deviceId": deviceId,
            "cmdRetries": cmdRetries,
            "cmdTimeout": cmdTimeout
        }
        logging.debug(f"usb_args: {usb_args}")
        self.was_app_running = False
        try:
            if pollingMode:
                startTime = time.time()
                i = 0
                usb = None
                while not usb and time.time() - startTime < pollingInterval:
                    try:
                        usb = USBManager(**usb_args)
                        self.usb = usb
                    except Exception as e:
                        i += 1
                        logging.debug(
                            f"Polling USB, failed for the {i}-th time")
                        time.sleep(0.1)
                if not usb:
                    raise RuntimeError('failed to connect')
            else:
                self.usb = USBManager(**usb_args)

            # bootloader requires to receive QUERY with device id = 0 to avoid
            # jump-to-application
            self.usb.QUERY(altDeviceId=0)
        except Exception as e:
            logging.info(f"USB connection failed: {e}")
            if serialMode:
                try:
                    logging.info("jumping to boot")
                    self.jump_to_boot(serialPort)
                    self.was_app_running = True
                except BaseException:
                    raise RuntimeError(
                        "failed to jump to boot using serial commands")
            try:
                self.usb = USBManager(**usb_args)
            except BaseException:
                raise RuntimeError("failed to init USB device")

            self.usb.QUERY(altDeviceId=0)

        try:
            address, length, proto_ver, boot_fw_version, _ = self.usb.QUERY()

        except BaseException:
            raise RuntimeError("failed to query device")

        logging.info("Response to QUERY, address = {}, length = {}, "
                     "proto_ver = {}, boot_ver = {}"
                     .format(address, length, proto_ver, boot_fw_version))

        self.starting_address = address
        self.memory_length = length
        self.boot_fw_version = boot_fw_version
        self.erased = False

    def jump_to_boot(self, serial_filename: str) -> NoReturn:
        """ use serial commands to make the application jump to bootloader /
        update mode.

        :argument serial_filename: the device file name
        """
        baudrate = 115200
        src_addr = 200
        mode = Protocol.ProtocolMode.DUPLEX

        def dec_slaves_config(slaves_config):
            # decode slaves_config
            src = slaves_config['slave_ids']
            out = []  # array of slaves addresses
            for i in range(0, len(src) * 8):
                bit = ((src[i // 8] >> (i % 8))) & 1
                if bit > 0:
                    out.append(i + 1)
            return out

        def dec_boot_versions(src):
            """ decode out parameters of command boot versions """

            def get_ver_tuple(x): return (x[0], x[1], x[2])

            s = src['boot_slaves_protocol_version']

            out = []
            for i in range(0, len(s) // 4):
                out.append(
                    (s[i * 4], (s[i * 4 + 1], s[i * 4 + 2], s[i * 4 + 3])))

            return {
                'boot_master_protocol': src['boot_master_protocol'][0],
                'boot_master': get_ver_tuple(src['boot_master']),
                'boot_slaves': out
            }

        def dec_fw_versions(src):
            """ decode out parameters of command fw versions """

            def get_ver_tuple(x): return (x[0], x[1], x[2])

            s = src['slaves']

            out = []
            for i in range(0, len(s) // 3):
                out.append(get_ver_tuple(
                    [s[i * 3], s[i * 3 + 1], s[i * 3 + 2]]))

            return {
                'MAB_MGB_protocol': src['MAB_MGB_protocol'][0],
                'master': get_ver_tuple(src['master']),
                'slaves': out
            }

        async def operations():
            endpoint = SerialDriver()
            try:
                endpoint.connect(device_name=serial_filename,
                                 device_baudrate=baudrate)

                proto = Protocol(mode, endpoint)
                node = proto.attach_node(src_addr)

                assert(await node.wait_for_event(
                    node.EventLabel.NODE_STATUS_CHANGED, node.NodeStatus.READY, 5) is not None)

                logging.debug(
                    "waiting for status_level to reach values 0x06 or 0x04")
                assert(await node.wait_for_recv_status_parameters(
                    {"status_level": lambda x: x == 0x06 or x == 0x04},
                    180))

                ok = False
                for i in range(0, 3):
                    logging.debug(
                        "command the node to enter diagnostic status")
                    assert (await node.send_request_and_watch(
                        "ENTER_DIAGNOSTIC",
                        status_params={"status_level": 0x07},
                        timeout_status_sec=25))[0] == node.RequestWatchResult.SUCCESS
                    again_to_reset_sts = await node.wait_for_recv_status_parameters(
                        {"status_level": lambda x: x != 0x07}, 5)
                    if not again_to_reset_sts:
                        ok = True
                        break
                    else:
                        logging.warning("node exited the diagnostic status")

                assert ok

                (result, out_params) = await node.send_request_and_watch(
                    "READ_SLAVES_CONFIGURATION", timeout_status_sec=2)
                if result == node.RequestWatchResult.SUCCESS:
                    self.slaves_configuration = dec_slaves_config(out_params)
                else:
                    logging.warning("failed to retrieve slaves configuration")

                (result, out_params) = await node.send_request_and_watch(
                    "FW_VERSIONS", timeout_status_sec=2)
                if result == node.RequestWatchResult.SUCCESS:
                    self.fw_versions = dec_fw_versions(out_params)
                else:
                    logging.warning("failed to retrieve fw versions")

                (result, out_params) = await node.send_request_and_watch(
                    "BOOT_VERSIONS", timeout_status_sec=2)
                if result == node.RequestWatchResult.SUCCESS:
                    self.boot_versions = dec_boot_versions(out_params)
                else:
                    logging.warning("failed to retrieve boot versions")

                (result, answer_params) = await node.send_request_and_watch(
                    "DIAG_JUMP_TO_BOOT", status_params={"status_level": 0x09},
                    timeout_status_sec=5)
                assert result == node.RequestWatchResult.SUCCESS

                await endpoint.disconnect()
                # wait for OS and drivers to initialize
                await asyncio.sleep(10)
            finally:
                await endpoint.disconnect()

        # using threads requires to create a new event loop
        event_loop = asyncio.new_event_loop()
        event_loop.run_until_complete(operations())

    def erase(self) -> NoReturn:
        """ erase application memory. """

        try:
            self.usb.ERASE()
            self.erased = True
        except BaseException:
            raise RuntimeError("failed to perform erase")

    def program(self, program_data: list) -> NoReturn:
        """ program the application on the proper memory space.

        :argument program_data: the entire application as a vector of bytes
        """

        if not self.erased:
            logging.warning("erase procedure not performed")

        try:
            program_segment = program_data[self.starting_address * \
                2: self.starting_address * 2 + self.memory_length * 2]
        except BaseException:
            raise RuntimeError("dimension of program does not fit memory")

        cursor = 0
        while cursor < len(program_segment):
            try:
                chunk_len = self.usb.DATA_ATTACHMENT_LEN
                if chunk_len + cursor > len(program_segment):
                    chunk_len = len(program_segment) - cursor
                    logging.debug("Chunk len is {}".format(chunk_len))
                chunk = bytes(program_segment[cursor:cursor + chunk_len])
                logging.debug("programming on address {} chunk {} "
                              .format(self.starting_address + cursor,
                                      " ".join(["%02X" % int(b) for b in chunk])))
                self.usb.PROGRAM(self.starting_address + cursor // 2, chunk)
                cursor += chunk_len
            except BaseException:
                raise RuntimeError("programming failed between program "
                                   "positions {} and {}".format(
                                       cursor, cursor + chunk_len))

        try:
            self.usb.PROGRAM_COMPLETE()
        except BaseException:
            raise RuntimeError("program failed during finalization")

    def verify(self, program_data: list) -> bool:
        """ verify the application memory on device against the given one.

        :argument program_data: the entire application as a vector of bytes
        :return: a boolean
        """

        try:
            program_segment = program_data[self.starting_address * \
                2: self.starting_address * 2 + self.memory_length * 2]
        except BaseException:
            raise RuntimeError("dimension of program does not fit memory")

        cursor = 0
        while cursor < len(program_segment):
            try:
                chunk_len = self.usb.DATA_ATTACHMENT_LEN
                if chunk_len + cursor > len(program_segment):
                    chunk_len = len(program_segment) - cursor
                    logging.debug("Chunk len is {}".format(chunk_len))
                chunk = bytes(program_segment[cursor:cursor + chunk_len])
                read_chunk = self.usb.GET_DATA(
                    self.starting_address + cursor // 2, chunk_len)

                if chunk != read_chunk:
                    logging.info("read {} is different from file {}"
                                 .format(" ".join(["%02X" % int(b) for b in read_chunk]),
                                         " ".join(["%02X" % int(b) for b in chunk])))
                    return False

                cursor += chunk_len
            except BaseException:
                raise RuntimeError("verify failed between program "
                                   "positions {} and {}".format(
                                       cursor, cursor + chunk_len))
        return True

    def reset(self) -> NoReturn:
        """ reset slaves and main boards. """

        try:
            self.usb.RESET_BOOT_MMT()
        except BaseException:
            raise RuntimeError("failed to perform reset")

    def jump(self) -> NoReturn:
        """ jump to main program. """

        try:
            self.usb.JUMP_TO_APPLICATION()
        except BaseException:
            raise RuntimeError("failed to jump to application")

    def disconnect(self):
        self.usb.disconnect()


class AlfaPackageLoader:
    class UserInterrupt(Exception):
        pass

    def __init__(self, package_data, conn_args, process_callback=None):
        self.package_data = package_data
        self.conn_args = conn_args
        self.process_callback = process_callback

        self.sts = {
            "process": {
                "current_op": "",
                "step": 0,
                "total_steps": 0,
                "subprocess": ""},
            "subprocess": {
                "current_op": "",
                "step": 0,
                "total_steps": 0}}

    def report_problem(self, problem):
        self.process_callback(status=None, problem=problem)

    def update_status(
            self,
            caller,
            current_op=None,
            step=None,
            total_steps=None):
        if caller == "main":
            key = "process"
            self.sts["subprocess"] = {
                "current_op": "", "step": 0, "total_steps": 0}
            self.sts["process"]["subprocess"] = ""
        else:
            key = "subprocess"
            self.sts["process"]["subprocess"] = caller

        if current_op is not None:
            self.sts[key]["current_op"] = current_op
        if step is not None:
            self.sts[key]["step"] = step
        if current_op is not None:
            self.sts[key]["total_steps"] = total_steps

        if self.process_callback(status=self.sts, problem=None):
            raise self.UserInterrupt

    def process(self):
        current_step = 1
        self.update_status("main", "loading package", 1, 4)
        self.load_package(self.package_data)

        current_step += 1
        self.update_status("main", "initialize", 2, 4)

        initialize_ok = False
        try:
            self.board_init()
            initialize_ok = True
        except Exception as e:
            logging.warning(
                f"need to reinitialize after programming master ({str(e)})")

        master_node = list(filter(
            lambda x: x["board-name"] == "master",
            self.manifest["programs"]))[0]

        conn_args = self.conn_args
        conn_args["deviceId"] = 255
        conn_args["serialMode"] = False
        conn_args["pollingMode"] = False

        self.update_status("main", "programming master", 3, 4)
        try:
            afl = None
            afl = AlfaFirmwareLoader(**conn_args)
            afl.erase()
            hexdata = self.programs_hex[master_node['filename']]
            afl.program(hexdata)
            afl.verify(hexdata)
        except Exception as e:
            self.report_problem("failed to program master 1st attempt")
            if not initialize_ok:
                raise RuntimeError(
                    f"failed to program master and to initialize ({str(e)})")
        finally:
            if afl is not None:
                afl.disconnect()

        if not initialize_ok:
            try:
                self.board_init()
            except BaseException:
                raise RuntimeError("failed to initialize")
            finally:
                afl.disconnect()

        program_steps = {}
        for program in self.manifest["programs"]:
            for addr in program['addresses']:
                if addr != 255 and addr in self.slaves_configuration:
                    program_steps[addr] = program
        logging.debug(f"programs steps: {program_steps}")

        current_step = 1
        total_steps = len(program_steps)

        self.update_status("main", "programming slaves", 4, 4)
        for address, step in program_steps.items():
            self.update_status("slaves", f"programming slave #{address}",
                               current_step, total_steps)

            conn_args["deviceId"] = address
            try:
                afl = AlfaFirmwareLoader(**conn_args)
                afl.erase()
                afl.program(self.programs_hex[step['filename']])
                afl.disconnect()
            except BaseException:
                self.report_problem(
                    f"failed to program slave with address {address}")
                if not initialize_ok:
                    raise RuntimeError(
                        "failed to program master and to initialize")

            current_step += 1

    def board_init(self):
        conn_args = self.conn_args
        conn_args["deviceId"] = 255
        conn_args["serialMode"] = True
        conn_args["pollingMode"] = False

        in_boot_mode = False

        self.update_status(
            "init", "retrieve data version and jump to boot", 1, 3)

        def check_invalid_ver(ufl): return ufl.was_app_running is False or \
            ufl.fw_versions is None or ufl.boot_versions is None or \
            ufl.slaves_configuration is None

        try:
            afl = None
            afl = AlfaFirmwareLoader(**conn_args)
            if check_invalid_ver(afl):
                logging.warning("app was not running or problem in retrieving "
                                "version data -> jump to app and retry")
                self.update_status("init", "jump to app", 2, 3)

                afl.jump()
                afl.disconnect()
                time.sleep(5)
                self.update_status("init", "jump to boot again", 3, 3)
                afl = AlfaFirmwareLoader(**conn_args)

                if check_invalid_ver(afl):
                    raise RuntimeError(
                        "app was not running or problem in retrieving "
                        "version data")

            self.boot_versions = afl.boot_versions
            self.fw_versions = afl.fw_versions
            self.slaves_configuration = afl.slaves_configuration

        except Exception as e:
            raise e
        finally:
            if afl is not None:
                afl.disconnect()

    def load_package(self, package_data):
        fp = BytesIO(package_data)
        zfp = zipfile.ZipFile(fp, "r")

        with zfp.open('manifest.txt', 'r') as mfp:
            self.manifest = yaml.load(mfp, Loader=yaml.SafeLoader)

        self.programs_hex = {}
        for program in self.manifest["programs"]:
            fn = program["filename"]
            with zfp.open(fn) as f:
                self.programs_hex[fn] = HexUtils.load_hex_to_array(
                    f.read().decode())
