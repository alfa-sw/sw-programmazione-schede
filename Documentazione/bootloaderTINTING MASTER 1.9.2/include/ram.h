/**/
/*============================================================================*/
/**
**      @file ram.h
**
**      @brief Global data module
**/
/*============================================================================*/
/**/
#include "define.h"
#include "mem.h"
#include "Macro.h"
#include "Bootloader.h"

#ifdef RAM_EXTERN_DISABLE
#define RAM_EXTERN
#else
#define RAM_EXTERN extern
#endif

/* G L O B A L S */
RAM_EXTERN char BL_StandAlone
#ifdef RAM_EXTERN_DISABLE
 = TRUE
#endif
;

RAM_EXTERN volatile long boot_fw_version __attribute__((space(data),
                                                     address(__BL_VERSION_ADDR)));

RAM_EXTERN EEPROM_TYPE EEParam;
RAM_EXTERN unsigned short CRCEEData;
RAM_EXTERN unsigned short CRCEEData_Def;
RAM_EXTERN unsigned short CRCEEData_Calc;
RAM_EXTERN unsigned short CRCEEData_Def_Calc;
RAM_EXTERN USB_STATE_TYPE USB_Status;

RAM_EXTERN unsigned short __attribute__((far)) BL_CRCFlash;
RAM_EXTERN unsigned short __attribute__((far)) BL_CRCFlashValue;

RAM_EXTERN Stato BLState;

RAM_EXTERN unsigned char  BL_slave_id;
RAM_EXTERN unsigned short BL_SLAVE_VERSION[3]; 
RAM_EXTERN unsigned short BL_MASTER_VERSION[3];
RAM_EXTERN unsigned short Fw_Request;
RAM_EXTERN unsigned short BL_Master_Version;

RAM_EXTERN procGUI_t procGUI;
RAM_EXTERN unsigned short startAddress;
RAM_EXTERN unsigned short USB_Connect;

RAM_EXTERN unsigned long pippo, pippo1, pippo2, pippo3, pippo4, pippo5, pippo6, pippo7, pippo8, pippo9;

