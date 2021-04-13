/**/
/*===========================================================================*/
/**
 **      @file    USB_ServerMg.c
 **
 **      @brief   I/O management
 **
 **      @version Haemotronic - Haemodrain
 **/
/*===========================================================================*/
/**/

#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "HardwareProfile.h"
#include "define.h"
#include "MACRO.h"
#include "ram.h"
#include "BL_USB_ServerMg.h"
#include "./USB/usb_config_device.h"
#include "./USB/usb_device.h"
#include "./USB/usb.h"
#include "HardwareProfile.h"
#include "./USB/usb_function_hid.h"
#include "Timermg.h"
#include "progMemFunctions.h"
#include "serialCom.h"
#include "BL_UART_ServerMg.h"

/*====== MACRO LOCALI ====================================================== */

// Section defining the address range to erase for the erase device
// command, along with the valid programming range to be reported by
// the QUERY_DEVICE command.
#define VectorsStart 0x00000000

// One page of vectors + general purpose program memory.
#define VectorsEnd   0x00000400

//Bootloader resides in memory range 0x400-0x17FF

// Beginning of application program memory (not occupied by
// bootloader).  **THIS VALUE MUST BE ALIGNED WITH BLOCK BOUNDRY**
// Also, in order to work correctly, make sure the StartPageToErase is
// set to erase this section.
// #define ProgramMemStart START_APPL_ADDRESS

// Macro per identificare programmazione rivolta verso la TINTING MASTER o verso gli slave
#define PROGRAM_MAB 0XFF

// Bootloader and vectors occupy first six 1024 word (1536 bytes due to
// 25% unimplemented bytes) pages
#define BeginPageToErase FIRST_PG_APPL

// Last full page of flash on the PIC24FJ256GB110, which does not
// contain the flash configuration words.
#define MaxPageToEraseNoConfigs 169

// Page 170 contains the flash configurations words on the
// PIC24FJ256GB110.  Page 170 is also smaller than the rest of the
// (1536 byte) pages.
#define MaxPageToEraseWithConfigs 170

// Must be instruction word aligned address.  This address does not get
// updated, but the one just below it does:

#define ProgramMemSlaveStopNoConfigs 0x00005800
#define ProgramMemSlaveStopNoConfigsHumidifier 0x0000AF00
#define ProgramMemSlaveStopNoConfigsTinting 0x0002A800
//#define ProgramMemSlaveStopNoConfigsTinting 0x0001FFFE
#define ProgramMemMasterStopNoConfigs 0x0002A800

// IE: If AddressToStopPopulating = 0x200, 0x1FF is the last
// programmed address (0x200 not programmed)
// #define ProgramMemStopNoConfigs               0x0002000

// Must be instruction word aligned address.  This address does not get
// updated, but the one just below it does: IE: If
// AddressToStopPopulating = 0x200, 0x1FF is the last programmed
// address (0x200 not programmed)
#define ProgramMemStopWithConfigs 0x0002ABF8

// 0x2ABFA is start of CW3 on PIC24FJ256GB110 Family devices
#define ConfigWordsStartAddress 0x0002ABF8

#define ConfigWordsStopAddress 0x0002AC00

// -- Switch State Variable Choices

// Command that the host uses to learn about the device (what regions
// can be programmed, and what type of memory is the region)
#define QUERY_DEVICE      0x02

// Note, this command is used for both locking and unlocking the config
// bits (see the "//Unlock Configs Command Definitions" below)
#define UNLOCK_CONFIG     0x03

// Host sends this command to start an erase operation.  Firmware
// controls which pages should be erased.
#define ERASE_DEVICE      0x04

// If host is going to send a full RequestDataBlockSize to be
// programmed, it uses this command.
#define PROGRAM_DEVICE    0x05

// If host send less than a RequestDataBlockSize to be programmed, or
// if it wished to program whatever was left in the buffer, it uses
// this command.
#define PROGRAM_COMPLETE  0x06

// The host sends this command in order to read out memory from the
// device.  Used during verify (and read/export hex operations)
#define GET_DATA          0x07

// Resets the microcontroller, so it can update the config bits (if
// they were programmed, and so as to leave the bootloader (and
// potentially go back into the main application)
#define RESET_DEVICE      0x08

// When this command is received, it is sent a Broadcast command JUMP_TO_APPLICATION to all the SLAVE and at the end also to TINTING MASTER
#define JUMP_TO_APPLICATION 0x09

// Request of BootLoader Firmware versione of a specified SLAVE, or TINTING MASTER
#define BOOT_FW_VERSION_REQUEST 0x0A

// -- Unlock Configs Command Definitions

// Sub-command for the ERASE_DEVICE command
#define UNLOCKCONFIG 0x00


// Sub-command for the ERASE_DEVICE command
#define LOCKCONFIG   0x01

// -- Query Device Response "Types"

// When the host sends a QUERY_DEVICE command, need to respond by
// populating a list of valid memory regions that exist in the device
// (and should be programmed)
#define TypeProgramMemory 0x01

#define TypeEEPROM        0x02
#define TypeConfigWords   0x03

// Sort of serves as a "null terminator" like number, which denotes the
// end of the memory region list has been reached.
#define TypeEndOfTypeList 0xFF

// BootState Variable States
#define IdleState    0x00
#define NotIdleState 0x01
#define WaitState 0x02

// OtherConstants
#define InvalidAddress 0xFFFFFFFF

// -- Application and Microcontroller constants

// For Flash memory: One byte per address on PIC18, two bytes per
// address on PIC24
#define BytesPerFlashAddress 0x02

#define TotalPacketSize 0x40

//PIC18 uses 2 byte instruction words, PIC24 uses 3 byte "instruction
//words" (which take 2 addresses, since each address is for a 16 bit
//word; the upper word contains a "phantom" byte which is
//unimplemented.).
#define WORDSIZE  0x02

// For PIC18F87J50 family devices, a flash block is 64 bytes
//#define FlashBlockSize 0x40

//Number of bytes in the "Data" field of a standard request to/from
//the PC.  Must be an even number from 2 to 56.
#define RequestDataBlockSize 56

//Number of bytes in the "Data" field of a BOOT_FW_VERSION_REQUEST command
#define RequestFWVersionBlockSize 3

//32 16-bit words of buffer
#define BufferSize 0x20

#define PWD_LGTH 8

