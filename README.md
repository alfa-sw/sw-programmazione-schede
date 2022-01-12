# alfa_fw_upgrader
alfa_fw_upgrader is a project to program the boards based on PIC 24.
These boards have a bootloader featuring a set of commands on USB to program
and verify the memory.  

It provides an application with either a GUI interface, either a command-line
interface but it is also a module with classes that can be imported into other
projects as a library to interact with the bootloader and perform memory
operations.

## Installation

### On linux

1. install libusb:
>     # apt install libusb-1.0.0

2. Adjust USB device permissions.
>     # echo 'SUBSYSTEM=="usb", ATTRS{idVendor}=="04d8", MODE="0666"' > 
>       /etc/udev/rules.d/99-microchip.rules 
>     # /etc/init.d/udev restart

3. create a virtualenv 
>     $ export VIRTENV_ROOT=desired-virtenv_root-path
>     $ mkdir ${VIRTENV_ROOT}
>     $ virtualenv -p /usr/bin/python3 ${VIRTENV_ROOT}

4. clone this project in ${PROJECT_ROOT}
>     $ git clone git@github.com:alfa-sw/sw-programmazione-schede.git
>     $ git checkout develop

5. activate virtualenv:
>     $ . ${VIRTENV_ROOT}/bin/activate
>     $ cd ${PROJECT_ROOT}

6. Install rs485_master in this virtualenv, following its README

7. Install in edit mode:
>     $ pip install -e ./

8. Run:
>     $ . ${VIRTENV_ROOT}/bin/activate

To use GUI application:
>     $ alfa_fw_upgrader_gui

To use CLI application:
>     $ alfa_fw_upgrader_cli

To start a service suitable for rapsberry environment:
>     $ alfa_fw_upgrader_service

### On windows

Install usb drivers.

1. download libusb from this [link](https://github.com/libusb/libusb/releases/download/v1.0.24/libusb-1.0.24.7z);
   extract file `VS2019/MS64/libusb-1.0.dll` from the archive and place 
   `libusb-1.0.dll` on `c:\windows\system32`

 **Next steps are needed if alfa_fw_upgrader fails to initialize the device.**

2. install Zadig from [here](https://zadig.akeo.ie/) and launch it

3. follow instructions in this [link](https://github.com/pbatard/libwdi/wiki/Zadig)
   to set the driver *WinUSB* for device `USB Hid Bootloader`

Install the application.

1. install python 3 (tested with 3.9) 

2. download repository from:
   https://github.com/alfa-sw/sw-programmazione-schede/archive/refs/heads/develop.zip
   and extract to `c:\work\alfa_fw_upgrader`

3. install virtual envinronment (adjust python interpreter path if needed)
>     set PYTHON_BIN=%USERPROFILE%\AppData\Local\Programs\Python\Python39\python.exe
>     %PYTHON_BIN% -m venv c:\work\alfa_fw_upgrader_env

4. activate new environment and install:
>     c:\work\alfa_fw_upgrader_env\Scripts\activate   
>     cd c:\work\alfa_fw_upgrader

5. Install rs485_master in this virtualenv, following its README

6. Install in edit mode:
>     python -m pip install -e ./

7. run the program:
>     c:\work\alfa_fw_upgrader_env\Scripts\activate   
>     alfa_fw_upgrader_gui.py
or
>     alfa_fw_upgrader_cli.py

### Troubleshooting

- On Windows, if the program exits suddently without any output, it could be the case that
  *libusbK* is installed. Run Zadig to set the driver *WinUSB*, then delete file 
  `c;\Windows\System32\libusbK.dll` if not needed for other purposes to avoid
  further problems.

## User guide

In order to update the firmware it is needed to put the board in update mode.
The application provides the following modalities (strategies) to achieve this:
 - strategy *simple*: an attempt to connect to USB is done; in case of failure
   an error is displayed;
 - strategy *polling*: multiple attempts to connect to USB until the timeout;
   the correct sequence is to start the application and then power on the board,
   previously connected via USB; in this way it is possible to upgrade the fw
   because bootloader go to update mode before jumping to main program;
 - strategy *serial*: an attempt to connect to USB is done; in case of failure
   it starts to communicate to the board using serial interface (RS232) and it
   send a command to go to update mode.

The strategy *serial* is useful in case of update while the machine is already
in working mode, e.g. in case of remote updates. The strategy *polling* is
suitable in case of manual update by connecting the board to a service PC
without need to attach a serial adapter too.

Two user interface are provided. If no arguments are given, it starts a GUI
based on Chromium/Chrome browser. Otherwise it starts a CLI interface,

Refers to program help for further usage details:

>     alfa_fw_upgrader -h

## Notes
In order to test the routine for hex file reading, it is required to install 
library *hexutils*:

>     pip install hexutils

However the library itself is not required to run the application and therefore
is is not a dependency of the project.

