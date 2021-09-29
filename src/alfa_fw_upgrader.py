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

"""

import asyncio

import usb.core
import usb.util
import struct
import logging

import argparse
import sys
import traceback

import time

from hexutils import HexUtils
from typing import Any, NoReturn

from mab_serial_lib import SerialDriver, Protocol

class USBManager:
    """ This class implements the USB communication with bootloader 
    
    Each message sent to bootloader has the following structure:
    
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
    
    Topology is the following:
                                                     +-------+
                                              +----> | PUMP1 |
      +----+             +-----+              |      +-------+
      | PC | <-- usb --> | MAB | <--- rs485 --+----> | PUMPn |
      +----+             +-----+                     +-------+
      
    In order to select the proper node, each of them has its own *device ID*.
    MAB has always a *device ID* = 0xFF. If the desired ID is not 0xFF,
    bootloader acts as a relay node:
     1. setup the slave node: it send a command on 485 to make it jump to 
        bootloader;
     2. transferring data from the PC and vice-versa.

    """
    
    PASSWORD_QUERY = [0x82, 0x14, 0x2A, 0x5D, 0x6F, 0x9A, 0x25, 0x01]
    USB_ID_VENDOR = 0x04d8
    USB_ID_PRODUCT = 0xe89b
    DATA_ATTACHMENT_LEN = 56

    CMD_ID_QUERY = 0x2
    CMD_ID_ERASE = 0x4
    CMD_ID_PROGRAM = 0x5
    CMD_ID_PROGRAM_COMPLETE = 0x6
    CMD_ID_VERIFY = 0x7
    CMD_ID_BOOT_FW_VERSION_REQUEST = 0x8
    
    def __init__(self, deviceId = 0xff):
        self._usb_init()
        self.deviceId = deviceId
        
    def _usb_init(self):
        """ USB initialization """
        
        self.dev = usb.core.find(idVendor = self.USB_ID_VENDOR,
                                 idProduct = self.USB_ID_PRODUCT)
        if self.dev is None:
            raise RuntimeError("USB device not found")           
        
        try:
            # needed in linux because there is a kernel driver that
            # take possession of our device
            self.dev.detach_kernel_driver(0)
        except:
            logging.info("failed to detach kernel driver")

        # following stuff is more or less a copy&paste
        # from PyUsb tutorial

        self.dev.set_configuration()

        cfg = self.dev.get_active_configuration()
        intf = cfg[(0,0)]

        self.ep_out = usb.util.find_descriptor(
            intf,
            custom_match = \
            lambda e: \
                usb.util.endpoint_direction(e.bEndpointAddress) == \
                usb.util.ENDPOINT_OUT)

        self.ep_in = usb.util.find_descriptor(
            intf,
            custom_match = \
            lambda e: \
                usb.util.endpoint_direction(e.bEndpointAddress) == \
                usb.util.ENDPOINT_IN)

        assert self.ep_out is not None
        assert self.ep_in is not None
        
        self.dev.reset()
        
        
    def _send_usb_message(self, data, timeout = 5000):
        """ send a message to USB endpoint """
        
        data_to_send = list(data) 
        ret = self.dev.write(self.ep_out, data_to_send, timeout)        
        if ret != len(data_to_send):
            raise RuntimeError("Returning value {} from write operation is not "
            "the expected one {}".format(ret, len(data_to_send)))
        logging.debug("Wrote data: {}".format(bytes(data_to_send).hex(":")))
    
    def _read_usb_message(self, length = 64, timeout = 5000):
        """ receive a message from USB endpoint 
        
        Note that communication will hang if the length is not the right one -
        this value depends on the command. """
    
        ret = self.dev.read(self.ep_in, length, timeout)
        logging.debug("Read data: {}".format(bytes(ret).hex(":")))
        return ret

    def QUERY(self):
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
        

        fmt = "<B{}sB".format(len(self.PASSWORD_QUERY))
        data = struct.pack(fmt, self.CMD_ID_QUERY, 
          bytes(self.PASSWORD_QUERY), self.deviceId)
        self._send_usb_message(data)
        buff = self._read_usb_message(64)[:13]
        cmd_id, bytesPerPacket, bytesPerAddress, memoryType,  \
        address1, lenght1, type2 = struct.unpack("<BBBBLLB", buff)
        
        try:
            assert cmd_id == self.CMD_ID_QUERY
            assert bytesPerPacket == self.DATA_ATTACHMENT_LEN
            assert bytesPerAddress == 2
            assert memoryType == 1
            assert type2 == 0xFF
        except:
            RuntimeError("Invalid data in QUERY response")
            
        return (address1, lenght1)

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
        self.QUERY()

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
        except:
            RuntimeError("Invalid data in GET_DATA response")

        return array[58 - bytesPerPacket:] 
        
    def BOOT_FW_VERSION_REQUEST(self) -> NoReturn:       
        data = struct.pack("<BB", self.CMD_ID_BOOT_FW_VERSION_REQUEST,
          self.deviceId)
        self._send_usb_message(data)
        buff = self._read_usb_message(4)
        cmd_id, version_major, version_minor, version_patch = \
          struct.unpack("<BBBB", buff)

        try:
            assert cmd_id == self.CMD_ID_QUERY
        except:
            RuntimeError("Invalid data in BOOT_FW_VERSION_REQUEST response")
            
        return (version_major, version_minor, version_patch)
        
        
class AlfaFirmwareLoader:
    """ Memory management of PIC24 based boards using USB protocol."""
    
    def __init__(self, deviceId = 0xff, serialPort = None):
        try:
            self.usb = USBManager(deviceId)
            # with bootloader 2.0 usb seems to working
            # but it does not respond due to hw misconfiguration,
            # this is why this call is here and repeated above
            address, length = self.usb.QUERY()
        except:
            print ("serialPort=",serialPort)
            if serialPort is not None:
                try:        
                    self.jump_to_boot(serialPort)
                except:
                    raise RuntimeError("failed to jump to boot using serial commands")
            try:
                self.usb = USBManager(deviceId)
                address, length = self.usb.QUERY()
            except:
                raise RuntimeError("failed to init USB device")
            
        # boot version is retrieved by using command BOOT_FW_REQUEST
        # however, executing this command on a bootloader not implementing it
        # results in a deadlock
        # TODO restore when fw is ready
        boot_fw_version = (0,0,0) # self.usb.BOOT_FW_VERSION_REQUEST()
        
        logging.info("Response to QUERY, address = {}, length = {}"
          .format(address, length))
          
        logging.info("Response to BOOT_FW_VERSION_REQUEST, result = {}"
          .format(boot_fw_version))
          
        self.starting_address = address
        self.memory_length = length
        self.boot_fw_version = boot_fw_version        
        self.erased = False

    def jump_to_boot(self, serial_filename):
        baudrate = 115200
        src_addr = 200
        mode = Protocol.ProtocolMode.DUPLEX
                
        async def operations():
            endpoint = SerialDriver()            
            try: # TODO replace with 'with e as endpoint'?
                endpoint.connect(device_name = serial_filename,
                                 device_baudrate = baudrate)
                
                proto = Protocol(mode, endpoint)
                node = proto.attach_node(src_addr)
                
                assert(await node.wait_for_event(
                 node.EventLabel.NODE_STATUS_CHANGED, node.NodeStatus.READY, 5) != None)

                assert(await node.wait_for_recv_status_parameters(
                    {"status_level": 0x06}, 15) == True)
                
                assert((await node.send_request_and_watch(
                  "ENTER_DIAGNOSTIC", status_params = {"status_level": 0x07},
                  timeout_status_sec = 25))[0] == node.RequestWatchResult.SUCCESS)

                (result, answer_params) = await node.send_request_and_watch(
                 "DIAG_JUMP_TO_BOOT", status_params = {"status_level": 0x09},
                 timeout_status_sec = 5)
                assert result == node.RequestWatchResult.SUCCESS
                endpoint.disconnect()
                await asyncio.sleep(5) # wait for OS and drivers to initialize
            finally:
                endpoint.disconnect()
                
        loop = asyncio.get_event_loop()
        loop.run_until_complete(operations())
        
    def erase(self):
        """ erase application memory. """
        
        try:
            self.usb.ERASE()
            self.erased = True
        except:
            raise RuntimeError("failed to perform erase")
                                  
    def program(self, program_data: list) -> NoReturn:
        """ program the application on the proper memory space.
        
        :argument program_data: the entire application as a vector of bytes 
        """
        
        if not self.erased:
            logging.warning("erase procedure not performed")
            
        try:
            program_segment = program_data[self.starting_address * 2:
                           self.starting_address * 2 + self.memory_length * 2]
        except:
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
                               chunk.hex(":")))
                self.usb.PROGRAM(self.starting_address + cursor // 2, chunk)
                cursor += chunk_len               
            except:
                raise RuntimeError("programming failed between program "
                                   "positions {} and {}".format(
                                   cursor, cursor + chunk_len))
                                   
        try:
            self.usb.PROGRAM_COMPLETE()
        except:
            raise RuntimeError("program failed during finalization")
       
    def verify(self, program_data: list) -> bool:
        """ verify the application memory on device against the given one.
        
        :argument program_data: the entire application as a vector of bytes 
        :return: a boolean
        """
             
        try:
            program_segment = program_data[self.starting_address * 2:
                           self.starting_address * 2 + self.memory_length * 2]
        except:
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
                     .format(read_chunk.hex(":"), chunk.hex(":")))
                    return False
                    
                cursor += chunk_len
            except:
                raise RuntimeError("verify failed between program "
                                   "positions {} and {}".format(
                                   cursor, cursor + chunk_len))
        return True

class Application:
    DESCRIPTION = "An utility to program Alfa PIC based boards " \
                  "using USB based bootloader."
    NAME = "alfa_fw_upgrader"
    EXAMPLE_TEXT = '''Actions:
- program: program the application memory with given hex file
- verify: verify the application memory against the given hex file
- info: get memory parameters and boot version

Examples:

To perform program and verify,
 > alfa_fw_upgrader -f master_tinting-boot.hex program verify

To perform verify only, with debug info and reset,
 > alfa_fw_upgrader -vv -f master_tinting-boot.hex verify reset'''

    actions = ('info', 'program', 'verify')

    errors_dict = {
        "FILENAME_REQUIRED": {
            "descr": "Filename is required with selected action(s)",
            "retcode": 1,
        },
        "FILE_LOAD_FAILED": {
            "descr": "Failed to load filename {}",
            "retcode": 2,
            "hints": [
                "Check if the file exists",
                "Check if the file has a valid Intel Hex format"
            ]

        },
        "INIT_FAILED": {
            "descr": "Failed to initialize ({})",
            "retcode": 3,
            "hints": [            
                "Check if the device is powered up and correctly connected",
                "Check if the device is in boot mode",
                "Check if the driver is working correctly. On windows, "
                "run Zadig and select WinUSB driver for this device and be "
                "sure to have library libusb-1.0.dll in c:\Windows\System32",
                "Check if the serial port is correctly plugged in and the"
                "serial port is correct, if using bootloader '2.0'"
            ]
        },
        "ERASE_FAILED": {
            "descr": "Failed to erase ({})",
            "retcode": 4
        },
        "VERIFY_FAILED": {
            "descr": "Failed to verify ({})",
            "retcode": 5
        },
        "VERIFY_DATA_MISMATCH": {
            "descr": "Verify failed due to data mismatch",
            "retcode": 6
        },
    }

    def _exit_error(self, error_key, format_arg = None):
        pr_exc = self.args.verbosity is not None
    
        error_item = self.errors_dict[error_key]
        descr = error_item["descr"] if not format_arg else \
               error_item["descr"].format(format_arg)
        hints = ""
        if "hints" in error_item:
            for h in error_item["hints"]:
                hints += " - " + h + "\n"
                
        text = "The following error occured during execution: "
        text += descr + "\n"
        if not pr_exc:
            text += "Increment level of verbosity to get exception\n"
        text += f"\nHints:\n{hints}\n" if hints else ""
        text += f"Return value is {error_item['retcode']}\n"
        sys.stderr.write(text)
        
        if pr_exc: 
            traceback.print_exc(file=sys.stdout)           
        exit(error_item["retcode"])
    

    def main(self):
        parser = argparse.ArgumentParser(
                          prog = self.NAME,
                          description=self.DESCRIPTION,
                          epilog=self.EXAMPLE_TEXT,
                          formatter_class=argparse.RawDescriptionHelpFormatter)
                           
        parser.add_argument('-f', '--filename', dest='hexfilename', type=str,
                            help='filename of the IntelHex file to load')

        parser.add_argument('-d', '--deviceid', dest='ID', type=int, default=255,
                            help='ID of the device (default=255)')

        parser.add_argument('--serial', dest='serial', action='store_true',
                            help="try to jump to boot using serial commands (default)")
        parser.add_argument('--no-serial', dest='serial', action='store_false',
                            help="do not try to jump to boot using serial commands")
        parser.set_defaults(serial=True)

        parser.add_argument('-s', '--serial-port', dest='serialport', type=str,
                            help='serial port to use (default=/dev/ttyUSB0)',
                            default='/dev/ttyUSB0')
        
        parser.add_argument("-v", "--verbosity", action="count",
                            help="increase output verbosity")

        parser.add_argument("actions", type=str, nargs='+',  
                            choices=self.actions,
                            help="actions to perform")

        if len(sys.argv) < 2:
            parser.print_help()
            sys.exit(1)
            
        self.args = parser.parse_args()

        level = "ERROR"
        if self.args.verbosity is not None:
            if self.args.verbosity >= 2:
                level = "DEBUG"
            elif self.args.verbosity == 1:
                level = "INFO"
            
        logging.basicConfig(
            stream=sys.stdout, level=level,
            format="[%(asctime)s]%(levelname)s %(funcName)s() "
                   "%(filename)s:%(lineno)d %(message)s")

        # put actions in the right order and avoid repetitions
        actions = [x for x in self.actions if x in self.args.actions]

        program_data = []
        if 'program' in actions or 'verify' in actions:
            if self.args.hexfilename is None:
                self._exit_error("FILENAME_REQUIRED")
            fn = self.args.hexfilename
            try:
                program_data = HexUtils.load_hex_file_to_array(fn)
            except:
                self._exit_error("FILE_LOAD_FAILED", fn)

        try:
            print("SERIAL=",  self.args.serial)
            print("SERIAL=",  self.args.serialport)
            ufl = AlfaFirmwareLoader(self.args.ID, 
                   self.args.serialport if self.args.serial else None)
        except Exception as e:
            self._exit_error("INIT_FAILED", str(e))
        
        for a in actions:
            if a == 'info':
                print("Boot version: {}.{}.{}".format(
                 ufl.boot_fw_version[0], ufl.boot_fw_version[1],
                 ufl.boot_fw_version[2]))
                print("Memory start address: {} / length: {}".format(
                 ufl.starting_address, ufl.memory_length))
            elif a == 'program':
                try:
                    ufl.erase()
                except:
                    self._exit_error("ERASE_FAILED")                   
                try:
                    ufl.program(program_data)
                except:
                    self._exit_error("PROGRAM_FAILED")
            elif a == 'verify':
                try:
                    if ufl.verify(program_data) is False:
                        self._exit_error("VERIFY_DATA_MISMATCH")
                except:
                    self._exit_error("VERIFY_FAILED")
                   
if __name__ == '__main__':
    Application().main()