/* MAX_NUM_PACK: 0x5800-0x1700 = 0x4100 = 16640 / 2 *3 = 24960 byte
   24960/56 = 474 pacchetti; 24960%56 = 40 ultimo pacchetto da 40 byte.
   -> MAX_NUM_PACK = 475 + 1. (deve essere > di 1 rispetto al valore reale)
   Lo stesso conteggio lo si può eseguire a partire dal file .hex
*/
//#define MAX_NUM_PACK 476
//  In realtà il numeor id pacchetti puo essere > della soglia fissata. Conviene quindi alzarla ad un valore simbolico
#define MAX_NUM_PACK 10000

/* MAX_NUM_PACK: 0xAF00-0x2000 = 0x8F00 = 36608 / 2 * 3 = 54912 byte
   54912 / 56 = 980 pacchetti; 54912 % 56 = 32 ultimo pacchetto da 32 byte.
   -> MAX_NUM_PACK_HUMIDIFIER = 981 + 1. (deve essere > di 1 rispetto al valore reale)
   Lo stesso conteggio lo si può eseguire a partire dal file .hex
*/
//#define MAX_NUM_PACK_HUMIDIFIER 982
//  In realtà il numeor id pacchetti puo essere > della soglia fissata. Conviene quindi alzarla ad un valore simbolico
#define MAX_NUM_PACK_HUMIDIFIER 10000
#define MAX_NUM_PACK_TINTING 10000


#define INIT_NUM_SENDING_RESET_DEVICE_CMD   (1)
/*====== TIPI LOCALI ======================================================== */
typedef union __attribute__ ((packed)) _USB_HID_BOOTLOADER_COMMAND
{
    unsigned char Contents[64];

    struct __attribute__ ((packed)) {
        unsigned char Command;
        WORD AddressHigh;
        WORD AddressLow;
        unsigned char Size;
        unsigned char PadBytes[(TotalPacketSize - 6) - (RequestDataBlockSize)];
        unsigned int Data[RequestDataBlockSize/WORDSIZE];
    };

    struct __attribute__ ((packed)) {
        unsigned char Command;
        DWORD Address;
        unsigned char Size;
        unsigned char PadBytes[(TotalPacketSize - 6) - (RequestDataBlockSize)];
        unsigned int Data[RequestDataBlockSize/WORDSIZE];
    };

    struct __attribute__ ((packed)) {
        unsigned char Command;
        unsigned char PacketDataFieldSize;
        unsigned char BytesPerAddress;
        unsigned char Type1;
        unsigned long Address1;
        unsigned long Length1;
        unsigned char Type2;
        unsigned long Address2;
        unsigned long Length2;
        unsigned char Type3;            //End of sections list indicator
        //goes here, when not programming
        //the vectors, in that case fill
        //with 0xFF.
        unsigned long Address3;
        unsigned long Length3;
        unsigned char Type4;            //End of sections list indicator
        //goes here, fill with 0xFF.

        unsigned char ExtraPadBytes[33];
    };

    struct __attribute__ ((packed)){  //For lock/unlock config command
        unsigned char Command;
        unsigned char LockValue;
    };

    struct __attribute__ ((packed)) {
      unsigned char Command;
      unsigned char Password[PWD_LGTH];
      unsigned char IdFwProgramming;
    };
    
    struct __attribute__ ((packed)) {
        unsigned char FW_Ver_ZZ;
        unsigned char FW_Ver_YY;
        unsigned char FW_Ver_XX;
    };
    
} PacketToFromPC;

/*====== VARIABILI LOCALI =================================================== */
PacketToFromPC PacketFromPC; //64 byte buffer for receiving packets on
                             //EP1 OUT from the PC
PacketToFromPC PacketToPC;   //64 byte buffer for sending packets on
                             //EP1 IN to the PC
PacketToFromPC PacketFromPCBuffer;

USB_HANDLE USBOutHandle = 0;
USB_HANDLE USBInHandle = 0;
BOOL BL_blinkStatusValid = TRUE;

DWORD_VAL FlashMemoryValue;

unsigned char MaxPageToErase;
unsigned long ProgramMemStopAddress;
unsigned char BootState;
unsigned short ProgrammingBuffer[BufferSize];
unsigned char BufferedDataIndex;
unsigned long ProgrammedPointer;
unsigned char ConfigsProtected;
unsigned long ProgramMemStart;
// extern Stato BLState;
/*====== VARIABILI GLOBALI ================================================== */


/*====== DICHIARAZIONE FUNZIONI LOCALI ====================================== */
void BL_BlinkUSBStatus(void);
BOOL Switch2IsPressed(void);
BOOL Switch3IsPressed(void);
void Emulate_Mouse(void);
void BL_ProcessIO(void);
void UserInit(void);
void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
void EraseDeviceMaster(void);
void EraseDeviceSlave(void);
void ProgramDeviceMaster(void);
void ProgramDeviceSlave(void);
void ProgramCompleteDeviceMaster(void);
void ProgramCompleteDeviceSlave(void);
void GetDataDeviceMaster(void);
void GetDataDeviceSlave(void);
void ResetDeviceMaster(void);
void ResetDeviceSlave(void);
void Boot_FW_Request(void);
void Master_Boot_FW_Request(void);
void Jump_To_Application_Request(void);

void BootApplication(void);
void UserUSBInit(void);
void BL_USB_Init(void);

static unsigned char check_password(unsigned char *received_pwd)
{
    // pwd (ASCII): TH<x2hRd
    const unsigned char mediconPassword[PWD_LGTH] = {
        0x82, 0x14, 0x2A, 0x5D, 0x6F, 0x9A, 0x25, 0x01};

    unsigned char ret = FALSE;
    unsigned char i;

    // potevo usare una memcmp, ma non voglio occupare altra memoria
    for (i = 0; i < PWD_LGTH; i++)
    {
        if (received_pwd[i] != mediconPassword[i])
            break;
    }

    if (i == PWD_LGTH)
        ret = TRUE;

    return ret;
}

/********************************************************************
 * Function:        void BL_USB_Init(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:
 *
 * Note:            None
 *******************************************************************/
