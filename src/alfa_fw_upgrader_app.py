from alfa_fw_upgrader import AlfaFirmwareLoader, HexUtils

import argparse
import sys
import traceback
import logging 
import os
from threading import Thread

import eel

HERE = os.path.dirname(os.path.abspath(__file__))

class GUIApplication:
    hex_available = None

    def _get_output(self, error_key, format_arg = None):
        error_item = Application.errors_dict[error_key]
        descr = error_item["descr"] if not format_arg else \
               error_item["descr"].format(format_arg)
        hints = ""
        if "hints" in error_item:
            for h in error_item["hints"]:
                hints += "<li>" + h + "</li>"
        text = "The following error occured during execution: <ul>"
        text += descr + "</ul>"
        text += f"Hints:{hints}" if hints else ""
        
        return text
        
    def process(self, setup_dict):
        print(setup_dict)
        
        device_id = int(setup_dict["device_id"])
        use_serial = setup_dict["use_serial"] 
        action = setup_dict["action"]

        if action == "init":        
            try:
                self.ufl = AlfaFirmwareLoader(
                      deviceId = device_id, 
                      serialMode = setup_dict["use_serial"],
                      pollingMode = False, # not implemented
                      serialPort = setup_dict["serial_port"])
                                  
                eel.stop_process_js({"success": True, "output": ""})                       
                eel.is_board_initialized_js(True)
            except Exception as e:
                eel.stop_process_js({
                 "success": False,
                 "output": self._get_output("INIT_FAILED", str(e))})
                eel.is_board_initialized_js(False)
        elif action == "info":
            eel.stop_process_js({
             "success": True,
             "output": "Memory start address: {} / length: {}"
              .format(self.ufl.starting_address, self.ufl.memory_length)
            })
        elif action == 'program':
            try:
                self.ufl.erase()
            except:
                eel.stop_process_js({
                 "success": False,
                 "output":  self._get_output("ERASE_FAILED")})
            try:
                self.ufl.program(self.program_data)
                eel.stop_process_js({"success": True, "output": ""})
            except:
                eel.stop_process_js({
                 "success": False,
                 "output":  self._get_output("PROGRAM_FAILED")})
        elif action == 'verify':
            try:
                if self.ufl.verify(self.program_data) is False:
                    eel.stop_process_js({
                     "success": False,
                     "output":  self._get_output("VERIFY_DATA_MISMATCH")})
                else:
                    eel.stop_process_js({"success": True, "output": ""})
            except Exception as e:
                eel.stop_process_js({
                 "success": False,
                 "output":  self._get_output("VERIFY_FAILED", str(e))})
        elif action == 'program_and_verify':
            try:
                self.ufl.erase()
            except:
                eel.stop_process_js({
                 "success": False,
                 "output":  self._get_output("ERASE_FAILED")})
            try:
                self.ufl.program(self.program_data)
                eel.stop_process_js({"success": True, "output": ""})
            except:
                eel.stop_process_js({
                 "success": False,
                 "output":  self._get_output("PROGRAM_FAILED")})
            try:
                if self.ufl.verify(self.program_data) is False:
                    eel.stop_process_js({
                     "success": False,
                     "output":  self._get_output("VERIFY_DATA_MISMATCH")})
                else:
                    eel.stop_process_js({"success": True, "output": ""})
            except Exception as e:
                eel.stop_process_js({
                 "success": False,
                 "output":  self._get_output("VERIFY_FAILED", str(e))})
                 
    def __init__(self):
        self.hex_available = False
    
        print("** No arguments given. -h for usage details of command line "
              "interface. Starting GUI **")
        path = os.path.join(HERE, "../", "templates", "gui-frontend")
        eel.init(path, allowed_extensions=['.js', '.html'])

        @eel.expose  # Expose this function to Javascript
        def say_hello_py(x):
            print('Hello from %s' % x)

        @eel.expose 
        def process_file(file_content):
            eel.start_process_js()
            try:
                self.program_data = HexUtils.load_hex_to_array(file_content)
                eel.stop_process_js({
                 "success": True,
                 "output":  ""})
                self.hex_available = True                
            except Exception as e:
                eel.stop_process_js({
                 "success": False,
                 "output":  self._get_output("VERIFY_FAILED", str(e))})            
                self.hex_available = False
                
            eel.is_hex_available_js(self.hex_available)
            
        eel.say_hello_js('Python World!') # Call a Javascript function

        @eel.expose
        def process(setup_dict):
            eel.start_process_js()
            eel.spawn(self.process, setup_dict)
            
    def run(self):
        eel.start('main.html', size=(700, 500))
    

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

        parser.add_argument('-s', '--strategy', dest='strategy',
                           choices=['simple', 'polling', 'serial'],
                           help="strategy for getting device ready for upgrade."
                                "- simple: try to connect USB and exit if "
                                "device not ready (default choice) "
                                "- polling: watch for USB device to became "
                                "available and avoid jumping to application "
                                "- serial: try to jump to boot using serial "
                                "commands")

        parser.add_argument('-p', '--serial-port', dest='serialport', type=str,
                            help="when strategy is 'serial', the serial port to "
                            "use (default=/dev/ttyUSB0)",
                            default='/dev/ttyUSB0')

        parser.add_argument('-t', '--polling-time', dest='pollingtime', type=str,
                            help="when strategy is 'polling', the polling time "
                            "in seconds (default=10)",
                            default=10)

        parser.add_argument("-v", "--verbosity", action="count",
                            help="increase output verbosity")

        parser.add_argument("actions", type=str, nargs='+',  
                            choices=self.actions,
                            help="actions to perform")

        if len(sys.argv) < 2:    
            logging.basicConfig(
                stream=sys.stdout, level="INFO",
                format="[%(asctime)s]%(levelname)s %(funcName)s() "
                       "%(filename)s:%(lineno)d %(message)s")
            gui = GUIApplication()
            gui.run()
        else:
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
                    with open(fn, 'r') as f:
                        file_content = f.read()
                        program_data = HexUtils.load_hex_to_array(file_content)
                except:
                    self._exit_error("FILE_LOAD_FAILED", fn)

            try:
                ufl = AlfaFirmwareLoader(
                      deviceId = self.args.ID, 
                      serialMode = self.args.strategy == "serial",
                      pollingMode = self.args.strategy == "polling",
                      serialPort = self.args.serialport,
                      pollingInterval = self.args.pollingtime)
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

