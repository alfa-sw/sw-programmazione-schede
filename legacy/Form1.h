/*********************************************************************
 *
 *                Microchip USB HID Bootloader Version 2.3
* 
 *********************************************************************
 * FileName:        Form1.h
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the Company) is intended and
 * supplied to you, the Companys customer, for use solely and
 * exclusively with Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN AS IS CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ********************************************************************

 Change History:
  Rev   Date         Description
  1.0   05/21/2008   Initial release: PIC18 devices supported
  2.2	06/10/2008	 Update to add PIC24F USB device support
  2.3	08/11/2008	 Update to add PIC32MX USB device support.
					 Minor TryToFindHIDDeviceFromVIDPID() update
					 for improved error handling.  Updated to fix
					 error programming non-J flash PIC18 devices.
 ********************************************************************/

#pragma once
#include <Windows.h>	//Gives us defitions for various common and not so common types like DWORD, PCHAR, HANDLE, etc.
#include <setupapi.h>	//Gives us definitions needed to use the SetupDixxx() functions.
#include <Dbt.h>		//Need this for definitions of WM_DEVICECHANGE messages

#include <ctime>
#include <sstream>
#include <iostream>

#include <msclr\marshal_cppstd.h>

#pragma region Constants
 //Modify this value to match the VID and PID in your USB device descriptor.
 //Use the formatting: "Vid_xxxx&Pid_xxxx" where xxxx is a 16-bit hexadecimal number.
#define MY_DEVICE_ID  "Vid_04d8&Pid_003c"	

//*********************** BOOTLOADER COMMANDS ******************************
#define QUERY_DEVICE				0x02
#define UNLOCK_CONFIG				0x03
#define ERASE_DEVICE				0x04
#define PROGRAM_DEVICE				0x05
#define PROGRAM_COMPLETE			0x06
#define GET_DATA					0x07
#define RESET_DEVICE				0x08
#define GET_ENCRYPTED_FF			0xFF
//**************************************************************************

//*********************** QUERY RESULTS ************************************
#define QUERY_IDLE					0xFF
#define QUERY_RUNNING				0x00
#define QUERY_SUCCESS				0x01
#define QUERY_WRITE_FILE_FAILED		0x02
#define QUERY_READ_FILE_FAILED		0x03
#define QUERY_MALLOC_FAILED			0x04
//**************************************************************************

//*********************** PROGRAMMING RESULTS ******************************
#define PROGRAM_IDLE				0xFF
#define PROGRAM_RUNNING				0x00
#define PROGRAM_SUCCESS				0x01
#define PROGRAM_WRITE_FILE_FAILED	0x02
#define PROGRAM_READ_FILE_FAILED	0x03
#define PROGRAM_RUNNING_ERASE		0x05
#define PROGRAM_RUNNING_PROGRAM		0x06
//**************************************************************************

//*********************** ERASE RESULTS ************************************
#define ERASE_IDLE					0xFF
#define ERASE_RUNNING				0x00
#define ERASE_SUCCESS				0x01
#define ERASE_WRITE_FILE_FAILED		0x02
#define ERASE_READ_FILE_FAILED		0x03
#define ERASE_VERIFY_FAILURE		0x04
#define ERASE_POST_QUERY_FAILURE	0x05
#define ERASE_POST_QUERY_RUNNING	0x06
#define ERASE_POST_QUERY_SUCCESS	0x07
//**************************************************************************

//*********************** VERIFY RESULTS ***********************************
#define VERIFY_IDLE					0xFF
#define VERIFY_RUNNING				0x00
#define VERIFY_SUCCESS				0x01
#define VERIFY_WRITE_FILE_FAILED	0x02
#define VERIFY_READ_FILE_FAILED		0x03
#define VERIFY_MISMATCH_FAILURE		0x04
//**************************************************************************

//*********************** READ RESULTS *************************************
#define READ_IDLE					0xFF
#define READ_RUNNING				0x00
#define READ_SUCCESS				0x01
#define READ_READ_FILE_FAILED		0x02
#define READ_WRITE_FILE_FAILED		0x03
//**************************************************************************

//*********************** UNLOCK CONFIG RESULTS ****************************
#define UNLOCK_CONFIG_IDLE			0xFF
#define UNLOCK_CONFIG_RUNNING		0x00
#define UNLOCK_CONFIG_SUCCESS		0x01
#define UNLOCK_CONFIG_FAILURE		0x02
//**************************************************************************

//*********************** BOOTLOADER STATES ********************************
#define BOOTLOADER_IDLE				0xFF
#define BOOTLOADER_QUERY			0x00
#define BOOTLOADER_PROGRAM			0x01
#define BOOTLOADER_ERASE			0x02
#define BOOTLOADER_VERIFY			0x03
#define BOOTLOADER_READ				0x04
#define BOOTLOADER_UNLOCK_CONFIG	0x05
#define BOOTLOADER_RESET			0x06
//**************************************************************************

//*********************** RESET RESULTS ************************************
#define RESET_IDLE					0xFF
#define RESET_RUNNING				0x00
#define RESET_SUCCESS				0x01
#define RESET_WRITE_FILE_FAILED		0x02
//**************************************************************************

//*********************** MEMORY REGION TYPES ******************************
#define MEMORY_REGION_PROGRAM_MEM	0x01
#define MEMORY_REGION_EEDATA		0x02
#define MEMORY_REGION_CONFIG		0x03
#define MEMORY_REGION_END			0xFF
//**************************************************************************

//*********************** HEX FILE CONSTANTS *******************************
#define HEX_FILE_EXTENDED_LINEAR_ADDRESS 0x04
#define HEX_FILE_EOF 0x01
#define HEX_FILE_DATA 0x00

//This is the number of bytes per line of the 
#define HEX_FILE_BYTES_PER_LINE 16
//**************************************************************************

//*********************** Device Family Definitions ************************
#define DEVICE_FAMILY_PIC18		1
#define DEVICE_FAMILY_PIC24		2
#define DEVICE_FAMILY_PIC32		3
//**************************************************************************

//*********************** Device to Program ********************************
#define DEVICE_MAB         		    0xFF
#define DEVICE_B1        			0x1
#define DEVICE_B2        		    0x2
#define DEVICE_B3        			0x3
#define DEVICE_B4        			0x4
#define DEVICE_B5        			0x5
#define DEVICE_B6        			0x6
#define DEVICE_B7        			0x7
#define DEVICE_B8        			0x8
#define DEVICE_C1        			0x9
#define DEVICE_C2        			0xA
#define DEVICE_C3        			0xB
#define DEVICE_C4        			0xC
#define DEVICE_C5        			0xD
#define DEVICE_C6        			0xE
#define DEVICE_C7        			0xF
#define DEVICE_C8        			0x10
#define DEVICE_C9        			0x11
#define DEVICE_C10        			0x12
#define DEVICE_C11        			0x13
#define DEVICE_C12        			0x14
#define DEVICE_C13        			0x15
#define DEVICE_C14        			0x16
#define DEVICE_C15        			0x17
#define DEVICE_C16        			0x18
#define DEVICE_C17        			0x19
#define DEVICE_C18        			0x1A
#define DEVICE_C19        			0x1B
#define DEVICE_C20       			0x1C
#define DEVICE_C21       			0x1D
#define DEVICE_C22       			0x1E
#define DEVICE_C23       			0x1F
#define DEVICE_C24       			0x20
#define DEVICE_X_AXIS       		0x21
#define DEVICE_Y_AXIS       		0x22
#define DEVICE_CONTAINER_1          0x23
#define DEVICE_CONTAINER_2          0x24
#define DEVICE_CONTAINER_3          0x25
#define DEVICE_CONTAINER_4          0x26
#define DEVICE_COVER_1				0x27
#define DEVICE_COVER_2				0x28
#define DEVICE_AUTOCAP				0x29
#define DEVICE_CAN_LIFTER           0x2A
#define DEVICE_HUMIDIFIER           0x2B
#define DEVICE_TINTING	            0x2C
#define DEVICE_UNUSED_4             0x2D
#define DEVICE_UNUSED_5             0x2E
#define DEVICE_UNUSED_6             0x2F
#define DEVICE_UNUSED_7             0x30
//**************************************************************************

#define PIC24_RESET_REMAP_OFFSET 0x1400
#define MAX_DATA_REGIONS 6

#define ID_FW_LGTH      (1)

#define USE_PASSWORD

#ifdef USE_PASSWORD
#define PWD_LGTH 8

#undef MY_DEVICE_ID
#define MY_DEVICE_ID  "Vid_04d8&Pid_e89b"
#endif

#define PACK_NULL  (0)

#pragma endregion

#pragma region Macros

#define DEBUGGING
//#define DEBUG_BUTTONS
//#define DEBUG_THREADS
//#define DEBUG_USB

//#define DONT_VERIFY_NONPROGRAMMED_ADDRESSES
//#define ENCRYPTED_BOOTLOADER

#if defined(DEBUGGING)
#define DEBUG_OUT(s) {tmr_ThreadStatus->Enabled = false;listBox1->Items->Add(s);listBox1->SelectedIndex = listBox1->Items->Count - 1;tmr_ThreadStatus->Enabled = true;}
#define DEBUG_PRINT_BUFFER(buffer,size) printBuffer(buffer,size)
#else
#define DEBUG_OUT(s)
#define DEBUG_PRINT_BUFFER(a,b)
#endif

#define DISABLE_PRINT()   do { enablePrint = false; } while (0)
#define ENABLE_PRINT()    do { enablePrint = true;  } while (0)
#define PRINT_STATUS(s)   do { addLogEntry((s));    } while (0)
	
#pragma endregion

#pragma region Type Definitions

#pragma pack(1)
typedef struct _MEMORY_REGION
{
	unsigned char Type;
	DWORD Address;
	DWORD Size;
}MEMORY_REGION;

typedef union _BOOTLOADER_COMMAND
{
	struct
	{
		unsigned char WindowsReserved;
		unsigned char Command;
		unsigned char Pad[63];
	}EnterBootloader;
	struct
	{
		unsigned char WindowsReserved;
		unsigned char Command;
#ifdef USE_PASSWORD
		unsigned char Password[PWD_LGTH];
		unsigned char IdFwProgramming;
		unsigned char Pad[63 - PWD_LGTH - ID_FW_LGTH];
#else
		unsigned char IdFwProgramming;
		unsigned char Pad[63 - ID_FW_LGTH];
#endif
	}QueryDevice;
	struct
	{
		unsigned char WindowsReserved;
		unsigned char Command;
		unsigned char BytesPerPacket;
		unsigned char DeviceFamily;
		MEMORY_REGION MemoryRegions[MAX_DATA_REGIONS];
		unsigned char Pad[8];
	}QueryResults;
	struct
	{
		unsigned char WindowsReserved;
		unsigned char Command;
		unsigned char Setting;
		unsigned char Pad[62];
	}UnlockConfig;
	struct
	{
		unsigned char WindowsReserved;
		unsigned char Command;
		unsigned char Pad[63];
	}EraseDevice;
	struct
	{
		unsigned char WindowsReserved;
		unsigned char Command;
		DWORD Address;
		unsigned char BytesPerPacket;
		unsigned char Data[58];
	}ProgramDevice;
	struct
	{
		unsigned char WindowsReserved;
		unsigned char Command;
		unsigned char Pad[63];
	}ProgramComplete;
	struct
	{
		unsigned char WindowsReserved;
		unsigned char Command;
		DWORD Address;
		unsigned char BytesPerPacket;
		unsigned char Pad[58];
	}GetData;
	struct
	{
		unsigned char WindowsReserved;
		unsigned char Command;
		DWORD Address;
		unsigned char BytesPerPacket;
		unsigned char Data[58];
	}GetDataResults;
	struct
	{
		unsigned char WindowsReserved;
		unsigned char Command;
		unsigned char Pad[63];
	}ResetDevice;
	struct
	{
		unsigned char WindowsReserved;
		unsigned char Command;
		unsigned char blockSize;
		unsigned char Data[63];
	}GetEncryptedFFResults;
	struct
	{
		unsigned char WindowsReserved;
		unsigned char Data[64];
	}PacketData;
	unsigned char RawData[65];
} BOOTLOADER_COMMAND;
#pragma pack()

#pragma endregion

unsigned char encryptionBlockSize;
unsigned char encryptedFF[64];
unsigned char deviceID;
unsigned int Enable_Functions;
unsigned int Disable_ProgramVerify;

namespace HIDBootLoader {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Text;

	using namespace System::Runtime::InteropServices;	//Need this to support "unmanaged" code.
	using namespace System::Threading;

#pragma region DLL Imports

	/*
	In order to use these unmanaged functions from within the managed .NET environment, we need
	to explicitly import the functions which we will be using from other .DLL file(s).  Simply
	including the appropriate header files is not enough.

	Note: In order to avoid potential name conflicts in the header files (which we still use),
	I have renamed the functions by adding "UM" (unmanaged) onto the end of them.  To find
	documentation for the functions in MSDN, search for the function name without the extra
	"UM" attached.
	Note2: In the header files (such as setupapi.h), normally the function names are
	remapped, depending upon if UNICODE is defined or not.  For example, two versions of the
	function SetupDiGetDeviceInterfaceDetail() exist.  One for UNICODE, and one for ANSI.
	If the wrong version of the function is called, things won't work correctly.  Therefore,
	in order to make sure the correct one gets called (based on your compiler settings, which
	may or may not define "UNICODE"), it is useful to explicity specify the CharSet when doing
	the DLL import.
	*/

#ifdef UNICODE
#define	Seeifdef	Unicode
#else
#define Seeifdef	Ansi
#endif

	//Returns a HDEVINFO type for a device information set (USB HID devices in
	//our case).  We will need the HDEVINFO as in input parameter for calling many of
	//the other SetupDixxx() functions.
	[DllImport("setupapi.dll", CharSet = CharSet::Seeifdef, EntryPoint = "SetupDiGetClassDevs")]
	extern "C" HDEVINFO  SetupDiGetClassDevsUM(
		LPGUID  ClassGuid,					//Input: Supply the class GUID here. 
		PCTSTR  Enumerator,					//Input: Use NULL here, not important for our purposes
		HWND  hwndParent,					//Input: Use NULL here, not important for our purposes
		DWORD  Flags);						//Input: Flags describing what kind of filtering to use.

	//Gives us "PSP_DEVICE_INTERFACE_DATA" which contains the Interface specific GUID (different
	//from class GUID).  We need the interface GUID to get the device path.
	[DllImport("setupapi.dll", CharSet = CharSet::Seeifdef, EntryPoint = "SetupDiEnumDeviceInterfaces")]
	extern "C" WINSETUPAPI BOOL WINAPI  SetupDiEnumDeviceInterfacesUM(
		HDEVINFO  DeviceInfoSet,			//Input: Give it the HDEVINFO we got from SetupDiGetClassDevs()
		PSP_DEVINFO_DATA  DeviceInfoData,	//Input (optional)
		LPGUID  InterfaceClassGuid,			//Input 
		DWORD  MemberIndex,					//Input: "Index" of the device you are interested in getting the path for.
		PSP_DEVICE_INTERFACE_DATA  DeviceInterfaceData);//Output: This function fills in an "SP_DEVICE_INTERFACE_DATA" structure.

	//SetupDiDestroyDeviceInfoList() frees up memory by destroying a DeviceInfoList
	[DllImport("setupapi.dll", CharSet = CharSet::Seeifdef, EntryPoint = "SetupDiDestroyDeviceInfoList")]
	extern "C" WINSETUPAPI BOOL WINAPI  SetupDiDestroyDeviceInfoListUM(
		HDEVINFO  DeviceInfoSet);			//Input: Give it a handle to a device info list to deallocate from RAM.

	//SetupDiEnumDeviceInfo() fills in an "SP_DEVINFO_DATA" structure, which we need for SetupDiGetDeviceRegistryProperty()
	[DllImport("setupapi.dll", CharSet = CharSet::Seeifdef, EntryPoint = "SetupDiEnumDeviceInfo")]
	extern "C" WINSETUPAPI BOOL WINAPI  SetupDiEnumDeviceInfoUM(
		HDEVINFO  DeviceInfoSet,
		DWORD  MemberIndex,
		PSP_DEVINFO_DATA  DeviceInfoData);

	//SetupDiGetDeviceRegistryProperty() gives us the hardware ID, which we use to check to see if it has matching VID/PID
	[DllImport("setupapi.dll", CharSet = CharSet::Seeifdef, EntryPoint = "SetupDiGetDeviceRegistryProperty")]
	extern "C"	WINSETUPAPI BOOL WINAPI  SetupDiGetDeviceRegistryPropertyUM(
		HDEVINFO  DeviceInfoSet,
		PSP_DEVINFO_DATA  DeviceInfoData,
		DWORD  Property,
		PDWORD  PropertyRegDataType,
		PBYTE  PropertyBuffer,
		DWORD  PropertyBufferSize,
		PDWORD  RequiredSize);

	//SetupDiGetDeviceInterfaceDetail() gives us a device path, which is needed before CreateFile() can be used.
	[DllImport("setupapi.dll", CharSet = CharSet::Seeifdef, EntryPoint = "SetupDiGetDeviceInterfaceDetail")]
	extern "C" BOOL SetupDiGetDeviceInterfaceDetailUM(
		HDEVINFO DeviceInfoSet,										//Input: Wants HDEVINFO which can be obtained from SetupDiGetClassDevs()
		PSP_DEVICE_INTERFACE_DATA DeviceInterfaceData,				//Input: Pointer to an structure which defines the device interface.  
		PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData,	//Output: Pointer to a strucutre, which will contain the device path.
		DWORD DeviceInterfaceDetailDataSize,						//Input: Number of bytes to retrieve.
		PDWORD RequiredSize,										//Output (optional): Te number of bytes needed to hold the entire struct 
		PSP_DEVINFO_DATA DeviceInfoData);							//Output

	[DllImport("user32.dll", CharSet = CharSet::Seeifdef, EntryPoint = "RegisterDeviceNotification")]
	extern "C" HDEVNOTIFY WINAPI RegisterDeviceNotificationUM(
		HANDLE hRecipient,
		LPVOID NotificationFilter,
		DWORD Flags);
#pragma endregion

#pragma region Global Variables
	/*** This section is all of the global variables releated to this namespace ***/

	//Globally Unique Identifier (GUID) for HID class devices.  Windows uses GUIDs to identify things.
	GUID InterfaceClassGuid = { 0x4d1e55b2, 0xf16f, 0x11cf, 0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 };

	HANDLE HandleToMyDevice = INVALID_HANDLE_VALUE;			//Variable we use to hold the handle
	PSP_DEVICE_INTERFACE_DETAIL_DATA MyStructureWithDetailedInterfaceDataInIt = new SP_DEVICE_INTERFACE_DETAIL_DATA;	//Make this global, so we can pass the device path to various CreateFile() calls all over the program
	BOOL MyDeviceAttachedStatus = false;	//False = disconnected, true = connected.
	DWORD ErrorStatusWrite = ERROR_SUCCESS;
	DWORD ErrorStatusRead = ERROR_SUCCESS;
	BOOL Status = false;

#ifdef USE_PASSWORD
	const unsigned char mediconPassword[PWD_LGTH] = { 0x82, 0x14, 0x2A, 0x5D, 0x6F, 0x9A, 0x25, 0x01 };  // pwd (ASCII): TH<x2hRd
#endif

#pragma endregion