void BL_USB_Init(void)
{

    AD1PCFGL = 0xFFFF;

    //  The USB specifications require that USB peripheral devices
    //  must never source current onto the Vbus pin.  Additionally,
    //  USB peripherals should not source current on D+ or D- when the
    //  host/hub is not actively powering the Vbus line.  When
    //  designing a self powered (as opposed to bus powered) USB
    //  peripheral device, the firmware should make sure not to turn
    //  on the USB module and D+ or D- pull up resistor unless Vbus is
    //  actively powered.  Therefore, the firmware needs some means to
    //  detect when Vbus is being powered by the host.  A 5V tolerant
    //  I/O pin can be connected to Vbus (through a resistor), and can
    //  be used to detect when Vbus is high (host actively powering),
    //  or low (host is shut down or otherwise not supplying power).
    //  The USB firmware can then periodically poll this I/O pin to
    //  know when it is okay to turn on the USB module/D+/D- pull up
    //  resistor.  When designing a purely bus powered peripheral
    //  device, it is not possible to source current on D+ or D- when
    //  the host is not actively providing power on Vbus. Therefore,
    //  implementing this bus sense feature is optional.  This
    //  firmware can be made to use this bus sense feature by making
    //  sure "USE_USB_BUS_SENSE_IO" has been defined in the
    //  HardwareProfile.h file.

#if defined(USE_USB_BUS_SENSE_IO)
    tris_usb_bus_sense = INPUT_PIN; // See HardwareProfile.h
#endif

    //  If the host PC sends a GetStatus (device) request, the
    //  firmware must respond and let the host know if the USB
    //  peripheral device is currently bus powered or self powered.
    //  See chapter 9 in the official USB specifications for details
    //  regarding this request.  If the peripheral device is capable
    //  of being both self and bus powered, it should not return a
    //  hard coded value for this request.  Instead, firmware should
    //  check if it is currently self or bus powered, and respond
    //  accordingly.  If the hardware has been configured like
    //  demonstrated on the PICDEM FS USB Demo Board, an I/O pin can
    //  be polled to determine the currently selected power source.
    //  On the PICDEM FS USB Demo Board, "RA2" is used for this
    //  purpose.  If using this feature, make sure
    //  "USE_SELF_POWER_SENSE_IO" has been defined in
    //  HardwareProfile.h, and that an appropriate I/O pin has been
    //  mapped to it in HardwareProfile.h.

#if defined(USE_SELF_POWER_SENSE_IO)
    tris_self_power = INPUT_PIN;      // See HardwareProfile.h
#endif

    USBDeviceInit();    //usb_device.c.  Initializes USB module SFRs
                        //and firmware variables to known states.

    UserUSBInit();
} // end BL_USB_Init


void UserUSBInit(void)
{
    //Initialize all of the LED pins
    // mInitAllLEDs();

    //Initialize all of the push buttons
    // mInitAllSwitches();

    //initialize the variable holding the handle for the last
    // transmission
    USBOutHandle = 0;
    USBInHandle = 0;

    BL_blinkStatusValid = TRUE;

    //Initialize bootloader state variables

    //Assume we will not allow erase/programming of config words (unless
    //host sends override command)
    MaxPageToErase = MaxPageToEraseNoConfigs;


    //Assume we will not erase or program the vector table at first.
    //Must receive unlock config bits/vectors command first.
    ConfigsProtected = LOCKCONFIG;
    BootState = IdleState;
    ProgrammedPointer = InvalidAddress;
    BufferedDataIndex = 0;
} // end UserUSBInit


/********************************************************************
 * Function:        void BL_USB_ServerMg(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user
 *                  routines. It is a mixture of both USB and
 *                  non-USB tasks.
 *
 * Note:            None
 *******************************************************************/
void BL_USB_ServerMg(void)
{
    unsigned char psv_shadow;

    if ((BLState.livello == INIT) || (BLState.livello == USB_CONNECT_EXECUTION))
    {
        psv_shadow = PSVPAG;
        /* set the PSVPAG for accessing gamma_factor[] */
        PSVPAG = __builtin_psvpage (&device_dsc);
        USBDeviceTasks();   // Check bus status and service USB interrupts.
        PSVPAG = psv_shadow;
        // Interrupt or polling method.  If using polling, must call
        // this function periodically.  This function will take care
        // of processing and responding to SETUP transactions
        // (such as during the enumeration process when you first
        // plug in).  USB hosts require that USB devices should accept
        // and process SETUP packets in a timely fashion.  Therefore,
        // when using polling, this function should be called
        // frequently (such as once about every 100 microseconds) at any
        // time that a SETUP packet might reasonably be expected to
        // be sent by the host to your device.  In most cases, the
        // USBDeviceTasks() function does not take very long to
        // execute (~50 instruction cycles) before it returns.

        // Application-specific tasks.

        // Application related code may be added here, or in the
        // ProcessIO() function.
        BL_ProcessIO();
    }
}//end BL_USB_ServerMg

/********************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user
 *                  routines. It is a mixture of both USB and
 *                  non-USB tasks.
 *
 * Note:            None
 *******************************************************************/
void BL_ProcessIO(void)
{
  if(USBDeviceState == CONFIGURED_STATE) {
    BLState.livello = USB_CONNECT_EXECUTION;
    BLState.fase = USB_CONNECT_OK;
  }
  else {
    BLState.livello = INIT;
    BLState.fase = 0;
  }

  // User Application USB tasks
  if((USBDeviceState < CONFIGURED_STATE) || (USBSuspendControl==1))
    return;

  BootApplication();
  USBDeviceTasks();
} //end ProcessIO


/********************************************************************
 * Function:        void BlinkUSBStatus(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        BlinkUSBStatus turns on and off LEDs
 *                  corresponding to the USB device state.
 *
 * Note:            mLED macros can be found in HardwareProfile.h
 *                  USBDeviceState is declared and updated in
 *                  usb_device.c.
 *******************************************************************/
void BL_BlinkUSBStatus(void)
{
    static WORD led_count=0;

    if(led_count == 0)led_count = 10000U;
    led_count--;

#define mLED_Both_Off()         {mLED_1_Off();mLED_2_Off();}
#define mLED_Both_On()          {mLED_1_On();mLED_2_On();}
#define mLED_Only_1_On()        {mLED_1_On();mLED_2_Off();}
#define mLED_Only_2_On()        {mLED_1_Off();mLED_2_On();}

    if(USBSuspendControl == 1)
    {
        if(led_count==0)
        {
            // mLED_1_Toggle();
            // mLED_2 = mLED_1;        // Both blink at the same time
        }//end if
    }
    else
    {
        if(USBDeviceState == DETACHED_STATE)
        {
            // mLED_Both_Off();
        }
        else if(USBDeviceState == ATTACHED_STATE)
        {
            // mLED_Both_On();
        }
        else if(USBDeviceState == POWERED_STATE)
        {
            // mLED_Only_1_On();
        }
        else if(USBDeviceState == DEFAULT_STATE)
        {
            // mLED_Only_2_On();
        }
        else if(USBDeviceState == ADDRESS_STATE)
        {
            if(led_count == 0)
            {
                // mLED_1_Toggle();
                // mLED_2_Off();
            }//end if
        }
        else if(USBDeviceState == CONFIGURED_STATE)
        {
            if(led_count==0)
            {
                // mLED_1_Toggle();
                // mLED_2 = !mLED_1;       // Alternate blink
            }//end if
        }//end if(...)
    }//end if(UCONbits.SUSPND...)

}//end BlinkUSBStatus

