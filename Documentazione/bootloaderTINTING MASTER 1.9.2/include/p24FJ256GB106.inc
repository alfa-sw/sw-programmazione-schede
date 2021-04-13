;==========================================================================
;  PIC24FJ256GB106 Standard Assembly Include File
; 
;  (c) Copyright 2008 Microchip Technology, All rights reserved
;==========================================================================

.NOLIST

;==========================================================================
;  This header file defines configurations, registers, and other useful
;  bits of information for the PIC24FJ256GB106 microcontroller.  These names
;  are taken to match the data sheets as closely as possible.
;
;  Note that the processor must be selected before this file is included.
;  The processor may be selected the following ways:
;
;       1. Command line switch:
;               C:\> pic30-as file.s -p24FJ256GB106
;       2. Placing a ".equ" directive before the ".include":
;               .equ __24FJ256GB106, 1
;               .include "p24FJ256GB106.inc"
;       3. Setting the processor in the MPLAB IDE Project Dialog
;==========================================================================

;==========================================================================
;
;       Verify Processor
;
;==========================================================================

.IFNDEF __24FJ256GB106
   .ERROR "Include file does not match processor setting."
.ENDIF


;==========================================================================
;
;       Register Definitions
;
;==========================================================================

;----- Register Equates (Low Byte, High Byte) -----------------------------

        .extern WREG0
        .equiv  WREG0L, _WREG0
        .equiv  WREG0H, _WREG0+1

        .extern WREG1
        .equiv  WREG1L, _WREG1
        .equiv  WREG1H, _WREG1+1

        .extern WREG2
        .equiv  WREG2L, _WREG2
        .equiv  WREG2H, _WREG2+1

        .extern WREG3
        .equiv  WREG3L, _WREG3
        .equiv  WREG3H, _WREG3+1

        .extern WREG4
        .equiv  WREG4L, _WREG4
        .equiv  WREG4H, _WREG4+1

        .extern WREG5
        .equiv  WREG5L, _WREG5
        .equiv  WREG5H, _WREG5+1

        .extern WREG6
        .equiv  WREG6L, _WREG6
        .equiv  WREG6H, _WREG6+1

        .extern WREG7
        .equiv  WREG7L, _WREG7
        .equiv  WREG7H, _WREG7+1

        .extern WREG8
        .equiv  WREG8L, _WREG8
        .equiv  WREG8H, _WREG8+1

        .extern WREG9
        .equiv  WREG9L, _WREG9
        .equiv  WREG9H, _WREG9+1

        .extern WREG10
        .equiv  WREG10L, _WREG10
        .equiv  WREG10H, _WREG10+1

        .extern WREG11
        .equiv  WREG11L, _WREG11
        .equiv  WREG11H, _WREG11+1

        .extern WREG12
        .equiv  WREG12L, _WREG12
        .equiv  WREG12H, _WREG12+1

        .extern WREG13
        .equiv  WREG13L, _WREG13
        .equiv  WREG13H, _WREG13+1

        .extern WREG14
        .equiv  WREG14L, _WREG14
        .equiv  WREG14H, _WREG14+1

        .extern WREG15
        .equiv  WREG15L, _WREG15
        .equiv  WREG15H, _WREG15+1

        .extern SPLIM
        .equiv  SPLIML, _SPLIM
        .equiv  SPLIMH, _SPLIM+1

        .extern PCL
        .equiv  PCLL, _PCL
        .equiv  PCLH, _PCL+1

        .extern PCH
        .equiv  PCHL, _PCH
        .equiv  PCHH, _PCH+1

        .extern TBLPAG
        .equiv  TBLPAGL, _TBLPAG
        .equiv  TBLPAGH, _TBLPAG+1

        .extern PSVPAG
        .equiv  PSVPAGL, _PSVPAG
        .equiv  PSVPAGH, _PSVPAG+1

        .extern RCOUNT
        .equiv  RCOUNTL, _RCOUNT
        .equiv  RCOUNTH, _RCOUNT+1

        .extern SR
        .equiv  SRL, _SR
        .equiv  SRH, _SR+1

        .extern CORCON
        .equiv  CORCONL, _CORCON
        .equiv  CORCONH, _CORCON+1

        .extern DISICNT
        .equiv  DISICNTL, _DISICNT
        .equiv  DISICNTH, _DISICNT+1

        .extern CNPD1
        .equiv  CNPD1L, _CNPD1
        .equiv  CNPD1H, _CNPD1+1

        .extern CNPD2
        .equiv  CNPD2L, _CNPD2
        .equiv  CNPD2H, _CNPD2+1

        .extern CNPD3
        .equiv  CNPD3L, _CNPD3
        .equiv  CNPD3H, _CNPD3+1

        .extern CNPD4
        .equiv  CNPD4L, _CNPD4
        .equiv  CNPD4H, _CNPD4+1

        .extern CNPD5
        .equiv  CNPD5L, _CNPD5
        .equiv  CNPD5H, _CNPD5+1

        .extern CNPD6
        .equiv  CNPD6L, _CNPD6
        .equiv  CNPD6H, _CNPD6+1

        .extern CNEN1
        .equiv  CNEN1L, _CNEN1
        .equiv  CNEN1H, _CNEN1+1

        .extern CNEN2
        .equiv  CNEN2L, _CNEN2
        .equiv  CNEN2H, _CNEN2+1

        .extern CNEN3
        .equiv  CNEN3L, _CNEN3
        .equiv  CNEN3H, _CNEN3+1

        .extern CNEN4
        .equiv  CNEN4L, _CNEN4
        .equiv  CNEN4H, _CNEN4+1

        .extern CNEN5
        .equiv  CNEN5L, _CNEN5
        .equiv  CNEN5H, _CNEN5+1

        .extern CNEN6
        .equiv  CNEN6L, _CNEN6
        .equiv  CNEN6H, _CNEN6+1

        .extern CNPU1
        .equiv  CNPU1L, _CNPU1
        .equiv  CNPU1H, _CNPU1+1

        .extern CNPU2
        .equiv  CNPU2L, _CNPU2
        .equiv  CNPU2H, _CNPU2+1

        .extern CNPU3
        .equiv  CNPU3L, _CNPU3
        .equiv  CNPU3H, _CNPU3+1

        .extern CNPU4
        .equiv  CNPU4L, _CNPU4
        .equiv  CNPU4H, _CNPU4+1

        .extern CNPU5
        .equiv  CNPU5L, _CNPU5
        .equiv  CNPU5H, _CNPU5+1

        .extern CNPU6
        .equiv  CNPU6L, _CNPU6
        .equiv  CNPU6H, _CNPU6+1

        .extern INTCON1
        .equiv  INTCON1L, _INTCON1
        .equiv  INTCON1H, _INTCON1+1

        .extern INTCON2
        .equiv  INTCON2L, _INTCON2
        .equiv  INTCON2H, _INTCON2+1

        .extern IFS0
        .equiv  IFS0L, _IFS0
        .equiv  IFS0H, _IFS0+1

        .extern IFS1
        .equiv  IFS1L, _IFS1
        .equiv  IFS1H, _IFS1+1

        .extern IFS2
        .equiv  IFS2L, _IFS2
        .equiv  IFS2H, _IFS2+1

        .extern IFS3
        .equiv  IFS3L, _IFS3
        .equiv  IFS3H, _IFS3+1

        .extern IFS4
        .equiv  IFS4L, _IFS4
        .equiv  IFS4H, _IFS4+1

        .extern IFS5
        .equiv  IFS5L, _IFS5
        .equiv  IFS5H, _IFS5+1

        .extern IEC0
        .equiv  IEC0L, _IEC0
        .equiv  IEC0H, _IEC0+1

        .extern IEC1
        .equiv  IEC1L, _IEC1
        .equiv  IEC1H, _IEC1+1

        .extern IEC2
        .equiv  IEC2L, _IEC2
        .equiv  IEC2H, _IEC2+1

        .extern IEC3
        .equiv  IEC3L, _IEC3
        .equiv  IEC3H, _IEC3+1

        .extern IEC4
        .equiv  IEC4L, _IEC4
        .equiv  IEC4H, _IEC4+1

        .extern IEC5
        .equiv  IEC5L, _IEC5
        .equiv  IEC5H, _IEC5+1

        .extern IPC0
        .equiv  IPC0L, _IPC0
        .equiv  IPC0H, _IPC0+1

        .extern IPC1
        .equiv  IPC1L, _IPC1
        .equiv  IPC1H, _IPC1+1

        .extern IPC2
        .equiv  IPC2L, _IPC2
        .equiv  IPC2H, _IPC2+1

        .extern IPC3
        .equiv  IPC3L, _IPC3
        .equiv  IPC3H, _IPC3+1

        .extern IPC4
        .equiv  IPC4L, _IPC4
        .equiv  IPC4H, _IPC4+1

        .extern IPC5
        .equiv  IPC5L, _IPC5
        .equiv  IPC5H, _IPC5+1

        .extern IPC6
        .equiv  IPC6L, _IPC6
        .equiv  IPC6H, _IPC6+1

        .extern IPC7
        .equiv  IPC7L, _IPC7
        .equiv  IPC7H, _IPC7+1

        .extern IPC8
        .equiv  IPC8L, _IPC8
        .equiv  IPC8H, _IPC8+1

        .extern IPC9
        .equiv  IPC9L, _IPC9
        .equiv  IPC9H, _IPC9+1

        .extern IPC10
        .equiv  IPC10L, _IPC10
        .equiv  IPC10H, _IPC10+1

        .extern IPC11
        .equiv  IPC11L, _IPC11
        .equiv  IPC11H, _IPC11+1

        .extern IPC12
        .equiv  IPC12L, _IPC12
        .equiv  IPC12H, _IPC12+1

        .extern IPC13
        .equiv  IPC13L, _IPC13
        .equiv  IPC13H, _IPC13+1

        .extern IPC15
        .equiv  IPC15L, _IPC15
        .equiv  IPC15H, _IPC15+1

        .extern IPC16
        .equiv  IPC16L, _IPC16
        .equiv  IPC16H, _IPC16+1

        .extern IPC18
        .equiv  IPC18L, _IPC18
        .equiv  IPC18H, _IPC18+1

        .extern IPC19
        .equiv  IPC19L, _IPC19
        .equiv  IPC19H, _IPC19+1

        .extern IPC20
        .equiv  IPC20L, _IPC20
        .equiv  IPC20H, _IPC20+1

        .extern IPC21
        .equiv  IPC21L, _IPC21
        .equiv  IPC21H, _IPC21+1

        .extern IPC22
        .equiv  IPC22L, _IPC22
        .equiv  IPC22H, _IPC22+1

        .extern IPC23
        .equiv  IPC23L, _IPC23
        .equiv  IPC23H, _IPC23+1

        .extern TMR1
        .equiv  TMR1L, _TMR1
        .equiv  TMR1H, _TMR1+1

        .extern PR1
        .equiv  PR1L, _PR1
        .equiv  PR1H, _PR1+1

        .extern T1CON
        .equiv  T1CONL, _T1CON
        .equiv  T1CONH, _T1CON+1

        .extern TMR2
        .equiv  TMR2L, _TMR2
        .equiv  TMR2H, _TMR2+1

        .extern TMR3HLD
        .equiv  TMR3HLDL, _TMR3HLD
        .equiv  TMR3HLDH, _TMR3HLD+1

        .extern TMR3
        .equiv  TMR3L, _TMR3
        .equiv  TMR3H, _TMR3+1

        .extern PR2
        .equiv  PR2L, _PR2
        .equiv  PR2H, _PR2+1

        .extern PR3
        .equiv  PR3L, _PR3
        .equiv  PR3H, _PR3+1

        .extern T2CON
        .equiv  T2CONL, _T2CON
        .equiv  T2CONH, _T2CON+1

        .extern T3CON
        .equiv  T3CONL, _T3CON
        .equiv  T3CONH, _T3CON+1

        .extern TMR4
        .equiv  TMR4L, _TMR4
        .equiv  TMR4H, _TMR4+1

        .extern TMR5HLD
        .equiv  TMR5HLDL, _TMR5HLD
        .equiv  TMR5HLDH, _TMR5HLD+1

        .extern TMR5
        .equiv  TMR5L, _TMR5
        .equiv  TMR5H, _TMR5+1

        .extern PR4
        .equiv  PR4L, _PR4
        .equiv  PR4H, _PR4+1

        .extern PR5
        .equiv  PR5L, _PR5
        .equiv  PR5H, _PR5+1

        .extern T4CON
        .equiv  T4CONL, _T4CON
        .equiv  T4CONH, _T4CON+1

        .extern T5CON
        .equiv  T5CONL, _T5CON
        .equiv  T5CONH, _T5CON+1

        .extern IC1CON1
        .equiv  IC1CON1L, _IC1CON1
        .equiv  IC1CON1H, _IC1CON1+1

        .extern IC1CON2
        .equiv  IC1CON2L, _IC1CON2
        .equiv  IC1CON2H, _IC1CON2+1

        .extern IC1BUF
        .equiv  IC1BUFL, _IC1BUF
        .equiv  IC1BUFH, _IC1BUF+1

        .extern IC1TMR
        .equiv  IC1TMRL, _IC1TMR
        .equiv  IC1TMRH, _IC1TMR+1

        .extern IC2CON1
        .equiv  IC2CON1L, _IC2CON1
        .equiv  IC2CON1H, _IC2CON1+1

        .extern IC2CON2
        .equiv  IC2CON2L, _IC2CON2
        .equiv  IC2CON2H, _IC2CON2+1

        .extern IC2BUF
        .equiv  IC2BUFL, _IC2BUF
        .equiv  IC2BUFH, _IC2BUF+1

        .extern IC2TMR
        .equiv  IC2TMRL, _IC2TMR
        .equiv  IC2TMRH, _IC2TMR+1

        .extern IC3CON1
        .equiv  IC3CON1L, _IC3CON1
        .equiv  IC3CON1H, _IC3CON1+1

        .extern IC3CON2
        .equiv  IC3CON2L, _IC3CON2
        .equiv  IC3CON2H, _IC3CON2+1

        .extern IC3BUF
        .equiv  IC3BUFL, _IC3BUF
        .equiv  IC3BUFH, _IC3BUF+1

        .extern IC3TMR
        .equiv  IC3TMRL, _IC3TMR
        .equiv  IC3TMRH, _IC3TMR+1

        .extern IC4CON1
        .equiv  IC4CON1L, _IC4CON1
        .equiv  IC4CON1H, _IC4CON1+1

        .extern IC4CON2
        .equiv  IC4CON2L, _IC4CON2
        .equiv  IC4CON2H, _IC4CON2+1

        .extern IC4BUF
        .equiv  IC4BUFL, _IC4BUF
        .equiv  IC4BUFH, _IC4BUF+1

        .extern IC4TMR
        .equiv  IC4TMRL, _IC4TMR
        .equiv  IC4TMRH, _IC4TMR+1

        .extern IC5CON1
        .equiv  IC5CON1L, _IC5CON1
        .equiv  IC5CON1H, _IC5CON1+1

        .extern IC5CON2
        .equiv  IC5CON2L, _IC5CON2
        .equiv  IC5CON2H, _IC5CON2+1

        .extern IC5BUF
        .equiv  IC5BUFL, _IC5BUF
        .equiv  IC5BUFH, _IC5BUF+1

        .extern IC5TMR
        .equiv  IC5TMRL, _IC5TMR
        .equiv  IC5TMRH, _IC5TMR+1

        .extern IC6CON1
        .equiv  IC6CON1L, _IC6CON1
        .equiv  IC6CON1H, _IC6CON1+1

        .extern IC6CON2
        .equiv  IC6CON2L, _IC6CON2
        .equiv  IC6CON2H, _IC6CON2+1

        .extern IC6BUF
        .equiv  IC6BUFL, _IC6BUF
        .equiv  IC6BUFH, _IC6BUF+1

        .extern IC6TMR
        .equiv  IC6TMRL, _IC6TMR
        .equiv  IC6TMRH, _IC6TMR+1

        .extern IC7CON1
        .equiv  IC7CON1L, _IC7CON1
        .equiv  IC7CON1H, _IC7CON1+1

        .extern IC7CON2
        .equiv  IC7CON2L, _IC7CON2
        .equiv  IC7CON2H, _IC7CON2+1

        .extern IC7BUF
        .equiv  IC7BUFL, _IC7BUF
        .equiv  IC7BUFH, _IC7BUF+1

        .extern IC7TMR
        .equiv  IC7TMRL, _IC7TMR
        .equiv  IC7TMRH, _IC7TMR+1

        .extern IC8CON1
        .equiv  IC8CON1L, _IC8CON1
        .equiv  IC8CON1H, _IC8CON1+1

        .extern IC8CON2
        .equiv  IC8CON2L, _IC8CON2
        .equiv  IC8CON2H, _IC8CON2+1

        .extern IC8BUF
        .equiv  IC8BUFL, _IC8BUF
        .equiv  IC8BUFH, _IC8BUF+1

        .extern IC8TMR
        .equiv  IC8TMRL, _IC8TMR
        .equiv  IC8TMRH, _IC8TMR+1

        .extern IC9CON1
        .equiv  IC9CON1L, _IC9CON1
        .equiv  IC9CON1H, _IC9CON1+1

        .extern IC9CON2
        .equiv  IC9CON2L, _IC9CON2
        .equiv  IC9CON2H, _IC9CON2+1

        .extern IC9BUF
        .equiv  IC9BUFL, _IC9BUF
        .equiv  IC9BUFH, _IC9BUF+1

        .extern IC9TMR
        .equiv  IC9TMRL, _IC9TMR
        .equiv  IC9TMRH, _IC9TMR+1

        .extern OC1CON1
        .equiv  OC1CON1L, _OC1CON1
        .equiv  OC1CON1H, _OC1CON1+1

        .extern OC1CON2
        .equiv  OC1CON2L, _OC1CON2
        .equiv  OC1CON2H, _OC1CON2+1

        .extern OC1RS
        .equiv  OC1RSL, _OC1RS
        .equiv  OC1RSH, _OC1RS+1

        .extern OC1R
        .equiv  OC1RL, _OC1R
        .equiv  OC1RH, _OC1R+1

        .extern OC1TMR
        .equiv  OC1TMRL, _OC1TMR
        .equiv  OC1TMRH, _OC1TMR+1

        .extern OC2CON1
        .equiv  OC2CON1L, _OC2CON1
        .equiv  OC2CON1H, _OC2CON1+1

        .extern OC2CON2
        .equiv  OC2CON2L, _OC2CON2
        .equiv  OC2CON2H, _OC2CON2+1

        .extern OC2RS
        .equiv  OC2RSL, _OC2RS
        .equiv  OC2RSH, _OC2RS+1

        .extern OC2R
        .equiv  OC2RL, _OC2R
        .equiv  OC2RH, _OC2R+1

        .extern OC2TMR
        .equiv  OC2TMRL, _OC2TMR
        .equiv  OC2TMRH, _OC2TMR+1

        .extern OC3CON1
        .equiv  OC3CON1L, _OC3CON1
        .equiv  OC3CON1H, _OC3CON1+1

        .extern OC3CON2
        .equiv  OC3CON2L, _OC3CON2
        .equiv  OC3CON2H, _OC3CON2+1

        .extern OC3RS
        .equiv  OC3RSL, _OC3RS
        .equiv  OC3RSH, _OC3RS+1

        .extern OC3R
        .equiv  OC3RL, _OC3R
        .equiv  OC3RH, _OC3R+1

        .extern OC3TMR
        .equiv  OC3TMRL, _OC3TMR
        .equiv  OC3TMRH, _OC3TMR+1

        .extern OC4CON1
        .equiv  OC4CON1L, _OC4CON1
        .equiv  OC4CON1H, _OC4CON1+1

        .extern OC4CON2
        .equiv  OC4CON2L, _OC4CON2
        .equiv  OC4CON2H, _OC4CON2+1

        .extern OC4RS
        .equiv  OC4RSL, _OC4RS
        .equiv  OC4RSH, _OC4RS+1

        .extern OC4R
        .equiv  OC4RL, _OC4R
        .equiv  OC4RH, _OC4R+1

        .extern OC4TMR
        .equiv  OC4TMRL, _OC4TMR
        .equiv  OC4TMRH, _OC4TMR+1

        .extern OC5CON1
        .equiv  OC5CON1L, _OC5CON1
        .equiv  OC5CON1H, _OC5CON1+1

        .extern OC5CON2
        .equiv  OC5CON2L, _OC5CON2
        .equiv  OC5CON2H, _OC5CON2+1

        .extern OC5RS
        .equiv  OC5RSL, _OC5RS
        .equiv  OC5RSH, _OC5RS+1

        .extern OC5R
        .equiv  OC5RL, _OC5R
        .equiv  OC5RH, _OC5R+1

        .extern OC5TMR
        .equiv  OC5TMRL, _OC5TMR
        .equiv  OC5TMRH, _OC5TMR+1

        .extern OC6CON1
        .equiv  OC6CON1L, _OC6CON1
        .equiv  OC6CON1H, _OC6CON1+1

        .extern OC6CON2
        .equiv  OC6CON2L, _OC6CON2
        .equiv  OC6CON2H, _OC6CON2+1

        .extern OC6RS
        .equiv  OC6RSL, _OC6RS
        .equiv  OC6RSH, _OC6RS+1

        .extern OC6R
        .equiv  OC6RL, _OC6R
        .equiv  OC6RH, _OC6R+1

        .extern OC6TMR
        .equiv  OC6TMRL, _OC6TMR
        .equiv  OC6TMRH, _OC6TMR+1

        .extern OC7CON1
        .equiv  OC7CON1L, _OC7CON1
        .equiv  OC7CON1H, _OC7CON1+1

        .extern OC7CON2
        .equiv  OC7CON2L, _OC7CON2
        .equiv  OC7CON2H, _OC7CON2+1

        .extern OC7RS
        .equiv  OC7RSL, _OC7RS
        .equiv  OC7RSH, _OC7RS+1

        .extern OC7R
        .equiv  OC7RL, _OC7R
        .equiv  OC7RH, _OC7R+1

        .extern OC7TMR
        .equiv  OC7TMRL, _OC7TMR
        .equiv  OC7TMRH, _OC7TMR+1

        .extern OC8CON1
        .equiv  OC8CON1L, _OC8CON1
        .equiv  OC8CON1H, _OC8CON1+1

        .extern OC8CON2
        .equiv  OC8CON2L, _OC8CON2
        .equiv  OC8CON2H, _OC8CON2+1

        .extern OC8RS
        .equiv  OC8RSL, _OC8RS
        .equiv  OC8RSH, _OC8RS+1

        .extern OC8R
        .equiv  OC8RL, _OC8R
        .equiv  OC8RH, _OC8R+1

        .extern OC8TMR
        .equiv  OC8TMRL, _OC8TMR
        .equiv  OC8TMRH, _OC8TMR+1

        .extern OC9CON1
        .equiv  OC9CON1L, _OC9CON1
        .equiv  OC9CON1H, _OC9CON1+1

        .extern OC9CON2
        .equiv  OC9CON2L, _OC9CON2
        .equiv  OC9CON2H, _OC9CON2+1

        .extern OC9RS
        .equiv  OC9RSL, _OC9RS
        .equiv  OC9RSH, _OC9RS+1

        .extern OC9R
        .equiv  OC9RL, _OC9R
        .equiv  OC9RH, _OC9R+1

        .extern OC9TMR
        .equiv  OC9TMRL, _OC9TMR
        .equiv  OC9TMRH, _OC9TMR+1

        .extern I2C1RCV
        .equiv  I2C1RCVL, _I2C1RCV
        .equiv  I2C1RCVH, _I2C1RCV+1

        .extern I2C1TRN
        .equiv  I2C1TRNL, _I2C1TRN
        .equiv  I2C1TRNH, _I2C1TRN+1

        .extern I2C1BRG
        .equiv  I2C1BRGL, _I2C1BRG
        .equiv  I2C1BRGH, _I2C1BRG+1

        .extern I2C1CON
        .equiv  I2C1CONL, _I2C1CON
        .equiv  I2C1CONH, _I2C1CON+1

        .extern I2C1STAT
        .equiv  I2C1STATL, _I2C1STAT
        .equiv  I2C1STATH, _I2C1STAT+1

        .extern I2C1ADD
        .equiv  I2C1ADDL, _I2C1ADD
        .equiv  I2C1ADDH, _I2C1ADD+1

        .extern I2C1MSK
        .equiv  I2C1MSKL, _I2C1MSK
        .equiv  I2C1MSKH, _I2C1MSK+1

        .extern I2C2RCV
        .equiv  I2C2RCVL, _I2C2RCV
        .equiv  I2C2RCVH, _I2C2RCV+1

        .extern I2C2TRN
        .equiv  I2C2TRNL, _I2C2TRN
        .equiv  I2C2TRNH, _I2C2TRN+1

        .extern I2C2BRG
        .equiv  I2C2BRGL, _I2C2BRG
        .equiv  I2C2BRGH, _I2C2BRG+1

        .extern I2C2CON
        .equiv  I2C2CONL, _I2C2CON
        .equiv  I2C2CONH, _I2C2CON+1

        .extern I2C2STAT
        .equiv  I2C2STATL, _I2C2STAT
        .equiv  I2C2STATH, _I2C2STAT+1

        .extern I2C2ADD
        .equiv  I2C2ADDL, _I2C2ADD
        .equiv  I2C2ADDH, _I2C2ADD+1

        .extern I2C2MSK
        .equiv  I2C2MSKL, _I2C2MSK
        .equiv  I2C2MSKH, _I2C2MSK+1

        .extern U1MODE
        .equiv  U1MODEL, _U1MODE
        .equiv  U1MODEH, _U1MODE+1

        .extern U1STA
        .equiv  U1STAL, _U1STA
        .equiv  U1STAH, _U1STA+1

        .extern U1TXREG
        .equiv  U1TXREGL, _U1TXREG
        .equiv  U1TXREGH, _U1TXREG+1

        .extern U1RXREG
        .equiv  U1RXREGL, _U1RXREG
        .equiv  U1RXREGH, _U1RXREG+1

        .extern U1BRG
        .equiv  U1BRGL, _U1BRG
        .equiv  U1BRGH, _U1BRG+1

        .extern U2MODE
        .equiv  U2MODEL, _U2MODE
        .equiv  U2MODEH, _U2MODE+1

        .extern U2STA
        .equiv  U2STAL, _U2STA
        .equiv  U2STAH, _U2STA+1

        .extern U2TXREG
        .equiv  U2TXREGL, _U2TXREG
        .equiv  U2TXREGH, _U2TXREG+1

        .extern U2RXREG
        .equiv  U2RXREGL, _U2RXREG
        .equiv  U2RXREGH, _U2RXREG+1

        .extern U2BRG
        .equiv  U2BRGL, _U2BRG
        .equiv  U2BRGH, _U2BRG+1

        .extern SPI1STAT
        .equiv  SPI1STATL, _SPI1STAT
        .equiv  SPI1STATH, _SPI1STAT+1

        .extern SPI1CON1
        .equiv  SPI1CON1L, _SPI1CON1
        .equiv  SPI1CON1H, _SPI1CON1+1

        .extern SPI1CON2
        .equiv  SPI1CON2L, _SPI1CON2
        .equiv  SPI1CON2H, _SPI1CON2+1

        .extern SPI1BUF
        .equiv  SPI1BUFL, _SPI1BUF
        .equiv  SPI1BUFH, _SPI1BUF+1

        .extern U3MODE
        .equiv  U3MODEL, _U3MODE
        .equiv  U3MODEH, _U3MODE+1

        .extern U3STA
        .equiv  U3STAL, _U3STA
        .equiv  U3STAH, _U3STA+1

        .extern U3TXREG
        .equiv  U3TXREGL, _U3TXREG
        .equiv  U3TXREGH, _U3TXREG+1

        .extern U3RXREG
        .equiv  U3RXREGL, _U3RXREG
        .equiv  U3RXREGH, _U3RXREG+1

        .extern U3BRG
        .equiv  U3BRGL, _U3BRG
        .equiv  U3BRGH, _U3BRG+1

        .extern SPI2STAT
        .equiv  SPI2STATL, _SPI2STAT
        .equiv  SPI2STATH, _SPI2STAT+1

        .extern SPI2CON1
        .equiv  SPI2CON1L, _SPI2CON1
        .equiv  SPI2CON1H, _SPI2CON1+1

        .extern SPI2CON2
        .equiv  SPI2CON2L, _SPI2CON2
        .equiv  SPI2CON2H, _SPI2CON2+1

        .extern SPI2BUF
        .equiv  SPI2BUFL, _SPI2BUF
        .equiv  SPI2BUFH, _SPI2BUF+1

        .extern I2C3RCV
        .equiv  I2C3RCVL, _I2C3RCV
        .equiv  I2C3RCVH, _I2C3RCV+1

        .extern I2C3TRN
        .equiv  I2C3TRNL, _I2C3TRN
        .equiv  I2C3TRNH, _I2C3TRN+1

        .extern I2C3BRG
        .equiv  I2C3BRGL, _I2C3BRG
        .equiv  I2C3BRGH, _I2C3BRG+1

        .extern I2C3CON
        .equiv  I2C3CONL, _I2C3CON
        .equiv  I2C3CONH, _I2C3CON+1

        .extern I2C3STAT
        .equiv  I2C3STATL, _I2C3STAT
        .equiv  I2C3STATH, _I2C3STAT+1

        .extern I2C3ADD
        .equiv  I2C3ADDL, _I2C3ADD
        .equiv  I2C3ADDH, _I2C3ADD+1

        .extern I2C3MSK
        .equiv  I2C3MSKL, _I2C3MSK
        .equiv  I2C3MSKH, _I2C3MSK+1

        .extern SPI3STAT
        .equiv  SPI3STATL, _SPI3STAT
        .equiv  SPI3STATH, _SPI3STAT+1

        .extern SPI3CON1
        .equiv  SPI3CON1L, _SPI3CON1
        .equiv  SPI3CON1H, _SPI3CON1+1

        .extern SPI3CON2
        .equiv  SPI3CON2L, _SPI3CON2
        .equiv  SPI3CON2H, _SPI3CON2+1

        .extern SPI3BUF
        .equiv  SPI3BUFL, _SPI3BUF
        .equiv  SPI3BUFH, _SPI3BUF+1

        .extern U4MODE
        .equiv  U4MODEL, _U4MODE
        .equiv  U4MODEH, _U4MODE+1

        .extern U4STA
        .equiv  U4STAL, _U4STA
        .equiv  U4STAH, _U4STA+1

        .extern U4TXREG
        .equiv  U4TXREGL, _U4TXREG
        .equiv  U4TXREGH, _U4TXREG+1

        .extern U4RXREG
        .equiv  U4RXREGL, _U4RXREG
        .equiv  U4RXREGH, _U4RXREG+1

        .extern U4BRG
        .equiv  U4BRGL, _U4BRG
        .equiv  U4BRGH, _U4BRG+1

        .extern TRISB
        .equiv  TRISBL, _TRISB
        .equiv  TRISBH, _TRISB+1

        .extern PORTB
        .equiv  PORTBL, _PORTB
        .equiv  PORTBH, _PORTB+1

        .extern LATB
        .equiv  LATBL, _LATB
        .equiv  LATBH, _LATB+1

        .extern ODCB
        .equiv  ODCBL, _ODCB
        .equiv  ODCBH, _ODCB+1

        .extern TRISC
        .equiv  TRISCL, _TRISC
        .equiv  TRISCH, _TRISC+1

        .extern PORTC
        .equiv  PORTCL, _PORTC
        .equiv  PORTCH, _PORTC+1

        .extern LATC
        .equiv  LATCL, _LATC
        .equiv  LATCH, _LATC+1

        .extern ODCC
        .equiv  ODCCL, _ODCC
        .equiv  ODCCH, _ODCC+1

        .extern TRISD
        .equiv  TRISDL, _TRISD
        .equiv  TRISDH, _TRISD+1

        .extern PORTD
        .equiv  PORTDL, _PORTD
        .equiv  PORTDH, _PORTD+1

        .extern LATD
        .equiv  LATDL, _LATD
        .equiv  LATDH, _LATD+1

        .extern ODCD
        .equiv  ODCDL, _ODCD
        .equiv  ODCDH, _ODCD+1

        .extern TRISE
        .equiv  TRISEL, _TRISE
        .equiv  TRISEH, _TRISE+1

        .extern PORTE
        .equiv  PORTEL, _PORTE
        .equiv  PORTEH, _PORTE+1

        .extern LATE
        .equiv  LATEL, _LATE
        .equiv  LATEH, _LATE+1

        .extern ODCE
        .equiv  ODCEL, _ODCE
        .equiv  ODCEH, _ODCE+1

        .extern TRISF
        .equiv  TRISFL, _TRISF
        .equiv  TRISFH, _TRISF+1

        .extern PORTF
        .equiv  PORTFL, _PORTF
        .equiv  PORTFH, _PORTF+1

        .extern LATF
        .equiv  LATFL, _LATF
        .equiv  LATFH, _LATF+1

        .extern ODCF
        .equiv  ODCFL, _ODCF
        .equiv  ODCFH, _ODCF+1

        .extern TRISG
        .equiv  TRISGL, _TRISG
        .equiv  TRISGH, _TRISG+1

        .extern PORTG
        .equiv  PORTGL, _PORTG
        .equiv  PORTGH, _PORTG+1

        .extern LATG
        .equiv  LATGL, _LATG
        .equiv  LATGH, _LATG+1

        .extern ODCG
        .equiv  ODCGL, _ODCG
        .equiv  ODCGH, _ODCG+1

        .extern PADCFG1
        .equiv  PADCFG1L, _PADCFG1
        .equiv  PADCFG1H, _PADCFG1+1

        .extern ADC1BUF0
        .equiv  ADC1BUF0L, _ADC1BUF0
        .equiv  ADC1BUF0H, _ADC1BUF0+1

        .extern ADC1BUF1
        .equiv  ADC1BUF1L, _ADC1BUF1
        .equiv  ADC1BUF1H, _ADC1BUF1+1

        .extern ADC1BUF2
        .equiv  ADC1BUF2L, _ADC1BUF2
        .equiv  ADC1BUF2H, _ADC1BUF2+1

        .extern ADC1BUF3
        .equiv  ADC1BUF3L, _ADC1BUF3
        .equiv  ADC1BUF3H, _ADC1BUF3+1

        .extern ADC1BUF4
        .equiv  ADC1BUF4L, _ADC1BUF4
        .equiv  ADC1BUF4H, _ADC1BUF4+1

        .extern ADC1BUF5
        .equiv  ADC1BUF5L, _ADC1BUF5
        .equiv  ADC1BUF5H, _ADC1BUF5+1

        .extern ADC1BUF6
        .equiv  ADC1BUF6L, _ADC1BUF6
        .equiv  ADC1BUF6H, _ADC1BUF6+1

        .extern ADC1BUF7
        .equiv  ADC1BUF7L, _ADC1BUF7
        .equiv  ADC1BUF7H, _ADC1BUF7+1

        .extern ADC1BUF8
        .equiv  ADC1BUF8L, _ADC1BUF8
        .equiv  ADC1BUF8H, _ADC1BUF8+1

        .extern ADC1BUF9
        .equiv  ADC1BUF9L, _ADC1BUF9
        .equiv  ADC1BUF9H, _ADC1BUF9+1

        .extern ADC1BUFA
        .equiv  ADC1BUFAL, _ADC1BUFA
        .equiv  ADC1BUFAH, _ADC1BUFA+1

        .extern ADC1BUFB
        .equiv  ADC1BUFBL, _ADC1BUFB
        .equiv  ADC1BUFBH, _ADC1BUFB+1

        .extern ADC1BUFC
        .equiv  ADC1BUFCL, _ADC1BUFC
        .equiv  ADC1BUFCH, _ADC1BUFC+1

        .extern ADC1BUFD
        .equiv  ADC1BUFDL, _ADC1BUFD
        .equiv  ADC1BUFDH, _ADC1BUFD+1

        .extern ADC1BUFE
        .equiv  ADC1BUFEL, _ADC1BUFE
        .equiv  ADC1BUFEH, _ADC1BUFE+1

        .extern ADC1BUFF
        .equiv  ADC1BUFFL, _ADC1BUFF
        .equiv  ADC1BUFFH, _ADC1BUFF+1

        .extern AD1CON1
        .equiv  AD1CON1L, _AD1CON1
        .equiv  AD1CON1H, _AD1CON1+1

        .extern AD1CON2
        .equiv  AD1CON2L, _AD1CON2
        .equiv  AD1CON2H, _AD1CON2+1

        .extern AD1CON3
        .equiv  AD1CON3L, _AD1CON3
        .equiv  AD1CON3H, _AD1CON3+1

        .extern AD1CHS
        .equiv  AD1CHSL, _AD1CHS
        .equiv  AD1CHSH, _AD1CHS+1

        .extern AD1PCFGH
        .equiv  AD1PCFGHL, _AD1PCFGH
        .equiv  AD1PCFGHH, _AD1PCFGH+1

        .extern AD1PCFGL
        .equiv  AD1PCFGLL, _AD1PCFGL
        .equiv  AD1PCFGLH, _AD1PCFGL+1

        .extern AD1CSSL
        .equiv  AD1CSSLL, _AD1CSSL
        .equiv  AD1CSSLH, _AD1CSSL+1

        .extern AD1CSSH
        .equiv  AD1CSSHL, _AD1CSSH
        .equiv  AD1CSSHH, _AD1CSSH+1

        .extern CTMUCON
        .equiv  CTMUCONL, _CTMUCON
        .equiv  CTMUCONH, _CTMUCON+1

        .extern CTMUICON
        .equiv  CTMUICONL, _CTMUICON
        .equiv  CTMUICONH, _CTMUICON+1

        .extern U1OTGIR
        .equiv  U1OTGIRL, _U1OTGIR
        .equiv  U1OTGIRH, _U1OTGIR+1

        .extern U1OTGIE
        .equiv  U1OTGIEL, _U1OTGIE
        .equiv  U1OTGIEH, _U1OTGIE+1

        .extern U1OTGSTAT
        .equiv  U1OTGSTATL, _U1OTGSTAT
        .equiv  U1OTGSTATH, _U1OTGSTAT+1

        .extern U1OTGCON
        .equiv  U1OTGCONL, _U1OTGCON
        .equiv  U1OTGCONH, _U1OTGCON+1

        .extern U1PWRC
        .equiv  U1PWRCL, _U1PWRC
        .equiv  U1PWRCH, _U1PWRC+1

        .extern U1IR
        .equiv  U1IRL, _U1IR
        .equiv  U1IRH, _U1IR+1

        .extern U1IE
        .equiv  U1IEL, _U1IE
        .equiv  U1IEH, _U1IE+1

        .extern U1EIR
        .equiv  U1EIRL, _U1EIR
        .equiv  U1EIRH, _U1EIR+1

        .extern U1EIE
        .equiv  U1EIEL, _U1EIE
        .equiv  U1EIEH, _U1EIE+1

        .extern U1STAT
        .equiv  U1STATL, _U1STAT
        .equiv  U1STATH, _U1STAT+1

        .extern U1CON
        .equiv  U1CONL, _U1CON
        .equiv  U1CONH, _U1CON+1

        .extern U1ADDR
        .equiv  U1ADDRL, _U1ADDR
        .equiv  U1ADDRH, _U1ADDR+1

        .extern U1BDTP1
        .equiv  U1BDTP1L, _U1BDTP1
        .equiv  U1BDTP1H, _U1BDTP1+1

        .extern U1FRML
        .equiv  U1FRMLL, _U1FRML
        .equiv  U1FRMLH, _U1FRML+1

        .extern U1FRMH
        .equiv  U1FRMHL, _U1FRMH
        .equiv  U1FRMHH, _U1FRMH+1

        .extern U1TOK
        .equiv  U1TOKL, _U1TOK
        .equiv  U1TOKH, _U1TOK+1

        .extern U1SOF
        .equiv  U1SOFL, _U1SOF
        .equiv  U1SOFH, _U1SOF+1

        .extern U1BDTP2
        .equiv  U1BDTP2L, _U1BDTP2
        .equiv  U1BDTP2H, _U1BDTP2+1

        .extern U1BDTP3
        .equiv  U1BDTP3L, _U1BDTP3
        .equiv  U1BDTP3H, _U1BDTP3+1

        .extern U1CNFG1
        .equiv  U1CNFG1L, _U1CNFG1
        .equiv  U1CNFG1H, _U1CNFG1+1

        .extern U1CNFG2
        .equiv  U1CNFG2L, _U1CNFG2
        .equiv  U1CNFG2H, _U1CNFG2+1

        .extern U1EP0
        .equiv  U1EP0L, _U1EP0
        .equiv  U1EP0H, _U1EP0+1

        .extern U1EP1
        .equiv  U1EP1L, _U1EP1
        .equiv  U1EP1H, _U1EP1+1

        .extern U1EP2
        .equiv  U1EP2L, _U1EP2
        .equiv  U1EP2H, _U1EP2+1

        .extern U1EP3
        .equiv  U1EP3L, _U1EP3
        .equiv  U1EP3H, _U1EP3+1

        .extern U1EP4
        .equiv  U1EP4L, _U1EP4
        .equiv  U1EP4H, _U1EP4+1

        .extern U1EP5
        .equiv  U1EP5L, _U1EP5
        .equiv  U1EP5H, _U1EP5+1

        .extern U1EP6
        .equiv  U1EP6L, _U1EP6
        .equiv  U1EP6H, _U1EP6+1

        .extern U1EP7
        .equiv  U1EP7L, _U1EP7
        .equiv  U1EP7H, _U1EP7+1

        .extern U1EP8
        .equiv  U1EP8L, _U1EP8
        .equiv  U1EP8H, _U1EP8+1

        .extern U1EP9
        .equiv  U1EP9L, _U1EP9
        .equiv  U1EP9H, _U1EP9+1

        .extern U1EP10
        .equiv  U1EP10L, _U1EP10
        .equiv  U1EP10H, _U1EP10+1

        .extern U1EP11
        .equiv  U1EP11L, _U1EP11
        .equiv  U1EP11H, _U1EP11+1

        .extern U1EP12
        .equiv  U1EP12L, _U1EP12
        .equiv  U1EP12H, _U1EP12+1

        .extern U1EP13
        .equiv  U1EP13L, _U1EP13
        .equiv  U1EP13H, _U1EP13+1

        .extern U1EP14
        .equiv  U1EP14L, _U1EP14
        .equiv  U1EP14H, _U1EP14+1

        .extern U1EP15
        .equiv  U1EP15L, _U1EP15
        .equiv  U1EP15H, _U1EP15+1

        .extern U1PWMRRS
        .equiv  U1PWMRRSL, _U1PWMRRS
        .equiv  U1PWMRRSH, _U1PWMRRS+1

        .extern U1PWMCON
        .equiv  U1PWMCONL, _U1PWMCON
        .equiv  U1PWMCONH, _U1PWMCON+1

        .extern PMCON
        .equiv  PMCONL, _PMCON
        .equiv  PMCONH, _PMCON+1

        .extern PMMODE
        .equiv  PMMODEL, _PMMODE
        .equiv  PMMODEH, _PMMODE+1

        .extern PMADDR
        .equiv  PMADDRL, _PMADDR
        .equiv  PMADDRH, _PMADDR+1

        .extern PMDOUT1
        .equiv  PMDOUT1L, _PMDOUT1
        .equiv  PMDOUT1H, _PMDOUT1+1

        .extern PMDOUT2
        .equiv  PMDOUT2L, _PMDOUT2
        .equiv  PMDOUT2H, _PMDOUT2+1

        .extern PMDIN1
        .equiv  PMDIN1L, _PMDIN1
        .equiv  PMDIN1H, _PMDIN1+1

        .extern PMDIN2
        .equiv  PMDIN2L, _PMDIN2
        .equiv  PMDIN2H, _PMDIN2+1

        .extern PMAEN
        .equiv  PMAENL, _PMAEN
        .equiv  PMAENH, _PMAEN+1

        .extern PMSTAT
        .equiv  PMSTATL, _PMSTAT
        .equiv  PMSTATH, _PMSTAT+1

        .extern ALRMVAL
        .equiv  ALRMVALL, _ALRMVAL
        .equiv  ALRMVALH, _ALRMVAL+1

        .extern ALCFGRPT
        .equiv  ALCFGRPTL, _ALCFGRPT
        .equiv  ALCFGRPTH, _ALCFGRPT+1

        .extern RTCVAL
        .equiv  RTCVALL, _RTCVAL
        .equiv  RTCVALH, _RTCVAL+1

        .extern RCFGCAL
        .equiv  RCFGCALL, _RCFGCAL
        .equiv  RCFGCALH, _RCFGCAL+1

        .extern CMSTAT
        .equiv  CMSTATL, _CMSTAT
        .equiv  CMSTATH, _CMSTAT+1

        .extern CVRCON
        .equiv  CVRCONL, _CVRCON
        .equiv  CVRCONH, _CVRCON+1

        .extern CM1CON
        .equiv  CM1CONL, _CM1CON
        .equiv  CM1CONH, _CM1CON+1

        .extern CM2CON
        .equiv  CM2CONL, _CM2CON
        .equiv  CM2CONH, _CM2CON+1

        .extern CM3CON
        .equiv  CM3CONL, _CM3CON
        .equiv  CM3CONH, _CM3CON+1

        .extern CRCCON
        .equiv  CRCCONL, _CRCCON
        .equiv  CRCCONH, _CRCCON+1

        .extern CRCXOR
        .equiv  CRCXORL, _CRCXOR
        .equiv  CRCXORH, _CRCXOR+1

        .extern CRCDAT
        .equiv  CRCDATL, _CRCDAT
        .equiv  CRCDATH, _CRCDAT+1

        .extern CRCWDAT
        .equiv  CRCWDATL, _CRCWDAT
        .equiv  CRCWDATH, _CRCWDAT+1

        .extern RPINR0
        .equiv  RPINR0L, _RPINR0
        .equiv  RPINR0H, _RPINR0+1

        .extern RPINR1
        .equiv  RPINR1L, _RPINR1
        .equiv  RPINR1H, _RPINR1+1

        .extern RPINR2
        .equiv  RPINR2L, _RPINR2
        .equiv  RPINR2H, _RPINR2+1

        .extern RPINR3
        .equiv  RPINR3L, _RPINR3
        .equiv  RPINR3H, _RPINR3+1

        .extern RPINR4
        .equiv  RPINR4L, _RPINR4
        .equiv  RPINR4H, _RPINR4+1

        .extern RPINR7
        .equiv  RPINR7L, _RPINR7
        .equiv  RPINR7H, _RPINR7+1

        .extern RPINR8
        .equiv  RPINR8L, _RPINR8
        .equiv  RPINR8H, _RPINR8+1

        .extern RPINR9
        .equiv  RPINR9L, _RPINR9
        .equiv  RPINR9H, _RPINR9+1

        .extern RPINR10
        .equiv  RPINR10L, _RPINR10
        .equiv  RPINR10H, _RPINR10+1

        .extern RPINR11
        .equiv  RPINR11L, _RPINR11
        .equiv  RPINR11H, _RPINR11+1

        .extern RPINR15
        .equiv  RPINR15L, _RPINR15
        .equiv  RPINR15H, _RPINR15+1

        .extern RPINR17
        .equiv  RPINR17L, _RPINR17
        .equiv  RPINR17H, _RPINR17+1

        .extern RPINR18
        .equiv  RPINR18L, _RPINR18
        .equiv  RPINR18H, _RPINR18+1

        .extern RPINR19
        .equiv  RPINR19L, _RPINR19
        .equiv  RPINR19H, _RPINR19+1

        .extern RPINR20
        .equiv  RPINR20L, _RPINR20
        .equiv  RPINR20H, _RPINR20+1

        .extern RPINR21
        .equiv  RPINR21L, _RPINR21
        .equiv  RPINR21H, _RPINR21+1

        .extern RPINR22
        .equiv  RPINR22L, _RPINR22
        .equiv  RPINR22H, _RPINR22+1

        .extern RPINR23
        .equiv  RPINR23L, _RPINR23
        .equiv  RPINR23H, _RPINR23+1

        .extern RPINR27
        .equiv  RPINR27L, _RPINR27
        .equiv  RPINR27H, _RPINR27+1

        .extern RPINR28
        .equiv  RPINR28L, _RPINR28
        .equiv  RPINR28H, _RPINR28+1

        .extern RPINR29
        .equiv  RPINR29L, _RPINR29
        .equiv  RPINR29H, _RPINR29+1

        .extern RPOR0
        .equiv  RPOR0L, _RPOR0
        .equiv  RPOR0H, _RPOR0+1

        .extern RPOR1
        .equiv  RPOR1L, _RPOR1
        .equiv  RPOR1H, _RPOR1+1

        .extern RPOR2
        .equiv  RPOR2L, _RPOR2
        .equiv  RPOR2H, _RPOR2+1

        .extern RPOR3
        .equiv  RPOR3L, _RPOR3
        .equiv  RPOR3H, _RPOR3+1

        .extern RPOR4
        .equiv  RPOR4L, _RPOR4
        .equiv  RPOR4H, _RPOR4+1

        .extern RPOR5
        .equiv  RPOR5L, _RPOR5
        .equiv  RPOR5H, _RPOR5+1

        .extern RPOR6
        .equiv  RPOR6L, _RPOR6
        .equiv  RPOR6H, _RPOR6+1

        .extern RPOR7
        .equiv  RPOR7L, _RPOR7
        .equiv  RPOR7H, _RPOR7+1

        .extern RPOR8
        .equiv  RPOR8L, _RPOR8
        .equiv  RPOR8H, _RPOR8+1

        .extern RPOR9
        .equiv  RPOR9L, _RPOR9
        .equiv  RPOR9H, _RPOR9+1

        .extern RPOR10
        .equiv  RPOR10L, _RPOR10
        .equiv  RPOR10H, _RPOR10+1

        .extern RPOR11
        .equiv  RPOR11L, _RPOR11
        .equiv  RPOR11H, _RPOR11+1

        .extern RPOR12
        .equiv  RPOR12L, _RPOR12
        .equiv  RPOR12H, _RPOR12+1

        .extern RPOR13
        .equiv  RPOR13L, _RPOR13
        .equiv  RPOR13H, _RPOR13+1

        .extern RPOR14
        .equiv  RPOR14L, _RPOR14
        .equiv  RPOR14H, _RPOR14+1

        .extern RPOR15
        .equiv  RPOR15L, _RPOR15
        .equiv  RPOR15H, _RPOR15+1

        .extern RCON
        .equiv  RCONL, _RCON
        .equiv  RCONH, _RCON+1

        .extern OSCCON
        .equiv  OSCCONL, _OSCCON
        .equiv  OSCCONH, _OSCCON+1

        .extern CLKDIV
        .equiv  CLKDIVL, _CLKDIV
        .equiv  CLKDIVH, _CLKDIV+1

        .extern OSCTUN
        .equiv  OSCTUNL, _OSCTUN
        .equiv  OSCTUNH, _OSCTUN+1

        .extern REFOCON
        .equiv  REFOCONL, _REFOCON
        .equiv  REFOCONH, _REFOCON+1

        .extern NVMCON
        .equiv  NVMCONL, _NVMCON
        .equiv  NVMCONH, _NVMCON+1

        .extern NVMKEY
        .equiv  NVMKEYL, _NVMKEY
        .equiv  NVMKEYH, _NVMKEY+1

        .extern PMD1
        .equiv  PMD1L, _PMD1
        .equiv  PMD1H, _PMD1+1

        .extern PMD2
        .equiv  PMD2L, _PMD2
        .equiv  PMD2H, _PMD2+1

        .extern PMD3
        .equiv  PMD3L, _PMD3
        .equiv  PMD3H, _PMD3+1

        .extern PMD4
        .equiv  PMD4L, _PMD4
        .equiv  PMD4H, _PMD4+1

        .extern PMD5
        .equiv  PMD5L, _PMD5
        .equiv  PMD5H, _PMD5+1

        .extern PMD6
        .equiv  PMD6L, _PMD6
        .equiv  PMD6H, _PMD6+1