	public ref class Form1 : public System::Windows::Forms::Form
	{
#pragma region Form1 Variables
		Thread^ ReadThread;
		Thread^ ProgramThread;
		Thread^ EraseThread;
		Thread^ QueryThread;
		Thread^ VerifyThread;
		Thread^ UnlockConfigThread;
		Thread^ ResetThread;

		unsigned char QueryThreadResults;
		unsigned char ProgramThreadResults;
		unsigned char EraseThreadResults;
		unsigned char VerifyThreadResults;
		unsigned char ReadThreadResults;
		unsigned char UnlockConfigThreadResults;
		unsigned char ResetThreadResults;

		bool  btn_Verify_restore;
		bool  btn_ResetDevice_restore;
		bool  btn_DumpMemory_restore;
		bool  btn_Query_restore;
		bool  btn_ProgramVerify_restore;
		bool  btn_OpenHexFile_restore;
		bool  btn_ExportHex_restore;
		bool  btn_EraseDevice_restore;
		bool  btn_ReadDevice_restore;
		bool  ckbox_ConfigWordProgramming_restore;

		static MEMORY_REGION* memoryRegions;
		unsigned char progressStatus;
		unsigned char bootloaderState;
		unsigned char bytesPerInstructionWord;
		unsigned char bytesPerAddressInHex;
		bool unlockStatus;
		bool enablePrint;
		unsigned char bytesPerAddress;
		unsigned char bytesPerPacket;
		unsigned char memoryRegionsDetected;
		bool inTimer;
		bool deviceAttached;

		unsigned char *pData;
		unsigned char *pData0;
		unsigned char *pData1;
		unsigned char *pData2;
		unsigned char *pData3;
		unsigned char *pData4;
		unsigned char *pData5;











	private: System::Windows::Forms::SaveFileDialog^  dialog_ExportHex;



	private: System::ComponentModel::IContainer^  components;































































private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
private: System::Windows::Forms::ProgressBar^  progressBar_Status;
private: System::Windows::Forms::CheckBox^  ckbox_ConfigWordProgramming;
private: System::Windows::Forms::ListBox^  listBox1;
private: System::Windows::Forms::Button^  btn_ProgramVerify;
private: System::Windows::Forms::Button^  btn_OpenHexFile;
private: System::Windows::Forms::Button^  btn_ReadDevice;
private: System::Windows::Forms::Button^  btn_ExportHex;
private: System::Windows::Forms::Button^  btn_EraseDevice;
private: System::Windows::Forms::Button^  btn_Verify;
private: System::Windows::Forms::Button^  btn_ResetDevice;
private: System::Windows::Forms::Button^  btn_DumpMemory;
private: System::Windows::Forms::Button^  btn_Query;

private: System::Windows::Forms::Button^  btn_MAB;
private: System::Windows::Forms::Button^  btn_ClearDevice;
private: System::Windows::Forms::Button^  btn_B1;
private: System::Windows::Forms::Button^  btn_B2;
private: System::Windows::Forms::Button^  btn_B3;
private: System::Windows::Forms::Button^  btn_B4;
private: System::Windows::Forms::Button^  btn_B5;
private: System::Windows::Forms::Button^  btn_B6;
private: System::Windows::Forms::Button^  btn_B7;
private: System::Windows::Forms::Button^  btn_B8;
private: System::Windows::Forms::Button^  btn_C1;
private: System::Windows::Forms::Button^  btn_C2;
private: System::Windows::Forms::Button^  btn_C3;
private: System::Windows::Forms::Button^  btn_C4;
private: System::Windows::Forms::Button^  btn_C5;
private: System::Windows::Forms::Button^  btn_C6;
private: System::Windows::Forms::Button^  btn_C7;
private: System::Windows::Forms::Button^  btn_C8;
private: System::Windows::Forms::Button^  btn_C9;
private: System::Windows::Forms::Button^  btn_C10;
private: System::Windows::Forms::Button^  btn_C11;
private: System::Windows::Forms::Button^  btn_C12;
private: System::Windows::Forms::Button^  btn_C13;
private: System::Windows::Forms::Button^  btn_C14;
private: System::Windows::Forms::Button^  btn_C15;
private: System::Windows::Forms::Button^  btn_C16;
private: System::Windows::Forms::Button^  btn_C17;
private: System::Windows::Forms::Button^  btn_C18;
private: System::Windows::Forms::Button^  btn_C19;
private: System::Windows::Forms::Button^  btn_C20;
private: System::Windows::Forms::Button^  btn_C21;
private: System::Windows::Forms::Button^  btn_C22;
private: System::Windows::Forms::Button^  btn_C23;
private: System::Windows::Forms::Button^  btn_C24;
private: System::Windows::Forms::Button^  btn_x_axis;
private: System::Windows::Forms::Button^  btn_y_axis;
private: System::Windows::Forms::Button^  btn_container_1;
private: System::Windows::Forms::Button^  btn_container_2;
private: System::Windows::Forms::Button^  btn_container_3;
private: System::Windows::Forms::Button^  btn_container_4;
private: System::Windows::Forms::Button^  btn_cover_1;
private: System::Windows::Forms::Button^  btn_cover_2;
private: System::Windows::Forms::Button^  btn_autocap;
private: System::Windows::Forms::Button^  btn_canlifter;
private: System::Windows::Forms::Button^  btn_humidifier;
private: System::Windows::Forms::Button^  btn_tinting;
private: System::Windows::Forms::Button^  btn_unused_4;
private: System::Windows::Forms::Button^  btn_unused_5;
private: System::Windows::Forms::Button^  btn_unused_6;
private: System::Windows::Forms::Button^  btn_unused_7;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel2;
private: System::Windows::Forms::Button^  btn_clear;













































































	private: System::Windows::Forms::Timer^  tmr_ThreadStatus;
#pragma endregion

#pragma region Constructor Functions
	public:
		Form1(void)
		{
			unsigned char i;

			InitializeComponent();

			btn_Verify_restore = btn_Verify->Enabled;
			btn_ResetDevice_restore = btn_ResetDevice->Enabled;
			btn_ProgramVerify_restore = btn_ProgramVerify->Enabled;
			btn_OpenHexFile_restore = btn_OpenHexFile->Enabled;
			btn_ExportHex_restore = btn_ExportHex->Enabled;
			btn_EraseDevice_restore = btn_EraseDevice->Enabled;
			btn_ReadDevice_restore = btn_ReadDevice->Enabled;
			ckbox_ConfigWordProgramming_restore = ckbox_ConfigWordProgramming->Enabled;

			//Create a new set of memory regions and an array of pointers
			//	to what will become the allocated memory for memory space
			memoryRegions = new MEMORY_REGION[MAX_DATA_REGIONS];

			//pData = new unsigned char*[MAX_DATA_REGIONS];
			memoryRegionsDetected = 0;

			//set all of the pointers to NULL so that we know that the
			//	memory hasn't been allocated yet.
			for (i = 0; i < MAX_DATA_REGIONS; i++)
			{
				setMemoryRegion(i, 0);
			}

			unlockStatus = false;
			enablePrint = false;
			inTimer = false;
			deviceAttached = false;

			//Set the progress status bar to 0%
			progressStatus = 0;

			//Set the number of bytes per address to 0 until we perform
			//	a query and get the real results
			bytesPerAddress = 0;

			/* Initialize the device which must be programmed to prevent error */
//			deviceID = DEVICE_MAB;
			deviceID = 0;

			//Set the initial state of main state machine to IDLE
			bootloaderState = BOOTLOADER_IDLE;
			ProgramThreadResults = PROGRAM_IDLE;

#if defined(DEBUGGING)
			//If we are running in DEBUGGING mode then enable and make
			//	visiable a couple of extra buttons for testing
			btn_DumpMemory->Visible = true;
			btn_DumpMemory->Enabled = true;

			btn_Query->Visible = true;
			btn_Query->Enabled = true;
#endif

			//Register for WM_DEVICECHANGE notifications:
			DEV_BROADCAST_DEVICEINTERFACE MyDeviceBroadcastHeader;// = new DEV_BROADCAST_HDR;
			MyDeviceBroadcastHeader.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
			MyDeviceBroadcastHeader.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
			MyDeviceBroadcastHeader.dbcc_reserved = 0;	//Reserved says not to use...
			MyDeviceBroadcastHeader.dbcc_classguid = InterfaceClassGuid;
			RegisterDeviceNotificationUM((HANDLE)this->Handle, &MyDeviceBroadcastHeader, DEVICE_NOTIFY_WINDOW_HANDLE);

			//Now make an initial attempt to find the device, if it was already connected to the PC and enumerated prior to launching the application.
			//If it is connected and present, we should open read and write pipes to the device so we can communicate with it later.
			//If it was not connected, we will have to wait until the user plugs the device in, and the WM_DEVICECHANGE user callback function can process
			//the message and again search for the device.
			Status = TryToFindHIDDeviceFromVIDPID();
			if (Status == TRUE)
			{
				HANDLE WriteHandleToMyDevice = INVALID_HANDLE_VALUE;
				HANDLE ReadHandleToMyDevice = INVALID_HANDLE_VALUE;

				//Open read and write pipes to the device.
				WriteHandleToMyDevice = CreateFile(MyStructureWithDetailedInterfaceDataInIt->DevicePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
				ErrorStatusWrite = GetLastError();
				ReadHandleToMyDevice = CreateFile(MyStructureWithDetailedInterfaceDataInIt->DevicePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
				ErrorStatusRead = GetLastError();

				if ((ErrorStatusRead == ERROR_SUCCESS) & (ErrorStatusWrite == ERROR_SUCCESS))
				{
					MyDeviceAttachedStatus = true;
					DeviceAttached();
					DEBUG_OUT("Successfully got read/write handles to device: " + MY_DEVICE_ID);
				}
				else
				{
					MyDeviceAttachedStatus = false;
					DeviceRemoved();
					DEBUG_OUT("Found the device, but could not open read/write handles.");
				}
			}
			else //Status was == FALSE, could not find device with matching VID and PID
			{
				MyDeviceAttachedStatus = false;
				DeviceRemoved();
				ENABLE_PRINT();
				PRINT_STATUS("Device not detected. Verify device is in bootloader mode.");
				DEBUG_OUT("Could not find device: " + MY_DEVICE_ID);
			}

			tmr_ThreadStatus->Enabled = true;
			tmr_ThreadStatus->Start();
		}
#pragma endregion

#pragma region Destructor Functions
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			unsigned char i;

			if (components)
			{
				delete components;
			}

			//For each of the possible allocated memory regions
			for (i = 0; i < MAX_DATA_REGIONS; i++)
			{
				pData = getMemoryRegion(i);
				if (pData != 0)
				{
					//If the pointer wasn't NULL then we must have
					//	allocated memory to that pointer.  Free the 
					//	pointer and set the pointer back to NULL.
					free(pData);
					setMemoryRegion(i, 0);
				}
			}
		}

#pragma endregion

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->tmr_ThreadStatus = (gcnew System::Windows::Forms::Timer(this->components));
			this->dialog_ExportHex = (gcnew System::Windows::Forms::SaveFileDialog());
			this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->progressBar_Status = (gcnew System::Windows::Forms::ProgressBar());
			this->flowLayoutPanel2 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->btn_clear = (gcnew System::Windows::Forms::Button());
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->ckbox_ConfigWordProgramming = (gcnew System::Windows::Forms::CheckBox());
			this->btn_ProgramVerify = (gcnew System::Windows::Forms::Button());
			this->btn_OpenHexFile = (gcnew System::Windows::Forms::Button());
			this->btn_ReadDevice = (gcnew System::Windows::Forms::Button());
			this->btn_ExportHex = (gcnew System::Windows::Forms::Button());
			this->btn_EraseDevice = (gcnew System::Windows::Forms::Button());
			this->btn_Verify = (gcnew System::Windows::Forms::Button());
			this->btn_ResetDevice = (gcnew System::Windows::Forms::Button());
			this->btn_DumpMemory = (gcnew System::Windows::Forms::Button());
			this->btn_Query = (gcnew System::Windows::Forms::Button());
			this->btn_MAB = (gcnew System::Windows::Forms::Button());
			this->btn_ClearDevice = (gcnew System::Windows::Forms::Button());
			this->btn_B1 = (gcnew System::Windows::Forms::Button());
			this->btn_B2 = (gcnew System::Windows::Forms::Button());
			this->btn_B3 = (gcnew System::Windows::Forms::Button());
			this->btn_B4 = (gcnew System::Windows::Forms::Button());
			this->btn_B5 = (gcnew System::Windows::Forms::Button());
			this->btn_B6 = (gcnew System::Windows::Forms::Button());
			this->btn_B7 = (gcnew System::Windows::Forms::Button());
			this->btn_B8 = (gcnew System::Windows::Forms::Button());
			this->btn_C1 = (gcnew System::Windows::Forms::Button());
			this->btn_C2 = (gcnew System::Windows::Forms::Button());
			this->btn_C3 = (gcnew System::Windows::Forms::Button());
			this->btn_C4 = (gcnew System::Windows::Forms::Button());
			this->btn_C5 = (gcnew System::Windows::Forms::Button());
			this->btn_C6 = (gcnew System::Windows::Forms::Button());
			this->btn_C7 = (gcnew System::Windows::Forms::Button());
			this->btn_C8 = (gcnew System::Windows::Forms::Button());
			this->btn_C9 = (gcnew System::Windows::Forms::Button());
			this->btn_C10 = (gcnew System::Windows::Forms::Button());
			this->btn_C11 = (gcnew System::Windows::Forms::Button());
			this->btn_C12 = (gcnew System::Windows::Forms::Button());
			this->btn_C13 = (gcnew System::Windows::Forms::Button());
			this->btn_C14 = (gcnew System::Windows::Forms::Button());
			this->btn_C15 = (gcnew System::Windows::Forms::Button());
			this->btn_C16 = (gcnew System::Windows::Forms::Button());
			this->btn_C17 = (gcnew System::Windows::Forms::Button());
			this->btn_C18 = (gcnew System::Windows::Forms::Button());
			this->btn_C19 = (gcnew System::Windows::Forms::Button());
			this->btn_C20 = (gcnew System::Windows::Forms::Button());
			this->btn_C21 = (gcnew System::Windows::Forms::Button());
			this->btn_C22 = (gcnew System::Windows::Forms::Button());
			this->btn_C23 = (gcnew System::Windows::Forms::Button());
			this->btn_C24 = (gcnew System::Windows::Forms::Button());
			this->btn_x_axis = (gcnew System::Windows::Forms::Button());
			this->btn_y_axis = (gcnew System::Windows::Forms::Button());
			this->btn_container_1 = (gcnew System::Windows::Forms::Button());
			this->btn_container_2 = (gcnew System::Windows::Forms::Button());
			this->btn_container_3 = (gcnew System::Windows::Forms::Button());
			this->btn_container_4 = (gcnew System::Windows::Forms::Button());
			this->btn_cover_1 = (gcnew System::Windows::Forms::Button());
			this->btn_cover_2 = (gcnew System::Windows::Forms::Button());
			this->btn_autocap = (gcnew System::Windows::Forms::Button());
			this->btn_canlifter = (gcnew System::Windows::Forms::Button());
			this->btn_humidifier = (gcnew System::Windows::Forms::Button());
			this->btn_tinting = (gcnew System::Windows::Forms::Button());
			this->btn_unused_4 = (gcnew System::Windows::Forms::Button());
			this->btn_unused_5 = (gcnew System::Windows::Forms::Button());
			this->btn_unused_6 = (gcnew System::Windows::Forms::Button());
			this->btn_unused_7 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->flowLayoutPanel1->SuspendLayout();
			this->flowLayoutPanel2->SuspendLayout();
			this->SuspendLayout();
			// 
			// tmr_ThreadStatus
			// 
			this->tmr_ThreadStatus->Interval = 1;
			this->tmr_ThreadStatus->Tick += gcnew System::EventHandler(this, &Form1::tmr_ThreadStatus_Tick);
			// 
			// dialog_ExportHex
			// 
			this->dialog_ExportHex->DefaultExt = L"hex";
			this->dialog_ExportHex->Filter = L"Hex Files|*.hex|All files|*.*";
			this->dialog_ExportHex->ShowHelp = true;
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->Controls->Add(this->progressBar_Status);
			this->flowLayoutPanel1->Controls->Add(this->flowLayoutPanel2);
			this->flowLayoutPanel1->Controls->Add(this->listBox1);
			this->flowLayoutPanel1->Location = System::Drawing::Point(0, 622);
			this->flowLayoutPanel1->Margin = System::Windows::Forms::Padding(4);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(984, 236);
			this->flowLayoutPanel1->TabIndex = 88;
			// 
			// progressBar_Status
			// 
			this->progressBar_Status->Location = System::Drawing::Point(4, 4);
			this->progressBar_Status->Margin = System::Windows::Forms::Padding(4);
			this->progressBar_Status->Name = L"progressBar_Status";
			this->progressBar_Status->Size = System::Drawing::Size(375, 32);
			this->progressBar_Status->TabIndex = 32;
			// 
			// flowLayoutPanel2
			// 
			this->flowLayoutPanel2->Controls->Add(this->btn_clear);
			this->flowLayoutPanel2->FlowDirection = System::Windows::Forms::FlowDirection::RightToLeft;
			this->flowLayoutPanel2->Location = System::Drawing::Point(387, 4);
			this->flowLayoutPanel2->Margin = System::Windows::Forms::Padding(4);
			this->flowLayoutPanel2->Name = L"flowLayoutPanel2";
			this->flowLayoutPanel2->Size = System::Drawing::Size(303, 39);
			this->flowLayoutPanel2->TabIndex = 33;
			this->flowLayoutPanel2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::flowLayoutPanel2_Paint);
			// 
			// btn_clear
			// 
			this->btn_clear->Location = System::Drawing::Point(199, 4);
			this->btn_clear->Margin = System::Windows::Forms::Padding(4, 4, 0, 4);
			this->btn_clear->Name = L"btn_clear";
			this->btn_clear->Size = System::Drawing::Size(104, 28);
			this->btn_clear->TabIndex = 0;
			this->btn_clear->Text = L"&Clear Log";
			this->btn_clear->UseVisualStyleBackColor = true;
			this->btn_clear->Click += gcnew System::EventHandler(this, &Form1::btn_clear_Click);
			// 
			// listBox1
			// 
			this->listBox1->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->listBox1->FormattingEnabled = true;
			this->listBox1->ItemHeight = 17;
			this->listBox1->Location = System::Drawing::Point(4, 51);
			this->listBox1->Margin = System::Windows::Forms::Padding(4);
			this->listBox1->Name = L"listBox1";
			this->listBox1->ScrollAlwaysVisible = true;
			this->listBox1->Size = System::Drawing::Size(979, 174);
			this->listBox1->TabIndex = 30;
			// 
			// ckbox_ConfigWordProgramming
			// 
			/*
			this->ckbox_ConfigWordProgramming->Location = System::Drawing::Point(0, 0);
			this->ckbox_ConfigWordProgramming->Name = L"ckbox_ConfigWordProgramming";
			this->ckbox_ConfigWordProgramming->Size = System::Drawing::Size(104, 24);
			this->ckbox_ConfigWordProgramming->TabIndex = 0;
			*/
			// ckbox_ConfigWordProgramming
			// 
			this->ckbox_ConfigWordProgramming->AutoSize = true;
			this->ckbox_ConfigWordProgramming->Enabled = false;
			this->ckbox_ConfigWordProgramming->Location = System::Drawing::Point(387, 10);
			this->ckbox_ConfigWordProgramming->Margin = System::Windows::Forms::Padding(4, 10, 4, 4);
			this->ckbox_ConfigWordProgramming->Name = L"ckbox_ConfigWordProgramming";
			this->ckbox_ConfigWordProgramming->Size = System::Drawing::Size(276, 21);
			this->ckbox_ConfigWordProgramming->TabIndex = 31;
			this->ckbox_ConfigWordProgramming->Text = L"Allow Configuration Word Programming";
			this->ckbox_ConfigWordProgramming->UseVisualStyleBackColor = true;
			this->ckbox_ConfigWordProgramming->CheckedChanged += gcnew System::EventHandler(this, &Form1::ckbox_ConfigWordProgramming_CheckedChanged_1);
			// 
			// btn_ProgramVerify
			// 
			this->btn_ProgramVerify->Enabled = false;
			this->btn_ProgramVerify->Location = System::Drawing::Point(129, 91);
			this->btn_ProgramVerify->Margin = System::Windows::Forms::Padding(4);
			this->btn_ProgramVerify->Name = L"btn_ProgramVerify";
			this->btn_ProgramVerify->Size = System::Drawing::Size(144, 30);
			this->btn_ProgramVerify->TabIndex = 22;
			this->btn_ProgramVerify->Text = L"&Program/Verify";
			this->btn_ProgramVerify->UseVisualStyleBackColor = true;
			this->btn_ProgramVerify->Click += gcnew System::EventHandler(this, &Form1::btn_ProgramVerify_Click);
			// 
			// btn_OpenHexFile
			// 
			this->btn_OpenHexFile->Enabled = false;
			this->btn_OpenHexFile->Location = System::Drawing::Point(129, 55);
			this->btn_OpenHexFile->Margin = System::Windows::Forms::Padding(4);
			this->btn_OpenHexFile->Name = L"btn_OpenHexFile";
			this->btn_OpenHexFile->Size = System::Drawing::Size(144, 30);
			this->btn_OpenHexFile->TabIndex = 24;
			this->btn_OpenHexFile->Text = L"&Open Hex File";
			this->btn_OpenHexFile->UseVisualStyleBackColor = true;
			this->btn_OpenHexFile->Click += gcnew System::EventHandler(this, &Form1::btn_OpenHexFile_Click);
			// 
			// btn_ReadDevice
			// 
			this->btn_ReadDevice->Enabled = false;
			this->btn_ReadDevice->Location = System::Drawing::Point(447, 55);
			this->btn_ReadDevice->Margin = System::Windows::Forms::Padding(4);
			this->btn_ReadDevice->Name = L"btn_ReadDevice";
			this->btn_ReadDevice->Size = System::Drawing::Size(144, 30);
			this->btn_ReadDevice->TabIndex = 25;
			this->btn_ReadDevice->Text = L"Read Device";
			this->btn_ReadDevice->UseVisualStyleBackColor = true;
			this->btn_ReadDevice->Click += gcnew System::EventHandler(this, &Form1::btn_ReadDevice_Click);
			// 
			// btn_ExportHex
			// 
			this->btn_ExportHex->Enabled = false;
			this->btn_ExportHex->Location = System::Drawing::Point(605, 55);
			this->btn_ExportHex->Margin = System::Windows::Forms::Padding(4);
			this->btn_ExportHex->Name = L"btn_ExportHex";
			this->btn_ExportHex->Size = System::Drawing::Size(144, 30);
			this->btn_ExportHex->TabIndex = 27;
			this->btn_ExportHex->Text = L"Export &Hex";
			this->btn_ExportHex->UseVisualStyleBackColor = true;
			this->btn_ExportHex->Click += gcnew System::EventHandler(this, &Form1::btn_ExportHex_Click);
			// 
			// btn_EraseDevice
			// 
			this->btn_EraseDevice->Enabled = false;
			this->btn_EraseDevice->Location = System::Drawing::Point(288, 55);
			this->btn_EraseDevice->Margin = System::Windows::Forms::Padding(4);
			this->btn_EraseDevice->Name = L"btn_EraseDevice";
			this->btn_EraseDevice->Size = System::Drawing::Size(144, 30);
			this->btn_EraseDevice->TabIndex = 28;
			this->btn_EraseDevice->Text = L"Erase Device";
			this->btn_EraseDevice->UseVisualStyleBackColor = true;
			this->btn_EraseDevice->Click += gcnew System::EventHandler(this, &Form1::btn_EraseDevice_Click);
			// 
			// btn_Verify
			// 
			this->btn_Verify->Enabled = false;
			this->btn_Verify->Location = System::Drawing::Point(289, 90);
			this->btn_Verify->Margin = System::Windows::Forms::Padding(4);
			this->btn_Verify->Name = L"btn_Verify";
			this->btn_Verify->Size = System::Drawing::Size(143, 30);
			this->btn_Verify->TabIndex = 32;
			this->btn_Verify->Text = L"&Verify";
			this->btn_Verify->UseVisualStyleBackColor = true;
			this->btn_Verify->Click += gcnew System::EventHandler(this, &Form1::btn_Verify_Click);
			// 
			// btn_ResetDevice
			// 
			this->btn_ResetDevice->Enabled = false;
			this->btn_ResetDevice->Location = System::Drawing::Point(447, 90);
			this->btn_ResetDevice->Margin = System::Windows::Forms::Padding(4);
			this->btn_ResetDevice->Name = L"btn_ResetDevice";
			this->btn_ResetDevice->Size = System::Drawing::Size(145, 30);
			this->btn_ResetDevice->TabIndex = 33;
			this->btn_ResetDevice->Text = L"&Reset Device";
			this->btn_ResetDevice->UseVisualStyleBackColor = true;
			this->btn_ResetDevice->Click += gcnew System::EventHandler(this, &Form1::btn_ResetDevice_Click);
			// 
			// btn_DumpMemory
			// 
			this->btn_DumpMemory->Location = System::Drawing::Point(765, 54);
			this->btn_DumpMemory->Margin = System::Windows::Forms::Padding(4);
			this->btn_DumpMemory->Name = L"btn_DumpMemory";
			this->btn_DumpMemory->Size = System::Drawing::Size(143, 30);
			this->btn_DumpMemory->TabIndex = 34;
			this->btn_DumpMemory->Text = L"Dump &Memory";
			this->btn_DumpMemory->UseVisualStyleBackColor = true;
			this->btn_DumpMemory->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// btn_Query
			// 
			this->btn_Query->Location = System::Drawing::Point(765, 91);
			this->btn_Query->Margin = System::Windows::Forms::Padding(4);
			this->btn_Query->Name = L"btn_Query";
			this->btn_Query->Size = System::Drawing::Size(143, 30);
			this->btn_Query->TabIndex = 35;
			this->btn_Query->Text = L"&Query";
			this->btn_Query->UseVisualStyleBackColor = true;
			this->btn_Query->Click += gcnew System::EventHandler(this, &Form1::btn_Query_Click);
			// 
			// btn_MAB
			// 
			this->btn_MAB->Location = System::Drawing::Point(131, 178);
			this->btn_MAB->Margin = System::Windows::Forms::Padding(4);
			this->btn_MAB->Name = L"btn_MAB";
			this->btn_MAB->Size = System::Drawing::Size(143, 28);
			this->btn_MAB->TabIndex = 37;
			this->btn_MAB->Text = L"MAB / Tinting Master";
			this->btn_MAB->UseVisualStyleBackColor = true;
			this->btn_MAB->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_Clear Device
			// 
			this->btn_ClearDevice->BackColor = System::Drawing::SystemColors::ControlLight;
			this->btn_ClearDevice->Location = System::Drawing::Point(131, 214);
			this->btn_ClearDevice->Margin = System::Windows::Forms::Padding(4);
			this->btn_ClearDevice->Name = L"btn_ClearDevice";
			this->btn_ClearDevice->Size = System::Drawing::Size(143, 28);
			this->btn_ClearDevice->TabIndex = 38;
			this->btn_ClearDevice->Text = L"Clear Device";
			this->btn_ClearDevice->UseVisualStyleBackColor = false;
			this->btn_ClearDevice->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_B1
			// 
			this->btn_B1->BackColor = System::Drawing::SystemColors::ControlLight;
			this->btn_B1->Location = System::Drawing::Point(289, 178);
			this->btn_B1->Margin = System::Windows::Forms::Padding(4);
			this->btn_B1->Name = L"btn_B1";
			this->btn_B1->Size = System::Drawing::Size(143, 28);
			this->btn_B1->TabIndex = 38;
			this->btn_B1->Text = L"B1";
			this->btn_B1->UseVisualStyleBackColor = false;
			this->btn_B1->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_B2
			// 
			this->btn_B2->Location = System::Drawing::Point(448, 178);
			this->btn_B2->Margin = System::Windows::Forms::Padding(4);
			this->btn_B2->Name = L"btn_B2";
			this->btn_B2->Size = System::Drawing::Size(143, 28);
			this->btn_B2->TabIndex = 39;
			this->btn_B2->Text = L"B2";
			this->btn_B2->UseVisualStyleBackColor = true;
			this->btn_B2->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_B3
			// 
			this->btn_B3->Location = System::Drawing::Point(607, 178);
			this->btn_B3->Margin = System::Windows::Forms::Padding(4);
			this->btn_B3->Name = L"btn_B3";
			this->btn_B3->Size = System::Drawing::Size(143, 28);
			this->btn_B3->TabIndex = 40;
			this->btn_B3->Text = L"B3";
			this->btn_B3->UseVisualStyleBackColor = true;
			this->btn_B3->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_B4
			// 
			this->btn_B4->Location = System::Drawing::Point(765, 178);
			this->btn_B4->Margin = System::Windows::Forms::Padding(4);
			this->btn_B4->Name = L"btn_B4";
			this->btn_B4->Size = System::Drawing::Size(143, 28);
			this->btn_B4->TabIndex = 41;
			this->btn_B4->Text = L"B4";
			this->btn_B4->UseVisualStyleBackColor = true;
			this->btn_B4->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_B5
			// 
			this->btn_B5->Location = System::Drawing::Point(289, 214);
			this->btn_B5->Margin = System::Windows::Forms::Padding(4);
			this->btn_B5->Name = L"btn_B5";
			this->btn_B5->Size = System::Drawing::Size(143, 28);
			this->btn_B5->TabIndex = 42;
			this->btn_B5->Text = L"B5";
			this->btn_B5->UseVisualStyleBackColor = true;
			this->btn_B5->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_B6
			// 
			this->btn_B6->Location = System::Drawing::Point(448, 214);
			this->btn_B6->Margin = System::Windows::Forms::Padding(4);
			this->btn_B6->Name = L"btn_B6";
			this->btn_B6->Size = System::Drawing::Size(143, 28);
			this->btn_B6->TabIndex = 43;
			this->btn_B6->Text = L"B6";
			this->btn_B6->UseVisualStyleBackColor = true;
			this->btn_B6->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_B7
			// 
			this->btn_B7->Location = System::Drawing::Point(607, 214);
			this->btn_B7->Margin = System::Windows::Forms::Padding(4);
			this->btn_B7->Name = L"btn_B7";
			this->btn_B7->Size = System::Drawing::Size(143, 28);
			this->btn_B7->TabIndex = 44;
			this->btn_B7->Text = L"B7";
			this->btn_B7->UseVisualStyleBackColor = true;
			this->btn_B7->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_B8
			// 
			this->btn_B8->Location = System::Drawing::Point(765, 214);
			this->btn_B8->Margin = System::Windows::Forms::Padding(4);
			this->btn_B8->Name = L"btn_B8";
			this->btn_B8->Size = System::Drawing::Size(143, 28);
			this->btn_B8->TabIndex = 45;
			this->btn_B8->Text = L"B8";
			this->btn_B8->UseVisualStyleBackColor = true;
			this->btn_B8->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C1
			// 
			this->btn_C1->BackColor = System::Drawing::SystemColors::ControlLight;
			this->btn_C1->Location = System::Drawing::Point(289, 250);
			this->btn_C1->Margin = System::Windows::Forms::Padding(4);
			this->btn_C1->Name = L"btn_C1";
			this->btn_C1->Size = System::Drawing::Size(143, 28);
			this->btn_C1->TabIndex = 46;
			this->btn_C1->Text = L"C1";
			this->btn_C1->UseVisualStyleBackColor = false;
			this->btn_C1->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C2
			// 
			this->btn_C2->Location = System::Drawing::Point(448, 250);
			this->btn_C2->Margin = System::Windows::Forms::Padding(4);
			this->btn_C2->Name = L"btn_C2";
			this->btn_C2->Size = System::Drawing::Size(143, 28);
			this->btn_C2->TabIndex = 47;
			this->btn_C2->Text = L"C2";
			this->btn_C2->UseVisualStyleBackColor = true;
			this->btn_C2->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C3
			// 
			this->btn_C3->Location = System::Drawing::Point(607, 250);
			this->btn_C3->Margin = System::Windows::Forms::Padding(4);
			this->btn_C3->Name = L"btn_C3";
			this->btn_C3->Size = System::Drawing::Size(143, 28);
			this->btn_C3->TabIndex = 48;
			this->btn_C3->Text = L"C3";
			this->btn_C3->UseVisualStyleBackColor = true;
			this->btn_C3->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C4
			// 
			this->btn_C4->Location = System::Drawing::Point(765, 250);
			this->btn_C4->Margin = System::Windows::Forms::Padding(4);
			this->btn_C4->Name = L"btn_C4";
			this->btn_C4->Size = System::Drawing::Size(143, 28);
			this->btn_C4->TabIndex = 49;
			this->btn_C4->Text = L"C4";
			this->btn_C4->UseVisualStyleBackColor = true;
			this->btn_C4->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C5
			// 
			this->btn_C5->Location = System::Drawing::Point(289, 286);
			this->btn_C5->Margin = System::Windows::Forms::Padding(4);
			this->btn_C5->Name = L"btn_C5";
			this->btn_C5->Size = System::Drawing::Size(143, 28);
			this->btn_C5->TabIndex = 50;
			this->btn_C5->Text = L"C5";
			this->btn_C5->UseVisualStyleBackColor = true;
			this->btn_C5->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C6
			// 
			this->btn_C6->Location = System::Drawing::Point(448, 286);
			this->btn_C6->Margin = System::Windows::Forms::Padding(4);
			this->btn_C6->Name = L"btn_C6";
			this->btn_C6->Size = System::Drawing::Size(143, 28);
			this->btn_C6->TabIndex = 51;
			this->btn_C6->Text = L"C6";
			this->btn_C6->UseVisualStyleBackColor = true;
			this->btn_C6->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C7
			// 
			this->btn_C7->Location = System::Drawing::Point(607, 286);
			this->btn_C7->Margin = System::Windows::Forms::Padding(4);
			this->btn_C7->Name = L"btn_C7";
			this->btn_C7->Size = System::Drawing::Size(143, 28);
			this->btn_C7->TabIndex = 52;
			this->btn_C7->Text = L"C7";
			this->btn_C7->UseVisualStyleBackColor = true;
			this->btn_C7->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C8
			// 
			this->btn_C8->Location = System::Drawing::Point(765, 286);
			this->btn_C8->Margin = System::Windows::Forms::Padding(4);
			this->btn_C8->Name = L"btn_C8";
			this->btn_C8->Size = System::Drawing::Size(143, 28);
			this->btn_C8->TabIndex = 53;
			this->btn_C8->Text = L"C8";
			this->btn_C8->UseVisualStyleBackColor = true;
			this->btn_C8->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C9
			// 
			this->btn_C9->Location = System::Drawing::Point(289, 321);
			this->btn_C9->Margin = System::Windows::Forms::Padding(4);
			this->btn_C9->Name = L"btn_C9";
			this->btn_C9->Size = System::Drawing::Size(143, 28);
			this->btn_C9->TabIndex = 54;
			this->btn_C9->Text = L"C9";
			this->btn_C9->UseVisualStyleBackColor = true;
			this->btn_C9->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C10
			// 
			this->btn_C10->Location = System::Drawing::Point(448, 321);
			this->btn_C10->Margin = System::Windows::Forms::Padding(4);
			this->btn_C10->Name = L"btn_C10";
			this->btn_C10->Size = System::Drawing::Size(143, 28);
			this->btn_C10->TabIndex = 55;
			this->btn_C10->Text = L"C10";
			this->btn_C10->UseVisualStyleBackColor = true;
			this->btn_C10->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C11
			// 
			this->btn_C11->Location = System::Drawing::Point(607, 321);
			this->btn_C11->Margin = System::Windows::Forms::Padding(4);
			this->btn_C11->Name = L"btn_C11";
			this->btn_C11->Size = System::Drawing::Size(143, 28);
			this->btn_C11->TabIndex = 56;
			this->btn_C11->Text = L"C11";
			this->btn_C11->UseVisualStyleBackColor = true;
			this->btn_C11->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C12
			// 
			this->btn_C12->Location = System::Drawing::Point(765, 321);
			this->btn_C12->Margin = System::Windows::Forms::Padding(4);
			this->btn_C12->Name = L"btn_C12";
			this->btn_C12->Size = System::Drawing::Size(143, 28);
			this->btn_C12->TabIndex = 57;
			this->btn_C12->Text = L"C12";
			this->btn_C12->UseVisualStyleBackColor = true;
			this->btn_C12->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C13
			// 
			this->btn_C13->Location = System::Drawing::Point(289, 357);
			this->btn_C13->Margin = System::Windows::Forms::Padding(4);
			this->btn_C13->Name = L"btn_C13";
			this->btn_C13->Size = System::Drawing::Size(143, 28);
			this->btn_C13->TabIndex = 58;
			this->btn_C13->Text = L"C13";
			this->btn_C13->UseVisualStyleBackColor = true;
			this->btn_C13->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C14
			// 
			this->btn_C14->Location = System::Drawing::Point(448, 357);
			this->btn_C14->Margin = System::Windows::Forms::Padding(4);
			this->btn_C14->Name = L"btn_C14";
			this->btn_C14->Size = System::Drawing::Size(143, 28);
			this->btn_C14->TabIndex = 59;
			this->btn_C14->Text = L"C14";
			this->btn_C14->UseVisualStyleBackColor = true;
			this->btn_C14->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C15
			// 
			this->btn_C15->Location = System::Drawing::Point(607, 357);
			this->btn_C15->Margin = System::Windows::Forms::Padding(4);
			this->btn_C15->Name = L"btn_C15";
			this->btn_C15->Size = System::Drawing::Size(143, 28);
			this->btn_C15->TabIndex = 60;
			this->btn_C15->Text = L"C15";
			this->btn_C15->UseVisualStyleBackColor = true;
			this->btn_C15->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C16
			// 
			this->btn_C16->Location = System::Drawing::Point(765, 357);
			this->btn_C16->Margin = System::Windows::Forms::Padding(4);
			this->btn_C16->Name = L"btn_C16";
			this->btn_C16->Size = System::Drawing::Size(143, 28);
			this->btn_C16->TabIndex = 61;
			this->btn_C16->Text = L"C16";
			this->btn_C16->UseVisualStyleBackColor = true;
			this->btn_C16->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C17
			// 
			this->btn_C17->Location = System::Drawing::Point(289, 393);
			this->btn_C17->Margin = System::Windows::Forms::Padding(4);
			this->btn_C17->Name = L"btn_C17";
			this->btn_C17->Size = System::Drawing::Size(143, 28);
			this->btn_C17->TabIndex = 62;
			this->btn_C17->Text = L"C17";
			this->btn_C17->UseVisualStyleBackColor = true;
			this->btn_C17->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C18
			// 
			this->btn_C18->Location = System::Drawing::Point(448, 393);
			this->btn_C18->Margin = System::Windows::Forms::Padding(4);
			this->btn_C18->Name = L"btn_C18";
			this->btn_C18->Size = System::Drawing::Size(143, 28);
			this->btn_C18->TabIndex = 63;
			this->btn_C18->Text = L"C18";
			this->btn_C18->UseVisualStyleBackColor = true;
			this->btn_C18->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C19
			// 
			this->btn_C19->Location = System::Drawing::Point(607, 393);
			this->btn_C19->Margin = System::Windows::Forms::Padding(4);
			this->btn_C19->Name = L"btn_C19";
			this->btn_C19->Size = System::Drawing::Size(143, 28);
			this->btn_C19->TabIndex = 64;
			this->btn_C19->Text = L"C19";
			this->btn_C19->UseVisualStyleBackColor = true;
			this->btn_C19->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C20
			// 
			this->btn_C20->Location = System::Drawing::Point(765, 393);
			this->btn_C20->Margin = System::Windows::Forms::Padding(4);
			this->btn_C20->Name = L"btn_C20";
			this->btn_C20->Size = System::Drawing::Size(143, 28);
			this->btn_C20->TabIndex = 65;
			this->btn_C20->Text = L"C20";
			this->btn_C20->UseVisualStyleBackColor = true;
			this->btn_C20->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C21
			// 
			this->btn_C21->Location = System::Drawing::Point(289, 428);
			this->btn_C21->Margin = System::Windows::Forms::Padding(4);
			this->btn_C21->Name = L"btn_C21";
			this->btn_C21->Size = System::Drawing::Size(143, 28);
			this->btn_C21->TabIndex = 66;
			this->btn_C21->Text = L"C21";
			this->btn_C21->UseVisualStyleBackColor = true;
			this->btn_C21->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C22
			// 
			this->btn_C22->Location = System::Drawing::Point(448, 428);
			this->btn_C22->Margin = System::Windows::Forms::Padding(4);
			this->btn_C22->Name = L"btn_C22";
			this->btn_C22->Size = System::Drawing::Size(143, 28);
			this->btn_C22->TabIndex = 67;
			this->btn_C22->Text = L"C22";
			this->btn_C22->UseVisualStyleBackColor = true;
			this->btn_C22->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C23
			// 
			this->btn_C23->Location = System::Drawing::Point(607, 428);
			this->btn_C23->Margin = System::Windows::Forms::Padding(4);
			this->btn_C23->Name = L"btn_C23";
			this->btn_C23->Size = System::Drawing::Size(143, 28);
			this->btn_C23->TabIndex = 68;
			this->btn_C23->Text = L"C23";
			this->btn_C23->UseVisualStyleBackColor = true;
			this->btn_C23->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_C24
			// 
			this->btn_C24->Location = System::Drawing::Point(765, 428);
			this->btn_C24->Margin = System::Windows::Forms::Padding(4);
			this->btn_C24->Name = L"btn_C24";
			this->btn_C24->Size = System::Drawing::Size(143, 28);
			this->btn_C24->TabIndex = 69;
			this->btn_C24->Text = L"C24";
			this->btn_C24->UseVisualStyleBackColor = true;
			this->btn_C24->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_x_axis
			// 
			this->btn_x_axis->Location = System::Drawing::Point(289, 464);
			this->btn_x_axis->Margin = System::Windows::Forms::Padding(4);
			this->btn_x_axis->Name = L"btn_x_axis";
			this->btn_x_axis->Size = System::Drawing::Size(143, 28);
			this->btn_x_axis->TabIndex = 70;
			this->btn_x_axis->Text = L"X Axis";
			this->btn_x_axis->UseVisualStyleBackColor = true;
			this->btn_x_axis->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_y_axis
			// 
			this->btn_y_axis->Location = System::Drawing::Point(448, 464);
			this->btn_y_axis->Margin = System::Windows::Forms::Padding(4);
			this->btn_y_axis->Name = L"btn_y_axis";
			this->btn_y_axis->Size = System::Drawing::Size(143, 28);
			this->btn_y_axis->TabIndex = 71;
			this->btn_y_axis->Text = L"Y Axis";
			this->btn_y_axis->UseVisualStyleBackColor = true;
			this->btn_y_axis->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_container_1
			// 
			this->btn_container_1->Location = System::Drawing::Point(607, 464);
			this->btn_container_1->Margin = System::Windows::Forms::Padding(4);
			this->btn_container_1->Name = L"btn_container_1";
			this->btn_container_1->Size = System::Drawing::Size(143, 28);
			this->btn_container_1->TabIndex = 72;
			this->btn_container_1->Text = L"Container 1";
			this->btn_container_1->UseVisualStyleBackColor = true;
			this->btn_container_1->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_container_2
			// 
			this->btn_container_2->Location = System::Drawing::Point(765, 464);
			this->btn_container_2->Margin = System::Windows::Forms::Padding(4);
			this->btn_container_2->Name = L"btn_container_2";
			this->btn_container_2->Size = System::Drawing::Size(143, 28);
			this->btn_container_2->TabIndex = 73;
			this->btn_container_2->Text = L"Container 2";
			this->btn_container_2->UseVisualStyleBackColor = true;
			this->btn_container_2->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_container_3
			// 
			this->btn_container_3->Location = System::Drawing::Point(289, 500);
			this->btn_container_3->Margin = System::Windows::Forms::Padding(4);
			this->btn_container_3->Name = L"btn_container_3";
			this->btn_container_3->Size = System::Drawing::Size(143, 28);
			this->btn_container_3->TabIndex = 74;
			this->btn_container_3->Text = L"Container 3";
			this->btn_container_3->UseVisualStyleBackColor = true;
			this->btn_container_3->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_container_4
			// 
			this->btn_container_4->Location = System::Drawing::Point(448, 500);
			this->btn_container_4->Margin = System::Windows::Forms::Padding(4);
			this->btn_container_4->Name = L"btn_container_4";
			this->btn_container_4->Size = System::Drawing::Size(143, 28);
			this->btn_container_4->TabIndex = 75;
			this->btn_container_4->Text = L"Container 4";
			this->btn_container_4->UseVisualStyleBackColor = true;
			this->btn_container_4->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_cover_1
			// 
			this->btn_cover_1->Location = System::Drawing::Point(607, 500);
			this->btn_cover_1->Margin = System::Windows::Forms::Padding(4);
			this->btn_cover_1->Name = L"btn_cover_1";
			this->btn_cover_1->Size = System::Drawing::Size(143, 28);
			this->btn_cover_1->TabIndex = 76;
			this->btn_cover_1->Text = L"Cover 1";
			this->btn_cover_1->UseVisualStyleBackColor = true;
			this->btn_cover_1->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_cover_2
			// 
			this->btn_cover_2->Location = System::Drawing::Point(765, 500);
			this->btn_cover_2->Margin = System::Windows::Forms::Padding(4);
			this->btn_cover_2->Name = L"btn_cover_2";
			this->btn_cover_2->Size = System::Drawing::Size(143, 28);
			this->btn_cover_2->TabIndex = 77;
			this->btn_cover_2->Text = L"Cover 2";
			this->btn_cover_2->UseVisualStyleBackColor = true;
			this->btn_cover_2->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_autocap
			// 
			this->btn_autocap->Location = System::Drawing::Point(289, 535);
			this->btn_autocap->Margin = System::Windows::Forms::Padding(4);
			this->btn_autocap->Name = L"btn_autocap";
			this->btn_autocap->Size = System::Drawing::Size(143, 28);
			this->btn_autocap->TabIndex = 78;
			this->btn_autocap->Text = L"Autocap";
			this->btn_autocap->UseVisualStyleBackColor = true;
			this->btn_autocap->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_canlifter
			// 
			this->btn_canlifter->Location = System::Drawing::Point(448, 535);
			this->btn_canlifter->Margin = System::Windows::Forms::Padding(4);
			this->btn_canlifter->Name = L"btn_canlifter";
			this->btn_canlifter->Size = System::Drawing::Size(143, 28);
			this->btn_canlifter->TabIndex = 79;
			this->btn_canlifter->Text = L"Can Lifter";
			this->btn_canlifter->UseVisualStyleBackColor = true;
			this->btn_canlifter->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_humidifier
			// 
			this->btn_humidifier->Location = System::Drawing::Point(607, 535);
			this->btn_humidifier->Margin = System::Windows::Forms::Padding(4);
			this->btn_humidifier->Name = L"btn_humidifier";
			this->btn_humidifier->Size = System::Drawing::Size(143, 28);
			this->btn_humidifier->TabIndex = 80;
			this->btn_humidifier->Text = L"Humidifier";
			this->btn_humidifier->UseVisualStyleBackColor = true;
			this->btn_humidifier->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_tinting
			// 
			this->btn_tinting->Location = System::Drawing::Point(765, 535);
			this->btn_tinting->Margin = System::Windows::Forms::Padding(4);
			this->btn_tinting->Name = L"btn_tinting";
			this->btn_tinting->Size = System::Drawing::Size(143, 28);
			this->btn_tinting->TabIndex = 81;
			this->btn_tinting->Text = L"Tinting Slave";
			this->btn_tinting->UseVisualStyleBackColor = true;
			this->btn_tinting->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_unused_4
			// 
			this->btn_unused_4->Location = System::Drawing::Point(289, 571);
			this->btn_unused_4->Margin = System::Windows::Forms::Padding(4);
			this->btn_unused_4->Name = L"btn_unused_4";
			this->btn_unused_4->Size = System::Drawing::Size(143, 28);
			this->btn_unused_4->TabIndex = 82;
			this->btn_unused_4->Text = L"Unused 4";
			this->btn_unused_4->UseVisualStyleBackColor = true;
			this->btn_unused_4->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_unused_5
			// 
			this->btn_unused_5->Location = System::Drawing::Point(448, 571);
			this->btn_unused_5->Margin = System::Windows::Forms::Padding(4);
			this->btn_unused_5->Name = L"btn_unused_5";
			this->btn_unused_5->Size = System::Drawing::Size(143, 28);
			this->btn_unused_5->TabIndex = 83;
			this->btn_unused_5->Text = L"Unused 5";
			this->btn_unused_5->UseVisualStyleBackColor = true;
			this->btn_unused_5->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_unused_6
			// 
			this->btn_unused_6->Location = System::Drawing::Point(607, 571);
			this->btn_unused_6->Margin = System::Windows::Forms::Padding(4);
			this->btn_unused_6->Name = L"btn_unused_6";
			this->btn_unused_6->Size = System::Drawing::Size(143, 28);
			this->btn_unused_6->TabIndex = 84;
			this->btn_unused_6->Text = L"Unused 6";
			this->btn_unused_6->UseVisualStyleBackColor = true;
			this->btn_unused_6->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// btn_unused_7
			// 
			this->btn_unused_7->Location = System::Drawing::Point(765, 571);
			this->btn_unused_7->Margin = System::Windows::Forms::Padding(4);
			this->btn_unused_7->Name = L"btn_unused_7";
			this->btn_unused_7->Size = System::Drawing::Size(143, 28);
			this->btn_unused_7->TabIndex = 85;
			this->btn_unused_7->Text = L"Unused 7";
			this->btn_unused_7->UseVisualStyleBackColor = true;
			this->btn_unused_7->Click += gcnew System::EventHandler(this, &Form1::btn_device_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(-1, 145);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(75, 25);
			this->label1->TabIndex = 86;
			this->label1->Text = L"Target";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(-1, 11);
			this->label2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(84, 25);
			this->label2->TabIndex = 87;
			this->label2->Text = L"Actions";
			this->label2->Click += gcnew System::EventHandler(this, &Form1::label2_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->ClientSize = System::Drawing::Size(980, 849);
			this->Controls->Add(this->flowLayoutPanel1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->btn_unused_7);
			this->Controls->Add(this->btn_unused_6);
			this->Controls->Add(this->btn_unused_5);
			this->Controls->Add(this->btn_unused_4);
			this->Controls->Add(this->btn_tinting);
			this->Controls->Add(this->btn_humidifier);
			this->Controls->Add(this->btn_canlifter);
			this->Controls->Add(this->btn_autocap);
			this->Controls->Add(this->btn_cover_2);
			this->Controls->Add(this->btn_cover_1);
			this->Controls->Add(this->btn_container_4);
			this->Controls->Add(this->btn_container_3);
			this->Controls->Add(this->btn_container_2);
			this->Controls->Add(this->btn_container_1);
			this->Controls->Add(this->btn_y_axis);
			this->Controls->Add(this->btn_x_axis);
			this->Controls->Add(this->btn_C24);
			this->Controls->Add(this->btn_C23);
			this->Controls->Add(this->btn_C22);
			this->Controls->Add(this->btn_C21);
			this->Controls->Add(this->btn_C20);
			this->Controls->Add(this->btn_C19);
			this->Controls->Add(this->btn_C18);
			this->Controls->Add(this->btn_C17);
			this->Controls->Add(this->btn_C16);
			this->Controls->Add(this->btn_C15);
			this->Controls->Add(this->btn_C14);
			this->Controls->Add(this->btn_C13);
			this->Controls->Add(this->btn_C12);
			this->Controls->Add(this->btn_C11);
			this->Controls->Add(this->btn_C10);
			this->Controls->Add(this->btn_C9);
			this->Controls->Add(this->btn_C8);
			this->Controls->Add(this->btn_C7);
			this->Controls->Add(this->btn_C6);
			this->Controls->Add(this->btn_C5);
			this->Controls->Add(this->btn_C4);
			this->Controls->Add(this->btn_C3);
			this->Controls->Add(this->btn_C2);
			this->Controls->Add(this->btn_C1);
			this->Controls->Add(this->btn_B8);
			this->Controls->Add(this->btn_B7);
			this->Controls->Add(this->btn_B6);
			this->Controls->Add(this->btn_B5);
			this->Controls->Add(this->btn_B4);
			this->Controls->Add(this->btn_B3);
			this->Controls->Add(this->btn_B2);
			this->Controls->Add(this->btn_B1);
			this->Controls->Add(this->btn_MAB);
			this->Controls->Add(this->btn_ClearDevice);
			this->Controls->Add(this->btn_Query);
			this->Controls->Add(this->btn_DumpMemory);
			this->Controls->Add(this->btn_ResetDevice);
			this->Controls->Add(this->btn_Verify);
			this->Controls->Add(this->btn_EraseDevice);
			this->Controls->Add(this->btn_ExportHex);
			this->Controls->Add(this->btn_ReadDevice);
			this->Controls->Add(this->btn_OpenHexFile);
			this->Controls->Add(this->btn_ProgramVerify);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Location = System::Drawing::Point(367, 71);
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"Form1";
			this->Text = L"Alfa USB Programmer v3.0.0";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->SizeChanged += gcnew System::EventHandler(this, &Form1::Form1_SizeChanged);
			this->flowLayoutPanel1->ResumeLayout(false);
			this->flowLayoutPanel2->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

#pragma region Query Functions
		/****************************************************************************
			Function:
				QueryThreadStart

			Description:
				This function queries the attached device for the programmable memory
				regions and stores the information returned into the memoryRegions
				array.

			Precondition:
				Device must be attached

			Parameters:
				None

			Return Values:
				None

			Other:
				QueryThreadResults should be set to QUERY_RUNNING before calling
				this function (or starting a thread that calls this function).
				Once this function is complete the QueryThreadResults will contain
				the error code associated with the results of the query.

			Remarks:
				Caution should be used to only have a single instance of this
				thread running at any given point of time.
		***************************************************************************/
	private: void QueryThreadStart()
	{
		BOOTLOADER_COMMAND myCommand = { 0 };
		BOOTLOADER_COMMAND myResponse = { 0 };

		DWORD BytesWritten = 0;
		DWORD ErrorStatus = ERROR_SUCCESS;
		DWORD BytesReceived = 0;

		HANDLE WriteHandleToMyDevice = INVALID_HANDLE_VALUE;
		HANDLE ReadHandleToMyDevice = INVALID_HANDLE_VALUE;

		memoryRegionsDetected = 0;

		//Create the write file and read file handles the to the USB device
		//  that we want to talk to
		WriteHandleToMyDevice = CreateFile(MyStructureWithDetailedInterfaceDataInIt->DevicePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
		ErrorStatusWrite = GetLastError();
		ReadHandleToMyDevice = CreateFile(MyStructureWithDetailedInterfaceDataInIt->DevicePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
		ErrorStatusRead = GetLastError();

		//Set the progress bar to 10%
		progressStatus = 10;

#if defined(ENCRYPTED_BOOTLOADER)

		//Prepare the command that we want to send, in this case the QUERY
		//  device command
		myCommand.QueryDevice.WindowsReserved = 0;
		myCommand.QueryDevice.Command = GET_ENCRYPTED_FF;

		//Send the command that we prepared
		WriteFile(WriteHandleToMyDevice, myCommand.RawData, 65, &BytesWritten, 0);	//Shouldn't really do this.  Becomes infinite blocking function if it can't successfully write, for example, because the USB firmware on the microcontroller never sets the UOWN bit for the OUT endpoint.

		//Get the error status of the last transmission
		ErrorStatus = GetLastError();
		if (ErrorStatus == ERROR_SUCCESS)
		{
			//if the command was sent successfully then
			//Set the status to 20%
			progressStatus = 20;

			//Try to read a packet from the device
			ReadFile(ReadHandleToMyDevice, myResponse.RawData, 65, &BytesReceived, 0);	//Shouldn't really do this.  Becomes infinite blocking function if it can't successfully write, for example, because the USB firmware on the microcontroller never sets the UOWN bit for the OUT endpoint.

			//Get the status of the read request
			ErrorStatus = GetLastError();
			if (ErrorStatus == ERROR_SUCCESS)
			{
				//If we were able to successfully read from the device
				unsigned char i;

#if defined(DEBUG_THREADS) && defined(DEBUG_USB)
				DEBUG_OUT("*** ENCRYPTED 0xFF RESULTS ***");
				printBuffer(myResponse.PacketData.Data, 64);
#endif

				//set the progress to 30%
				progressStatus = 30;

				encryptionBlockSize = myResponse.GetEncryptedFFResults.blockSize;

				//for each of the possible memory regions
				for (i = 0; i < myResponse.GetEncryptedFFResults.blockSize; i++)
				{
					encryptedFF[i] = myResponse.GetEncryptedFFResults.Data[i];
				}
#if defined(DEBUG_THREADS)
				DEBUG_OUT(HexToString(encryptionBlockSize, 1));
				printBuffer(encryptedFF, encryptionBlockSize);
				DEBUG_OUT("********************************************");
#endif
			}
			else
			{
				//If the read from the device failed then indicate the failure
				//  in the results variable
				QueryThreadResults = QUERY_READ_FILE_FAILED;
				//Close the write and read files 
				CloseHandle(WriteHandleToMyDevice);
				CloseHandle(ReadHandleToMyDevice);

				//We are done so set the progress to 100%
				progressStatus = 100;
				return;
			}
		}
		else
		{
			//If the write to the device failed then indicate the failure in 
			//  the results variable
			QueryThreadResults = QUERY_WRITE_FILE_FAILED;

			//Close the write and read files 
			CloseHandle(WriteHandleToMyDevice);
			CloseHandle(ReadHandleToMyDevice);

			//We are done so set the progress to 100%
			progressStatus = 100;
			return;
		}

#endif

		//Prepare the command that we want to send, in this case the QUERY
		//  device command
		myCommand.QueryDevice.WindowsReserved = 0;
		myCommand.QueryDevice.Command = QUERY_DEVICE;
		myCommand.QueryDevice.IdFwProgramming = deviceID;

#ifdef USE_PASSWORD
		memcpy(myCommand.QueryDevice.Password, mediconPassword, PWD_LGTH);
#endif

		//Send the command that we prepared
		WriteFile(WriteHandleToMyDevice, myCommand.RawData, 65, &BytesWritten, 0);	//Shouldn't really do this.  Becomes infinite blocking function if it can't successfully write, for example, because the USB firmware on the microcontroller never sets the UOWN bit for the OUT endpoint.

		//Get the error status of the last transmission
		ErrorStatus = GetLastError();
		if (ErrorStatus == ERROR_SUCCESS)
		{
			//if the command was sent successfully then
			//Set the status to 50%
			progressStatus = 50;

			//Try to read a packet from the device
			ReadFile(ReadHandleToMyDevice, myResponse.RawData, 65, &BytesReceived, 0);	//Shouldn't really do this.  Becomes infinite blocking function if it can't successfully write, for example, because the USB firmware on the microcontroller never sets the UOWN bit for the OUT endpoint.

			//Get the status of the read request
			ErrorStatus = GetLastError();
			if (ErrorStatus == ERROR_SUCCESS)
			{
				//If we were able to successfully read from the device
				unsigned char i;

#if defined(DEBUG_THREADS) && defined(DEBUG_USB)
				DEBUG_OUT("*** QUERY RESULTS ***");
				printBuffer(myResponse.PacketData.Data, 64);
#endif

				//set the progress to 90%
				progressStatus = 90;

				//for each of the possible memory regions
				for (i = 0; i < sizeof(memoryRegions); i++)
				{
					//If the type of region is 0xFF that means that we have
					//  reached the end of the regions array.
					if (myResponse.QueryResults.MemoryRegions[i].Type == 0xFF)
					{
						break;
					}

					//copy the data from the packet to the local memory regions array
					memoryRegions[i].Type = myResponse.QueryResults.MemoryRegions[i].Type;
					memoryRegions[i].Address = myResponse.QueryResults.MemoryRegions[i].Address;
					memoryRegions[i].Size = myResponse.QueryResults.MemoryRegions[i].Size;
					memoryRegionsDetected++;

#if defined(DEBUG_THREADS)
					DEBUG_OUT(HexToString(memoryRegions[i].Type, 1));
					DEBUG_OUT(HexToString(memoryRegions[i].Address, 4));
					DEBUG_OUT(HexToString(memoryRegions[i].Size, 4));
					DEBUG_OUT("********************************************");
#endif

				}

#if defined(DEBUG_THREADS)
				DEBUG_OUT(HexToString(memoryRegionsDetected, 1));
#endif

				//copy the last of the data out of the results packet
				switch (myResponse.QueryResults.DeviceFamily)
				{
				case DEVICE_FAMILY_PIC18:
					bytesPerAddress = 1;
					ckbox_ConfigWordProgramming_restore = true;
					break;
				case DEVICE_FAMILY_PIC24:
					bytesPerAddress = 2;
					ckbox_ConfigWordProgramming_restore = true;
					break;
				case DEVICE_FAMILY_PIC32:
					bytesPerAddress = 1;
					ckbox_ConfigWordProgramming_restore = false;
					break;
				default:
					break;
				}
				bytesPerPacket = myResponse.QueryResults.BytesPerPacket;

#if defined(DEBUG_THREADS)
				DEBUG_OUT("********************************************");
				DEBUG_OUT(String::Concat("Bytes per address = 0x", HexToString(bytesPerAddress, 1)));
				DEBUG_OUT(String::Concat("Bytes per packet = 0x", HexToString(bytesPerPacket, 1)));
				DEBUG_OUT("********************************************");
#endif

				//Mark the results of this request as successful
				QueryThreadResults = QUERY_SUCCESS;
			}
			else
			{
				//If the read from the device failed then indicate the failure
				//  in the results variable
				QueryThreadResults = QUERY_READ_FILE_FAILED;
			}
		}
		else
		{
			//If the write to the device failed then indicate the failure in 
			//  the results variable
			QueryThreadResults = QUERY_WRITE_FILE_FAILED;
		}

		//Close the write and read files 
		CloseHandle(WriteHandleToMyDevice);
		CloseHandle(ReadHandleToMyDevice);

		//We are done so set the progress to 100%
		progressStatus = 100;
	}

#pragma endregion

#pragma region Erase Functions
			 /****************************************************************************
				 Function:
					 btn_EraseDevice_Click

				 Description:
					 This function is called when the erase button is clicked.  An erase
					 thread is is created.

				 Precondition:
					 Device must be attached

				 Parameters:
					 System::Object^  sender - the source of the event
					 System::EventArgs^  e - the event arguments for the event

				 Return Values:
					 None

				 Other:
					 This function will change the state of the bootloaderState variable
					 and thus effect the main loop.

				 Remarks:
					 None
			 ***************************************************************************/
	private: System::Void btn_EraseDevice_Click(System::Object^  sender, System::EventArgs^  e)
	{
		DEBUG_OUT(">>btn_EraseDevice pressed");

		DisableButtons();

		//update the state of the booloader state machine to reflect
		//  that we are starting an erase cycle
		bootloaderState = BOOTLOADER_ERASE;
		EraseThreadResults = ERASE_RUNNING;

		//If an erase thread already exists
		if (EraseThread)
		{
			//If it is still running
			if (EraseThread->IsAlive)
			{
				//Then we don't want to create a new one.  Only one instance
				//  of this thread should be running at any point of time
				DEBUG_OUT("Erase thread already running");
				return;
			}

			//if there is a thread but it isn't running then destroy the old
			//  one so we can make a new one.
			delete EraseThread;
		}

		//If we are in not in debugging mode then clear the status box
#if !defined(DEBUGGING)
		listBox1->Items->Clear();
#endif

		//Enable the main state machine to print out a new status
		ENABLE_PRINT();
		PRINT_STATUS("Erasing Device (no status update until complete, may take several seconds)");

#if defined(DEBUG_THREADS)
		//If we are debugging then run the erase function inline
		//  instead of in a thread so that we can print to the
		//  window.
		EraseThreadStart();
#else
		//If we are not in debugging mode then run the erase 
		//  function as a separete thread so that the user form
		//  is still responsive while the erase function is taking
		//  place
		EraseThread = gcnew Thread(gcnew ThreadStart(this, &HIDBootLoader::Form1::EraseThreadStart));
		EraseThread->Start();
#endif
	}

			 /****************************************************************************
				 Function:
					 EraseThreadStart

				 Description:
					 This function starts a new thread that erases the attached device.

				 Precondition:
					 Device must be attached

				 Parameters:
					 None

				 Return Values:
					 None

				 Other:
					 EraseThreadResults should be set to ERASE_RUNNING before calling
					 this function or creating a thread that uses this function.  The
					 EraseThreadResults variable is modified to show the results of
					 the operation.

				 Remarks:
					 Caution should be used to only have a single instance of this
					 thread running at any given point of time.
			 ***************************************************************************/
	private: System::Void EraseThreadStart(void)
	{
		BOOTLOADER_COMMAND myCommand = { 0 };
		DWORD BytesWritten = 0;
		DWORD ErrorStatus = ERROR_SUCCESS;

		HANDLE WriteHandleToMyDevice = INVALID_HANDLE_VALUE;

		//Create a new write handle to the device
		WriteHandleToMyDevice = CreateFile(MyStructureWithDetailedInterfaceDataInIt->DevicePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
		ErrorStatusWrite = GetLastError();

		//Create the command packet that we want to send to the device.  The
		//  Command should be erase and the WindowsReserved byte should be
		//  always set to 0.
		myCommand.EraseDevice.WindowsReserved = 0;
		myCommand.EraseDevice.Command = ERASE_DEVICE;

		//Send the command to the device
		WriteFile(WriteHandleToMyDevice, myCommand.RawData, 65, &BytesWritten, 0);

		//Get the status of the last transmission
		ErrorStatus = GetLastError();
		if (ErrorStatus == ERROR_SUCCESS)
		{
			EraseThreadResults = ERASE_SUCCESS;
		}
		else
		{
			EraseThreadResults = ERASE_WRITE_FILE_FAILED;
		}
	}
#pragma endregion

#pragma region Coniguration Word Lock/Unlock Functions
			 /****************************************************************************
				 Function:
					 ckbox_ConfigWordProgramming_CheckedChanged

				 Description:
					 This function is called whenever the the unlock configuration word
					 check box state is changed.

				 Precondition:
					 Device must be attached

				 Parameters:
					 System::Object^  sender - the source of the event
					 System::EventArgs^  e - the event arguments for the event

				 Return Values:
					 None

				 Other:
					 None

				 Remarks:
					 None
			 ***************************************************************************/
	private: System::Void ckbox_ConfigWordProgramming_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		listBox1->Items->Clear();
		if (ckbox_ConfigWordProgramming->Checked)
		{
			btn_EraseDevice_restore = false;
			ENABLE_PRINT();
			PRINT_STATUS("Disabling Erase button to prevent accidental erasing of the configuration words");
			ENABLE_PRINT();
			PRINT_STATUS("     without reprogramming them");
		}
		

		//Check to see if there is already an instance of a thread
		if (UnlockConfigThread)
		{
			//see if it is still alive
			if (UnlockConfigThread->IsAlive)
			{
				//if it is still alive then bail, we don't want two instances
				//  of the thread running at once
				DEBUG_OUT("Verify thread already active...");
				return;
			}
			//if an instance exists but is not alive then delete it so we
			//  create a new instance
			delete UnlockConfigThread;
		}

		btn_Verify_restore = false;
		btn_ProgramVerify_restore = false;
		btn_ExportHex_restore = false;
		btn_ReadDevice_restore = false;

		//Update the status of the checkbox to a local bool that both
		//  the thread and the main application can access
		unlockStatus = ckbox_ConfigWordProgramming->Checked;

		//Change the state of the main state machine to indicate that we are
		//  moving into the unlock config section
		bootloaderState = BOOTLOADER_UNLOCK_CONFIG;
		UnlockConfigThreadResults = UNLOCK_CONFIG_RUNNING;

#if defined(DEBUG_THREADS)
		//If we are debugging the thread functions then instead of creating
		//  a new thread, call the function in line so that we can print
		//  to the screen in the function
		UnlockConfigThreadStart();
#else
		//If we aren't debugging the thread function then create a new
		//  thread for the function so that the user application does not
		//  lock up while the USB transfers are waiting to complete
		UnlockConfigThread = gcnew Thread(gcnew ThreadStart(this, &HIDBootLoader::Form1::UnlockConfigThreadStart));
		UnlockConfigThread->Start();
#endif
	}

			 /****************************************************************************
				 Function:
					 UnlockConfigThreadStart

				 Description:
					 This function starts after the configuration word unlock box state
					 is changed.

				 Precondition:
					 Device must be attached

				 Parameters:
					 None

				 Return Values:
					 None

				 Other:
					 UnlockConfigThreadResults should be set to UNLOCK_CONFIG_RUNNING
					 before this function is called.  The state of the
					 UnlockConfigThreadResults is changed when this thread is complete

				 Remarks:
					 Caution should be used to only have a single instance of this
					 thread running at any given point of time.
			 ***************************************************************************/
	private: System::Void UnlockConfigThreadStart(void)
	{
		//First byte must = 0, otherwise doesn't work.  Also, must "send" 65 bytes exactly to get WriteFile to work.  Only 64 bytes are actually sent over bus.
		BOOTLOADER_COMMAND myCommand = { 0 };
		DWORD BytesWritten = 0;
		DWORD ErrorStatus = ERROR_SUCCESS;

		HANDLE WriteHandleToMyDevice = INVALID_HANDLE_VALUE;

		//Create a new write file handle to the device so that we can send 
		//  packets to the device
		WriteHandleToMyDevice = CreateFile(MyStructureWithDetailedInterfaceDataInIt->DevicePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
		ErrorStatusWrite = GetLastError();

		//Set up the command that needs to be sent to the device
		myCommand.UnlockConfig.WindowsReserved = 0;
		myCommand.UnlockConfig.Command = UNLOCK_CONFIG;
		if (unlockStatus)
		{
			myCommand.UnlockConfig.Setting = 0x00;	//0x00 is sub-command to unlock the config bits
		}
		else
		{
			myCommand.UnlockConfig.Setting = 0x01;	//0x01 is sub-command to lock the config bits
		}

		//Send the command to the device
		WriteFile(WriteHandleToMyDevice, myCommand.RawData, 65, &BytesWritten, 0);

		//Get the status of the last transmission
		ErrorStatus = GetLastError();
		if (ErrorStatus == ERROR_SUCCESS)
		{
			UnlockConfigThreadResults = UNLOCK_CONFIG_SUCCESS;
		}
		else
		{
			UnlockConfigThreadResults = UNLOCK_CONFIG_FAILURE;
		}
	}
#pragma endregion

#pragma region Read Functions
			 /****************************************************************************
				 Function:
					 btn_ReadDevice_Click

				 Description:
					 This function is called when the read button is pressed.  A new
					 thread is created that will read out the contents of the device
					 into the memory allocated for the device

				 Precondition:
					 pData should have valid allocated memory for each of the memory
					 ranges specified in the query results.

				 Parameters:
					 None

				 Return Values:
					 None

				 Other:
					 None

				 Remarks:
					 None
			 ***************************************************************************/
	private: System::Void btn_ReadDevice_Click(System::Object^  sender, System::EventArgs^  e)
	{
		DEBUG_OUT(">>btn_ReadDevice pressed");

		//disable all of the buttons on the form until the read is complete
		DisableButtons();

		//If there is already an instance of the read thread
		if (ReadThread)
		{
			//if it is still running
			if (ReadThread->IsAlive)
			{
				//then bail.  We don't want two instances of the same thread running
				//  at any given point of time
				DEBUG_OUT("Read thread already running");
				return;
			}
		}

		//change the state of the bootloader main state machine and the read state machine
		bootloaderState = BOOTLOADER_READ;
		ReadThreadResults = PROGRAM_RUNNING;

		//If we are in not in debugging mode then clear the status box
#if !defined(DEBUGGING)
		listBox1->Items->Clear();
#endif

		ENABLE_PRINT();

#if defined(DEBUG_THREADS)
		//If we are debugging the threads then call the thread function in line 
		//  so that we are able to print to the status box
		ReadThreadStart();
#else
		//If we aren't debugging the threads then spawn a new thread for the read
		//  function so that the user application does not lock up while the read
		//  is in process.
		ReadThread = gcnew Thread(gcnew ThreadStart(this, &HIDBootLoader::Form1::ReadThreadStart));
		ReadThread->Start();
#endif
	}

			 /****************************************************************************
				 Function:
					 ReadThreadStart

				 Description:
					 This function reads out all of the memory specified in the query
					 results command and stores it in the allocated memory.

				 Precondition:
					 pData has valid pointers to allocated memory the correct size
					 for each of the memory ranges specified in the query results
					 command.

				 Parameters:
					 None

				 Return Values:
					 None

				 Other:
					 ReadThreadResults is READ_RUNNING while this thread is running.
					 ReadThreadResults contains either READ_SUCCESS or one of the
					 failure codes defined in the constants definition section.  This
					 thread also changes the value of the progressStatus thus changing
					 the state of the progress bar on the form.

				 Remarks:
					 Caution should be used to verify that only one instance of this
					 thread is running at any point of time.  While it may not be
					 harmful to the device to have multiple instances of this thread
					 running, it could cause a deadlock of the program or other
					 malfunctioning behavior resulting in poor user experience.
			 ***************************************************************************/
	private: void ReadThreadStart()
	{
		BOOTLOADER_COMMAND myCommand = { 0 };
		BOOTLOADER_COMMAND myCommand2 = { 0 };
		BOOTLOADER_COMMAND myResponse = { 0 };

		DWORD BytesWritten = 0;
		DWORD BytesWritten2 = 0;
		DWORD ErrorStatus = ERROR_SUCCESS;
		DWORD BytesReceived = 0;
		DWORD BytesReceived2 = 0;
		DWORD ErrorStatusSave = 0xFFFFFFFF;

		DWORD packetsWritten = 0;
		DWORD packetsRead = 0;
		unsigned char* pSave;

		unsigned char* p;
		DWORD AddressToRequest;
		unsigned long size;
		unsigned char i, currentMemoryRegion;
		bool foundError = false;

		unsigned int PendingPackets = 0;

		unsigned char* lastAddress = 0;

		HANDLE AsyncWriteHandleToMyDevice = INVALID_HANDLE_VALUE;
		HANDLE AsyncReadHandleToMyDevice = INVALID_HANDLE_VALUE;

		OVERLAPPED OverlappedWriteStructure;
		OVERLAPPED OverlappedWriteStructure2;
		OVERLAPPED OverlappedReadStructure;

		HANDLE WriteCompleteEvent;
		HANDLE WriteCompleteEvent2;
		HANDLE ReadCompleteEvent;

		WriteCompleteEvent = CreateEvent(NULL, TRUE, TRUE, (LPCTSTR)"WriteEvent");
		WriteCompleteEvent2 = CreateEvent(NULL, TRUE, TRUE, (LPCTSTR)"WriteEvent2");
		ReadCompleteEvent = CreateEvent(NULL, TRUE, TRUE, (LPCTSTR)"ReadEvent");

		OverlappedWriteStructure.Internal = 0;
		OverlappedWriteStructure.InternalHigh = 0;
		OverlappedWriteStructure.Offset = 0;
		OverlappedWriteStructure.OffsetHigh = 0;
		OverlappedWriteStructure.Pointer = 0;
		OverlappedWriteStructure.hEvent = WriteCompleteEvent;

		OverlappedWriteStructure2.Internal = 0;
		OverlappedWriteStructure2.InternalHigh = 0;
		OverlappedWriteStructure2.Offset = 0;
		OverlappedWriteStructure2.OffsetHigh = 0;
		OverlappedWriteStructure2.Pointer = 0;
		OverlappedWriteStructure2.hEvent = WriteCompleteEvent2;

		OverlappedReadStructure.Internal = 0;
		OverlappedReadStructure.InternalHigh = 0;
		OverlappedReadStructure.Offset = 0;
		OverlappedReadStructure.OffsetHigh = 0;
		OverlappedReadStructure.Pointer = 0;
		OverlappedReadStructure.hEvent = ReadCompleteEvent;

		SetEvent(ReadCompleteEvent);
		SetEvent(WriteCompleteEvent2);
		SetEvent(WriteCompleteEvent);

		/* If the function succeeds, the return value is an open handle to the specified file, device, named pipe, or mail slot. */
		AsyncWriteHandleToMyDevice = CreateFile(MyStructureWithDetailedInterfaceDataInIt->DevicePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		ErrorStatus = GetLastError();
		AsyncReadHandleToMyDevice = CreateFile(MyStructureWithDetailedInterfaceDataInIt->DevicePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		ErrorStatus = GetLastError();

		//Read
		for (currentMemoryRegion = 0; currentMemoryRegion < memoryRegionsDetected; currentMemoryRegion++)
		{
			AddressToRequest = memoryRegions[currentMemoryRegion].Address;
			size = memoryRegions[currentMemoryRegion].Size;
			p = getMemoryRegion(currentMemoryRegion);
			pSave = getMemoryRegion(currentMemoryRegion);

			myCommand.GetData.Command = GET_DATA;
			myCommand.GetData.Address = AddressToRequest;
			myCommand.GetData.BytesPerPacket = bytesPerPacket;
			myCommand.GetData.WindowsReserved = 0;

			if ((AddressToRequest + (bytesPerPacket / bytesPerAddress)) > (memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size))
			{
				myCommand.GetData.BytesPerPacket = (unsigned char)(bytesPerPacket - (((AddressToRequest + (bytesPerPacket / bytesPerAddress)) - (memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size))*bytesPerAddress));
			}

			while ((myCommand.GetData.BytesPerPacket % bytesPerAddress) != 0)
			{
				myCommand.GetData.BytesPerPacket++;
			}

			if ((AddressToRequest < (memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size)))
			{
				OverlappedWriteStructure.Internal = 0;
				OverlappedWriteStructure.InternalHigh = 0;
				OverlappedWriteStructure.Offset = 0;
				OverlappedWriteStructure.OffsetHigh = 0;
				OverlappedWriteStructure.Pointer = 0;
				OverlappedWriteStructure.hEvent = WriteCompleteEvent;

				if (WriteFile(AsyncWriteHandleToMyDevice, &myCommand, 65, &BytesWritten, &OverlappedWriteStructure))
				{
					PendingPackets++;
					AddressToRequest += (bytesPerPacket / bytesPerAddress);
					packetsWritten++;

#if defined(DEBUG_USB) && defined(DEBUG_THREADS)
					DEBUG_OUT(">> OUT Data packet");
					printBuffer(myCommand.PacketData.Data, 64);
					DEBUG_OUT("");
#endif
				}
				else //probably because I/O pending, but need to make certain
				{
					ErrorStatusSave = GetLastError();
					if (ErrorStatusSave == ERROR_IO_PENDING)
					{
						PendingPackets++;
						packetsWritten++;
						AddressToRequest += (bytesPerPacket / bytesPerAddress);

#if defined(DEBUG_USB) && defined(DEBUG_THREADS)
						DEBUG_OUT(">> OUT Data packet");
						printBuffer(myCommand.PacketData.Data, 64);
						DEBUG_OUT("");
#endif
					}
				}
			}

			if (PendingPackets > 0)
			{
				OverlappedReadStructure.Internal = 0;
				OverlappedReadStructure.InternalHigh = 0;
				OverlappedReadStructure.Offset = 0;
				OverlappedReadStructure.OffsetHigh = 0;
				OverlappedReadStructure.Pointer = 0;
				OverlappedReadStructure.hEvent = ReadCompleteEvent;
				SetEvent(ReadCompleteEvent);

				myResponse.GetDataResults.WindowsReserved = 0;

				if (ReadFile(AsyncReadHandleToMyDevice, myResponse.RawData, 65, &BytesReceived, &OverlappedReadStructure))
				{
				}
				else //probably because I/O pending, but need to make certain
				{
					ErrorStatusSave = GetLastError();
				}
			}


			while ((PendingPackets != 0) || (AddressToRequest < (memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size)))
			{
				progressStatus = (unsigned char)(((100 * AddressToRequest) / (memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size)));

				if ((HasOverlappedIoCompleted(&OverlappedWriteStructure)) && (PendingPackets < 15))
				{
					myCommand.GetData.Command = GET_DATA;
					myCommand.GetData.Address = AddressToRequest;
					myCommand.GetData.BytesPerPacket = bytesPerPacket;
					myCommand.GetData.WindowsReserved = 0;

					if ((AddressToRequest + (bytesPerPacket / bytesPerAddress)) > (memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size))
					{
						myCommand.GetData.BytesPerPacket = (unsigned char)(bytesPerPacket - (((AddressToRequest + (bytesPerPacket / bytesPerAddress)) - (memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size))*bytesPerAddress));
					}

					while ((myCommand.GetData.BytesPerPacket % bytesPerAddress) != 0)
					{
						myCommand.GetData.BytesPerPacket++;
					}

					OverlappedWriteStructure.Internal = 0;
					OverlappedWriteStructure.InternalHigh = 0;
					OverlappedWriteStructure.Offset = 0;
					OverlappedWriteStructure.OffsetHigh = 0;
					OverlappedWriteStructure.Pointer = 0;
					OverlappedWriteStructure.hEvent = WriteCompleteEvent;

					if ((AddressToRequest < (memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size)))
					{
						if (WriteFile(AsyncWriteHandleToMyDevice, &myCommand, 65, &BytesWritten, &OverlappedWriteStructure))
						{
							PendingPackets++;
							packetsWritten++;
							AddressToRequest += (bytesPerPacket / bytesPerAddress);

#if defined(DEBUG_USB) && defined(DEBUG_THREADS)
							DEBUG_OUT(">> OUT Data packet");
							printBuffer(myCommand.PacketData.Data, 64);
							DEBUG_OUT("");
#endif
						}
						else //probably because I/O pending, but need to make certain
						{
							ErrorStatusSave = GetLastError();
							if (ErrorStatusSave == ERROR_IO_PENDING)
							{
								PendingPackets++;
								packetsWritten++;
								AddressToRequest += (bytesPerPacket / bytesPerAddress);

#if defined(DEBUG_USB) && defined(DEBUG_THREADS)
								DEBUG_OUT(">> OUT Data packet");
								printBuffer(myCommand.PacketData.Data, 64);
								DEBUG_OUT("");
#endif
							}
						}
					}//address requested is still in rage
				}//Overlapped write complete

				if ((HasOverlappedIoCompleted(&OverlappedWriteStructure2)) && (PendingPackets < 15))
				{
					myCommand2.GetData.Command = GET_DATA;
					myCommand2.GetData.Address = AddressToRequest;
					myCommand2.GetData.BytesPerPacket = bytesPerPacket;
					myCommand2.GetData.WindowsReserved = 0;

					if ((AddressToRequest + (bytesPerPacket / bytesPerAddress)) > (memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size))
					{
						myCommand2.GetData.BytesPerPacket = (unsigned char)(bytesPerPacket - (((AddressToRequest + (bytesPerPacket / bytesPerAddress)) - (memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size))*bytesPerAddress));
					}

					while ((myCommand.GetData.BytesPerPacket % bytesPerAddress) != 0)
					{
						myCommand2.GetData.BytesPerPacket++;
					}

					OverlappedWriteStructure2.Internal = 0;
					OverlappedWriteStructure2.InternalHigh = 0;
					OverlappedWriteStructure2.Offset = 0;
					OverlappedWriteStructure2.OffsetHigh = 0;
					OverlappedWriteStructure2.Pointer = 0;
					OverlappedWriteStructure2.hEvent = WriteCompleteEvent2;

					if ((AddressToRequest < (memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size)))
					{
						if (WriteFile(AsyncWriteHandleToMyDevice, &myCommand2, 65, &BytesWritten2, &OverlappedWriteStructure2))
						{
							PendingPackets++;
							packetsWritten++;
							AddressToRequest += (bytesPerPacket / bytesPerAddress);

#if defined(DEBUG_USB) && defined(DEBUG_THREADS)
							DEBUG_OUT(">> OUT Data packet");
							printBuffer(myCommand2.PacketData.Data, 64);
							DEBUG_OUT("");
#endif
						}
						else //probably because I/O pending, but need to make certain
						{
							ErrorStatusSave = GetLastError();
							if (ErrorStatusSave == ERROR_IO_PENDING)
							{
								PendingPackets++;
								packetsWritten++;
								AddressToRequest += (bytesPerPacket / bytesPerAddress);

#if defined(DEBUG_USB) && defined(DEBUG_THREADS)
								DEBUG_OUT(">> OUT Data packet");
								printBuffer(myCommand2.PacketData.Data, 64);
								DEBUG_OUT("");
#endif
							}
						}
					}
				}

				if (HasOverlappedIoCompleted(&OverlappedReadStructure))
				{
					//We have successfully received a packet so decrease the number
					//  of transmissions that we are waiting to complete and increase
					//  the total number of packets successfully read
					PendingPackets--;
					packetsRead++;

					//Get a pointer to the preallocated data
					pSave = (p + (myResponse.GetDataResults.Address - memoryRegions[currentMemoryRegion].Address)*bytesPerAddress);

					//Copy all of the data received data in the preallocated memory
					for (i = 0; i < myResponse.GetDataResults.BytesPerPacket; i++)
					{
						*pSave++ = myResponse.GetDataResults.Data[sizeof(myResponse.GetDataResults.Data) - myResponse.GetDataResults.BytesPerPacket + i];// = tempData = *pSave++;
					}

#if defined(DEBUG_USB) && defined(DEBUG_THREADS)
					DEBUG_OUT("<< IN Data packet");
					printBuffer(myResponse.PacketData.Data, 64);
					DEBUG_OUT("");
#endif

					//If we are still waiting for more packets to arrive
					if (PendingPackets > 0)
					{
						//Prepare another transfer
						OverlappedReadStructure.Internal = 0;
						OverlappedReadStructure.InternalHigh = 0;
						OverlappedReadStructure.Offset = 0;
						OverlappedReadStructure.OffsetHigh = 0;
						OverlappedReadStructure.Pointer = 0;
						OverlappedReadStructure.hEvent = ReadCompleteEvent;

						myResponse.GetDataResults.WindowsReserved = 0;
						
						//initiate a read from the device
						if (ReadFile(AsyncReadHandleToMyDevice, myResponse.RawData, 65, &BytesReceived, &OverlappedReadStructure))
						{
							//If the read was successful, do nothing.  We need to wait for the 
							//  read to complete
						}
						else //probably because I/O pending, but need to make certain
						{
							//If the read resulted in an error then save the last error that
							//  we received
							ErrorStatusSave = GetLastError();
						}
					}
				}//has overlapped completed
			}	//while
		}//for loop Read
		ENABLE_PRINT();
		ReadThreadResults = READ_SUCCESS;
		progressStatus = 100;
		CloseHandle(AsyncWriteHandleToMyDevice);
		CloseHandle(AsyncReadHandleToMyDevice);
	}
#pragma endregion

#pragma region Verify Functions
			 /****************************************************************************
				 Function:
					 VerifyThreadStart

				 Description:
					 This function is the main body of the verification process.  This
					 thread reads all of the memory regions reported in the query command
					 and compares them to the contents loaded in the RAM.

				 Precondition:
					 pData should be loaded with the memory that needs to be verified
					 against.  At a minimum pData should have allocated enough memory
					 to cover the memory regions listed in the memoryRegions array.

				 Parameters:
					 None

				 Return Values:
					 None

				 Other:
					 VerifyThreadResults is VERIFY_RUNNING while this thread is running.
					 VerifyThreadResults contains either VERIFY_SUCCESS or one of the
					 failure codes defined in the constants definition section.  This
					 thread also changes the value of the progressStatus thus changing
					 the state of the progress bar on the form.

				 Remarks:
					 Caution should be used to verify that only one instance of this
					 thread is running at any point of time.  While it may not be
					 harmful to the device to have multiple instances of this thread
					 running, it could cause a deadlock of the program or other
					 malfunctioning behavior resulting in poor user experience.
			 ***************************************************************************/
	private: void VerifyThreadStart()
	{
		BOOTLOADER_COMMAND myCommand = { 0 };
		BOOTLOADER_COMMAND myResponse = { 0 };
		DWORD BytesWritten = 0;
		DWORD ErrorStatus = ERROR_SUCCESS;
		DWORD BytesReceived = 0;
		DWORD ErrorStatusSave = 0xFFFFFFFF;

		DWORD packetsWritten = 0;
		DWORD packetsRead = 0;

		unsigned char* pSave;

		unsigned char* p;
		DWORD AddressToRequest;
		unsigned long size;
		unsigned char i, currentMemoryRegion;
		bool foundError = false;
		unsigned char tempData, tempData2;

		unsigned char* lastAddress = 0;


		HANDLE WriteHandleToMyDevice = INVALID_HANDLE_VALUE;
		HANDLE ReadHandleToMyDevice = INVALID_HANDLE_VALUE;

		WriteHandleToMyDevice = CreateFile(MyStructureWithDetailedInterfaceDataInIt->DevicePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
		ErrorStatusWrite = GetLastError();
		ReadHandleToMyDevice = CreateFile(MyStructureWithDetailedInterfaceDataInIt->DevicePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
		ErrorStatusRead = GetLastError();

		if (deviceID == DEVICE_TINTING)
		{
			ENABLE_PRINT();
			VerifyThreadResults = VERIFY_SUCCESS;
			progressStatus = 100;
			CloseHandle(WriteHandleToMyDevice);
			CloseHandle(ReadHandleToMyDevice);
			return;
		}
		else
		{
			//Verify
			for (currentMemoryRegion = 0; currentMemoryRegion < memoryRegionsDetected; currentMemoryRegion++)
			{
				//If we aren't suppose to program config words then we shouldn't
				//  verify them.
				if (ckbox_ConfigWordProgramming->Checked == false)
				{
					//If this region is configuration memory
					if (memoryRegions[currentMemoryRegion].Type == 0x03)
					{
						//continue back to the top of the loop
						continue;
					}
				}


				/* Preparazione delle variabili con i dati che dovranno poi essere inseriti nel pacchetto */
				AddressToRequest = memoryRegions[currentMemoryRegion].Address;
				size = memoryRegions[currentMemoryRegion].Size;
				p = getMemoryRegion(currentMemoryRegion);
				pSave = getMemoryRegion(currentMemoryRegion);


				while (AddressToRequest < (memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size))
				{
					/* Preparazone del pacchetto */
					myCommand.GetData.Command = GET_DATA;
					myCommand.GetData.Address = AddressToRequest;
					myCommand.GetData.BytesPerPacket = bytesPerPacket;
					myCommand.GetData.WindowsReserved = 0;

					if ((AddressToRequest + (bytesPerPacket / bytesPerAddress)) > (memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size))
					{
						myCommand.GetData.BytesPerPacket = (unsigned char)(bytesPerPacket - (((AddressToRequest + (bytesPerPacket / bytesPerAddress)) - (memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size))*bytesPerAddress));
					}

					while ((myCommand.GetData.BytesPerPacket % bytesPerAddress) != 0)
					{
						myCommand.GetData.BytesPerPacket++;
					}
					/* incremento della progress bar */
					progressStatus = (unsigned char)(((100 * (AddressToRequest - memoryRegions[currentMemoryRegion].Address)) / memoryRegions[currentMemoryRegion].Size));
					/* scrittura del primo pacchetto sulla pipe */
					WriteFile(WriteHandleToMyDevice, myCommand.RawData, 65, &BytesWritten, 0);
					ErrorStatus = GetLastError();
					if (ErrorStatus == ERROR_SUCCESS) /* cioh non ci sono stati errori nella procedura di scrittura*/
					{
						packetsWritten++;
						AddressToRequest += (bytesPerPacket / bytesPerAddress);
						//Try to read a packet from the device
						ReadFile(ReadHandleToMyDevice, myResponse.RawData, 65, &BytesReceived, 0); //Shouldn't really do this.  Becomes infinite blocking function if it can't successfully write, for example, because the USB firmware on the microcontroller never sets the UOWN bit for the OUT endpoint.
						ErrorStatus = GetLastError();
						if (ErrorStatus == ERROR_SUCCESS) // Se il pacchetto h stato letto correttamente
						{
							/* Controllo di non aver ricevuto nessun pacchetto nullo */
							if ((myResponse.GetDataResults.Address == PACK_NULL) || (myResponse.GetDataResults.BytesPerPacket == PACK_NULL) || (myResponse.GetDataResults.Command == PACK_NULL))
							{
								ENABLE_PRINT();
								VerifyThreadResults = VERIFY_MISMATCH_FAILURE;
								foundError = true;
								progressStatus = 100;
								return;
							}
							else
							{
								packetsRead++;
								pSave = (p + (myResponse.GetDataResults.Address - memoryRegions[currentMemoryRegion].Address)*bytesPerAddress);
								//Copy all of the data received data in the preallocated memory.
								//Confronto fra i dati ricevuti e quelli in memoria
								for (i = 0; i < myResponse.GetDataResults.BytesPerPacket; i++)
								{
									tempData = *pSave++;
									tempData2 = myResponse.GetDataResults.Data[sizeof(myResponse.GetDataResults.Data) - myResponse.GetDataResults.BytesPerPacket + i];
									if (tempData != tempData2)
									{
										if ((bytesPerAddress == 2) && (((i + 1) % 4) == 0))
										{
											//This is a PIC24 (bytesPerAddress == 2)
											//  we are on the fourth byte of an address
											//  we can ignore this byte
										}
	#if !defined(ENCRYPTED_BOOTLOADER)
										else if ((bytesPerAddress == 2) && (myResponse.GetDataResults.Address == PIC24_RESET_REMAP_OFFSET))
										{
											unsigned char k;
											unsigned char *tempPtr;

											for (k = 0; k < memoryRegionsDetected; k++)
											{
												if (memoryRegions[k].Address == PIC24_RESET_REMAP_OFFSET)
												{
													tempPtr = getMemoryRegion(k);
												}
											}

											switch (i)
											{
											case 0:
												if (tempData2 == *tempPtr)
												{
													break;
												}
												//Fall-through
											case 1:
												if (tempData2 == *(tempPtr + 1))
												{
													break;
												}
												//Fall-through
											case 2:
												if (tempData2 == *(tempPtr + 2))
												{
													break;
												}
											case 3:
												if (tempData2 == *(tempPtr + 3))
												{
													break;
												}
											case 4:
												if (tempData2 == *(tempPtr + 4))
												{
													break;
												}
											case 5:
												if (tempData2 == *(tempPtr + 5))
												{
													break;
												}
												//Fall-through
											default:
												//Any instruction other than 0x1400 that is different is an error
	#if defined(DEBUG_THREADS) //&& defined(DEBUG_USB)
												DEBUG_OUT("<< Failing IN packet");
												printBuffer(myResponse.PacketData.Data, 64);
												DEBUG_OUT("");
	#endif

	#if defined(DEBUG_THREADS)
												DEBUG_OUT("---- Failing Address Information ----");
												DEBUG_OUT(String::Concat(" Address = 0x", HexToString(myResponse.GetDataResults.Address + i, 4), " Expected = 0x", HexToString(tempData, 1), " Got = 0x", HexToString(tempData2, 1)));
												DEBUG_OUT("");
	#endif

												ENABLE_PRINT();
												VerifyThreadResults = VERIFY_MISMATCH_FAILURE;
												foundError = true;
												progressStatus = 100;
												return;
												break;
											}
										}
										else if ((bytesPerAddress == 2) && (myResponse.GetDataResults.Address == 0x00))
										{
											switch (i)
											{
											case 0:
												if (tempData2 == 0x00)
												{
													break;
												}
												//Fall-through
											case 1:
												if (tempData2 == 0x04)
												{
													break;
												}
												//Fall-through
											case 2:
												if (tempData2 == 0x04)
												{
													break;
												}
												//Fall-through
											case 3:
												if (tempData2 == 0x00)
												{
													break;
												}
												//Fall-through
											case 4:
												if (tempData2 == 0x00)
												{
													break;
												}
												//Fall-through
											case 5:
												if (tempData2 == 0x00)
												{
													break;
												}
												//Fall-through
											default:
												//Any instruction other than 0x1400 that is different is an error
	#if defined(DEBUG_THREADS) //&& defined(DEBUG_USB)
												DEBUG_OUT("<< Failing IN packet");
												printBuffer(myResponse.PacketData.Data, 64);
												DEBUG_OUT("");
	#endif

	#if defined(DEBUG_THREADS)
												DEBUG_OUT("---- Failing Address Information ----");
												DEBUG_OUT(String::Concat(" Address = 0x", HexToString(myResponse.GetDataResults.Address + i, 4), " Expected = 0x", HexToString(tempData, 1), " Got = 0x", HexToString(tempData2, 1)));
												DEBUG_OUT("");
	#endif

												ENABLE_PRINT();
												VerifyThreadResults = VERIFY_MISMATCH_FAILURE;
												foundError = true;
												progressStatus = 100;
												return;
												break;
											}
										}
	#endif
										else
										{
	#if defined(DEBUG_THREADS) //&& defined(DEBUG_USB)
											DEBUG_OUT("<< Failing IN packet");
											printBuffer(myResponse.PacketData.Data, 64);
											DEBUG_OUT("");
	#endif

	#if defined(DEBUG_THREADS)
											DEBUG_OUT("---- Failing Address Information ----");
											DEBUG_OUT(String::Concat(" Address = 0x", HexToString(myResponse.GetDataResults.Address + i, 4), " Expected = 0x", HexToString(tempData, 1), " Got = 0x", HexToString(tempData2, 1)));
											DEBUG_OUT("");
	#endif

											ENABLE_PRINT();
											VerifyThreadResults = VERIFY_MISMATCH_FAILURE;
											foundError = true;
											progressStatus = 100;
											return;
									}
								}

							}
						}
					}
						else // lettura del pacchetto fallita
						{
							//If the read from the device failed then indicate the failure
							//  in the results variable
							VerifyThreadResults = VERIFY_READ_FILE_FAILED;
							//Close the write and read files 
							CloseHandle(WriteHandleToMyDevice);
							CloseHandle(ReadHandleToMyDevice);

							//We are done so set the progress to 100%
							progressStatus = 100;
							return;
						}
				}
					else // scrittura del pacchetto fallita
					{
						//If the read from the device failed then indicate the failure
							//  in the results variable
						VerifyThreadResults = VERIFY_WRITE_FILE_FAILED;
						//Close the write and read files 
						CloseHandle(WriteHandleToMyDevice);
						CloseHandle(ReadHandleToMyDevice);

						//We are done so set the progress to 100%
						progressStatus = 100;
						return;
					}
			}
		}
	}
		ENABLE_PRINT();
		VerifyThreadResults = VERIFY_SUCCESS;
		progressStatus = 100;
		CloseHandle(WriteHandleToMyDevice);
		CloseHandle(ReadHandleToMyDevice);
	}

			 /****************************************************************************
				 Function:
					 btn_Verify_Click

				 Description:
					 This function is called when the "Verify" Button is clicked.  This
					 function checks to see if a verify thread is already running.  If
					 there isn't already a verify thread running then this function will
					 create a new verify thread and then exit letting the verify thread
					 run.

				 Precondition:
					 None

				 Parameters:
					 System::Object^  sender - the source of the event
					 System::EventArgs^  e - the event arguments for the event

				 Return Values:
					 None

				 Remarks:
					 bootloaderState changed to BOOTLOADER_VERIFY if a new verify
					 thread is created.
			 ***************************************************************************/
	private: System::Void btn_Verify_Click(System::Object^  sender, System::EventArgs^  e)
	{
		DisableButtons();

		bootloaderState = BOOTLOADER_VERIFY;
		VerifyThreadResults = VERIFY_RUNNING;

		if (VerifyThread)
		{
			if (VerifyThread->IsAlive)
			{
				DEBUG_OUT("Verify thread already active...");
				return;
			}
		}

		//If we are in not in debugging mode then clear the status box
#if !defined(DEBUGGING)
		listBox1->Items->Clear();
#endif

		ENABLE_PRINT();

#if defined(DEBUG_THREADS)
		VerifyThreadStart();
#else
		VerifyThread = gcnew Thread(gcnew ThreadStart(this, &HIDBootLoader::Form1::VerifyThreadStart));
		VerifyThread->Start();
#endif

	}
#pragma endregion

#pragma region Hex File Functions

			 /****************************************************************************
				 Function:
					 btn_OpenHexFile_Click

				 Description:
					 This function will open a file open box letting the user select a
					 hex file to open.  When a file is selected this function will also
					 read the hex file out and copy it into the allocated memory.  Only
					 addresses in the specified ranges are saved

				 Precondition:
					 pData should have enough memory allocated to them to cover the memory
					 regions specified in query command.

				 Parameters:
					 Object^ sender - the orgin of the event
					 EventArgs^ e - the arguments of the event

				 Return Values:
					 None

				 Other:
					 None

				 Remarks:
					 None
			 ***************************************************************************/
	private: System::Void btn_OpenHexFile_Click(System::Object^  sender, System::EventArgs^  e)
	{
		//Variables required for reading a file
		String ^fileSupportBuffer;
		FileStream ^fileSupportFileStream;
		StreamReader ^fileSupportStreamReader;
		Stream^ myStream;

		unsigned char i;
		bool hexFileError;
		unsigned long extendedAddress;
		bool hexFileEOF;

		DisableButtons();

		//Create a new instance of a OpenFileDialog box
		OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;

		//Set the parameters for the open file dialog box
		openFileDialog1->Filter = "Hex files (*.hex)|*.hex|All files (*.*)|*.*";
		openFileDialog1->FilterIndex = 1;
		openFileDialog1->RestoreDirectory = true;

		//Try to delete any previous file objects if they exist
		try
		{
			if (fileSupportFileStream)
			{
				delete fileSupportFileStream;
			}
		}
		catch (...) {}

		try
		{
			if (fileSupportStreamReader)
			{
				fileSupportStreamReader->Close();
				delete (IDisposable^)fileSupportStreamReader;
			}
		}
		catch (...) {}

		//Set the hex file error detection to false initially
		hexFileError = false;

		//Open the dialog
		if (openFileDialog1->ShowDialog() == ::System::Windows::Forms::DialogResult::OK)
		{
			//if they pressed OK and a file was selected
			if ((myStream = openFileDialog1->OpenFile()) != nullptr)
			{
				//If there was a file selected
				try
				{
					//Try to open the file to read
					fileSupportFileStream = gcnew FileStream(openFileDialog1->FileName, FileMode::Open, FileAccess::Read);
					fileSupportStreamReader = gcnew StreamReader(fileSupportFileStream);
				}
				catch (...)
				{
					//If we couldn't open the file then destroy any of the file
					//  support variables that may exist
					if (fileSupportFileStream)
					{
						fileSupportFileStream->Close();
						delete fileSupportFileStream;
					}

					if (myStream)
					{
						myStream->Close();
						delete myStream;
					}

					//There was an error so bail
					return;
				}

				//Initially indicate that we haven't reached the end of file
				hexFileEOF = false;

				//Start to read the file by reading a new line
				while ((fileSupportBuffer = fileSupportStreamReader->ReadLine()) && (hexFileEOF == false))
				{
					unsigned long recordLength;
					unsigned long addressField;
					unsigned long recordType;
					unsigned long checksum;
					String^ dataPayload;

					//Trim off the white space from the line
					fileSupportBuffer = fileSupportBuffer->Trim();

					//If the line wasn't blank
					if (fileSupportBuffer->Length > 0)
					{
						if (fileSupportBuffer[0] != ':')
						{
							//If the first character of the line wasn't ":" then
							//  something is wrong with the hex file
							DEBUG_OUT("ERROR: no leading ':' in row");
							hexFileError = true;
						}
						else
						{
							//remove the ":" from the record
							fileSupportBuffer = fileSupportBuffer->Substring(1, (fileSupportBuffer->Length - 1));
						}

						//get the record length, address, record type, data, and checksum of the line
						recordLength = StringToHex(fileSupportBuffer->Substring(0, 2));
						String^ temp = fileSupportBuffer->Substring(2, 4);
						addressField = StringToHex(fileSupportBuffer->Substring(2, 4));
						recordType = StringToHex(fileSupportBuffer->Substring(6, 2));
						dataPayload = fileSupportBuffer->Substring(8, recordLength * 2);
						checksum = StringToHex(fileSupportBuffer->Substring((recordLength * 2) + 8, 2));

						unsigned char j;
						unsigned checksumCalculated;

						//Initialize the checksum value to 0
						checksumCalculated = 0;

						//For each byte in the data payload length
						for (j = 0; j < (recordLength + 4); j++)
						{
							//Calculate it into the checksum
							checksumCalculated += StringToHex(fileSupportBuffer->Substring(j * 2, 2));
						}
						//complete the checksum calculation
						checksumCalculated = (~checksumCalculated) + 1;

						//If the calculated checksum doesn't match the checksum read
						//  from the file
						if ((checksumCalculated & 0x000000FF) != checksum)
						{
							//then error out of the function
							DEBUG_OUT("ERROR: Checksum error");
							hexFileError = true;
						}

						if (hexFileError == false)
						{
							switch (recordType)
							{
							case HEX_FILE_EXTENDED_LINEAR_ADDRESS:
								//if this record is an extended address record then 
								//  save off the extended address value so we can later
								//  add it to each of the address fields that we read
								extendedAddress = StringToHex(dataPayload);
								break;
							case HEX_FILE_EOF:
								hexFileEOF = true;
								break;
							case HEX_FILE_DATA:
							{
								unsigned long totalAddress;
DWORD pippo;
								bool savedData;
								bool foundMemoryRegion;

								savedData = false;
								foundMemoryRegion = false;

								//The total address is the extended address plus the current
								//  address field.
								totalAddress = (extendedAddress << 16) + addressField;
								if (totalAddress >= 0x5800)
									pippo = 1;
								//for each of the valid memory regions we got from the query
								//  command
								for (i = 0; i < memoryRegionsDetected; i++)
								{
									pData = getMemoryRegion(i);
									//If the total address read from the hex file falls within 
									//  the valid memory range found in the query results
									if ((totalAddress >= (memoryRegions[i].Address * bytesPerAddress)) && (totalAddress < ((memoryRegions[i].Address + memoryRegions[i].Size) * bytesPerAddress)))
									{
										for (j = 0; j < (recordLength); j++)
										{
											unsigned long data;
											unsigned char *p;
											unsigned char *limit;

											//Record the data from the hex file into the memory allocated
											//  for that specific memory region.
											p = (unsigned char*)((totalAddress - (memoryRegions[i].Address * bytesPerAddress)) + j);
											data = StringToHex(dataPayload->Substring(j * 2, 2));
											p = (unsigned char*)(pData + (totalAddress - (memoryRegions[i].Address * bytesPerAddress)) + j);
											limit = (unsigned char*)(pData + ((memoryRegions[i].Size + 1)*bytesPerAddress));
											if (p >= limit)
											{
												break;
											}
											if (bytesPerAddress == 2)
											{
#if !defined(ENCRYPTED_BOOTLOADER)
												if ((totalAddress + j) < 0x6)
												{
													unsigned char k;
													unsigned char *userResetVectorPtr;

													switch ((totalAddress + j))
													{
													case 0:
														*pData = 0x00;
														break;
													case 1:
													case 2:
														*(pData + j) = 0x04;
														break;
													case 3:
													case 4:
													case 5:
														*(pData + j) = 0x00;
													default:
														*pData = (unsigned char)(data);
														break;
													}

													for (k = 0; k < memoryRegionsDetected; k++)
													{
														if (memoryRegions[k].Address == PIC24_RESET_REMAP_OFFSET)
														{
															pData = getMemoryRegion(k);
															userResetVectorPtr = (unsigned char*)(pData + totalAddress + j);
															*userResetVectorPtr = (unsigned char)(data);
															pData = getMemoryRegion(i);
														}
													}
												}
												else if ((totalAddress + j) == (PIC24_RESET_REMAP_OFFSET * 2))
												{
												}
												else if ((totalAddress + j) == ((PIC24_RESET_REMAP_OFFSET * 2) + 1))
												{
												}
												else if ((totalAddress + j) == ((PIC24_RESET_REMAP_OFFSET * 2) + 2))
												{
												}
												else if ((totalAddress + j) == ((PIC24_RESET_REMAP_OFFSET * 2) + 3))
												{
												}
												else if ((totalAddress + j) == ((PIC24_RESET_REMAP_OFFSET * 2) + 4))
												{
												}
												else if ((totalAddress + j) == ((PIC24_RESET_REMAP_OFFSET * 2) + 5))
												{
												}
												else
#endif
												{
													*p = (unsigned char)(data);
												}
											}
											else
											{
												*p = (unsigned char)(data);
											}
										}
										break;
									}
								}
								break;
							}
							default:
								break;
							}
						}
						else
						{
							//If there was an error in the hex file then 
							//  return from the function
							DEBUG_OUT("ERROR: Error in hex file somewhere");

							try
							{
								if (fileSupportFileStream)
								{
									fileSupportFileStream->Close();
									delete fileSupportFileStream;
								}
							}
							catch (...) {}

							try
							{
								if (fileSupportStreamReader)
								{
									fileSupportStreamReader->Close();
									delete (IDisposable^)fileSupportStreamReader;
								}
							}
							catch (...) {}

							try
							{
								if (myStream)
								{
									myStream->Close();
									delete myStream;
								}
							}
							catch (...) {}

							return;
						}
					}
				}

				DEBUG_OUT("Loading Hex File Complete");

				//If the hex file completed successfully, then enable any buttons
				//  that are valid now that we have loaded data.
				btn_ProgramVerify_restore = true;
				btn_EraseDevice_restore = true;
				btn_ExportHex_restore = true;
				btn_Verify_restore = true;
				Disable_ProgramVerify = 0;
			}
			else
			{
				DEBUG_OUT("---- Couldn't read the specified hex file ----");
			}
		}
		else
		{
			DEBUG_OUT("---- Open hex file terminated ----");
		}
		try
		{
			if (fileSupportFileStream)
			{
				fileSupportFileStream->Close();
				delete fileSupportFileStream;
			}
		}
		catch (...) {}

		try
		{
			if (fileSupportStreamReader)
			{
				fileSupportStreamReader->Close();
				delete (IDisposable^)fileSupportStreamReader;
			}
		}
		catch (...) {}

		try
		{
			if (myStream)
			{
				myStream->Close();
				delete myStream;
			}
		}
		catch (...) {}
	}

			 /****************************************************************************
				 Function:
					 btn_ExportHex_Click

				 Description:
					 This function opens a "save as" box and allows the user to select
					 where they want to save the contents of the allocated memory.
					 This function then writes all of the data in the allocated memory to
					 the specified file

				 Precondition:
					 pData should contain the data that needs to be exported.

				 Parameters:
					 Object^  sender - the source of the event that caused this function
						 to run
					 EventArgs^  e - the arguments of the event that caused this function
						 to run

				 Return Values:
					 None

				 Other:
					 None

				 Remarks:
					 None
			 ***************************************************************************/
	private: System::Void btn_ExportHex_Click(System::Object^  sender, System::EventArgs^  e)
	{
		FileStream ^fileSupportFileStream;
		StreamWriter ^fileSupportStreamWriter;

		DisableButtons();

		//Open a show dialog box for the "Save As" for the hex file
		if (dialog_ExportHex->ShowDialog() == ::System::Windows::Forms::DialogResult::OK)
		{
			try
			{
				//Try to create a write stream to the specified file
				fileSupportStreamWriter = gcnew StreamWriter(dialog_ExportHex->FileName, false);
			}
			catch (Exception^ e)
			{
				std::string unmanaged
					(msclr::interop::marshal_as<std::string>(e->Message));
				
				//If there was an error, print it out and delete any created 
				//  objects
				PRINT_STATUS(unmanaged);

				if (fileSupportFileStream)
				{
					delete fileSupportFileStream;
				}
				//There was an error
				return;
			}

			DWORD BytesWritten = 0;
			DWORD ErrorStatus = ERROR_SUCCESS;
			DWORD BytesReceived = 0;

			unsigned char* p;
			DWORD address;
			unsigned long size;
			unsigned char i, currentMemoryRegion;
			unsigned char bytesThisLine;
			unsigned char checksum;
			unsigned char dataByte;
			String ^ printString;
			unsigned int lastVector, thisVector;

			//Verify
			for (currentMemoryRegion = 0; currentMemoryRegion < memoryRegionsDetected; currentMemoryRegion++)
			{
				//get the address, size, and pointer to the data for the current memory region
				address = (memoryRegions[currentMemoryRegion].Address * bytesPerAddress);
				size = memoryRegions[currentMemoryRegion].Size;
				p = getMemoryRegion(currentMemoryRegion);

				//calculate the checksum for the initial extended address vector for this memory region
				checksum = 0x01 + ~(0x02 + 0x04 + (unsigned char)((address >> 24) & 0xFF) + (unsigned char)((address >> 16) & 0xFF));

				//write the extended address vector for this memory region
				fileSupportStreamWriter->WriteLine(String::Concat(":02000004", HexToString((address >> 16), 2), HexToString(checksum, 1)));

				//preconfigure the last vector as the one we just wrote
				lastVector = ((address >> 16) & 0xFFFF);;

				while (1)
				{
					//calculate the vector that we are on
					thisVector = ((address >> 16) & 0xFFFF);

					//if the last vector doesn't match the vector for this address
					if (thisVector != lastVector)
					{
						//We need to write a new vector address to the hex file for
						//  this memory address.  First calculate a new checksum for
						//  this entry.
						checksum = 0x01 + ~(0x02 + 0x04 + (unsigned char)((address >> 24) & 0xFF) + (unsigned char)((address >> 16) & 0xFF));

						//Then write the extended address vector to the hex file
						fileSupportStreamWriter->WriteLine(String::Concat(":02000004", HexToString((address >> 16), 2), HexToString(checksum, 1)));
					}

					//zero out the checksum so we can start calcluating
					//  the checksum for the data row
					checksum = 0x00;

					//Add the ":" as the first item of the line
					printString = ":";

					//preset the number of bytes on this line as the maximum
					//  bytes per line as defined in the constants section
					bytesThisLine = HEX_FILE_BYTES_PER_LINE;

					//If the remaining size of the memory region doesn't have
					//  that many bytes left in it
					if ((address + HEX_FILE_BYTES_PER_LINE) > ((memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size)*bytesPerAddress))
					{
						//then readjust the size of the number of bytes in this line
						//  to be the number of remaining bytes
						bytesThisLine = (unsigned char)(HEX_FILE_BYTES_PER_LINE - ((address + HEX_FILE_BYTES_PER_LINE) - ((memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size)*bytesPerAddress)));
					}

					//add the number of bytes in this line to the line
					printString = String::Concat(printString, HexToString(bytesThisLine, 1));

					//Add the number of bytes in this line to the checksum calculation
					checksum += ~(bytesThisLine)+1;

					//add the address of this line to the line
					printString = String::Concat(printString, HexToString(address, 2));

					//add the address of this line to the checksum
					checksum += ~((unsigned char)(address & 0xFF)) + 1;
					checksum += ~((unsigned char)((address >> 8) & 0xFF)) + 1;

					//specify that this is a data row
					printString = String::Concat(printString, "00");

					for (i = 0; i < bytesThisLine; i++)
					{
						//Get the data from the allocated memory region, add it to
						//  the line that will be printed to the hex file, and also
						//  add it to the checksum calculation
						dataByte = *p++;
						printString = String::Concat(printString, HexToString(dataByte, 1));
						checksum += ~dataByte + 1;
					}

					//Finally add the checksum on to the end of the line that we will
					//  be printing
					printString = String::Concat(printString, HexToString(checksum, 1));

					//Write the line to the hex file
					fileSupportStreamWriter->WriteLine(printString);

					//increase the address to point to the address of the next line
					address += bytesThisLine;

					//If the resulting address is outside or at the end of the memory
					//  region, then we are done and we should break out of this loop
					if (address >= ((memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size)*bytesPerAddress))
					{
						break;
					}

					//Copy the last extended vector used for the next loop
					lastVector = thisVector;
				}//while
			}//verify

			//Write the end of file command, close the file, and notify the user
			fileSupportStreamWriter->WriteLine(":00000001FF");
			fileSupportStreamWriter->Close();
			PRINT_STATUS("Export completed successfully");
		}
	}
#pragma endregion

#pragma region Programming Functions

			 /****************************************************************************
				 Function:
					 btn_ProgramVerify_Click

				 Description:
					 This function is called when the Program/Verify button is clicked.
					 A new thread is created that will perform the actual program and
					 verify functions

				 Precondition:
					 pData should be loaded with the memory that needs to be programmed
					 and verified.

				 Parameters:
					 Object^ sender - the source of the event
					 EventArgs^ e - the arguments of the event

				 Return Values:
					 None

				 Other:
					 None

				 Remarks:
					 None
			 ***************************************************************************/
	private: System::Void btn_ProgramVerify_Click(System::Object^  sender, System::EventArgs^  e)
	{
		DEBUG_OUT(">>btn_ProgramVerify pressed");

		if (deviceID == 0) {
			DEBUG_OUT("No Device Selected");
			return;
		}

		DisableButtons();

		//Set the bootloader and programming states
		bootloaderState = BOOTLOADER_PROGRAM;
		ProgramThreadResults = PROGRAM_RUNNING;

		//If a programming thread already exists
		if (ProgramThread)
		{
			//and it is still running
			if (ProgramThread->IsAlive)
			{
				//then bail out of this fuction.  We don't want to have two
				//  instances of the same thread running at the same time
				DEBUG_OUT("Program thread already running");
				return;
			}

			//If there isn't a thread running but there is a instance sitting
			//  around, then let's kill that instance so we can create another
			delete ProgramThread;
		}

		//If we are in not in debugging mode then clear the status box
#if !defined(DEBUGGING)
		listBox1->Items->Clear();
#endif

		//Allow the bootloader main thread to print a message regarding the
		//  start of the programming sequence
		ENABLE_PRINT();

#if defined(DEBUG_THREADS)
		//If we are debugging the threads then call the programming thread
		//  start function inline so that we are able to print to the menu
		ProgramThreadStart();
#else
		//If we aren't debugging the threads, then let's create a new
		//  instances of the thread so that the user form does not lock up
		//  while the programming sequence is running.
		ProgramThread = gcnew Thread(gcnew ThreadStart(this, &HIDBootLoader::Form1::ProgramThreadStart));
		ProgramThread->Start();
#endif
	}

			 /****************************************************************************
				 Function:
					 ProgramThreadStart

				 Description:
					 This function is the main body of the programming process.  This
					 thread programs the contents of the allocated memory into the device
					 and verifies the results.

				 Precondition:
					 pData should be loaded with the memory that needs to be programmed
					 against.

				 Parameters:
					 None

				 Return Values:
					 None

				 Other:
					 ProgramThreadResults should be set to PROGRAM_RUNNING before this
					 function is called.  ProgramThreadResults contains either
					 PROGRAM_SUCCESS or one of the failure codes defined in the
					 constants definition section.  This	thread also changes the value
					 of the progressStatus thus changing the state of the progress
					 bar on the form.

				 Remarks:
					 Caution should be used to verify that only one instance of this
					 thread is running at any point of time.  While it may not be
					 harmful to the device to have multiple instances of this thread
					 running, it could cause a deadlock of the program or other
					 malfunctioning behavior resulting in poor user experience.
			 ***************************************************************************/
	private: void ProgramThreadStart()
	{
		BOOTLOADER_COMMAND myCommand = { 0 };
		BOOTLOADER_COMMAND myResponse = { 0 };

		DWORD BytesWritten = 0;
		DWORD ErrorStatus = ERROR_SUCCESS;
		DWORD BytesReceived = 0;

		unsigned char* p;
		DWORD address;
		unsigned long size;
		unsigned char i, currentByteInAddress, currentMemoryRegion;
		bool configsProgrammed, everythingElseProgrammed;
		bool skipBlock, blockSkipped;

		HANDLE WriteHandleToMyDevice = INVALID_HANDLE_VALUE;
		HANDLE ReadHandleToMyDevice = INVALID_HANDLE_VALUE;

		//Create a write handle to the USB device
		WriteHandleToMyDevice = CreateFile(MyStructureWithDetailedInterfaceDataInIt->DevicePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
		ErrorStatusWrite = GetLastError();
		ReadHandleToMyDevice = CreateFile(MyStructureWithDetailedInterfaceDataInIt->DevicePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
		ErrorStatusRead = GetLastError();

		configsProgrammed = false;
		everythingElseProgrammed = false;

		//Enable the main thread to print a new message that will say 
		//  that this thread has started
		ENABLE_PRINT();

		//Switch the programming thread states to indicated that we
		//  are starting the erase sequence of the programming cycle
		ProgramThreadResults = PROGRAM_RUNNING_ERASE;

		//Change the status of the progress bar to 0%
		progressStatus = 0;

#if defined(ENCRYPTED_BOOTLOADER)
		unsigned char encryptionBlockSize;
		unsigned char encryptedFF[64];

		//Prepare the command that we want to send, in this case the QUERY
		//  device command
		myCommand.QueryDevice.WindowsReserved = 0;
		myCommand.QueryDevice.Command = GET_ENCRYPTED_FF;

		//Send the command that we prepared
		WriteFile(WriteHandleToMyDevice, myCommand.RawData, 65, &BytesWritten, 0);	//Shouldn't really do this.  Becomes infinite blocking function if it can't successfully write, for example, because the USB firmware on the microcontroller never sets the UOWN bit for the OUT endpoint.

		//Get the error status of the last transmission
		ErrorStatus = GetLastError();
		if (ErrorStatus == ERROR_SUCCESS)
		{
			//if the command was sent successfully then
			//Set the status to 20%
			progressStatus = 20;

			//Try to read a packet from the device
			ReadFile(ReadHandleToMyDevice, myResponse.RawData, 65, &BytesReceived, 0);	//Shouldn't really do this.  Becomes infinite blocking function if it can't successfully write, for example, because the USB firmware on the microcontroller never sets the UOWN bit for the OUT endpoint.

			//Get the status of the read request
			ErrorStatus = GetLastError();
			if (ErrorStatus == ERROR_SUCCESS)
			{
				//If we were able to successfully read from the device
				unsigned char i;

#if defined(DEBUG_THREADS) && defined(DEBUG_USB)
				DEBUG_OUT("*** ENCRYPTED 0xFF RESULTS ***");
				printBuffer(myResponse.PacketData.Data, 64);
#endif

				//set the progress to 30%
				progressStatus = 30;

				encryptionBlockSize = myResponse.GetEncryptedFFResults.blockSize;

				//for each of the possible memory regions
				for (i = 0; i < myResponse.GetEncryptedFFResults.blockSize; i++)
				{
					encryptedFF[i] = myResponse.GetEncryptedFFResults.Data[i];
				}
#if defined(DEBUG_THREADS)
				DEBUG_OUT(HexToString(encryptionBlockSize, 1));
				printBuffer(encryptedFF, encryptionBlockSize);
				DEBUG_OUT("********************************************");
#endif
			}
			else
			{
				//If the read from the device failed then indicate the failure
				//  in the results variable
				ProgramThreadResults = PROGRAM_WRITE_FILE_FAILED;
				//Close the write and read files 
				CloseHandle(WriteHandleToMyDevice);
				CloseHandle(ReadHandleToMyDevice);

				//We are done so set the progress to 100%
				progressStatus = 100;
				return;
			}
		}
		else
		{
			//If the write to the device failed then indicate the failure in 
			//  the results variable
			ProgramThreadResults = PROGRAM_WRITE_FILE_FAILED;

			//Close the write and read files 
			CloseHandle(WriteHandleToMyDevice);
			CloseHandle(ReadHandleToMyDevice);

			//We are done so set the progress to 100%
			progressStatus = 100;
			return;
	}
#endif
		//Setup the packet that we want to send to the device
		myCommand.EraseDevice.WindowsReserved = 0;
		myCommand.EraseDevice.Command = ERASE_DEVICE;

		//Send the command in the myCommand variable to the device
		WriteFile(WriteHandleToMyDevice, myCommand.RawData, 65, &BytesWritten, 0);

		ErrorStatus = GetLastError();
		if (ErrorStatus == ERROR_SUCCESS)
		{
			//If we were able to successfully send the erase command to
			//  the device then let's prepare a query command to determine
			//  when the is responding to commands again
			myCommand.QueryDevice.WindowsReserved = 0;
			myCommand.QueryDevice.Command = QUERY_DEVICE;
			myCommand.QueryDevice.IdFwProgramming = deviceID;

			// lo metto anche qui per non avere problemi
#ifdef USE_PASSWORD
			memcpy(myCommand.QueryDevice.Password, mediconPassword, PWD_LGTH);
#endif

			//Set the progress bar to 50%
			progressStatus = 50;

			//send the command to the device
			WriteFile(WriteHandleToMyDevice, myCommand.RawData, 65, &BytesWritten, 0);
			ErrorStatus = GetLastError();
			if (ErrorStatus != ERROR_SUCCESS)
			{
				ENABLE_PRINT();
				ProgramThreadResults = PROGRAM_WRITE_FILE_FAILED;
				return;
			}

			//Try to read a packet from the device
			ReadFile(ReadHandleToMyDevice, myResponse.RawData, 65, &BytesReceived, 0);	//Shouldn't really do this.  Becomes infinite blocking function if it can't successfully write, for example, because the USB firmware on the microcontroller never sets the UOWN bit for the OUT endpoint.

			//Get the status of the read request
			ErrorStatus = GetLastError();
			if (ErrorStatus != ERROR_SUCCESS)
			{
				//if there was an error in reading from the device
				//  then enable the main state machine to print an
				//  error, indicate the failure, and exit this thread
				ENABLE_PRINT();
				ProgramThreadResults = PROGRAM_READ_FILE_FAILED;
				return;
			}

			//If we were able to read from the device then it is back from
			//  the erase cycle already.  Allow the main thread to print out
			//  a message to the status box and change the state of the 
			//  programming thread to indicate that it is now programming
			//  the device.
			ENABLE_PRINT();
			ProgramThreadResults = PROGRAM_RUNNING_PROGRAM;

			//if the program config words box is not checked
			if (ckbox_ConfigWordProgramming->Checked == false)
			{
				//then we don't need to program the configuration bits
				//  so mark that we already have programmed them
				configsProgrammed = true;
			}

			//While we haven't programmed everything in the device yet
			while ((configsProgrammed == false) || (everythingElseProgrammed == false))
			{
				for (currentMemoryRegion = 0; currentMemoryRegion < memoryRegionsDetected; currentMemoryRegion++)
				{
					//If we haven't programmed the configuration words then we want
					//  to do this first.  The problem is that if we have erased the
					//  configuration words and we receive a device reset before we
					//  reprogram the configuration words, then the device may not be
					//  capable of running on the USB any more.  To try to minimize the
					//  possibility of this occurrance, we first search all of the
					//  memory regions and look for any configuration regions and program
					//  these regions first.  This minimizes the time that the configuration
					//  words are left unprogrammed.

					//If the configuration words are not programmed yet
					if (configsProgrammed == false)
					{
						//If the current memory region is not a configuration section
						//  then continue to the top of the for loop and look at the
						//  next memory region.  We don't want to waste time yet looking
						//  at the other memory regions.  We will come back later for
						//  the other regions.
						if (memoryRegions[currentMemoryRegion].Type != MEMORY_REGION_CONFIG)
						{
							continue;
						}
					}
					else
					{
						//If the configuration words are already programmed then if this
						//  region is a configuration region then we want to continue
						//  back to the top of the for loop and skip over this region.
						//  We don't want to program the configuration regions twice.
						if (memoryRegions[currentMemoryRegion].Type == MEMORY_REGION_CONFIG)
						{
							continue;
						}
					}

					//Get the address, size, and data for the current memory region
					address = memoryRegions[currentMemoryRegion].Address;
					size = memoryRegions[currentMemoryRegion].Size;
					p = getMemoryRegion(currentMemoryRegion);

					//Mark that we intend to skip the first block unless we find a non-0xFF
					//  byte in the packet
					skipBlock = true;

					//Mark that we didn't skip the last block
					blockSkipped = false;

					//indicate that we are at the first byte of the current address
					currentByteInAddress = 1;

					//while the current address is less than the end address
					while (address < (memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size))
					{
						//prepare a program device command to send to the device
 						myCommand.ProgramDevice.WindowsReserved = 0;
						myCommand.ProgramDevice.Command = PROGRAM_DEVICE;
						myCommand.ProgramDevice.Address = address;

						//Update the progress status with a percentage of how many
						//  bytes are in the memory region vs how many have already been
						//  programmed
						progressStatus = (unsigned char)(((100 * (address - memoryRegions[currentMemoryRegion].Address)) / memoryRegions[currentMemoryRegion].Size));
						//progressStatus = (unsigned char)(address - memoryRegions[currentMemoryRegion].Address);

						//for as many bytes as we can fit in a packet
						for (i = 0; i < bytesPerPacket; i++)
						{
							unsigned char data;

							//load up the byte from the allocated memory into the packet
							data = *p++;
							myCommand.ProgramDevice.Data[i + (sizeof(myCommand.ProgramDevice.Data) - bytesPerPacket)] = data;

#if !defined(ENCRYPTED_BOOTLOADER)
							//if the byte wasn't 0xFF
							if (data != 0xFF)
							{
								if (bytesPerAddress == 2)
								{
									if ((address % 2) != 0)
									{
										if (currentByteInAddress == 2)
										{
											//We can skip this block because we don't care about this byte
											//  it is byte 4 of a 3 word instruction on PIC24
											//myCommand.ProgramDevice.Data[i+(sizeof(myCommand.ProgramDevice.Data)-bytesPerPacket)] = 0;
										}
										else
										{
											//Then we can't skip this block of data
											skipBlock = false;
										}
									}
									else
									{
										//Then we can't skip this block of data
										skipBlock = false;
									}
								}
								else
								{
									//Then we can't skip this block of data
									skipBlock = false;
								}
							}
#else
							if (data != encryptedFF[i%encryptionBlockSize])
							{
								//Then we can't skip this block of data
								skipBlock = false;
							}
#endif

							if (currentByteInAddress == bytesPerAddress)
							{
								//If we have written enough bytes per address to be
								//  at the next address, then increment the address
								//  variable and reset the count.  
								address++;
								currentByteInAddress = 1;
							}
							else
							{
								//If we haven't written enough bytes to fill this 
								//  address then increment the number of bytes that
								//  we have added for this address
								currentByteInAddress++;
							}

							//If we have reached the end of the memory region, then we
							//  need to pad the data at the end of the packet instead
							//  of the front of the packet so we need to shift the data
							//  to the back of the packet.
							if (address >= (memoryRegions[currentMemoryRegion].Address + memoryRegions[currentMemoryRegion].Size))
							{
								unsigned char n;

								i++;

								//for each byte of the packet
								for (n = 0; n < sizeof(myCommand.ProgramDevice.Data); n++)
								{
									if (n < i)
									{
										//move it from where it is to the the back of the packet thus
										//  shifting all of the data down
										myCommand.ProgramDevice.Data[sizeof(myCommand.ProgramDevice.Data) - n - 1] = myCommand.ProgramDevice.Data[i + (sizeof(myCommand.ProgramDevice.Data) - bytesPerPacket) - n - 1];
									}
									else
									{
										//set the remaining data values to 0
										myCommand.ProgramDevice.Data[sizeof(myCommand.ProgramDevice.Data) - n - 1] = 0;

									}
								}

								//If this was the last address then break out of the for loop
								//  that is writing bytes to the packet
								break;
							}
							}

						//The number of bytes programmed is still contained in the last loop
						//  index, i.  Copy that number into the packet that is going to the device
						myCommand.ProgramDevice.BytesPerPacket = i;

						//If the block was all 0xFF then we can just skip actually programming
						//  this device.  Otherwise enter the programming sequence
						if (skipBlock == false)
						{
							//If we skipped one block before this block then we may need
							//  to send a proramming complete command to the device before
							//  sending the data for this command.
							if (blockSkipped == true)
							{
								BOOTLOADER_COMMAND cmdProgrammingComplete = { 0 };

								//Send the programming complete command
								cmdProgrammingComplete.ProgramComplete.Command = PROGRAM_COMPLETE;
								WriteFile(WriteHandleToMyDevice, cmdProgrammingComplete.RawData, 65, &BytesWritten, 0);

								ErrorStatus = GetLastError();

								if (ErrorStatus != ERROR_SUCCESS)
								{
									ENABLE_PRINT();
									ProgramThreadResults = PROGRAM_WRITE_FILE_FAILED;
									return;
								}

								//since we have now indicated that the programming is complete
								//  then we now mark that we haven't skipped any blocks
								blockSkipped = false;
							}

#if defined(DEBUG_THREADS) && defined(DEBUG_USB)
							DEBUG_OUT(">>> USB OUT Packet >>>");
							printBuffer(myCommand.PacketData.Data, 64);
#endif
							//Send the program command to the device
							WriteFile(WriteHandleToMyDevice, myCommand.RawData, 65, &BytesWritten, 0);
							ErrorStatus = GetLastError();
							if (ErrorStatus != ERROR_SUCCESS)
							{
								ENABLE_PRINT();
								ProgramThreadResults = PROGRAM_WRITE_FILE_FAILED;
								return;
							}

							//initially mark that we are skipping the block.  We will
							//  set this back to false on the first byte we find that is 
							//  not 0xFF.
							skipBlock = true;
						}
						else
						{
							//If we are skipping this block then mark that we have skipped
							//  a block and initially mark that we will be skipping the
							//  next block.  We will set skipBlock to false if we find
							//  a byte that is non-0xFF in the next packet
							blockSkipped = true;
							skipBlock = true;
						}
						} //while

						//Now that we are done with all of the addresses in this memory region,
						//  before we move on we need to send a programming complete command to
						//  the device.
					myCommand.ProgramComplete.WindowsReserved = 0;
					myCommand.ProgramComplete.Command = PROGRAM_COMPLETE;
					WriteFile(WriteHandleToMyDevice, myCommand.RawData, 65, &BytesWritten, 0);

					ErrorStatus = GetLastError();
					if (ErrorStatus != ERROR_SUCCESS)
					{
						ENABLE_PRINT();
						ProgramThreadResults = PROGRAM_WRITE_FILE_FAILED;
						return;
					}

					}//for each memory region


				if (configsProgrammed == false)
				{
					//If the configuration bits haven't been programmed yet then the first
					//  pass through the for loop that just completed will have programmed
					//  just the configuration bits so mark them as complete.
					configsProgrammed = true;
				}
				else
				{
					//If the configuration bits were already programmed then this loop must
					//  have programmed all of the other memory regions.  Mark everything
					//  else as being complete.
					everythingElseProgrammed = true;
				}
				}//while
			} //If write file
		else
		{
			//If the write file failed then notify the user
			ENABLE_PRINT();
			ProgramThreadResults = PROGRAM_WRITE_FILE_FAILED;
			return;
		}

		//If we make it to this point then the programming completed successfully.
		//  Notify the user and mark this thread as successful
		ENABLE_PRINT();
		ProgramThreadResults = PROGRAM_SUCCESS;
		}
#pragma endregion

#pragma region USB Functions
			 /****************************************************************************
				 Function:
					 WndProc

				 Description:
					 None

				 Precondition:
					 None

				 Parameters:
					 Message% m

				 Return Values:
					 None

				 Other:
					 None

				 Remarks:
					 None
			 ***************************************************************************/
	protected: virtual void WndProc(Message% m) override {

		// Listen for Windows messages.  We will receive various different types of messages, but the ones we really want to use are the WM_DEVICECHANGE messages.
		if (m.Msg == WM_DEVICECHANGE)
		{
			if (((int)m.WParam == DBT_DEVICEARRIVAL) || ((int)m.WParam == DBT_DEVICEREMOVEPENDING) || ((int)m.WParam == DBT_DEVICEREMOVECOMPLETE) || ((int)m.WParam == DBT_CONFIGCHANGED))
			{
				MyCallBackOnWM_DEVICECHANGE(m);
			}
		}
		Form::WndProc(m);
	}

			   /****************************************************************************
				   Function:
					   MyCallBackOnWM_DEVICECHANGE

				   Description:
					   None

				   Precondition:
					   None

				   Parameters:
					   Message% m

				   Return Values:
					   None

				   Other:
					   None

				   Remarks:
					   None
			   ***************************************************************************/
	private: System::Void MyCallBackOnWM_DEVICECHANGE(Message% m) {
		if ((int)m.WParam == DBT_DEVICEARRIVAL)
		{
			DEBUG_OUT("WM_DEVICECHANGE: DBT_DEVICEARRIVAL");
		}
		if ((int)m.WParam == DBT_DEVICEREMOVECOMPLETE)
		{
			DEBUG_OUT("WM_DEVICECHANGE: DBT_DEVICEREMOVECOMPLETE");
		}
		if ((int)m.WParam == DBT_DEVICEREMOVEPENDING)
		{
			DEBUG_OUT("WM_DEVICECHANGE: DBT_DEVICEREMOVEPENDING");
		}

		HANDLE WriteHandleToMyDevice = INVALID_HANDLE_VALUE;
		HANDLE ReadHandleToMyDevice = INVALID_HANDLE_VALUE;

		Status = TryToFindHIDDeviceFromVIDPID();
		if (Status == TRUE)
		{
			//Open read and write pipes to the device, but only if they weren't already open.  We would know if they were open if MyDeviceAttachedStatus was set to true.
			if (MyDeviceAttachedStatus == false)
			{
				WriteHandleToMyDevice = CreateFile(MyStructureWithDetailedInterfaceDataInIt->DevicePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
				ErrorStatusWrite = GetLastError();
				ReadHandleToMyDevice = CreateFile(MyStructureWithDetailedInterfaceDataInIt->DevicePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
				ErrorStatusRead = GetLastError();

				if ((ErrorStatusRead == ERROR_SUCCESS) && (ErrorStatusWrite == ERROR_SUCCESS))
				{
					DeviceAttached();
					MyDeviceAttachedStatus = true;
					DEBUG_OUT("Successfully got read/write handles to device: " + MY_DEVICE_ID);
				}
				else
				{
					MyDeviceAttachedStatus = false;
					DeviceRemoved();
					DEBUG_OUT("Found the device, but could not open read/write handles.");
				}
			}
		}
		else //Status was == FALSE, could not find device with matching VID and PID
		{
			MyDeviceAttachedStatus = false;
			DeviceRemoved();
			CloseHandle(WriteHandleToMyDevice);		//Close any previously open handles in case user presses button more than once.  This won't necessarily be a problem.  Depending upon the sharing
			CloseHandle(ReadHandleToMyDevice);		//access rights, we may not be allowed to have multiple open handles of some types to the same device at once.  Therefore, we close them before opening them.
			DEBUG_OUT("Could not find device: " + MY_DEVICE_ID);
		}
	}

			 /****************************************************************************
				 Function:
					 TryToFindHIDDeviceFromVIDPID

				 Description:
					 None

				 Precondition:
					 None

				 Parameters:
					 unsigned short vid
					 unsigned short pid

				 Return Values:
					 None

				 Other:
					 None

				 Remarks:
					 None
			 ***************************************************************************/
			 bool TryToFindHIDDeviceFromVIDPID(void)
			 {
				 /*
				 Before we can "connect" our application to our USB embedded device, we must first find the device.
				 A USB bus can have many devices simultaneously connected, so somehow we have to find our device, and only
				 our device.  This is done with the Vendor ID (VID) and Product ID (PID).  Each USB product line should have
				 a unique combination of VID and PID.

				 Microsoft has created a number of functions which are useful for finding plug and play devices.  Documentation
				 for each function used can be found in the MSDN library.  We will be using the following functions:

				 SetupDiGetClassDevs()					//provided by setupapi.dll, which comes with Windows
				 SetupDiEnumDeviceInterfaces()			//provided by setupapi.dll, which comes with Windows
				 GetLastError()							//provided by kernel32.dll, which comes with Windows
				 SetupDiDestroyDeviceInfoList()			//provided by setupapi.dll, which comes with Windows
				 SetupDiGetDeviceInterfaceDetail()		//provided by setupapi.dll, which comes with Windows
				 SetupDiGetDeviceRegistryProperty()		//provided by setupapi.dll, which comes with Windows
				 malloc()								//part of C runtime library, msvcrt.dll?
				 CreateFile()							//provided by kernel32.dll, which comes with Windows

				 We will also be using the following unusual data types and structures.  Documentation can also be found in
				 the MSDN library:

				 PSP_DEVICE_INTERFACE_DATA
				 PSP_DEVICE_INTERFACE_DETAIL_DATA
				 SP_DEVINFO_DATA
				 HDEVINFO
				 HANDLE

				 The ultimate objective of the following code is to get the device path, which is needed for CreateFile(), which opens
				 a communications pipe to a specific device (such as a HID class USB device endpoint).  CreateFile() returns a "handle"
				 which is needed later when calling ReadFile() or WriteFile().  These functions are used to actually send and receive
				 application related data to/from the USB peripheral device.

				 However, in order to call CreateFile(), we first need to get the device path for the USB device
				 with the correct VID and PID.  Getting the device path is a multi-step round about process, which
				 requires calling several of the SetupDixxx() functions provided by setupapi.dll.
				 */


				 HDEVINFO DeviceInfoTable = INVALID_HANDLE_VALUE;
				 PSP_DEVICE_INTERFACE_DATA InterfaceDataStructure = new SP_DEVICE_INTERFACE_DATA;
				 //PSP_DEVICE_INTERFACE_DETAIL_DATA MyStructureWithDetailedInterfaceDataInIt = new SP_DEVICE_INTERFACE_DETAIL_DATA;	//Make this global, so we can pass the device path to various CreateFile() calls all over the program

				 SP_DEVINFO_DATA DevInfoData;

				 DWORD InterfaceIndex = 0;
				 DWORD StatusLastError = 0;
				 DWORD dwRegType;
				 DWORD dwRegSize;
				 DWORD StructureSize = 0;
				 PBYTE PropertyValueBuffer;
				 bool MatchFound = false;
				 DWORD ErrorStatus;

				 String^ DeviceIDToFind = MY_DEVICE_ID;

				 //First populate a list of plugged in devices (by specifying "DIGCF_PRESENT"), which are of the specified class GUID. 
				 DeviceInfoTable = SetupDiGetClassDevsUM(&InterfaceClassGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

				 //Now look through the list we just populated.  We are trying to see if any of them match our device. 
				 while (true)
				 {
					 InterfaceDataStructure->cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
					 if (SetupDiEnumDeviceInterfacesUM(DeviceInfoTable, NULL, &InterfaceClassGuid, InterfaceIndex, InterfaceDataStructure))
					 {
						 ErrorStatus = GetLastError();
						 if (ERROR_NO_MORE_ITEMS == ErrorStatus)	//Did we reach the end of the list of matching devices in the DeviceInfoTable?
						 {	//Cound not find the device.  Must not have been attached.
							 SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
							 return false;
						 }
					 }
					 else	//Else some other kind of unknown error ocurred...
					 {
						 ErrorStatus = GetLastError();
						 SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
						 return false;
					 }

					 //Now retrieve the hardware ID from the registry.  The hardware ID contains the VID and PID, which we will then 
					 //check to see if it is the correct device or not.

					 //Initialize an appropriate SP_DEVINFO_DATA structure.  We need this structure for SetupDiGetDeviceRegistryProperty().
					 DevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
					 SetupDiEnumDeviceInfoUM(DeviceInfoTable, InterfaceIndex, &DevInfoData);

					 //First query for the size of the hardware ID, so we can know how big a buffer to allocate for the data.
					 SetupDiGetDeviceRegistryPropertyUM(DeviceInfoTable, &DevInfoData, SPDRP_HARDWAREID, &dwRegType, NULL, 0, &dwRegSize);

					 //Allocate a buffer for the hardware ID.
					 PropertyValueBuffer = (BYTE *)malloc(dwRegSize);
					 if (PropertyValueBuffer == NULL)	//if null, error, couldn't allocate enough memory
					 {	//Can't really recover from this situation, just exit instead.
						 SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
						 return false;
					 }

					 //Retrieve the hardware IDs for the current device we are looking at.  PropertyValueBuffer gets filled with a 
					 //REG_MULTI_SZ (array of null terminated strings).  To find a device, we only care about the very first string in the
					 //buffer, which will be the "device ID".  The device ID is a string which contains the VID and PID, in the example 
					 //format "Vid_04d8&Pid_003f".
					 SetupDiGetDeviceRegistryPropertyUM(DeviceInfoTable, &DevInfoData, SPDRP_HARDWAREID, &dwRegType, PropertyValueBuffer, dwRegSize, NULL);

					 //Now check if the first string in the hardware ID matches the device ID of my USB device.
#ifdef UNICODE
					 String^ DeviceIDFromRegistry = gcnew String((wchar_t *)PropertyValueBuffer);
#else
					 String^ DeviceIDFromRegistry = gcnew String((char *)PropertyValueBuffer);
#endif

					 free(PropertyValueBuffer);		//No longer need the PropertyValueBuffer, free the memory to prevent potential memory leaks

					 //Convert both strings to lower case.  This makes the code more robust/portable across OS Versions
					 DeviceIDFromRegistry = DeviceIDFromRegistry->ToLowerInvariant();
					 DeviceIDToFind = DeviceIDToFind->ToLowerInvariant();
					 //Now check if the hardware ID we are looking at contains the correct VID/PID
					 MatchFound = DeviceIDFromRegistry->Contains(DeviceIDToFind);
					 if (MatchFound == true)
					 {
						 //Device must have been found.  (Goal: Open read and write handles)  In order to do this, we will need the actual device path first.
						 //We can get the path by calling SetupDiGetDeviceInterfaceDetail(), however, we have to call this function twice:  The first
						 //time to get the size of the required structure/buffer to hold the detailed interface data, then a second time to actually 
						 //get the structure (after we have allocated enough memory for the structure.)
						 MyStructureWithDetailedInterfaceDataInIt->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
						 //First call populates "StructureSize" with the correct value
						 SetupDiGetDeviceInterfaceDetailUM(DeviceInfoTable, InterfaceDataStructure, NULL, NULL, &StructureSize, NULL);
						 MyStructureWithDetailedInterfaceDataInIt = (PSP_DEVICE_INTERFACE_DETAIL_DATA)(malloc(StructureSize));		//Allocate enough memory
						 if (MyStructureWithDetailedInterfaceDataInIt == NULL)	//if null, error, couldn't allocate enough memory
						 {	//Can't really recover from this situation, just exit instead.
							 SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
							 return false;
						 }
						 MyStructureWithDetailedInterfaceDataInIt->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
						 //Now call SetupDiGetDeviceInterfaceDetail() a second time to receive the goods.  
						 SetupDiGetDeviceInterfaceDetailUM(DeviceInfoTable, InterfaceDataStructure, MyStructureWithDetailedInterfaceDataInIt, StructureSize, NULL, NULL);

						 //We now have the proper device path, and we can finally open read and write handles to the device.
						 SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
						 return true;		//We are also returning the device path which is contained inside the global structure MyStructureWithDetailedInterfaceDataInIt.
											 //We will use the path when we later call CreateFile() to get handles to the device.
					 }

					 InterfaceIndex++;
					 //Keep looping until we either find a device with matching VID and PID, or until we run out of items.
				 }//end of while(true)	
			 }//end of TryToFindHIDDeviceFromVIDPID()	 




			 /****************************************************************************
				 Function:
					 DeviceAttached

				 Description:
					 None

				 Precondition:
					 None

				 Parameters:
					 None

				 Return Values:
					 None

				 Other:
					 None

				 Remarks:
					 None
			 ***************************************************************************/
	private: System::Void DeviceAttached(void)
	{
		bootloaderState = BOOTLOADER_QUERY;
		QueryThreadResults = QUERY_RUNNING;

		deviceAttached = true;

		listBox1->Items->Clear();
		ENABLE_PRINT();
		PRINT_STATUS("Device attached.");

#if defined(ENCRYPTED_BOOTLOADER)
		ckbox_ConfigWordProgramming->Checked = TRUE;
		ckbox_ConfigWordProgramming_restore = FALSE;
		ckbox_ConfigWordProgramming_CheckedChanged(this, gcnew System::EventArgs());
		ckbox_ConfigWordProgramming->Enabled = FALSE;
#endif

		if (QueryThread)
		{
			if (QueryThread->IsAlive)
			{
				DEBUG_OUT("Query thread already running");
				return;
			}
		}

#if defined(DEBUG_THREADS)
		QueryThreadStart();
#else
		QueryThread = gcnew Thread(gcnew ThreadStart(this, &HIDBootLoader::Form1::QueryThreadStart));
		QueryThread->Start();
#endif
	}


			 /****************************************************************************
				 Function:
					 DeviceRemoved

				 Description:
					 None

				 Precondition:
					 None

				 Parameters:
					 None

				 Return Values:
					 None

				 Other:
					 None

				 Remarks:
					 None
			 ***************************************************************************/
	private: System::Void DeviceRemoved(void)
	{
		unsigned char i;

#if !defined(DEBUGGING)
		listBox1->Items->Clear();
#endif

		if (deviceAttached == true)
		{
			ENABLE_PRINT();
			PRINT_STATUS("Device removed");
		}

		unlockStatus = false;

#if !defined(DEBUG_BUTTONS)
		btn_OpenHexFile_restore = false;
		btn_ExportHex_restore = false;
		btn_ProgramVerify_restore = false;
		btn_ReadDevice_restore = false;
		btn_EraseDevice_restore = false;
		btn_Verify_restore = false;
		btn_ResetDevice_restore = false;
		ckbox_ConfigWordProgramming->Checked = false;
		ckbox_ConfigWordProgramming_restore = false;
#else
		btn_OpenHexFile_restore = true;
		btn_ExportHex_restore = true;
		btn_ProgramVerify_restore = true;
		btn_ReadDevice_restore = true;
		btn_EraseDevice_restore = true;
		btn_Verify_restore = true;
		btn_ResetDevice_restore = true;
		ckbox_ConfigWordProgramming_restore = true;
#endif

		for (i = 0; i < MAX_DATA_REGIONS; i++)
		{
			pData = getMemoryRegion(i);
			if (pData != 0)
			{
				free(pData);
				setMemoryRegion(i, 0);
			}
		}

	}
#pragma endregion

#pragma region Reset Functions
			 /****************************************************************************
				 Function:
					 btn_ResetDevice_Click

				 Description:
					 This function is called when the reset device button is pressed on
					 the main form.  If thread debugging is not enabled, this function
					 will launch a new thread that will try to send out the reset
					 command to the device.  If the reset is successful then the device
					 will likely drop off of the bus.

				 Precondition:
					 None

				 Parameters:
					 Object^ sender - the source of the event
					 EventArgs^ e - the arguments of the event

				 Return Values:
					 None

				 Other:
					 None

				 Remarks:
					 If the reset is successful then the device
					 will likely drop off of the bus.
			 ***************************************************************************/
	private: System::Void btn_ResetDevice_Click(System::Object^  sender, System::EventArgs^  e)
	{
		DEBUG_OUT(">>btn_ResetDevice Pressed");

		listBox1->Items->Clear();
		DisableButtons();

#if !defined(ENCRYPTED_BOOTLOADER)
		ckbox_ConfigWordProgramming->Checked = false;
#endif
		ckbox_ConfigWordProgramming_restore = false;
		unlockStatus = false;

		//update the state of the booloader state machine to reflect
		//  that we are starting a reset
		bootloaderState = BOOTLOADER_RESET;
		ResetThreadResults = RESET_RUNNING;

		//If an reset thread already exists
		if (ResetThread)
		{
			//If it is still running
			if (ResetThread->IsAlive)
			{
				//Then we don't want to create a new one.  Only one instance
				//  of this thread should be running at any point of time
				DEBUG_OUT("Reset thread already running");
				return;
			}

			//if there is a thread but it isn't running then destroy the old
			//  one so we can make a new one.
			delete ResetThread;
		}

		//Enable the main state machine to print out a new status
		ENABLE_PRINT();

#if defined(DEBUG_THREADS)
		//If we are debugging then run the erase function inline
		//  instead of in a thread so that we can print to the
		//  window.
		ResetThreadStart();
#else
		//If we are not in debugging mode then run the erase 
		//  function as a separete thread so that the user form
		//  is still responsive while the erase function is taking
		//  place
		ResetThread = gcnew Thread(gcnew ThreadStart(this, &HIDBootLoader::Form1::ResetThreadStart));
		ResetThread->Start();
#endif
	}

			 /****************************************************************************
				 Function:
					 ResetThreadStart

				 Description:
					 This function starts a new thread that resets the attached device.

				 Precondition:
					 Device must be attached

				 Parameters:
					 None

				 Return Values:
					 None

				 Other:
					 ResetThreadResults should be set to RESET_RUNNING before calling
					 this function or creating a thread that uses this function.  The
					 ResetThreadResults variable is modified to show the results of
					 the operation.

				 Remarks:
					 If the reset is successful then the device
					 will likely drop off of the bus.
			 ***************************************************************************/
	private: System::Void ResetThreadStart(void)
	{
		BOOTLOADER_COMMAND myCommand = { 0 };
		DWORD BytesWritten = 0;
		DWORD ErrorStatus = ERROR_SUCCESS;

		HANDLE WriteHandleToMyDevice = INVALID_HANDLE_VALUE;

		//Update the progress status to 0%
		progressStatus = 0;

		//Create a new write handle to the device
		WriteHandleToMyDevice = CreateFile(MyStructureWithDetailedInterfaceDataInIt->DevicePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
		ErrorStatusWrite = GetLastError();

		//Create the command packet that we want to send to the device.  The
		//  Command should be erase and the WindowsReserved byte should be
		//  always set to 0.
		myCommand.ResetDevice.WindowsReserved = 0;
		myCommand.ResetDevice.Command = RESET_DEVICE;

		//Update the progress status to 10%
		progressStatus = 10;

		//Send the command to the device
		WriteFile(WriteHandleToMyDevice, myCommand.RawData, 65, &BytesWritten, 0);

		//Get the status of the last transmission
		ErrorStatus = GetLastError();
		if (ErrorStatus == ERROR_SUCCESS)
		{
			ResetThreadResults = RESET_SUCCESS;
		}
		else
		{
			ResetThreadResults = RESET_WRITE_FILE_FAILED;
		}

		//Update the progress status to 100%
		progressStatus = 100;

	}
#pragma endregion

#pragma region Main State Machine
			 /****************************************************************************
				 Function:
					 tmr_ThreadStatus_Tick

				 Description:
					 This function is launched once a millisecond from the timer event.
					 This functions purpose is to keep track of the states of the various
					 threads and where they are in the requested actions.  This function
					 will take the results and progress status indicated by the various
					 worker threads and print the results in the status box

				 Precondition:
					 Before this timer is initially called the bootloaderState variable
					 should be set to IDLE to make sure that no action is taken until
					 the user clicks on an interface button.

				 Parameters:
					 Object^ sender - the source of the event (Form1)
					 EventArgs^ e -  the arguments of the event

				 Return Values:
					 None

				 Other:
					 The status box and progress bar are updated through this function.
					 Additional threads my also be launched from this function depending
					 on the action 
					 that was just completed and what may be required next.

				 Remarks:
					 None
			 ***************************************************************************/
	private: System::Void tmr_ThreadStatus_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		if (inTimer == true)
		{
			return;
		}

		inTimer = true;

		//If we are debugging the buttons then make sure that all of them are
		//  enabled.
#if defined(DEBUG_BUTTONS)
		btn_OpenHexFile_restore = true;
		btn_ExportHex_restore = true;
		btn_ProgramVerify_restore = true;
		btn_ReadDevice_restore = true;
		btn_EraseDevice_restore = true;
		btn_Verify_restore = true;
		btn_ResetDevice_restore = true;
		ckbox_ConfigWordProgramming_restore = true;

		btn_ClearListbox->Enabled = true;
		btn_ClearListbox->Visible = true;
#endif

		if (progressStatus > 100)
		{
			progressStatus = 100;
		}

		//Update the progress status bar
		progressBar_Status->Value = progressStatus;

		//Determine what to do based on the current bootloader state
		switch (bootloaderState)
		{
			/*****************************************************/
			/*************** BOOTLOADER IDLE *********************/
			/*****************************************************/
		case BOOTLOADER_IDLE:
			//If any of the buttons were disabled due to a thread running
			//  since that thread must now be done, reenable all of the
			//  buttons
			EnableButtons();
			break;

			/*****************************************************/
			/*************** BOOTLOADER RESET ********************/
			/*****************************************************/
		case BOOTLOADER_RESET:
			//Check the results of the read thread
			switch (ResetThreadResults)
			{
			case RESET_SUCCESS:
				//If the Read was successful then print out 
				//  a status saying it was successful
				ENABLE_PRINT();
				PRINT_STATUS("Device Successfully Reset");

				//return the bootloader and read threads to idle
				bootloaderState = BOOTLOADER_IDLE;
				ResetThreadResults = RESET_IDLE;
				break;

			case RESET_WRITE_FILE_FAILED:
				//If the Read failed then print out 
				//  a status saying it failed
				ENABLE_PRINT();
				PRINT_STATUS("Unable to reset the device");

				//return the bootloader and read threads to idle
				bootloaderState = BOOTLOADER_IDLE;
				ResetThreadResults = RESET_IDLE;
				break;

			default:
				//If it is still running then do nothing
				break;
			}
			break;

			/*****************************************************/
			/*************** BOOTLOADER UNLOCK CONFIG ************/
			/*****************************************************/
		case BOOTLOADER_UNLOCK_CONFIG:
			//Check the status of the unlock config thread results
			switch (UnlockConfigThreadResults)
			{
			case UNLOCK_CONFIG_SUCCESS:
				//If the unlock was successful, then print
				//  out a status message saying it was
				ENABLE_PRINT();
				PRINT_STATUS("Configuration bits unlocked\\locked successfully");

				//Set the current state of the bootloader and
				//  the unlock config threads to idle
				bootloaderState = BOOTLOADER_IDLE;
				UnlockConfigThreadResults = UNLOCK_CONFIG_IDLE;

				//Now that the configuration bits were either
				//  unlocked or locked, the available memory
				//  ranges may have changed so we should query
				//  the device again to find out what these new
				//  regions may be.
				bootloaderState = BOOTLOADER_QUERY;
				QueryThreadResults = QUERY_RUNNING;

				//If there is already a QueryThread
				if (QueryThread)
				{
					//If that thread is already running
					if (QueryThread->IsAlive)
					{
						//error out.  We don't want to have two instances
						//  of the thread running at the same time
						DEBUG_OUT("Query thread already running");
						inTimer = false;
						return;
					}

					//If the thread exists but isn't running then let's 
					//  delete the current instance of the thread so that
					//  we can create a new one
					delete QueryThread;
				}

#if defined(DEBUG_THREADS)
				//If we are debugging the threads then let's call the 
				//  function in line so that we can print messages to the
				//  status box
				QueryThreadStart();
#else
				//If we aren't debugging the threads then let's create a 
				//  new thread to do the query so that the main application
				//  does not lock up while the USB transactions are pending
				QueryThread = gcnew Thread(gcnew ThreadStart(this, &HIDBootLoader::Form1::QueryThreadStart));
				QueryThread->Start();
#endif
				break;

			case UNLOCK_CONFIG_FAILURE:
				//If the unlock process was a failure, then let's send a message
				//  to the status box saying it was a failure
				ENABLE_PRINT();
				PRINT_STATUS("Unabled to unlock\\lock the configuration bits");

				//Set the bootloader and unlock config thread states to idle
				bootloaderState = BOOTLOADER_IDLE;
				UnlockConfigThreadResults = UNLOCK_CONFIG_IDLE;
				break;

			default:
				//If we are in any other state, do nothing
				break;
			}
			break;

			/*****************************************************/
			/*************** BOOTLOADER READ *********************/
			/*****************************************************/
		case BOOTLOADER_READ:
			//check the status of the read thread
			switch (ReadThreadResults)
			{
			case READ_RUNNING:
				//if was are running, the notify the user that
				//  we are currently reading the device
				PRINT_STATUS("Reading Device");
				break;

			case READ_SUCCESS:
				//If the read is complete then notify the user
				ENABLE_PRINT();
				PRINT_STATUS("Read Complete");

				//Return the bootloader and read thread states to idle
				bootloaderState = BOOTLOADER_IDLE;
				ReadThreadResults = READ_IDLE;

				//enable the export hex button, since we now have
				//  information that could be exported
				btn_ExportHex_restore = true;
				break;

			case READ_WRITE_FILE_FAILED:
				//Fall through
			case READ_READ_FILE_FAILED:
				bootloaderState = BOOTLOADER_IDLE;
				ReadThreadResults = READ_IDLE;
				ENABLE_PRINT();
				PRINT_STATUS("Unabled to complete read operation");
				progressBar_Status->Value = 100;
				delete ReadThread;
				break;
			default:
				bootloaderState = BOOTLOADER_IDLE;
				ReadThreadResults = READ_IDLE;
				break;
			}
			break;

			/*****************************************************/
			/*************** BOOTLOADER VERIFY *******************/
			/*****************************************************/
		case BOOTLOADER_VERIFY:
			DisableButtons();

			//Check the current state of the verify thread
			switch (VerifyThreadResults)
			{
			case VERIFY_RUNNING:
				//If we are still running then print the verify
				//  started message just once.
				PRINT_STATUS("Verify Started");
				break;
			case VERIFY_SUCCESS:
				//If the verificaiton was successful,
				//  if the verify was created from the program
				//  thread then the programThreadResults will
				//  not be idle
				if (ProgramThreadResults != PROGRAM_IDLE)
				{
					//the verify is now complete to the program
					//  cycle is too.  Set the state to idle
					ProgramThreadResults = PROGRAM_IDLE;

					//And print out that the program/verify was
					//  successful
					ENABLE_PRINT();
					PRINT_STATUS("Erase/Program/Verify Completed Successfully");
					if (deviceID == DEVICE_MAB)
						this->btn_MAB->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_B1)
						this->btn_B1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_B2)
						this->btn_B2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_B3)
						this->btn_B3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_B4)
						this->btn_B4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_B5)
						this->btn_B5->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_B6)
						this->btn_B6->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_B7)
						this->btn_B7->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_B8)
						this->btn_B8->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C1)
						this->btn_C1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C2)
						this->btn_C2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C3)
						this->btn_C3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C4)
						this->btn_C4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C5)
						this->btn_C5->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C6)
						this->btn_C6->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C7)
						this->btn_C7->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C8)
						this->btn_C8->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C9)
						this->btn_C9->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C10)
						this->btn_C10->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C11)
						this->btn_C11->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C12)
						this->btn_C12->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C13)
						this->btn_C13->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C14)
						this->btn_C14->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C15)
						this->btn_C15->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C16)
						this->btn_C16->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C17)
						this->btn_C17->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C18)
						this->btn_C18->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C19)
						this->btn_C19->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C20)
						this->btn_C20->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C21)
						this->btn_C21->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C22)
						this->btn_C22->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C23)
						this->btn_C23->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_C24)
						this->btn_C24->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_X_AXIS)
						this->btn_x_axis->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
							static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_Y_AXIS)
						this->btn_y_axis->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
						static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_CONTAINER_1)
						this->btn_container_1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
						static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_CONTAINER_2)
						this->btn_container_2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
						static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_CONTAINER_3)
						this->btn_container_3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
						static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_CONTAINER_4)
						this->btn_container_4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
						static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_COVER_1)
						this->btn_cover_1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
						static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_COVER_2)
						this->btn_cover_2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
						static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_AUTOCAP)
						this->btn_autocap->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
						static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_CAN_LIFTER)
						this->btn_canlifter->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
						static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_HUMIDIFIER)
						this->btn_humidifier->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
						static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_TINTING)
						this->btn_tinting->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
						static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_UNUSED_4)
						this->btn_unused_4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
						static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_UNUSED_5)
						this->btn_unused_5->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
						static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_UNUSED_6)
						this->btn_unused_6->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
						static_cast<System::Int32>(static_cast<System::Byte>(0)));
					else if (deviceID == DEVICE_UNUSED_7)
						this->btn_unused_7->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
						static_cast<System::Int32>(static_cast<System::Byte>(0)));
						deviceID = 0;
				}
				else
				{
					//If the program state is idle then this must
					//  have been the result of the user clicking
					//  the verify button.  All we need to do here
					//  is indicate that the verify was successful
					ENABLE_PRINT();
					PRINT_STATUS("Verify Completed Successfully");
				}
		
				//In either case the verify is complete to switch the 
				//  bootloader state and the verify state to idle
				bootloaderState = BOOTLOADER_IDLE;
				VerifyThreadResults = VERIFY_IDLE;
				Disable_ProgramVerify = 1;
