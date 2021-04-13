/**/
/*============================================================================*/
/**
**      @file    progMemFunctions.c
**
**      @brief   Functions to program or to read flash. They can be used for
**               PIC24F family and dsPIC33F device
**
**      @version Alfa Color Tester
**/
/*============================================================================*/
/**/

/*===== INCLUSIONI ========================================================== */
#include "GenericTypeDefs.h"
#include "Compiler.h"

#include "progMemFunctions.h"
/*====== MACRO LOCALI ======================================================= */
/*====== TIPI LOCALI ======================================================== */
/*====== VARIABILI LOCALI =================================================== */
/*====== VARIABILI GLOBALI ================================================== */
/*====== COSTANTI LOCALI ==================================================== */
/*===== DICHIARAZIONE FUNZIONI LOCALI ======================================= */

void WriteFlashWord(long Addr,long Val);
DWORD ReadProgramMemory(DWORD);
void WriteFlashSubBlock(DWORD StartAddress, unsigned short Size,
                                      unsigned short* DataBuffer);
void EraseFlash(unsigned char PageToErase);

/*====== DEFINIZIONE FUNZIONI LOCALI ======================================== */

void WriteFlashWord(long Addr,long Val)

/******************************************************************************
 *
 *      Oggetto        : Use word writes to write code chunks less than a full
 *                                         64 byte block size.
 *
 *      Parametri      : Addr(address), Val(valore)
 *
 *      Ritorno        : void
 *
 ******************************************************************************/
{
  DWORD_VAL Address = {Addr};
  DWORD_VAL Value = {Val};

  NVMCON = 0x4003;                //Perform WORD write next time WR
                                    //gets set = 1.

  TBLPAG = Address.word.HW;

  //Write the low word to the latch
  __builtin_tblwtl(Address.word.LW, Value.word.LW );

  //Write the high word to the latch (8 bits of data + 8 bits of
  //"phantom data")
  __builtin_tblwth(Address.word.LW, Value.word.HW );

  //Disable interrupts for next few instructions for unlock sequence
  asm("DISI #16");
  __builtin_write_NVM();

  while(NVMCONbits.WR == 1){}

  //Good practice to clear WREN bit anytime we are not expecting to
  //do erase/write operations, further reducing probability of
  //accidental activation.
  NVMCONbits.WREN = 0;

}/*end TimerMg*/


/*********************************************************************
 * Function:        DWORD ReadProgramMemory(DWORD address)
 *
 * PreCondition:    None
 *
 * Input:           Program memory address to read from.  Should be
 *                            an even number.
 *
 * Output:          Program word at the specified address.  For the
 *                            PIC24, dsPIC, etc. which have a 24 bit program
 *                            word size, the upper byte is 0x00.
 *
 * Side Effects:    None
 *
 * Overview:        Modifies and restores TBLPAG.  Make sure that if
 *                            using interrupts and the PSV feature of the CPU
 *                            in an ISR that the TBLPAG register is preloaded
 *                            with the correct value (rather than assuming
 *                            TBLPAG is always pointing to the .const section.
 *
 * Note:            None
 ********************************************************************/
DWORD ReadProgramMemory(DWORD address)
{
    DWORD_VAL dwvResult;
    WORD wTBLPAGSave;

    wTBLPAGSave = TBLPAG;
    TBLPAG = ((DWORD_VAL*)&address)->w[1];

    dwvResult.w[1] = __builtin_tblrdh((WORD)address);
    dwvResult.w[0] = __builtin_tblrdl((WORD)address);
    TBLPAG = wTBLPAGSave;

    return dwvResult.Val;
}


/*******************************************************************************
 * Function: void WriteFlashSubBlock(DWORD StartAddress, unsigned
 * short Size, unsigned short* DataBuffer)
 *
 * PreCondition:    None
 *
 * Input:           Program memory address to read from.  Should be
 *                            an even number.
 *
 * Output:          Program word at the specified address.  For the
 *                            PIC24, dsPIC, etc. which have a 24 bit program
 *                            word size, the upper byte is 0x00.
 *
 * Side Effects:    None
 *
 * Overview:        Modifies and restores TBLPAG.  Make sure that if
 *                            using interrupts and the PSV feature of the CPU
 *                            in an ISR that the TBLPAG register is preloaded
 *                            with the correct value (rather than assuming
 *                            TBLPAG is always pointing to the .const section.
 *
 * Note:            None
 ******************************************************************************/
//Use word writes to write code chunks less than a full 64 byte block size.
void WriteFlashSubBlock(DWORD StartAddress, unsigned short Size,
                         unsigned short * DataBuffer)
{
  unsigned short DataIndex = 0;

  DWORD_VAL Address;

  NVMCON = 0x4003;                //Perform WORD write next time WR gets set = 1.

  while(DataIndex < Size)                 //While data is still in the buffer.
  {
      Address = (DWORD_VAL)(StartAddress + DataIndex);
      TBLPAG = Address.word.HW;

      //Write the low word to the latch
      __builtin_tblwtl(Address.word.LW, DataBuffer[DataIndex]);

      //Write the high word to the latch (8 bits of data + 8 bits of
      //"phantom data")
      __builtin_tblwth(Address.word.LW, DataBuffer[DataIndex + 1]);
      DataIndex = DataIndex + 2;

      //Disable interrupts for next few instructions for unlock
      //sequence
      asm("DISI #16");
      __builtin_write_NVM();

      while(NVMCONbits.WR == 1){}

  }

  //Good practice to clear WREN bit anytime we are not expecting to
  //do erase/write operations, further reducing probability of
  //accidental activation.
  NVMCONbits.WREN = 0;
}

/******************************************************************************
 *
 *      Oggetto        : Use word writes to write code chunks less than a full
 *                                         64 byte block size.
 *
 *      Parametri      : Addr(address), Val(valore)
 *
 *      Ritorno        : void
 *
 ******************************************************************************/
void EraseFlashPage(unsigned char PageToErase)
{
  DWORD_VAL MemAddressToErase = {0x00000000};
  MemAddressToErase = (DWORD_VAL)(((DWORD)PageToErase) << 10);

  NVMCON = 0x4042;                                //Erase page on next WR

  TBLPAG = MemAddressToErase.byte.UB;
  __builtin_tblwtl(MemAddressToErase.word.LW, 0xFFFF);

  //Disable interrupts for next few instructions for unlock sequence
  asm("DISI #16");
  __builtin_write_NVM();

  while(NVMCONbits.WR == 1) {}

  //EECON1bits.WREN = 0; //Good practice now to clear the WREN bit,
  //as further protection against any future accidental activation
  //of self write/erase operations.
}
