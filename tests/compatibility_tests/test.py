import unittest
import logging
import test_config as cfg
import os
import subprocess
import re
import json 

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

    def program(self, hexfile, device_id):
        actions = ['program', 'verify', 'jump']
        cmd = cfg.alfacmd.format(deviceId = device_id, 
                                 actions = " ".join(actions),
                                 hexfile_arg = f"-f {hexfile}")
        print(cmd)
        ret = subprocess.run(cmd.split(' '))
        print("The exit code was: %d" % ret.returncode)
        return ret.returncode == 0    
        
    def get_versions(self):
        cmd = cfg.alfacmd.format(deviceId = "255", 
                                 actions = "info",
                                 hexfile_arg = "")

        cmd = ' '.join(cmd.split()) # remove double spaces
        logging.info(f"Executing command: {cmd}")

        ret = subprocess.run(cmd.split(" "), capture_output = True)
        print("The exit code was: %d" % ret.returncode)
        
        if ret.returncode != 0:
            logging.info(f"Failed to retrieve version, retcode={ret.returncode}, "
                    f"output={ret.stdout} stderr={ret.stderr}")
            return None
        
        logging.debug("Stdout was:", ret.stdout.decode("utf-8"))

        m = re.search('FW versions JSON\:(.*)\n', ret.stdout.decode("utf-8"))
        if m is None:
            return None
        json_str = m.group(1)
        logging.debug("JSON string is:", json_str)
        try:
            return json.loads(json_str)
        except:
            logging.warning("Failed to decode versions")
            return None
        
    def test_file_driver(self):
        logging.basicConfig(level=logging.INFO)
        # currently execute only 1st test
        # TODO execute all them or give user the possibility to choose
        test = cfg.tests[0] 

        fn_mmt_prog = cfg.mmt_file.format(
                       version = cfg.mmt_variants["version"][test["mmt_variants"]["version"]], 
                       boot = cfg.mmt_variants["boot"][test["mmt_variants"]["boot"]],
                       slaves = cfg.mmt_variants["slaves"][test["mmt_variants"]["slaves"]])

        fn_mmt_boot = cfg.mmt_boot_file.format(
                       version = cfg.mmt_boot_variants["version"][test["mmt_boot_variants"]["version"]])

        fn_sccb_prog = cfg.sccb_file.format(
                       version = cfg.sccb_variants["version"][test["sccb_variants"]["version"]], 
                       boot = cfg.sccb_variants["boot"][test["sccb_variants"]["boot"]])

        fn_sccb_boot = cfg.sccb_boot_file.format(
                       version = cfg.sccb_boot_variants["version"][test["sccb_boot_variants"]["version"]])

        print("Filename program MMT: ", fn_mmt_prog)
        print("Filename boot MMT: ", fn_mmt_boot)
        print("Filename program SCCB: ", fn_sccb_prog)
        print("Filename boot SCCB: ", fn_sccb_boot)

        input(f"1. Power off.\n" \
              f"2. Connect {cfg.mmt_pickit_serial} to MMT.\n" \
              f"3. Connect {cfg.sccb_pickit_serial} to SCCB.\n" \
              f"4. Power on.\n" \
              "Press Enter to continue")
        
       
        assert True == self.program_with_jtag(os.path.join(HERE, "hex", fn_mmt_boot), cfg.mmt_pickit_serial, cfg.mmt_micro)
        assert True == self.program_with_jtag(os.path.join(HERE, "hex", fn_sccb_boot), cfg.sccb_pickit_serial, cfg.sccb_micro)
        
        input("1. Power off.\n" \
              "2. Disconnect programmers.\n" \
              "3. Power on.\n" \
              "4. Wait 5 seconds.\n" \
              "Press Enter to continue")
        
        assert True == self.program(os.path.join(HERE, "hex", fn_mmt_prog), 255)
        assert True == self.program(os.path.join(HERE, "hex", fn_sccb_prog), 8)

        vers = self.get_versions()
        assert vers is not None
        logging.info(f"Retrived versions: {vers}")
        print(tuple(vers["slaves"][7]))
        assert tuple(vers["slaves"][7]) == test["sccb_8_result_version"]
        assert tuple(vers["master"]) == test["mmt_result_version"]        


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    unittest.main()
    


