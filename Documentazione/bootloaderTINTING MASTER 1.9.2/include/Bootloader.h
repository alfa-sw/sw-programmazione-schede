/**/
/*============================================================================*/
/**
**      @file    Bootloader.h
**
**      @brief   BootLoader shared text module header file
**/
/*============================================================================*/
/**/

#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

/** Bootloader function pointers */

typedef struct {
  unsigned short (*ptrCRCarea)(unsigned char *pointer, unsigned short n_char,
			       unsigned short CRCinit);
  void (*ptrEEPROMReadArray)(unsigned short StartAddress, unsigned short Length,
			     unsigned char *DataPtr);
  void (*ptrEEPROMWriteArray)(unsigned short StartAddress, unsigned short Length,
			      unsigned char *DataPtr);
  void (*ptrUpdateCRCEEParam)(unsigned char *pointer, unsigned short n_char,
			      unsigned short address_crc);
  unsigned short (*ptrCRCareaFlash)(unsigned long address, unsigned long n_word,
				    unsigned short CRCinit);
  void (*ptrBL_SSN_GetValue)(unsigned char * number_buffer);

  unsigned char (*ptrEEPROMWriteArrayInPage)(unsigned short StartAddress,
					     unsigned char Length,
					     unsigned char *DataPtr);
  void (*ptrEEPROMWriteByte)(unsigned Data, unsigned Address);

  void (*ptrEEPROMWriteByteNotBlocking)(unsigned Data, unsigned Address);
  union _EEPROMStatus_ (*ptrEEPROMReadStatus)();
} BootloaderPointers_T;

/**
 * GUI communication data structure
 ******************************************/
typedef struct _procGUI_t procGUI_t;
struct __attribute__ ((packed)) _procGUI_t
{
  unsigned char slaves_en[N_SLAVES_BYTES];// volumi dispensazione (basi e coloranti)
};

#endif /* __BOOTLOADER_H__ */
