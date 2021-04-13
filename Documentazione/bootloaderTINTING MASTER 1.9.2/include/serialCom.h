/**/
/*============================================================================*/
/**
**      @file      SerialCom.h
**
**      @brief     Header file relativo a SerialCom.c
**
**      @version   Alfa color tester
**/
/*============================================================================*/
/**/

#ifndef _SERIAL_COM_H_
#define _SERIAL_COM_H_

#include <stdlib.h> /* for NULL */
#include "main.h"

/*===== INCLUSIONI ========================================================= */
/*===== DICHIARAZIONI LOCALI ================================================*/
/*===== DEFINE GENERICHE ====================================================*/

/* reserved ASCII codes */
#define ASCII_STX (0x02)
#define ASCII_ETX (0x03)
#define ASCII_ESC (0x1B)

/* stuffying encodings */
#define ASCII_ZERO   ('0') /* ESC -> ESC ZERO  */
#define ASCII_TWO    ('2') /* STX -> ESC TWO   */
#define ASCII_THREE  ('3') /* ETX -> ESC THREE */

/* maximum payload size, calculated as slightly worse than the worst
 * case, with stuffying applied to *all* the bytes of the longest
 * message. */
#define BUFFER_SIZE (130)

#define FRAME_LENGTH_BYTE_POS    (2)
#define FRAME_PAYLOAD_START      (1 + FRAME_LENGTH_BYTE_POS)
#define FRAME_BGN_OVERHEAD       (FRAME_PAYLOAD_START)

#define FRAME_CRC_LENGTH         (4)
#define FRAME_END_OVERHEAD       (1 + FRAME_CRC_LENGTH)
#define MIN_FRAME_SIZE           (1 + FRAME_BGN_OVERHEAD + FRAME_END_OVERHEAD)
#define MAX_FRAME_SIZE           (BUFFER_SIZE)

#define LSN(x) ((x) & 0x0F)                                // Least Significant Nibble
#define MSN(x) (((x) & 0xF0) >> 4)                         // Most Significant Nibble
#define LSB(x) ((x) & 0x00FF)                              // Least Significant Byte
#define MSB(x) (((x) & 0xFF00) >> 8)                       // Most Significant Byte
#define LSW(x) ((unsigned long)(x) & 0x0000FFFFL)          // Least Significant Word
#define MSW(x) (((unsigned long)(x) & 0xFFFF0000) >> 16)   // Most Significant Word
#define MSB_MSW(x) (((x) & 0xFF000000L) >> 24)             // Most Significant Byte of Most Significant Word
#define LSB_MSW(x) (((x) & 0x00FF0000L) >> 16)             // Least Significant Byte of Most Significant Word
#define MSB_LSW(x) (((x) & 0x0000FF00L) >> 8)              // Most Significant Byte of Least Significant Word
#define LSB_LSW(x) (((x) & 0x000000FFL))                   // Least Significant Byte of Least Significant Word

//#define NUM_MAX_RETRY_BROADCAST                (10)
#define NUM_MAX_RETRY_BROADCAST                (30)
#define NUM_MAX_SENDING_RESET_DEVICE_MESSAGE   (5)

#define BROADCAST_ID 0x00

/**
 * @brief The device ID for MASTER -> SLAVE msgs
 */
#define MASTER_DEVICE_ID(id)                    \
  (id)

/**
 * @brief The device ID for SLAVE -> MASTER msgs
 */
#define SLAVE_DEVICE_ID(id)                     \
  (100 + (id))

/**
 * @brief True iff ID is a valid query or reply ID
 */
#define IS_VALID_ID(id)                                 \
  (((0   < (id)) && ((id) <=       N_SLAVES)) ||        \
   ((100 < (id)) && ((id) <= 100 + N_SLAVES)))

/**
 * @brief All fixed position values are transferred with
 * a positive offset to avoid clashes with reserved bytes.
 */
#define ADD_OFFSET(c)                           \
  ((c) + 0x20)
#define REMOVE_OFFSET(c)                        \
  ((c) - 0x20)

/**
 * @brief low-level write c into buf and increment index.
 * @param buf, the buffer to write to
 * @param ndx, the index to write c at
 * @param c, the character to be written
 */
#define WRITE_BYTE(buf, ndx, c)                   \
  do {                                            \
    *((buf) + (ndx)) = (c);                       \
    ++ (ndx);                                     \
  } while (0)

/* just an alias for code uniformity */
#define STUFF_BYTE(buf, ndx, c) \
  BL_stuff_byte((buf), &(ndx), (c))

