# Project: Alfa color Bootloader
PROJECT_NAME = bootloaderMABrd

# Directories
ZDRIVE        = C:\PROGETTI\Alfa\repository\fw-mab\Source_Code\PIC\Program\Alfa_Color_Tester

BASEDIR       = $(ZDRIVE)\$(PROJECT_NAME)

INCLUDE       = $(BASEDIR)\include

OUT           = $(BASEDIR)\out
OBJ_OUT       = $(BASEDIR)\obj

MC_BASEDIR    = $(ZDRIVE)\Microchip
MC_INC        = $(MC_BASEDIR)\Include
MC_USB_LIB    = $(MC_BASEDIR)\Usb
MC_FS_LIB     = $(MC_BASEDIR)\MDD File System

C30_BASEDIR   = C:\Program Files\Microchip\MPLAB C30
C30_LIB       = $(C30_BASEDIR)\lib
C30_INC       = $(C30_BASEDIR)\include
C30_SUPPORT   = $(C30_BASEDIR)\support\PIC24F\h

# Macros
DEBUG_COMMON  = -D__DEBUG -D_DEBUG_MPLAB
DEBUGGER_ICD3 = $(DEBUG_COMMON) -D__MPLAB_DEBUGGER_ICD3=1
DEBUGGER_RICE = $(DEBUG_COMMON) -D__MPLAB_DEBUGGER_REAL_ICE=1
GLD_FILE      = $(BASEDIR)\p24FJ256GB106.gld
TARGET_FLAG   = -mcpu=24FJ256GB106
TARGET_DEFINE = -D__PIC24F__
OPT_LEV       = s

# CRC calculation parameters
FLASH_BEGIN_ADDR = 0x000000
FLASH_END_ADDR   = 0x002BFE
CRC_ARGS    = H 0 N $(FLASH_BEGIN_ADDR) $(FLASH_END_ADDR) B

# Assembler macros
AFLAGS        = -I "$(INCLUDE)" --defsym __24FJ256GB106=1

# Compiler macros
DEFINES  = -D_COMPILE_DEVICE_
#CFLAGS   = $(TARGET_FLAG) -x c -std=gnu99 -O$(OPT_LEV) -mconst-in-code -mlarge-code -Wall -Wextra -Werror -O3 -Os
CFLAGS   = $(TARGET_FLAG) -x c -std=gnu99 -O$(OPT_LEV) -mconst-in-code -mlarge-code -Wall -Wextra -O3 -Os

# Inclusion path
INCPATH = -I"$(INCLUDE)" -I"$(MC_BASEDIR)" -I"$(MC_INC)"		\
-I"$(MC_USB_LIB)" -I"$(MC_USB_LIB)\HID Device Driver" -I"$(BASEDIR)"	\
-I"$(C30_LIB)" -I"$(C30_INC)" -I"$(C30_SUPPORT)"

LFLAGS_COMMON    = -Wl,--report-mem,-L"$(C30_LIB)",-Map="$(OUT)\$(PROJECT_NAME).map"
LFLAGS_RELEASE   = -Wl,-T$(GLD_FILE),--defsym=__MPLAB_BUILD=1
LFLAGS_BL_ICD3   = -Wl,-T$(GLD_FILE),--defsym=__MPLAB_DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,--defsym=__ICD2RAM=1
LFLAGS_BL_RICE   = -Wl,-T$(GLD_FILE),--defsym=__MPLAB_DEBUG=1,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1,--defsym=__ICD2R

# Programs
AS = pic30-as.exe
CC = pic30-gcc.exe
LD = pic30-ld.exe
AR = pic30-ar.exe
HX = pic30-bin2hex.exe
RM = del
CRC = crcpr.exe
CP = copy

# Additional tools
CTAGS = ctags.exe
MD5 = md5sum.exe --tag

# Assembly objects
OBJECTS_ASM = $(OBJ_OUT)\cputestisr.o $(OBJ_OUT)\cputestsubsets.o

