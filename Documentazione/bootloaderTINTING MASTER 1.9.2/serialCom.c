/**/
/*============================================================================*/
/**
**      @file      SerialCom.c
**
**      @brief     Modulo di comunicazione seriale
**
**      @version   Alfa color tester
**/
/*============================================================================*/
/**/


/*======================== INCLUSIONI ======================================= */
#if defined (__PIC24FJ256GB110__) || defined(__PIC24FJ256GB106__)
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "HardwareProfile.h"
#endif

#include "Macro.h"
#include "ram.h"
#include "serialCom.h"
#include "BL_UART_ServerMg.h"

#include "timerMg.h"
#include "mem.h"
#include "const.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

/* ===== MACRO LOCALI ====================================================== */
/**
 * @brief Stores a byte into RX uartBuffer, performs boundary
 * checking.
 *
 * @param uartBuffer_t buf, RX buffer
 * @param char c, the character to be stored
 */
#define STORE_BYTE(buf, c)                      \
  do {                                          \
    (buf).buffer[(buf).index ++ ] = (c);        \
    if ((buf).index >= BUFFER_SIZE)             \
    {                                           \
      SIGNAL_ERROR();                           \
    }                                           \
  } while(0)

/**
 * @brief Resets the receiver
 */
#define RESET_RECEIVER()                        \
  do {                                          \
    BL_initBuffer(&BL_rxBuffer);                \
  } while (0)

/**
 * @brief Last char was an escape?
 */
#define IS_ESCAPE()                             \
  (BL_rxBuffer.escape != FALSE)

/**
 * @brief Signal last char was an escape
 */
#define SIGNAL_ESCAPE()                         \
  do {                                          \
    BL_rxBuffer.escape = TRUE;                  \
  } while (0)

/**
 * @brief Signal last char was not an escape
 */
#define CLEAR_ESCAPE()                          \
  do {                                          \
    BL_rxBuffer.escape = FALSE;                 \
  } while (0)

/**
 * @brief Serial error?
 */
#define IS_ERROR()                              \
  (BL_rxBuffer.bufferFlags.serialError != FALSE)

#define SIGNAL_ERROR()                          \
  do {                                          \
    BL_rxBuffer.bufferFlags.serialError = TRUE; \
  } while (0)

#define IS_FROM_DISPLAY(id)                     \
  ((id) < 100)

#define IS_FROM_SLAVE(id)                       \
  ((id) > 100)

/* These offsets apply ok, after payload unstuffying */
#define PUMP_STATE_OFS (4)
#define RLC_AIR_DETECTION_OFS (7)

#if defined (__PIC24FJ256GB110__)
#define ENABLE_TX_MULTIPROCESSOR LATDbits.LATD13
#elif defined(__PIC24FJ256GB106__)
#define ENABLE_TX_MULTIPROCESSOR LATCbits.LATC14
#else
#define ENABLE_TX_MULTIPROCESSOR LATAbits.LATA8
#endif

#define INPUT_PIN           1
#define OUTPUT_PIN          0

uartBuffer_t BL_rxBuffer;
uartBuffer_t BL_txBuffer;
serialSlave_t BL_serialSlave;
static unsigned char deviceID;

/* ===== PROTOTIPI FUNZIONI LOCALI ========================================= */
static void BL_makeMessage(void);
static void BL_decodeMessage(void);
static void BL_sendMessage(void);
static void BL_initBuffer(uartBuffer_t*);

static void BL_unstuffMessage();
static void BL_rebuildMessage(unsigned char);

