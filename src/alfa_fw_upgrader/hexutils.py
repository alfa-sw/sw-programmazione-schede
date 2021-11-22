""" 
Utilities to handle Intel Hex format. 

Keep in mind that Hex is a dictionary [address] -> data. In order to
get a "binary" (array of bytes representing the whole program) we need to
fill missing addresses.

"""
import logging

class HexUtils:
    def load_mplab_table(filename: str) -> list:
        """ load binary array from the table exported from MPLAB IPE to file.
        The table should be exported in this way:
        1. setup and connect
        2. click "read" button
        3. Window -> Target Memory Views -> Program view (note that
        you need to tinker with settings in order to see this menu item)
        4. on the table, right click -> Output to file
        """
        
        str2hex = lambda s: int(s, 16)
        pData = {}
        
        with open(filename,'r') as fileHex:
            lines = fileHex.readlines()[1:]
            for line in lines:
                if len(line) <= 0:
                    continue  
                addressField = str2hex(line[0:6]) * 2
                values_str = list(filter(None, line[14:50].split(' ')))
                j = addressField
                for v in values_str:                
                    pData[j + 3] = 0
                    pData[j + 0] = str2hex(v[4:6])
                    pData[j + 1] = str2hex(v[2:4])
                    pData[j + 2] = str2hex(v[0:2])
                    j = j + 4
        return pData

    def dict_to_array(src: dict, size = None) -> list:
        """ get the array of bytes from a dictionary address """
        
        if size is None:
            size = max(src.keys())

        # erased memory has all bytes to 0xFF, except for
        # "phantom" bytes, always set to 0
        out = [0 if (x + 1) % 4 == 0 else 0xFF for x in range(0, size)]
        
        for addr, x in src.items():
            try:
                out[addr] = x
            except:
                logging.debug("addr:{} is out of memory".format(addr))
        return out
    
    def load_hex_to_dict(filecontent):
        """ get the dictionary from an hex file. """
            
        HEX_FILE_EXTENDED_LINEAR_ADDRESS = 0x04
        HEX_FILE_EOF = 0x01
        HEX_FILE_DATA = 0x00

        str2hex = lambda s: int(s, 16)
        extendedAddress = None
        pData = {}
        
        lines = filecontent.split('\n')
        for line in lines:
            line = line.strip()
            if len(line) <= 0:
                continue  
            if line[0]!=':':
                raise ValueError("invalid format")
            line = line.strip(':')
            
            recordLength = str2hex(line[0:2])
            addressField= str2hex(line[2:6])
            recordType= str2hex(line[6:8])
            dataPayload = line[8 : 8 + recordLength * 2]
            checksum = str2hex(line[8 + recordLength * 2:
                                   10 + recordLength * 2])

            checksumCalculated=0
            for j in range(0,recordLength+4):
                checksumCalculated += str2hex(line[j*2 : (j * 2) + 2])       
            checksumCalculated = ~checksumCalculated
            checksumCalculated += 1
            checksumCalculated = checksumCalculated & 0x000000FF
    
            if (checksumCalculated & 0x000000FF) != checksum:
                raise ValueError("invalid checksum (calculated:{}, read:{}"
                                 .format(checksumCalculated, checksum))
                                 
            if recordType == HEX_FILE_EXTENDED_LINEAR_ADDRESS:
                extendedAddress = str2hex(dataPayload)
            elif recordType == HEX_FILE_EOF:
                break
            elif recordType == HEX_FILE_DATA:
                totalAddress = (extendedAddress << 16) + addressField 
                for i in range(0, recordLength):
                    datum = str2hex(dataPayload[i*2:i*2+2])
                    pData[totalAddress + i] = datum                          
        return pData
    
    def load_hex_to_array(file_content):
        """ get binary from an hex file. """
        dic = HexUtils.load_hex_to_dict(file_content)
        return HexUtils.dict_to_array(dic)