/**
 * @brief Frame initialization
 */
#define FRAME_BEGIN(txb, idx, id)                               \
  do {                                                          \
    WRITE_BYTE((txb)->buffer, (idx), ASCII_STX);                \
    WRITE_BYTE((txb)->buffer, (idx), ADD_OFFSET( (id)));        \
    ++ (idx); /* reserved for pktlen */                         \
  } while (0)

/**
 * @brief Frame finalization
 */
#define FRAME_END(txb, idx)                                             \
  do {                                                                  \
    unionWord_t crc;                                                    \
                                                                        \
    /* fix pkt len */                                                   \
    (txb)->buffer [ FRAME_LENGTH_BYTE_POS ] =                           \
      ADD_OFFSET( FRAME_END_OVERHEAD + (idx));                          \
    (txb)->length = ( FRAME_END_OVERHEAD + (idx));                      \
                                                                        \
    /* crc16, sent one nibble at the time, w/ offset, big-endian */     \
    crc.uword = CRCarea((txb)->buffer, (idx), NULL);                 \
    WRITE_BYTE((txb)->buffer, (idx), ADD_OFFSET( MSN( crc.byte[1])));   \
    WRITE_BYTE((txb)->buffer, (idx), ADD_OFFSET( LSN( crc.byte[1])));   \
    WRITE_BYTE((txb)->buffer, (idx), ADD_OFFSET( MSN( crc.byte[0])));   \
    WRITE_BYTE((txb)->buffer, (idx), ADD_OFFSET( LSN( crc.byte[0])));   \
                                                                        \
    /* ETX = frame end */                                               \
    WRITE_BYTE((txb)->buffer, (idx), ASCII_ETX);                        \
  } while (0)

/**
 * @brief Check CRC16, CRC is expected to be sent over the wire in
 * Big-Endian, offseted format.
 * @return true iff CRC checks ok
 */
#define CHECK_CRC16(rxb) \
  ((((unsigned short)(REMOVE_OFFSET((rxb)->buffer[(rxb)->index - 4])) << 0xC) | \
    ((unsigned short)(REMOVE_OFFSET((rxb)->buffer[(rxb)->index - 3])) << 0x8) | \
    ((unsigned short)(REMOVE_OFFSET((rxb)->buffer[(rxb)->index - 2])) << 0x4) | \
    ((unsigned short)(REMOVE_OFFSET((rxb)->buffer[(rxb)->index - 1])) << 0x0) ) \
   == CRCarea((rxb)->buffer, (rxb)->length, NULL))


// stati del buffer di ricezione
enum
{
  /* 0 */ WAIT_STX,
  /* 1 */ WAIT_ID,
  /* 2 */ WAIT_LENGTH,
  /* 3 */ WAIT_DATA,
  /* 4 */ WAIT_CRC,
  /* 5 */ WAIT_ETX,
};

/*===== TIPI ================================================================*/
typedef struct
{
  unsigned char buffer[BUFFER_SIZE];
  unsigned char length;
  unsigned char index;
  unsigned char status;
  unsigned char escape; // rx only
  union __attribute__ ((packed))
    {
      unsigned char allFlags;
      struct
      {
        unsigned char txRequest   : 1;
        unsigned char txReady     : 1;
        unsigned char decodeDone  : 1;
        unsigned char rxCompleted : 1;
        unsigned char serialError : 1;
        unsigned char uartBusy    : 1;
        unsigned char startTx     : 1;
        unsigned char unused      : 1;
      };
    } bufferFlags;
} uartBuffer_t;


typedef struct
{
  void (*makeSerialMsg)(uartBuffer_t *, unsigned char);
  void (*decodeSerialMsg)(uartBuffer_t *,unsigned char);
  unsigned char numRetry;
  unsigned char procMsg;
} serialSlave_t;


extern serialSlave_t BL_serialSlave;
extern uartBuffer_t BL_rxBuffer;
extern uartBuffer_t BL_txBuffer;
/*===== PROTOTIPI FUNZIONI ==================================================*/
#define isNewProcessingMsg()          (BL_serialSlave.procMsg)
#define resetNewProcessingMsg()       (BL_serialSlave.procMsg = FALSE)
#define setNewProcessingMsg()         (BL_serialSlave.procMsg = TRUE)
#define setTxRequestMsg()             (BL_txBuffer.bufferFlags.txRequest = TRUE)


extern void BL_initSerialCom(void);
extern void BL_serialCommManager(void);

extern void BL_stuff_byte(unsigned char *buf, unsigned char *ndx, char c);

#endif