//				Disable_ProgramVerify = 0;

				break;

			case VERIFY_WRITE_FILE_FAILED:
				//Fall through
			case VERIFY_READ_FILE_FAILED:
				//Fall through
			case VERIFY_MISMATCH_FAILURE:
				//If the verify failed for any reason
				if (ProgramThreadResults != PROGRAM_IDLE)
				{
					//if we were in here from a program/verify sequence
					//  then the programming sequence is also complete.
					ProgramThreadResults = PROGRAM_IDLE;

					//Notify the user of the failure
					ENABLE_PRINT();
					PRINT_STATUS("Program/Verify Failure");
				}
				else
				{
					//Otherwise it was only a verify.  Notify the user.
					ENABLE_PRINT();
					PRINT_STATUS("Verify Failure");
				}

				//We are done with the verify so set the main bootloader
				//  state and the verify states to idle
				bootloaderState = BOOTLOADER_IDLE;
				VerifyThreadResults = VERIFY_IDLE;
				break;
			default:
				//if we are found to be in any other unknown state then
				//  return to the idle state
				bootloaderState = BOOTLOADER_IDLE;
				VerifyThreadResults = VERIFY_IDLE;
				break;
			}
			break;

			/*****************************************************/
			/*************** BOOTLOADER PROGRAM ******************/
			/*****************************************************/
		case BOOTLOADER_PROGRAM:
			//Check the state of the programming thread
			switch (ProgramThreadResults)
			{
			case PROGRAM_RUNNING:
				//If it is running still, do nothing
				break;

			case PROGRAM_RUNNING_ERASE:
				//When we have started the erase, notify the user
				PRINT_STATUS("Erase Started (no status update until complete, may take several seconds)");
				break;

			case PROGRAM_RUNNING_PROGRAM:
				//If we haven't printed a message yet after
				//  finishing the erase
				if (enablePrint)
				{
					//then say that the erase is complete
					PRINT_STATUS("Erase Complete");
					//and enable one more print
					ENABLE_PRINT();
				}
				//say that the programming sequence is running
				PRINT_STATUS("Programming Started");
				break;

			case PROGRAM_SUCCESS:
				//If the programming sequence was successful, then
				//  notify the user
				ENABLE_PRINT();
				PRINT_STATUS("Programming Complete");

				//since the programming is complete we want to verify
				//  the results.  Set the bootloader state to verify and
				//  set the verify thread status to running
				bootloaderState = BOOTLOADER_VERIFY;
				VerifyThreadResults = VERIFY_RUNNING;

				//Notify the user that the verify process is now running
				ENABLE_PRINT();
				PRINT_STATUS("Verify Running");

				//If there already exists a verify thread
				if (VerifyThread)
				{
					//and it is still running
					if (VerifyThread->IsAlive)
					{
						//then bail.  we don't want two running instances
						//  of any one thread
						DEBUG_OUT("Verify thread already running");
						inTimer = false;
						return;
					}

					//If the thread exists but isn't running then let's 
					//  delete the thread instance.
					delete VerifyThread;
				}

#if defined(DEBUG_THREADS)
				//If we are debugging the threads then call the function
				//  in line so that we can print to the status box
				VerifyThreadStart();
#else
				//if we aren't running in debug mode, then spawn a new 
				//  thread to handle the verify process so that the user
				//  interface doesn't lock up while the verify is running
				VerifyThread = gcnew Thread(gcnew ThreadStart(this, &HIDBootLoader::Form1::VerifyThreadStart));
				VerifyThread->Start();
#endif
				break;

			case PROGRAM_READ_FILE_FAILED:
				//Fall through
			case PROGRAM_WRITE_FILE_FAILED:
				//If the programming sequence failed, then notify the user
				ENABLE_PRINT();
				PRINT_STATUS("Error during the Program/Verify process");

				//Return the program and bootloader sequence to idle
				bootloaderState = BOOTLOADER_IDLE;
				ProgramThreadResults = PROGRAM_IDLE;
				break;

			default:
				//If we are in an unknown state then return to idle
				bootloaderState = BOOTLOADER_IDLE;
				ProgramThreadResults = PROGRAM_IDLE;
				break;
			}
			break;

			/*****************************************************/
			/*************** BOOTLOADER ERASE ********************/
			/*****************************************************/
		case BOOTLOADER_ERASE:
			//Check the status of the erase thread
			switch (EraseThreadResults)
			{
			case ERASE_RUNNING:
				//If it is running then let the user know
				PRINT_STATUS("Erase running");
				break;

			case ERASE_SUCCESS:
				//If transfer of the erase command is complete then
				//  it is still useful to know when the device is done
				//  erasing.  Since there is no response sent back to
				//  indicate when it is complete I will just query the
				//  device and when that query returns successfully I
				//  will know that the erase is done and the device is 
				//  responding to USB commands again

				//If a query thread already exists
				if (QueryThread)
				{
					//And is already running
					if (QueryThread->IsAlive)
					{
						//Bail out.  We don't want to have two instances
						//  of the same thread running at one time
						DEBUG_OUT("Query thread already running");
						inTimer = false;
						return;
					}

					//If the thread exists but isn't running then we
					//  should delete the current instance
					delete QueryThread;
				}

				//Set the query state machine to running
				QueryThreadResults = QUERY_RUNNING;

#if defined(DEBUG_THREADS)
				//If we are debugging the threads then let's call the
				//  function in line so that we can print to the status box
				QueryThreadStart();
#else
				//otherwise if we aren't debugging the thread function then
				//  let's create a new thread that will do the query process
				//  that way the user application isn't locked up while the 
				//  USB transfers are being run
				QueryThread = gcnew Thread(gcnew ThreadStart(this, &HIDBootLoader::Form1::QueryThreadStart));
				QueryThread->Start();
#endif

				//Change the erase state to indicate that we are done
				//  with the erase command and are now waiting for the
				//  post erase query to complete
				EraseThreadResults = ERASE_POST_QUERY_RUNNING;
				break;

			case ERASE_POST_QUERY_RUNNING:
				//If we are waiting for the query after the erase to complete
				//  then look at the query results to see how we are doing
				if (QueryThreadResults != QUERY_RUNNING)
				{
					//If the query is done running then
					if (QueryThreadResults == QUERY_SUCCESS)
					{
						//if the query was a success, then the erase was a success
						EraseThreadResults = ERASE_POST_QUERY_SUCCESS;
					}
					else
					{
						//if the query failed, then the erase condition is unknown
						//  and thus should be returned as a failure
						EraseThreadResults = ERASE_POST_QUERY_FAILURE;
					}
				}
				break;

			case ERASE_POST_QUERY_SUCCESS:
				//If the erase proccess + query was successful, then notify the user
				ENABLE_PRINT();
				PRINT_STATUS("Erase Complete");

				//and set the bootloader and erase states to idle
				bootloaderState = BOOTLOADER_IDLE;
				EraseThreadResults = ERASE_IDLE;
				break;

			case ERASE_POST_QUERY_FAILURE:
				//fall through
			case ERASE_WRITE_FILE_FAILED:
				//If there was an error for any reason, then notify the user
				ENABLE_PRINT();
				PRINT_STATUS("Error while erasing device");

				//and set the bootloader and erase states to idle
				bootloaderState = BOOTLOADER_IDLE;
				EraseThreadResults = ERASE_IDLE;
				break;

			default:
				//If we got into some unknown state then return to idle
				bootloaderState = BOOTLOADER_IDLE;
				EraseThreadResults = ERASE_IDLE;
				break;
			}
			break;

			/*****************************************************/
			/*************** BOOTLOADER QUERY ********************/
			/*****************************************************/
		case BOOTLOADER_QUERY:
			//Check the status of the query thread
			switch (QueryThreadResults)
			{
			case QUERY_RUNNING:
				//If we are running, notify the user
				PRINT_STATUS("Query Running");
				break;

			case QUERY_SUCCESS:
			{
				unsigned char loopCounter;
				unsigned long tempLong;
				unsigned char *tempPointer;
				bool mallocFailed;

				//If the query was successful, notify the user
				//ENABLE_PRINT();
				//PRINT_STATUS("Query Successful");

				//Enable the basic buttons that are valid now
				//  that we know a device is attached
				btn_OpenHexFile_restore = true;
				btn_ReadDevice_restore = true;
				btn_ResetDevice_restore = true;

				btn_EraseDevice_restore = true;
				if (ckbox_ConfigWordProgramming->Checked)
				{
					btn_EraseDevice_restore = false;
				}

				//initialize a variable to keep track if we were
				//  able to allocate memory for each of the specified
				//  memory regions
				mallocFailed = false;

				//for each of the possible memory regions
				for (loopCounter = 0; loopCounter < MAX_DATA_REGIONS; loopCounter++)
				{
					pData = getMemoryRegion(loopCounter);
					//If the current pointer isn't NULL
					if (pData != 0)
					{
						//Free the old data and set the pointer to 0
						free(pData);
						setMemoryRegion(loopCounter, 0);
					}
				}

				loopCounter = 0;
				while (loopCounter != 0)
				{
					loopCounter = 0;
				}
				//For each of the memory regions detected
				for (loopCounter = 0; loopCounter < memoryRegionsDetected; loopCounter++)
				{
					unsigned long size, b;

					//Get the size of the data
					size = memoryRegions[loopCounter].Size;
					b = bytesPerAddress;

					//Allocate enough memory for the memory region
					pData = (unsigned char*)malloc((memoryRegions[loopCounter].Size + 1) * bytesPerAddress);
					setMemoryRegion(loopCounter, pData);

					//If the malloc failed
					if (pData == 0)
					{
						//Print out an error message, if in debug mode
						DEBUG_OUT("??? malloc failed ???");
						DEBUG_OUT(HexToString(size, 4));
						DEBUG_OUT(HexToString(b, 4));

						//indicate that we had a malloc failure
						mallocFailed = true;
						break;
					}
				}

				//If we were unable to malloc memory for each of
				//  the memory regions then we should free the data
				//  for all of the regions and report a failure
				if (mallocFailed == true)
				{
					//Then for every region
					for (loopCounter = 0; loopCounter < MAX_DATA_REGIONS; loopCounter++)
					{
						pData = getMemoryRegion(loopCounter);

						//If we did allocate some memory
						if (pData != 0)
						{
							//free the memory and set the pointer to 0
							free(pData);
							setMemoryRegion(loopCounter, 0);
						}
					}

					//Notify the user of the failure
					ENABLE_PRINT();
					PRINT_STATUS("Application unable to allocate enough memory for the specified memory regions");

					//return the bootloader and the query thread to the idle state
					bootloaderState = BOOTLOADER_IDLE;
					QueryThreadResults = QUERY_IDLE;
					inTimer = false;
					return;
				}

				//If all of the memory allocations where successful
				//  then for each memory region
				for (loopCounter = 0; loopCounter < memoryRegionsDetected; loopCounter++)
				{
					tempPointer = getMemoryRegion(loopCounter);

					//Set all of the data in that memory region defaultly to 0xFF
					for (tempLong = 0; tempLong < (memoryRegions[loopCounter].Size * bytesPerAddress); tempLong++)
					{
#if !defined(ENCRYPTED_BOOTLOADER)
						if ((bytesPerAddress == 2) && (((tempLong + 1) % 4) == 0))
						{
							//zero out every 4th byte on the PIC24
							*tempPointer++ = 0;
						}
						else
						{
							*tempPointer++ = 0xFF;
						}
#else
						*tempPointer++ = encryptedFF[tempLong%encryptionBlockSize];
#endif
						}
					}

				//return the bootloader and the query thread to the idle state
				bootloaderState = BOOTLOADER_IDLE;
				QueryThreadResults = QUERY_IDLE;

				break;
				}
			case QUERY_WRITE_FILE_FAILED:
				//Fall through
			case QUERY_READ_FILE_FAILED:
				//if the query failed, then notify the user
				ENABLE_PRINT();
				PRINT_STATUS("Application unable to communicate to the device");

				//and return the bootloader and query threads to the idle state
				bootloaderState = BOOTLOADER_IDLE;
				QueryThreadResults = QUERY_IDLE;
				break;

			default:
				//If we are in an unknown state, return the bootloader and
				//  query thread to an idle state
				bootloaderState = BOOTLOADER_IDLE;
				QueryThreadResults = QUERY_IDLE;
				break;
			}
			break;
			} //end of the bootloader state switch statement
		inTimer = false;
		}//end of the main bootloader function
