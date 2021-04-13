/**/
/*============================================================================*/
/**
**      @file    main.h
**
**      @brief
**/
/*============================================================================*/
/**/
#ifndef __MAIN_H__
#define __MAIN_H__

/*===== PROTOTIPI FUNZIONI ==================================================*/
extern void BL_ServerMg(void);

extern unsigned short CRCarea(unsigned char *pointer, unsigned short n_char,
                              unsigned short CRCinit);

extern unsigned short BL_TimeBase;

#endif /* __MAIN_H__ */