;----- SR Bits -----------------------------------------------------
        .equiv C,       0x0000
        .equiv Z,       0x0001
        .equiv OV,      0x0002
        .equiv N,       0x0003
        .equiv RA,      0x0004
        .equiv IPL0,    0x0005
        .equiv IPL1,    0x0006
        .equiv IPL2,    0x0007
        .equiv DC,      0x0008



;----- CORCON Bits -----------------------------------------------------
        .equiv PSV,     0x0002
        .equiv IPL3,    0x0003


;----- CNPD1 Bits -----------------------------------------------------
        .equiv CN0PDE,  0x0000
        .equiv CN1PDE,  0x0001
        .equiv CN2PDE,  0x0002
        .equiv CN3PDE,  0x0003
        .equiv CN4PDE,  0x0004
        .equiv CN5PDE,  0x0005
        .equiv CN6PDE,  0x0006
        .equiv CN7PDE,  0x0007
        .equiv CN8PDE,  0x0008
        .equiv CN9PDE,  0x0009
        .equiv CN10PDE, 0x000A
        .equiv CN11PDE, 0x000B
        .equiv CN12PDE, 0x000C
        .equiv CN13PDE, 0x000D
        .equiv CN14PDE, 0x000E
        .equiv CN15PDE, 0x000F


;----- CNPD2 Bits -----------------------------------------------------
        .equiv CN16PDE, 0x0000
        .equiv CN17PDE, 0x0001
        .equiv CN18PDE, 0x0002
        .equiv CN22PDE, 0x0006
        .equiv CN23PDE, 0x0007
        .equiv CN24PDE, 0x0008
        .equiv CN25PDE, 0x0009
        .equiv CN26PDE, 0x000A
        .equiv CN27PDE, 0x000B
        .equiv CN28PDE, 0x000C
        .equiv CN29PDE, 0x000D
        .equiv CN30PDE, 0x000E
        .equiv CN31PDE, 0x000F


;----- CNPD3 Bits -----------------------------------------------------
        .equiv CN32PDE, 0x0000


;----- CNPD4 Bits -----------------------------------------------------
        .equiv CN49PDE, 0x0001
        .equiv CN50PDE, 0x0002
        .equiv CN51PDE, 0x0003
        .equiv CN52PDE, 0x0004
        .equiv CN53PDE, 0x0005
        .equiv CN54PDE, 0x0006
        .equiv CN55PDE, 0x0007
        .equiv CN56PDE, 0x0008
        .equiv CN58PDE, 0x000A
        .equiv CN59PDE, 0x000B
        .equiv CN60PDE, 0x000C
        .equiv CN61PDE, 0x000D
        .equiv CN62PDE, 0x000E
        .equiv CN63PDE, 0x000F


;----- CNPD5 Bits -----------------------------------------------------
        .equiv CN64PDE, 0x0000
        .equiv CN65PDE, 0x0001
        .equiv CN68PDE, 0x0004
        .equiv CN69PDE, 0x0005
        .equiv CN71PDE, 0x0007


;----- CNEN1 Bits -----------------------------------------------------
        .equiv CN0IE,   0x0000
        .equiv CN1IE,   0x0001
        .equiv CN2IE,   0x0002
        .equiv CN3IE,   0x0003
        .equiv CN4IE,   0x0004
        .equiv CN5IE,   0x0005
        .equiv CN6IE,   0x0006
        .equiv CN7IE,   0x0007
        .equiv CN8IE,   0x0008
        .equiv CN9IE,   0x0009
        .equiv CN10IE,  0x000A
        .equiv CN11IE,  0x000B
        .equiv CN12IE,  0x000C
        .equiv CN13IE,  0x000D
        .equiv CN14IE,  0x000E
        .equiv CN15IE,  0x000F


;----- CNEN2 Bits -----------------------------------------------------
        .equiv CN16IE,  0x0000
        .equiv CN17IE,  0x0001
        .equiv CN18IE,  0x0002
        .equiv CN22IE,  0x0006
        .equiv CN23IE,  0x0007
        .equiv CN24IE,  0x0008
        .equiv CN25IE,  0x0009
        .equiv CN26IE,  0x000A
        .equiv CN27IE,  0x000B
        .equiv CN28IE,  0x000C
        .equiv CN29IE,  0x000D
        .equiv CN30IE,  0x000E
        .equiv CN31IE,  0x000F


;----- CNEN3 Bits -----------------------------------------------------
        .equiv CN32IE,  0x0000


;----- CNEN4 Bits -----------------------------------------------------
        .equiv CN49IE,  0x0001
        .equiv CN50IE,  0x0002
        .equiv CN51IE,  0x0003
        .equiv CN52IE,  0x0004
        .equiv CN53IE,  0x0005
        .equiv CN54IE,  0x0006
        .equiv CN55IE,  0x0007
        .equiv CN56IE,  0x0008
        .equiv CN58IE,  0x000A
        .equiv CN59IE,  0x000B
        .equiv CN60IE,  0x000C
        .equiv CN61IE,  0x000D
        .equiv CN62IE,  0x000E
        .equiv CN63IE,  0x000F


;----- CNEN5 Bits -----------------------------------------------------
        .equiv CN64IE,  0x0000
        .equiv CN65IE,  0x0001
        .equiv CN68IE,  0x0004
        .equiv CN69IE,  0x0005
        .equiv CN71IE,  0x0007


;----- CNPU1 Bits -----------------------------------------------------
        .equiv CN0PUE,  0x0000
        .equiv CN1PUE,  0x0001
        .equiv CN2PUE,  0x0002
        .equiv CN3PUE,  0x0003
        .equiv CN4PUE,  0x0004
        .equiv CN5PUE,  0x0005
        .equiv CN6PUE,  0x0006
        .equiv CN7PUE,  0x0007
        .equiv CN8PUE,  0x0008
        .equiv CN9PUE,  0x0009
        .equiv CN10PUE, 0x000A
        .equiv CN11PUE, 0x000B
        .equiv CN12PUE, 0x000C
        .equiv CN13PUE, 0x000D
        .equiv CN14PUE, 0x000E
        .equiv CN15PUE, 0x000F


;----- CNPU2 Bits -----------------------------------------------------
        .equiv CN16PUE, 0x0000
        .equiv CN17PUE, 0x0001
        .equiv CN18PUE, 0x0002
        .equiv CN22PUE, 0x0006
        .equiv CN23PUE, 0x0007
        .equiv CN24PUE, 0x0008
        .equiv CN25PUE, 0x0009
        .equiv CN26PUE, 0x000A
        .equiv CN27PUE, 0x000B
        .equiv CN28PUE, 0x000C
        .equiv CN29PUE, 0x000D
        .equiv CN30PUE, 0x000E
        .equiv CN31PUE, 0x000F


;----- CNPU3 Bits -----------------------------------------------------
        .equiv CN32PUE, 0x0000


;----- CNPU4 Bits -----------------------------------------------------
        .equiv CN49PUE, 0x0001
        .equiv CN50PUE, 0x0002
        .equiv CN51PUE, 0x0003
        .equiv CN52PUE, 0x0004
        .equiv CN53PUE, 0x0005
        .equiv CN54PUE, 0x0006
        .equiv CN55PUE, 0x0007
        .equiv CN56PUE, 0x0008
        .equiv CN58PUE, 0x000A
        .equiv CN59PUE, 0x000B
        .equiv CN60PUE, 0x000C
        .equiv CN61PUE, 0x000D
        .equiv CN62PUE, 0x000E
        .equiv CN63PUE, 0x000F