# BootLoader C Objects
BL_OBJECTS_C = $(OBJ_OUT)\usb_descriptors.o $(OBJ_OUT)\main.o		\
        $(OBJ_OUT)\usb_function_hid.o $(OBJ_OUT)\BL_USB_ServerMg.o	\
        $(OBJ_OUT)\ram.o $(OBJ_OUT)\usb_device.o $(OBJ_OUT)\Const.o	\
        $(OBJ_OUT)\TimerMg.o $(OBJ_OUT)\eeprom.o			\
        $(OBJ_OUT)\progMemFunctions.o $(OBJ_OUT)\SN_Mg.o		\
        $(OBJ_OUT)\serialCom.o $(OBJ_OUT)\BL_UART_ServerMg.o		\
        $(OBJ_OUT)\spimpol.o

default:
	@echo ** Pick a target. Valid targets follow:
	@echo **
	@echo ** icd3
	@echo ** rice
	@echo ** release

release:clean objects link hexify crc
	@echo Relase build completed 

	copy $(OUT)\$(PROJECT_NAME).hex $(BASEDIR)\dist\BootLoaderMAB.hex
	@echo copy done...

icd3:	clean objects-for-icd3 link-icd3 hexify crc
	@echo ICD3 Debg build completed

rice:	clean objects-for-rice link-rice hexify crc
	@echo RealICE Debug build completed

crc:
	$(CRC) $(OUT)\$(PROJECT_NAME).hex $(CRC_ARGS)
	@echo CRC done...

# This requires GNU coreutils which can be found at
# http://gnuwin32.sourceforge.net/packages/coreutils.htm
md5:
	@$(MD5) $(OUT)\$(PROJECT_NAME).hex

# This target requires Exuberant CTags
# http://ctags.sourceforge.net/
etags:
	@echo Creating tags file (emacs format...)
	@$(CTAGS) -Ref TAGS .

hexify: $(OUT)\$(PROJECT_NAME).hex
	@echo Hex Converter done...

$(OUT)\$(PROJECT_NAME).hex : $(OUT)\$(PROJECT_NAME).cof
	$(HX) "$(OUT)\$(PROJECT_NAME).cof" -v

link: $(OUT)\$(PROJECT_NAME).cof
	@echo Linking done...

$(OUT)\$(PROJECT_NAME).cof :
	$(CC) $(TARGET_FLAG)  $(OBJECTS_ASM) $(BL_OBJECTS_C) -o"$(OUT)\$(PROJECT_NAME).cof" $(LFLAGS_COMMON) $(LFLAGS_RELEASE)

link-icd3: $(OBJECTS_ASM) $(BL_OBJECTS_C)
	$(CC) $(TARGET_FLAG) $(OBJECTS_ASM) $(BL_OBJECTS_C) -o"$(OUT)\$(PROJECT_NAME).cof" $(LFLAGS_COMMON) $(LFLAGS_BL_ICD3)

link-rice: $(OBJECTS_ASM) $(BL_OBJECTS_C)
	$(CC) $(TARGET_FLAG) $(OBJECTS_ASM) $(BL_OBJECTS_C) -o"$(OUT)\$(PROJECT_NAME).cof" $(LFLAGS_COMMON) $(LFLAGS_BL_RICE)

# default compilation target (release)
objects: $(OBJECTS_ASM) $(BL_OBJECTS_C)
	@echo Objects done...

# compilation target for the ICD3 debugger
objects-for-icd3: CFLAGS  += -O0 -Os -g
objects-for-icd3: DEFINES += $(DEBUGGER_ICD3)
objects-for-icd3: objects

# compilation target for the RealIce debugger
objects-for-rice: CFLAGS  += -O0 -Os -g
objects-for-rice: DEFINES += $(DEBUGGER_RICE)
objects-for-rice: objects

clean:
	@echo Cleaning all...
	@$(RM) "$(OBJ_OUT)\*.o"
	@$(RM) "$(OUT)\$(PROJECT_NAME).cof"
	@$(RM) "$(OUT)\$(PROJECT_NAME).hex"