#pragma endregion

#pragma region Support Functions
/****************************************************************************
	Function:
		HexToString

	Description:
		Takes in input unsigned long and converts it to a String

	Precondition:
		None

	Parameters:
		unsigned long input - number to print to the string
		unsigned char bytes - number of bytes in input

	Return Values:
		String^ - the converted String value of the input parameter

	Other:
		None

	Remarks:
		None
***************************************************************************/
			 String^ HexToString(unsigned long input, unsigned char bytes)
			 {
				 String^ returnString;
				 wchar_t returnArray[9];

				 unsigned char i;
				 unsigned char c;

				 for (i = 0; i < 9; i++)
				 {
					 returnArray[i] = '0';
				 }

				 for (i = 0; i < bytes * 2; i++)
				 {
					 c = (unsigned char)(input & 0x0000000F);

					 if (c <= 9)
					 {
						 returnArray[7 - i] = c + '0';
					 }
					 else
					 {
						 returnArray[7 - i] = c + 'A' - 10;
					 }

					 input >>= 4;
				 }
				 returnArray[9] = 0;
				 returnString = gcnew String(returnArray);
				 returnString = returnString->Substring(8 - (bytes * 2), bytes * 2);
				 return returnString;
			 }

			 /****************************************************************************
				 Function:
					 StringToHex

				 Description:
					 Takes in the input string and converts it to an unsigned long

				 Precondition:
					 None

				 Parameters:
					 String^ s - the string that needs to be converted

				 Return Values:
					 unsigned long - the resulting number

				 Other:
					 None

				 Remarks:
					 None
			 ***************************************************************************/
			 unsigned long StringToHex(String^ s)
			 {
				 unsigned long returnAddress;
				 unsigned long placeMultiplier;
				 unsigned char i;
				 wchar_t c;

				 returnAddress = 0;
				 placeMultiplier = 1;

				 for (i = 0; i < s->Length; i++)
				 {
					 c = s[s->Length - 1 - i];
					 if ((c >= 'A') && (c <= 'F'))
					 {
						 c = 10 + (c - 'A');
					 }
					 else if ((c >= 'a') && (c <= 'f'))
					 {
						 c = 10 + (c - 'a');
					 }
					 else
					 {
						 c = c - '0';
					 }

					 returnAddress += (c * placeMultiplier);
					 placeMultiplier *= 16;
				 }

				 return returnAddress;
			 }