;----- CNPU5 Bits -----------------------------------------------------
        .equiv CN64PUE, 0x0000
        .equiv CN65PUE, 0x0001
        .equiv CN68PUE, 0x0004
        .equiv CN69PUE, 0x0005
        .equiv CN71PUE, 0x0007


;----- INTCON1 Bits -----------------------------------------------------
        .equiv OSCFAIL, 0x0001
        .equiv STKERR,  0x0002
        .equiv ADDRERR, 0x0003
        .equiv MATHERR, 0x0004
        .equiv NSTDIS,  0x000F


;----- INTCON2 Bits -----------------------------------------------------
        .equiv INT0EP,  0x0000
        .equiv INT1EP,  0x0001
        .equiv INT2EP,  0x0002
        .equiv DISI,    0x000E
        .equiv ALTIVT,  0x000F


;----- IFS0 Bits -----------------------------------------------------
        .equiv INT0IF,  0x0000
        .equiv IC1IF,   0x0001
        .equiv OC1IF,   0x0002
        .equiv T1IF,    0x0003
        .equiv IC2IF,   0x0005
        .equiv OC2IF,   0x0006
        .equiv T2IF,    0x0007
        .equiv T3IF,    0x0008
        .equiv SPF1IF,  0x0009
        .equiv SPI1IF,  0x000A
        .equiv U1RXIF,  0x000B
        .equiv U1TXIF,  0x000C
        .equiv AD1IF,   0x000D


;----- IFS1 Bits -----------------------------------------------------
        .equiv SI2C1IF, 0x0000
        .equiv MI2C1IF, 0x0001
        .equiv CMIF,    0x0002
        .equiv CNIF,    0x0003
        .equiv INT1IF,  0x0004
        .equiv IC7IF,   0x0006
        .equiv IC8IF,   0x0007
        .equiv OC3IF,   0x0009
        .equiv OC4IF,   0x000A
        .equiv T4IF,    0x000B
        .equiv T5IF,    0x000C
        .equiv INT2IF,  0x000D
        .equiv U2RXIF,  0x000E
        .equiv U2TXIF,  0x000F


;----- IFS2 Bits -----------------------------------------------------
        .equiv SPF2IF,  0x0000
        .equiv SPI2IF,  0x0001
        .equiv IC3IF,   0x0005
        .equiv IC4IF,   0x0006
        .equiv IC5IF,   0x0007
        .equiv IC6IF,   0x0008
        .equiv OC5IF,   0x0009
        .equiv OC6IF,   0x000A
        .equiv OC7IF,   0x000B
        .equiv OC8IF,   0x000C
        .equiv PMPIF,   0x000D


;----- IFS3 Bits -----------------------------------------------------
        .equiv SI2C2IF, 0x0001
        .equiv MI2C2IF, 0x0002
        .equiv INT3IF,  0x0005
        .equiv INT4IF,  0x0006
        .equiv RTCIF,   0x000E


;----- IFS4 Bits -----------------------------------------------------
        .equiv U1ERIF,  0x0001
        .equiv U2ERIF,  0x0002
        .equiv CRCIF,   0x0003
        .equiv LVDIF,   0x0008
        .equiv CTMUIF,  0x000D


;----- IFS5 Bits -----------------------------------------------------
        .equiv U3ERIF,  0x0001
        .equiv U3RXIF,  0x0002
        .equiv U3TXIF,  0x0003
        .equiv SI2C3IF, 0x0004
        .equiv MI2C3IF, 0x0005
        .equiv USB1IF,  0x0006
        .equiv U4ERIF,  0x0007
        .equiv U4RXIF,  0x0008
        .equiv U4TXIF,  0x0009
        .equiv SPF3IF,  0x000A
        .equiv SPI3IF,  0x000B
        .equiv OC9IF,   0x000C
        .equiv IC9IF,   0x000D


;----- IEC0 Bits -----------------------------------------------------
        .equiv INT0IE,  0x0000
        .equiv IC1IE,   0x0001
        .equiv OC1IE,   0x0002
        .equiv T1IE,    0x0003
        .equiv IC2IE,   0x0005
        .equiv OC2IE,   0x0006
        .equiv T2IE,    0x0007
        .equiv T3IE,    0x0008
        .equiv SPF1IE,  0x0009
        .equiv SPI1IE,  0x000A
        .equiv U1RXIE,  0x000B
        .equiv U1TXIE,  0x000C
        .equiv AD1IE,   0x000D


;----- IEC1 Bits -----------------------------------------------------
        .equiv SI2C1IE, 0x0000
        .equiv MI2C1IE, 0x0001
        .equiv CMIE,    0x0002
        .equiv CNIE,    0x0003
        .equiv INT1IE,  0x0004
        .equiv IC7IE,   0x0006
        .equiv IC8IE,   0x0007
        .equiv OC3IE,   0x0009
        .equiv OC4IE,   0x000A
        .equiv T4IE,    0x000B
        .equiv T5IE,    0x000C
        .equiv INT2IE,  0x000D
        .equiv U2RXIE,  0x000E
        .equiv U2TXIE,  0x000F


;----- IEC2 Bits -----------------------------------------------------
        .equiv SPF2IE,  0x0000
        .equiv SPI2IE,  0x0001
        .equiv IC3IE,   0x0005
        .equiv IC4IE,   0x0006
        .equiv IC5IE,   0x0007
        .equiv IC6IE,   0x0008
        .equiv OC5IE,   0x0009
        .equiv OC6IE,   0x000A
        .equiv OC7IE,   0x000B
        .equiv OC8IE,   0x000C
        .equiv PMPIE,   0x000D


;----- IEC3 Bits -----------------------------------------------------
        .equiv SI2C2IE, 0x0001
        .equiv MI2C2IE, 0x0002
        .equiv INT3IE,  0x0005
        .equiv INT4IE,  0x0006
        .equiv RTCIE,   0x000E


;----- IEC4 Bits -----------------------------------------------------
        .equiv U1ERIE,  0x0001
        .equiv U2ERIE,  0x0002
        .equiv CRCIE,   0x0003
        .equiv LVDIE,   0x0008
        .equiv CTMUIE,  0x000D


;----- IEC5 Bits -----------------------------------------------------
        .equiv U3ERIE,  0x0001
        .equiv U3RXIE,  0x0002
        .equiv U3TXIE,  0x0003
        .equiv SI2C3IE, 0x0004
        .equiv MI2C3IE, 0x0005
        .equiv USB1IE,  0x0006
        .equiv U4ERIE,  0x0007
        .equiv U4RXIE,  0x0008
        .equiv U4TXIE,  0x0009
        .equiv SPF3IE,  0x000A
        .equiv SPI3IE,  0x000B
        .equiv OC9IE,   0x000C
        .equiv IC9IE,   0x000D


;----- IPC0 Bits -----------------------------------------------------
        .equiv INT0IP0, 0x0000
        .equiv INT0IP1, 0x0001
        .equiv INT0IP2, 0x0002
        .equiv IC1IP0,  0x0004
        .equiv IC1IP1,  0x0005
        .equiv IC1IP2,  0x0006
        .equiv OC1IP0,  0x0008
        .equiv OC1IP1,  0x0009
        .equiv OC1IP2,  0x000A
        .equiv T1IP0,   0x000C
        .equiv T1IP1,   0x000D
        .equiv T1IP2,   0x000E



;----- IPC1 Bits -----------------------------------------------------
        .equiv IC2IP0,  0x0004
        .equiv IC2IP1,  0x0005
        .equiv IC2IP2,  0x0006
        .equiv OC2IP0,  0x0008
        .equiv OC2IP1,  0x0009
        .equiv OC2IP2,  0x000A
        .equiv T2IP0,   0x000C
        .equiv T2IP1,   0x000D
        .equiv T2IP2,   0x000E



;----- IPC2 Bits -----------------------------------------------------
        .equiv T3IP0,   0x0000
        .equiv T3IP1,   0x0001
        .equiv T3IP2,   0x0002
        .equiv SPF1IP0, 0x0004
        .equiv SPF1IP1, 0x0005
        .equiv SPF1IP2, 0x0006
        .equiv SPI1IP0, 0x0008
        .equiv SPI1IP1, 0x0009
        .equiv SPI1IP2, 0x000A
        .equiv U1RXIP0, 0x000C
        .equiv U1RXIP1, 0x000D
        .equiv U1RXIP2, 0x000E



;----- IPC3 Bits -----------------------------------------------------
        .equiv U1TXIP0, 0x0000
        .equiv U1TXIP1, 0x0001
        .equiv U1TXIP2, 0x0002
        .equiv AD1IP0,  0x0004
        .equiv AD1IP1,  0x0005
        .equiv AD1IP2,  0x0006



;----- IPC4 Bits -----------------------------------------------------
        .equiv SI2C1P0, 0x0000
        .equiv SI2C1P1, 0x0001
        .equiv SI2C1P2, 0x0002
        .equiv MI2C1P0, 0x0004
        .equiv MI2C1P1, 0x0005
        .equiv MI2C1P2, 0x0006
        .equiv CMIP0,   0x0008
        .equiv CMIP1,   0x0009
        .equiv CMIP2,   0x000A
        .equiv CNIP0,   0x000C
        .equiv CNIP1,   0x000D
        .equiv CNIP2,   0x000E



;----- IPC5 Bits -----------------------------------------------------
        .equiv INT1IP0, 0x0000
        .equiv INT1IP1, 0x0001
        .equiv INT1IP2, 0x0002
        .equiv IC7IP0,  0x0008
        .equiv IC7IP1,  0x0009
        .equiv IC7IP2,  0x000A
        .equiv IC8IP0,  0x000C
        .equiv IC8IP1,  0x000D
        .equiv IC8IP2,  0x000E



;----- IPC6 Bits -----------------------------------------------------
        .equiv OC3IP0,  0x0004
        .equiv OC3IP1,  0x0005
        .equiv OC3IP2,  0x0006
        .equiv OC4IP0,  0x0008
        .equiv OC4IP1,  0x0009
        .equiv OC4IP2,  0x000A
        .equiv T4IP0,   0x000C
        .equiv T4IP1,   0x000D
        .equiv T4IP2,   0x000E



;----- IPC7 Bits -----------------------------------------------------
        .equiv T5IP0,   0x0000
        .equiv T5IP1,   0x0001
        .equiv T5IP2,   0x0002
        .equiv INT2IP0, 0x0004
        .equiv INT2IP1, 0x0005
        .equiv INT2IP2, 0x0006
        .equiv U2RXIP0, 0x0008
        .equiv U2RXIP1, 0x0009
        .equiv U2RXIP2, 0x000A
        .equiv U2TXIP0, 0x000C
        .equiv U2TXIP1, 0x000D
        .equiv U2TXIP2, 0x000E



;----- IPC8 Bits -----------------------------------------------------
        .equiv SPF2IP0, 0x0000
        .equiv SPF2IP1, 0x0001
        .equiv SPF2IP2, 0x0002
        .equiv SPI2IP0, 0x0004
        .equiv SPI2IP1, 0x0005
        .equiv SPI2IP2, 0x0006



;----- IPC9 Bits -----------------------------------------------------
        .equiv IC3IP0,  0x0004
        .equiv IC3IP1,  0x0005
        .equiv IC3IP2,  0x0006
        .equiv IC4IP0,  0x0008
        .equiv IC4IP1,  0x0009
        .equiv IC4IP2,  0x000A
        .equiv IC5IP0,  0x000C
        .equiv IC5IP1,  0x000D
        .equiv IC5IP2,  0x000E



;----- IPC10 Bits -----------------------------------------------------
        .equiv IC6IP0,  0x0000
        .equiv IC6IP1,  0x0001
        .equiv IC6IP2,  0x0002
        .equiv OC5IP0,  0x0004
        .equiv OC5IP1,  0x0005
        .equiv OC5IP2,  0x0006
        .equiv OC6IP0,  0x0008
        .equiv OC6IP1,  0x0009
        .equiv OC6IP2,  0x000A
        .equiv OC7IP0,  0x000C
        .equiv OC7IP1,  0x000D
        .equiv OC7IP2,  0x000E



;----- IPC11 Bits -----------------------------------------------------
        .equiv OC8IP0,  0x0000
        .equiv OC8IP1,  0x0001
        .equiv OC8IP2,  0x0002
        .equiv PMPIP0,  0x0004
        .equiv PMPIP1,  0x0005
        .equiv PMPIP2,  0x0006



;----- IPC12 Bits -----------------------------------------------------
        .equiv SI2C2P0, 0x0004
        .equiv SI2C2P1, 0x0005
        .equiv SI2C2P2, 0x0006
        .equiv MI2C2P0, 0x0008
        .equiv MI2C2P1, 0x0009
        .equiv MI2C2P2, 0x000A



;----- IPC13 Bits -----------------------------------------------------
        .equiv INT3IP0, 0x0004
        .equiv INT3IP1, 0x0005
        .equiv INT3IP2, 0x0006
        .equiv INT4IP0, 0x0008
        .equiv INT4IP1, 0x0009
        .equiv INT4IP2, 0x000A



;----- IPC15 Bits -----------------------------------------------------
        .equiv RTCIP0,  0x0008
        .equiv RTCIP1,  0x0009
        .equiv RTCIP2,  0x000A



;----- IPC16 Bits -----------------------------------------------------
        .equiv U1ERIP0, 0x0004
        .equiv U1ERIP1, 0x0005
        .equiv U1ERIP2, 0x0006
        .equiv U2ERIP0, 0x0008
        .equiv U2ERIP1, 0x0009
        .equiv U2ERIP2, 0x000A
        .equiv CRCIP0,  0x000C
        .equiv CRCIP1,  0x000D
        .equiv CRCIP2,  0x000E



;----- IPC18 Bits -----------------------------------------------------
        .equiv LVDIP0,  0x0000
        .equiv LVDIP1,  0x0001
        .equiv LVDIP2,  0x0002



;----- IPC19 Bits -----------------------------------------------------
        .equiv CTMUIP0, 0x0004
        .equiv CTMUIP1, 0x0005
        .equiv CTMUIP2, 0x0006



;----- IPC20 Bits -----------------------------------------------------
        .equiv U3ERIP0, 0x0004
        .equiv U3ERIP1, 0x0005
        .equiv U3ERIP2, 0x0006
        .equiv U3RXIP0, 0x0008
        .equiv U3RXIP1, 0x0009
        .equiv U3RXIP2, 0x000A
        .equiv U3TXIP0, 0x000C
        .equiv U3TXIP1, 0x000D
        .equiv U3TXIP2, 0x000E



;----- IPC21 Bits -----------------------------------------------------
        .equiv SI2C3P0, 0x0000
        .equiv SI2C3P1, 0x0001
        .equiv SI2C3P2, 0x0002
        .equiv MI2C3P0, 0x0004
        .equiv MI2C3P1, 0x0005
        .equiv MI2C3P2, 0x0006
        .equiv USB1IP0, 0x0008
        .equiv USB1IP1, 0x0009
        .equiv USB1IP2, 0x000A
        .equiv U4ERIP0, 0x000C
        .equiv U4ERIP1, 0x000D
        .equiv U4ERIP2, 0x000E



;----- IPC22 Bits -----------------------------------------------------
        .equiv U4RXIP0, 0x0000
        .equiv U4RXIP1, 0x0001
        .equiv U4RXIP2, 0x0002
        .equiv U4TXIP0, 0x0004
        .equiv U4TXIP1, 0x0005
        .equiv U4TXIP2, 0x0006
        .equiv SPF3IP0, 0x0008
        .equiv SPF3IP1, 0x0009
        .equiv SPF3IP2, 0x000A
        .equiv SPI3IP0, 0x000C
        .equiv SPI3IP1, 0x000D
        .equiv SPI3IP2, 0x000E



;----- IPC23 Bits -----------------------------------------------------
        .equiv OC9IP0,  0x0000
        .equiv OC9IP1,  0x0001
        .equiv OC9IP2,  0x0002
        .equiv IC9IP0,  0x0004
        .equiv IC9IP1,  0x0005
        .equiv IC9IP2,  0x0006



;----- T1CON Bits -----------------------------------------------------
        .equiv TCS,     0x0001
        .equiv TSYNC,   0x0002
        .equiv TCKPS0,  0x0004
        .equiv TCKPS1,  0x0005
        .equiv TGATE,   0x0006
        .equiv TSIDL,   0x000D
        .equiv TON,     0x000F



;----- T2CON Bits -----------------------------------------------------
       ;.equiv TCS,     0x0001
        .equiv T32,     0x0003
       ;.equiv TCKPS0,  0x0004
       ;.equiv TCKPS1,  0x0005
       ;.equiv TGATE,   0x0006
       ;.equiv TSIDL,   0x000D
       ;.equiv TON,     0x000F



;----- T3CON Bits -----------------------------------------------------
       ;.equiv TCS,     0x0001
       ;.equiv TCKPS0,  0x0004
       ;.equiv TCKPS1,  0x0005
       ;.equiv TGATE,   0x0006
       ;.equiv TSIDL,   0x000D
       ;.equiv TON,     0x000F



;----- T4CON Bits -----------------------------------------------------
       ;.equiv TCS,     0x0001
       ;.equiv T32,     0x0003
       ;.equiv TCKPS0,  0x0004
       ;.equiv TCKPS1,  0x0005
       ;.equiv TGATE,   0x0006
       ;.equiv TSIDL,   0x000D
       ;.equiv TON,     0x000F



;----- T5CON Bits -----------------------------------------------------
       ;.equiv TCS,     0x0001
       ;.equiv TCKPS0,  0x0004
       ;.equiv TCKPS1,  0x0005
       ;.equiv TGATE,   0x0006
       ;.equiv TSIDL,   0x000D
       ;.equiv TON,     0x000F



;----- IC1CON1 Bits -----------------------------------------------------
        .equiv ICM0,    0x0000
        .equiv ICM1,    0x0001
        .equiv ICM2,    0x0002
        .equiv ICBNE,   0x0003
        .equiv ICOV,    0x0004
        .equiv ICI0,    0x0005
        .equiv ICI1,    0x0006
        .equiv ICTSEL0, 0x000A
        .equiv ICTSEL1, 0x000B
        .equiv ICTSEL2, 0x000C
        .equiv ICSIDL,  0x000D



;----- IC1CON2 Bits -----------------------------------------------------
        .equiv SYNCSEL0, 0x0000
        .equiv SYNCSEL1, 0x0001
        .equiv SYNCSEL2, 0x0002
        .equiv SYNCSEL3, 0x0003
        .equiv SYNCSEL4, 0x0004
        .equiv TRIGSTAT, 0x0006
        .equiv ICTRIG,  0x0007
        .equiv IC32,    0x0008



;----- IC2CON1 Bits -----------------------------------------------------
       ;.equiv ICM0,    0x0000
       ;.equiv ICM1,    0x0001
       ;.equiv ICM2,    0x0002
       ;.equiv ICBNE,   0x0003
       ;.equiv ICOV,    0x0004
       ;.equiv ICI0,    0x0005
       ;.equiv ICI1,    0x0006
       ;.equiv ICTSEL0, 0x000A
       ;.equiv ICTSEL1, 0x000B
       ;.equiv ICTSEL2, 0x000C
       ;.equiv ICSIDL,  0x000D



;----- IC2CON2 Bits -----------------------------------------------------
       ;.equiv SYNCSEL0, 0x0000
       ;.equiv SYNCSEL1, 0x0001
       ;.equiv SYNCSEL2, 0x0002
       ;.equiv SYNCSEL3, 0x0003
       ;.equiv SYNCSEL4, 0x0004
       ;.equiv TRIGSTAT, 0x0006
       ;.equiv ICTRIG,  0x0007
       ;.equiv IC32,    0x0008



;----- IC3CON1 Bits -----------------------------------------------------
       ;.equiv ICM0,    0x0000
       ;.equiv ICM1,    0x0001
       ;.equiv ICM2,    0x0002
       ;.equiv ICBNE,   0x0003
       ;.equiv ICOV,    0x0004
       ;.equiv ICI0,    0x0005
       ;.equiv ICI1,    0x0006
       ;.equiv ICTSEL0, 0x000A
       ;.equiv ICTSEL1, 0x000B
       ;.equiv ICTSEL2, 0x000C
       ;.equiv ICSIDL,  0x000D



;----- IC3CON2 Bits -----------------------------------------------------
       ;.equiv SYNCSEL0, 0x0000
       ;.equiv SYNCSEL1, 0x0001
       ;.equiv SYNCSEL2, 0x0002
       ;.equiv SYNCSEL3, 0x0003
       ;.equiv SYNCSEL4, 0x0004
       ;.equiv TRIGSTAT, 0x0006
       ;.equiv ICTRIG,  0x0007
       ;.equiv IC32,    0x0008



;----- IC4CON1 Bits -----------------------------------------------------
       ;.equiv ICM0,    0x0000
       ;.equiv ICM1,    0x0001
       ;.equiv ICM2,    0x0002
       ;.equiv ICBNE,   0x0003
       ;.equiv ICOV,    0x0004
       ;.equiv ICI0,    0x0005
       ;.equiv ICI1,    0x0006
       ;.equiv ICTSEL0, 0x000A
       ;.equiv ICTSEL1, 0x000B
       ;.equiv ICTSEL2, 0x000C
       ;.equiv ICSIDL,  0x000D



;----- IC4CON2 Bits -----------------------------------------------------
       ;.equiv SYNCSEL0, 0x0000
       ;.equiv SYNCSEL1, 0x0001
       ;.equiv SYNCSEL2, 0x0002
       ;.equiv SYNCSEL3, 0x0003
       ;.equiv SYNCSEL4, 0x0004
       ;.equiv TRIGSTAT, 0x0006
       ;.equiv ICTRIG,  0x0007
       ;.equiv IC32,    0x0008



;----- IC5CON1 Bits -----------------------------------------------------
       ;.equiv ICM0,    0x0000
       ;.equiv ICM1,    0x0001
       ;.equiv ICM2,    0x0002
       ;.equiv ICBNE,   0x0003
       ;.equiv ICOV,    0x0004
       ;.equiv ICI0,    0x0005
       ;.equiv ICI1,    0x0006
       ;.equiv ICTSEL0, 0x000A
       ;.equiv ICTSEL1, 0x000B
       ;.equiv ICTSEL2, 0x000C
       ;.equiv ICSIDL,  0x000D



;----- IC5CON2 Bits -----------------------------------------------------
       ;.equiv SYNCSEL0, 0x0000
       ;.equiv SYNCSEL1, 0x0001
       ;.equiv SYNCSEL2, 0x0002
       ;.equiv SYNCSEL3, 0x0003
       ;.equiv SYNCSEL4, 0x0004
       ;.equiv TRIGSTAT, 0x0006
       ;.equiv ICTRIG,  0x0007
       ;.equiv IC32,    0x0008



;----- IC6CON1 Bits -----------------------------------------------------
       ;.equiv ICM0,    0x0000
       ;.equiv ICM1,    0x0001
       ;.equiv ICM2,    0x0002
       ;.equiv ICBNE,   0x0003
       ;.equiv ICOV,    0x0004
       ;.equiv ICI0,    0x0005
       ;.equiv ICI1,    0x0006
       ;.equiv ICTSEL0, 0x000A
       ;.equiv ICTSEL1, 0x000B
       ;.equiv ICTSEL2, 0x000C
       ;.equiv ICSIDL,  0x000D



;----- IC6CON2 Bits -----------------------------------------------------
       ;.equiv SYNCSEL0, 0x0000
       ;.equiv SYNCSEL1, 0x0001
       ;.equiv SYNCSEL2, 0x0002
       ;.equiv SYNCSEL3, 0x0003
       ;.equiv SYNCSEL4, 0x0004
       ;.equiv TRIGSTAT, 0x0006
       ;.equiv ICTRIG,  0x0007
       ;.equiv IC32,    0x0008



;----- IC7CON1 Bits -----------------------------------------------------
       ;.equiv ICM0,    0x0000
       ;.equiv ICM1,    0x0001
       ;.equiv ICM2,    0x0002
       ;.equiv ICBNE,   0x0003
       ;.equiv ICOV,    0x0004
       ;.equiv ICI0,    0x0005
       ;.equiv ICI1,    0x0006
       ;.equiv ICTSEL0, 0x000A
       ;.equiv ICTSEL1, 0x000B
       ;.equiv ICTSEL2, 0x000C
       ;.equiv ICSIDL,  0x000D



;----- IC7CON2 Bits -----------------------------------------------------
       ;.equiv SYNCSEL0, 0x0000
       ;.equiv SYNCSEL1, 0x0001
       ;.equiv SYNCSEL2, 0x0002
       ;.equiv SYNCSEL3, 0x0003
       ;.equiv SYNCSEL4, 0x0004
       ;.equiv TRIGSTAT, 0x0006
       ;.equiv ICTRIG,  0x0007
       ;.equiv IC32,    0x0008



;----- IC8CON1 Bits -----------------------------------------------------
       ;.equiv ICM0,    0x0000
       ;.equiv ICM1,    0x0001
       ;.equiv ICM2,    0x0002
       ;.equiv ICBNE,   0x0003
       ;.equiv ICOV,    0x0004
       ;.equiv ICI0,    0x0005
       ;.equiv ICI1,    0x0006
       ;.equiv ICTSEL0, 0x000A
       ;.equiv ICTSEL1, 0x000B
       ;.equiv ICTSEL2, 0x000C
       ;.equiv ICSIDL,  0x000D



;----- IC8CON2 Bits -----------------------------------------------------
       ;.equiv SYNCSEL0, 0x0000
       ;.equiv SYNCSEL1, 0x0001
       ;.equiv SYNCSEL2, 0x0002
       ;.equiv SYNCSEL3, 0x0003
       ;.equiv SYNCSEL4, 0x0004
       ;.equiv TRIGSTAT, 0x0006
       ;.equiv ICTRIG,  0x0007
       ;.equiv IC32,    0x0008



;----- IC9CON1 Bits -----------------------------------------------------
       ;.equiv ICM0,    0x0000
       ;.equiv ICM1,    0x0001
       ;.equiv ICM2,    0x0002
       ;.equiv ICBNE,   0x0003
       ;.equiv ICOV,    0x0004
       ;.equiv ICI0,    0x0005
       ;.equiv ICI1,    0x0006
       ;.equiv ICTSEL0, 0x000A
       ;.equiv ICTSEL1, 0x000B
       ;.equiv ICTSEL2, 0x000C
       ;.equiv ICSIDL,  0x000D



;----- IC9CON2 Bits -----------------------------------------------------
       ;.equiv SYNCSEL0, 0x0000
       ;.equiv SYNCSEL1, 0x0001
       ;.equiv SYNCSEL2, 0x0002
       ;.equiv SYNCSEL3, 0x0003
       ;.equiv SYNCSEL4, 0x0004
       ;.equiv TRIGSTAT, 0x0006
       ;.equiv ICTRIG,  0x0007
       ;.equiv IC32,    0x0008



;----- OC1CON1 Bits -----------------------------------------------------
        .equiv OCM0,    0x0000
        .equiv OCM1,    0x0001
        .equiv OCM2,    0x0002
        .equiv TRIGMODE, 0x0003
        .equiv OCFLT0,  0x0004
        .equiv OCFLT1,  0x0005
        .equiv OCFLT2,  0x0006
        .equiv ENFLT0,  0x0007
        .equiv ENFLT1,  0x0008
        .equiv ENFLT2,  0x0009
        .equiv OCTSEL0, 0x000A
        .equiv OCTSEL1, 0x000B
        .equiv OCTSEL2, 0x000C
        .equiv OCSIDL,  0x000D



;----- OC1CON2 Bits -----------------------------------------------------
       ;.equiv SYNCSEL0, 0x0000
       ;.equiv SYNCSEL1, 0x0001
       ;.equiv SYNCSEL2, 0x0002
       ;.equiv SYNCSEL3, 0x0003
       ;.equiv SYNCSEL4, 0x0004
        .equiv OCTRIS,  0x0005
       ;.equiv TRIGSTAT, 0x0006
        .equiv OCTRIG,  0x0007
        .equiv OC32,    0x0008
        .equiv OCINV,   0x000C
        .equiv FLTTRIEN, 0x000D
        .equiv FLTOUT,  0x000E
        .equiv FLTMODE, 0x000F



;----- OC2CON1 Bits -----------------------------------------------------
       ;.equiv OCM0,    0x0000
       ;.equiv OCM1,    0x0001
       ;.equiv OCM2,    0x0002
       ;.equiv TRIGMODE, 0x0003
       ;.equiv OCFLT0,  0x0004
       ;.equiv OCFLT1,  0x0005
       ;.equiv OCFLT2,  0x0006
       ;.equiv ENFLT0,  0x0007
       ;.equiv ENFLT1,  0x0008
       ;.equiv ENFLT2,  0x0009
       ;.equiv OCTSEL0, 0x000A
       ;.equiv OCTSEL1, 0x000B
       ;.equiv OCTSEL2, 0x000C
       ;.equiv OCSIDL,  0x000D



;----- OC2CON2 Bits -----------------------------------------------------
       ;.equiv SYNCSEL0, 0x0000
       ;.equiv SYNCSEL1, 0x0001
       ;.equiv SYNCSEL2, 0x0002
       ;.equiv SYNCSEL3, 0x0003
       ;.equiv SYNCSEL4, 0x0004
       ;.equiv OCTRIS,  0x0005
       ;.equiv TRIGSTAT, 0x0006
       ;.equiv OCTRIG,  0x0007
       ;.equiv OC32,    0x0008
       ;.equiv OCINV,   0x000C
       ;.equiv FLTTRIEN, 0x000D
       ;.equiv FLTOUT,  0x000E
       ;.equiv FLTMODE, 0x000F