/* ===== DEFINIZIONE FUNZIONI LOCALI ======================================= */
static void BL_unstuffMessage()
/*******************************************************************************
**
**   @brief Performs byte unstuffying on rx buffer. This function is a
**   private service of rebuildMessage()
**
**
*******************************************************************************/
{
    unsigned char i, j, c;

    // Skip 3 bytes from frame head: [ STX, ID, LEN ] 
    unsigned char *p = BL_rxBuffer.buffer + FRAME_PAYLOAD_START;

    /* i is the read index, j is the write index. For each iteration, j
     * is always incremented by 1, i may be incremented by 1 or 2,
     * depending on whether p[i] is a stuffed character or not. At the
     * end of the cycle (length bytes read) j is less than or equal to
     * i. (i - j) is the amount that must be subtracted to the payload
     * length. */

    i = j = 0;
    while (i < BL_rxBuffer.length) {
        c = *(p + i);
        ++ i;

        if (c == ASCII_ESC) {
            c = *(p + i) - ASCII_ZERO;
            ++ i;

            if (!c)
              *(p + j) = ASCII_ESC;
            else
              *(p + j) = c;
        }
        else
            *(p + j) = c;

        ++ j;
    }
    // Done with unstuffying, now fix payload length. 
    BL_rxBuffer.length -= (i - j);
}

static void BL_rebuildMessage(unsigned char receivedByte)
/*******************************************************************************
**
**      @brief Called by  _U1RXInterrupt: update the rx buffer  with
**             subsequent received bytes
**
**      @param receivedByte received bytes
**
**      @retval void
**
*******************************************************************************/
{
    if (! IS_ERROR()) {
        switch(BL_rxBuffer.status) {
            case WAIT_STX:
                if (receivedByte == ASCII_STX) {
                    STORE_BYTE(BL_rxBuffer, receivedByte);
                    BL_rxBuffer.status = WAIT_ID;
                }
            break;

            case WAIT_ID:
                STORE_BYTE(BL_rxBuffer, receivedByte);
                deviceID = REMOVE_OFFSET(receivedByte);
                if (! IS_VALID_ID(deviceID)) {
                    SIGNAL_ERROR();
                    Nop();
                    Nop();
                }
                else
                  BL_rxBuffer.status = WAIT_LENGTH;
            break;

            case WAIT_LENGTH:
                STORE_BYTE(BL_rxBuffer, receivedByte);
                if ( receivedByte < ADD_OFFSET( MIN_FRAME_SIZE) || receivedByte > ADD_OFFSET( MAX_FRAME_SIZE) )
                    SIGNAL_ERROR();
                else {
                    /* The length embedded in the frame takes into account the
                     * entire frame length, for ease of implementation of tx/rx
                     * code. Here we discard the final 5 bytes (4 CRC + ETX). Later
                     * on, after the crc check, we'll be able to discard also the
                     * initial overhead [ STX, ID, LEN ] */
                    BL_rxBuffer.length  = REMOVE_OFFSET(receivedByte);
                    BL_rxBuffer.length -= FRAME_END_OVERHEAD;
                    BL_rxBuffer.status = WAIT_DATA;
                }
            break;

            case WAIT_DATA:
                // Check stuffying encoding 
                if (IS_ESCAPE()) {
                    // ESC ZERO --> ESC, ESC TWO --> STX, ESC THREE --> ETX 
                    if (receivedByte != ASCII_ZERO && receivedByte != ASCII_TWO && receivedByte != ASCII_THREE)
                        // Ilegal encoding detected 
                        SIGNAL_ERROR();

                    CLEAR_ESCAPE();
                }
                else {
                    if (receivedByte == ASCII_ESC)
                      SIGNAL_ESCAPE();
                }
                STORE_BYTE(BL_rxBuffer, receivedByte);
                if (BL_rxBuffer.index == BL_rxBuffer.length)
                    BL_rxBuffer.status = WAIT_CRC;
            break;

            case WAIT_CRC:
                STORE_BYTE(BL_rxBuffer, receivedByte);
                // Received four CRC bytes? 
                if (BL_rxBuffer.index == FRAME_CRC_LENGTH + BL_rxBuffer.length)
                    BL_rxBuffer.status = WAIT_ETX;
            break;

            case WAIT_ETX:
                if (receivedByte != ASCII_ETX || ! CHECK_CRC16(&BL_rxBuffer))
                    SIGNAL_ERROR();
                else {
                    STORE_BYTE(BL_rxBuffer, receivedByte);
                    BL_rxBuffer.length -= FRAME_PAYLOAD_START;
                    // Frame ok, we can now "unstuff" the payload 
                    BL_unstuffMessage();
                    if (deviceID == SLAVE_DEVICE_ID(BL_slave_id))        
                      BL_rxBuffer.bufferFlags.rxCompleted = TRUE;
                    if (! BL_rxBuffer.bufferFlags.rxCompleted)
                      SIGNAL_ERROR();
                }
            break;

            default:
                SIGNAL_ERROR();
            break;
        } // switch 
    } // if (! IS_ERROR) 

    if (IS_ERROR())
        RESET_RECEIVER();
} // rebuildMessage() 

