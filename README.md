# alfa_fw_upgrader
alfa_fw_upgrader is a project to program the boards based on PIC 24.
These boards have a bootloader featuring a set of commands on USB to program
and verify the memory.  

It provides a command-line application, but it is also a module with classes
that can be imported into other projects as a library to interact with
the bootloader and perform memory operations.

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
>     $ alfa_fw_upgrader

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

5. run the program:
>     c:\work\alfa_fw_upgrader_env\Scripts\activate   
>     alfa_fw_upgrader_launcher.py

### Troubleshooting

- On Windows, if the program exits suddently without any output, it could be the case that
  *libusbK* is installed. Run Zadig to set the driver *WinUSB*, then delete file 
  `c;\Windows\System32\libusbK.dll` if not needed for other purposes to avoid
  further problems.

### Notes
In order to test the routine for hex file reading, it is required to install 
library *hexutils*:

>     pip install hexutils

However the library itself is not required to run the application and therefore
is is not a dependency of the project.