# cpu test files (assembly sources)
$(OBJ_OUT)\cputestisr.o :
	$(AS) $(AFLAGS) "cputestisr.s" -o "$(OBJ_OUT)\cputestisr.o"

$(OBJ_OUT)\cputestsubsets.o :
	$(AS) $(AFLAGS) "cputestsubsets.s" -o "$(OBJ_OUT)\cputestsubsets.o"

# Boot Loader files (c sources)
$(OBJ_OUT)\usb_descriptors.o :
	$(CC) $(CFLAGS) $(DEFINES) $(INCPATH) -c "usb_descriptors.c" -o"$(OBJ_OUT)\usb_descriptors.o"

$(OBJ_OUT)\flash.o :
	$(CC) $(CFLAGS) $(DEFINES) $(INCPATH) -c "flash.c" -o"$(OBJ_OUT)\flash.o"

$(OBJ_OUT)\main.o :
	$(CC) $(CFLAGS) $(DEFINES) $(INCPATH) -c "main.c" -o"$(OBJ_OUT)\main.o"

$(OBJ_OUT)\usb_function_hid.o :
	$(CC) $(CFLAGS) $(DEFINES) $(INCPATH) -c "$(MC_USB_LIB)\HID Device Driver\usb_function_hid.c" -o"$(OBJ_OUT)\usb_function_hid.o"

$(OBJ_OUT)\usb_device.o :
	$(CC) $(CFLAGS) $(DEFINES) $(INCPATH) -c "$(MC_USB_LIB)\usb_device.c" -o"$(OBJ_OUT)\usb_device.o"

$(OBJ_OUT)\eeprom.o :
	$(CC) $(CFLAGS) $(DEFINES) $(INCPATH) -c "eeprom.c" -o"$(OBJ_OUT)\eeprom.o"

$(OBJ_OUT)\spimpol.o :
	$(CC) $(CFLAGS) $(DEFINES) $(INCPATH) -c "spimpol.c" -o"$(OBJ_OUT)\spimpol.o"

$(OBJ_OUT)\BL_USB_ServerMg.o :
	$(CC) $(CFLAGS) $(DEFINES) $(INCPATH) -c "BL_USB_ServerMg.c" -o"$(OBJ_OUT)\BL_USB_ServerMg.o"

$(OBJ_OUT)\ram.o :
	$(CC) $(CFLAGS) $(DEFINES) $(INCPATH) -c "ram.c" -o"$(OBJ_OUT)\ram.o"

$(OBJ_OUT)\Const.o :
	$(CC) $(CFLAGS) $(DEFINES) $(INCPATH) -c "Const.c" -o"$(OBJ_OUT)\Const.o"

$(OBJ_OUT)\Timermg.o :
	$(CC) $(CFLAGS) $(DEFINES) $(INCPATH) -c "Timermg.c" -o"$(OBJ_OUT)\Timermg.o"

$(OBJ_OUT)\SN_Mg.o :
	$(CC) $(CFLAGS) $(DEFINES) $(INCPATH) -c "SN_Mg.c" -o"$(OBJ_OUT)\SN_Mg.o"

$(OBJ_OUT)\serialCom.o :
	$(CC) $(CFLAGS) $(DEFINES) $(INCPATH) -c "serialCom.c" -o"$(OBJ_OUT)\serialCom.o"

$(OBJ_OUT)\BL_UART_ServerMg.o :
	$(CC) $(CFLAGS) $(DEFINES) $(INCPATH) -c "BL_UART_ServerMg.c" -o"$(OBJ_OUT)\BL_UART_ServerMg.o"

$(OBJ_OUT)\progMemFunctions.o :
	$(CC) $(CFLAGS) $(DEFINES) $(INCPATH) -c "progMemFunctions.c" -o"$(OBJ_OUT)\progMemFunctions.o"