;----- OC3CON1 Bits -----------------------------------------------------
       ;.equiv OCM0,    0x0000
       ;.equiv OCM1,    0x0001
       ;.equiv OCM2,    0x0002
       ;.equiv TRIGMODE, 0x0003
       ;.equiv OCFLT0,  0x0004
       ;.equiv OCFLT1,  0x0005
       ;.equiv OCFLT2,  0x0006
       ;.equiv ENFLT0,  0x0007
       ;.equiv ENFLT1,  0x0008
       ;.equiv ENFLT2,  0x0009
       ;.equiv OCTSEL0, 0x000A
       ;.equiv OCTSEL1, 0x000B
       ;.equiv OCTSEL2, 0x000C
       ;.equiv OCSIDL,  0x000D



;----- OC3CON2 Bits -----------------------------------------------------
       ;.equiv SYNCSEL0, 0x0000
       ;.equiv SYNCSEL1, 0x0001
       ;.equiv SYNCSEL2, 0x0002
       ;.equiv SYNCSEL3, 0x0003
       ;.equiv SYNCSEL4, 0x0004
       ;.equiv OCTRIS,  0x0005
       ;.equiv TRIGSTAT, 0x0006
       ;.equiv OCTRIG,  0x0007
       ;.equiv OC32,    0x0008
       ;.equiv OCINV,   0x000C
       ;.equiv FLTTRIEN, 0x000D
       ;.equiv FLTOUT,  0x000E
       ;.equiv FLTMODE, 0x000F



;----- OC4CON1 Bits -----------------------------------------------------
       ;.equiv OCM0,    0x0000
       ;.equiv OCM1,    0x0001
       ;.equiv OCM2,    0x0002
       ;.equiv TRIGMODE, 0x0003
       ;.equiv OCFLT0,  0x0004
       ;.equiv OCFLT1,  0x0005
       ;.equiv OCFLT2,  0x0006
       ;.equiv ENFLT0,  0x0007
       ;.equiv ENFLT1,  0x0008
       ;.equiv ENFLT2,  0x0009
       ;.equiv OCTSEL0, 0x000A
       ;.equiv OCTSEL1, 0x000B
       ;.equiv OCTSEL2, 0x000C
       ;.equiv OCSIDL,  0x000D



;----- OC4CON2 Bits -----------------------------------------------------
       ;.equiv SYNCSEL0, 0x0000
       ;.equiv SYNCSEL1, 0x0001
       ;.equiv SYNCSEL2, 0x0002
       ;.equiv SYNCSEL3, 0x0003
       ;.equiv SYNCSEL4, 0x0004
       ;.equiv OCTRIS,  0x0005
       ;.equiv TRIGSTAT, 0x0006
       ;.equiv OCTRIG,  0x0007
       ;.equiv OC32,    0x0008
       ;.equiv OCINV,   0x000C
       ;.equiv FLTTRIEN, 0x000D
       ;.equiv FLTOUT,  0x000E
       ;.equiv FLTMODE, 0x000F



;----- OC5CON1 Bits -----------------------------------------------------
       ;.equiv OCM0,    0x0000
       ;.equiv OCM1,    0x0001
       ;.equiv OCM2,    0x0002
       ;.equiv TRIGMODE, 0x0003
       ;.equiv OCFLT0,  0x0004
       ;.equiv OCFLT1,  0x0005
       ;.equiv OCFLT2,  0x0006
       ;.equiv ENFLT0,  0x0007
       ;.equiv ENFLT1,  0x0008
       ;.equiv ENFLT2,  0x0009
       ;.equiv OCTSEL0, 0x000A
       ;.equiv OCTSEL1, 0x000B
       ;.equiv OCTSEL2, 0x000C
       ;.equiv OCSIDL,  0x000D



;----- OC5CON2 Bits -----------------------------------------------------
       ;.equiv SYNCSEL0, 0x0000
       ;.equiv SYNCSEL1, 0x0001
       ;.equiv SYNCSEL2, 0x0002
       ;.equiv SYNCSEL3, 0x0003
       ;.equiv SYNCSEL4, 0x0004
       ;.equiv OCTRIS,  0x0005
       ;.equiv TRIGSTAT, 0x0006
       ;.equiv OCTRIG,  0x0007
       ;.equiv OC32,    0x0008
       ;.equiv OCINV,   0x000C
       ;.equiv FLTTRIEN, 0x000D
       ;.equiv FLTOUT,  0x000E
       ;.equiv FLTMODE, 0x000F



;----- OC6CON1 Bits -----------------------------------------------------
       ;.equiv OCM0,    0x0000
       ;.equiv OCM1,    0x0001
       ;.equiv OCM2,    0x0002
       ;.equiv TRIGMODE, 0x0003
       ;.equiv OCFLT0,  0x0004
       ;.equiv OCFLT1,  0x0005
       ;.equiv OCFLT2,  0x0006
       ;.equiv ENFLT0,  0x0007
       ;.equiv ENFLT1,  0x0008
       ;.equiv ENFLT2,  0x0009
       ;.equiv OCTSEL0, 0x000A
       ;.equiv OCTSEL1, 0x000B
       ;.equiv OCTSEL2, 0x000C
       ;.equiv OCSIDL,  0x000D



;----- OC6CON2 Bits -----------------------------------------------------
       ;.equiv SYNCSEL0, 0x0000
       ;.equiv SYNCSEL1, 0x0001
       ;.equiv SYNCSEL2, 0x0002
       ;.equiv SYNCSEL3, 0x0003
       ;.equiv SYNCSEL4, 0x0004
       ;.equiv OCTRIS,  0x0005
       ;.equiv TRIGSTAT, 0x0006
       ;.equiv OCTRIG,  0x0007
       ;.equiv OC32,    0x0008
       ;.equiv OCINV,   0x000C
       ;.equiv FLTTRIEN, 0x000D
       ;.equiv FLTOUT,  0x000E
       ;.equiv FLTMODE, 0x000F



;----- OC7CON1 Bits -----------------------------------------------------
       ;.equiv OCM0,    0x0000
       ;.equiv OCM1,    0x0001
       ;.equiv OCM2,    0x0002
       ;.equiv TRIGMODE, 0x0003
       ;.equiv OCFLT0,  0x0004
       ;.equiv OCFLT1,  0x0005
       ;.equiv OCFLT2,  0x0006
       ;.equiv ENFLT0,  0x0007
       ;.equiv ENFLT1,  0x0008
       ;.equiv ENFLT2,  0x0009
       ;.equiv OCTSEL0, 0x000A
       ;.equiv OCTSEL1, 0x000B
       ;.equiv OCTSEL2, 0x000C
       ;.equiv OCSIDL,  0x000D



;----- OC7CON2 Bits -----------------------------------------------------
       ;.equiv SYNCSEL0, 0x0000
       ;.equiv SYNCSEL1, 0x0001
       ;.equiv SYNCSEL2, 0x0002
       ;.equiv SYNCSEL3, 0x0003
       ;.equiv SYNCSEL4, 0x0004
       ;.equiv OCTRIS,  0x0005
       ;.equiv TRIGSTAT, 0x0006
       ;.equiv OCTRIG,  0x0007
       ;.equiv OC32,    0x0008
       ;.equiv OCINV,   0x000C
       ;.equiv FLTTRIEN, 0x000D
       ;.equiv FLTOUT,  0x000E
       ;.equiv FLTMODE, 0x000F



;----- OC8CON1 Bits -----------------------------------------------------
       ;.equiv OCM0,    0x0000
       ;.equiv OCM1,    0x0001
       ;.equiv OCM2,    0x0002
       ;.equiv TRIGMODE, 0x0003
       ;.equiv OCFLT0,  0x0004
       ;.equiv OCFLT1,  0x0005
       ;.equiv OCFLT2,  0x0006
       ;.equiv ENFLT0,  0x0007
       ;.equiv ENFLT1,  0x0008
       ;.equiv ENFLT2,  0x0009
       ;.equiv OCTSEL0, 0x000A
       ;.equiv OCTSEL1, 0x000B
       ;.equiv OCTSEL2, 0x000C
       ;.equiv OCSIDL,  0x000D



;----- OC8CON2 Bits -----------------------------------------------------
       ;.equiv SYNCSEL0, 0x0000
       ;.equiv SYNCSEL1, 0x0001
       ;.equiv SYNCSEL2, 0x0002
       ;.equiv SYNCSEL3, 0x0003
       ;.equiv SYNCSEL4, 0x0004
       ;.equiv OCTRIS,  0x0005
       ;.equiv TRIGSTAT, 0x0006
       ;.equiv OCTRIG,  0x0007
       ;.equiv OC32,    0x0008
       ;.equiv OCINV,   0x000C
       ;.equiv FLTTRIEN, 0x000D
       ;.equiv FLTOUT,  0x000E
       ;.equiv FLTMODE, 0x000F



;----- OC9CON1 Bits -----------------------------------------------------
       ;.equiv OCM0,    0x0000
       ;.equiv OCM1,    0x0001
       ;.equiv OCM2,    0x0002
       ;.equiv TRIGMODE, 0x0003
       ;.equiv OCFLT0,  0x0004
       ;.equiv OCFLT1,  0x0005
       ;.equiv OCFLT2,  0x0006
       ;.equiv ENFLT0,  0x0007
       ;.equiv ENFLT1,  0x0008
       ;.equiv ENFLT2,  0x0009
       ;.equiv OCTSEL0, 0x000A
       ;.equiv OCTSEL1, 0x000B
       ;.equiv OCTSEL2, 0x000C
       ;.equiv OCSIDL,  0x000D



;----- OC9CON2 Bits -----------------------------------------------------
       ;.equiv SYNCSEL0, 0x0000
       ;.equiv SYNCSEL1, 0x0001
       ;.equiv SYNCSEL2, 0x0002
       ;.equiv SYNCSEL3, 0x0003
       ;.equiv SYNCSEL4, 0x0004
       ;.equiv OCTRIS,  0x0005
       ;.equiv TRIGSTAT, 0x0006
       ;.equiv OCTRIG,  0x0007
       ;.equiv OC32,    0x0008
       ;.equiv OCINV,   0x000C
       ;.equiv FLTTRIEN, 0x000D
       ;.equiv FLTOUT,  0x000E
       ;.equiv FLTMODE, 0x000F



;----- I2C1CON Bits -----------------------------------------------------
        .equiv SEN,     0x0000
        .equiv RSEN,    0x0001
        .equiv PEN,     0x0002
        .equiv RCEN,    0x0003
        .equiv ACKEN,   0x0004
        .equiv ACKDT,   0x0005
        .equiv STREN,   0x0006
        .equiv GCEN,    0x0007
        .equiv SMEN,    0x0008
        .equiv DISSLW,  0x0009
        .equiv A10M,    0x000A
        .equiv IPMIEN,  0x000B
        .equiv SCLREL,  0x000C
        .equiv I2CSIDL, 0x000D
        .equiv I2CEN,   0x000F


;----- I2C1STAT Bits -----------------------------------------------------
        .equiv TBF,     0x0000
        .equiv RBF,     0x0001
        .equiv R_W,     0x0002
        .equiv S,       0x0003
        .equiv P,       0x0004
        .equiv D_A,     0x0005
        .equiv I2COV,   0x0006
        .equiv IWCOL,   0x0007
        .equiv ADD10,   0x0008
        .equiv GCSTAT,  0x0009
        .equiv BCL,     0x000A
        .equiv TRSTAT,  0x000E
        .equiv ACKSTAT, 0x000F


;----- I2C2CON Bits -----------------------------------------------------
       ;.equiv SEN,     0x0000
       ;.equiv RSEN,    0x0001
       ;.equiv PEN,     0x0002
       ;.equiv RCEN,    0x0003
       ;.equiv ACKEN,   0x0004
       ;.equiv ACKDT,   0x0005
       ;.equiv STREN,   0x0006
       ;.equiv GCEN,    0x0007
       ;.equiv SMEN,    0x0008
       ;.equiv DISSLW,  0x0009
       ;.equiv A10M,    0x000A
       ;.equiv IPMIEN,  0x000B
       ;.equiv SCLREL,  0x000C
       ;.equiv I2CSIDL, 0x000D
       ;.equiv I2CEN,   0x000F


;----- I2C2STAT Bits -----------------------------------------------------
       ;.equiv TBF,     0x0000
       ;.equiv RBF,     0x0001
       ;.equiv R_W,     0x0002
       ;.equiv S,       0x0003
       ;.equiv P,       0x0004
       ;.equiv D_A,     0x0005
       ;.equiv I2COV,   0x0006
       ;.equiv IWCOL,   0x0007
       ;.equiv ADD10,   0x0008
       ;.equiv GCSTAT,  0x0009
       ;.equiv BCL,     0x000A
       ;.equiv TRSTAT,  0x000E
       ;.equiv ACKSTAT, 0x000F


;----- U1MODE Bits -----------------------------------------------------
        .equiv STSEL,   0x0000
        .equiv PDSEL0,  0x0001
        .equiv PDSEL1,  0x0002
        .equiv BRGH,    0x0003
        .equiv RXINV,   0x0004
        .equiv ABAUD,   0x0005
        .equiv LPBACK,  0x0006
        .equiv WAKE,    0x0007
        .equiv UEN0,    0x0008
        .equiv UEN1,    0x0009
        .equiv RTSMD,   0x000B
        .equiv IREN,    0x000C
        .equiv USIDL,   0x000D
        .equiv UARTEN,  0x000F



;----- U1STA Bits -----------------------------------------------------
        .equiv URXDA,   0x0000
        .equiv OERR,    0x0001
        .equiv FERR,    0x0002
        .equiv PERR,    0x0003
        .equiv RIDLE,   0x0004
        .equiv ADDEN,   0x0005
        .equiv URXISEL0, 0x0006
        .equiv URXISEL1, 0x0007
        .equiv TRMT,    0x0008
        .equiv UTXBF,   0x0009
        .equiv UTXEN,   0x000A
        .equiv UTXBRK,  0x000B
        .equiv UTXISEL0, 0x000D
        .equiv UTXINV,  0x000E
        .equiv UTXISEL1, 0x000F



;----- U2MODE Bits -----------------------------------------------------
       ;.equiv STSEL,   0x0000
       ;.equiv PDSEL0,  0x0001
       ;.equiv PDSEL1,  0x0002
       ;.equiv BRGH,    0x0003
       ;.equiv RXINV,   0x0004
       ;.equiv ABAUD,   0x0005
       ;.equiv LPBACK,  0x0006
       ;.equiv WAKE,    0x0007
       ;.equiv UEN0,    0x0008
       ;.equiv UEN1,    0x0009
       ;.equiv RTSMD,   0x000B
       ;.equiv IREN,    0x000C
       ;.equiv USIDL,   0x000D
       ;.equiv UARTEN,  0x000F



;----- U2STA Bits -----------------------------------------------------
       ;.equiv URXDA,   0x0000
       ;.equiv OERR,    0x0001
       ;.equiv FERR,    0x0002
       ;.equiv PERR,    0x0003
       ;.equiv RIDLE,   0x0004
       ;.equiv ADDEN,   0x0005
       ;.equiv URXISEL0, 0x0006
       ;.equiv URXISEL1, 0x0007
       ;.equiv TRMT,    0x0008
       ;.equiv UTXBF,   0x0009
       ;.equiv UTXEN,   0x000A
       ;.equiv UTXBRK,  0x000B
       ;.equiv UTXISEL0, 0x000D
       ;.equiv UTXINV,  0x000E
       ;.equiv UTXISEL1, 0x000F



;----- SPI1STAT Bits -----------------------------------------------------
        .equiv SPIRBF,  0x0000
        .equiv SPITBF,  0x0001
        .equiv SISEL0,  0x0002
        .equiv SISEL1,  0x0003
        .equiv SISEL2,  0x0004
        .equiv SRXMPT,  0x0005
        .equiv SPIROV,  0x0006
        .equiv SRMPT,   0x0007
        .equiv SPIBEC0, 0x0008
        .equiv SPIBEC1, 0x0009
        .equiv SPIBEC2, 0x000A
        .equiv SPISIDL, 0x000D
        .equiv SPIEN,   0x000F



;----- SPI1CON1 Bits -----------------------------------------------------
        .equiv PPRE0,   0x0000
        .equiv PPRE1,   0x0001
        .equiv SPRE0,   0x0002
        .equiv SPRE1,   0x0003
        .equiv SPRE2,   0x0004
        .equiv MSTEN,   0x0005
        .equiv CKP,     0x0006
        .equiv SSEN,    0x0007
        .equiv CKE,     0x0008
        .equiv SMP,     0x0009
        .equiv MODE16,  0x000A
        .equiv DISSDO,  0x000B
        .equiv DISSCK,  0x000C



;----- SPI1CON2 Bits -----------------------------------------------------
        .equiv SPIBEN,  0x0000
        .equiv SPIFE,   0x0001
        .equiv SPIFPOL, 0x000D
        .equiv SPIFSD,  0x000E
        .equiv FRMEN,   0x000F


;----- U3MODE Bits -----------------------------------------------------
       ;.equiv STSEL,   0x0000
       ;.equiv PDSEL0,  0x0001
       ;.equiv PDSEL1,  0x0002
       ;.equiv BRGH,    0x0003
       ;.equiv RXINV,   0x0004
       ;.equiv ABAUD,   0x0005
       ;.equiv LPBACK,  0x0006
       ;.equiv WAKE,    0x0007
       ;.equiv UEN0,    0x0008
       ;.equiv UEN1,    0x0009
       ;.equiv RTSMD,   0x000B
       ;.equiv IREN,    0x000C
       ;.equiv USIDL,   0x000D
       ;.equiv UARTEN,  0x000F



;----- U3STA Bits -----------------------------------------------------
       ;.equiv URXDA,   0x0000
       ;.equiv OERR,    0x0001
       ;.equiv FERR,    0x0002
       ;.equiv PERR,    0x0003
       ;.equiv RIDLE,   0x0004
       ;.equiv ADDEN,   0x0005
       ;.equiv URXISEL0, 0x0006
       ;.equiv URXISEL1, 0x0007
       ;.equiv TRMT,    0x0008
       ;.equiv UTXBF,   0x0009
       ;.equiv UTXEN,   0x000A
       ;.equiv UTXBRK,  0x000B
       ;.equiv UTXISEL0, 0x000D
       ;.equiv UTXINV,  0x000E
       ;.equiv UTXISEL1, 0x000F



;----- SPI2STAT Bits -----------------------------------------------------
       ;.equiv SPIRBF,  0x0000
       ;.equiv SPITBF,  0x0001
       ;.equiv SISEL0,  0x0002
       ;.equiv SISEL1,  0x0003
       ;.equiv SISEL2,  0x0004
       ;.equiv SRXMPT,  0x0005
       ;.equiv SPIROV,  0x0006
       ;.equiv SRMPT,   0x0007
       ;.equiv SPIBEC0, 0x0008
       ;.equiv SPIBEC1, 0x0009
       ;.equiv SPIBEC2, 0x000A
       ;.equiv SPISIDL, 0x000D
       ;.equiv SPIEN,   0x000F



;----- SPI2CON1 Bits -----------------------------------------------------
       ;.equiv PPRE0,   0x0000
       ;.equiv PPRE1,   0x0001
       ;.equiv SPRE0,   0x0002
       ;.equiv SPRE1,   0x0003
       ;.equiv SPRE2,   0x0004
       ;.equiv MSTEN,   0x0005
       ;.equiv CKP,     0x0006
       ;.equiv SSEN,    0x0007
       ;.equiv CKE,     0x0008
       ;.equiv SMP,     0x0009
       ;.equiv MODE16,  0x000A
       ;.equiv DISSDO,  0x000B
       ;.equiv DISSCK,  0x000C



;----- SPI2CON2 Bits -----------------------------------------------------
       ;.equiv SPIBEN,  0x0000
       ;.equiv SPIFE,   0x0001
       ;.equiv SPIFPOL, 0x000D
       ;.equiv SPIFSD,  0x000E
       ;.equiv FRMEN,   0x000F


;----- I2C3CON Bits -----------------------------------------------------
       ;.equiv SEN,     0x0000
       ;.equiv RSEN,    0x0001
       ;.equiv PEN,     0x0002
       ;.equiv RCEN,    0x0003
       ;.equiv ACKEN,   0x0004
       ;.equiv ACKDT,   0x0005
       ;.equiv STREN,   0x0006
       ;.equiv GCEN,    0x0007
       ;.equiv SMEN,    0x0008
       ;.equiv DISSLW,  0x0009
       ;.equiv A10M,    0x000A
       ;.equiv IPMIEN,  0x000B
       ;.equiv SCLREL,  0x000C
       ;.equiv I2CSIDL, 0x000D
       ;.equiv I2CEN,   0x000F


;----- I2C3STAT Bits -----------------------------------------------------
       ;.equiv TBF,     0x0000
       ;.equiv RBF,     0x0001
       ;.equiv R_W,     0x0002
       ;.equiv S,       0x0003
       ;.equiv P,       0x0004
       ;.equiv D_A,     0x0005
       ;.equiv I2COV,   0x0006
       ;.equiv IWCOL,   0x0007
       ;.equiv ADD10,   0x0008
       ;.equiv GCSTAT,  0x0009
       ;.equiv BCL,     0x000A
       ;.equiv TRSTAT,  0x000E
       ;.equiv ACKSTAT, 0x000F


;----- SPI3STAT Bits -----------------------------------------------------
       ;.equiv SPIRBF,  0x0000
       ;.equiv SPITBF,  0x0001
       ;.equiv SISEL0,  0x0002
       ;.equiv SISEL1,  0x0003
       ;.equiv SISEL2,  0x0004
       ;.equiv SRXMPT,  0x0005
       ;.equiv SPIROV,  0x0006
       ;.equiv SRMPT,   0x0007
       ;.equiv SPIBEC0, 0x0008
       ;.equiv SPIBEC1, 0x0009
       ;.equiv SPIBEC2, 0x000A
       ;.equiv SPISIDL, 0x000D
       ;.equiv SPIEN,   0x000F



;----- SPI3CON1 Bits -----------------------------------------------------
       ;.equiv PPRE0,   0x0000
       ;.equiv PPRE1,   0x0001
       ;.equiv SPRE0,   0x0002
       ;.equiv SPRE1,   0x0003
       ;.equiv SPRE2,   0x0004
       ;.equiv MSTEN,   0x0005
       ;.equiv CKP,     0x0006
       ;.equiv SSEN,    0x0007
       ;.equiv CKE,     0x0008
       ;.equiv SMP,     0x0009
       ;.equiv MODE16,  0x000A
       ;.equiv DISSDO,  0x000B
       ;.equiv DISSCK,  0x000C



;----- SPI3CON2 Bits -----------------------------------------------------
       ;.equiv SPIBEN,  0x0000
       ;.equiv SPIFE,   0x0001
       ;.equiv SPIFPOL, 0x000D
       ;.equiv SPIFSD,  0x000E
       ;.equiv FRMEN,   0x000F


;----- U4MODE Bits -----------------------------------------------------
       ;.equiv STSEL,   0x0000
       ;.equiv PDSEL0,  0x0001
       ;.equiv PDSEL1,  0x0002
       ;.equiv BRGH,    0x0003
       ;.equiv RXINV,   0x0004
       ;.equiv ABAUD,   0x0005
       ;.equiv LPBACK,  0x0006
       ;.equiv WAKE,    0x0007
       ;.equiv UEN0,    0x0008
       ;.equiv UEN1,    0x0009
       ;.equiv RTSMD,   0x000B
       ;.equiv IREN,    0x000C
       ;.equiv USIDL,   0x000D
       ;.equiv UARTEN,  0x000F



;----- U4STA Bits -----------------------------------------------------
       ;.equiv URXDA,   0x0000
       ;.equiv OERR,    0x0001
       ;.equiv FERR,    0x0002
       ;.equiv PERR,    0x0003
       ;.equiv RIDLE,   0x0004
       ;.equiv ADDEN,   0x0005
       ;.equiv URXISEL0, 0x0006
       ;.equiv URXISEL1, 0x0007
       ;.equiv TRMT,    0x0008
       ;.equiv UTXBF,   0x0009
       ;.equiv UTXEN,   0x000A
       ;.equiv UTXBRK,  0x000B
       ;.equiv UTXISEL0, 0x000D
       ;.equiv UTXINV,  0x000E
       ;.equiv UTXISEL1, 0x000F



;----- TRISB Bits -----------------------------------------------------
        .equiv TRISB0,  0x0000
        .equiv TRISB1,  0x0001
        .equiv TRISB2,  0x0002
        .equiv TRISB3,  0x0003
        .equiv TRISB4,  0x0004
        .equiv TRISB5,  0x0005
        .equiv TRISB6,  0x0006
        .equiv TRISB7,  0x0007
        .equiv TRISB8,  0x0008
        .equiv TRISB9,  0x0009
        .equiv TRISB10, 0x000A
        .equiv TRISB11, 0x000B
        .equiv TRISB12, 0x000C
        .equiv TRISB13, 0x000D
        .equiv TRISB14, 0x000E
        .equiv TRISB15, 0x000F


;----- PORTB Bits -----------------------------------------------------
        .equiv RB0,     0x0000
        .equiv RB1,     0x0001
        .equiv RB2,     0x0002
        .equiv RB3,     0x0003
        .equiv RB4,     0x0004
        .equiv RB5,     0x0005
        .equiv RB6,     0x0006
        .equiv RB7,     0x0007
        .equiv RB8,     0x0008
        .equiv RB9,     0x0009
        .equiv RB10,    0x000A
        .equiv RB11,    0x000B
        .equiv RB12,    0x000C
        .equiv RB13,    0x000D
        .equiv RB14,    0x000E
        .equiv RB15,    0x000F


;----- LATB Bits -----------------------------------------------------
        .equiv LATB0,   0x0000
        .equiv LATB1,   0x0001
        .equiv LATB2,   0x0002
        .equiv LATB3,   0x0003
        .equiv LATB4,   0x0004
        .equiv LATB5,   0x0005
        .equiv LATB6,   0x0006
        .equiv LATB7,   0x0007
        .equiv LATB8,   0x0008
        .equiv LATB9,   0x0009
        .equiv LATB10,  0x000A
        .equiv LATB11,  0x000B
        .equiv LATB12,  0x000C
        .equiv LATB13,  0x000D
        .equiv LATB14,  0x000E
        .equiv LATB15,  0x000F


;----- ODCB Bits -----------------------------------------------------
        .equiv ODB0,    0x0000
        .equiv ODB1,    0x0001
        .equiv ODB2,    0x0002
        .equiv ODB3,    0x0003
        .equiv ODB4,    0x0004
        .equiv ODB5,    0x0005
        .equiv ODB6,    0x0006
        .equiv ODB7,    0x0007
        .equiv ODB8,    0x0008
        .equiv ODB9,    0x0009
        .equiv ODB10,   0x000A
        .equiv ODB11,   0x000B
        .equiv ODB12,   0x000C
        .equiv ODB13,   0x000D
        .equiv ODB14,   0x000E
        .equiv ODB15,   0x000F


;----- TRISC Bits -----------------------------------------------------
        .equiv TRISC12, 0x000C
        .equiv TRISC13, 0x000D
        .equiv TRISC14, 0x000E
        .equiv TRISC15, 0x000F


;----- PORTC Bits -----------------------------------------------------
        .equiv RC12,    0x000C
        .equiv RC13,    0x000D
        .equiv RC14,    0x000E
        .equiv RC15,    0x000F


;----- LATC Bits -----------------------------------------------------
        .equiv LATC12,  0x000C
        .equiv LATC13,  0x000D
        .equiv LATC14,  0x000E
        .equiv LATC15,  0x000F


;----- ODCC Bits -----------------------------------------------------
        .equiv ODC12,   0x000C
        .equiv ODC13,   0x000D
        .equiv ODC14,   0x000E
        .equiv ODC15,   0x000F


;----- TRISD Bits -----------------------------------------------------
        .equiv TRISD0,  0x0000
        .equiv TRISD1,  0x0001
        .equiv TRISD2,  0x0002
        .equiv TRISD3,  0x0003
        .equiv TRISD4,  0x0004
        .equiv TRISD5,  0x0005
        .equiv TRISD6,  0x0006
        .equiv TRISD7,  0x0007
        .equiv TRISD8,  0x0008
        .equiv TRISD9,  0x0009
        .equiv TRISD10, 0x000A
        .equiv TRISD11, 0x000B


;----- PORTD Bits -----------------------------------------------------
        .equiv RD0,     0x0000
        .equiv RD1,     0x0001
        .equiv RD2,     0x0002
        .equiv RD3,     0x0003
        .equiv RD4,     0x0004
        .equiv RD5,     0x0005
        .equiv RD6,     0x0006
        .equiv RD7,     0x0007
        .equiv RD8,     0x0008
        .equiv RD9,     0x0009
        .equiv RD10,    0x000A
        .equiv RD11,    0x000B


;----- LATD Bits -----------------------------------------------------
        .equiv LATD0,   0x0000
        .equiv LATD1,   0x0001
        .equiv LATD2,   0x0002
        .equiv LATD3,   0x0003
        .equiv LATD4,   0x0004
        .equiv LATD5,   0x0005
        .equiv LATD6,   0x0006
        .equiv LATD7,   0x0007
        .equiv LATD8,   0x0008
        .equiv LATD9,   0x0009
        .equiv LATD10,  0x000A
        .equiv LATD11,  0x000B


;----- ODCD Bits -----------------------------------------------------
        .equiv ODD0,    0x0000
        .equiv ODD1,    0x0001
        .equiv ODD2,    0x0002
        .equiv ODD3,    0x0003
        .equiv ODD4,    0x0004
        .equiv ODD5,    0x0005
        .equiv ODD6,    0x0006
        .equiv ODD7,    0x0007
        .equiv ODD8,    0x0008
        .equiv ODD9,    0x0009
        .equiv ODD10,   0x000A
        .equiv ODD11,   0x000B


