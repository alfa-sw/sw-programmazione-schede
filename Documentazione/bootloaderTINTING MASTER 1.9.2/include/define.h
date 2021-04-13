/**/
/*============================================================================*/
/**
 **      @file    define.h
 **
 **      @brief   File di definizioni
 **
 **      @date
 **
 **      @version Haemotronic - Haemodrain
 **/
/*============================================================================*/
/**/
#ifndef __DEFINE_H
#define __DEFINE_H

/* Flash layout parameters */
#define FLASH_PAGE_WORD_LENGTH 1024
#define FLASH_PAGE_BYTE_LENGTH 1536

#define START_BL_ADDRESS           0x00000400L
#define START_APPL_ADDRESS         0x00002C00L

#define START_APPL_ADDRESS_MASTER  START_APPL_ADDRESS
#define START_APPL_ADDRESS_SLAVE   0x00001700L
#define START_APPL_ADDRESS_HUMIDIFIER  0x00002000L
#define START_APPL_ADDRESS_TINTING     0x00002C00L

#define FIRST_PG_APPL              (START_APPL_ADDRESS/FLASH_PAGE_WORD_LENGTH)

//Number of bytes in the "Data" field of a standard request to/from
//the PC.  Must be an even number from 2 to 56.
#define RequestDataBlockSize   56

/* 6 bytes are required to represent 48 bits */
#define N_SLAVES_BYTES 6

#define INPUT 1
#define OUTPUT 0

/******************************************************************************************/
/*********************************** HW_IO_Remapping **************************************/
/******************************************************************************************/

//PPS Outputs
#define NULL_IO     0
#define C1OUT_IO    1
#define C2OUT_IO    2
#define U1TX_IO     3
#define U1RTS_IO    4
#define U2TX_IO     5
#define U2RTS_IO    6
#define SDO1_IO     7
#define SCK1_IO     8
#define SS1OUT_IO   9
#define SDO2_IO    10
#define SCK2_IO    11
#define SS2OUT_IO  12
#define SDI1_IO    13
#define OC1_IO     18
#define OC2_IO     19
#define OC3_IO     20
#define OC4_IO     21
#define OC5_IO     22
#define OC6_IO     23
#define OC7_IO     24
#define OC8_IO     25
#define SDI2_IO    26
#define U3TX_IO    28
#define U3RTS_IO   29
#define U4TX_IO    30
#define U4RTS_IO   31
#define SDO3_IO    32
#define SCK3_IO    33
#define SS3OUT_IO  34
#define OC9_IO     35

//----------------PORT B------------------------------------------------------//
#define SET_RB0(x) ( LATBbits.LATB0 = (x) )
#define SET_RB1(x) ( LATBbits.LATB1 = (x) )
#define SET_RB2(x) ( LATBbits.LATB2 = (x) )
#define SET_RB3(x) ( LATBbits.LATB3 = (x) )
#define SET_RB4(x) ( LATBbits.LATB4 = (x) )
#define SET_RB5(x) ( LATBbits.LATB5 = (x) )
#define SET_RB6(x) ( LATBbits.LATB6 = (x) )
#define SET_RB7(x) ( LATBbits.LATB7 = (x) )
#define SET_RB8(x) ( LATBbits.LATB8 = (x) )
#define SET_RB9(x) ( LATBbits.LATB9 = (x) )
#define SET_RB10(x) ( LATBbits.LATB10 = (x) )
#define SET_RB11(x) ( LATBbits.LATB11 = (x) )
#define SET_RB12(x) ( LATBbits.LATB12 = (x) )
#define SET_RB13(x) ( LATBbits.LATB13 = (x) )
#define SET_RB14(x) ( LATBbits.LATB14 = (x) )
#define SET_RB15(x) ( LATBbits.LATB15 = (x) )

#define TOGGLE_RB6() ( LATBbits.LATB6 ^= TRUE )
#define TOGGLE_RB7() ( LATBbits.LATB7 ^= TRUE )