#pragma endregion

#pragma region Form Functions
			 /****************************************************************************
				 Function:
					 Form1_SizeChanged

				 Description:
					 This function resizes the user listbox when the window resizes.

				 Precondition:
					 None

				 Parameters:
					 Object^ sender - the source of the event (Form1)
					 EventArgs^ e - the arguments of the event

				 Return Values:
					 None

				 Other:
					 None

				 Remarks:
					 None
			 ***************************************************************************/
	private: System::Void Form1_SizeChanged(System::Object^  sender, System::EventArgs^  e) {
		//listBox1->Width = this->Size.Width - 2 * this->Padding.Horizontal;
		//listBox1->Left = 0; //  (this->Width - listBox1->Width) / 2;
		//
		//listBox1->Height = this->Size.Height * 20 / 100;
		//listBox1->Top = this->Height - listBox1->Height - 50;
	}

			 /****************************************************************************
				 Function:
					 DisableButtons

				 Description:
					 This function saves the current state of all of the buttons and
					 disables them.  Calling the EnableButtons function will restore the
					 previous state of the buttons

				 Precondition:
					 None

				 Parameters:
					 None

				 Return Values:
					 None

				 Other:
					 None

				 Remarks:
					 None
			 ***************************************************************************/
	private: void DisableButtons(void)
	{

		//Save the current state of the user inputs
		btn_Verify_restore = btn_Verify->Enabled;
		btn_ResetDevice_restore = btn_ResetDevice->Enabled;
		btn_ProgramVerify_restore = btn_ProgramVerify->Enabled;
		btn_OpenHexFile_restore = btn_OpenHexFile->Enabled;
		btn_ExportHex_restore = btn_ExportHex->Enabled;
		btn_EraseDevice_restore = btn_EraseDevice->Enabled;
		btn_ReadDevice_restore = btn_ReadDevice->Enabled;

#if defined(ENCRYPTED_BOOTLOADER)
		ckbox_ConfigWordProgramming_restore = ckbox_ConfigWordProgramming->Enabled;
#endif
	}

			 /****************************************************************************
				 Function:
					 EnableButtons

				 Description:
					 This function restores the enabled states of the buttons fromn the
					 _restore variables.  This is used in conjunction with the
					 DisableButtons function.  In order to change the state of a button,
					 one needs to modify the corresponding _restore variable as well or
					 the next time that this function is called the _restore value will
					 over write the ->Enabled member.

				 Precondition:
					 None

				 Parameters:
					 None

				 Return Values:
					 None

				 Other:
					 None

				 Remarks:
					 In order to change the state of a button,
					 one needs to modify the corresponding _restore variable as well or
					 the next time that this function is called the _restore value will
					 over write the ->Enabled member.
			 ***************************************************************************/
	private: void EnableButtons(void)
	{
 if (Enable_Functions == 7) {
		//Restore all of the saved inputs
		btn_Verify->Enabled = btn_Verify_restore;
		btn_ResetDevice->Enabled = btn_ResetDevice_restore;
		btn_ProgramVerify->Enabled = btn_ProgramVerify_restore;
		btn_OpenHexFile->Enabled = btn_OpenHexFile_restore;
		btn_ExportHex->Enabled = btn_ExportHex_restore;
		btn_EraseDevice->Enabled = btn_EraseDevice_restore;
		btn_ReadDevice->Enabled = btn_ReadDevice_restore;
		btn_DumpMemory->Enabled = true;
		btn_Query->Enabled = true;
 }
 else if (Disable_ProgramVerify == 1) {
	 btn_Verify->Enabled = false;
	 btn_ResetDevice->Enabled = false;
	 btn_ExportHex->Enabled = false;
	 btn_ProgramVerify->Enabled = false;
	 btn_OpenHexFile->Enabled = btn_OpenHexFile_restore;
	 btn_EraseDevice->Enabled = false;
	 btn_ReadDevice->Enabled = false;
	 btn_DumpMemory->Enabled = false;
	 btn_Query->Enabled = false;
 }
//Restore all of the saved inputs
 else {
		btn_Verify->Enabled = false;
		btn_ResetDevice->Enabled = false;
		btn_ExportHex->Enabled = false;
		btn_ProgramVerify->Enabled = btn_ProgramVerify_restore;
		btn_OpenHexFile->Enabled = btn_OpenHexFile_restore;
		btn_EraseDevice->Enabled = false;
		btn_ReadDevice->Enabled = false;
		btn_DumpMemory->Enabled = false;
		btn_Query->Enabled = false;
 }
	#if !defined(ENCRYPTED_BOOTLOADER)
		ckbox_ConfigWordProgramming->Enabled = ckbox_ConfigWordProgramming_restore;
#endif
	}