/******************************************************************************
 * Function:        void BootApplication(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user routines.
 *                  It is a mixture of both USB and non-USB tasks.
 *
 * Note:            None
 *****************************************************************************/
void BootApplication(void)
{
    unsigned char i;
    unsigned int j;

    if(BootState == IdleState) {
        // Did we receive a command?
        if(!USBHandleBusy(USBOutHandle)) {
            for(i = 0; i < TotalPacketSize; i++)
                PacketFromPC.Contents[i] = PacketFromPCBuffer.Contents[i];

            USBOutHandle = USBRxOnePacket(HID_EP,(BYTE*)&PacketFromPCBuffer,64);
            BootState = NotIdleState;

            // Prepare the next packet we will send to the host, by initializing the entire packet to 0x00.
            for(i = 0; i < TotalPacketSize; i++) {
                // This saves code space, since we don't have to do it independently in the QUERY_DEVICE and GET_DATA cases.
                PacketToPC.Contents[i] = 0;
            }
        }
    }
    else if (BootState == NotIdleState) /*(BootState must be in NotIdleState)*/ {
        switch(PacketFromPC.Command) {
            case JUMP_TO_APPLICATION:		
                progBoot.IDtype = 0;	
            break;

            case BOOT_FW_VERSION_REQUEST:
                if (check_password(PacketFromPC.Password)) {
                    progBoot.IDtype = PacketFromPC.IdFwProgramming;
                    BL_slave_id = progBoot.IDtype;
        		}
            break;
		
            case QUERY_DEVICE:
            {
                if (check_password(PacketFromPC.Password))
                {
                    progBoot.IDtype = PacketFromPC.IdFwProgramming;

                    if (isMasterProgramming()) {
                        ProgramMemStopAddress = ProgramMemMasterStopNoConfigs;
                        ProgramMemStart       = START_APPL_ADDRESS_MASTER;
                    }
                    else {
                        BL_slave_id = progBoot.IDtype;
                        if (BL_slave_id == HUMIDIFIER_ID) {
                            ProgramMemStart = START_APPL_ADDRESS_HUMIDIFIER;
                            ProgramMemStopAddress = ProgramMemSlaveStopNoConfigsHumidifier;			
                        }
                        else if (BL_slave_id == TINTING_ID) {
                            ProgramMemStart = START_APPL_ADDRESS_TINTING;
                            ProgramMemStopAddress = ProgramMemSlaveStopNoConfigsTinting;			
                        }
                        else {
                            ProgramMemStart = START_APPL_ADDRESS_SLAVE;				  
                            ProgramMemStopAddress = ProgramMemSlaveStopNoConfigs;
                        }
                    }

                    // Prepare a response packet, which lets the PC software know about the memory ranges of this device
                    PacketToPC.Command = (unsigned char)QUERY_DEVICE;
                    PacketToPC.PacketDataFieldSize = \
                    (unsigned char)RequestDataBlockSize;
                    // DeviceFamily (cfr. HIDBootloader)
                    PacketToPC.BytesPerAddress = \
                    (unsigned char)BytesPerFlashAddress;
                    
                    PacketToPC.Type1 = (unsigned char)TypeProgramMemory;
                    PacketToPC.Address1 = (unsigned long)ProgramMemStart;
                    // Size of program memory area
                    PacketToPC.Length1 = \
                    (unsigned long)(ProgramMemStopAddress - ProgramMemStart);

                    PacketToPC.Type2 = (unsigned char)TypeEndOfTypeList;
                    if(ConfigsProtected == UNLOCKCONFIG) /* Only for master */ {
                        // Overwrite the 0xFF end of list indicator if we wish to program the Vectors.
                        PacketToPC.Type2 = (unsigned char)TypeProgramMemory;
                        PacketToPC.Address2 = (unsigned long)VectorsStart;

                        // Size of program memory area
                        PacketToPC.Length2 = \
                        (unsigned long)(VectorsEnd - VectorsStart);

                        PacketToPC.Type3 = (unsigned char)TypeConfigWords;
                        PacketToPC.Address3 = \
                        (unsigned long)ConfigWordsStartAddress;
                        PacketToPC.Length3 = \
                        (unsigned long)(ConfigWordsStopAddress -
                          ConfigWordsStartAddress);
                        PacketToPC.Type4 = (unsigned char)TypeEndOfTypeList;
                    }
                    //Init pad bytes to 0x00...  Already done after we received the QUERY_DEVICE command (just after calling HIDRxPacket()).
                    if(!USBHandleBusy(USBInHandle)) {
                      USBInHandle = USBTxOnePacket(HID_EP,(BYTE*)&PacketToPC,64);
                      BootState = IdleState;
                    }
                }
            }
            break;

            case UNLOCK_CONFIG: // Only for master  
            {
                if(PacketFromPC.LockValue == UNLOCKCONFIG) {
                    // Assume we will not allow erase/programming of config words (unless host sends override command)
                    MaxPageToErase = MaxPageToEraseWithConfigs;
                    // ProgramMemStopAddress = ProgramMemStopWithConfigs;
                    ConfigsProtected = UNLOCKCONFIG;
                }
                else {
                    MaxPageToErase = MaxPageToEraseNoConfigs;
                    ConfigsProtected = LOCKCONFIG;
                }
                BootState = IdleState;
            }
            break;
        } // switch()

        if(isMasterProgramming()) {
            switch(PacketFromPC.Command) {
                case BOOT_FW_VERSION_REQUEST:
                    Master_Boot_FW_Request();		  	
                break;

                case ERASE_DEVICE:
                    EraseDeviceMaster();
                break;

                case PROGRAM_DEVICE:
                    ProgramDeviceMaster();
                break;

                case PROGRAM_COMPLETE:
                    ProgramCompleteDeviceMaster();
                break;

                case GET_DATA:
                    GetDataDeviceMaster();
                break;

                case RESET_DEVICE:
                    ResetDeviceMaster();
                break;
            }
        } // if() 
        else {
            switch(PacketFromPC.Command) {
                case JUMP_TO_APPLICATION:
                    Jump_To_Application_Request();
                break;

                case BOOT_FW_VERSION_REQUEST:
                    Boot_FW_Request();
                break;

                case ERASE_DEVICE:
                    if (BL_slave_id == HUMIDIFIER_ID) {	  
                        progBoot.startAddress = __ACTS_CODE_BASE_HUMIDIFIER;
                        progBoot.numDataPack  = MAX_NUM_PACK_HUMIDIFIER;
                    }
                    else if (BL_slave_id == TINTING_ID) {	  
                        progBoot.startAddress = __ACTS_CODE_BASE_TINTING;
                        progBoot.numDataPack  = MAX_NUM_PACK_TINTING;
                    }		  
                    else {	  
                        progBoot.startAddress = __ACTS_CODE_BASE;
                        progBoot.numDataPack  = MAX_NUM_PACK;
                    }		
                    EraseDeviceSlave();
                break;

                case PROGRAM_DEVICE:
                    ProgramDeviceSlave();
                break;

                case PROGRAM_COMPLETE:
                    ProgramCompleteDeviceSlave();
                break;

                case GET_DATA:
                    GetDataDeviceSlave();
                break;

                case RESET_DEVICE:
                    ResetDeviceSlave();
                break;
            }//End switch
        }
    } // End else if
    else /* Bootstate == WaitState */ {
        if (isUART_FW_Upload_Ack()) {
           BootState = IdleState;
           resetNewProcessingMsg();
        }
        else if (isUART_Slave_FW_Version()) {
           PacketToPC.FW_Ver_ZZ = (unsigned char)BL_SLAVE_VERSION[0];
           PacketToPC.FW_Ver_YY = (unsigned char)BL_SLAVE_VERSION[1];
           PacketToPC.FW_Ver_XX = (unsigned char)BL_SLAVE_VERSION[2];
           USBInHandle = USBTxOnePacket(HID_EP, (BYTE*)&PacketToPC.Contents[0],64);
           BootState = IdleState;
           resetNewProcessingMsg();
        }	
        else if (isUART_Master_FW_Version()) {
           BL_Master_Version = 0;
           PacketToPC.FW_Ver_ZZ = (unsigned char)BL_MASTER_VERSION[0];
           PacketToPC.FW_Ver_YY = (unsigned char)BL_MASTER_VERSION[1];
           PacketToPC.FW_Ver_XX = (unsigned char)BL_MASTER_VERSION[2];
           USBInHandle = USBTxOnePacket(HID_EP, (BYTE*)&PacketToPC.Contents[0],64);
           BootState = IdleState;
           resetNewProcessingMsg();
        }		  
        else if (isUART_Programming_Data()) {
           for (i = 0; i < BYTES2WORDS(RequestDataBlockSize); ++ i) {
             PacketToPC.Data[i]= progBoot.bufferData[i];
           }
           USBInHandle = USBTxOnePacket(HID_EP, (BYTE*)&PacketToPC.Contents[0],64);
           BootState = IdleState;
           resetNewProcessingMsg();
        }
        else if (isEndProgramming()) {
            //Disable USB module
            U1CON = 0x0000;
            for(j = 0; j < 0xFFFF; j++)
              Nop();
            Reset();
        }
        //Call USBDriverService() periodically to prevent
        //falling off the bus if any SETUP packets should
        //happen to arrive.
        USBDeviceTasks();
    }

}//End BootApplication()