#define TOGGLE_RB50() ( LATBbits.LATB5 ^= TRUE )

//----------------PORT C------------------------------------------------------//
#define SET_RC12(x) ( LATCbits.LATC12 = (x) )
#define SET_RC13(x) ( LATCbits.LATC13 = (x) )
#define SET_RC14(x) ( LATCbits.LATC14 = (x) )
#define SET_RC15(x) ( LATCbits.LATC15 = (x) )
//----------------PORT D------------------------------------------------------//
#define SET_RD0(x)  ( LATDbits.LATD0 = (x) )
#define SET_RD1(x)  ( LATDbits.LATD1 = (x) )
#define SET_RD2(x)  ( LATDbits.LATD2 = (x) )
#define SET_RD3(x)  ( LATDbits.LATD3 = (x) )
#define SET_RD4(x)  ( LATDbits.LATD4 = (x) )
#define SET_RD5(x)  ( LATDbits.LATD5 = (x) )
#define SET_RD6(x)  ( LATDbits.LATD6 = (x) )
#define SET_RD7(x)  ( LATDbits.LATD7 = (x) )
#define SET_RD8(x)  ( LATDbits.LATD8 = (x) )
#define SET_RD9(x)  ( LATDbits.LATD9 = (x) )
#define SET_RD10(x)  ( LATDbits.LATD10 = (x) )
#define SET_RD11(x)  ( LATDbits.LATD11 = (x) )
//----------------PORT E------------------------------------------------------//
#define SET_RE0(x)  ( LATEbits.LATE0 = (x) )
#define SET_RE1(x)  ( LATEbits.LATE1 = (x) )
#define SET_RE2(x)  ( LATEbits.LATE2 = (x) )
#define SET_RE3(x)  ( LATEbits.LATE3 = (x) )
#define SET_RE4(x)  ( LATEbits.LATE4 = (x) )
#define SET_RE5(x)  ( LATEbits.LATE5 = (x) )
#define SET_RE6(x)  ( LATEbits.LATE6 = (x) )
//----------------PORT F------------------------------------------------------//
#define SET_RF0(x)  ( LATFbits.LATF0 = (x) )
#define SET_RF1(x)  ( LATFbits.LATF1 = (x) )
#define SET_RF3(x)  ( LATFbits.LATF3 = (x) )
#define SET_RF4(x)  ( LATFbits.LATF4 = (x) )
#define SET_RF5(x)  ( LATFbits.LATF5 = (x) )
//----------------PORT F------------------------------------------------------//
#define SET_RG2(x)  ( LATGbits.LATG2 = (x) )
#define SET_RG3(x)  ( LATGbits.LATG3 = (x) )
#define SET_RG6(x)  ( LATGbits.LATG6 = (x) )
#define SET_RG7(x)  ( LATGbits.LATG7 = (x) )
#define SET_RG8(x)  ( LATGbits.LATG8 = (x) )
#define SET_RG9(x)  ( LATGbits.LATG9 = (x) )
/******************************************************************************************/
#define SPIM_PIC24
#define SPIM_BLOCKING_FUNCTION
#define SPIM_PPRE (unsigned)0
#define SPIM_SPRE (unsigned)0
/***************************************** Main *******************************************/
/******************************************************************************************/
#define BL_STAND_ALONE_CHECK            (START_APPL_ADDRESS + 4)
#define APPL_FLASH_MEMORY_ERASED_VALUE  0x00FFFFFFL

enum
{
    MOTOR_TABLE,
    MOTOR_PUMP,
    MOTOR_VALVE,
    ALL_DRIVERS
};

// Periferiche SPI
enum
{
  SPI_1,
  SPI_2,
  SPI_3
};

