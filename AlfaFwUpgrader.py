import usb.core
import usb.util
import struct
import logging

from hexutils import HexUtils

class USBManager:
    PASSWORD_QUERY = [0x82, 0x14, 0x2A, 0x5D, 0x6F, 0x9A, 0x25, 0x01]
    USB_ID_VENDOR = 0x04d8
    USB_ID_PRODUCT = 0xe89b
    MSG_LEN = 64
    DATA_ATTACHMENT_LEN = 56

    CMD_ID_QUERY = 2
    CMD_ID_ERASE = 4
    CMD_ID_PROGRAM = 5
    CMD_ID_PROGRAM_COMPLETE = 6
    CMD_ID_VERIFY = 7
    
    def __init__(self, deviceId = 0xff):
        self._usb_init()
        self.deviceId = deviceId
        
    def _usb_init(self):
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
        #msg_len = self.MSG_LEN
        #if len(data) > msg_len:
        #    msg_len = len(data)
#            raise ValueError("too much bytes on the USB message to send")
        
        #zeros = [0] * (msg_len - len(data)) 
        data_to_send = list(data) #+ zeros
        #print(data_to_send)
        ret = self.dev.write(self.ep_out, data_to_send, timeout)        
        if ret != len(data_to_send):
            raise RuntimeError("Returning value {} from write operation is not "
            "the expected one {}".format(ret, len(data_to_send)))
        logging.debug("Wrote data: {}".format(bytes(data_to_send).hex(":")))
    
    def _read_usb_message(self, length = 64, timeout = 5000):
        ret = self.dev.read(self.ep_in, length, timeout)
        logging.debug("Read data: {}".format(bytes(ret).hex(":")))
        return ret

    def QUERY(self):
        # ‘Command’: unsigned char - QUERY_DEVICE
        # ‘PacketDataFieldSize’: unsigned char - numero di bytes nel campo ‘Data’. Valore: ‘0x38’ (=56)
        # ‘BytesPerAddress’: unsigned char - numero di bytes per indirizzo . Valore: ‘0x02’
        # ‘Type1’: unsigned char - tipo di memoria. Valore: ‘0x01’
 	    # ‘Address1’: unsigned long - indirizzo iniziale di memoria FLASH.
        # ‘Lenght1’: unsigned long - dimensione della memoria FLASH.
        # ‘Type2’: unsigned char - possibili valori:
        #    ‘0xFF’: fine della lista dei parametri di memoria
        
        fmt = "<B{}sB".format(len(self.PASSWORD_QUERY))
        data = struct.pack(fmt, self.CMD_ID_QUERY, 
          bytes(self.PASSWORD_QUERY), 0xFF)
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

    def ERASE(self):
        data = struct.pack("<B", self.CMD_ID_ERASE)
        self._send_usb_message(data)
        
        # erase command does not produce any answer - send QUERY and wait
        # for the answer from this command
        self.QUERY()

    def PROGRAM(self, address, chunk):
        if len(chunk) > self.DATA_ATTACHMENT_LEN:
            raise ValueError("too much bytes on the PROGRAM message")

        if len(chunk) < 58:
            # align to right
            array = [0] * (58 - len(chunk)) + list(chunk)

        data = struct.pack("<BLB58s", self.CMD_ID_PROGRAM, address, len(chunk),
                           bytes(array))
        self._send_usb_message(data)

    def PROGRAM_COMPLETE(self):
        # note that in some docs report some parameters but it is better
        # to use it just as a signal of program complete to bootloader
        
        data = struct.pack("<B", self.CMD_ID_PROGRAM_COMPLETE)
        self._send_usb_message(data)
        
    def GET_DATA(self, address, length):
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
        
class USBFirmwareLoader:
    def __init__(self):
        try:
            self.usb = USBManager()
        except:
            raise RuntimeError("failed to init USB device")
            
        address, length = self.usb.QUERY()
        logging.info("Response to QUERY, address = {}, length = {}"
          .format(address, length))
        self.starting_address = address
        self.memory_length = length
        self.erased = False
        
    def erase(self):
        try:
            self.usb.ERASE()
            self.erased = True
        except:
            raise RuntimeError("failed to perform erase")
                                  
    def program(self, program_data):
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
       
    def verify(self, program_data):
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
                   
if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    
    ufl = USBFirmwareLoader()
    ufl.erase()
    
    fn = "Master_Tinting-boot-nodipswitch.hex"
    program_data = HexUtils.load_hex_file_to_array(fn)
    ufl.program(program_data)
    
    assert(ufl.verify(program_data) == True)