// *****************************************************************************
// ************** USB Callback Functions ***************************************
// *****************************************************************************

// The USB firmware stack will call the callback functions USBCBxxx()
// in response to certain USB related events.  For example, if the
// host PC is powering down, it will stop sending out Start of Frame
// (SOF) packets to your device.  In response to this, all USB devices
// are supposed to decrease their power consumption from the USB Vbus
// to <2.5mA each.  The USB module detects this condition (which
// according to the USB specifications is 3+ms of no bus activity/SOF
// packets) and then calls the USBCBSuspend() function.  You should
// modify these callback functions to take appropriate actions for
// each of these conditions.  For example, in the USBCBSuspend(), you
// may wish to add code that will decrease power consumption from Vbus
// to <2.5mA (such as by clock switching, turning off LEDs, putting
// the microcontroller to sleep, etc.).  Then, in the
// USBCBWakeFromSuspend() function, you may then wish to add code that
// undoes the power saving things done in the USBCBSuspend() function.

// The USBCBSendResume() function is special, in that the USB stack
// will not automatically call this function.  This function is meant
// to be called from the application firmware instead.  See the
// additional comments near the function.

/******************************************************************************
 * Function:        void USBCBSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Call back that is invoked when a USB suspend is detected
 *
 * Note:            None
 *****************************************************************************/
void USBCBSuspend(void)
{
    //Example power saving code.  Insert appropriate code here for the
    //desired application behavior.  If the microcontroller will be
    //put to sleep, a process similar to that shown below may be used:

#if 0
    ConfigureIOPinsForLowPower();
    SaveStateOfAllInterruptEnableBits();
    DisableAllInterruptEnableBits();

    //should enable at least USBActivityIF as a wake source
    EnableOnlyTheInterruptsWhichWillBeUsedToWakeTheMicro();
    Sleep();

    //Preferrably, this should be done in the USBCBWakeFromSuspend()
    //function instead.
    RestoreStateOfAllPreviouslySavedInterruptEnableBits();

    //Preferrably, this should be done in the USBCBWakeFromSuspend()
    //function instead.
    RestoreIOPinsToNormal();
#endif

    //IMPORTANT NOTE: Do not clear the USBActivityIF (ACTVIF) bit
    //here.  This bit is cleared inside the usb_device.c file.
    //Clearing USBActivityIF here will cause things to not work as
    //intended.

#if defined(__C30__)
#if 0
    U1EIR = 0xFFFF;
    U1IR = 0xFFFF;
    U1OTGIR = 0xFFFF;
    IFS5bits.USB1IF = 0;
    IEC5bits.USB1IE = 1;
    U1OTGIEbits.ACTVIE = 1;
    U1OTGIRbits.ACTVIF = 1;
    TRISA &= 0xFF3F;
    LATAbits.LATA6 = 1;
    Sleep();
    LATAbits.LATA6 = 0;
#endif
#endif
}


