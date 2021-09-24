#!/usr/bin/env python

from hexutils import HexUtils
from alfa_fw_upgrader import USBFirmwareLoader
import unittest
import logging
import os

class TestFileDriver(unittest.TestCase):
    def test_file_driver(self):
        logging.basicConfig(level=logging.INFO)

        ufl = USBFirmwareLoader()
        ufl.erase()

        here = os.path.dirname(os.path.abspath(__file__))
        fn = os.path.join(here, "Master_Tinting-boot-nodipswitch.hex")
        program_data = HexUtils.load_hex_file_to_array(fn)
        ufl.program(program_data)
        assert ufl.verify(program_data)
           
if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    unittest.main()
    
