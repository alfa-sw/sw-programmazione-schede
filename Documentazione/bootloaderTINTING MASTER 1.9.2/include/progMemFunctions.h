/**/
/*============================================================================*/
/**
 **      @file    progMemFunctions.h
 **
 **      @brief   header of progMemFunctions.c
 **
 **      @version Alfa Color Tester
 **/
/*============================================================================*/
/**/

#ifndef __PROG_MEM_FUNCTIONS_H__
#define __PROG_MEM_FUNCTIONS_H__

extern void WriteFlashWord(long Addr,long Val);

extern DWORD ReadProgramMemory(DWORD address);

extern void WriteFlashSubBlock(DWORD StartAddress,
                               unsigned short Size,
                               unsigned short * DataBuffer);

extern void EraseFlashPage(unsigned char PageToErase);

#endif /* __PROG_MEM_FUNCTIONS_H__ */