#pragma endregion

#pragma region Debugging Functions

#if defined(DEBUGGING)
	private: System::Void printBuffer(unsigned char* buffer, DWORD size)
	{
#define NUM_BYTES_PER_ROW 32
		DWORD i;
		String^ s;

		s = "";

		for (i = 0; i < size; i++)
		{
			if ((i%NUM_BYTES_PER_ROW) == 0)
			{
				if (i != 0)
				{
					DEBUG_OUT(s);
					s = "";
				}
				s = String::Concat("  ", HexToString(*(buffer + i), 1));
			}
			else
			{
				s = String::Concat(s, " ", HexToString(*(buffer + i), 1));
			}
		}

		if (String::Compare(s, "") != 0)
		{
			DEBUG_OUT(s);
		}
	}


	private: System::Void dumpMemoryRegions(void)
	{
		unsigned char i, k;
		unsigned char *p;
		unsigned long j;
		String^ s;
		unsigned long size;

		for (i = 0; i < memoryRegionsDetected; i++)
		{
			j = 0;
			k = bytesPerAddress;
			p = getMemoryRegion(i);
			size = memoryRegions[i].Size;
			size *= bytesPerAddress;

			DEBUG_OUT(String::Concat("****** MEMORY REGION ", HexToString(i, 1), " ******"));
			while (j < size)
			{
				if ((j % (16 * bytesPerAddress)) == 0)
				{
					if (j != 0)
					{
						DEBUG_OUT(s);
					}
					s = String::Concat(HexToString((memoryRegions[i].Address + (j / bytesPerAddress)), 4), ": ", HexToString(*(p + (2 * k) - bytesPerAddress + j - 1), 1));
				}
				else
				{
					s = String::Concat(s, HexToString(*(p + (2 * k) - bytesPerAddress + j - 1), 1));
				}

				k--;
				if (k == 0)
				{
					s = String::Concat(s, " ");
					k = bytesPerAddress;
				}


				j++;
			}
			if (((j - 1) % (16 * bytesPerAddress)) != 0)
			{
				DEBUG_OUT(s);
			}
		}
	}

