/**/
/*===========================================================================*/
/**
 **      @file    keyboard.c
 **
 **      @brief   Gestione della tastiera
 **
 **      @version Haemotronic - Haemodrain
 **/
/*===========================================================================*/
/**/

/*===== INCLUSIONI ========================================================= */
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "HardwareProfile.h"
#include "DEFINE.H"
#include "MACRO.H"
#include "RAM.H"
#include "CONST.H"
#include "TIMERMG.H"
#include "keyboard.h"

/*====== MACRO LOCALI ====================================================== */

/*====== TIPI LOCALI ======================================================== */


/*====== VARIABILI LOCALI =================================================== */
BL_SwitchStatusType BL_SwitchStatus, BL_SwitchNotFiltered;
//static unsigned char BL_ReadKeyb;
static unsigned char __attribute__((far)) psv_shadow;
static unsigned char  BL_FILTRAGGIO_LOW[FILTER_WINDOW];
static unsigned char  BL_n_filter;
static unsigned char BL_zero_counter, BL_one_counter;
static unsigned char BL_ChangeStatus, BL_Out_Status;
static signed char BL_index_0, BL_index_1;
static unsigned char BL_DummyOutput_low,BL_shift;
static unsigned char BL_fNewReading;
/*====== COSTANTI LOCALI =================================================== */



/*====== DEFINIZIONE FUNZIONI LOCALI ======================================= */
void BL_Keyboard(void);
void BL_ReadKeybStatus(void);
unsigned char BL_FilterSensorInput(unsigned char InputFilter);
void BL_GetKeybStatus(unsigned char SwStatus);


void BL_Keyboard (void)
/*
**=============================================================================
**
**      Oggetto : Funzione principale del modulo KEYBOARD chiama in
**                sequenza le funzioni locali
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
  //if (BL_StandAlone == TRUE)
  //{
  BL_ReadKeybStatus();

  /* Da inserire Flag di filtraggio */
  if  (StatusTimer(T_DEL_FILTER_KEY) == T_ELAPSED)
    {
      BL_SwitchStatus.byte = BL_FilterSensorInput(BL_SwitchNotFiltered.byte);
      StartTimer(T_DEL_FILTER_KEY);
    }
  BL_GetKeybStatus(BL_SwitchStatus.byte);
  //}
} /* end Keyboard */

void BL_ReadKeybStatus (void)
/*
**=============================================================================
**
**      Oggetto        : Lettura dello stato degli switch
**
**      Parametri      : void
**
**      Ritorno        : void
**
**      Vers. - autore : 1.0  nuovo  F.Coiro
**
**=============================================================================
*/
{
  BL_SwitchNotFiltered.Bit.Sw_OnOff = Sw_ON_OFF;
  BL_SwitchNotFiltered.Bit.Sw_Start = Sw_START;
  BL_SwitchNotFiltered.Bit.Sw_Stop  = Sw_STOP;
  // BL_SwitchNotFiltered.Bit.Sw_Sel   = Sw_SEL;
  // BL_SwitchNotFiltered.Bit.Sw_RS   = Sw_RemoteStop;
} /* end Keyboard */

void BL_GetKeybStatus (unsigned char SwStatus)
/*
**=============================================================================
**
**      Oggetto        : Funzione che aggiorna la variabile di stato KeyboardStatus
**
**      Parametri      : unsigned char SwStatus
**
**      Ritorno        : void
**
**      Vers. - autore : 1.0  nuovo  F.Coiro
**
**=============================================================================
*/
{
  if ((SwStatus != SWITCH_ON_OFF) && (SwStatus != SWITCH_START) && (SwStatus != SWITCH_STOP) && (SwStatus != SWITCH_SEL))
    {
      BL_fNewReading=1;
      BL_KeyboardStatus = NO_TASTI;
    }

  if ((BL_StandAlone == BL_WAIT_CHECK_STAND_ALONE) ||  (BL_StandAlone == BL_FORCED_STAND_ALONE))
    {
      if (SwStatus == SWITCH_START_ON_OFF)
        {
          BL_KeyboardStatus = START_ON_OFF;
        }
      else if (SwStatus == SWITCH_START)
        {
          BL_KeyboardStatus = START;
        }

    }

  if (BL_fNewReading)
    {
      if (SwStatus == SWITCH_ON_OFF)
        {
          BL_KeyboardStatus = ON_OFF;
        }
      else if (BL_StandAlone != BL_NO_STAND_ALONE)
        {
          if (SwStatus == SWITCH_START)
            {
              BL_KeyboardStatus = START;
            }
          else if (SwStatus == SWITCH_STOP)
            {
              BL_KeyboardStatus = STOP;
            }
          else if (SwStatus == SWITCH_SEL)
            {
              BL_KeyboardStatus = SEL;
            }
        }

      if (BL_KeyboardStatus)
        {
          BL_fNewReading=0;
        }
    }


} /* end Keyboard */