typedef enum {
  PROC_OK,             /* Procedura corretta      */
  PROC_ERROR,          /* Valore scritto e letto non coincidono  */
  PROC_PROTECTED,      /* La flash è protetta in scrittura       */
  PROC_ERROR_PROTECT,  /* La flash si trova nello stato protetto */
  PROC_ERROR_12V,
  PROC_ADDRESS_ERROR,  /* Indirizzo errato */
  PROC_MAX_FAILS,
  PROC_CHK_ERROR
} PROC_RES;



/******************************************************************************************/
/*********************************** BL_USB_ServerMg **************************************/
/******************************************************************************************/

enum {
  USB_POWER_OFF = 0,
  USB_STATE_NUM
};

typedef struct __attribute__ ((packed)) {
  unsigned char Connect;
  unsigned char Rx;
  unsigned char Tx;
} USB_STATE_TYPE;

typedef struct __attribute__ ((packed)) {
  unsigned char livello;
  unsigned char fase;
  unsigned char step;
  PROC_RES ProcRes;
} Stato;

/*Enum per BLState.livello*/
enum {
  POWER_OFF = 0,
  INIT,
  USB_CONNECT_EXECUTION,  /* supported if BOOTLOADER_USB macro is uncommented */
  UART_FW_UPLOAD,         /* firmware upload */
  UART_FW_UPLOAD_FAILED,  /* firmware upload failed */
  JMP_TO_APP,			  /* jump to application program */
};

/*Enum per BLState.fase per BLState.livello=USB_CONNECT_EXECUTION*/
enum {
  USB_OPEN_SES_REQ=0,
  USB_CONNECT_OK,
  USB_CONNECT_FAILED,
};

/*Enum per BLState.step per BLState.fase=SAT_FW_UPLOAD --- Utilizzati sia per
  USB che per comunicazione seriale */
enum {
  ERASE_DEVICE=0,
  WAIT_DATA_PACKET,
  PROGRAM_DEVICE,
  PROGRAM_END
};



/******************************************************************************************/
/*********************************** EEPROM Address ***************************************/
/******************************************************************************************/

#define EEPROM_BL_START_ADDRESS 0x0004


/*Mappa EE dati utilizzati SOLO dal BL*/
/* Legenda indirizzi :
   Char  ->  EE_NomeParam
   Short ->  EE_NomeParam_H (MSB)
   EE_NomeParam_L (LSB)
   Long  ->  EE_NomeParam_HH (MSB of MSW)
EE_NomeParam_HL (LSB of MSW)
EE_NomeParam_LH (MSB of LSW)
EE_NomeParam_LL (LSB of LSW)
*/


typedef enum {
  EE_CRC_VALUE        = 0,
  EE_CRC_CONST_VALUE  = 2,
  EE_MODULE_STATUS    = 4,
  EE_SIMPIN           = 5,
  EE_APN              = 10,
  EE_USER             = 110,
  EE_PASSWORD         = 142,
  EE_DNS1             = 174,
  EE_DNS2             = 190,
  EE_SOCKUDP          = 206,
} EE_ADDRESS;

#define EE_SIZE 302 /*bytes*/
typedef struct {
  unsigned char ModuleStatus;
  unsigned char SIM_Pin[1];
  unsigned char Apn[1];
  unsigned char User[1];
  unsigned char Password[1];
  unsigned char DNS1[1];
  unsigned char DNS2[1];
  unsigned char SockUdp[1];
}E2Prom_Fields;


typedef union __attribute__ ((packed))  {
  unsigned char Data[EE_SIZE];
  E2Prom_Fields Var;
} EEPROM_TYPE;

typedef union {unsigned short bb;
  unsigned char b[2];
}wb_desc_type;


/*Definizione SwitchStatusType*/
typedef union {
  unsigned char byte;
  struct {
    unsigned char  Sw_OnOff  : 1,
      Sw_Start  : 1,
      Sw_Stop   : 1,
      Sw_Sel    : 1,
      Sw_RS     : 1,
      unused    : 3;
  } Bit;
} BL_SwitchStatusType;

#endif /* __DEFINE_H */