;----- TRISE Bits -----------------------------------------------------
        .equiv TRISE0,  0x0000
        .equiv TRISE1,  0x0001
        .equiv TRISE2,  0x0002
        .equiv TRISE3,  0x0003
        .equiv TRISE4,  0x0004
        .equiv TRISE5,  0x0005
        .equiv TRISE6,  0x0006


;----- PORTE Bits -----------------------------------------------------
        .equiv RE0,     0x0000
        .equiv RE1,     0x0001
        .equiv RE2,     0x0002
        .equiv RE3,     0x0003
        .equiv RE4,     0x0004
        .equiv RE5,     0x0005
        .equiv RE6,     0x0006


;----- LATE Bits -----------------------------------------------------
        .equiv LATE0,   0x0000
        .equiv LATE1,   0x0001
        .equiv LATE2,   0x0002
        .equiv LATE3,   0x0003
        .equiv LATE4,   0x0004
        .equiv LATE5,   0x0005
        .equiv LATE6,   0x0006


;----- ODCE Bits -----------------------------------------------------
        .equiv ODE0,    0x0000
        .equiv ODE1,    0x0001
        .equiv ODE2,    0x0002
        .equiv ODE3,    0x0003
        .equiv ODE4,    0x0004
        .equiv ODE5,    0x0005
        .equiv ODE6,    0x0006


;----- TRISF Bits -----------------------------------------------------
        .equiv TRISF0,  0x0000
        .equiv TRISF1,  0x0001
        .equiv TRISF3,  0x0003
        .equiv TRISF4,  0x0004
        .equiv TRISF5,  0x0005


;----- PORTF Bits -----------------------------------------------------
        .equiv RF0,     0x0000
        .equiv RF1,     0x0001
        .equiv RF3,     0x0003
        .equiv RF4,     0x0004
        .equiv RF5,     0x0005


;----- LATF Bits -----------------------------------------------------
        .equiv LATF0,   0x0000
        .equiv LATF1,   0x0001
        .equiv LATF3,   0x0003
        .equiv LATF4,   0x0004
        .equiv LATF5,   0x0005


;----- ODCF Bits -----------------------------------------------------
        .equiv ODF0,    0x0000
        .equiv ODF1,    0x0001
        .equiv ODF3,    0x0003
        .equiv ODF4,    0x0004
        .equiv ODF5,    0x0005


;----- TRISG Bits -----------------------------------------------------
        .equiv TRISG2,  0x0002
        .equiv TRISG3,  0x0003
        .equiv TRISG6,  0x0006
        .equiv TRISG7,  0x0007
        .equiv TRISG8,  0x0008
        .equiv TRISG9,  0x0009


;----- PORTG Bits -----------------------------------------------------
        .equiv RG2,     0x0002
        .equiv RG3,     0x0003
        .equiv RG6,     0x0006
        .equiv RG7,     0x0007
        .equiv RG8,     0x0008
        .equiv RG9,     0x0009


;----- LATG Bits -----------------------------------------------------
        .equiv LATG2,   0x0002
        .equiv LATG3,   0x0003
        .equiv LATG6,   0x0006
        .equiv LATG7,   0x0007
        .equiv LATG8,   0x0008
        .equiv LATG9,   0x0009


;----- ODCG Bits -----------------------------------------------------
        .equiv ODG2,    0x0002
        .equiv ODG3,    0x0003
        .equiv ODG6,    0x0006
        .equiv ODG7,    0x0007
        .equiv ODG8,    0x0008
        .equiv ODG9,    0x0009


;----- PADCFG1 Bits -----------------------------------------------------
        .equiv PMPTTL,  0x0000
        .equiv RTSECSEL, 0x0001

        .equiv RTSECSEL0, 0x0001


;----- AD1CON1 Bits -----------------------------------------------------
        .equiv DONE,    0x0000
        .equiv SAMP,    0x0001
        .equiv ASAM,    0x0002
        .equiv SSRC0,   0x0005
        .equiv SSRC1,   0x0006
        .equiv SSRC2,   0x0007
        .equiv FORM0,   0x0008
        .equiv FORM1,   0x0009
        .equiv ADSIDL,  0x000D
        .equiv ADON,    0x000F



;----- AD1CON2 Bits -----------------------------------------------------
        .equiv ALTS,    0x0000
        .equiv BUFM,    0x0001
        .equiv SMPI0,   0x0002
        .equiv SMPI1,   0x0003
        .equiv SMPI2,   0x0004
        .equiv SMPI3,   0x0005
        .equiv BUFS,    0x0007
        .equiv CSCNA,   0x000A
        .equiv OFFCAL,  0x000C
        .equiv VCFG0,   0x000D
        .equiv VCFG1,   0x000E
        .equiv VCFG2,   0x000F



;----- AD1CON3 Bits -----------------------------------------------------
        .equiv ADCS0,   0x0000
        .equiv ADCS1,   0x0001
        .equiv ADCS2,   0x0002
        .equiv ADCS3,   0x0003
        .equiv ADCS4,   0x0004
        .equiv ADCS5,   0x0005
        .equiv ADCS6,   0x0006
        .equiv ADCS7,   0x0007
        .equiv SAMC0,   0x0008
        .equiv SAMC1,   0x0009
        .equiv SAMC2,   0x000A
        .equiv SAMC3,   0x000B
        .equiv SAMC4,   0x000C
        .equiv ADRC,    0x000F



;----- AD1CHS Bits -----------------------------------------------------
        .equiv CH0SA0,  0x0000
        .equiv CH0SA1,  0x0001
        .equiv CH0SA2,  0x0002
        .equiv CH0SA3,  0x0003
        .equiv CH0SA4,  0x0004
        .equiv CH0NA,   0x0007
        .equiv CH0SB0,  0x0008
        .equiv CH0SB1,  0x0009
        .equiv CH0SB2,  0x000A
        .equiv CH0SB3,  0x000B
        .equiv CH0SB4,  0x000C
        .equiv CH0NB,   0x000F



;----- AD1PCFGH Bits -----------------------------------------------------
        .equiv PCFG16,  0x0000
        .equiv PCFG17,  0x0001


;----- AD1PCFGL Bits -----------------------------------------------------
        .equiv PCFG0,   0x0000
        .equiv PCFG1,   0x0001
        .equiv PCFG2,   0x0002
        .equiv PCFG3,   0x0003
        .equiv PCFG4,   0x0004
        .equiv PCFG5,   0x0005
        .equiv PCFG6,   0x0006
        .equiv PCFG7,   0x0007
        .equiv PCFG8,   0x0008
        .equiv PCFG9,   0x0009
        .equiv PCFG10,  0x000A
        .equiv PCFG11,  0x000B
        .equiv PCFG12,  0x000C
        .equiv PCFG13,  0x000D
        .equiv PCFG14,  0x000E
        .equiv PCFG15,  0x000F


;----- AD1CSSL Bits -----------------------------------------------------
        .equiv CSSL0,   0x0000
        .equiv CSSL1,   0x0001
        .equiv CSSL2,   0x0002
        .equiv CSSL3,   0x0003
        .equiv CSSL4,   0x0004
        .equiv CSSL5,   0x0005
        .equiv CSSL6,   0x0006
        .equiv CSSL7,   0x0007
        .equiv CSSL8,   0x0008
        .equiv CSSL9,   0x0009
        .equiv CSSL10,  0x000A
        .equiv CSSL11,  0x000B
        .equiv CSSL12,  0x000C
        .equiv CSSL13,  0x000D
        .equiv CSSL14,  0x000E
        .equiv CSSL15,  0x000F


;----- AD1CSSH Bits -----------------------------------------------------
        .equiv CSSL16,  0x0000
        .equiv CSSL17,  0x0001


;----- CTMUCON Bits -----------------------------------------------------
        .equiv EDG1STAT, 0x0000
        .equiv EDG2STAT, 0x0001
        .equiv EDG1SEL0, 0x0002
        .equiv EDG1SEL1, 0x0003
        .equiv EDG1POL, 0x0004
        .equiv EDG2SEL0, 0x0005
        .equiv EDG2SEL1, 0x0006
        .equiv EDG2POL, 0x0007
        .equiv CTTRIG,  0x0008
        .equiv IDISSEN, 0x0009
        .equiv EDGSEQEN, 0x000A
        .equiv EDGEN,   0x000B
        .equiv TGEN,    0x000C
        .equiv CTMUSIDL, 0x000D
        .equiv CTMUEN,  0x000F



;----- CTMUICON Bits -----------------------------------------------------
        .equiv IRNG0,   0x0008
        .equiv IRNG1,   0x0009
        .equiv ITRIM0,  0x000A
        .equiv ITRIM1,  0x000B
        .equiv ITRIM2,  0x000C
        .equiv ITRIM3,  0x000D
        .equiv ITRIM4,  0x000E
        .equiv ITRIM5,  0x000F



;----- U1OTGIR Bits -----------------------------------------------------
        .equiv VBUSVDIF, 0x0000
        .equiv SESENDIF, 0x0002
        .equiv SESVDIF, 0x0003
        .equiv ACTVIF,  0x0004
        .equiv LSTATEIF, 0x0005
        .equiv T1MSECIF, 0x0006
        .equiv IDIF,    0x0007


;----- U1OTGIE Bits -----------------------------------------------------
        .equiv VBUSVDIE, 0x0000
        .equiv SESENDIE, 0x0002
        .equiv SESVDIE, 0x0003
        .equiv ACTVIE,  0x0004
        .equiv LSTATEIE, 0x0005
        .equiv T1MSECIE, 0x0006
        .equiv IDIE,    0x0007


;----- U1OTGSTAT Bits -----------------------------------------------------
        .equiv VBUSVD,  0x0000
        .equiv SESEND,  0x0002
        .equiv SESVD,   0x0003
        .equiv LSTATE,  0x0005
        .equiv ID,      0x0007


;----- U1OTGCON Bits -----------------------------------------------------
        .equiv VBUSDIS, 0x0000
        .equiv VBUSCHG, 0x0001
        .equiv OTGEN,   0x0002
        .equiv VBUSON,  0x0003
        .equiv DMPULDWN, 0x0004
        .equiv DPPULDWN, 0x0005
        .equiv DMPULUP, 0x0006
        .equiv DPPULUP, 0x0007


;----- U1PWRC Bits -----------------------------------------------------
        .equiv USBPWR,  0x0000
        .equiv USUSPEND, 0x0001
        .equiv USLPGRD, 0x0004
        .equiv UACTPND, 0x0007


;----- U1IR Bits -----------------------------------------------------
        .equiv DETACHIF, 0x0000
        .equiv UERRIF,  0x0001
        .equiv SOFIF,   0x0002
        .equiv TRNIF,   0x0003
        .equiv IDLEIF,  0x0004
        .equiv RESUMEIF, 0x0005
        .equiv ATTACHIF, 0x0006
        .equiv STALLIF, 0x0007

        .equiv URSTIF,  0x0000


;----- U1IE Bits -----------------------------------------------------
        .equiv DETACHIE, 0x0000
        .equiv UERRIE,  0x0001
        .equiv SOFIE,   0x0002
        .equiv TRNIE,   0x0003
        .equiv IDLEIE,  0x0004
        .equiv RESUMEIE, 0x0005
        .equiv ATTACHIE, 0x0006
        .equiv STALLIE, 0x0007

        .equiv URSTIE,  0x0000


;----- U1EIR Bits -----------------------------------------------------
        .equiv PIDEF,   0x0000
        .equiv CRC5EF,  0x0001
        .equiv CRC16EF, 0x0002
        .equiv DFN8EF,  0x0003
        .equiv BTOEF,   0x0004
        .equiv DMAEF,   0x0005
        .equiv BTSEF,   0x0007

        .equiv EOFEF,   0x0001


;----- U1EIE Bits -----------------------------------------------------
        .equiv PIDEE,   0x0000
        .equiv CRC5EE,  0x0001
        .equiv CRC16EE, 0x0002
        .equiv DFN8EE,  0x0003
        .equiv BTOEE,   0x0004
        .equiv DMAEE,   0x0005
        .equiv BTSEE,   0x0007

        .equiv EOFEE,   0x0001


;----- U1STAT Bits -----------------------------------------------------
        .equiv PPBI,    0x0002
        .equiv DIR,     0x0003
        .equiv ENDPT0,  0x0004
        .equiv ENDPT1,  0x0005
        .equiv ENDPT2,  0x0006
        .equiv ENDPT3,  0x0007



;----- U1CON Bits -----------------------------------------------------
        .equiv USBEN,   0x0000
        .equiv PPBRST,  0x0001
        .equiv RESUME,  0x0002
        .equiv HOSTEN,  0x0003
        .equiv USBRST,  0x0004
        .equiv PKTDIS,  0x0005
        .equiv SE0,     0x0006
        .equiv JSTATE,  0x0007

        .equiv SOFEN,   0x0000
        .equiv TOKBUSY, 0x0005


;----- U1ADDR Bits -----------------------------------------------------
        .equiv DEVADDR0, 0x0000
        .equiv DEVADDR1, 0x0001
        .equiv DEVADDR2, 0x0002
        .equiv DEVADDR3, 0x0003
        .equiv DEVADDR4, 0x0004
        .equiv DEVADDR5, 0x0005
        .equiv DEVADDR6, 0x0006
        .equiv LOWSPDEN, 0x0007

        .equiv LSPDEN,  0x0007


;----- U1FRML Bits -----------------------------------------------------
        .equiv FRM0,    0x0000
        .equiv FRM1,    0x0001
        .equiv FRM2,    0x0002
        .equiv FRM3,    0x0003
        .equiv FRM4,    0x0004
        .equiv FRM5,    0x0005
        .equiv FRM6,    0x0006
        .equiv FRM7,    0x0007


;----- U1FRMH Bits -----------------------------------------------------
        .equiv FRM8,    0x0000
        .equiv FRM9,    0x0001
        .equiv FRM10,   0x0002


;----- U1TOK Bits -----------------------------------------------------
        .equiv EP0,     0x0000
        .equiv EP1,     0x0001
        .equiv EP2,     0x0002
        .equiv EP3,     0x0003
        .equiv PID0,    0x0004
        .equiv PID1,    0x0005
        .equiv PID2,    0x0006
        .equiv PID3,    0x0007



;----- U1CNFG1 Bits -----------------------------------------------------
        .equiv PPB0,    0x0000
        .equiv PPB1,    0x0001
        .equiv USBSIDL, 0x0004
        .equiv UOEMON,  0x0006
        .equiv UTEYE,   0x0007



;----- U1CNFG2 Bits -----------------------------------------------------
        .equiv UTRDIS,  0x0000
        .equiv UVCMPDIS, 0x0001
        .equiv UVBUSDIS, 0x0002
        .equiv EXTI2CEN, 0x0003


;----- U1EP0 Bits -----------------------------------------------------
        .equiv EPHSHK,  0x0000
        .equiv EPSTALL, 0x0001
        .equiv EPINEN,  0x0002
        .equiv EPOUTEN, 0x0003
        .equiv EPCONDIS, 0x0004
        .equiv RETRYDIS, 0x0006
        .equiv LOWSPD,  0x0007

        .equiv LSPD,    0x0007


;----- U1EP1 Bits -----------------------------------------------------
       ;.equiv EPHSHK,  0x0000
       ;.equiv EPSTALL, 0x0001
       ;.equiv EPINEN,  0x0002
       ;.equiv EPOUTEN, 0x0003
       ;.equiv EPCONDIS, 0x0004


;----- U1EP2 Bits -----------------------------------------------------
       ;.equiv EPHSHK,  0x0000
       ;.equiv EPSTALL, 0x0001
       ;.equiv EPINEN,  0x0002
       ;.equiv EPOUTEN, 0x0003
       ;.equiv EPCONDIS, 0x0004


;----- U1EP3 Bits -----------------------------------------------------
       ;.equiv EPHSHK,  0x0000
       ;.equiv EPSTALL, 0x0001
       ;.equiv EPINEN,  0x0002
       ;.equiv EPOUTEN, 0x0003
       ;.equiv EPCONDIS, 0x0004


;----- U1EP4 Bits -----------------------------------------------------
       ;.equiv EPHSHK,  0x0000
       ;.equiv EPSTALL, 0x0001
       ;.equiv EPINEN,  0x0002
       ;.equiv EPOUTEN, 0x0003
       ;.equiv EPCONDIS, 0x0004


;----- U1EP5 Bits -----------------------------------------------------
       ;.equiv EPHSHK,  0x0000
       ;.equiv EPSTALL, 0x0001
       ;.equiv EPINEN,  0x0002
       ;.equiv EPOUTEN, 0x0003
       ;.equiv EPCONDIS, 0x0004


;----- U1EP6 Bits -----------------------------------------------------
       ;.equiv EPHSHK,  0x0000
       ;.equiv EPSTALL, 0x0001
       ;.equiv EPINEN,  0x0002
       ;.equiv EPOUTEN, 0x0003
       ;.equiv EPCONDIS, 0x0004


;----- U1EP7 Bits -----------------------------------------------------
       ;.equiv EPHSHK,  0x0000
       ;.equiv EPSTALL, 0x0001
       ;.equiv EPINEN,  0x0002
       ;.equiv EPOUTEN, 0x0003
       ;.equiv EPCONDIS, 0x0004


;----- U1EP8 Bits -----------------------------------------------------
       ;.equiv EPHSHK,  0x0000
       ;.equiv EPSTALL, 0x0001
       ;.equiv EPINEN,  0x0002
       ;.equiv EPOUTEN, 0x0003
       ;.equiv EPCONDIS, 0x0004


;----- U1EP9 Bits -----------------------------------------------------
       ;.equiv EPHSHK,  0x0000
       ;.equiv EPSTALL, 0x0001
       ;.equiv EPINEN,  0x0002
       ;.equiv EPOUTEN, 0x0003
       ;.equiv EPCONDIS, 0x0004


;----- U1EP10 Bits -----------------------------------------------------
       ;.equiv EPHSHK,  0x0000
       ;.equiv EPSTALL, 0x0001
       ;.equiv EPINEN,  0x0002
       ;.equiv EPOUTEN, 0x0003
       ;.equiv EPCONDIS, 0x0004


;----- U1EP11 Bits -----------------------------------------------------
       ;.equiv EPHSHK,  0x0000
       ;.equiv EPSTALL, 0x0001
       ;.equiv EPINEN,  0x0002
       ;.equiv EPOUTEN, 0x0003
       ;.equiv EPCONDIS, 0x0004


;----- U1EP12 Bits -----------------------------------------------------
       ;.equiv EPHSHK,  0x0000
       ;.equiv EPSTALL, 0x0001
       ;.equiv EPINEN,  0x0002
       ;.equiv EPOUTEN, 0x0003
       ;.equiv EPCONDIS, 0x0004


;----- U1EP13 Bits -----------------------------------------------------
       ;.equiv EPHSHK,  0x0000
       ;.equiv EPSTALL, 0x0001
       ;.equiv EPINEN,  0x0002
       ;.equiv EPOUTEN, 0x0003
       ;.equiv EPCONDIS, 0x0004


;----- U1EP14 Bits -----------------------------------------------------
       ;.equiv EPHSHK,  0x0000
       ;.equiv EPSTALL, 0x0001
       ;.equiv EPINEN,  0x0002
       ;.equiv EPOUTEN, 0x0003
       ;.equiv EPCONDIS, 0x0004

        .equiv EPTXEN,  0x0002
        .equiv EPRXEN,  0x0003


;----- U1EP15 Bits -----------------------------------------------------
       ;.equiv EPHSHK,  0x0000
       ;.equiv EPSTALL, 0x0001
       ;.equiv EPINEN,  0x0002
       ;.equiv EPOUTEN, 0x0003
       ;.equiv EPCONDIS, 0x0004


;----- U1PWMRRS Bits -----------------------------------------------------
        .equiv USBRS0,  0x0000
        .equiv USBRS1,  0x0001
        .equiv USBRS2,  0x0002
        .equiv USBRS3,  0x0003
        .equiv USBRS4,  0x0004
        .equiv USBRS5,  0x0005
        .equiv USBRS6,  0x0006
        .equiv USBRS7,  0x0007
        .equiv USBR0,   0x0008
        .equiv USBR1,   0x0009
        .equiv USBR2,   0x000A
        .equiv USBR3,   0x000B
        .equiv USBR4,   0x000C
        .equiv USBR5,   0x000D
        .equiv USBR6,   0x000E
        .equiv USBR7,   0x000F



        .equiv PER0,    0x0000
        .equiv PER1,    0x0001
        .equiv PER2,    0x0002
        .equiv PER3,    0x0003
        .equiv PER4,    0x0004
        .equiv PER5,    0x0005
        .equiv PER6,    0x0006
        .equiv PER7,    0x0007
        .equiv DC0,     0x0008
        .equiv DC1,     0x0009
        .equiv DC2,     0x000A
        .equiv DC3,     0x000B
        .equiv DC4,     0x000C
        .equiv DC5,     0x000D
        .equiv DC6,     0x000E
        .equiv DC7,     0x000F


;----- U1PWMCON Bits -----------------------------------------------------
        .equiv CNTEN,   0x0008
        .equiv PWMPOL,  0x0009
        .equiv USBTSEL0, 0x000A
        .equiv USBTSEL1, 0x000B
        .equiv USBTSEL2, 0x000C
        .equiv PWMEN,   0x000F



;----- PMCON Bits -----------------------------------------------------
        .equiv RDSP,    0x0000
        .equiv WRSP,    0x0001
        .equiv BEP,     0x0002
        .equiv CS1P,    0x0003
        .equiv CS2P,    0x0004
        .equiv ALP,     0x0005
        .equiv CSF0,    0x0006
        .equiv CSF1,    0x0007
        .equiv PTRDEN,  0x0008
        .equiv PTWREN,  0x0009
        .equiv PTBEEN,  0x000A
        .equiv ADRMUX0, 0x000B
        .equiv ADRMUX1, 0x000C
        .equiv PSIDL,   0x000D
        .equiv PMPEN,   0x000F



;----- PMMODE Bits -----------------------------------------------------
        .equiv WAITE0,  0x0000
        .equiv WAITE1,  0x0001
        .equiv WAITM0,  0x0002
        .equiv WAITM1,  0x0003
        .equiv WAITM2,  0x0004
        .equiv WAITM3,  0x0005
        .equiv WAITB0,  0x0006
        .equiv WAITB1,  0x0007
        .equiv MODE0,   0x0008
        .equiv MODE1,   0x0009
       ;.equiv MODE16,  0x000A
        .equiv INCM0,   0x000B
        .equiv INCM1,   0x000C
        .equiv IRQM0,   0x000D
        .equiv IRQM1,   0x000E
        .equiv BUSY,    0x000F



;----- PMADDR Bits -----------------------------------------------------
        .equiv CS1,     0x000E
        .equiv CS2,     0x000F



;----- PMDOUT1 Bits -----------------------------------------------------
       ;.equiv CS1,     0x000E
       ;.equiv CS2,     0x000F



;----- PMAEN Bits -----------------------------------------------------
        .equiv PTEN0,   0x0000
        .equiv PTEN1,   0x0001
        .equiv PTEN2,   0x0002
        .equiv PTEN3,   0x0003
        .equiv PTEN4,   0x0004
        .equiv PTEN5,   0x0005
        .equiv PTEN6,   0x0006
        .equiv PTEN7,   0x0007
        .equiv PTEN8,   0x0008
        .equiv PTEN9,   0x0009
        .equiv PTEN10,  0x000A
        .equiv PTEN11,  0x000B
        .equiv PTEN12,  0x000C
        .equiv PTEN13,  0x000D
        .equiv PTEN14,  0x000E
        .equiv PTEN15,  0x000F


;----- PMSTAT Bits -----------------------------------------------------
        .equiv OB0E,    0x0000
        .equiv OB1E,    0x0001
        .equiv OB2E,    0x0002
        .equiv OB3E,    0x0003
        .equiv OBUF,    0x0006
        .equiv OBE,     0x0007
        .equiv IB0F,    0x0008
        .equiv IB1F,    0x0009
        .equiv IB2F,    0x000A
        .equiv IB3F,    0x000B
        .equiv IBOV,    0x000E
        .equiv IBF,     0x000F


;----- ALCFGRPT Bits -----------------------------------------------------
        .equiv ARPT0,   0x0000
        .equiv ARPT1,   0x0001
        .equiv ARPT2,   0x0002
        .equiv ARPT3,   0x0003
        .equiv ARPT4,   0x0004
        .equiv ARPT5,   0x0005
        .equiv ARPT6,   0x0006
        .equiv ARPT7,   0x0007
        .equiv ALRMPTR0, 0x0008
        .equiv ALRMPTR1, 0x0009
        .equiv AMASK0,  0x000A
        .equiv AMASK1,  0x000B
        .equiv AMASK2,  0x000C
        .equiv AMASK3,  0x000D
        .equiv CHIME,   0x000E
        .equiv ALRMEN,  0x000F



;----- RCFGCAL Bits -----------------------------------------------------
        .equiv CAL0,    0x0000
        .equiv CAL1,    0x0001
        .equiv CAL2,    0x0002
        .equiv CAL3,    0x0003
        .equiv CAL4,    0x0004
        .equiv CAL5,    0x0005
        .equiv CAL6,    0x0006
        .equiv CAL7,    0x0007
        .equiv RTCPTR0, 0x0008
        .equiv RTCPTR1, 0x0009
        .equiv RTCOE,   0x000A
        .equiv HALFSEC, 0x000B
        .equiv RTCSYNC, 0x000C
        .equiv RTCWREN, 0x000D
        .equiv RTCEN,   0x000F



;----- CMSTAT Bits -----------------------------------------------------
        .equiv C1OUT,   0x0000
        .equiv C2OUT,   0x0001
        .equiv C3OUT,   0x0002
        .equiv C4OUT,   0x0003
        .equiv C5OUT,   0x0004
        .equiv C6OUT,   0x0005
        .equiv C1EVT,   0x0008
        .equiv C2EVT,   0x0009
        .equiv C3EVT,   0x000A
        .equiv C4EVT,   0x000B
        .equiv C5EVT,   0x000C
        .equiv C6EVT,   0x000D
        .equiv CMIDL,   0x000F


;----- CVRCON Bits -----------------------------------------------------
        .equiv CVR0,    0x0000
        .equiv CVR1,    0x0001
        .equiv CVR2,    0x0002
        .equiv CVR3,    0x0003
        .equiv CVRSS,   0x0004
        .equiv CVRR,    0x0005
        .equiv CVROE,   0x0006
        .equiv CVREN,   0x0007



;----- CM1CON Bits -----------------------------------------------------
        .equiv CCH0,    0x0000
        .equiv CCH1,    0x0001
        .equiv CREF,    0x0004
        .equiv EVPOL0,  0x0006
        .equiv EVPOL1,  0x0007
        .equiv COUT,    0x0008
        .equiv CEVT,    0x0009
        .equiv CPOL,    0x000D
        .equiv COE,     0x000E
        .equiv CON,     0x000F


        .equiv CEN,     0x000F


;----- CM2CON Bits -----------------------------------------------------
       ;.equiv CCH0,    0x0000
       ;.equiv CCH1,    0x0001
       ;.equiv CREF,    0x0004
       ;.equiv EVPOL0,  0x0006
       ;.equiv EVPOL1,  0x0007
       ;.equiv COUT,    0x0008
       ;.equiv CEVT,    0x0009
       ;.equiv CPOL,    0x000D
       ;.equiv COE,     0x000E
       ;.equiv CON,     0x000F


       ;.equiv CEN,     0x000F


;----- CM3CON Bits -----------------------------------------------------
       ;.equiv CCH0,    0x0000
       ;.equiv CCH1,    0x0001
       ;.equiv CREF,    0x0004
       ;.equiv EVPOL0,  0x0006
       ;.equiv EVPOL1,  0x0007
       ;.equiv COUT,    0x0008
       ;.equiv CEVT,    0x0009
       ;.equiv CPOL,    0x000D
       ;.equiv COE,     0x000E
       ;.equiv CON,     0x000F


       ;.equiv CEN,     0x000F


;----- CRCCON Bits -----------------------------------------------------
        .equiv PLEN0,   0x0000
        .equiv PLEN1,   0x0001
        .equiv PLEN2,   0x0002
        .equiv PLEN3,   0x0003
        .equiv CRCGO,   0x0004
        .equiv CRCMPT,  0x0006
        .equiv CRCFUL,  0x0007
        .equiv VWORD0,  0x0008
        .equiv VWORD1,  0x0009
        .equiv VWORD2,  0x000A
        .equiv VWORD3,  0x000B
        .equiv VWORD4,  0x000C
        .equiv CSIDL,   0x000D



;----- CRCXOR Bits -----------------------------------------------------
        .equiv X1,      0x0001
        .equiv X2,      0x0002
        .equiv X3,      0x0003
        .equiv X4,      0x0004
        .equiv X5,      0x0005
        .equiv X6,      0x0006
        .equiv X7,      0x0007
        .equiv X8,      0x0008
        .equiv X9,      0x0009
        .equiv X10,     0x000A
        .equiv X11,     0x000B
        .equiv X12,     0x000C
        .equiv X13,     0x000D
        .equiv X14,     0x000E
        .equiv X15,     0x000F


;----- RPINR0 Bits -----------------------------------------------------
        .equiv INT1R0,  0x0008
        .equiv INT1R1,  0x0009
        .equiv INT1R2,  0x000A
        .equiv INT1R3,  0x000B
        .equiv INT1R4,  0x000C
        .equiv INT1R5,  0x000D



;----- RPINR1 Bits -----------------------------------------------------
        .equiv INT2R0,  0x0000
        .equiv INT2R1,  0x0001
        .equiv INT2R2,  0x0002
        .equiv INT2R3,  0x0003
        .equiv INT2R4,  0x0004
        .equiv INT2R5,  0x0005
        .equiv INT3R0,  0x0008
        .equiv INT3R1,  0x0009
        .equiv INT3R2,  0x000A
        .equiv INT3R3,  0x000B
        .equiv INT3R4,  0x000C
        .equiv INT3R5,  0x000D



