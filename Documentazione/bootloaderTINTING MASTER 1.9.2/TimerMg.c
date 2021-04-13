/**/
/*============================================================================*/
/**
**      @file    timer.c
**
**      @brief   Timer management
**
**      @version Haemotronic - Haemodrain
**/
/*============================================================================*/
/**/

/*===== INCLUSIONI ========================================================= */
#include "Compiler.h"
#include "TimerMg.h"
#include "macro.h"
#include "mem.h"
/*====== MACRO LOCALI ====================================================== */

/*====== TIPI LOCALI ======================================================== */

/*====== VARIABILI LOCALI =================================================== */
static unsigned short BL_MonTimeBase;

/*====== VARIABILI GLOBALI =================================================== */
unsigned short BL_TimeBase;

timerstype BL_TimStr[N_TIMERS];

unsigned short BL_Durata[N_TIMERS] = {
/*  0 */    DELAY_T_LED,
/*  1 */    DELAY_T_FILTER_KEY,
/*  2 */    DELAY_FORCE_STAND_ALONE,
/*  3 */    DELAY_INTRA_FRAMES,
/*  4 */    DELAY_RETRY_BROADCAST_MSG,
/*  5 */    DELAY_SEND_RESET_MSG,
/*  6 */    DELAY_SLAVE_WINDOW_TIMER,
};
             
/*====== COSTANTI LOCALI =================================================== */

/*====== DEFINIZIONE FUNZIONI LOCALI ======================================= */
void BL_TimerMg(void);
void BL_TimerInit(void);

void BL_TimerMg(void)
/*
**=============================================================================
**
**      Oggetto        : Funzione principale del modulo TIMERMG
**                       chiama le funzioni locali in sequenza
**
**      Parametri      : void
**
**      Ritorno        : void
**
**      Vers. - autore : 1.0  nuovo  R.Tartarini
**
**=============================================================================
*/
{
    unsigned char temp;
    _T5IE = 0;
    BL_MonTimeBase = BL_TimeBase;
    _T5IE=1;
    for (temp = 0; temp < N_TIMERS; temp++) {
        if (BL_TimStr[temp].Flg == T_RUNNING) {
            if ((BL_MonTimeBase - BL_TimStr[temp].InitBase) >= BL_Durata[temp])
                BL_TimStr[temp].Flg = T_ELAPSED;
        }
        if (BL_TimStr[temp].Flg == T_STARTED) {
            BL_TimStr[temp].InitBase = BL_MonTimeBase;
            BL_TimStr[temp].Flg = T_RUNNING;
        }
    }
} // end TimerMg

void BL_TimerInit (void)
/*
**=============================================================================
**
**      Oggetto        : Inizializzazione TMR5 per TimeBase BL
**
**      Parametri      : void
**
**      Ritorno        : void
**
**      Vers. - autore : 1.0  nuovo  F.Coiro
**=============================================================================
*/
{
  T5CON = 0x0010; // Prescaler 1:8 
  PR5 = 0x4E20; // FCY = 16MHz, Prescaler 1:8, TB = 10ms  
  // TMR 5 Interrupt Settings
  IFS1 &= 0xBFFF;
  IEC1 |= 0x1000;
  // TMR 5 ON
  T5CON |= 0x8000;
  BL_TimeBase = 0;
}

void _ISRFAST __attribute__((interrupt, auto_psv)) _AltT5Interrupt(void)
/*******************************************************************************
**
**   @brief Timer 5 interrupt routine
**
**   @param void
**
**   @return void
**
 ******************************************************************************/
{
    if ( _T5IE && _T5IF) {
        _T5IF = 0;
        BL_TimeBase++;
    }
}