/******************************************************************************
 * Function:        void _USB1Interrupt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the USB interrupt bit is set
 *                  In this example the interrupt is only used when
 *                  the device goes to sleep when it receives a USB
 *                  suspend command
 *
 * Note:            None
 *****************************************************************************/
#if 0
void __attribute__ ((interrupt)) _USB1Interrupt(void)
{
#if !defined(self_powered)
if(U1OTGIRbits.ACTVIF)
{
    LATAbits.LATA7 = 1;

    IEC5bits.USB1IE = 0;
    U1OTGIEbits.ACTVIE = 0;
    IFS5bits.USB1IF = 0;

    //USBClearInterruptFlag(USBActivityIFReg,USBActivityIFBitNum);
    USBClearInterruptFlag(USBIdleIFReg,USBIdleIFBitNum);
    //USBSuspendControl = 0;
    LATAbits.LATA7 = 0;
}
#endif
}
#endif

/******************************************************************************
 * Function:        void USBCBWakeFromSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The host may put USB peripheral devices in low power
 *                  suspend mode (by "sending" 3+ms of idle).  Once in
 *                  suspend mode, the host may wake the device back up
 *                  by sending non- idle state signalling.
 *
 *                  This call back is invoked when a wakeup from USB
 *                  suspend is detected.
 *
 * Note:            None
 *****************************************************************************/
void USBCBWakeFromSuspend(void)
{
    // If clock switching or other power savings measures were taken
    // when executing the USBCBSuspend() function, now would be a good
    // time to switch back to normal full power run mode conditions.
    // The host allows a few milliseconds of wakeup time, after which
    // the device must be fully back to normal, and capable of
    // receiving and processing USB packets.  In order to do this, the
    // USB module must receive proper clocking (IE: 48MHz clock must
    // be available to SIE for full speed USB operation).
}

/********************************************************************
 * Function:        void USBCB_SOF_Handler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB host sends out a SOF packet to full-speed
 *                  devices every 1 ms. This interrupt may be useful
 *                  for isochronous pipes. End designers should
 *                  implement callback routine as necessary.
 *
 * Note:            None
 *******************************************************************/
void USBCB_SOF_Handler(void)
{
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.
}

/*******************************************************************
 * Function:        void USBCBErrorHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The purpose of this callback is mainly for
 *                  debugging during development. Check UEIR to see
 *                  which error causes the interrupt.
 *
 * Note:            None
 *******************************************************************/
void USBCBErrorHandler(void)
{
    // No need to clear UEIR to 0 here.
    // Callback caller is already doing that.

    // Typically, user firmware does not need to do anything special
    // if a USB error occurs.  For example, if the host sends an OUT
    // packet to your device, but the packet gets corrupted (ex:
    // because of a bad connection, or the user unplugs the USB cable
    // during the transmission) this will typically set one or more
    // USB error interrupt flags.  Nothing specific needs to be done
    // however, since the SIE will automatically send a "NAK" packet
    // to the host.  In response to this, the host will normally retry
    // to send the packet again, and no data loss occurs.  The system
    // will typically recover automatically, without the need for
    // application firmware intervention.

    // Nevertheless, this callback function is provided, such as
    // for debugging purposes.
}


/*******************************************************************
 * Function:        void USBCBCheckOtherReq(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        When SETUP packets arrive from the host, some firmware
 *                  must process the request and respond appropriately
 *                  to fulfill the request.  Some of the SETUP packets
 *                  will be for standard USB "chapter 9" (as in,
 *                  fulfilling chapter 9 of the official USB
 *                  specifications) requests, while others may be
 *                  specific to the USB device class that is being
 *                  implemented.  For example, a HID class device
 *                  needs to be able to respond to "GET REPORT" type
 *                  of requests.  This is not a standard USB chapter 9
 *                  request, and therefore not handled by
 *                  usb_device.c.  Instead this request should be
 *                  handled by class specific firmware, such as that
 *                  contained in usb_function_hid.c.
 *
 * Note:            None
 *******************************************************************/
void USBCBCheckOtherReq(void)
{
    USBCheckHIDRequest();
}//end


/*******************************************************************
 * Function:        void USBCBStdSetDscHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USBCBStdSetDscHandler() callback function is called
 *                  when a SETUP, bRequest: SET_DESCRIPTOR request
 *                  arrives.  Typically SET_DESCRIPTOR requests are
 *                  not used in most applications, and it is optional
 *                  to support this type of request.
 *
 * Note:            None
 *******************************************************************/
void USBCBStdSetDscHandler(void)
{
    // Must claim session ownership if supporting this request
}//end


/*******************************************************************
 * Function:        void USBCBInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the device becomes
 *                  initialized, which occurs after the host sends a
 *                  SET_CONFIGURATION (wValue not = 0) request.  This
 *                  callback function should initialize the endpoints
 *                  for the device's usage according to the current
 *                  configuration.
 *
 * Note:            None
 *******************************************************************/
void USBCBInitEP(void)
{
    //enable the HID endpoint
    USBEnableEndpoint(HID_EP,USB_IN_ENABLED |
                      USB_OUT_ENABLED |
                      USB_HANDSHAKE_ENABLED |
                      USB_DISALLOW_SETUP);

    //Arm the OUT endpoint for the first packet
    USBOutHandle = HIDRxPacket(HID_EP,(BYTE*)&PacketFromPCBuffer,64);
}

