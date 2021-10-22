import unittest
import logging
import test_config as cfg
import os
import subprocess
import re
import json 
import time

HERE = os.path.dirname(os.path.abspath(__file__))

class TestProgram(unittest.TestCase):

    def program_with_jtag(self, hexfile, pickit_serial, micro):
        cmd = cfg.ipecmd.format(pickit_serial = pickit_serial, 
                                micro = micro,
                                hexfile = hexfile)
        print(cmd)
        ret = subprocess.run(cmd.split(" "))
        print("The exit code was: %d" % ret.returncode)
        return ret.returncode == 0

    def program(self, hexfile, device_id, jump_to_application = False):
        actions = ['program', 'verify']
        if jump_to_application:
            actions.append('jump')
            
        cmd = cfg.alfacmd.format(deviceId = device_id, 
                                 actions = " ".join(actions),
                                 hexfile_arg = f"-f {hexfile}")
        print(cmd)
        ret = subprocess.run(cmd.split(' '))
        print("The exit code was: %d" % ret.returncode)
        return ret.returncode == 0    
        
    def get_versions(self, jump_to_application = False):
        actions = "info" + (" jump" if jump_to_application else "")
        cmd = cfg.alfacmd.format(deviceId = "255", 
                                 actions = actions,
                                 hexfile_arg = "")

        cmd = ' '.join(cmd.split()) # remove double spaces
        logging.info(f"Executing command: {cmd}")

        ret = subprocess.run(cmd.split(" "), capture_output = True)
        print("The exit code was: %d" % ret.returncode)
        
        if ret.returncode != 0:
            logging.info(f"Failed to retrieve version, retcode={ret.returncode}, "
                    f"output={ret.stdout} stderr={ret.stderr}")
            return None
        
        logging.debug("Stdout was:{}".format(ret.stdout.decode("utf-8")))

        try:
            m = re.search('FW versions JSON\:(.*)\n', ret.stdout.decode("utf-8"))
            if m is None:
                return None
            json_str = m.group(1)
            print (json_str)
            logging.debug(f"JSON string is: {json_str}")
            return json.loads(json_str)
        except:
            logging.info("Failed to decode version")
            return None
            
    def get_hex_filename(self, test_idx = 0):
        # currently execute only 1st test
        # TODO execute all them or give user the possibility to choose
        test = cfg.tests[test_idx] 
 
        if test["mmt_variants"]["boot"] == "yes":
            fn_mmt_prog = cfg.mmt_file.format(
                           version = cfg.mmt_variants["version"][test["mmt_variants"]["version"]], 
                           boot = cfg.mmt_variants["boot"][test["mmt_variants"]["boot"]])

            fn_mmt_boot = cfg.mmt_boot_file.format(
                           version = cfg.mmt_boot_variants["version"][test["mmt_boot_variants"]["version"]])
        else:
            fn_mmt_prog = None
            fn_mmt_boot = cfg.mmt_file.format(
                           version = cfg.mmt_variants["version"][test["mmt_variants"]["version"]], 
                           boot = cfg.mmt_variants["boot"][test["mmt_variants"]["boot"]])
        
        if test["sccb_8_variants"]["boot"] == "yes":
            fn_sccb_8_prog = cfg.sccb_file.format(
                           version = cfg.sccb_variants["version"][test["sccb_8_variants"]["version"]], 
                           boot = cfg.sccb_variants["boot"][test["sccb_8_variants"]["boot"]])

            fn_sccb_8_boot = cfg.sccb_boot_file.format(
                           version = cfg.sccb_boot_variants["version"][test["sccb_8_boot_variants"]["version"]])
        else:
            fn_sccb_8_prog = None
            fn_sccb_8_boot = cfg.sccb_file.format(
                           version = cfg.sccb_variants["version"][test["sccb_8_variants"]["version"]], 
                           boot = cfg.sccb_variants["boot"][test["sccb_8_variants"]["boot"]])
        
        fn_sccb_1_prog = None
        fn_sccb_1_boot = None
        
        if "sccb_1_variants" in test:
            if test["sccb_1_variants"]["boot"] == "yes":        
                fn_sccb_1_prog = cfg.sccb_file.format(
                               version = cfg.sccb_variants["version"][test["sccb_1_variants"]["version"]], 
                               boot = cfg.sccb_variants["boot"][test["sccb_1_variants"]["boot"]])

                fn_sccb_1_boot = cfg.sccb_boot_file.format(
                               version = cfg.sccb_boot_variants["version"][test["sccb_1_boot_variants"]["version"]])
            else:
                fn_sccb_1_prog = None
                fn_sccb_1_boot = cfg.sccb_file.format(
                               version = cfg.sccb_variants["version"][test["sccb_1_variants"]["version"]], 
                               boot = cfg.sccb_variants["boot"][test["sccb_1_variants"]["boot"]])
                               
        print("Filename program MMT: ", fn_mmt_prog)
        print("Filename boot MMT: ", fn_mmt_boot)
        print("Filename program SCCB 8: ", fn_sccb_8_prog)
        print("Filename boot SCCB 8: ", fn_sccb_8_boot)
        print("Filename program SCCB 1: ", fn_sccb_1_prog)
        print("Filename boot SCCB 1: ", fn_sccb_1_boot)
        
        return {
            "fn_mmt_prog": fn_mmt_prog,
            "fn_mmt_boot": fn_mmt_boot,
            "fn_sccb_8_prog": fn_sccb_8_prog,
            "fn_sccb_8_boot": fn_sccb_8_boot,
            "fn_sccb_1_prog": fn_sccb_1_prog,
            "fn_sccb_1_boot": fn_sccb_1_boot            
        }

    def complete_jtag_program(self, test_idx, consider_slave_1 = False):
        fns = self.get_hex_filename(test_idx)
 
        input(f"1. Power off.\n" \
              f"2. Connect {cfg.mmt_pickit_serial} to MMT.\n" \
              f"3. Connect {cfg.sccb_pickit_serial} to SCCB, slave 8.\n" \
              f"4. Power on.\n" \
              "Press Enter to continue")
                
        assert True == self.program_with_jtag(
                         os.path.join(HERE, "hex", fns["fn_mmt_boot"]),
                         cfg.mmt_pickit_serial, cfg.mmt_micro)
                         
        assert True == self.program_with_jtag(
                         os.path.join(HERE, "hex", fns["fn_sccb_8_boot"]),
                         cfg.sccb_pickit_serial, cfg.sccb_micro)

        if consider_slave_1:
            input(f"1. Connect {cfg.sccb_pickit_serial} to SCCB, slave 1.\n" \
                  "Press Enter to continue")
            assert True == self.program_with_jtag(
                             os.path.join(HERE, "hex", fns["fn_sccb_1_boot"]),
                             cfg.sccb_pickit_serial, cfg.sccb_micro)           

        input("1. Power off.\n" \
              "2. Disconnect programmers.\n" \
              "3. Power on.\n" \
              "4. Wait 5 seconds.\n" \
              "Press Enter to continue")

    def complete_program_and_check(self, test_idx):
        test = cfg.tests[test_idx] 
        fns = self.get_hex_filename(test_idx)
        consider_slave_1 = test["consider_sccb_1"]
        
        self.complete_jtag_program(test_idx, consider_slave_1 = consider_slave_1)
        
        # load mmt program, then jump to boot
        if fns["fn_mmt_prog"] is not None:
            ret_mmt = self.program(os.path.join(HERE, "hex", fns["fn_mmt_prog"]), 255, True)
        
        input(f"1. Power off.\n" \
              "3. Power on.\n" \
              "4. Wait 5 seconds.\n" \
              "Press Enter to continue")
              
        # we check for info, to make sure that program is started
        # get_version return None if versions are not retrieved,
        # i.e. application was not running        
        vers = self.get_versions(True)
        
        if test["mmt_result_version"] is not None:
            assert vers is not None
            logging.info(f"Retrived versions: {vers}")
            print(vers)
            
            if fns["fn_sccb_8_prog"] is not None: 
                ret_sccb = self.program(os.path.join(HERE, "hex", fns["fn_sccb_8_prog"]), 8, True)     
            
            # check for return of program of MMT, if test requires it           
            assert test["mmt_result_version"] is None or fns["fn_mmt_prog"] or ret_mmt == True
            
            # check for return of program of SCCB/8, if test requires it
            assert test["sccb_8_result_version"] is None or fns["fn_sccb_8_prog"] or ret_sccb == True
            
            if consider_slave_1:
                if fns["fn_sccb_1_prog"] is not None: 
                    ret_sccb_2 = self.program(os.path.join(HERE, "hex", fns["fn_sccb_1_prog"]), 1, True)
                    assert test["sccb_1_result_version"] is None or ret_sccb_2 == True
                
            # check for info, again, for final version checking
            vers = self.get_versions(True)  
            
            if test["mmt_result_version"] is None:              
                assert tuple(vers["master"]) == (0, 0, 0)
            else:
                assert tuple(vers["master"]) == test["mmt_result_version"]
            
            if test["sccb_8_result_version"] is None: 
                assert tuple(vers["slaves"][7]) ==  (0, 0, 0)           
            else:
                assert tuple(vers["slaves"][7]) == test["sccb_8_result_version"]    
                   
            if consider_slave_1:           
                if test["sccb_1_result_version"] is None: 
                    assert tuple(vers["slaves"][0]) ==  (0, 0, 0)           
                else:
                    assert tuple(vers["slaves"][0]) == test["sccb_1_result_version"]    


        else:
            assert vers is None
            


    """ *** TESTS *** """
    
    @unittest.SkipTest
    def test_1_repeat_jumps(self):
        print("test_1_repeat_jumps")
        self.complete_program_and_check(0)
    
        i = 0
        while i < 10:
            i += 1
            print(f"Testing iteration {i}")
            vers = self.get_versions(jump_to_application = True)
            assert vers is not None
            logging.info(f"Retrived versions: {vers}")
            time.sleep(2)

    @unittest.SkipTest
    def test_scenario_1(self):
        print("test_scenario_1")
        self.complete_program_and_check(0)

    @unittest.SkipTest
    def test_scenario_2(self):
        print("test_scenario_2")    
        self.complete_program_and_check(1)

    @unittest.SkipTest
    def test_scenario_3(self):
        print("test_scenario_3")    
        self.complete_program_and_check(2)

    @unittest.SkipTest
    def test_scenario_4(self):
        print("test_scenario_4")    
        self.complete_program_and_check(3)

    @unittest.SkipTest
    def test_scenario_5(self):
        print("test_scenario_5")    
        self.complete_program_and_check(4)

    @unittest.SkipTest
    def test_scenario_5_bis(self):
        print("test_scenario_5bis")    
        self.complete_program_and_check(5)

    @unittest.SkipTest
    def test_scenario_6(self):
        print("test_scenario_6")    
        self.complete_program_and_check(6)

    @unittest.SkipTest
    def test_scenario_7(self):
        print("test_scenario_7")    
        self.complete_program_and_check(7)

    @unittest.SkipTest
    def test_scenario_8(self):
        print("test_scenario_8")    
        self.complete_program_and_check(8)

    @unittest.SkipTest
    def test_scenario_9(self):
        print("test_scenario_9")    
        self.complete_program_and_check(9)

    def test_scenario_10(self):
        print("test_scenario_10")    
        self.complete_program_and_check(10)
        
if __name__ == '__main__':
    logging.basicConfig(level=logging.DEBUG)
    unittest.main()
    


