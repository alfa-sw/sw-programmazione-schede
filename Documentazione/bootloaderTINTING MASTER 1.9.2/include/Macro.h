/**/
/*============================================================================*/
/**
**      @file    Macro.h
**
**      @brief   Macro definitions
**/
/*============================================================================*/
/**/

#ifndef __MACRO_H__
#define __MACRO_H__

#define BL_SW_VERSION 0x010902

#define BYTES2WORDS(x)                          \
  ((x) / 2)

#define HALT()                                  \
  do {                                          \
    Sleep();                                    \
  } while (1)

#if ! defined __DEBUG
#  define DISABLE_WDT()                           \
  do {                                            \
    _SWDTEN = 0;                                  \
  } while (0)

#  define ENABLE_WDT()                            \
  do {                                            \
    ClrWdt();                                     \
    _SWDTEN = 1;                                  \
  } while (0)

#else
#  define DISABLE_WDT()
#  define ENABLE_WDT()
#endif

#define BL_WAIT_CHECK_STAND_ALONE               0
#define BL_NO_STAND_ALONE                       1
#define BL_STAND_ALONE                          2
#define BL_FORCED_STAND_ALONE                   3

#endif /* __MACRO_H__ */
