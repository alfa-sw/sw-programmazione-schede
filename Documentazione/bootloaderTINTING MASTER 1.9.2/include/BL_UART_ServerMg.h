/**/
/*============================================================================*/
/**
**      @file    BL_UART_ServerMg.h
**
**      @brief   File di inclusioni relativo alle funzioni condivise
**
**      @date    16/10/2015
**
**      @version Alfa Color Tester
**/
/*============================================================================*/
/**/
#ifndef __BL_UART_SERVERMG_H__
#define __BL_UART_SERVERMG_H__

#include "serialCom.h"

/*===== MACRO LOCALI =========================================================*/
typedef struct __attribute__ ((packed)) {
  unsigned char  typeMessage;
//  unsigned short startAddress;
//  unsigned short  address;
  unsigned long startAddress;
  unsigned long  address;
  unsigned short numDataPack;
  unsigned short idDataPackRx;
  unsigned short idDataPackTx;
  unsigned short idDataPackExpected;
  unsigned char  numDataBytesPack;
  unsigned short bufferData[BYTES2WORDS(RequestDataBlockSize)];
  unsigned char  errorType;
  unsigned char  num_retry_broadcast;
  unsigned char  IDtype;
  unsigned char  numSendingResetDeviceCmd;
  unsigned char  endProgramming;
}progBoot_t;

typedef union __attribute__ ((packed))
{
  unsigned short uword;
  signed short   sword;
  unsigned char  byte[2];
} unionWord_t;


typedef union __attribute__ ((packed))
{
  unsigned long  udword;
  signed long    sdword;
  unsigned short word[2];
  unsigned char  byte[4];
} unionDWord_t;


/* Tipo di pacchetti TINTING MASTER -> Actuators_BL */
enum
{
  CMD_FW_UPLOAD      = 0x0F,
  DATA_FW_UPLOAD     = 0x10,
  END_DATA_FW_UPLOAD = 0x0A,
  GET_PROGRAM_DATA   = 0x0D,
  RESET_SLAVE        = 0x0E,
  CMD_FORCE_SLAVE_BL = 0x0B,
  CMD_FRMWR_REQUEST  = 0x09,
  CMD_JUMP_TO_APPLICATION = 0x0C,  
};

/* Tipo di pacchetti Actuators_BL -> TINTING MASTER */
enum
{
  ACK_FW_UPLOAD      = 0x11,
  NACK_FW_UPLOAD     = 0x12,
  SEND_PROGRAM_DATA  = 0x13,
  SEND_FRMWR_VERSION = 0x14  
};

/* Tipo di errore */
enum
{
  PACK_LOST = 0x02,
};

/* watch out: slave ids here start from 1, not 0 */
enum {
  B1_BASE_ID = 1,
  B2_BASE_ID,
  B3_BASE_ID,
  B4_BASE_ID,
  B5_BASE_ID,
  B6_BASE_ID,
  B7_BASE_ID,
  B8_BASE_ID,
  C1_COLOR_ID,
  C2_COLOR_ID,
  C3_COLOR_ID,
  C4_COLOR_ID,
  C5_COLOR_ID,
  C6_COLOR_ID,
  C7_COLOR_ID,
  C8_COLOR_ID,
  C9_COLOR_ID,
  C10_COLOR_ID,
  C11_COLOR_ID,
  C12_COLOR_ID,
  C13_COLOR_ID,
  C14_COLOR_ID,
  C15_COLOR_ID,
  C16_COLOR_ID,
  C17_COLOR_ID,
  C18_COLOR_ID,
  C19_COLOR_ID,
  C20_COLOR_ID,
  C21_COLOR_ID,
  C22_COLOR_ID,
  C23_COLOR_ID,
  C24_COLOR_ID,
  MOVE_X_AXIS_ID,
  MOVE_Y_AXIS_ID,
  STORAGE_CONTAINER1_ID,
  STORAGE_CONTAINER2_ID,
  STORAGE_CONTAINER3_ID,
  STORAGE_CONTAINER4_ID,
  PLUG_COVER_1_ID,
  PLUG_COVER_2_ID,
  AUTOCAP_ID,
  CAN_LIFTER_ID,
  HUMIDIFIER_ID,
  TINTING_ID,
  GENERIC_ACT13_ID,
  GENERIC_ACT14_ID,
  GENERIC_ACT15_ID,
  GENERIC_ACT16_ID,
  N_SLAVES = 63
};

#define isUART_FW_Upload_Ack()      (progBoot.typeMessage == ACK_FW_UPLOAD     && isNewProcessingMsg())
#define isUART_FW_Upload_Nack()     (progBoot.typeMessage == NACK_FW_UPLOAD    && isNewProcessingMsg())
#define isUART_Programming_Data()   (progBoot.typeMessage == SEND_PROGRAM_DATA && isNewProcessingMsg())
#define isUART_Reset()              (progBoot.typeMessage == RESET_SLAVE)
#define setEndProgramming()         (progBoot.endProgramming = TRUE)
#define isEndProgramming()          (progBoot.endProgramming == TRUE)
#define isUART_Slave_FW_Version()   (progBoot.typeMessage == SEND_FRMWR_VERSION && isNewProcessingMsg())
#define isUART_Master_FW_Version()  (BL_Master_Version == 1)

#define enBroadcastMessage()                                    \
  do {                                                          \
    progBoot.num_retry_broadcast = 0;                           \
    BL_slave_id = BROADCAST_ID;                                 \
  } while (0)

#define isMasterProgramming()                   \
  (progBoot.IDtype == PROGRAM_MAB)

/*===== TIPI LOCALI ==========================================================*/
/*===== DICHIARAZIONI LOCALI =================================================*/
/*===== VARIABILI LOCALI =====================================================*/
/*===== COSTANTI LOCALI ======================================================*/
/*===== DICHIARAZIONE FUNZIONI GLOBALI =======================================*/
extern progBoot_t progBoot;

extern void setBootMessage(unsigned char packet_type);
extern void MakeBootMessage(uartBuffer_t *txBuffer, unsigned char slave_id);
extern void DecodeBootMessage(uartBuffer_t *rxBuffer,unsigned char slave_id);

#endif /* __BL_UART_SERVERMG_H__ */