;----- RPINR2 Bits -----------------------------------------------------
        .equiv INT4R0,  0x0000
        .equiv INT4R1,  0x0001
        .equiv INT4R2,  0x0002
        .equiv INT4R3,  0x0003
        .equiv INT4R4,  0x0004
        .equiv INT4R5,  0x0005
        .equiv T1CKR0,  0x0008
        .equiv T1CKR1,  0x0009
        .equiv T1CKR2,  0x000A
        .equiv T1CKR3,  0x000B
        .equiv T1CKR4,  0x000C
        .equiv T1CKR5,  0x000D



;----- RPINR3 Bits -----------------------------------------------------
        .equiv T2CKR0,  0x0000
        .equiv T2CKR1,  0x0001
        .equiv T2CKR2,  0x0002
        .equiv T2CKR3,  0x0003
        .equiv T2CKR4,  0x0004
        .equiv T2CKR5,  0x0005
        .equiv T3CKR0,  0x0008
        .equiv T3CKR1,  0x0009
        .equiv T3CKR2,  0x000A
        .equiv T3CKR3,  0x000B
        .equiv T3CKR4,  0x000C
        .equiv T3CKR5,  0x000D



;----- RPINR4 Bits -----------------------------------------------------
        .equiv T4CKR0,  0x0000
        .equiv T4CKR1,  0x0001
        .equiv T4CKR2,  0x0002
        .equiv T4CKR3,  0x0003
        .equiv T4CKR4,  0x0004
        .equiv T4CKR5,  0x0005
        .equiv T5CKR0,  0x0008
        .equiv T5CKR1,  0x0009
        .equiv T5CKR2,  0x000A
        .equiv T5CKR3,  0x000B
        .equiv T5CKR4,  0x000C
        .equiv T5CKR5,  0x000D



;----- RPINR7 Bits -----------------------------------------------------
        .equiv IC1R0,   0x0000
        .equiv IC1R1,   0x0001
        .equiv IC1R2,   0x0002
        .equiv IC1R3,   0x0003
        .equiv IC1R4,   0x0004
        .equiv IC1R5,   0x0005
        .equiv IC2R0,   0x0008
        .equiv IC2R1,   0x0009
        .equiv IC2R2,   0x000A
        .equiv IC2R3,   0x000B
        .equiv IC2R4,   0x000C
        .equiv IC2R5,   0x000D



;----- RPINR8 Bits -----------------------------------------------------
        .equiv IC3R0,   0x0000
        .equiv IC3R1,   0x0001
        .equiv IC3R2,   0x0002
        .equiv IC3R3,   0x0003
        .equiv IC3R4,   0x0004
        .equiv IC3R5,   0x0005
        .equiv IC4R0,   0x0008
        .equiv IC4R1,   0x0009
        .equiv IC4R2,   0x000A
        .equiv IC4R3,   0x000B
        .equiv IC4R4,   0x000C
        .equiv IC4R5,   0x000D



;----- RPINR9 Bits -----------------------------------------------------
        .equiv IC5R0,   0x0000
        .equiv IC5R1,   0x0001
        .equiv IC5R2,   0x0002
        .equiv IC5R3,   0x0003
        .equiv IC5R4,   0x0004
        .equiv IC5R5,   0x0005
        .equiv IC6R0,   0x0008
        .equiv IC6R1,   0x0009
        .equiv IC6R2,   0x000A
        .equiv IC6R3,   0x000B
        .equiv IC6R4,   0x000C
        .equiv IC6R5,   0x000D



;----- RPINR10 Bits -----------------------------------------------------
        .equiv IC7R0,   0x0000
        .equiv IC7R1,   0x0001
        .equiv IC7R2,   0x0002
        .equiv IC7R3,   0x0003
        .equiv IC7R4,   0x0004
        .equiv IC7R5,   0x0005
        .equiv IC8R0,   0x0008
        .equiv IC8R1,   0x0009
        .equiv IC8R2,   0x000A
        .equiv IC8R3,   0x000B
        .equiv IC8R4,   0x000C
        .equiv IC8R5,   0x000D



;----- RPINR11 Bits -----------------------------------------------------
        .equiv OCFAR0,  0x0000
        .equiv OCFAR1,  0x0001
        .equiv OCFAR2,  0x0002
        .equiv OCFAR3,  0x0003
        .equiv OCFAR4,  0x0004
        .equiv OCFAR5,  0x0005
        .equiv OCFBR0,  0x0008
        .equiv OCFBR1,  0x0009
        .equiv OCFBR2,  0x000A
        .equiv OCFBR3,  0x000B
        .equiv OCFBR4,  0x000C
        .equiv OCFBR5,  0x000D



;----- RPINR15 Bits -----------------------------------------------------
        .equiv IC9R0,   0x0008
        .equiv IC9R1,   0x0009
        .equiv IC9R2,   0x000A
        .equiv IC9R3,   0x000B
        .equiv IC9R4,   0x000C
        .equiv IC9R5,   0x000D



;----- RPINR17 Bits -----------------------------------------------------
        .equiv U3RXR0,  0x0008
        .equiv U3RXR1,  0x0009
        .equiv U3RXR2,  0x000A
        .equiv U3RXR3,  0x000B
        .equiv U3RXR4,  0x000C
        .equiv U3RXR5,  0x000D



;----- RPINR18 Bits -----------------------------------------------------
        .equiv U1RXR0,  0x0000
        .equiv U1RXR1,  0x0001
        .equiv U1RXR2,  0x0002
        .equiv U1RXR3,  0x0003
        .equiv U1RXR4,  0x0004
        .equiv U1RXR5,  0x0005
        .equiv U1CTSR0, 0x0008
        .equiv U1CTSR1, 0x0009
        .equiv U1CTSR2, 0x000A
        .equiv U1CTSR3, 0x000B
        .equiv U1CTSR4, 0x000C
        .equiv U1CTSR5, 0x000D



;----- RPINR19 Bits -----------------------------------------------------
        .equiv U2RXR0,  0x0000
        .equiv U2RXR1,  0x0001
        .equiv U2RXR2,  0x0002
        .equiv U2RXR3,  0x0003
        .equiv U2RXR4,  0x0004
        .equiv U2RXR5,  0x0005
        .equiv U2CTSR0, 0x0008
        .equiv U2CTSR1, 0x0009
        .equiv U2CTSR2, 0x000A
        .equiv U2CTSR3, 0x000B
        .equiv U2CTSR4, 0x000C
        .equiv U2CTSR5, 0x000D



;----- RPINR20 Bits -----------------------------------------------------
        .equiv SDI1R0,  0x0000
        .equiv SDI1R1,  0x0001
        .equiv SDI1R2,  0x0002
        .equiv SDI1R3,  0x0003
        .equiv SDI1R4,  0x0004
        .equiv SDI1R5,  0x0005
        .equiv SCK1R0,  0x0008
        .equiv SCK1R1,  0x0009
        .equiv SCK1R2,  0x000A
        .equiv SCK1R3,  0x000B
        .equiv SCK1R4,  0x000C
        .equiv SCK1R5,  0x000D



;----- RPINR21 Bits -----------------------------------------------------
        .equiv SS1R0,   0x0000
        .equiv SS1R1,   0x0001
        .equiv SS1R2,   0x0002
        .equiv SS1R3,   0x0003
        .equiv SS1R4,   0x0004
        .equiv SS1R5,   0x0005
        .equiv U3CTSR0, 0x0008
        .equiv U3CTSR1, 0x0009
        .equiv U3CTSR2, 0x000A
        .equiv U3CTSR3, 0x000B
        .equiv U3CTSR4, 0x000C
        .equiv U3CTSR5, 0x000D



;----- RPINR22 Bits -----------------------------------------------------
        .equiv SDI2R0,  0x0000
        .equiv SDI2R1,  0x0001
        .equiv SDI2R2,  0x0002
        .equiv SDI2R3,  0x0003
        .equiv SDI2R4,  0x0004
        .equiv SDI2R5,  0x0005
        .equiv SCK2R0,  0x0008
        .equiv SCK2R1,  0x0009
        .equiv SCK2R2,  0x000A
        .equiv SCK2R3,  0x000B
        .equiv SCK2R4,  0x000C
        .equiv SCK2R5,  0x000D



;----- RPINR23 Bits -----------------------------------------------------
        .equiv SS2R0,   0x0000
        .equiv SS2R1,   0x0001
        .equiv SS2R2,   0x0002
        .equiv SS2R3,   0x0003
        .equiv SS2R4,   0x0004
        .equiv SS2R5,   0x0005



;----- RPINR27 Bits -----------------------------------------------------
        .equiv U4RXR0,  0x0000
        .equiv U4RXR1,  0x0001
        .equiv U4RXR2,  0x0002
        .equiv U4RXR3,  0x0003
        .equiv U4RXR4,  0x0004
        .equiv U4RXR5,  0x0005
        .equiv U4CTSR0, 0x0008
        .equiv U4CTSR1, 0x0009
        .equiv U4CTSR2, 0x000A
        .equiv U4CTSR3, 0x000B
        .equiv U4CTSR4, 0x000C
        .equiv U4CTSR5, 0x000D



;----- RPINR28 Bits -----------------------------------------------------
        .equiv SDI3R0,  0x0000
        .equiv SDI3R1,  0x0001
        .equiv SDI3R2,  0x0002
        .equiv SDI3R3,  0x0003
        .equiv SDI3R4,  0x0004
        .equiv SDI3R5,  0x0005
        .equiv SCK3R0,  0x0008
        .equiv SCK3R1,  0x0009
        .equiv SCK3R2,  0x000A
        .equiv SCK3R3,  0x000B
        .equiv SCK3R4,  0x000C
        .equiv SCK3R5,  0x000D



;----- RPINR29 Bits -----------------------------------------------------
        .equiv SS3R0,   0x0000
        .equiv SS3R1,   0x0001
        .equiv SS3R2,   0x0002
        .equiv SS3R3,   0x0003
        .equiv SS3R4,   0x0004
        .equiv SS3R5,   0x0005



;----- RPOR0 Bits -----------------------------------------------------
        .equiv RP0R0,   0x0000
        .equiv RP0R1,   0x0001
        .equiv RP0R2,   0x0002
        .equiv RP0R3,   0x0003
        .equiv RP0R4,   0x0004
        .equiv RP0R5,   0x0005
        .equiv RP1R0,   0x0008
        .equiv RP1R1,   0x0009
        .equiv RP1R2,   0x000A
        .equiv RP1R3,   0x000B
        .equiv RP1R4,   0x000C
        .equiv RP1R5,   0x000D



;----- RPOR1 Bits -----------------------------------------------------
        .equiv RP2R0,   0x0000
        .equiv RP2R1,   0x0001
        .equiv RP2R2,   0x0002
        .equiv RP2R3,   0x0003
        .equiv RP2R4,   0x0004
        .equiv RP2R5,   0x0005
        .equiv RP3R0,   0x0008
        .equiv RP3R1,   0x0009
        .equiv RP3R2,   0x000A
        .equiv RP3R3,   0x000B
        .equiv RP3R4,   0x000C
        .equiv RP3R5,   0x000D



;----- RPOR2 Bits -----------------------------------------------------
        .equiv RP4R0,   0x0000
        .equiv RP4R1,   0x0001
        .equiv RP4R2,   0x0002
        .equiv RP4R3,   0x0003
        .equiv RP4R4,   0x0004
        .equiv RP4R5,   0x0005



;----- RPOR3 Bits -----------------------------------------------------
        .equiv RP6R0,   0x0000
        .equiv RP6R1,   0x0001
        .equiv RP6R2,   0x0002
        .equiv RP6R3,   0x0003
        .equiv RP6R4,   0x0004
        .equiv RP6R5,   0x0005
        .equiv RP7R0,   0x0008
        .equiv RP7R1,   0x0009
        .equiv RP7R2,   0x000A
        .equiv RP7R3,   0x000B
        .equiv RP7R4,   0x000C
        .equiv RP7R5,   0x000D



;----- RPOR4 Bits -----------------------------------------------------
        .equiv RP8R0,   0x0000
        .equiv RP8R1,   0x0001
        .equiv RP8R2,   0x0002
        .equiv RP8R3,   0x0003
        .equiv RP8R4,   0x0004
        .equiv RP8R5,   0x0005
        .equiv RP9R0,   0x0008
        .equiv RP9R1,   0x0009
        .equiv RP9R2,   0x000A
        .equiv RP9R3,   0x000B
        .equiv RP9R4,   0x000C
        .equiv RP9R5,   0x000D



;----- RPOR5 Bits -----------------------------------------------------
        .equiv RP10R0,  0x0000
        .equiv RP10R1,  0x0001
        .equiv RP10R2,  0x0002
        .equiv RP10R3,  0x0003
        .equiv RP10R4,  0x0004
        .equiv RP10R5,  0x0005
        .equiv RP11R0,  0x0008
        .equiv RP11R1,  0x0009
        .equiv RP11R2,  0x000A
        .equiv RP11R3,  0x000B
        .equiv RP11R4,  0x000C
        .equiv RP11R5,  0x000D



;----- RPOR6 Bits -----------------------------------------------------
        .equiv RP12R0,  0x0000
        .equiv RP12R1,  0x0001
        .equiv RP12R2,  0x0002
        .equiv RP12R3,  0x0003
        .equiv RP12R4,  0x0004
        .equiv RP12R5,  0x0005
        .equiv RP13R0,  0x0008
        .equiv RP13R1,  0x0009
        .equiv RP13R2,  0x000A
        .equiv RP13R3,  0x000B
        .equiv RP13R4,  0x000C
        .equiv RP13R5,  0x000D



;----- RPOR7 Bits -----------------------------------------------------
        .equiv RP14R0,  0x0000
        .equiv RP14R1,  0x0001
        .equiv RP14R2,  0x0002
        .equiv RP14R3,  0x0003
        .equiv RP14R4,  0x0004
        .equiv RP14R5,  0x0005



;----- RPOR8 Bits -----------------------------------------------------
        .equiv RP16R0,  0x0000
        .equiv RP16R1,  0x0001
        .equiv RP16R2,  0x0002
        .equiv RP16R3,  0x0003
        .equiv RP16R4,  0x0004
        .equiv RP16R5,  0x0005
        .equiv RP17R0,  0x0008
        .equiv RP17R1,  0x0009
        .equiv RP17R2,  0x000A
        .equiv RP17R3,  0x000B
        .equiv RP17R4,  0x000C
        .equiv RP17R5,  0x000D



;----- RPOR9 Bits -----------------------------------------------------
        .equiv RP18R0,  0x0000
        .equiv RP18R1,  0x0001
        .equiv RP18R2,  0x0002
        .equiv RP18R3,  0x0003
        .equiv RP18R4,  0x0004
        .equiv RP18R5,  0x0005
        .equiv RP19R0,  0x0008
        .equiv RP19R1,  0x0009
        .equiv RP19R2,  0x000A
        .equiv RP19R3,  0x000B
        .equiv RP19R4,  0x000C
        .equiv RP19R5,  0x000D



;----- RPOR10 Bits -----------------------------------------------------
        .equiv RP20R0,  0x0000
        .equiv RP20R1,  0x0001
        .equiv RP20R2,  0x0002
        .equiv RP20R3,  0x0003
        .equiv RP20R4,  0x0004
        .equiv RP20R5,  0x0005
        .equiv RP21R0,  0x0008
        .equiv RP21R1,  0x0009
        .equiv RP21R2,  0x000A
        .equiv RP21R3,  0x000B
        .equiv RP21R4,  0x000C
        .equiv RP21R5,  0x000D



;----- RPOR11 Bits -----------------------------------------------------
        .equiv RP22R0,  0x0000
        .equiv RP22R1,  0x0001
        .equiv RP22R2,  0x0002
        .equiv RP22R3,  0x0003
        .equiv RP22R4,  0x0004
        .equiv RP22R5,  0x0005
        .equiv RP23R0,  0x0008
        .equiv RP23R1,  0x0009
        .equiv RP23R2,  0x000A
        .equiv RP23R3,  0x000B
        .equiv RP23R4,  0x000C
        .equiv RP23R5,  0x000D



;----- RPOR12 Bits -----------------------------------------------------
        .equiv RP24R0,  0x0000
        .equiv RP24R1,  0x0001
        .equiv RP24R2,  0x0002
        .equiv RP24R3,  0x0003
        .equiv RP24R4,  0x0004
        .equiv RP24R5,  0x0005
        .equiv RP25R0,  0x0008
        .equiv RP25R1,  0x0009
        .equiv RP25R2,  0x000A
        .equiv RP25R3,  0x000B
        .equiv RP25R4,  0x000C
        .equiv RP25R5,  0x000D



;----- RPOR13 Bits -----------------------------------------------------
        .equiv RP26R0,  0x0000
        .equiv RP26R1,  0x0001
        .equiv RP26R2,  0x0002
        .equiv RP26R3,  0x0003
        .equiv RP26R4,  0x0004
        .equiv RP26R5,  0x0005
        .equiv RP27R0,  0x0008
        .equiv RP27R1,  0x0009
        .equiv RP27R2,  0x000A
        .equiv RP27R3,  0x000B
        .equiv RP27R4,  0x000C
        .equiv RP27R5,  0x000D



;----- RPOR14 Bits -----------------------------------------------------
        .equiv RP28R0,  0x0000
        .equiv RP28R1,  0x0001
        .equiv RP28R2,  0x0002
        .equiv RP28R3,  0x0003
        .equiv RP28R4,  0x0004
        .equiv RP28R5,  0x0005
        .equiv RP29R0,  0x0008
        .equiv RP29R1,  0x0009
        .equiv RP29R2,  0x000A
        .equiv RP29R3,  0x000B
        .equiv RP29R4,  0x000C
        .equiv RP29R5,  0x000D



;----- RPOR15 Bits -----------------------------------------------------
        .equiv RP30R0,  0x0000
        .equiv RP30R1,  0x0001
        .equiv RP30R2,  0x0002
        .equiv RP30R3,  0x0003
        .equiv RP30R4,  0x0004
        .equiv RP30R5,  0x0005



;----- RCON Bits -----------------------------------------------------
        .equiv POR,     0x0000
        .equiv BOR,     0x0001
        .equiv IDLE,    0x0002
        .equiv SLEEP,   0x0003
        .equiv WDTO,    0x0004
        .equiv SWDTEN,  0x0005
        .equiv SWR,     0x0006
        .equiv EXTR,    0x0007
        .equiv VREGS,   0x0008
        .equiv CM,      0x0009
        .equiv IOPUWR,  0x000E
        .equiv TRAPR,   0x000F


;----- OSCCON Bits -----------------------------------------------------
        .equiv OSWEN,   0x0000
        .equiv SOSCEN,  0x0001
        .equiv POSCEN,  0x0002
        .equiv CF,      0x0003
        .equiv LOCK,    0x0005
        .equiv IOLOCK,  0x0006
        .equiv CLKLOCK, 0x0007
        .equiv NOSC0,   0x0008
        .equiv NOSC1,   0x0009
        .equiv NOSC2,   0x000A
        .equiv COSC0,   0x000C
        .equiv COSC1,   0x000D
        .equiv COSC2,   0x000E


        .equiv LPOSCEN, 0x0001


;----- CLKDIV Bits -----------------------------------------------------
        .equiv USBDOZE0, 0x0006
        .equiv USBDOZE1, 0x0007
        .equiv RCDIV0,  0x0008
        .equiv RCDIV1,  0x0009
        .equiv RCDIV2,  0x000A
        .equiv DOZEN,   0x000B
        .equiv DOZE0,   0x000C
        .equiv DOZE1,   0x000D
        .equiv DOZE2,   0x000E
        .equiv ROI,     0x000F



        .equiv CPDIV0,  0x0006
        .equiv CPDIV1,  0x0007


;----- OSCTUN Bits -----------------------------------------------------
        .equiv TUN0,    0x0000
        .equiv TUN1,    0x0001
        .equiv TUN2,    0x0002
        .equiv TUN3,    0x0003
        .equiv TUN4,    0x0004
        .equiv TUN5,    0x0005



;----- REFOCON Bits -----------------------------------------------------
        .equiv RODIV0,  0x0008
        .equiv RODIV1,  0x0009
        .equiv RODIV2,  0x000A
        .equiv RODIV3,  0x000B
        .equiv ROSEL,   0x000C
        .equiv ROSSLP,  0x000D
        .equiv ROON,    0x000F



;----- NVMCON Bits -----------------------------------------------------
        .equiv NVMOP0,  0x0000
        .equiv NVMOP1,  0x0001
        .equiv NVMOP2,  0x0002
        .equiv NVMOP3,  0x0003
        .equiv ERASE,   0x0006
        .equiv WRERR,   0x000D
        .equiv WREN,    0x000E
        .equiv WR,      0x000F



        .equiv PROGOP0, 0x0000
        .equiv PROGOP1, 0x0001
        .equiv PROGOP2, 0x0002
        .equiv PROGOP3, 0x0003


;----- PMD1 Bits -----------------------------------------------------
        .equiv ADC1MD,  0x0000
        .equiv SPI1MD,  0x0003
        .equiv SPI2MD,  0x0004
        .equiv U1MD,    0x0005
        .equiv U2MD,    0x0006
        .equiv I2C1MD,  0x0007
        .equiv T1MD,    0x000B
        .equiv T2MD,    0x000C
        .equiv T3MD,    0x000D
        .equiv T4MD,    0x000E
        .equiv T5MD,    0x000F


;----- PMD2 Bits -----------------------------------------------------
        .equiv OC1MD,   0x0000
        .equiv OC2MD,   0x0001
        .equiv OC3MD,   0x0002
        .equiv OC4MD,   0x0003
        .equiv OC5MD,   0x0004
        .equiv OC6MD,   0x0005
        .equiv OC7MD,   0x0006
        .equiv OC8MD,   0x0007
        .equiv IC1MD,   0x0008
        .equiv IC2MD,   0x0009
        .equiv IC3MD,   0x000A
        .equiv IC4MD,   0x000B
        .equiv IC5MD,   0x000C
        .equiv IC6MD,   0x000D
        .equiv IC7MD,   0x000E
        .equiv IC8MD,   0x000F


;----- PMD3 Bits -----------------------------------------------------
        .equiv I2C2MD,  0x0001
        .equiv I2C3MD,  0x0002
        .equiv U3MD,    0x0003
        .equiv CRCPMD,  0x0007
        .equiv PMPMD,   0x0008
        .equiv RTCCMD,  0x0009
        .equiv CMPMD,   0x000A

        .equiv CRCMD,   0x0007


;----- PMD4 Bits -----------------------------------------------------
        .equiv USB1MD,  0x0000
        .equiv LVDMD,   0x0001
        .equiv CTMUMD,  0x0002
        .equiv REFOMD,  0x0003
        .equiv U4MD,    0x0005
        .equiv UPWMMD,  0x0006


;----- PMD5 Bits -----------------------------------------------------
        .equiv OC9MD,   0x0000
        .equiv IC9MD,   0x0008


;----- PMD6 Bits -----------------------------------------------------
        .equiv SPI3MD,  0x0000



;==========================================================================
;
;   Configuration Words
;
;   Configuration words exist in Program Space and their locations are
;   defined in the device linker script. They can be set in source code
;   or in the MPLAB IDE. Each configuration word should be specified
;   only once (multiple settings may be combined.)
;
;==========================================================================
;
;  Setting configuration words using macros:
;
;  The following macro named 'config' can be used to set configuration words:
;
        .macro config REG, VALUE
        .pushsection \REG.sec, code
        .global \REG
\REG:   .pword \VALUE
        .popsection
        .endm
;
;  For example, to set CONFIG1 using the macro above, the following line
;  can be pasted only at the beginning of the assembly source code,
;  immediately below the '.include' directive.
;
;        config __CONFIG1, POSCMOD_EC & FCKSM_CSECMD
;
;  Note that the 'config' macro takes two arguments: the first is the name
;  of a configuration word (in this case, __CONFIG1), the second argument
;  is a boolean expression that may include multiple settings.
;  The example above would enable the external clock (POSCMOD_EC)
;  and also enable clock switching (FCKSM_CSECMD).
;
;  A description of all possible settings for each configuration word
;  appears below.
;
;==========================================================================
;
;  Setting configuration words without using macros:
;
;  To set configuration words without using the 'config' macro,
;  use the following format:
;
;           .section __CONFIG1.sec, code
;           .global __CONFIG1
;__CONFIG1: .pword POSCMOD_EC & FCKSM_CSECMD
;
;  This source code is equivalent to the expanded macro from the previous
;  example.
;
;==========================================================================


