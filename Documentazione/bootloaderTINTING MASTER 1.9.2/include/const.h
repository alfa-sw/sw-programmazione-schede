/**/
/*============================================================================*/
/**
**      @file    const.h
**
**      @brief   Definizione costanti globali
**
**      @date
**
**      @version Haemotronic - Haemodrain
**/
/*============================================================================*/
/**/

#include "Bootloader.h"

#ifndef __CONST_H__
#define __CONST_H__

/* ======= Definizione macro per il solo file CONST.H ====================== */
#ifndef NULL
#define NULL 0
#endif

#define BY 0
#define WD 1

#define NSK 8

//extern const unsigned short CRC_TABLE[];
//extern const unsigned short BL_MASK_BIT [];
//extern const unsigned char BL_MASK_BIT_8[];
//extern const E2Prom_Fields E2Default_Cfg;

extern const unsigned short
/*__attribute__((space(psv), section ("CRCTable")))*/ CRC_TABLE[256];
extern unsigned short PtrJMPBoot;

#define JUMP_TO_BOOT_DONE (0xAA)

#endif /* __CONST_H__ */
