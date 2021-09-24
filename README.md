# alfa_fw_upgrader
alfa_fw_upgrader is an application to program Alfa PIC based boards using
USB based bootloader.

## Installation

### On linux

1. install libusb:
>     # apt install libusb

2. create a virtualenv 
>     $ export VIRTENV_ROOT=desired-virtenv_root-path
>     $ mkdir ${VIRTENV_ROOT}
>     $ virtualenv -p /usr/bin/python3 ${VIRTENV_ROOT}

3. clone this project in ${PROJECT_ROOT}
>     $ git clone git@github.com:alfa-sw/sw-programmazione-schede.git
>     $ git checkout develop

4. build Install in edit mode:
>     $ . ${VIRTENV_ROOT}/bin/activate
>     $ cd ${PROJECT_ROOT}
>     $ pip install -e ./

5. Run:
>     $ . ${VIRTENV_ROOT}/bin/activate
>     $ alfa_fw_upgrader.py

### On windows

Install usb drivers.

1. download libusb from this [link](https://github.com/libusb/libusb/releases/download/v1.0.24/libusb-1.0.24.7z)
   and place `libusb-1.0.dll` on `c:\windows\system32`

 -- Next steps are needed if alfa_fw_upgrader fails to initialize the device. --

2. install Zadig from [here](https://zadig.akeo.ie/) and launch it

3. follow instructions in this [link](https://github.com/pbatard/libwdi/wiki/Zadig)
   to set the driver *WinUSB* for device `USB Hid Bootloader`


Install the application.

1. install python > 3.7

2. download repository from:
   https://github.com/alfa-sw/sw-programmazione-schede/archive/refs/heads/master.zip
   and extract to `c:\work\alfa_fw_upgrader`

3. install virtual envinronment
>    set PYTHON_BIN=%USERPROFILE%\AppData\Local\Programs\Python\Python39\python.exe
>    %PYTHON_BIN% -m venv c:\work\alfa_fw_upgrader_env

4. activate new environment and install:
>    c:\work\alfa_fw_upgrader_env\Script\activate   
>    cd c:\work\alfa_fw_upgrader
>    python -m pip install -e ./

5. run the program:
>    c:\work\alfa_fw_upgrader_env\Script\activate   
>    alfa_fw_upgrader.py 


