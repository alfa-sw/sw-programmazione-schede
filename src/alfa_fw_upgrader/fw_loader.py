"""
alfa_fw_upgrader - a package to program Alfa PIC based boards using USB based bootloader.

This module contains code to jump to boot and safely load a firmware.

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
   have the value 0x07. Repeat at least 3 times in case of timeout.
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

# pylint: disable=invalid-name
# pylint: disable=missing-docstring
# pylint: disable=broad-except
# pylint: disable=logging-format-interpolation
# pylint: disable=logging-fstring-interpolation

import asyncio

import usb.core
import usb.util
import struct
import logging
import time
import zipfile
from io import BytesIO
import yaml
from crc import CrcCalculator, Crc16

from alfa_fw_upgrader.hexutils import HexUtils
from alfa_fw_upgrader.usb import USBManager

from typing import NoReturn

from alfa_serial_lib import Protocol, Node, Request

class AlfaFirmwareLoader:
    """ Memory management of PIC24 based boards using USB protocol."""

    def __init__(self, deviceId=0xff, pollingMode=False, serialMode=False,
                 serialPort='/dev/ttyUSB0', pollingInterval=10, cmdRetries=0,
                 cmdTimeout=15000):
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

        self._current_program_data = None
        self._current_program_segment = None
        self._current_checksum = None

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
                    except Exception:
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
                except BaseException as e:
                    raise RuntimeError(
                        "failed to jump to boot using serial commands") from e
            try:
                self.usb = USBManager(**usb_args)
            except BaseException as e:
                raise RuntimeError("failed to init USB device") from e

            self.usb.QUERY(altDeviceId=0)

        self.starting_address = None
        self.memory_length = None
        self.erased = False
        self._update_from_query()

    def _update_from_query(self):
        """ update object members from answer to QUERY """
        try:
            address, length, proto_ver, boot_version, boot_status, digest = \
                self.usb.QUERY()
        except BaseException as e:
            raise RuntimeError("failed to query device") from e

        logging.info(
            "Response to QUERY, address = {}, length = {}, proto_ver = {},"
            "boot_ver = {} boot_status = {} digest = {}" .format(
                address,
                length,
                proto_ver,
                boot_version,
                boot_status,
                digest))

        assert(self.starting_address is None or self.starting_address == address)
        assert(self.memory_length is None or self.memory_length == length)

        self.starting_address = address
        self.memory_length = length
        self.boot_fw_version = boot_version
        self.proto_ver = proto_ver
        self.boot_status = boot_status
        self.digest = digest

    def _program_data_process(self, program_data) -> tuple:
        """ extract the segment of the program data corresponding to
        application memory and perform CRC16 calculation """

        if self._current_program_data == program_data:
            return (self._current_program_segment, self._current_checksum)

        try:
            program_segment = program_data[self.starting_address * 2:
                                           self.starting_address * 2
                                           + self.memory_length * 2]
        except BaseException as e:
            raise RuntimeError(
                "dimension of program does not fit memory") from e

        try:
            data = program_segment
            crc_calculator = CrcCalculator(Crc16.CCITT)
            checksum = crc_calculator.calculate_checksum(data)
            logging.info(f"Calculated checksum is {checksum}")
        except BaseException as e:
            raise RuntimeError("calculation of CRC failed") from e

        self._current_program_segment = program_segment
        self._current_checksum = checksum
        return (program_segment, checksum)

    async def _get_configuration_duplex(self, master: Node):
        result = await master.send_request_and_wait("READ_SLAVES_CONFIGURATION")
        if result.status != result.RequestStatus.SUCCESS:
            logging.warning("failed to retrieve slaves configuration")
        self.slaves_configuration = result.custom_answer_dict

        result = await master.send_request_and_wait("FW_VERSIONS")
        if result.status != result.RequestStatus.SUCCESS:
            logging.warning("failed to retrieve fw versions")
        self.fw_versions = result.custom_answer_dict

        result = await master.send_request_and_wait("BOOT_VERSIONS")
        if result.status != result.RequestStatus.SUCCESS:
            logging.warning("failed to retrieve boot versions")
        self.boot_versions = result.custom_answer_dict

    def jump_to_boot(self, serial_filename: str) -> NoReturn:
        """ use serial commands to make the application jump to bootloader /
        update mode.

        :argument serial_filename: the device file name
        """
        baudrate = 115200
        src_addr = 200
        mode = Protocol.ProtocolMode.DUPLEX

        async def operations():
            try:
                conn_params = dict(device_name=serial_filename,
                                   device_baudrate=baudrate)

                proto = Protocol(mode, conn_params)
                node = proto.attach_node(src_addr)

                task = asyncio.ensure_future(proto.run())

                assert await node.wait_for_status(
                    {"status_level": lambda x: x != "POWER_OFF"}, 300)

                ok = False
                for _ in range(0, 3):
                    logging.debug(
                        "command the node to enter diagnostic status")
                    j = 0
                    result = None
                    while j < 5 and result != Request.RequestStatus.SUCCESS:
                        req = await node.send_request_and_wait("ENTER_DIAGNOSTIC")
                        result = req.status
                        j = j + 1
                    assert result == Request.RequestStatus.SUCCESS
                    again_to_reset_sts = await node.wait_for_status(
                        {"status_level": lambda x: x != "DIAGNOSTIC"}, 5)
                    if not again_to_reset_sts:
                        ok = True
                        break
                    logging.warning("node exited the diagnostic status")

                assert ok

                await self._get_configuration_duplex(node)

                node.send_request("DIAG_JUMP_TO_BOOT")

                # do not wait for a response, just time to send command
                await asyncio.sleep(1)
                
                try:
                    task.cancel()
                    await task
                except asyncio.CancelledError:
                    logging.info(f"task cancelled {task}")
                proto.serial.close()

                # wait for OS and drivers to initialize
                await asyncio.sleep(10)
            finally:
                try:
                    task.cancel()
                    await task
                except asyncio.CancelledError:
                    logging.info(f"task cancelled {task}")

        # using threads requires to create a new event loop
        event_loop = asyncio.new_event_loop()
        event_loop.run_until_complete(operations())

    def erase(self) -> NoReturn:
        """ erase application memory. """

        try:
            self.usb.ERASE()
            self.erased = True
        except BaseException as e:
            raise RuntimeError("failed to perform erase") from e

    def program(self, program_data: list) -> NoReturn:
        """ program the application on the proper memory space.

        :argument program_data: the entire application as a vector of bytes
        """

        if not self.erased:
            logging.warning("erase procedure not performed")

        (program_segment, digest) = self._program_data_process(program_data)

        cursor = 0
        while cursor < len(program_segment):
            try:
                chunk_len = self.usb.DATA_ATTACHMENT_LEN
                if chunk_len + cursor > len(program_segment):
                    chunk_len = len(program_segment) - cursor
                    logging.debug("Chunk len is %d", chunk_len)
                chunk = bytes(program_segment[cursor:cursor + chunk_len])
                if logging.getLogger().isEnabledFor(logging.DEBUG):
                    logging.debug("programming on address {} chunk {} "
                                  .format(self.starting_address + cursor,
                                          " ".join(["%02X" % int(b) for b in chunk])))
                self.usb.PROGRAM(self.starting_address + cursor // 2, chunk)
                cursor += chunk_len
            except BaseException as e:
                raise RuntimeError("programming failed between program "
                                   "positions {} and {}".format(
                                       cursor, cursor + chunk_len)) from e

    def seal(self, program_data: list) -> NoReturn:
        """ set the digest value. To call after programming and verifying the
        application.

        :argument program_data: the entire application as a vector of bytes
        """

        (_, digest) = self._program_data_process(program_data)

        try:
            self.usb.PROGRAM_COMPLETE(digest)
            if self.proto_ver > 0:
                time.sleep(1)
                self._update_from_query()
                if self.digest != digest:
                    raise RuntimeError(
                        "digest not correctly saved by bootloader")

        except BaseException as e:
            raise RuntimeError("program failed during finalization") from e

    def verify(self, program_data: list, check_digest=True) -> bool:
        """ verify the application memory on device against the given one.

        :argument program_data: the entire application as a vector of bytes
        :argument check_digest: flag to check digest value
        :return: a boolean
        """

        (program_segment, digest) = self._program_data_process(program_data)

        cursor = 0
        while cursor < len(program_segment):
            try:
                chunk_len = self.usb.DATA_ATTACHMENT_LEN
                if chunk_len + cursor > len(program_segment):
                    chunk_len = len(program_segment) - cursor
                    logging.debug(f"Chunk len is {chunk_len}")
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

        if check_digest and self.proto_ver > 0:
            self._update_from_query()
            if self.digest != digest:
                raise RuntimeError(
                    "digest not correctly saved by bootloader")

        return True

    def reset(self) -> NoReturn:
        """ reset slaves and main boards. """

        try:
            self.usb.RESET_BOOT_MMT()
            return
        except BaseException as e:
            raise RuntimeError("failed to perform reset") from e

    def jump(self) -> NoReturn:
        """ jump to main program. """

        try:
            self.usb.JUMP_TO_APPLICATION()
        except BaseException as e:
            raise RuntimeError("failed to jump to application") from e

    def disconnect(self):
        self.usb.disconnect()
