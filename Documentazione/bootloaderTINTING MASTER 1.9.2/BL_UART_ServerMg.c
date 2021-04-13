/**/
/*========================================================================== */
/**
 **      @file    BL_UART_ServerMg.c
 **
 **      @brief   UART I/O management module
 **/
/*========================================================================== */
/**/

#include "GenericTypeDefs.h"
#include "Compiler.h"

#include "define.h"
#include "serialCom.h"
#include "Macro.h"
#include "ram.h"
#include "progMemFunctions.h"
#include "Timermg.h"
#include "BL_UART_ServerMg.h"

progBoot_t progBoot;

void MakeBootMessage(uartBuffer_t *txBuffer, unsigned char slave_id)
/*******************************************************************************
**  @brief Create the serial message for  MABRD
**
**  @param txBuffer pointer to the tx buffer
**
**  @param slave_id slave identifier
**
**  @retval void
**
*******************************************************************************/
{
    unsigned char idx = 0;
    unsigned char i;
    // Initialize tx frame, reserve extra byte for pktlen
    FRAME_BEGIN(txBuffer, idx, slave_id);
    if (slave_id != 0 && slave_id != 0xff) {
        Nop();
    }
    STUFF_BYTE(txBuffer->buffer, idx, progBoot.typeMessage);

    switch (progBoot.typeMessage) {
        case CMD_FRMWR_REQUEST:	
pippo3 = 1;            
        break;

        case CMD_FW_UPLOAD:
            if (BL_slave_id == TINTING_ID) {		
                STUFF_BYTE( txBuffer->buffer, idx, LSB_LSW(progBoot.startAddress));
                STUFF_BYTE( txBuffer->buffer, idx, MSB_LSW(progBoot.startAddress));
                STUFF_BYTE( txBuffer->buffer, idx, LSB_MSW(progBoot.startAddress));
            }
            else {				
                STUFF_BYTE( txBuffer->buffer, idx, LSB(progBoot.startAddress));
                STUFF_BYTE( txBuffer->buffer, idx, MSB(progBoot.startAddress));
            }
            STUFF_BYTE( txBuffer->buffer, idx, LSB(progBoot.numDataPack));
            STUFF_BYTE( txBuffer->buffer, idx, MSB(progBoot.numDataPack));
        break;

        case DATA_FW_UPLOAD:
            STUFF_BYTE( txBuffer->buffer, idx, LSB(progBoot.idDataPackTx));
            STUFF_BYTE( txBuffer->buffer, idx, MSB(progBoot.idDataPackTx));
            if (BL_slave_id == TINTING_ID) {		
                STUFF_BYTE( txBuffer->buffer, idx, LSB_LSW(progBoot.address));
                STUFF_BYTE( txBuffer->buffer, idx, MSB_LSW(progBoot.address));
                STUFF_BYTE( txBuffer->buffer, idx, LSB_MSW(progBoot.address));
            }
            else {					
                STUFF_BYTE( txBuffer->buffer, idx, LSB(progBoot.address));
                STUFF_BYTE( txBuffer->buffer, idx, MSB(progBoot.address));
            }
            STUFF_BYTE( txBuffer->buffer, idx, progBoot.numDataBytesPack);

            for(i = 0; i < BYTES2WORDS(RequestDataBlockSize); ++ i) {
              STUFF_BYTE( txBuffer->buffer, idx, LSB(progBoot.bufferData[i]));
              STUFF_BYTE( txBuffer->buffer, idx, MSB(progBoot.bufferData[i]));
            }
        break;

        case END_DATA_FW_UPLOAD:
            STUFF_BYTE( txBuffer->buffer, idx, LSB(progBoot.idDataPackTx));
            STUFF_BYTE( txBuffer->buffer, idx, MSB(progBoot.idDataPackTx));
            if (BL_slave_id == TINTING_ID) {		
                STUFF_BYTE( txBuffer->buffer, idx, LSB_LSW(progBoot.address));
                STUFF_BYTE( txBuffer->buffer, idx, MSB_LSW(progBoot.address));
                STUFF_BYTE( txBuffer->buffer, idx, LSB_MSW(progBoot.address));
            }
            else {					
                STUFF_BYTE( txBuffer->buffer, idx, LSB(progBoot.address));
                STUFF_BYTE( txBuffer->buffer, idx, MSB(progBoot.address));
            }
            STUFF_BYTE( txBuffer->buffer, idx, progBoot.numDataBytesPack);

            for(i = 0; i < BYTES2WORDS(progBoot.numDataBytesPack); ++ i) {
                STUFF_BYTE( txBuffer->buffer, idx, LSB(progBoot.bufferData[i]));
                STUFF_BYTE( txBuffer->buffer, idx, MSB(progBoot.bufferData[i]));
            }
        break;

         case GET_PROGRAM_DATA:
            STUFF_BYTE( txBuffer->buffer, idx, progBoot.numDataBytesPack);
            if (BL_slave_id == TINTING_ID) {		
                STUFF_BYTE( txBuffer->buffer, idx, LSB_LSW(progBoot.address));
                STUFF_BYTE( txBuffer->buffer, idx, MSB_LSW(progBoot.address));
                STUFF_BYTE( txBuffer->buffer, idx, LSB_MSW(progBoot.address));
            }
            else {					
                STUFF_BYTE( txBuffer->buffer, idx, LSB(progBoot.address));
                STUFF_BYTE( txBuffer->buffer, idx, MSB(progBoot.address));
            }
        break;

        case RESET_SLAVE:
        break;

        default:
        break;
    }
    FRAME_END( txBuffer, idx);
}

