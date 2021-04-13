/**/
/*============================================================================*/
/**
**      @file    BL_USB_ServerMg.h
**
**      @brief   File di inclusioni relativo alle funzioni condivise con
**
**      @date    05/10/2010
**
**      @version Haemotronic - Haemodrain
**/
/*============================================================================*/
/**/

#ifndef __BL_USB_SERVERMG_H__
#define __BL_USB_SERVERMG_H__

/*===== INCLUSIONI ===========================================================*/
/*===== MACRO LOCALI =========================================================*/
/*===== TIPI LOCALI ==========================================================*/
/*===== DICHIARAZIONI LOCALI =================================================*/
/*===== VARIABILI LOCALI =====================================================*/
/*===== VARIABILI LOCALI =====================================================*/
/*===== COSTANTI LOCALI ======================================================*/

/*===== DICHIARAZIONE FUNZIONI LOCALI ========================================*/
extern void BL_USB_Init(void);
extern void BL_USB_ServerMg(void);
extern void WriteFlashWord(long Addr,long Val);
extern DWORD ReadProgramMemory(DWORD address);
extern void WriteFlashSubBlock (DWORD StartAddress, unsigned short Size,
				unsigned short * DataBuffer);
extern void EraseFlash(unsigned char PageToErase);
extern void jump_to_appl(void);
extern char CheckApplPres(DWORD address);
extern void Jump_To_Application_Request(void);
#endif /* __BL_USB_SERVERMG_H__ */