static void BL_makeMessage(void)
/*******************************************************************************
**      @brief Management of the fixed time window Display-slaves:
**             if the answer from the Involved slave is received,
**             the following actions are performed:
**             - update the serialSlave struct for the subsequent slave
**               interrogated,
**             - make the packet to be transmitted, calling the message
**               make function related to the new slave
**             If the time window is elapsed without answer and the number
**             of retries is lower than admitted:
**             - increase the number of retries for the current slave
**             - send again the message to the same slave
**
**      @param void
**
**      @retval void
*******************************************************************************/
{
#ifdef REMOTE_UPDATING        
    if ((BLState.livello == USB_CONNECT_EXECUTION) && (StatusTimer(T_RETRY_BROADCAST_MSG) != T_RUNNING) && (PtrJMPBoot == JUMP_TO_BOOT_DONE)) {
        if  (progBoot.num_retry_broadcast <= NUM_MAX_RETRY_BROADCAST) {
            setBootMessage(CMD_FORCE_SLAVE_BL);
            progBoot.num_retry_broadcast++;
            StartTimer(T_RETRY_BROADCAST_MSG);
        }
        else
            StopTimer(T_RETRY_BROADCAST_MSG);
    }
#else
    if ((BLState.livello == USB_CONNECT_EXECUTION) && (StatusTimer(T_RETRY_BROADCAST_MSG) != T_RUNNING) ) {
        if  (progBoot.num_retry_broadcast <= NUM_MAX_RETRY_BROADCAST) {
            setBootMessage(CMD_FORCE_SLAVE_BL);
            progBoot.num_retry_broadcast++;
            StartTimer(T_RETRY_BROADCAST_MSG);
        }
        else
            StopTimer(T_RETRY_BROADCAST_MSG);
    }
#endif    
    else if ((BLState.livello == JMP_TO_APP) && (StatusTimer(T_RETRY_BROADCAST_MSG) != T_RUNNING) ) {
        if  (progBoot.num_retry_broadcast <= NUM_MAX_RETRY_BROADCAST) {
            setBootMessage(CMD_JUMP_TO_APPLICATION);
            progBoot.num_retry_broadcast++;
            StartTimer(T_RETRY_BROADCAST_MSG);
        }
        else
            StopTimer(T_RETRY_BROADCAST_MSG);        
    }

    if(isUART_Reset() && StatusTimer(T_SEND_RESET_MSG) == T_ELAPSED) {
        if(progBoot.numSendingResetDeviceCmd <= NUM_MAX_SENDING_RESET_DEVICE_MESSAGE) {
            setBootMessage(RESET_SLAVE);
            StartTimer(T_SEND_RESET_MSG);
            progBoot.numSendingResetDeviceCmd++;
        }
        else {
            StopTimer(T_SEND_RESET_MSG);
            setEndProgramming();
        }
    }

    if ((BL_txBuffer.bufferFlags.txRequest == TRUE )&& (BL_txBuffer.bufferFlags.uartBusy == FALSE)) {
        BL_txBuffer.bufferFlags.txRequest = FALSE;
        BL_initBuffer(&BL_txBuffer);
        BL_serialSlave.makeSerialMsg(&BL_txBuffer,BL_slave_id);
        BL_txBuffer.bufferFlags.txReady = TRUE;
    }
}


