[This is an extract of README.md from: https://github.com/alfa-sw/sw-programmazione-schede]

alfa_fw_upgrader is a project to program the boards based on PIC 24.
These boards have a bootloader featuring a set of commands on USB to program
and verify the memory.  

It provides an application with either a GUI interface, either a command-line
interface but it is also a module with classes that can be imported into other
projects as a library to interact with the bootloader and perform memory
operations.

Note: Chrome/Chromium must be installed to use GUI.

INSTALLATION OF USB DRIVERS ON WINDOWS
======================================

Install usb drivers.

1. download libusb from this [link](https://github.com/libusb/libusb/releases/download/v1.0.24/libusb-1.0.24.7z);
   extract file `VS2019/MS64/libusb-1.0.dll` from the archive and place 
   `libusb-1.0.dll` on `c:\windows\system32`

 **Next steps are needed if alfa_fw_upgrader fails to initialize the device.**

2. install Zadig from [here](https://zadig.akeo.ie/) and launch it

3. follow instructions in this [link](https://github.com/pbatard/libwdi/wiki/Zadig)
   to set the driver *WinUSB* for device `USB Hid Bootloader`

TROUBLESHOOTING
===============

- On Windows, if the program exits suddently without any output, it could be the case that
  *libusbK* is installed. Run Zadig to set the driver *WinUSB*, then delete file 
  `c;\Windows\System32\libusbK.dll` if not needed for other purposes to avoid
  further problems.

USER GUIDE
==========

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


