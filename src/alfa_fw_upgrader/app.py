from alfa_fw_upgrader.lib import AlfaFirmwareLoader, AlfaPackageLoader
from alfa_fw_upgrader.hexutils import HexUtils

from alfa_fw_upgrader.data import templates

import argparse
import sys
import traceback
import logging

from io import StringIO

import os
import json
import threading
from pathlib import Path

import yaml

import eel
from appdirs import AppDirs

if sys.version_info >= (3, 9):
    # we use importlib.resource version 3.9 API
    import importlib.resources as importlib_resources
else:
    import importlib_resources

if sys.version_info >= (3, 8):
    import importlib.metadata as importlib_metadata
else:
    import importlib_metadata

if __package__ is None:
    # workaround for pyinstaller setting __package__ to None
    __package__ = "alfa_fw_upgrader"


HERE = os.path.dirname(os.path.abspath(__file__))
USERDIR = AppDirs("alfa_fw_upgrader", "alfa").user_data_dir


class GUIApplication:
    hex_available = None

    default_settings = {
        "serial_port": "/dev/ttyUSB0",
        "strategy": "simple",
        "expert": False
    }

    def _get_output(self, error_key, format_arg=None):
        error_item = Application.errors_dict[error_key]
        descr = error_item["descr"] if not format_arg else \
            error_item["descr"].format(format_arg)
        hints = ""
        if "hints" in error_item:
            for h in error_item["hints"]:
                hints += "<li>" + h + "</li>"
        text = "The following error occured during execution: <b>"
        text += descr + "</b>"
        text += f"<br>Hints:<ol>{hints}</ol>" if hints else ""

        return text

    def process_manual(self, setup_dict):
        # print(setup_dict)

        action = setup_dict["action"]
        # print(setup_dict)
        if action == "connect":
            try:
                device_id = int(setup_dict["device_id"])
                self.ufl = AlfaFirmwareLoader(
                    deviceId=device_id,
                    serialMode=self.settings["strategy"] == "serial",
                    pollingMode=self.settings["strategy"] == "polling",
                    serialPort=self.settings["serial_port"])
                eel.update_process_js({"result": "ok", "output": ""})

            except Exception as e:
                eel.update_process_js({
                    "result": "fail",
                    "output": self._get_output("INIT_FAILED", str(e))})
        elif action == "disconnect":
            try:
                self.ufl.disconnect()
                del self.ufl
                eel.update_process_js({
                    "result": "ok",
                    "output": ""})
            except AttributeError as e:
                eel.update_process_js({
                    "result": "fail",
                    "output": self._get_output("INIT_FAILED", str(e))})

        elif action == "info":
            eel.update_process_js({
                "result": "ok",
                "output": "Memory start address: {} / length: {}\n"
                "Boot version: {} (proto={})\n"
                "Boot versions: {}\n"
                "FW versions: {}\n"
                "Slaves enabled addresses: {}\n"
                "Boot status: {}\n"
                "Application digest: {}\n"
                .format(self.ufl.starting_address,
                        self.ufl.memory_length,
                        self.ufl.boot_fw_version if self.ufl.boot_fw_version
                        is not None else "N/A",
                        self.ufl.proto_ver if self.ufl.proto_ver
                        is not None else "N/A",
                        self.ufl.boot_versions if self.ufl.boot_versions
                        is not None else "N/A",
                        self.ufl.fw_versions if self.ufl.fw_versions
                        is not None else "N/A",
                        self.ufl.slaves_configuration if self.ufl.slaves_configuration
                        is not None else "N/A",
                        self.ufl.boot_status if self.ufl.boot_status
                        is not None else "N/A",
                        self.ufl.digest if self.ufl.digest
                        is not None else "N/A",)
            })

        elif action == 'verify':
            try:
                if not self.ufl.verify(self.program_data):
                    eel.update_process_js({
                        "result": "fail",
                        "output": self._get_output("VERIFY_DATA_MISMATCH")})
                else:
                    eel.update_process_js({"result": "ok", "output": ""})
            except Exception as e:
                eel.update_process_js({
                    "result": "fail",
                    "output": self._get_output("VERIFY_FAILED", str(e))})

        elif action == 'program':
            try:
                self.ufl.erase()
            except BaseException:
                eel.update_process_js({
                    "result": "fail",
                    "output": self._get_output("ERASE_FAILED")})
                return

            try:
                self.ufl.program(self.program_data)
            except BaseException:
                eel.update_process_js({
                    "result": "fail",
                    "output": self._get_output("PROGRAM_FAILED")})
                return

            try:
                if not self.ufl.verify(self.program_data, check_digest=False):
                    eel.update_process_js({
                        "result": "fail",
                        "output": self._get_output("VERIFY_DATA_MISMATCH")})
                    return
            except Exception as e:
                eel.update_process_js({
                    "result": "fail",
                    "output": self._get_output("VERIFY_FAILED", str(e))})
                return

            try:
                self.ufl.seal(self.program_data)
                eel.update_process_js({"result": "ok", "output": ""})
            except BaseException:
                eel.update_process_js({
                    "result": "fail",
                    "output": self._get_output("DIGEST_FAILED")})
                return

        elif action == 'jump':
            try:
                self.ufl.jump()
                eel.update_process_js({"result": "ok", "output": ""})
            except BaseException:
                eel.update_process_js({
                    "result": "fail",
                    "output": self._get_output("COMMAND_FAILED")})
        elif action == 'reset':
            try:
                self.ufl.reset()
                eel.update_process_js({"result": "ok", "output": ""})
            except BaseException:
                eel.update_process_js({
                    "result": "fail",
                    "output": self._get_output("COMMAND_FAILED")})
        else:
            eel.update_process_js({
                "result": "fail",
                "output": "invalid action"})

    def get_settings(self):
        try:
            with open(os.path.join(USERDIR, 'settings.yaml'), 'r') as f:
                self.settings = yaml.load(f.read(), Loader=yaml.SafeLoader)
            self.settings_from_default = False
        except BaseException:
            logging.info("failed to load settings, setting defaults")
            self.settings = self.default_settings.copy()
            self.settings_from_default = True

    def save_settings(self):
        fn = os.path.join(USERDIR, 'settings.yaml')
        Path(USERDIR).mkdir(parents=True, exist_ok=True)

        with open(fn, 'w+') as f:
            f.write(yaml.dump(self.settings))

    def _set_logging_stream(self, reset_to_stderr=False):
        self.log_stream = sys.stderr if reset_to_stderr else StringIO()
        logging.debug("setting logging stream %s", self.log_stream)
        self.logging_handler.setStream(self.log_stream)

    def __init__(self):
        self.hex_available = False

        print("** No arguments given. -h for usage details of command line "
              "interface. Starting GUI **")

        self.get_settings()
        eel.init(importlib_resources.files(templates),
                 allowed_extensions=['.js', '.html', '.ico'])

        self.worker = None
        self.stop_request = False

        @eel.expose  # Expose this function to Javascript
        def say_hello_py():
            return importlib_metadata.version(__package__)

        @eel.expose  # Expose this function to Javascript
        def get_settings():
            ret = self.settings.copy()
            if self.settings_from_default:
                ret["_default"] = True
            return ret

        @eel.expose  # Expose this function to Javascript
        def save_settings(new_settings):
            self.settings = {}
            for k, s in self.default_settings.items():
                self.settings[k] = new_settings[k]
            self.save_settings()

        @eel.expose
        def process_hex(file_content):
            self._set_logging_stream()
            logging.info("processing hex file")
            try:
                self.program_data = HexUtils.load_hex_to_array(file_content)
                eel.update_process_js({
                    "result": "ok",
                    "output": ""})
                self.hex_available = True
            except Exception as e:
                eel.update_process_js({
                    "result": "fail",
                    "output": self._get_output("VERIFY_FAILED", str(e))})
                self.hex_available = False

        @eel.expose
        def process_manual(setup_dict):
            self._set_logging_stream()
            self.process_manual(setup_dict)

        @eel.expose
        def process_machine(setup_dict):
            self._set_logging_stream()

            if setup_dict['action'] != "start":
                logging.info("Stop request")
                self.stop_request = True
                return

            incoming_data = setup_dict['filedata']
            data = bytes([ord(char) for char in incoming_data])
            if self.worker and self.worker.is_alive():
                eel.update_process_js({
                    "result": "fail",
                    "output": "system is busy"})
                return

            self.worker = threading.Thread(
                target=self.process_machine, args=(data,))
            self.worker.start()

        @eel.expose
        def get_log():
            try:
                log = str(self.log_stream.getvalue())
                return log
            except BaseException:
                return "no log available for last command"

    def process_machine(self, filedata):
        self._set_logging_stream()
        problems = []
        self.stop_request = False

        def callback(status=None, problem=None):
            if status is not None:
                if status["process"]["subprocess"] != "":
                    print(" Subtask {}/{}: {}".format(
                        status["subprocess"]["step"],
                        status["subprocess"]["total_steps"],
                        status["subprocess"]["current_op"]))
                else:
                    print("Task {}/{}: {}".format(
                        status["process"]["step"],
                        status["process"]["total_steps"],
                        status["process"]["current_op"]))

                eel.update_process_js({
                    "result": "update_status",
                    "output": status
                })

            if problem is not None:
                problems.append(problem)
                print("WARNING: ", problem)
                eel.update_process_js({
                    "result": "update_problem",
                    "output": problem
                })

            return self.stop_request

        conn_args = {
            "serialPort": self.settings["serial_port"]
        }

        apl = AlfaPackageLoader(filedata, conn_args, callback)

        print("Starting to update...")
        try:
            apl.process()
        except AlfaPackageLoader.UserInterrupt:
            eel.update_process_js({
                "result": "fail",
                "output": "Process aborted"})
        except Exception as e:
            eel.update_process_js({
                "result": "fail",
                "output": self._get_output("UPDATE_FAILED", str(e))})
            #~ traceback.print_exc(file=sys.stderr)
        else:
            eel.update_process_js({"result": "ok", "output": ""})
        logging.info("Update finished")

    def run(self):
        logging.basicConfig(
            stream=sys.stdout, level="DEBUG",
            format="[%(asctime)s]%(levelname)s %(funcName)s() "
                   "%(filename)s:%(lineno)d %(message)s")

        self.logging_handler = logging.StreamHandler(sys.stderr)
        logging.basicConfig(
            level="DEBUG",
            format="[%(asctime)s]%(levelname)s %(funcName)s() "
                   "%(filename)s:%(lineno)d %(message)s")
        logging.getLogger().addHandler(self.logging_handler)
        self._set_logging_stream(reset_to_stderr=True)

        # to start without opening a new browser:
        #~ eel.start('index.html', mode=False, port=8080)
        eel.start('index.html')

