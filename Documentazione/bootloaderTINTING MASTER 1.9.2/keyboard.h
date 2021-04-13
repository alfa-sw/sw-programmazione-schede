/**/
/*============================================================================*/
/**
**      @file    keyboard.h
**
**      @brief
**
**      @version Haemotronic - Haemodrain
**/
/*============================================================================*/
/**/
#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

/*===== MACRO LOCALI =========================================================*/
#define SWITCH_ON_OFF        0x01
#define SWITCH_START         0x02
#define SWITCH_START_ON_OFF  0x03
#define SWITCH_STOP          0x04
#define SWITCH_SEL           0x10

/* Macro per la definizione degli Array dei filtri */
#define INPUT_ARRAY 5

#define FILTER_WINDOW           5                /* Larghezza della finestra del filtro */
#define FILTER_WINDOW_LENGTH    (FILTER_WINDOW-1)
#define FILTER_WINDOW_LOOP      (FILTER_WINDOW-2)
#define MAX_CHANGE              (FILTER_WINDOW/2)
#define MIN_COUNT               (FILTER_WINDOW*3/4)
#define ERRORE_FILTRO           2
#define LOW_FILTER              0
#define HIGH_FILTER             1
#define COUNT_RESET             0
#define MASK_FILTER_OFF         0x0000

/*===== PROTOTIPI FUNZIONI ==================================================*/
extern void BL_Keyboard(void);
extern void BL_ReadKeybStatus(void);
extern void BL_GetKeybStatus(unsigned char SwStatus);
extern unsigned char BL_FilterSensorInput(unsigned char InputFilter);

#endif /* __KEYBOARD_H__ */