static void BL_decodeMessage(void)
/*******************************************************************************
**      @brief decode the received message, calling the decode
**             function related to the Involved slave: call to
**             serialSlave->decodeSerialMsg(&rxBuffer)
**
**
**      @param void
**
**      @retval void
*******************************************************************************/
{
    // A seconda dello stato e dello slave interrogato, chiamo le differenti funzioni di decodifica
    if ( BL_rxBuffer.bufferFlags.rxCompleted == TRUE) {	  
        StopTimer(T_SLAVE_WINDOW_TIMER);
        if (StatusTimer(T_DELAY_INTRA_FRAMES) == T_HALTED)
            StartTimer(T_DELAY_INTRA_FRAMES);
        else if (StatusTimer(T_DELAY_INTRA_FRAMES) == T_ELAPSED) {		
            BL_serialSlave.decodeSerialMsg(&BL_rxBuffer,BL_slave_id);
            BL_initBuffer(&BL_rxBuffer);
            StopTimer(T_DELAY_INTRA_FRAMES);
            if (Fw_Request == TRUE) {
                Fw_Request = FALSE;
                progBoot.typeMessage = SEND_FRMWR_VERSION;
            }            
        }
    }
    
    // Timeout Ricezione comandi scaduto
    else if (StatusTimer(T_SLAVE_WINDOW_TIMER) == T_ELAPSED) {
        StopTimer(T_SLAVE_WINDOW_TIMER);
        StopTimer(T_DELAY_INTRA_FRAMES);
        BL_initBuffer(&BL_rxBuffer);
        if (Fw_Request == TRUE) {
            Fw_Request = FALSE;
            setNewProcessingMsg();
            progBoot.typeMessage = SEND_FRMWR_VERSION;
        }
    }
    
}

static void BL_sendMessage(void)
/*******************************************************************************
**      @brief Start the transmission, enabling the UART 3 transmission
**             flag and filling the UART3 tx buffer with the first byte
**             to be transmitted
**
**
**      @param void
**
**      @retval void
*******************************************************************************/
{
    if(BL_txBuffer.bufferFlags.txReady == TRUE) {
        if ((BL_txBuffer.bufferFlags.uartBusy != TRUE) && (BL_txBuffer.length <= BUFFER_SIZE)) {
            // Enable Tx multiprocessor line
            ENABLE_TX_MULTIPROCESSOR=1;
            // Pulisco l'interrupt flag della trasmissione
            IFS5bits.U3TXIF = 0;
            // Abilito il flag UARTx En
            IEC5bits.U3TXIE = 1;
            // Scarico il primo byte nel buffer di trasmissione : Write data byte to lower byte of UxTXREG word
            // Take control of buffer
            BL_txBuffer.bufferFlags.uartBusy = TRUE;
            if (U3STAbits.TRMT)
                U3TXREG = BL_txBuffer.buffer[BL_txBuffer.index++];
            else {
                U3MODEbits.UARTEN = 0;
                BL_initSerialCom();
            }
        }
    }
}

static void BL_initBuffer(uartBuffer_t *buffer)
/*******************************************************************************
**      @brief init buffer
**
**      @param buffer pointer to the buffer
**
**      @retval void
*******************************************************************************/
{
    memset(buffer->buffer, 0, BUFFER_SIZE);
    buffer->bufferFlags.allFlags = 0;

    buffer->status = WAIT_STX;
    buffer->index = 0;
    buffer->length = 0;
    buffer->escape = FALSE;
}