#endif

			 unsigned char* getMemoryRegion(unsigned char region)
			 {
				 switch (region)
				 {
				 case 0:
					 return pData0;
				 case 1:
					 return pData1;
				 case 2:
					 return pData2;
				 case 3:
					 return pData3;
				 case 4:
					 return pData4;
				 case 5:
					 return pData5;
				 default:
					 return 0;
				 }
			 }

			 void setMemoryRegion(unsigned char region, unsigned char* p)
			 {
				 switch (region)
				 {
				 case 0:
					 pData0 = p;
					 break;
				 case 1:
					 pData1 = p;
					 break;
				 case 2:
					 pData2 = p;
					 break;
				 case 3:
					 pData3 = p;
					 break;
				 case 4:
					 pData4 = p;
					 break;
				 case 5:
					 pData5 = p;
					 break;
				 default:
					 return;
				 }
			 }

	private: System::Void btn_Query_Click(System::Object^  sender, System::EventArgs^  e) {
		DEBUG_OUT(">>btn_QueryVerify pressed");

		DisableButtons();

		//Set the bootloader and programming states
		bootloaderState = BOOTLOADER_QUERY;
		ProgramThreadResults = QUERY_RUNNING;

		//If a programming thread already exists
		if (QueryThread)
		{
			//and it is still running
			if (QueryThread->IsAlive)
			{
				//then bail out of this fuction.  We don't want to have two
				//  instances of the same thread running at the same time
				DEBUG_OUT("Query thread already running");
				return;
			}

			//If there isn't a thread running but there is a instance sitting
			//  around, then let's kill that instance so we can create another
			delete QueryThread;
		}

		//If we are in not in debugging mode then clear the status box
#if !defined(DEBUGGING)
		listBox1->Items->Clear();
#endif

		//Allow the bootloader main thread to print a message regarding the
		//  start of the programming sequence
		ENABLE_PRINT();

#if defined(DEBUG_THREADS)
		//If we are debugging the threads then call the programming thread
		//  start function inline so that we are able to print to the menu
		QueryThreadStart();
#else
		//If we aren't debugging the threads, then let's create a new
		//  instances of the thread so that the user form does not lock up
		//  while the programming sequence is running.
		QueryThread = gcnew Thread(gcnew ThreadStart(this, &HIDBootLoader::Form1::QueryThreadStart));
		QueryThread->Start();
#endif
	}

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
#if defined(DEBUGGING)
		dumpMemoryRegions();
