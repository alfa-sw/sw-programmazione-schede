/**/
/*============================================================================*/
/**
 **      @file    TimerMg.h
 **
 **      @brief
 **
 **      @version Haemotronic - Haemodrain
 **/
/*============================================================================*/
/**/

#ifndef __TIMER_MG_H__
#define __TIMER_MG_H__

/*===== MACRO LOCALI =========================================================*/
#define BASE_T_TIMERMG 20

#define START_TIMER 1
#define STOP_TIMER  0
#define T_CLEAR_IF_ELAPSED 1
#define T_START_IF_ELAPSED 2
#define T_READ 0
#define T_ELAPSED       -1
#define T_RUNNING       2
#define T_HALTED        0
#define T_STARTED       1
#define T_NOT_RUNNING   -2

/* Base tempi uguale a 10ms --> Quindi 1s == 100 impulsi da 10ms */
#define DELAY_T_LED                     100
#define DELAY_T_FILTER_KEY              2
#define DELAY_FORCE_STAND_ALONE         200
#define DELAY_INTRA_FRAMES              2       /* 2 ms */
#define DELAY_RETRY_BROADCAST_MSG       10      /* 100 ms */
#define DELAY_SEND_RESET_MSG            10      /* 100 ms */
#define DELAY_SLAVE_WINDOW_TIMER        50     /* 500 ms */

#define StopTimer(Timer)    (BL_TimStr[Timer].Flg = STOP_TIMER)
#define StartTimer(Timer)   (BL_TimStr[Timer].Flg = START_TIMER)
#define StatusTimer(Timer)  (BL_TimStr[Timer].Flg)


/*===== TIPI ================================================================*/
typedef struct {
     signed char Flg;
     unsigned short InitBase;
} timerstype;

enum {
     T_LED,                      /*  0 */
     T_DEL_FILTER_KEY,           /*  1 */
     T_WAIT_FORCE_BL,            /*  2 */
     T_DELAY_INTRA_FRAMES,       /*  3 */
     T_RETRY_BROADCAST_MSG,      /*  4 */
     T_SEND_RESET_MSG,           /*  5 */
     T_SLAVE_WINDOW_TIMER,       /*  6 */     
     N_TIMERS
};

// extern unsigned short BL_TimeBase;

extern timerstype BL_TimStr[N_TIMERS];
extern unsigned short BL_Durata[N_TIMERS];
extern void BL_TimerMg (void);
extern void BL_TimerInit (void);

#endif /* __TIMER_MG_H__ */