void DecodeBootMessage(uartBuffer_t *rxBuffer, unsigned char slave_id)
/*******************************************************************************
**  @brief Create the serial message for  MMT
**
**  @param txBuffer pointer to the tx buffer
**
**  @param slave_id slave identifier
**
**  @retval void
**
 ******************************************************************************/
{
    unsigned char idx = FRAME_PAYLOAD_START;
    unsigned char i;
    unionWord_t  tmpWord1;

    // Suppress warning 
    (void) slave_id;
    progBoot.typeMessage = rxBuffer->buffer[idx++];
    switch (progBoot.typeMessage) {
        case SEND_FRMWR_VERSION:
            // Versione Firmware Slave
            BL_SLAVE_VERSION[0] = rxBuffer->buffer[idx++];
            BL_SLAVE_VERSION[1] = rxBuffer->buffer[idx++];
            BL_SLAVE_VERSION[2] = rxBuffer->buffer[idx++];
        break;

        case ACK_FW_UPLOAD:
            // ID del pacchetto di dati ricevuto 
            tmpWord1.byte[0]= rxBuffer->buffer[idx++];
            tmpWord1.byte[1]= rxBuffer->buffer[idx++];
            progBoot.idDataPackRx = tmpWord1.uword;
        break;

        case NACK_FW_UPLOAD:
            // ID dell'ultimo pacchetto di dati ricevuto
            tmpWord1.byte[0]= rxBuffer->buffer[idx++];
            tmpWord1.byte[1]= rxBuffer->buffer[idx++];
            progBoot.idDataPackRx = tmpWord1.uword;
            // Tipologia di errore riscontrato 
            progBoot.errorType =  rxBuffer->buffer[idx++];
        break;

        case SEND_PROGRAM_DATA:
            // Pacchetto contenente copia dati flash ACT 
            for( i = 0; i < BYTES2WORDS(RequestDataBlockSize); ++ i) {
                tmpWord1.byte[0] = rxBuffer->buffer[idx++];
                tmpWord1.byte[1] = rxBuffer->buffer[idx++];
                progBoot.bufferData[i] = tmpWord1.uword;
            }
        break;

        default:
        break;
    }
    setNewProcessingMsg();
}

void setBootMessage(unsigned char packet_type)
/*******************************************************************************
**
**   @brief Request to send the packet_type serial message
**
**   @param packet_type type of packet
**
**   @return void
**
*******************************************************************************/
{
    progBoot.typeMessage = packet_type;
    setTxRequestMsg();
}
