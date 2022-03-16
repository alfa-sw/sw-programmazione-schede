"""
alfa_fw_upgrader - a package to program Alfa PIC based boards using USB based bootloader.

This module contains code to load an update package.
This is a zip file containing a manifest file and firmware files.

"""

# pylint: disable=invalid-name
# pylint: disable=missing-docstring
# pylint: disable=broad-except
# pylint: disable=logging-format-interpolation
# pylint: disable=logging-fstring-interpolation

import logging
import time
import zipfile
from io import BytesIO
import yaml

from alfa_fw_upgrader.hexutils import HexUtils
from typing import NoReturn

from alfa_fw_upgrader.fw_loader import AlfaFirmwareLoader

class AlfaPackageLoader:
    class UserInterrupt(Exception):
        pass

    def __init__(self, package_data, serial_port, process_callback=None):
        self.package_data = package_data
        self.process_callback = process_callback
        self.serial_port = serial_port

        self.sts = {
            "process": {
                "current_op": "",
                "step": 0,
                "total_steps": 0,
                "subprocess": ""},
            "subprocess": {
                "current_op": "",
                "step": 0,
                "total_steps": 0}}

        self.programs_hex = {}
        self.boot_versions = None
        self.fw_versions = None
        self.slaves_configuration = None
        self.manifest = None

    def report_problem(self, problem):
        self.process_callback(status=None, problem=problem)

    def update_status(
            self,
            caller,
            current_op=None,
            step=None,
            total_steps=None):
        if caller == "main":
            key = "process"
            self.sts["subprocess"] = {
                "current_op": "", "step": 0, "total_steps": 0}
            self.sts["process"]["subprocess"] = ""
        else:
            key = "subprocess"
            self.sts["process"]["subprocess"] = caller

        if current_op is not None:
            self.sts[key]["current_op"] = current_op
        if step is not None:
            self.sts[key]["step"] = step
        if current_op is not None:
            self.sts[key]["total_steps"] = total_steps

        if self.process_callback(status=self.sts, problem=None):
            raise self.UserInterrupt

    def process(self):
        current_step = 1
        self.update_status("main", "loading package", 1, 5)
        self.load_package(self.package_data)

        current_step += 1
        self.update_status("main", "initialize", 2, 5)

        params = dict(device_id = 255,
                      use_serial_proto = True,
                      polling_mode = False,
                      serial_port = self.serial_port,
                      is_serial_proto_duplex = True)

        initialize_ok = False
        try:
            self.board_init(params)
            initialize_ok = True
        except Exception as e:
            logging.warning(
                f"need to reinitialize after programming master ({str(e)})")

        master_node = list(filter(
            lambda x: x["board-name"] == "master",
            self.manifest["programs"]))[0]

        self.update_status("main", "programming master", 3, 5)
        try:
            afl = None
            afl = AlfaFirmwareLoader(**params)
            afl.erase()
            hexdata = self.programs_hex[master_node['filename']]
            afl.program(hexdata)
            assert afl.verify(hexdata, check_digest=False)
            afl.seal(hexdata)
            afl.disconnect()
        except Exception as e:
            self.report_problem("failed to program master 1st attempt")
            if not initialize_ok:
                raise RuntimeError(
                    f"failed to program master and init ({str(e)})") from e
        finally:
            if afl is not None:
                afl.disconnect()

        if not initialize_ok:
            try:
                self.board_init(params)
            except BaseException as e:
                raise RuntimeError("failed to initialize") from e
            finally:
                afl.disconnect()

        program_steps = {}

        if self.boot_versions['boot_master_protocol'] < 1:
            raise RuntimeError("upgrade not supported by master")
        for program in self.manifest["programs"]:
            for addr in program['addresses']:
                if addr != 255 and addr in self.slaves_configuration:
                    program_steps[addr] = program
        logging.debug(f"programs steps: {program_steps} "
                      f"slaves_configuration: {self.slaves_configuration}")

        current_step = 1
        total_steps = len(program_steps)

        self.update_status("main", "programming slaves", 4, 5)
        for address, step in program_steps.items():
            self.update_status("slaves", f"programming slave #{address}",
                               current_step, total_steps)

            try:
                program = self.programs_hex[step['filename']]
                params["device_id"] = address
                afl = AlfaFirmwareLoader(**params)
                if afl.boot_fw_version is None or afl.proto_ver < 1:
                    self.report_problem(
                        f"slave with address {address} is incompatible or "
                        f"not present - NOT upgrading")
                else:
                    afl.erase()
                    afl.program(program)
                    assert afl.verify(program, check_digest=False)
                    afl.seal(program)

            except BaseException as e:
                self.report_problem(
                    f"failed to program slave with address {address}, {e}")
            finally:
                if afl is not None:
                    afl.disconnect()

            current_step += 1

        try:
            self.update_status("main", "jumping to application", 5, 5)
            params["device_id"] = 255
            afl = AlfaFirmwareLoader(**params)
            afl.jump()
            afl.disconnect()
        except BaseException as e:
            raise RuntimeError("failed to jump to application") from e
        finally:
            afl.disconnect()

    def board_init(self, params):
        self.update_status(
            "init", "retrieve data version and jump to boot", 1, 3)

        def check_invalid_ver(ufl):
            return ufl.was_app_running is False or \
                ufl.fw_versions is None or ufl.boot_versions is None or \
                ufl.slaves_configuration is None

        params["use_serial_proto"] = True
        params["device_id"] = 255

        try:
            afl = None
            afl = AlfaFirmwareLoader(**params)
            if check_invalid_ver(afl):
                logging.warning("app was not running or problem in retrieving "
                                "version data -> jump to app and retry")
                self.update_status("init", "jump to app", 2, 3)

                afl.jump()
                afl.disconnect()
                time.sleep(5)
                self.update_status("init", "jump to boot again", 3, 3)
                afl = AlfaFirmwareLoader(**params)

                if check_invalid_ver(afl):
                    raise RuntimeError(
                        "app was not running or problem in retrieving "
                        "version data")

            self.boot_versions = afl.boot_versions
            self.fw_versions = afl.fw_versions
            self.slaves_configuration = afl.slaves_configuration

        except Exception as e:
            raise e
        finally:
            if afl is not None:
                afl.disconnect()

    def load_package(self, package_data):
        fp = BytesIO(package_data)
        with zipfile.ZipFile(fp, "r") as zfp:
            with zfp.open('manifest.txt', 'r') as mfp:
                self.manifest = yaml.load(mfp, Loader=yaml.SafeLoader)

            self.programs_hex = {}
            for program in self.manifest["programs"]:
                fn = program["filename"]
                with zfp.open(fn) as f:
                    self.programs_hex[fn] = HexUtils.load_hex_to_array(
                        f.read().decode())