;----- CONFIG3 (0x2abfa) --------------------------------------------------
;
;  The following settings are available for CONFIG3:
;
;   Write Protection Location:
;     WPEND_WPSTARTMEM     Write Protect from page 0 to WPFP
;     WPEND_WPENDMEM       Write Protect from WPFP to the last page of memory
;
;   Write Protect Configuration Page:
;     WPCFG_WPCFGEN        Enabled
;     WPCFG_WPCFGDIS       Disabled
;
;   Write Protection Disable:
;     WPDIS_WPEN           Enabled
;     WPDIS_WPDIS          Disabled
;
;   Write Protection Flash Page:
;     WPFP_WPFP0           0
;     WPFP_WPFP1           1
;     WPFP_WPFP2           2
;     WPFP_WPFP3           3
;     WPFP_WPFP4           4
;     WPFP_WPFP5           5
;     WPFP_WPFP6           6
;     WPFP_WPFP7           7
;     WPFP_WPFP8           8
;     WPFP_WPFP9           9
;     WPFP_WPFP10          10
;     WPFP_WPFP11          11
;     WPFP_WPFP12          12
;     WPFP_WPFP13          13
;     WPFP_WPFP14          14
;     WPFP_WPFP15          15
;     WPFP_WPFP16          16
;     WPFP_WPFP17          17
;     WPFP_WPFP18          18
;     WPFP_WPFP19          19
;     WPFP_WPFP20          20
;     WPFP_WPFP21          21
;     WPFP_WPFP22          22
;     WPFP_WPFP23          23
;     WPFP_WPFP24          24
;     WPFP_WPFP25          25
;     WPFP_WPFP26          26
;     WPFP_WPFP27          27
;     WPFP_WPFP28          28
;     WPFP_WPFP29          29
;     WPFP_WPFP30          30
;     WPFP_WPFP31          31
;     WPFP_WPFP32          32
;     WPFP_WPFP33          33
;     WPFP_WPFP34          34
;     WPFP_WPFP35          35
;     WPFP_WPFP36          36
;     WPFP_WPFP37          37
;     WPFP_WPFP38          38
;     WPFP_WPFP39          39
;     WPFP_WPFP40          40
;     WPFP_WPFP41          41
;     WPFP_WPFP42          42
;     WPFP_WPFP43          43
;     WPFP_WPFP44          44
;     WPFP_WPFP45          45
;     WPFP_WPFP46          46
;     WPFP_WPFP47          47
;     WPFP_WPFP48          48
;     WPFP_WPFP49          49
;     WPFP_WPFP50          50
;     WPFP_WPFP51          51
;     WPFP_WPFP52          52
;     WPFP_WPFP53          53
;     WPFP_WPFP54          54
;     WPFP_WPFP55          55
;     WPFP_WPFP56          56
;     WPFP_WPFP57          57
;     WPFP_WPFP58          58
;     WPFP_WPFP59          59
;     WPFP_WPFP60          60
;     WPFP_WPFP61          61
;     WPFP_WPFP62          62
;     WPFP_WPFP63          63
;     WPFP_WPFP64          64
;     WPFP_WPFP65          65
;     WPFP_WPFP66          66
;     WPFP_WPFP67          67
;     WPFP_WPFP68          68
;     WPFP_WPFP69          69
;     WPFP_WPFP70          70
;     WPFP_WPFP71          71
;     WPFP_WPFP72          72
;     WPFP_WPFP73          73
;     WPFP_WPFP74          74
;     WPFP_WPFP75          75
;     WPFP_WPFP76          76
;     WPFP_WPFP77          77
;     WPFP_WPFP78          78
;     WPFP_WPFP79          79
;     WPFP_WPFP80          80
;     WPFP_WPFP81          81
;     WPFP_WPFP82          82
;     WPFP_WPFP83          83
;     WPFP_WPFP84          84
;     WPFP_WPFP85          85
;     WPFP_WPFP86          86
;     WPFP_WPFP87          87
;     WPFP_WPFP88          88
;     WPFP_WPFP89          89
;     WPFP_WPFP90          90
;     WPFP_WPFP91          91
;     WPFP_WPFP92          92
;     WPFP_WPFP93          93
;     WPFP_WPFP94          94
;     WPFP_WPFP95          95
;     WPFP_WPFP96          96
;     WPFP_WPFP97          97
;     WPFP_WPFP98          98
;     WPFP_WPFP99          99
;     WPFP_WPFP100         100
;     WPFP_WPFP101         101
;     WPFP_WPFP102         102
;     WPFP_WPFP103         103
;     WPFP_WPFP104         104
;     WPFP_WPFP105         105
;     WPFP_WPFP106         106
;     WPFP_WPFP107         107
;     WPFP_WPFP108         108
;     WPFP_WPFP109         109
;     WPFP_WPFP110         110
;     WPFP_WPFP111         111
;     WPFP_WPFP112         112
;     WPFP_WPFP113         113
;     WPFP_WPFP114         114
;     WPFP_WPFP115         115
;     WPFP_WPFP116         116
;     WPFP_WPFP117         117
;     WPFP_WPFP118         118
;     WPFP_WPFP119         119
;     WPFP_WPFP120         120
;     WPFP_WPFP121         121
;     WPFP_WPFP122         122
;     WPFP_WPFP123         123
;     WPFP_WPFP124         124
;     WPFP_WPFP125         125
;     WPFP_WPFP126         126
;     WPFP_WPFP127         127
;     WPFP_WPFP128         128
;     WPFP_WPFP129         129
;     WPFP_WPFP130         130
;     WPFP_WPFP131         131
;     WPFP_WPFP132         132
;     WPFP_WPFP133         133
;     WPFP_WPFP134         134
;     WPFP_WPFP135         135
;     WPFP_WPFP136         136
;     WPFP_WPFP137         137
;     WPFP_WPFP138         138
;     WPFP_WPFP139         139
;     WPFP_WPFP140         140
;     WPFP_WPFP141         141
;     WPFP_WPFP142         142
;     WPFP_WPFP143         143
;     WPFP_WPFP144         144
;     WPFP_WPFP145         145
;     WPFP_WPFP146         146
;     WPFP_WPFP147         147
;     WPFP_WPFP148         148
;     WPFP_WPFP149         149
;     WPFP_WPFP150         150
;     WPFP_WPFP151         151
;     WPFP_WPFP152         152
;     WPFP_WPFP153         153
;     WPFP_WPFP154         154
;     WPFP_WPFP155         155
;     WPFP_WPFP156         156
;     WPFP_WPFP157         157
;     WPFP_WPFP158         158
;     WPFP_WPFP159         159
;     WPFP_WPFP160         160
;     WPFP_WPFP161         161
;     WPFP_WPFP162         162
;     WPFP_WPFP163         163
;     WPFP_WPFP164         164
;     WPFP_WPFP165         165
;     WPFP_WPFP166         166
;     WPFP_WPFP167         167
;     WPFP_WPFP168         168
;     WPFP_WPFP169         169
;     WPFP_WPFP170         170
;     WPFP_WPFP171         171
;     WPFP_WPFP172         172
;     WPFP_WPFP173         173
;     WPFP_WPFP174         174
;     WPFP_WPFP175         175
;     WPFP_WPFP176         176
;     WPFP_WPFP177         177
;     WPFP_WPFP178         178
;     WPFP_WPFP179         179
;     WPFP_WPFP180         180
;     WPFP_WPFP181         181
;     WPFP_WPFP182         182
;     WPFP_WPFP183         183
;     WPFP_WPFP184         184
;     WPFP_WPFP185         185
;     WPFP_WPFP186         186
;     WPFP_WPFP187         187
;     WPFP_WPFP188         188
;     WPFP_WPFP189         189
;     WPFP_WPFP190         190
;     WPFP_WPFP191         191
;     WPFP_WPFP192         192
;     WPFP_WPFP193         193
;     WPFP_WPFP194         194
;     WPFP_WPFP195         195
;     WPFP_WPFP196         196
;     WPFP_WPFP197         197
;     WPFP_WPFP198         198
;     WPFP_WPFP199         199
;     WPFP_WPFP200         200
;     WPFP_WPFP201         201
;     WPFP_WPFP202         202
;     WPFP_WPFP203         203
;     WPFP_WPFP204         204
;     WPFP_WPFP205         205
;     WPFP_WPFP206         206
;     WPFP_WPFP207         207
;     WPFP_WPFP208         208
;     WPFP_WPFP209         209
;     WPFP_WPFP210         210
;     WPFP_WPFP211         211
;     WPFP_WPFP212         212
;     WPFP_WPFP213         213
;     WPFP_WPFP214         214
;     WPFP_WPFP215         215
;     WPFP_WPFP216         216
;     WPFP_WPFP217         217
;     WPFP_WPFP218         218
;     WPFP_WPFP219         219
;     WPFP_WPFP220         220
;     WPFP_WPFP221         221
;     WPFP_WPFP222         222
;     WPFP_WPFP223         223
;     WPFP_WPFP224         224
;     WPFP_WPFP225         225
;     WPFP_WPFP226         226
;     WPFP_WPFP227         227
;     WPFP_WPFP228         228
;     WPFP_WPFP229         229
;     WPFP_WPFP230         230
;     WPFP_WPFP231         231
;     WPFP_WPFP232         232
;     WPFP_WPFP233         233
;     WPFP_WPFP234         234
;     WPFP_WPFP235         235
;     WPFP_WPFP236         236
;     WPFP_WPFP237         237
;     WPFP_WPFP238         238
;     WPFP_WPFP239         239
;     WPFP_WPFP240         240
;     WPFP_WPFP241         241
;     WPFP_WPFP242         242
;     WPFP_WPFP243         243
;     WPFP_WPFP244         244
;     WPFP_WPFP245         245
;     WPFP_WPFP246         246
;     WPFP_WPFP247         247
;     WPFP_WPFP248         248
;     WPFP_WPFP249         249
;     WPFP_WPFP250         250
;     WPFP_WPFP251         251
;     WPFP_WPFP252         252
;     WPFP_WPFP253         253
;     WPFP_WPFP254         254
;     WPFP_WPFP255         255
;     WPFP_WPFP256         256
;     WPFP_WPFP257         257
;     WPFP_WPFP258         258
;     WPFP_WPFP259         259
;     WPFP_WPFP260         260
;     WPFP_WPFP261         261
;     WPFP_WPFP262         262
;     WPFP_WPFP263         263
;     WPFP_WPFP264         264
;     WPFP_WPFP265         265
;     WPFP_WPFP266         266
;     WPFP_WPFP267         267
;     WPFP_WPFP268         268
;     WPFP_WPFP269         269
;     WPFP_WPFP270         270
;     WPFP_WPFP271         271
;     WPFP_WPFP272         272
;     WPFP_WPFP273         273
;     WPFP_WPFP274         274
;     WPFP_WPFP275         275
;     WPFP_WPFP276         276
;     WPFP_WPFP277         277
;     WPFP_WPFP278         278
;     WPFP_WPFP279         279
;     WPFP_WPFP280         280
;     WPFP_WPFP281         281
;     WPFP_WPFP282         282
;     WPFP_WPFP283         283
;     WPFP_WPFP284         284
;     WPFP_WPFP285         285
;     WPFP_WPFP286         286
;     WPFP_WPFP287         287
;     WPFP_WPFP288         288
;     WPFP_WPFP289         289
;     WPFP_WPFP290         290
;     WPFP_WPFP291         291
;     WPFP_WPFP292         292
;     WPFP_WPFP293         293
;     WPFP_WPFP294         294
;     WPFP_WPFP295         295
;     WPFP_WPFP296         296
;     WPFP_WPFP297         297
;     WPFP_WPFP298         298
;     WPFP_WPFP299         299
;     WPFP_WPFP300         300
;     WPFP_WPFP301         301
;     WPFP_WPFP302         302
;     WPFP_WPFP303         303
;     WPFP_WPFP304         304
;     WPFP_WPFP305         305
;     WPFP_WPFP306         306
;     WPFP_WPFP307         307
;     WPFP_WPFP308         308
;     WPFP_WPFP309         309
;     WPFP_WPFP310         310
;     WPFP_WPFP311         311
;     WPFP_WPFP312         312
;     WPFP_WPFP313         313
;     WPFP_WPFP314         314
;     WPFP_WPFP315         315
;     WPFP_WPFP316         316
;     WPFP_WPFP317         317
;     WPFP_WPFP318         318
;     WPFP_WPFP319         319
;     WPFP_WPFP320         320
;     WPFP_WPFP321         321
;     WPFP_WPFP322         322
;     WPFP_WPFP323         323
;     WPFP_WPFP324         324
;     WPFP_WPFP325         325
;     WPFP_WPFP326         326
;     WPFP_WPFP327         327
;     WPFP_WPFP328         328
;     WPFP_WPFP329         329
;     WPFP_WPFP330         330
;     WPFP_WPFP331         331
;     WPFP_WPFP332         332
;     WPFP_WPFP333         333
;     WPFP_WPFP334         334
;     WPFP_WPFP335         335
;     WPFP_WPFP336         336
;     WPFP_WPFP337         337
;     WPFP_WPFP338         338
;     WPFP_WPFP339         339
;     WPFP_WPFP340         340
;     WPFP_WPFP341         341
;     WPFP_WPFP342         342
;     WPFP_WPFP343         343
;     WPFP_WPFP344         344
;     WPFP_WPFP345         345
;     WPFP_WPFP346         346
;     WPFP_WPFP347         347
;     WPFP_WPFP348         348
;     WPFP_WPFP349         349
;     WPFP_WPFP350         350
;     WPFP_WPFP351         351
;     WPFP_WPFP352         352
;     WPFP_WPFP353         353
;     WPFP_WPFP354         354
;     WPFP_WPFP355         355
;     WPFP_WPFP356         356
;     WPFP_WPFP357         357
;     WPFP_WPFP358         358
;     WPFP_WPFP359         359
;     WPFP_WPFP360         360
;     WPFP_WPFP361         361
;     WPFP_WPFP362         362
;     WPFP_WPFP363         363
;     WPFP_WPFP364         364
;     WPFP_WPFP365         365
;     WPFP_WPFP366         366
;     WPFP_WPFP367         367
;     WPFP_WPFP368         368
;     WPFP_WPFP369         369
;     WPFP_WPFP370         370
;     WPFP_WPFP371         371
;     WPFP_WPFP372         372
;     WPFP_WPFP373         373
;     WPFP_WPFP374         374
;     WPFP_WPFP375         375
;     WPFP_WPFP376         376
;     WPFP_WPFP377         377
;     WPFP_WPFP378         378
;     WPFP_WPFP379         379
;     WPFP_WPFP380         380
;     WPFP_WPFP381         381
;     WPFP_WPFP382         382
;     WPFP_WPFP383         383
;     WPFP_WPFP384         384
;     WPFP_WPFP385         385
;     WPFP_WPFP386         386
;     WPFP_WPFP387         387
;     WPFP_WPFP388         388
;     WPFP_WPFP389         389
;     WPFP_WPFP390         390
;     WPFP_WPFP391         391
;     WPFP_WPFP392         392
;     WPFP_WPFP393         393
;     WPFP_WPFP394         394
;     WPFP_WPFP395         395
;     WPFP_WPFP396         396
;     WPFP_WPFP397         397
;     WPFP_WPFP398         398
;     WPFP_WPFP399         399
;     WPFP_WPFP400         400
;     WPFP_WPFP401         401
;     WPFP_WPFP402         402
;     WPFP_WPFP403         403
;     WPFP_WPFP404         404
;     WPFP_WPFP405         405
;     WPFP_WPFP406         406
;     WPFP_WPFP407         407
;     WPFP_WPFP408         408
;     WPFP_WPFP409         409
;     WPFP_WPFP410         410
;     WPFP_WPFP411         411
;     WPFP_WPFP412         412
;     WPFP_WPFP413         413
;     WPFP_WPFP414         414
;     WPFP_WPFP415         415
;     WPFP_WPFP416         416
;     WPFP_WPFP417         417
;     WPFP_WPFP418         418
;     WPFP_WPFP419         419
;     WPFP_WPFP420         420
;     WPFP_WPFP421         421
;     WPFP_WPFP422         422
;     WPFP_WPFP423         423
;     WPFP_WPFP424         424
;     WPFP_WPFP425         425
;     WPFP_WPFP426         426
;     WPFP_WPFP427         427
;     WPFP_WPFP428         428
;     WPFP_WPFP429         429
;     WPFP_WPFP430         430
;     WPFP_WPFP431         431
;     WPFP_WPFP432         432
;     WPFP_WPFP433         433
;     WPFP_WPFP434         434
;     WPFP_WPFP435         435
;     WPFP_WPFP436         436
;     WPFP_WPFP437         437
;     WPFP_WPFP438         438
;     WPFP_WPFP439         439
;     WPFP_WPFP440         440
;     WPFP_WPFP441         441
;     WPFP_WPFP442         442
;     WPFP_WPFP443         443
;     WPFP_WPFP444         444
;     WPFP_WPFP445         445
;     WPFP_WPFP446         446
;     WPFP_WPFP447         447
;     WPFP_WPFP448         448
;     WPFP_WPFP449         449
;     WPFP_WPFP450         450
;     WPFP_WPFP451         451
;     WPFP_WPFP452         452
;     WPFP_WPFP453         453
;     WPFP_WPFP454         454
;     WPFP_WPFP455         455
;     WPFP_WPFP456         456
;     WPFP_WPFP457         457
;     WPFP_WPFP458         458
;     WPFP_WPFP459         459
;     WPFP_WPFP460         460
;     WPFP_WPFP461         461
;     WPFP_WPFP462         462
;     WPFP_WPFP463         463
;     WPFP_WPFP464         464
;     WPFP_WPFP465         465
;     WPFP_WPFP466         466
;     WPFP_WPFP467         467
;     WPFP_WPFP468         468
;     WPFP_WPFP469         469
;     WPFP_WPFP470         470
;     WPFP_WPFP471         471
;     WPFP_WPFP472         472
;     WPFP_WPFP473         473
;     WPFP_WPFP474         474
;     WPFP_WPFP475         475
;     WPFP_WPFP476         476
;     WPFP_WPFP477         477
;     WPFP_WPFP478         478
;     WPFP_WPFP479         479
;     WPFP_WPFP480         480
;     WPFP_WPFP481         481
;     WPFP_WPFP482         482
;     WPFP_WPFP483         483
;     WPFP_WPFP484         484
;     WPFP_WPFP485         485
;     WPFP_WPFP486         486
;     WPFP_WPFP487         487
;     WPFP_WPFP488         488
;     WPFP_WPFP489         489
;     WPFP_WPFP490         490
;     WPFP_WPFP491         491
;     WPFP_WPFP492         492
;     WPFP_WPFP493         493
;     WPFP_WPFP494         494
;     WPFP_WPFP495         495
;     WPFP_WPFP496         496
;     WPFP_WPFP497         497
;     WPFP_WPFP498         498
;     WPFP_WPFP499         499
;     WPFP_WPFP500         500
;     WPFP_WPFP501         501
;     WPFP_WPFP502         502
;     WPFP_WPFP503         503
;     WPFP_WPFP504         504
;     WPFP_WPFP505         505
;     WPFP_WPFP506         506
;     WPFP_WPFP507         507
;     WPFP_WPFP508         508
;     WPFP_WPFP509         509
;     WPFP_WPFP510         510
;     WPFP_WPFP511         511
;
        .equiv WPEND_WPSTARTMEM, 0x7FFF ; Write Protect from page 0 to WPFP
        .equiv WPEND_WPENDMEM,  0xFFFF ; Write Protect from WPFP to the last page of memory

        .equiv WPCFG_WPCFGEN,   0xBFFF ; Enabled
        .equiv WPCFG_WPCFGDIS,  0xFFFF ; Disabled

        .equiv WPDIS_WPEN,      0xDFFF ; Enabled
        .equiv WPDIS_WPDIS,     0xFFFF ; Disabled

        .equiv WPFP_WPFP0,      0xFE00 ; 0
        .equiv WPFP_WPFP1,      0xFE01 ; 1
        .equiv WPFP_WPFP2,      0xFE02 ; 2
        .equiv WPFP_WPFP3,      0xFE03 ; 3
        .equiv WPFP_WPFP4,      0xFE04 ; 4
        .equiv WPFP_WPFP5,      0xFE05 ; 5
        .equiv WPFP_WPFP6,      0xFE06 ; 6
        .equiv WPFP_WPFP7,      0xFE07 ; 7
        .equiv WPFP_WPFP8,      0xFE08 ; 8
        .equiv WPFP_WPFP9,      0xFE09 ; 9
        .equiv WPFP_WPFP10,     0xFE0A ; 10
        .equiv WPFP_WPFP11,     0xFE0B ; 11
        .equiv WPFP_WPFP12,     0xFE0C ; 12
        .equiv WPFP_WPFP13,     0xFE0D ; 13
        .equiv WPFP_WPFP14,     0xFE0E ; 14
        .equiv WPFP_WPFP15,     0xFE0F ; 15
        .equiv WPFP_WPFP16,     0xFE10 ; 16
        .equiv WPFP_WPFP17,     0xFE11 ; 17
        .equiv WPFP_WPFP18,     0xFE12 ; 18
        .equiv WPFP_WPFP19,     0xFE13 ; 19
        .equiv WPFP_WPFP20,     0xFE14 ; 20
        .equiv WPFP_WPFP21,     0xFE15 ; 21
        .equiv WPFP_WPFP22,     0xFE16 ; 22
        .equiv WPFP_WPFP23,     0xFE17 ; 23
        .equiv WPFP_WPFP24,     0xFE18 ; 24
        .equiv WPFP_WPFP25,     0xFE19 ; 25
        .equiv WPFP_WPFP26,     0xFE1A ; 26
        .equiv WPFP_WPFP27,     0xFE1B ; 27
        .equiv WPFP_WPFP28,     0xFE1C ; 28
        .equiv WPFP_WPFP29,     0xFE1D ; 29
        .equiv WPFP_WPFP30,     0xFE1E ; 30
        .equiv WPFP_WPFP31,     0xFE1F ; 31
        .equiv WPFP_WPFP32,     0xFE20 ; 32
        .equiv WPFP_WPFP33,     0xFE21 ; 33
        .equiv WPFP_WPFP34,     0xFE22 ; 34
        .equiv WPFP_WPFP35,     0xFE23 ; 35
        .equiv WPFP_WPFP36,     0xFE24 ; 36
        .equiv WPFP_WPFP37,     0xFE25 ; 37
        .equiv WPFP_WPFP38,     0xFE26 ; 38
        .equiv WPFP_WPFP39,     0xFE27 ; 39
        .equiv WPFP_WPFP40,     0xFE28 ; 40
        .equiv WPFP_WPFP41,     0xFE29 ; 41
        .equiv WPFP_WPFP42,     0xFE2A ; 42
        .equiv WPFP_WPFP43,     0xFE2B ; 43
        .equiv WPFP_WPFP44,     0xFE2C ; 44
        .equiv WPFP_WPFP45,     0xFE2D ; 45
        .equiv WPFP_WPFP46,     0xFE2E ; 46
        .equiv WPFP_WPFP47,     0xFE2F ; 47
        .equiv WPFP_WPFP48,     0xFE30 ; 48
        .equiv WPFP_WPFP49,     0xFE31 ; 49
        .equiv WPFP_WPFP50,     0xFE32 ; 50
        .equiv WPFP_WPFP51,     0xFE33 ; 51
        .equiv WPFP_WPFP52,     0xFE34 ; 52
        .equiv WPFP_WPFP53,     0xFE35 ; 53
        .equiv WPFP_WPFP54,     0xFE36 ; 54
        .equiv WPFP_WPFP55,     0xFE37 ; 55
        .equiv WPFP_WPFP56,     0xFE38 ; 56
        .equiv WPFP_WPFP57,     0xFE39 ; 57
        .equiv WPFP_WPFP58,     0xFE3A ; 58
        .equiv WPFP_WPFP59,     0xFE3B ; 59
        .equiv WPFP_WPFP60,     0xFE3C ; 60
        .equiv WPFP_WPFP61,     0xFE3D ; 61
        .equiv WPFP_WPFP62,     0xFE3E ; 62
        .equiv WPFP_WPFP63,     0xFE3F ; 63
        .equiv WPFP_WPFP64,     0xFE40 ; 64
        .equiv WPFP_WPFP65,     0xFE41 ; 65
        .equiv WPFP_WPFP66,     0xFE42 ; 66
        .equiv WPFP_WPFP67,     0xFE43 ; 67
        .equiv WPFP_WPFP68,     0xFE44 ; 68
        .equiv WPFP_WPFP69,     0xFE45 ; 69
        .equiv WPFP_WPFP70,     0xFE46 ; 70
        .equiv WPFP_WPFP71,     0xFE47 ; 71
        .equiv WPFP_WPFP72,     0xFE48 ; 72
        .equiv WPFP_WPFP73,     0xFE49 ; 73
        .equiv WPFP_WPFP74,     0xFE4A ; 74
        .equiv WPFP_WPFP75,     0xFE4B ; 75
        .equiv WPFP_WPFP76,     0xFE4C ; 76
        .equiv WPFP_WPFP77,     0xFE4D ; 77
        .equiv WPFP_WPFP78,     0xFE4E ; 78
        .equiv WPFP_WPFP79,     0xFE4F ; 79
        .equiv WPFP_WPFP80,     0xFE50 ; 80
        .equiv WPFP_WPFP81,     0xFE51 ; 81
        .equiv WPFP_WPFP82,     0xFE52 ; 82
        .equiv WPFP_WPFP83,     0xFE53 ; 83
        .equiv WPFP_WPFP84,     0xFE54 ; 84
        .equiv WPFP_WPFP85,     0xFE55 ; 85
        .equiv WPFP_WPFP86,     0xFE56 ; 86
        .equiv WPFP_WPFP87,     0xFE57 ; 87
        .equiv WPFP_WPFP88,     0xFE58 ; 88
        .equiv WPFP_WPFP89,     0xFE59 ; 89
        .equiv WPFP_WPFP90,     0xFE5A ; 90
        .equiv WPFP_WPFP91,     0xFE5B ; 91
        .equiv WPFP_WPFP92,     0xFE5C ; 92
        .equiv WPFP_WPFP93,     0xFE5D ; 93
        .equiv WPFP_WPFP94,     0xFE5E ; 94
        .equiv WPFP_WPFP95,     0xFE5F ; 95
        .equiv WPFP_WPFP96,     0xFE60 ; 96
        .equiv WPFP_WPFP97,     0xFE61 ; 97
        .equiv WPFP_WPFP98,     0xFE62 ; 98
        .equiv WPFP_WPFP99,     0xFE63 ; 99
        .equiv WPFP_WPFP100,    0xFE64 ; 100
        .equiv WPFP_WPFP101,    0xFE65 ; 101
        .equiv WPFP_WPFP102,    0xFE66 ; 102
        .equiv WPFP_WPFP103,    0xFE67 ; 103
        .equiv WPFP_WPFP104,    0xFE68 ; 104
        .equiv WPFP_WPFP105,    0xFE69 ; 105
        .equiv WPFP_WPFP106,    0xFE6A ; 106
        .equiv WPFP_WPFP107,    0xFE6B ; 107
        .equiv WPFP_WPFP108,    0xFE6C ; 108
        .equiv WPFP_WPFP109,    0xFE6D ; 109
        .equiv WPFP_WPFP110,    0xFE6E ; 110
        .equiv WPFP_WPFP111,    0xFE6F ; 111
        .equiv WPFP_WPFP112,    0xFE70 ; 112
        .equiv WPFP_WPFP113,    0xFE71 ; 113
        .equiv WPFP_WPFP114,    0xFE72 ; 114
        .equiv WPFP_WPFP115,    0xFE73 ; 115
        .equiv WPFP_WPFP116,    0xFE74 ; 116
        .equiv WPFP_WPFP117,    0xFE75 ; 117
        .equiv WPFP_WPFP118,    0xFE76 ; 118
        .equiv WPFP_WPFP119,    0xFE77 ; 119
        .equiv WPFP_WPFP120,    0xFE78 ; 120
        .equiv WPFP_WPFP121,    0xFE79 ; 121
        .equiv WPFP_WPFP122,    0xFE7A ; 122
        .equiv WPFP_WPFP123,    0xFE7B ; 123
        .equiv WPFP_WPFP124,    0xFE7C ; 124
        .equiv WPFP_WPFP125,    0xFE7D ; 125
        .equiv WPFP_WPFP126,    0xFE7E ; 126
        .equiv WPFP_WPFP127,    0xFE7F ; 127
        .equiv WPFP_WPFP128,    0xFE80 ; 128
        .equiv WPFP_WPFP129,    0xFE81 ; 129
        .equiv WPFP_WPFP130,    0xFE82 ; 130
        .equiv WPFP_WPFP131,    0xFE83 ; 131
        .equiv WPFP_WPFP132,    0xFE84 ; 132
        .equiv WPFP_WPFP133,    0xFE85 ; 133
        .equiv WPFP_WPFP134,    0xFE86 ; 134
        .equiv WPFP_WPFP135,    0xFE87 ; 135
        .equiv WPFP_WPFP136,    0xFE88 ; 136
        .equiv WPFP_WPFP137,    0xFE89 ; 137
        .equiv WPFP_WPFP138,    0xFE8A ; 138
        .equiv WPFP_WPFP139,    0xFE8B ; 139
        .equiv WPFP_WPFP140,    0xFE8C ; 140
        .equiv WPFP_WPFP141,    0xFE8D ; 141
        .equiv WPFP_WPFP142,    0xFE8E ; 142
        .equiv WPFP_WPFP143,    0xFE8F ; 143
        .equiv WPFP_WPFP144,    0xFE90 ; 144
        .equiv WPFP_WPFP145,    0xFE91 ; 145
        .equiv WPFP_WPFP146,    0xFE92 ; 146
        .equiv WPFP_WPFP147,    0xFE93 ; 147
        .equiv WPFP_WPFP148,    0xFE94 ; 148
        .equiv WPFP_WPFP149,    0xFE95 ; 149
        .equiv WPFP_WPFP150,    0xFE96 ; 150
        .equiv WPFP_WPFP151,    0xFE97 ; 151
        .equiv WPFP_WPFP152,    0xFE98 ; 152
        .equiv WPFP_WPFP153,    0xFE99 ; 153
        .equiv WPFP_WPFP154,    0xFE9A ; 154
        .equiv WPFP_WPFP155,    0xFE9B ; 155
        .equiv WPFP_WPFP156,    0xFE9C ; 156
        .equiv WPFP_WPFP157,    0xFE9D ; 157
        .equiv WPFP_WPFP158,    0xFE9E ; 158
        .equiv WPFP_WPFP159,    0xFE9F ; 159
        .equiv WPFP_WPFP160,    0xFEA0 ; 160
        .equiv WPFP_WPFP161,    0xFEA1 ; 161
        .equiv WPFP_WPFP162,    0xFEA2 ; 162
        .equiv WPFP_WPFP163,    0xFEA3 ; 163
        .equiv WPFP_WPFP164,    0xFEA4 ; 164
        .equiv WPFP_WPFP165,    0xFEA5 ; 165
        .equiv WPFP_WPFP166,    0xFEA6 ; 166
        .equiv WPFP_WPFP167,    0xFEA7 ; 167
        .equiv WPFP_WPFP168,    0xFEA8 ; 168
        .equiv WPFP_WPFP169,    0xFEA9 ; 169
        .equiv WPFP_WPFP170,    0xFEAA ; 170
        .equiv WPFP_WPFP171,    0xFEAB ; 171
        .equiv WPFP_WPFP172,    0xFEAC ; 172
        .equiv WPFP_WPFP173,    0xFEAD ; 173
        .equiv WPFP_WPFP174,    0xFEAE ; 174
        .equiv WPFP_WPFP175,    0xFEAF ; 175
        .equiv WPFP_WPFP176,    0xFEB0 ; 176
        .equiv WPFP_WPFP177,    0xFEB1 ; 177
        .equiv WPFP_WPFP178,    0xFEB2 ; 178
        .equiv WPFP_WPFP179,    0xFEB3 ; 179
        .equiv WPFP_WPFP180,    0xFEB4 ; 180
        .equiv WPFP_WPFP181,    0xFEB5 ; 181
        .equiv WPFP_WPFP182,    0xFEB6 ; 182
        .equiv WPFP_WPFP183,    0xFEB7 ; 183
        .equiv WPFP_WPFP184,    0xFEB8 ; 184
        .equiv WPFP_WPFP185,    0xFEB9 ; 185
        .equiv WPFP_WPFP186,    0xFEBA ; 186
        .equiv WPFP_WPFP187,    0xFEBB ; 187
        .equiv WPFP_WPFP188,    0xFEBC ; 188
        .equiv WPFP_WPFP189,    0xFEBD ; 189
        .equiv WPFP_WPFP190,    0xFEBE ; 190
        .equiv WPFP_WPFP191,    0xFEBF ; 191
        .equiv WPFP_WPFP192,    0xFEC0 ; 192
        .equiv WPFP_WPFP193,    0xFEC1 ; 193
        .equiv WPFP_WPFP194,    0xFEC2 ; 194
        .equiv WPFP_WPFP195,    0xFEC3 ; 195
        .equiv WPFP_WPFP196,    0xFEC4 ; 196
        .equiv WPFP_WPFP197,    0xFEC5 ; 197
        .equiv WPFP_WPFP198,    0xFEC6 ; 198
        .equiv WPFP_WPFP199,    0xFEC7 ; 199
        .equiv WPFP_WPFP200,    0xFEC8 ; 200
        .equiv WPFP_WPFP201,    0xFEC9 ; 201
        .equiv WPFP_WPFP202,    0xFECA ; 202
        .equiv WPFP_WPFP203,    0xFECB ; 203
        .equiv WPFP_WPFP204,    0xFECC ; 204
        .equiv WPFP_WPFP205,    0xFECD ; 205
        .equiv WPFP_WPFP206,    0xFECE ; 206
        .equiv WPFP_WPFP207,    0xFECF ; 207
        .equiv WPFP_WPFP208,    0xFED0 ; 208
        .equiv WPFP_WPFP209,    0xFED1 ; 209
        .equiv WPFP_WPFP210,    0xFED2 ; 210
        .equiv WPFP_WPFP211,    0xFED3 ; 211
        .equiv WPFP_WPFP212,    0xFED4 ; 212
        .equiv WPFP_WPFP213,    0xFED5 ; 213
        .equiv WPFP_WPFP214,    0xFED6 ; 214
        .equiv WPFP_WPFP215,    0xFED7 ; 215
        .equiv WPFP_WPFP216,    0xFED8 ; 216
        .equiv WPFP_WPFP217,    0xFED9 ; 217
        .equiv WPFP_WPFP218,    0xFEDA ; 218
        .equiv WPFP_WPFP219,    0xFEDB ; 219
        .equiv WPFP_WPFP220,    0xFEDC ; 220
        .equiv WPFP_WPFP221,    0xFEDD ; 221
        .equiv WPFP_WPFP222,    0xFEDE ; 222
        .equiv WPFP_WPFP223,    0xFEDF ; 223
        .equiv WPFP_WPFP224,    0xFEE0 ; 224
        .equiv WPFP_WPFP225,    0xFEE1 ; 225
        .equiv WPFP_WPFP226,    0xFEE2 ; 226
        .equiv WPFP_WPFP227,    0xFEE3 ; 227
        .equiv WPFP_WPFP228,    0xFEE4 ; 228
        .equiv WPFP_WPFP229,    0xFEE5 ; 229
        .equiv WPFP_WPFP230,    0xFEE6 ; 230
        .equiv WPFP_WPFP231,    0xFEE7 ; 231
        .equiv WPFP_WPFP232,    0xFEE8 ; 232
        .equiv WPFP_WPFP233,    0xFEE9 ; 233
        .equiv WPFP_WPFP234,    0xFEEA ; 234
        .equiv WPFP_WPFP235,    0xFEEB ; 235
        .equiv WPFP_WPFP236,    0xFEEC ; 236
        .equiv WPFP_WPFP237,    0xFEED ; 237
        .equiv WPFP_WPFP238,    0xFEEE ; 238
        .equiv WPFP_WPFP239,    0xFEEF ; 239
        .equiv WPFP_WPFP240,    0xFEF0 ; 240
        .equiv WPFP_WPFP241,    0xFEF1 ; 241
        .equiv WPFP_WPFP242,    0xFEF2 ; 242
        .equiv WPFP_WPFP243,    0xFEF3 ; 243
        .equiv WPFP_WPFP244,    0xFEF4 ; 244
        .equiv WPFP_WPFP245,    0xFEF5 ; 245
        .equiv WPFP_WPFP246,    0xFEF6 ; 246
        .equiv WPFP_WPFP247,    0xFEF7 ; 247
        .equiv WPFP_WPFP248,    0xFEF8 ; 248
        .equiv WPFP_WPFP249,    0xFEF9 ; 249
        .equiv WPFP_WPFP250,    0xFEFA ; 250
        .equiv WPFP_WPFP251,    0xFEFB ; 251
        .equiv WPFP_WPFP252,    0xFEFC ; 252
        .equiv WPFP_WPFP253,    0xFEFD ; 253
        .equiv WPFP_WPFP254,    0xFEFE ; 254
        .equiv WPFP_WPFP255,    0xFEFF ; 255
        .equiv WPFP_WPFP256,    0xFF00 ; 256
        .equiv WPFP_WPFP257,    0xFF01 ; 257
        .equiv WPFP_WPFP258,    0xFF02 ; 258
        .equiv WPFP_WPFP259,    0xFF03 ; 259
        .equiv WPFP_WPFP260,    0xFF04 ; 260
        .equiv WPFP_WPFP261,    0xFF05 ; 261
        .equiv WPFP_WPFP262,    0xFF06 ; 262
        .equiv WPFP_WPFP263,    0xFF07 ; 263
        .equiv WPFP_WPFP264,    0xFF08 ; 264
        .equiv WPFP_WPFP265,    0xFF09 ; 265
        .equiv WPFP_WPFP266,    0xFF0A ; 266
        .equiv WPFP_WPFP267,    0xFF0B ; 267
        .equiv WPFP_WPFP268,    0xFF0C ; 268
        .equiv WPFP_WPFP269,    0xFF0D ; 269
        .equiv WPFP_WPFP270,    0xFF0E ; 270
        .equiv WPFP_WPFP271,    0xFF0F ; 271
        .equiv WPFP_WPFP272,    0xFF10 ; 272
        .equiv WPFP_WPFP273,    0xFF11 ; 273
        .equiv WPFP_WPFP274,    0xFF12 ; 274
        .equiv WPFP_WPFP275,    0xFF13 ; 275
        .equiv WPFP_WPFP276,    0xFF14 ; 276
        .equiv WPFP_WPFP277,    0xFF15 ; 277
        .equiv WPFP_WPFP278,    0xFF16 ; 278
        .equiv WPFP_WPFP279,    0xFF17 ; 279
        .equiv WPFP_WPFP280,    0xFF18 ; 280
        .equiv WPFP_WPFP281,    0xFF19 ; 281
        .equiv WPFP_WPFP282,    0xFF1A ; 282
        .equiv WPFP_WPFP283,    0xFF1B ; 283
        .equiv WPFP_WPFP284,    0xFF1C ; 284
        .equiv WPFP_WPFP285,    0xFF1D ; 285
        .equiv WPFP_WPFP286,    0xFF1E ; 286
        .equiv WPFP_WPFP287,    0xFF1F ; 287
        .equiv WPFP_WPFP288,    0xFF20 ; 288
        .equiv WPFP_WPFP289,    0xFF21 ; 289
        .equiv WPFP_WPFP290,    0xFF22 ; 290
        .equiv WPFP_WPFP291,    0xFF23 ; 291
        .equiv WPFP_WPFP292,    0xFF24 ; 292
        .equiv WPFP_WPFP293,    0xFF25 ; 293
        .equiv WPFP_WPFP294,    0xFF26 ; 294
        .equiv WPFP_WPFP295,    0xFF27 ; 295
        .equiv WPFP_WPFP296,    0xFF28 ; 296
        .equiv WPFP_WPFP297,    0xFF29 ; 297
        .equiv WPFP_WPFP298,    0xFF2A ; 298
        .equiv WPFP_WPFP299,    0xFF2B ; 299
        .equiv WPFP_WPFP300,    0xFF2C ; 300
        .equiv WPFP_WPFP301,    0xFF2D ; 301
        .equiv WPFP_WPFP302,    0xFF2E ; 302
        .equiv WPFP_WPFP303,    0xFF2F ; 303
        .equiv WPFP_WPFP304,    0xFF30 ; 304
        .equiv WPFP_WPFP305,    0xFF31 ; 305
        .equiv WPFP_WPFP306,    0xFF32 ; 306
        .equiv WPFP_WPFP307,    0xFF33 ; 307
        .equiv WPFP_WPFP308,    0xFF34 ; 308
        .equiv WPFP_WPFP309,    0xFF35 ; 309
        .equiv WPFP_WPFP310,    0xFF36 ; 310
        .equiv WPFP_WPFP311,    0xFF37 ; 311
        .equiv WPFP_WPFP312,    0xFF38 ; 312
        .equiv WPFP_WPFP313,    0xFF39 ; 313
        .equiv WPFP_WPFP314,    0xFF3A ; 314
        .equiv WPFP_WPFP315,    0xFF3B ; 315
        .equiv WPFP_WPFP316,    0xFF3C ; 316
        .equiv WPFP_WPFP317,    0xFF3D ; 317
        .equiv WPFP_WPFP318,    0xFF3E ; 318
        .equiv WPFP_WPFP319,    0xFF3F ; 319
        .equiv WPFP_WPFP320,    0xFF40 ; 320
        .equiv WPFP_WPFP321,    0xFF41 ; 321
        .equiv WPFP_WPFP322,    0xFF42 ; 322
        .equiv WPFP_WPFP323,    0xFF43 ; 323
        .equiv WPFP_WPFP324,    0xFF44 ; 324
        .equiv WPFP_WPFP325,    0xFF45 ; 325
        .equiv WPFP_WPFP326,    0xFF46 ; 326
        .equiv WPFP_WPFP327,    0xFF47 ; 327
        .equiv WPFP_WPFP328,    0xFF48 ; 328
        .equiv WPFP_WPFP329,    0xFF49 ; 329
        .equiv WPFP_WPFP330,    0xFF4A ; 330
        .equiv WPFP_WPFP331,    0xFF4B ; 331
        .equiv WPFP_WPFP332,    0xFF4C ; 332
        .equiv WPFP_WPFP333,    0xFF4D ; 333
        .equiv WPFP_WPFP334,    0xFF4E ; 334
        .equiv WPFP_WPFP335,    0xFF4F ; 335
        .equiv WPFP_WPFP336,    0xFF50 ; 336
        .equiv WPFP_WPFP337,    0xFF51 ; 337
        .equiv WPFP_WPFP338,    0xFF52 ; 338
        .equiv WPFP_WPFP339,    0xFF53 ; 339
        .equiv WPFP_WPFP340,    0xFF54 ; 340
        .equiv WPFP_WPFP341,    0xFF55 ; 341
        .equiv WPFP_WPFP342,    0xFF56 ; 342
        .equiv WPFP_WPFP343,    0xFF57 ; 343
        .equiv WPFP_WPFP344,    0xFF58 ; 344
        .equiv WPFP_WPFP345,    0xFF59 ; 345
        .equiv WPFP_WPFP346,    0xFF5A ; 346
        .equiv WPFP_WPFP347,    0xFF5B ; 347
        .equiv WPFP_WPFP348,    0xFF5C ; 348
        .equiv WPFP_WPFP349,    0xFF5D ; 349
        .equiv WPFP_WPFP350,    0xFF5E ; 350
        .equiv WPFP_WPFP351,    0xFF5F ; 351
        .equiv WPFP_WPFP352,    0xFF60 ; 352
        .equiv WPFP_WPFP353,    0xFF61 ; 353
        .equiv WPFP_WPFP354,    0xFF62 ; 354
        .equiv WPFP_WPFP355,    0xFF63 ; 355
        .equiv WPFP_WPFP356,    0xFF64 ; 356
        .equiv WPFP_WPFP357,    0xFF65 ; 357
        .equiv WPFP_WPFP358,    0xFF66 ; 358
        .equiv WPFP_WPFP359,    0xFF67 ; 359
        .equiv WPFP_WPFP360,    0xFF68 ; 360
        .equiv WPFP_WPFP361,    0xFF69 ; 361
        .equiv WPFP_WPFP362,    0xFF6A ; 362
        .equiv WPFP_WPFP363,    0xFF6B ; 363
        .equiv WPFP_WPFP364,    0xFF6C ; 364
        .equiv WPFP_WPFP365,    0xFF6D ; 365
        .equiv WPFP_WPFP366,    0xFF6E ; 366
        .equiv WPFP_WPFP367,    0xFF6F ; 367
        .equiv WPFP_WPFP368,    0xFF70 ; 368
        .equiv WPFP_WPFP369,    0xFF71 ; 369
        .equiv WPFP_WPFP370,    0xFF72 ; 370
        .equiv WPFP_WPFP371,    0xFF73 ; 371
        .equiv WPFP_WPFP372,    0xFF74 ; 372
        .equiv WPFP_WPFP373,    0xFF75 ; 373
        .equiv WPFP_WPFP374,    0xFF76 ; 374
        .equiv WPFP_WPFP375,    0xFF77 ; 375
        .equiv WPFP_WPFP376,    0xFF78 ; 376
        .equiv WPFP_WPFP377,    0xFF79 ; 377
        .equiv WPFP_WPFP378,    0xFF7A ; 378
        .equiv WPFP_WPFP379,    0xFF7B ; 379
        .equiv WPFP_WPFP380,    0xFF7C ; 380
        .equiv WPFP_WPFP381,    0xFF7D ; 381
        .equiv WPFP_WPFP382,    0xFF7E ; 382
        .equiv WPFP_WPFP383,    0xFF7F ; 383
        .equiv WPFP_WPFP384,    0xFF80 ; 384
        .equiv WPFP_WPFP385,    0xFF81 ; 385
        .equiv WPFP_WPFP386,    0xFF82 ; 386
        .equiv WPFP_WPFP387,    0xFF83 ; 387
        .equiv WPFP_WPFP388,    0xFF84 ; 388
        .equiv WPFP_WPFP389,    0xFF85 ; 389
        .equiv WPFP_WPFP390,    0xFF86 ; 390
        .equiv WPFP_WPFP391,    0xFF87 ; 391
        .equiv WPFP_WPFP392,    0xFF88 ; 392
        .equiv WPFP_WPFP393,    0xFF89 ; 393
        .equiv WPFP_WPFP394,    0xFF8A ; 394
        .equiv WPFP_WPFP395,    0xFF8B ; 395
        .equiv WPFP_WPFP396,    0xFF8C ; 396
        .equiv WPFP_WPFP397,    0xFF8D ; 397
        .equiv WPFP_WPFP398,    0xFF8E ; 398
        .equiv WPFP_WPFP399,    0xFF8F ; 399
        .equiv WPFP_WPFP400,    0xFF90 ; 400
        .equiv WPFP_WPFP401,    0xFF91 ; 401
        .equiv WPFP_WPFP402,    0xFF92 ; 402
        .equiv WPFP_WPFP403,    0xFF93 ; 403
        .equiv WPFP_WPFP404,    0xFF94 ; 404
        .equiv WPFP_WPFP405,    0xFF95 ; 405
        .equiv WPFP_WPFP406,    0xFF96 ; 406
        .equiv WPFP_WPFP407,    0xFF97 ; 407
        .equiv WPFP_WPFP408,    0xFF98 ; 408
        .equiv WPFP_WPFP409,    0xFF99 ; 409
        .equiv WPFP_WPFP410,    0xFF9A ; 410
        .equiv WPFP_WPFP411,    0xFF9B ; 411
        .equiv WPFP_WPFP412,    0xFF9C ; 412
        .equiv WPFP_WPFP413,    0xFF9D ; 413
        .equiv WPFP_WPFP414,    0xFF9E ; 414
        .equiv WPFP_WPFP415,    0xFF9F ; 415
        .equiv WPFP_WPFP416,    0xFFA0 ; 416
        .equiv WPFP_WPFP417,    0xFFA1 ; 417
        .equiv WPFP_WPFP418,    0xFFA2 ; 418
        .equiv WPFP_WPFP419,    0xFFA3 ; 419
        .equiv WPFP_WPFP420,    0xFFA4 ; 420
        .equiv WPFP_WPFP421,    0xFFA5 ; 421
        .equiv WPFP_WPFP422,    0xFFA6 ; 422
        .equiv WPFP_WPFP423,    0xFFA7 ; 423
        .equiv WPFP_WPFP424,    0xFFA8 ; 424
        .equiv WPFP_WPFP425,    0xFFA9 ; 425
        .equiv WPFP_WPFP426,    0xFFAA ; 426
        .equiv WPFP_WPFP427,    0xFFAB ; 427
        .equiv WPFP_WPFP428,    0xFFAC ; 428
        .equiv WPFP_WPFP429,    0xFFAD ; 429
        .equiv WPFP_WPFP430,    0xFFAE ; 430
        .equiv WPFP_WPFP431,    0xFFAF ; 431
        .equiv WPFP_WPFP432,    0xFFB0 ; 432
        .equiv WPFP_WPFP433,    0xFFB1 ; 433
        .equiv WPFP_WPFP434,    0xFFB2 ; 434
        .equiv WPFP_WPFP435,    0xFFB3 ; 435
        .equiv WPFP_WPFP436,    0xFFB4 ; 436
        .equiv WPFP_WPFP437,    0xFFB5 ; 437
        .equiv WPFP_WPFP438,    0xFFB6 ; 438
        .equiv WPFP_WPFP439,    0xFFB7 ; 439
        .equiv WPFP_WPFP440,    0xFFB8 ; 440
        .equiv WPFP_WPFP441,    0xFFB9 ; 441
        .equiv WPFP_WPFP442,    0xFFBA ; 442
        .equiv WPFP_WPFP443,    0xFFBB ; 443
        .equiv WPFP_WPFP444,    0xFFBC ; 444
        .equiv WPFP_WPFP445,    0xFFBD ; 445
        .equiv WPFP_WPFP446,    0xFFBE ; 446
        .equiv WPFP_WPFP447,    0xFFBF ; 447
        .equiv WPFP_WPFP448,    0xFFC0 ; 448
        .equiv WPFP_WPFP449,    0xFFC1 ; 449
        .equiv WPFP_WPFP450,    0xFFC2 ; 450
        .equiv WPFP_WPFP451,    0xFFC3 ; 451
        .equiv WPFP_WPFP452,    0xFFC4 ; 452
        .equiv WPFP_WPFP453,    0xFFC5 ; 453
        .equiv WPFP_WPFP454,    0xFFC6 ; 454
        .equiv WPFP_WPFP455,    0xFFC7 ; 455
        .equiv WPFP_WPFP456,    0xFFC8 ; 456
        .equiv WPFP_WPFP457,    0xFFC9 ; 457
        .equiv WPFP_WPFP458,    0xFFCA ; 458
        .equiv WPFP_WPFP459,    0xFFCB ; 459
        .equiv WPFP_WPFP460,    0xFFCC ; 460
        .equiv WPFP_WPFP461,    0xFFCD ; 461
        .equiv WPFP_WPFP462,    0xFFCE ; 462
        .equiv WPFP_WPFP463,    0xFFCF ; 463
        .equiv WPFP_WPFP464,    0xFFD0 ; 464
        .equiv WPFP_WPFP465,    0xFFD1 ; 465
        .equiv WPFP_WPFP466,    0xFFD2 ; 466
        .equiv WPFP_WPFP467,    0xFFD3 ; 467
        .equiv WPFP_WPFP468,    0xFFD4 ; 468
        .equiv WPFP_WPFP469,    0xFFD5 ; 469
        .equiv WPFP_WPFP470,    0xFFD6 ; 470
        .equiv WPFP_WPFP471,    0xFFD7 ; 471
        .equiv WPFP_WPFP472,    0xFFD8 ; 472
        .equiv WPFP_WPFP473,    0xFFD9 ; 473
        .equiv WPFP_WPFP474,    0xFFDA ; 474
        .equiv WPFP_WPFP475,    0xFFDB ; 475
        .equiv WPFP_WPFP476,    0xFFDC ; 476
        .equiv WPFP_WPFP477,    0xFFDD ; 477
        .equiv WPFP_WPFP478,    0xFFDE ; 478
        .equiv WPFP_WPFP479,    0xFFDF ; 479
        .equiv WPFP_WPFP480,    0xFFE0 ; 480
        .equiv WPFP_WPFP481,    0xFFE1 ; 481
        .equiv WPFP_WPFP482,    0xFFE2 ; 482
        .equiv WPFP_WPFP483,    0xFFE3 ; 483
        .equiv WPFP_WPFP484,    0xFFE4 ; 484
        .equiv WPFP_WPFP485,    0xFFE5 ; 485
        .equiv WPFP_WPFP486,    0xFFE6 ; 486
        .equiv WPFP_WPFP487,    0xFFE7 ; 487
        .equiv WPFP_WPFP488,    0xFFE8 ; 488
        .equiv WPFP_WPFP489,    0xFFE9 ; 489
        .equiv WPFP_WPFP490,    0xFFEA ; 490
        .equiv WPFP_WPFP491,    0xFFEB ; 491
        .equiv WPFP_WPFP492,    0xFFEC ; 492
        .equiv WPFP_WPFP493,    0xFFED ; 493
        .equiv WPFP_WPFP494,    0xFFEE ; 494
        .equiv WPFP_WPFP495,    0xFFEF ; 495
        .equiv WPFP_WPFP496,    0xFFF0 ; 496
        .equiv WPFP_WPFP497,    0xFFF1 ; 497
        .equiv WPFP_WPFP498,    0xFFF2 ; 498
        .equiv WPFP_WPFP499,    0xFFF3 ; 499
        .equiv WPFP_WPFP500,    0xFFF4 ; 500
        .equiv WPFP_WPFP501,    0xFFF5 ; 501
        .equiv WPFP_WPFP502,    0xFFF6 ; 502
        .equiv WPFP_WPFP503,    0xFFF7 ; 503
        .equiv WPFP_WPFP504,    0xFFF8 ; 504
        .equiv WPFP_WPFP505,    0xFFF9 ; 505
        .equiv WPFP_WPFP506,    0xFFFA ; 506
        .equiv WPFP_WPFP507,    0xFFFB ; 507
        .equiv WPFP_WPFP508,    0xFFFC ; 508
        .equiv WPFP_WPFP509,    0xFFFD ; 509
        .equiv WPFP_WPFP510,    0xFFFE ; 510
        .equiv WPFP_WPFP511,    0xFFFF ; 511