class Application:
    DESCRIPTION = "An utility to program Alfa PIC based boards " \
                  "using USB based bootloader."
    NAME = "alfa_fw_upgrader"
    EXAMPLE_TEXT = '''Actions:
- update: program and verify boards according to given package file
- program: program and verify the application memory with given hex file
- verify: verify the application memory against the given hex file
- info: get memory parameters and boot version
- reset: send command to reset slaves and the board
- jump: send command to jump to main program

Examples:

To perform program and verify,
 > alfa_fw_upgrader -f master_tinting-boot.hex program verify

To perform verify only, with debug info and reset,
 > alfa_fw_upgrader -vv -f master_tinting-boot.hex verify reset'''

    actions = ('update', 'info', 'program', 'verify', 'jump', 'reset')

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
                "sure to have library libusb-1.0.dll in c:\\Windows\\System32",
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
            "descr": "Verify failed due to data or digest value mismatch",
            "retcode": 6
        },
        "COMMAND_FAILED": {
            "descr": "Invalid answer to command or timeout",
            "retcode": 7
        },
        "UPDATE_FAILED": {
            "descr": "Update process failed due to a fatal error ({})",
            "retcode": 8
        },
        "PROGRAM_FAILED": {
            "descr": "Failed to program ({})",
            "retcode": 9
        },
        "DIGEST_FAILED": {
            "descr": "Failed to set digest value ({})",
            "retcode": 9
        }
    }

    def _exit_error(self, error_key, format_arg=None):
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
            traceback.print_exc(file=sys.stderr)
        exit(error_item["retcode"])

    def main(self):
        parser = argparse.ArgumentParser(
            prog=self.NAME,
            description=self.DESCRIPTION,
            epilog=self.EXAMPLE_TEXT,
            formatter_class=argparse.RawDescriptionHelpFormatter)

        parser.add_argument(
            '-f',
            '--filename',
            dest='filename',
            type=str,
            help='filename of the IntelHex file or update package to load')

        parser.add_argument(
            '-d',
            '--deviceid',
            dest='ID',
            type=int,
            default=255,
            help='ID of the device (default=255)')

        parser.add_argument(
            '-s',
            '--strategy',
            dest='strategy',
            choices=[
                'simple',
                'polling',
                'serial'],
            help="strategy for getting device ready for upgrade."
            "- simple: try to connect USB and exit if "
            "device not ready (default choice) "
            "- polling: watch for USB device to became "
            "available and avoid jumping to application "
            "- serial: try to jump to boot using serial "
            "commands")

        parser.add_argument(
            '-p',
            '--serial-port',
            dest='serialport',
            type=str,
            help="when strategy is 'serial', the serial port to "
            "use (default=/dev/ttyUSB0)",
            default='/dev/ttyUSB0')

        parser.add_argument(
            '-t',
            '--polling-time',
            dest='pollingtime',
            type=str,
            help="when strategy is 'polling', the polling time "
            "in seconds (default = 10)",
            default=10)

        parser.add_argument(
            '-cr',
            '--command-retries',
            dest='cmdretries',
            type=int,
            help="number of retries for USB commands "
            "in case of failure (default = 0)",
            default=0)

        parser.add_argument(
            '-ct',
            '--command-timeout',
            dest='cmdtimeout',
            type=int,
            help="timeout for USB commands in milliseconds "
            "(default = 1000)",
            default=1000)

        parser.add_argument("-v", "--verbosity", action="count",
                            help="increase output verbosity")

        parser.add_argument("actions", type=str, nargs='+',
                            choices=self.actions,
                            help="actions to perform")

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

        if 'update' in self.args.actions:
            if self.args.filename is None:
                self._exit_error("FILENAME_REQUIRED")

            with open(self.args.filename, 'rb') as f:
                zip_data = f.read()

            problems = []

            def callback(status=None, problem=None):
                if status is not None:
                    if status["process"]["subprocess"] != "":
                        print(" Subtask {}/{}: {}".format(
                            status["subprocess"]["step"],
                            status["subprocess"]["total_steps"],
                            status["subprocess"]["current_op"]))
                    else:
                        print("Task {}/{}: {}".format(
                            status["process"]["step"],
                            status["process"]["total_steps"],
                            status["process"]["current_op"]))

                if problem is not None:
                    problems.append(problem)
                    print("WARNING: ", problem)

            conn_args = {
                "serialPort": self.args.serialport,
                "pollingInterval": self.args.pollingtime,
                "cmdRetries": self.args.cmdretries,
                "cmdTimeout": self.args.cmdtimeout
            }

            apl = AlfaPackageLoader(zip_data, conn_args, callback)

            print("Starting to update...")
            try:
                apl.process()
            except Exception as e:
                self._exit_error("UPDATE_FAILED", str(e))

            print("Update finished")

            if len(problems) > 0:
                print("but the following problem occurred:")
                for e in problems:
                    print(f" - {e}")

        else:
            # put actions in the right order and avoid repetitions
            actions = [x for x in self.actions if x in self.args.actions]

            program_data = []
            if 'program' in actions or 'verify' in actions:
                if self.args.filename is None:
                    self._exit_error("FILENAME_REQUIRED")
                fn = self.args.filename
                try:
                    with open(fn, 'r') as f:
                        file_content = f.read()
                        program_data = HexUtils.load_hex_to_array(
                            file_content)
                except BaseException:
                    self._exit_error("FILE_LOAD_FAILED", fn)

            try:
                ufl = AlfaFirmwareLoader(
                    deviceId=self.args.ID,
                    serialMode=self.args.strategy == "serial",
                    pollingMode=self.args.strategy == "polling",
                    serialPort=self.args.serialport,
                    pollingInterval=self.args.pollingtime,
                    cmdRetries=self.args.cmdretries,
                    cmdTimeout=self.args.cmdtimeout)
            except Exception as e:
                self._exit_error("INIT_FAILED", str(e))

            for a in actions:
                if a == 'info':
                    if ufl.boot_fw_version is not None:
                        print("Boot version: {}.{}.{} (proto = {})".format(
                            ufl.boot_fw_version[0], ufl.boot_fw_version[1],
                            ufl.boot_fw_version[2], ufl.proto_ver))
                    else:
                        print("Boot version: N/A")

                    print("Memory start address: {} / length: {}".format(
                        ufl.starting_address, ufl.memory_length))

                    if ufl.boot_versions is not None:
                        print(
                            "Boot versions: {}".format(
                                ufl.boot_versions))
                    else:
                        print("Boot versions: N/A")
                    if ufl.fw_versions is not None:
                        print("FW versions: {}".format(ufl.fw_versions))
                        print(
                            "FW versions JSON: {}".format(
                                json.dumps(
                                    ufl.fw_versions)))
                    else:
                        print("FW versions: N/A")
                        print("FW versions JSON: N/A")
                    if ufl.slaves_configuration is not None:
                        print(
                            "Slaves enabled addresses: {}".format(
                                ufl.slaves_configuration))
                    else:
                        print("Slaves enabled addresses: N/A")

                    if ufl.boot_status is not None:
                        print(
                            "Boot status: {}".format(
                                ufl.boot_status))
                    else:
                        print("Boot status: N/A")

                    if ufl.digest is not None:
                        print(
                            "Application digest: {}".format(
                                "%04X" % ufl.digest))
                    else:
                        print("Application digest: N/A")

                elif a == 'program':
                    try:
                        ufl.erase()
                    except BaseException:
                        self._exit_error("ERASE_FAILED")
                    try:
                        ufl.program(program_data)
                    except BaseException:
                        self._exit_error("PROGRAM_FAILED")
                    try:
                        if not ufl.verify(
                                program_data, check_digest=False):
                            self._exit_error("VERIFY_DATA_MISMATCH")
                    except BaseException:
                        self._exit_error("VERIFY_FAILED")
                    try:
                        ufl.seal(program_data)
                    except BaseException:
                        self._exit_error("DIGEST_FAILED")

                elif a == 'verify':
                    try:
                        if not ufl.verify(program_data):
                            self._exit_error("VERIFY_DATA_MISMATCH")
                    except BaseException:
                        self._exit_error("VERIFY_FAILED")
                elif a == 'reset':
                    try:
                        ufl.reset()
                    except BaseException:
                        self._exit_error("COMMAND_FAILED")
                elif a == 'jump':
                    try:
                        ufl.jump()
                    except BaseException:
                        self._exit_error("COMMAND_FAILED")

            ufl.disconnect()


if __name__ == '__main__':
    Application().main()