/********************************************************************
 * Function:        void USBCBSendResume(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB specifications allow some types of USB peripheral
 *                  devices to wake up a host PC (such as if it is in
 *                  a low power suspend to RAM state).  This can be a
 *                  very useful feature in some USB applications, such
 *                  as an Infrared remote control receiver.  If a user
 *                  presses the "power" button on a remote control, it
 *                  is nice that the IR receiver can detect this
 *                  signalling, and then send a USB "command" to the
 *                  PC to wake up.
 *
 *                  The USBCBSendResume() "callback" function is used
 *                  to send this special USB signalling which wakes up
 *                  the PC.  This function may be called by
 *                  application firmware to wake up the PC.  This
 *                  function should only be called when:
 *
 *                  1.  The USB driver used on the host PC supports
 *                      the remote wakeup capability.
 *                  2.  The USB configuration descriptor indicates the
 *                      device is remote wakeup capable in the
 *                      bmAttributes field.
 *                  3.  The USB host PC is currently sleeping, and has
 *                      previously sent your device a SET FEATURE
 *                      setup packet which "armed" the remote wakeup
 *                      capability.
 *
 *                  This callback should send a RESUME signal that has
 *                  the period of 1-15ms.
 *
 * Note:            Interrupt vs. Polling
 *                  -Primary clock
 *                  -Secondary clock ***** MAKE NOTES ABOUT THIS *******
 *                   > Can switch to primary first by calling
 *                   USBCBWakeFromSuspend()
 *
 *                  The modifiable section in this routine should be changed
 *                  to meet the application needs. Current implementation
 *                  temporary blocks other functions from executing for a
 *                  period of 1-13 ms depending on the core frequency.
 *
 *                  According to USB 2.0 specification section 7.1.7.7,
 *                  "The remote wakeup device must hold the resume signaling
 *                  for at lest 1 ms but for no more than 15 ms."
 *                  The idea here is to use a delay counter loop, using a
 *                  common value that would work over a wide range of core
 *                  frequencies.
 *                  That value selected is 1800. See table below:
 *                  ==========================================================
 *                  Core Freq(MHz)      MIP         RESUME Signal Period (ms)
 *                  ==========================================================
 *                      48              12          1.05
 *                       4              1           12.6
 *                  ==========================================================
 *                  * These timing could be incorrect when using code
 *                    optimization or extended instruction mode,
 *                    or when having other interrupts enabled.
 *                    Make sure to verify using the MPLAB SIM's Stopwatch
 *                    and verify the actual signal on an oscilloscope.
 *******************************************************************/
void USBCBSendResume(void)
{
    static WORD delay_count;

    USBResumeControl = 1;                // Start RESUME signaling

    delay_count = 1800U;                // Set RESUME line for 1-13 ms
    do
    {
        delay_count--;
    }while(delay_count);
    USBResumeControl = 0;
}

void EraseDeviceMaster(void)
/*******************************************************************************
**  @brief  Procedure to erase MASTER FLASH
**
**  @param  void
**
**  @retval void
**
*******************************************************************************/
{
    unsigned char i;
    // Temporaneamente posizionato in questo punto ??? 
    BL_StandAlone = BL_STAND_ALONE;
    DISABLE_WDT();

    for (i = BeginPageToErase; i <= MaxPageToErase; ++ i)  {
        EraseFlashPage(i);
        // Call USBDriverService() periodically to prevent falling off the bus if any SETUP packets should happen to arrive
        USBDeviceTasks();
    }

    if (ConfigsProtected == UNLOCKCONFIG) {
        // Erase the Vectors separately
        TBLPAG = 0x0000;
        __builtin_tblwtl(0x0000, 0xFFFF);
        // Disable interrupts for next few instructions for unlock sequence 
        asm("DISI #12");
        __builtin_write_NVM();
    }
    
    // Good practice to clear WREN bit anytime we are not expecting to do erase/write operations, further reducing probability of accidental activation
    NVMCONbits.WREN = 0;
    BootState = IdleState;
    ENABLE_WDT();
}

void EraseDeviceSlave(void)
/*******************************************************************************
**  @brief  Procedure to erase SLAVE FLASH
**
**  @param  void
**
**  @retval void
**
*******************************************************************************/
{
    progBoot.idDataPackTx = 0;
    setBootMessage(CMD_FW_UPLOAD);
    BootState = WaitState;
}

void ProgramDeviceMaster(void)
/*******************************************************************************
**  @brief  Procedure to program MASTER FLASH
**
**  @param  void
**
**  @retval void
**
*******************************************************************************/
{
    unsigned char i;

    if (ProgrammedPointer == (unsigned long)InvalidAddress)
        ProgrammedPointer = PacketFromPC.Address;

    if (ProgrammedPointer == (unsigned long)PacketFromPC.Address) {
        DISABLE_WDT();
        for (i = 0; i < PacketFromPC.Size/WORDSIZE; ++ i) {
            // Data field is right justified. Need to put it in the buffer left justified
            ProgrammingBuffer[BufferedDataIndex] = \
            PacketFromPC.Data[(RequestDataBlockSize -
              PacketFromPC.Size)/WORDSIZE + i];

            BufferedDataIndex ++;
            ProgrammedPointer ++;

            // Need to make sure it doesn't call WriteFlashSubBlock() unless BufferedDataIndex/2 is an integer
            if (BufferedDataIndex == (RequestDataBlockSize/WORDSIZE)) {
              WriteFlashSubBlock(ProgrammedPointer-BufferedDataIndex,
                                 BufferedDataIndex,ProgrammingBuffer);
              BufferedDataIndex = 0;
            }
        }
        ENABLE_WDT();
    }
    // else host sent us a non-contiguous packet address... to make this firmware simpler, host should not do this without 
    // sending a PROGRAM_COMPLETE command in between program sections. */
    BootState = IdleState;
}


void ProgramDeviceSlave(void)
/*******************************************************************************
**  @brief  Procedure to program SLAVE FLASH
**
**  @param  void
**
**  @retval void
**
*******************************************************************************/
{
    unsigned char i;
    if (ProgrammedPointer == (unsigned long)InvalidAddress)
      ProgrammedPointer = PacketFromPC.Address;

    if (ProgrammedPointer == (unsigned long)PacketFromPC.Address) {
        progBoot.idDataPackTx ++;
        progBoot.address = (unsigned long)PacketFromPC.Address;
        progBoot.numDataBytesPack = PacketFromPC.Size;
        for(i = 0; i < BYTES2WORDS(PacketFromPC.Size); ++ i) {
          progBoot.bufferData[i] = PacketFromPC.Data[BYTES2WORDS(RequestDataBlockSize -
                                                                 PacketFromPC.Size) + i];
          ++ ProgrammedPointer;
          ++ BufferedDataIndex;
        }
        setBootMessage(DATA_FW_UPLOAD);
        BootState = WaitState;
        BufferedDataIndex = 0;
    }
    else
        BootState = IdleState;
}

void ProgramCompleteDeviceMaster(void)
/*******************************************************************************
**  @brief  Procedure to Program Complete packet (MASTER FLASH)
**
**  @param  void
**
**  @retval void
**
*******************************************************************************/
{
    DISABLE_WDT();
    WriteFlashSubBlock(ProgrammedPointer-BufferedDataIndex,
                       BufferedDataIndex,ProgrammingBuffer);

    BufferedDataIndex = 0;

    // Reinitialize pointer to an invalid range, so we know the next
    // PROGRAM_DEVICE will be the start address of a contiguous section
    ProgrammedPointer = InvalidAddress;
    BootState = IdleState;
    ENABLE_WDT();
}

