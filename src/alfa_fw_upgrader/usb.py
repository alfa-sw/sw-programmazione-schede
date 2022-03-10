"""
alfa_fw_upgrader - a package to program Alfa PIC based boards using USB based bootloader.

This module implements USB commands.

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
"""

import usb.core
import usb.util
import struct
import logging

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
            usb.util.endpoint_direction(e.bEndpointAddress)
            == usb.util.ENDPOINT_OUT)

        self.ep_in = usb.util.find_descriptor(
            intf,
            custom_match=lambda e:
            usb.util.endpoint_direction(e.bEndpointAddress)
            == usb.util.ENDPOINT_IN)

        assert self.ep_out is not None
        assert self.ep_in is not None

    def _send_usb_message(self, data, timeout=None):
        """ send a message to USB endpoint """

        data_to_send = list(data)
        if timeout is None:
            timeout = self.cmd_timeout

        if logging.getLogger().isEnabledFor(logging.DEBUG):
            logging.debug("Writing data: {}".format(
                " ".join(["%02X" % int(b) for b in bytes(data_to_send)])))
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

        if timeout is None:
            timeout = self.cmd_timeout
        ret = self.dev.read(self.ep_in, length, timeout)

        if logging.getLogger().isEnabledFor(logging.DEBUG):
            logging.debug("Read data: {}".format(
                " ".join(["%02X" % int(b) for b in bytes(ret)])))
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
        # +--------+----------------+---------+-----------+--------+--------+
        # | CMD_ID | BytesPerPacket | MemType | StartAddr | Length | Marker |
        # | <byte> |     <byte>     | <byte>  | <uint32>  |<uint32>| <byte> |
        # |  [2]   |      [2]       |  [1]    |    ?      |   ?    | [0xFF] |
        # +--------+----------------+---------+-----------+--------+--------+
        # +--------------+----------+----------+----------+---------+---------+
        # |BOOTLOADER    | VER_MAJOR| VER_MINOR| VER_PATCH| BOOT STS| DIGEST  |
        # |_PROTO_VERSION|  <byte>  |  <byte>  |  <byte>  | <byte>  | <uint16>|
        # | <byte>[0/1]  |     ?    |   ?      |   ?      |  ?      |  ?      |
        # +--------------+----------+----------+----------+---------+---------+

        if altDeviceId is None:
            deviceId = self.deviceId
        else:
            deviceId = altDeviceId

        fmt = "<B{}sB".format(len(self.PASSWORD_QUERY))
        data = struct.pack(fmt, self.CMD_ID_QUERY,
                           bytes(self.PASSWORD_QUERY), deviceId)
        self._send_usb_message(data)

        if timeout is None:
            timeout = self.cmd_timeout

        buff = self._read_usb_message(64, timeout=timeout)[:20]
        cmd_id, bytesPerPacket, bytesPerAddress, memoryType,  \
            address1, lenght1, type2, proto_ver, \
            ver_major, ver_minor, ver_patch, boot_status, digest = \
            struct.unpack("<BBBBLLBBBBBBH", buff)

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
            assert proto_ver in (0, 1)
        except BaseException:
            RuntimeError("Invalid data in QUERY response")

        if boot_status > 0:
            logging.warning(f"Reported boot status is {boot_status}")

        return (address1, lenght1, proto_ver, ver, boot_status, digest)

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

        timeout = 5000  # response takes longer time after erase command
        if self.cmd_timeout > timeout:
            timeout = self.cmd_timeout

        self.QUERY(timeout=timeout)

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
    def PROGRAM_COMPLETE(self, digest: int) -> NoReturn:
        """ Send the bootloader to signal that programming is complete.
        :parameter digest: 16 bit word resulting from hashing the program
        No return."""

        trailing = [0xFF] * 61  # command requires trailing sequence of 0xFF
        data = struct.pack("<BH61s", self.CMD_ID_PROGRAM_COMPLETE,
                           digest, bytes(trailing))
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
        # ~    RuntimeError("Invalid data in JUMP_TO_APPLICATION response")

    @repetible
    def RESET_BOOT_MMT(self) -> NoReturn:
        data = struct.pack("<B", self.CMD_ID_RESET_BOOT_MMT)
        self._send_usb_message(data)

        # the command does not reply anything
        # ~ buff = self._read_usb_message(1)
        # ~ cmd_id = struct.unpack("<B", buff)
        # ~
        # ~ try:
        # ~   assert cmd_id == self.CMD_ID_RESET_BOOT_MMT
        # ~ except BaseException:
        # ~   RuntimeError("Invalid data in JUMP_TO_APPLICATION response")
