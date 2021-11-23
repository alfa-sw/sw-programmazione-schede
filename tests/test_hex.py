#!/usr/bin/env python

from alfa_fw_upgrader.hexutils import HexUtils
from alfa_fw_upgrader.lib import AlfaFirmwareLoader
import unittest
import logging
import os

class TestFileDriver(unittest.TestCase):
    def test_hex(self):
        # IntelHex library is used to check correctness of the function
        # load_hex_file_to_dict()
        from intelhex import IntelHex
        
        import pprint
        here = os.path.dirname(os.path.abspath(__file__))

        f  = 'Master_Tinting-boot-nodipswitch.hex'

        fn = os.path.join(here, f)

        print("loading mplab table...")
        dict1 = HexUtils.load_mplab_table(os.path.join(here,
               'TABLE_from_MPLAB_IPE.txt'))

        with open(fn, 'r') as f:
            file_content = f.read()
        
            print("loading hex with IntelHex lib...")
            ih = IntelHex()
            ih.loadhex(fn)
            dict2 = ih.todict() 

            print("loading hex with my funct...")
            dict3 = HexUtils.load_hex_to_dict(file_content)

            print("converting arrays...")
            array1 = HexUtils.dict_to_array(dict1, len(dict1))
            array2 = HexUtils.dict_to_array(dict2, len(dict1))
            array3 = HexUtils.dict_to_array(dict3, len(dict1))

            assert array1 == array2
            assert array2 == array3
           
if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    unittest.main()
    
   