unsigned char BL_FilterSensorInput(unsigned char InputFilter)
/*
**=============================================================================
**
**      Oggetto        : Funzione che filtra lo stato dei pulsanti della tastiera
**                       da eventuali disturbi elettrici
**
**      Parametri      : unsigned char InputFilter
**
**      Ritorno        : unsigned char DummyOutput_low
**
**      Data - autore  : 16/05/2006  F.Coiro
**
**=============================================================================
*/
{
  unsigned char temp_uc;
  signed char temp_sc;

  if (BL_n_filter < FILTER_WINDOW_LENGTH)  /* n_filter = Finestra campioni del filtro */
    {
      BL_n_filter++;
    }
  else
    {
      BL_n_filter = 0;
    }

  BL_FILTRAGGIO_LOW[BL_n_filter] = InputFilter;

  for(temp_uc = 0 ; temp_uc < INPUT_ARRAY ; temp_uc++) /* INPUT_ARRAY = N° di ingressi da filtrare (8*2) */
    {
      psv_shadow = PSVPAG;
      /* set the PSVPAG for accessing APPL_MASK_BIT_8[] */
      PSVPAG = __builtin_psvpage (BL_MASK_BIT_8);

      BL_shift = BL_MASK_BIT_8[temp_uc];
      /* restore the PSVPAG for the compiler-managed PSVPAG */
      PSVPAG = psv_shadow;

      BL_shift = BL_MASK_BIT_8[temp_uc];

      /*ByteLow*/
      for(temp_sc = FILTER_WINDOW_LOOP ; temp_sc >= 0 ; temp_sc--)
        {
          /*Indice 0*/
          BL_index_0 = BL_n_filter - temp_sc;
          if (BL_index_0 < 0)
            {
              BL_index_0 += FILTER_WINDOW;
            }
          /*Indice 1*/
          BL_index_1 = BL_n_filter - temp_sc - 1;
          if (BL_index_1 < 0)
            {
              BL_index_1 += FILTER_WINDOW;
            }

          if ( (BL_FILTRAGGIO_LOW[BL_index_0] ^ BL_FILTRAGGIO_LOW[BL_index_1]) & BL_shift)
            {
              BL_ChangeStatus++;
            }

          if ( BL_FILTRAGGIO_LOW[BL_index_0] & BL_shift)
            {
              BL_one_counter++;
            }
          else
            {
              BL_zero_counter++;
            }

          if (temp_sc == 0)
            {
              if (BL_FILTRAGGIO_LOW[BL_index_1] & BL_shift)
                {
                  BL_one_counter++;
                }
              else
                {
                  BL_zero_counter++;
                }
            }
        }

      if (BL_ChangeStatus > MAX_CHANGE)
        {
          if (BL_zero_counter >= MIN_COUNT)
            {
              BL_Out_Status = LOW_FILTER;
            }
          else if (BL_one_counter >= MIN_COUNT)
            {
              BL_Out_Status = HIGH_FILTER;
            }
          else
            {
              BL_Out_Status = ERRORE_FILTRO;
            }
        }
      else
        {
          if (BL_zero_counter > BL_one_counter)
            {
              BL_Out_Status = LOW_FILTER;
            }
          else
            {
              BL_Out_Status = HIGH_FILTER;
            }
        }

      BL_zero_counter = COUNT_RESET;
      BL_one_counter = COUNT_RESET;
      BL_ChangeStatus = COUNT_RESET;

      /* Segnale d'ingresso filtrato */
      if (BL_Out_Status != ERRORE_FILTRO)
        {
          if (!temp_uc)
            {
              BL_DummyOutput_low = BL_Out_Status;
            }
          else
            {
              BL_DummyOutput_low |= (BL_Out_Status << temp_uc);
            }
        }
    }
  return (BL_DummyOutput_low);
} /*end FilterSensorInput*/