void ProgramCompleteDeviceSlave(void)
/*******************************************************************************
**  @brief  Procedure to Program Complete packet (SLAVE FLASH)
**
**  @param  void
**
**  @retval void
**
*******************************************************************************/
{
    unsigned char i;

    ProgrammedPointer = InvalidAddress;
    progBoot.idDataPackTx ++;
    progBoot.address = (unsigned long)PacketFromPC.Address;
    progBoot.numDataBytesPack = PacketFromPC.Size;

    for(i = 0; i < BYTES2WORDS(PacketFromPC.Size); ++ i) {
        progBoot.bufferData[i] = PacketFromPC.Data[(RequestDataBlockSize -
          PacketFromPC.Size)/WORDSIZE+i];
    }
    setBootMessage(END_DATA_FW_UPLOAD);
    BootState = WaitState;
}


void GetDataDeviceMaster(void)
/*******************************************************************************
**  @brief  Procedure to Get Data packet (MASTER FLASH)
**
**  @param  void
**
**  @retval void
**
*******************************************************************************/
{
    unsigned char i;

    if(!USBHandleBusy(USBInHandle)) {
        // Init pad bytes to 0x00...  Already done after we received the QUERY_DEVICE command (just after calling HIDRxReport()).
        PacketToPC.Command = GET_DATA;
        PacketToPC.Address = PacketFromPC.Address;
        PacketToPC.Size = PacketFromPC.Size;

        for(i = 0; i < (PacketFromPC.Size/2); i=i+2) {
            FlashMemoryValue = \
            (DWORD_VAL) ReadProgramMemory(PacketFromPC.Address + i);

            // Low word, pure 16-bits of real data
            PacketToPC.Data[RequestDataBlockSize/WORDSIZE + i -
            PacketFromPC.Size/WORDSIZE] = \
            FlashMemoryValue.word.LW;
            // Set the "phantom byte" = 0x00, since this is what is in the .HEX file generatd by MPLAB.
            FlashMemoryValue.byte.MB = 0x00;
            // Needs to be 0x00 so as to match, and successfully verify, even though the actual table read yeilded 0xFF for this phantom byte.
            // Upper word, which contains the phantom byte
            PacketToPC.Data[RequestDataBlockSize/WORDSIZE + i + 1 -
            PacketFromPC.Size/WORDSIZE] = \
            FlashMemoryValue.word.HW;
        }
        USBInHandle = USBTxOnePacket(HID_EP, (BYTE*)&PacketToPC.Contents[0],64);
        BootState = IdleState;
    }
}

void GetDataDeviceSlave(void)
/*******************************************************************************
**  @brief  Procedure to Get Data packet (SLAVE FLASH)
**
**  @param  void
**
**  @retval void
**
*******************************************************************************/
{
    PacketToPC.Command = GET_DATA;
    PacketToPC.Address = PacketFromPC.Address;
    PacketToPC.Size = PacketFromPC.Size;
    progBoot.numDataBytesPack = PacketFromPC.Size;
    progBoot.address = (unsigned long)PacketFromPC.Address;
    setBootMessage(GET_PROGRAM_DATA);
    BootState = WaitState;
}

void ResetDeviceMaster(void)
/*******************************************************************************
**  @brief  Procedure to Reset packet (MASTER FLASH)
**
**  @param  void
**
**  @retval void
**
*******************************************************************************/
{
    unsigned i;
    DISABLE_WDT();

    // Temporaneamente posizionato in questo punto la scrittura della locazione all'indirizzo 0x1400 che
    // definisce la condizione di applicazione presente o no*/

    // Disable USB module

    if (ReadProgramMemory(BL_STAND_ALONE_CHECK) != APPL_FLASH_MEMORY_ERASED_VALUE) {
        // Applicativo presente, invio comando di JUMP_APPLICATIVO
        U1CON = 0x0000;
        // And wait awhile for the USB cable capacitance to discharge down to disconnected (SE0) state.  Otherwise host might not realize we
        // disconnected/reconnected when we do the reset. */
        for (i = 0; i < 0xFFFF; ++ i)
            Nop();
        jump_to_appl();
    }
    else  {
        // Applicativo NON presente
        ENABLE_WDT(); //No point in re-enabling it here...
    }
}

void ResetDeviceSlave(void)
/*******************************************************************************
**  @brief  Procedure to Reset packet (DEVICE FLASH)
**
**  @param  void
**
**  @retval void
**
*******************************************************************************/
{
    setBootMessage(RESET_SLAVE);
    StartTimer(T_SEND_RESET_MSG);

    progBoot.numSendingResetDeviceCmd = INIT_NUM_SENDING_RESET_DEVICE_CMD;

    BootState = WaitState;
}

void Boot_FW_Request(void)
/*******************************************************************************
**  @brief  Procedure to Request FW Version of SLAVE 
**
**  @param  void
**
**  @retval void
**
*******************************************************************************/
{
    BootState = WaitState;
    BL_SLAVE_VERSION[0] = 0xFF;
    BL_SLAVE_VERSION[1] = 0xFF;
    BL_SLAVE_VERSION[2] = 0xFF;
    Fw_Request = TRUE;
    StartTimer(T_SLAVE_WINDOW_TIMER);    
    setBootMessage(CMD_FRMWR_REQUEST);
}

void Master_Boot_FW_Request(void)		  	
/*******************************************************************************
**  @brief  Procedure to Request FW TINTING MASTER Version  
**
**  @param  void
**
**  @retval void
**
*******************************************************************************/
{
    BootState = WaitState;
    BL_MASTER_VERSION[0] = (unsigned short)(BL_SW_VERSION & 0x0000FF);
    BL_MASTER_VERSION[1] = (unsigned short)((BL_SW_VERSION & 0x00FF00)>>8);
    BL_MASTER_VERSION[2] = (unsigned short)((BL_SW_VERSION & 0xFF0000)>>16);
    BL_Master_Version = 1;
}

void Jump_To_Application_Request(void)
/*******************************************************************************
**  @brief  Procedure to Request Jump To Application program
**
**  @param  void
**
**  @retval void
**
*******************************************************************************/
{
    BootState = IdleState;    
    // Application Program is present, JUMP TO APPLICATION can be done
    if (CheckApplPres(BL_STAND_ALONE_CHECK) == BL_NO_STAND_ALONE) {        
        BLState.livello = JMP_TO_APP;
        StopTimer(T_WAIT_FORCE_BL);  
        StartTimer(T_WAIT_FORCE_BL);  
    }
}
