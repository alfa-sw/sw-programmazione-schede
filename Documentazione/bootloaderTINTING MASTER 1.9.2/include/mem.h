/**/
/*============================================================================*/
/**
**      @file    mem.h
**
**      @brief   Macro per gli indirizzi in memoria
**/
/*============================================================================*/
/**/

#ifndef MEM_H_DEFINED
#define MEM_H_DEFINED

/** IMPORTANT REMARK: these macros *must* match the memory map defined
 * in the gld files. */

/* -- Program memory macros ------------------------------------------------ */
#define __BL_CODE_BASE (0x0000L)
#define __BL_CODE_END  (0x2BFEL)

/* Used to program the actuators internal flash */
#define __ACTS_CODE_BASE (0x1700L)
#define __ACTS_CODE_BASE_HUMIDIFIER (0x2000L)
#define __ACTS_CODE_BASE_TINTING (0x2C00L)

#define __APPL_CODE_BASE (0x2C00L)

#define __APPL_GOTO_ADDR "0x2C04"

#define __BOOT_CODE_FW_VER (__BL_CODE_END-2)

/* -- Data memory macros -------------------------------------------------- */
#define __APPL_DATA_BASE (0x1010)
#define __APPL_DATA_END  (0x4000)

// #define __TEST_RESULTS_ADDR (__APPL_DATA_BASE - 0x10)
#define __BL_VERSION_ADDR (__APPL_DATA_BASE - 0x18)
#define __JMP_BOOT_ADDR   (__APPL_DATA_BASE - 0x1C)

/* -- Interrupt handlers ----------------------------------------------------- */
// Interrupt vector addresses
#define __APPL_T1    (__APPL_CODE_BASE + 0x14)  //  0x2C14 
#define __APPL_U2RX1 (__APPL_CODE_BASE + 0x3A)  //  0x2C3A 
#define __APPL_U2TX1 (__APPL_CODE_BASE + 0x60)  //  0x2C60 
#define __APPL_U3RX1 (__APPL_CODE_BASE + 0x86)  //  0x2C86 
#define __APPL_U3TX1 (__APPL_CODE_BASE + 0xAC)  //  0x2CAC 
#define __APPL_SPI1  (__APPL_CODE_BASE + 0xD2)  //  0x2CD2 
#define __APPL_SPI2  (__APPL_CODE_BASE + 0xF8)  //  0x2CF8 
#define __APPL_SPI3  (__APPL_CODE_BASE + 0x11E) //  0x2D1E 
#define __APPL_I2C3  (__APPL_CODE_BASE + 0x144) //  0x2D44 

#endif