#endif
}
	private: System::Void btn_ClearListbox_Click(System::Object^  sender, System::EventArgs^  e) {
		listBox1->Items->Clear();
	}
#pragma endregion

#pragma region Custom User Interaction Functions

	private: void addLogEntry(const std::string s)
	{
		time_t currentTime
			(std::time(nullptr));
		
		if (enablePrint) {
			std::ostringstream oss;

			oss
				<< std::ctime(&currentTime)
				<< " :: "
				<< s;

			String^ entry = gcnew String(oss.str().c_str());
			listBox1->Items->Add(entry);

			listBox1->SelectedIndex = listBox1->Items->Count - 1;
			DISABLE_PRINT();
		}
	}

	private: System::Void btn_device_Click(System::Object^  sender, System::EventArgs^  e) {
		System::Windows::Forms::Button^ origin = dynamic_cast <System::Windows::Forms::Button^>
			(sender);
		
		if ((deviceID != 0) && (origin->Name != "btn_ClearDevice")){
//			DEBUG_OUT("Another device already selected");
			PRINT_STATUS("Another device already selected");
			return;
		}
	
		if (origin->Name == "btn_MAB") {
			deviceID = DEVICE_MAB;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now MAB / Tinting Master.");
		}
		else if (origin->Name == "btn_ClearDevice") {
			deviceID = 0;
			//if (Enable_Functions != 7)
			//	Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Clear Device ID");
			return;
		}
		else if (origin->Name == "btn_B1") {
			deviceID = DEVICE_B1;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now B1.");
		}
		else if (origin->Name == "btn_B2") {
			deviceID = DEVICE_B2;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now B2.");
		}
		else if (origin->Name == "btn_B3") {
			deviceID = DEVICE_B3;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now B3.");
		}

		else if (origin->Name == "btn_B4") {
			deviceID = DEVICE_B4;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now B4.");
		}

		else if (origin->Name == "btn_B5") {
			deviceID = DEVICE_B5;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now B5.");
		}

		else if (origin->Name == "btn_B6") {
			deviceID = DEVICE_B6;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now B6.");
		}

		else if (origin->Name == "btn_B7") {
			deviceID = DEVICE_B7;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now B7.");
		}

		else if (origin->Name == "btn_B8") {
			deviceID = DEVICE_B8;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now B8.");
		}

		else if (origin->Name == "btn_C1") {
			deviceID = DEVICE_C1;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C1.");
		}

		else if (origin->Name == "btn_C2") {
			deviceID = DEVICE_C2;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C2.");
		}
		else if (origin->Name == "btn_C3") {
			deviceID = DEVICE_C3;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C3.");
		}

		else if (origin->Name == "btn_C4") {
			deviceID = DEVICE_C4;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C4.");
		}

		else if (origin->Name == "btn_C5") {
			deviceID = DEVICE_C5;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C5.");
		}

		else if (origin->Name == "btn_C6") {
			deviceID = DEVICE_C6;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C6.");
		}

		else if (origin->Name == "btn_C7") {
			deviceID = DEVICE_C7;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C7.");
		}

		else if (origin->Name == "btn_C8") {
			deviceID = DEVICE_C8;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C8.");
		}

		else if (origin->Name == "btn_C9") {
			deviceID = DEVICE_C9;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C9.");
		}

		else if (origin->Name == "btn_C10") {
			deviceID = DEVICE_C10;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C10.");
		}

		else if (origin->Name == "btn_C11") {
			deviceID = DEVICE_C11;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C11.");
		}

		else if (origin->Name == "btn_C12") {
			deviceID = DEVICE_C12;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C12.");
		}

		else if (origin->Name == "btn_C13") {
			deviceID = DEVICE_C13;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C13.");
		}

		else if (origin->Name == "btn_C14") {
			deviceID = DEVICE_C14;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C14.");
		}

		else if (origin->Name == "btn_C15") {
			deviceID = DEVICE_C15;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C15.");
		}

		else if (origin->Name == "btn_C16") {
			deviceID = DEVICE_C16;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C16.");
		}

		else if (origin->Name == "btn_C17") {
			deviceID = DEVICE_C17;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C17.");
		}

		else if (origin->Name == "btn_C18") {
			deviceID = DEVICE_C18;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C18.");
		}

		else if (origin->Name == "btn_C19") {
			deviceID = DEVICE_C19;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C19.");
		}

		else if (origin->Name == "btn_C20") {
			deviceID = DEVICE_C20;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C20.");
		}

		else if (origin->Name == "btn_C21") {
			deviceID = DEVICE_C21;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C22.");
		}

		else if (origin->Name == "btn_C22") {
			deviceID = DEVICE_C22;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C22.");
		}

		else if (origin->Name == "btn_C23") {
			deviceID = DEVICE_C23;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C23.");
		}

		else if (origin->Name == "btn_C24") {
			deviceID = DEVICE_C24;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now C24.");
		}

		else if (origin->Name == "btn_x_axis") {
			deviceID = DEVICE_X_AXIS;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now X axis.");
		}

		else if (origin->Name == "btn_y_axis") {
			deviceID = DEVICE_Y_AXIS;
			Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now Y axis.");
		}

		else if (origin->Name == "btn_container_1") {
			deviceID = DEVICE_CONTAINER_1;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now Container 1.");
		}

		else if (origin->Name == "btn_container_2") {
			deviceID = DEVICE_CONTAINER_2;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now Container 2.");
		}

		else if (origin->Name == "btn_container_3") {
			deviceID = DEVICE_CONTAINER_3;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now Container 3.");
		}

		else if (origin->Name == "btn_container_4") {
			deviceID = DEVICE_CONTAINER_4;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now Container 4.");
		}

		else if (origin->Name == "btn_cover_1") {
			deviceID = DEVICE_COVER_1;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now Cover 1.");
		}

		else if (origin->Name == "btn_cover_2") {
			deviceID = DEVICE_COVER_2;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now Cover 2.");
		}

		else if (origin->Name == "btn_autocap") {
			deviceID = DEVICE_AUTOCAP;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now Autocap.");
		}

		else if (origin->Name == "btn_canlifter") {
			deviceID = DEVICE_CAN_LIFTER;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now Can Lifter.");
			if (Enable_Functions != 7)
				Enable_Functions = 1;
		}

		else if (origin->Name == "btn_humidifier") {
			deviceID = DEVICE_HUMIDIFIER;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now Humidifier.");
			if (Enable_Functions != 7)
				Enable_Functions = 1;
		}

		else if (origin->Name == "btn_tinting") {
			deviceID = DEVICE_TINTING;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now Tinting Slave.");
			if (Enable_Functions != 7) {			
				Enable_Functions = 1;
			}
		}

		else if (origin->Name == "btn_unused_4") {
			deviceID = DEVICE_UNUSED_4;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now Unused 4.");
		}

		else if (origin->Name == "btn_unused_5") {
			deviceID = DEVICE_UNUSED_5;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now Unused 5.");
			if (Enable_Functions != 7) {
				if (Enable_Functions == 3)
					Enable_Functions = 5;
				else
					Enable_Functions = 0;
			}
		}

		else if (origin->Name == "btn_unused_6") {
			deviceID = DEVICE_UNUSED_6;
			if (Enable_Functions != 7)
				Enable_Functions = 0;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now Unused 6.");
		}

		else if (origin->Name == "btn_unused_7") {
			deviceID = DEVICE_UNUSED_7;
			ENABLE_PRINT();
			PRINT_STATUS("Selected target device is now Unused 7.");
			if (Enable_Functions != 7) {
				if (Enable_Functions == 5)
					Enable_Functions = 7;
				else
					Enable_Functions = 0;
			}
		}

		DEBUG_OUT(">>QueryVerify command sent to MAB BOOT LOADER");

		//Set the bootloader and programming states
		bootloaderState = BOOTLOADER_QUERY;
		ProgramThreadResults = QUERY_RUNNING;

		//If we are in not in debugging mode then clear the status box
#if !defined(DEBUGGING)
		listBox1->Items->Clear();
#endif

		//Allow the bootloader main thread to print a message regarding the
		//  start of the programming sequence
		ENABLE_PRINT();

#if defined(DEBUG_THREADS)
		//If we are debugging the threads then call the programming thread
		//  start function inline so that we are able to print to the menu
		QueryThreadStart();
#else
		//If we aren't debugging the threads, then let's create a new
		//  instances of the thread so that the user form does not lock up
		//  while the programming sequence is running.
		QueryThread = gcnew Thread(gcnew ThreadStart(this, &HIDBootLoader::Form1::QueryThreadStart));
		QueryThread->Start();
#endif
	}
#pragma endregion




private: System::Void btn_clear_Click(System::Object^  sender, System::EventArgs^  e) {
	listBox1->Items->Clear();

	ENABLE_PRINT();
	PRINT_STATUS("Log cleared.");
}
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
	}
private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void ckbox_ConfigWordProgramming_CheckedChanged_1(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void flowLayoutPanel2_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
}
};
} /* namespace HIDBootloader */