;----- CONFIG2 (0x2abfc) --------------------------------------------------
;
;  The following settings are available for CONFIG2:
;
;   Two Speed Start-up:
;     IESO_OFF             Disabled
;     IESO_ON              Enabled
;
;   96MHz PLL Prescaler:
;     PLLDIV_NODIV         No Divide (4MHz input)
;     PLLDIV_DIV2          Divide by 2 (8MHz input)
;     PLLDIV_DIV3          Divide by 3 (12MHz input)
;     PLLDIV_DIV4          Divide by 4 (16MHz input)
;     PLLDIV_DIV5          Divide by 5 (20MHz input)
;     PLLDIV_DIV6          Divide by 6 (24MHz input)
;     PLLDIV_DIV10         Divide by 10 (40MHz input)
;     PLLDIV_DIV12         Divide by 12 (48MHz input)
;
;   Disable 96MHz PLL:
;     PLL96DIS_ON          Enabled
;     PLL96DIS_OFF         Disabled
;
;   Oscillator Selection:
;     FNOSC_FRC            Fast RC oscillator
;     FNOSC_FRCPLL         Fast RC oscillator w/ divide and PLL
;     FNOSC_PRI            Primary oscillator (XT, HS, EC)
;     FNOSC_PRIPLL         Primary oscillator (XT, HS, EC) w/ PLL
;     FNOSC_SOSC           Secondary oscillator
;     FNOSC_LPRC           Low power RC oscillator
;     FNOSC_FRCDIV         Fast RC oscillator with divide
;
;   Clock switching and clock monitor:
;     FCKSM_CSECME         Both enabled
;     FCKSM_CSECMD         Only clock switching enabled
;     FCKSM_CSDCMD         Both disabled
;
;   OSCO/RC15 function:
;     OSCIOFNC_ON          RC15
;     OSCIOFNC_OFF         OSCO or Fosc/2
;
;   RP Register Protection:
;     IOL1WAY_OFF          Unlimited Writes To RP Registers
;     IOL1WAY_ON           Write RP Registers Once
;
;   Disable Internal USB Voltage Regulator:
;     DISUVREG_ON          Enabled
;     DISUVREG_OFF         Disabled
;
;   Oscillator Selection:
;     POSCMOD_EC           External clock
;     POSCMOD_XT           XT oscillator
;     POSCMOD_HS           HS oscillator
;     POSCMOD_NONE         Primary disabled
;
        .equiv IESO_OFF,        0x7FFF ; Disabled
        .equiv IESO_ON,         0xFFFF ; Enabled

        .equiv PLLDIV_NODIV,    0x8FFF ; No Divide (4MHz input)
        .equiv PLLDIV_DIV2,     0x9FFF ; Divide by 2 (8MHz input)
        .equiv PLLDIV_DIV3,     0xAFFF ; Divide by 3 (12MHz input)
        .equiv PLLDIV_DIV4,     0xBFFF ; Divide by 4 (16MHz input)
        .equiv PLLDIV_DIV5,     0xCFFF ; Divide by 5 (20MHz input)
        .equiv PLLDIV_DIV6,     0xDFFF ; Divide by 6 (24MHz input)
        .equiv PLLDIV_DIV10,    0xEFFF ; Divide by 10 (40MHz input)
        .equiv PLLDIV_DIV12,    0xFFFF ; Divide by 12 (48MHz input)

        .equiv PLL96DIS_ON,     0xF7FF ; Enabled
        .equiv PLL96DIS_OFF,    0xFFFF ; Disabled

        .equiv FNOSC_FRC,       0xF8FF ; Fast RC oscillator
        .equiv FNOSC_FRCPLL,    0xF9FF ; Fast RC oscillator w/ divide and PLL
        .equiv FNOSC_PRI,       0xFAFF ; Primary oscillator (XT, HS, EC)
        .equiv FNOSC_PRIPLL,    0xFBFF ; Primary oscillator (XT, HS, EC) w/ PLL
        .equiv FNOSC_SOSC,      0xFCFF ; Secondary oscillator
        .equiv FNOSC_LPRC,      0xFDFF ; Low power RC oscillator
        .equiv FNOSC_FRCDIV,    0xFFFF ; Fast RC oscillator with divide

        .equiv FCKSM_CSECME,    0xFF3F ; Both enabled
        .equiv FCKSM_CSECMD,    0xFF7F ; Only clock switching enabled
        .equiv FCKSM_CSDCMD,    0xFFBF ; Both disabled

        .equiv OSCIOFNC_ON,     0xFFDF ; RC15
        .equiv OSCIOFNC_OFF,    0xFFFF ; OSCO or Fosc/2

        .equiv IOL1WAY_OFF,     0xFFEF ; Unlimited Writes To RP Registers
        .equiv IOL1WAY_ON,      0xFFFF ; Write RP Registers Once

        .equiv DISUVREG_ON,     0xFFF7 ; Enabled
        .equiv DISUVREG_OFF,    0xFFFF ; Disabled

        .equiv POSCMOD_EC,      0xFFFC ; External clock
        .equiv POSCMOD_XT,      0xFFFD ; XT oscillator
        .equiv POSCMOD_HS,      0xFFFE ; HS oscillator
        .equiv POSCMOD_NONE,    0xFFFF ; Primary disabled


;----- CONFIG1 (0x2abfe) --------------------------------------------------
;
;  The following settings are available for CONFIG1:
;
;   JTAG:
;     JTAGEN_OFF           Disabled
;     JTAGEN_ON            Enabled
;
;   Code Protect:
;     GCP_ON               Enabled
;     GCP_OFF              Disabled
;
;   Write Protect:
;     GWRP_ON              Enabled
;     GWRP_OFF             Disabled
;
;   Background Debugger:
;     BKBUG_ON             Enabled
;     BKBUG_OFF            Disabled
;
;   Clip-on Emulation mode:
;     COE_ON               Enabled
;     COE_OFF              Disabled
;
;   ICD pins select:
;     ICS_PGx3             EMUC/EMUD share PGC3/PGD3
;     ICS_PGx2             EMUC/EMUD share PGC2/PGD2
;     ICS_PGx1             EMUC/EMUD share PGC1/PGD1
;
;   Watchdog Timer:
;     FWDTEN_OFF           Disabled
;     FWDTEN_ON            Enabled
;
;   Windowed WDT:
;     WINDIS_ON            Enabled
;     WINDIS_OFF           Disabled
;
;   Watchdog prescaler:
;     FWPSA_PR32           1:32
;     FWPSA_PR128          1:128
;
;   Watchdog postscale:
;     WDTPS_PS1            1:1
;     WDTPS_PS2            1:2
;     WDTPS_PS4            1:4
;     WDTPS_PS8            1:8
;     WDTPS_PS16           1:16
;     WDTPS_PS32           1:32
;     WDTPS_PS64           1:64
;     WDTPS_PS128          1:128
;     WDTPS_PS256          1:256
;     WDTPS_PS512          1:512
;     WDTPS_PS1024         1:1,024
;     WDTPS_PS2048         1:2,048
;     WDTPS_PS4096         1:4,096
;     WDTPS_PS8192         1:8,192
;     WDTPS_PS16384        1:16,384
;     WDTPS_PS32768        1:32,768
;
        .equiv JTAGEN_OFF,      0x3FFF ; Disabled
        .equiv JTAGEN_ON,       0x7FFF ; Enabled

        .equiv GCP_ON,          0x5FFF ; Enabled
        .equiv GCP_OFF,         0x7FFF ; Disabled

        .equiv GWRP_ON,         0x6FFF ; Enabled
        .equiv GWRP_OFF,        0x7FFF ; Disabled

        .equiv BKBUG_ON,        0x77FF ; Enabled
        .equiv BKBUG_OFF,       0x7FFF ; Disabled

        .equiv COE_ON,          0x7BFF ; Enabled
        .equiv COE_OFF,         0x7FFF ; Disabled

        .equiv ICS_PGx3,        0x7DFF ; EMUC/EMUD share PGC3/PGD3
        .equiv ICS_PGx2,        0x7EFF ; EMUC/EMUD share PGC2/PGD2
        .equiv ICS_PGx1,        0x7FFF ; EMUC/EMUD share PGC1/PGD1

        .equiv FWDTEN_OFF,      0x7F7F ; Disabled
        .equiv FWDTEN_ON,       0x7FFF ; Enabled

        .equiv WINDIS_ON,       0x7FBF ; Enabled
        .equiv WINDIS_OFF,      0x7FFF ; Disabled

        .equiv FWPSA_PR32,      0x7FEF ; 1:32
        .equiv FWPSA_PR128,     0x7FFF ; 1:128

        .equiv WDTPS_PS1,       0x7FF0 ; 1:1
        .equiv WDTPS_PS2,       0x7FF1 ; 1:2
        .equiv WDTPS_PS4,       0x7FF2 ; 1:4
        .equiv WDTPS_PS8,       0x7FF3 ; 1:8
        .equiv WDTPS_PS16,      0x7FF4 ; 1:16
        .equiv WDTPS_PS32,      0x7FF5 ; 1:32
        .equiv WDTPS_PS64,      0x7FF6 ; 1:64
        .equiv WDTPS_PS128,     0x7FF7 ; 1:128
        .equiv WDTPS_PS256,     0x7FF8 ; 1:256
        .equiv WDTPS_PS512,     0x7FF9 ; 1:512
        .equiv WDTPS_PS1024,    0x7FFA ; 1:1,024
        .equiv WDTPS_PS2048,    0x7FFB ; 1:2,048
        .equiv WDTPS_PS4096,    0x7FFC ; 1:4,096
        .equiv WDTPS_PS8192,    0x7FFD ; 1:8,192
        .equiv WDTPS_PS16384,   0x7FFE ; 1:16,384
        .equiv WDTPS_PS32768,   0x7FFF ; 1:32,768


.LIST