void BL_initSerialCom(void)
/*******************************************************************************
**      @brief Set UART3 registers; reset rreceiver and transmission
**             buffers and flags. Start the FIRST_LINK timer window
**
**      @param void
**
**      @retval void
*******************************************************************************/
{
    // Make sure to set LAT bit corresponding to TxPin as high before UART initialization
    // EN TX
    LATDbits.LATD11 = 1;
    TRISDbits.TRISD11 = OUTPUT;
    // EN RX
    LATDbits.LATD12 = 0;
    TRISDbits.TRISD12 = INPUT;
    // UART3 ENABLE MULTIPROCESSOR RD13
    ENABLE_TX_MULTIPROCESSOR = 0;
    TRISDbits.TRISD13  = OUTPUT;
	// BaudRate = 115200; Clock Frequency = 32MHz; 
    U3BRG = 33;    
    // UARTEN = Disabled - USIDL = Continue module operation in Idle Mode - IREN = IrDa Encoder and Decoder disabled - RTSMD = UxRTS pin in Flow Control Mode
    // UEN1:UEN0 = UxTX and UxRX pins are enabled and used - WAKE = No Wake-up enabled - LPBACK = Loopback mode is disabled - ABAUD = Baud rate measurement disabled or completed
    // RXINV = UxRX Idle state is '1' - BRGH = High-Speed mode - PDSEL = 8-bit data, no parity - STSEL = One Stop bit 
    U3MODE = 0x08;
    U3STA = 0x00;
    // Enabling UARTEN bit
    U3MODEbits.UARTEN  = 1;      
    // Interrupt when last char is tranferred into TSR Register: so transmit buffer is empty
    U3STAbits.UTXISEL1 = 0;
    U3STAbits.UTXISEL0 = 1;
    // Transmit Enable
    U3STAbits.UTXEN = 1; 
    // Reset Interrupt flags
    IFS5bits.U3RXIF = 0;
    IFS5bits.U3TXIF = 0;
    // Start RX
    IEC5bits.U3RXIE = 1;
        
    BL_initBuffer(&BL_rxBuffer);
    BL_initBuffer(&BL_txBuffer);

    BL_serialSlave.makeSerialMsg=&MakeBootMessage;
    BL_serialSlave.decodeSerialMsg=&DecodeBootMessage;
    
    StopTimer(T_SLAVE_WINDOW_TIMER);
}

void BL_serialCommManager(void)
/*******************************************************************************
**      @brief Sequencer of the module
**
**      @param void
**
**      @retval void
*******************************************************************************/
{
    BL_decodeMessage();
    BL_makeMessage();
    BL_sendMessage();
}

void __attribute__((__interrupt__, no_auto_psv)) _AltU3TXInterrupt(void)
/*******************************************************************************
**      @brief Interrupt in tx della UART3
**
**      @param void
**
**      @retval void
*******************************************************************************/
{
    if (_U3TXIE && _U3TXIF) {
        _U3TXIF = 0;
        if (BL_txBuffer.index == BL_txBuffer.length) {
            // Disable Tx multiprocessor line
            ENABLE_TX_MULTIPROCESSOR = 0;
            // Disabilito il flag UARTx En
            IEC5bits.U3TXIE = 0;
            BL_txBuffer.bufferFlags.uartBusy = FALSE;
            BL_txBuffer.bufferFlags.txReady = FALSE;
        }
        else
            U3TXREG = BL_txBuffer.buffer[BL_txBuffer.index++];
    }
}

void __attribute__((__interrupt__, no_auto_psv)) _AltU3RXInterrupt(void)
/*******************************************************************************
**      @brief Interrupt in tx della UART3
**
**      @param void
**
**      @retval void
*******************************************************************************/
{
    register unsigned char flushUart;

    if (_U3RXIE && _U3RXIF) {
        _U3RXIF = 0;
        // Overrun Error
        if (U3STAbits.OERR) {
            // Segnalazione Overrun Error 
            U3STAbits.OERR = 0;
            SIGNAL_ERROR();
        }
        // Framing Error
        if (U3STAbits.FERR) {
            flushUart = U3RXREG;
            // Segnalazione Framing Error
            SIGNAL_ERROR();
        }
        BL_rebuildMessage(U3RXREG);
    }
}


void BL_stuff_byte(unsigned char *buf, unsigned char *ndx, char c)
/*******************************************************************************
**
**   @brief Writes c at the ndx-th position of buf, performing byte
**   stuffying if necessary. (*ndx) is incremented accordingly.
**
**   @param buf, the output buffer
**   @param ndx, a pointer to the current writing position in the output buffer
**   @param c, the character to be written
**
**
*******************************************************************************/
{
    // STX --> ESC TWO, ETX --> ESC THREE 
    if ((c == ASCII_STX) || (c == ASCII_ETX)) {
        WRITE_BYTE(buf, *ndx, ASCII_ESC);
        WRITE_BYTE(buf, *ndx, c + ASCII_ZERO);
    }
    // ESC --> ESC ZERO 
    else if (c == ASCII_ESC) {
        WRITE_BYTE(buf, *ndx, ASCII_ESC);
        WRITE_BYTE(buf, *ndx, ASCII_ZERO);
    }
    // Regular char, nothing fancy here 
    else
        WRITE_BYTE(buf, *ndx, c);
}
