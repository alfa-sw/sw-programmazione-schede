/**/
/*===========================================================================*/
/**
 **      @file    main.c
 **
 **      @brief   MABrd Bootloder main module
 **/
/*===========================================================================*/
/**/
#include "GenericTypeDefs.h"
#include "Compiler.h"

#include "HardwareProfile.h"
#include "mem.h"
#include "define.h"
#include "MACRO.h"
#include "ram.h"
#include "const.h"
#include "main.h"
#include "BL_USB_ServerMg.h"
#include "BL_UART_ServerMg.h"
#include "Bootloader.h"
#include "TimerMg.h"
#include "serialCom.h"

#include "progMemFunctions.h"

/** C O N F I G U R A T I O N **************************************************/
// PIC24FJ256GB110 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1
#pragma config WDTPS = PS128   // Watchdog Timer Postscaler bits->1:128
#pragma config FWPSA = PR128   // WDT Prescaler->Prescaler ratio of 1:128
#pragma config WINDIS = OFF    // Watchdog Timer Window->Standard Watchdog Timer enabled,(Windowed-mode is disabled)
#pragma config ICS    = PGx2   // Comm Channel Select->Emulator functions are shared with PGEC2/PGED2
#pragma config BKBUG  = ON     // Background Debug->Device resets into Debug mode
#pragma config GWRP   = OFF    // General Code Segment Write Protect->Writes to program memory are allowed
#pragma config GCP    = OFF    // General Code Segment Code Protect->Code protection is disabled
#pragma config JTAGEN = OFF    // JTAG Port Enable->JTAG port is disabled
// CONFIG2
#pragma config POSCMOD  = HS   // Primary Oscillator Select->HS oscillator mode selected
#pragma config DISUVREG = OFF  // Internal USB 3.3V Regulator Disable bit->Regulator is disabled
#pragma config IOL1WAY  = OFF  // IOLOCK One-Way Set Enable bit->Unlimited Writes To RP Registers
#pragma config OSCIOFNC = OFF  // Primary Oscillator Output Function->OSCO functions as CLKO (FOSC/2)
#pragma config FCKSM    = CSDCMD // Clock Switching and Monitor->Both Clock Switching and Fail-safe Clock Monitor are disabled
#pragma config FNOSC    = PRIPLL // Oscillator Select->Primary oscillator with PLL module
#pragma config PLLDIV   = DIV2 // Oscillator input divided by 2
#pragma config IESO     = OFF  // Internal External Switch Over Mode->IESO mode (Two-speed start-up)disabled
// CONFIG3
#pragma config WPFP  = WPFP511 // Write Protection Flash Page Segment Boundary->Highest Page (same as page 170)
#pragma config WPDIS = WPDIS   // Segment Write Protection Disable bit->Segmented code protection disabled
#pragma config WPCFG = WPCFGDIS// Configuration Word Code Page Protection Select bit->Last page(at the top of program memory) and Flash configuration words are not protected
#pragma config WPEND = WPENDMEM// Segment Write Protection End Page Select bit->Write Protect from WPFP to the last page of memory
        
#if defined (__DEBUG)
#pragma config FWDTEN = OFF     // Watchdog Timer Enable->Watchdog Timer is disabled
#else
#pragma config FWDTEN = ON     // Watchdog Timer Enable->Watchdog Timer is enabled
#endif

const unsigned long __attribute__ ((space(psv), address (__BOOT_CODE_FW_VER)))
dummy1 = BL_SW_VERSION; 


/** P R O T O T Y P E S *******************************************************/
static void BL_Init(void);
static void BL_UserInit(void);
static void BL_IORemapping(void);
// -----------------------------------------------------------------------------
void Pippo(void);
// -----------------------------------------------------------------------------
/*
**=============================================================================
**
**      Oggetto        : Funzione di servizio degli Interrupt NON usati dal 
**                       BootLoader
**                                    
**      Parametri      : void
**
**      Ritorno        : void
**
**      Vers. - autore : 1.0 Michele Abelli
**
 ******************************************************************************/
void Pippo(void)
{
   unsigned int a;
   for (a = 0; a < 10; a++){}
}

void BL_GestStandAlone(void)
/*******************************************************************************
**
**      Oggetto : Gestione accensione e spegnimento apparecchio
**      attraverso il tasto ON/OFF
**
**      Parametri      : void
**
**      Ritorno        : void
**
**      Vers. - autore : 1.0  nuovo  G.Comai
**
 ******************************************************************************/
{
    // Se il cavo USB e' staccato --> Procediamo con il salto al programma Applicativo, ma solo se è presente
    if ( (BLState.livello == INIT) && (USB_Connect == FALSE) ) {
        enBroadcastMessage(); 
        if ((StatusTimer(T_WAIT_FORCE_BL) == T_ELAPSED) && (BL_StandAlone == BL_WAIT_CHECK_STAND_ALONE))
            BL_StandAlone = CheckApplPres(BL_STAND_ALONE_CHECK);

        if ((StatusTimer(T_WAIT_FORCE_BL) == T_ELAPSED))
            StopTimer(T_WAIT_FORCE_BL);
    }
    
#ifdef REMOTE_UPDATING    
    else if ( (BLState.livello == INIT) && (USB_Connect == TRUE) ) {
        USB_Connect = FALSE;        
        Jump_To_Application_Request();	        
    }
    // Se il cavo USB e' attaccato e in precedenza NON è stato inviato il comando JUMP_TO_BOOT all'applicativo MAB --> Procediamo con il salto al programma Applicativo,, ma solo se è presente
    else if ( (BLState.livello == USB_CONNECT_EXECUTION) && (PtrJMPBoot != JUMP_TO_BOOT_DONE) ){
//        enBroadcastMessage();
        if ((StatusTimer(T_WAIT_FORCE_BL) == T_ELAPSED) && (BL_StandAlone == BL_WAIT_CHECK_STAND_ALONE))
            BL_StandAlone = CheckApplPres(BL_STAND_ALONE_CHECK);

        if ((StatusTimer(T_WAIT_FORCE_BL) == T_ELAPSED))
            StopTimer(T_WAIT_FORCE_BL);
    }
#endif
    
    // E' arrivato un comando "JUMP_TO_APPLICATION", ed in precedenza è stata verificata la presenza di un Applicativo in memoria --> 
    // Procediamo con il salto al programma Applicativo
    else if (BLState.livello == JMP_TO_APP) {
        enBroadcastMessage();
        if (StatusTimer(T_WAIT_FORCE_BL) == T_ELAPSED) {
            StopTimer(T_WAIT_FORCE_BL);  		
            BL_StandAlone = BL_NO_STAND_ALONE;
        }
    }
    if (BLState.livello == USB_CONNECT_EXECUTION)
        USB_Connect = TRUE;    
}
  
unsigned short CRCarea(unsigned char *pointer, unsigned short n_char,unsigned short CRCinit)
/*******************************************************************************
**
**      Oggetto        : Calcola CRC di una zona di byte specificata
**                       dai parametri di ingresso
**
**      Parametri      : pointer      Indirizzo iniziale dell'area
**                                    da controllare
**                       n_char       Numero dei bytes da includere nel calcolo
**                       CRCinit      Valore iniziale di CRC ( = 0 se n_char
**                                    copre l'intera zona da verificare,
**                                    = CRCarea della zona precedente se
**                                    si sta procedendo a blocchi
**
**      Ritorno        : CRCarea      Nuovo valore del CRC calcolato
**
**      Vers. - autore : 1.0  nuovo   G. Comai
**
 ******************************************************************************/
{
    // La routine proviene dalla dispensa "CRC Fundamentals", pagg. 196, 197.

    /* Nota sull'algoritmo:
       dato un vettore, se ne calcoli il CRC_16: se si accodano i 2 bytes del
       CRC_16 a tale vettore (low byte first!!!), il CRC_16 calcolato
       sul vettore così ottenuto DEVE valere zero.
       Tale proprietà può essere sfruttata nelle comunicazione seriali
       per verificare che un messaggio ricevuto,
       contenente in coda i 2 bytes del CRC_16 (calcolati dal trasmettitore),
       sia stato inviato correttamente: il CRC_16, calcolato dal ricevente
       sul messaggio complessivo deve valere zero. */

    unsigned long i;
    unsigned short index;
    unsigned char psv_shadow;

    // save the PSVPAG 
    psv_shadow = PSVPAG;
    // set the PSVPAG for accessing CRC_TABLE[] 
    PSVPAG = __builtin_psvpage (CRC_TABLE);

    for (i = 0; i < n_char; i++) {
        index = ( (CRCinit ^ ( (unsigned short) *pointer & 0x00FF) ) & 0x00FF);
        CRCinit = ( (CRCinit >> 8) & 0x00FF) ^ CRC_TABLE[index];
        pointer = pointer + 1;
        // Reset Watchdog
        ClrWdt();
      } // end for 

    // Restore the PSVPAG for the compiler-managed PSVPAG 
    PSVPAG = psv_shadow;

    return CRCinit;
} // end CRCarea 

/*******************************************************************************
 * Function:        static void BL_Init(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        BL_Init is a centralize initialization
 *                  routine. All required USB initialization routines
 *                  are called from here.
 *
 *                  User application initialization routine should
 *                  also be called from here.
 *
 * Note:            None
 ******************************************************************************/
static void BL_Init(void)
{
  // use Alternate Vector Table 
  INTCON2bits.ALTIVT = 1;
  BL_TimerInit();
  BL_USB_Init();
  BL_initSerialCom();
  BL_UserInit();
}

/*******************************************************************************
 * Function:        void BL_UserInit(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        BL_User_Init is a centralize initialization
 *                  routine. All required USB initialization routines
 *                  are called from here.
 *
 *                  User application initialization routine should
 *                  also be called from here.
 *
 * Note:            None
 ******************************************************************************/
static void BL_UserInit(void)
{
  USB_Status.Connect = USB_POWER_OFF;
  BLState.livello = INIT;
  StartTimer(T_DEL_FILTER_KEY);
  BL_StandAlone = BL_WAIT_CHECK_STAND_ALONE;
  StartTimer(T_WAIT_FORCE_BL);
  Fw_Request = FALSE;
  USB_Connect = FALSE;
}

static void BL_IORemapping(void)
/*******************************************************************************
**
**      Oggetto        : Remapping PIN PIC
**
**
**      Parametri      : void
**
**      Ritorno        : void
**
**      Vers. - autore : 1.0  nuovo  F.Coiro
 ******************************************************************************/
{
    __builtin_write_OSCCONL(OSCCON & 0xbf); /*UnLock IO Pin Remapping*/
    // UART2 - RS232
    RPOR15bits.RP30R = 0x0005;  //RF2->UART2:U2TX;
    RPINR19bits.U2RXR = 0x0011; //RF5->UART2:U2RX;
    // UART3 - RS485
    RPINR17bits.U3RXR = 0x002A; //RD12->UART3:U3RX;
    RPOR6bits.RP12R   = 0x001C; //RD11->UART3:U3TX;
    // SPI1 - MOTOR DRIVER    
 //   RPINR20bits.SCK1R = 0x0000; //RB0->SPI1:SCK1IN;
 //   RPOR6bits.RP13R = 0x0007;   //RB2->SPI1:SDO1;
 //   RPINR20bits.SDI1R = 0x0001; //RB1->SPI1:SDI1;
    // SPI2 - EEPPROM
    RPOR9bits.RP19R = 0x000A;   //RG8->SPI2:SDO2;
    RPOR10bits.RP21R = 0x000B;  //RG6->SPI2:SCK2OUT;
    RPINR22bits.SDI2R = 0x001A; //RG7->SPI2:SDI2;
    // SPI3 - TC72 TEMPERATURE SENSOR
    RPOR11bits.RP23R = 0x0021;  //RD2->SPI3:SCK3OUT;
    RPINR28bits.SDI3R = 0x0004; //RD9->SPI3:SDI3;
    RPOR5bits.RP11R = 0x0020;   //RD0->SPI3:SDO3;
    // I2C3 - SHT31 HUMIDITY SENSOR     
    // NO assignements    
  __builtin_write_OSCCONL(OSCCON | 0x40);   /*Lock IO Pin Remapping*/ 
    // -------------------------------------------------------------------------
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATB = 0x0000;
    LATC = 0x0000;
    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCB = 0x0000;
    ODCC = 0x0000;
    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    AD1PCFGH = 0x0000;
    // No Analog Input, all Digital 
    AD1PCFGL = 0xFFFF;    
    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    // -------------------------------------------------------------------------    
    TRISAbits.TRISA0  = INPUT;  // FO_ACC
    TRISAbits.TRISA1  = INPUT;  // BUSY_BRD
    TRISAbits.TRISA4  = INPUT;  // BUTTON
    TRISAbits.TRISA5  = OUTPUT; // CS_PMP
    TRISAbits.TRISA6  = OUTPUT; // NEB_IN
    TRISAbits.TRISA7  = INPUT;  // NEB_F
    TRISAbits.TRISA9  = INPUT; // FO_GEN1
    TRISAbits.TRISA14 = INPUT;  // LEV_SENS
    // -------------------------------------------------------------------------
    TRISBbits.TRISB0   = OUTPUT; // SCK_DRIVER
    TRISBbits.TRISB1   = OUTPUT; // SDI_DRIVER
    TRISBbits.TRISB2   = INPUT;  // SDO_DRIVER
    TRISBbits.TRISB3   = INPUT;  // BRUSH_F1
    TRISBbits.TRISB4   = OUTPUT; // STCK_PMP
    TRISBbits.TRISB5   = OUTPUT; // STCK_EV
    TRISBbits.TRISB8   = INPUT;  // BUSY_EV
    TRISBbits.TRISB9   = OUTPUT; // CS_EV
    TRISBbits.TRISB10  = OUTPUT; // STBY_RST_PMP
    TRISBbits.TRISB11  = INPUT;  // FLAG_PMP
    TRISBbits.TRISB12  = INPUT;  // BUSY_PMP
    TRISBbits.TRISB13  = OUTPUT; // STBY_RST_BRD
    TRISBbits.TRISB14  = OUTPUT; // STCK_BRD
    TRISBbits.TRISB15  = INPUT;  // FLAG_BRD
    // -------------------------------------------------------------------------
    TRISCbits.TRISC2   = INPUT;  // FO_VALV
    TRISCbits.TRISC3   = OUTPUT; // EEPROM_CS
    TRISCbits.TRISC4   = INPUT;  // FO_BRD
    TRISCbits.TRISC12  = INPUT;  // OSC1
    TRISCbits.TRISC13  = OUTPUT; // I3_BRUSH
    TRISCbits.TRISC14  = OUTPUT; // I4_BRUSH
    TRISCbits.TRISC15  = INPUT;  // OSC2
    // -------------------------------------------------------------------------
    TRISDbits.TRISD0   = OUTPUT; // SDO_TC72
    TRISDbits.TRISD1   = OUTPUT; // RELAY
    TRISDbits.TRISD2   = OUTPUT; // SCK_TC72
    TRISDbits.TRISD4   = INPUT;  // RELAY_F
    TRISDbits.TRISD5   = OUTPUT; // LASER_BHL
    TRISDbits.TRISD6   = OUTPUT; // IN1_BRUSH
    TRISDbits.TRISD7   = OUTPUT; // DECAY
    TRISDbits.TRISD8   = INPUT;  // FO_HOME
    TRISDbits.TRISD9   = INPUT;  // SDI_TC72
    TRISDbits.TRISD10  = OUTPUT; // I2_BRUSH
    TRISDbits.TRISD11  = OUTPUT; // RS485_TXD
    TRISDbits.TRISD12  = INPUT;  // RS485_RXD
    TRISDbits.TRISD13  = OUTPUT; // RS485_DE
    TRISDbits.TRISD14  = OUTPUT; // CS_BRD
    TRISDbits.TRISD15  = INPUT;  // INT_CAR
    // -------------------------------------------------------------------------
    TRISEbits.TRISE1   = INPUT;  // FLAG_EV
    TRISEbits.TRISE2   = INPUT;  // FO_CPR
    TRISEbits.TRISE3   = OUTPUT; // OUT_24V_IN
    TRISEbits.TRISE4   = OUTPUT; // LED_ON_OFF
    TRISEbits.TRISE5   = OUTPUT; // RST_BRUSH
    TRISEbits.TRISE6   = OUTPUT; // SCL_SHT31
    TRISEbits.TRISE7   = OUTPUT; // SDA_SHT31
    TRISEbits.TRISE9   = INPUT;  // BRUSH_F2    
    // -------------------------------------------------------------------------
    TRISFbits.TRISF0   = OUTPUT; // RST_SHT31
    TRISFbits.TRISF1   = INPUT;  // ALERT_SHT31
    TRISFbits.TRISF2   = OUTPUT; // RS232_TXD
    TRISFbits.TRISF4   = INPUT;  // FO_GEN2
    TRISFbits.TRISF5   = INPUT;  // RS232_RXD
    TRISFbits.TRISF8   = INPUT;  // IO_GEN2
    TRISFbits.TRISF12  = INPUT;  // INT_PAN
    TRISFbits.TRISF13  = INPUT;  // IO_GEN1
    // -------------------------------------------------------------------------
    TRISGbits.TRISG1   = OUTPUT; // IN2_BRUSH
    TRISGbits.TRISG2   = OUTPUT; // USB_DP
    TRISGbits.TRISG3   = OUTPUT; // USB_DN
    TRISGbits.TRISG6   = OUTPUT; // SCK_EEPROM
    TRISGbits.TRISG7   = INPUT;  // SDI_EEPROM
    TRISGbits.TRISG8   = OUTPUT; // SDO_EEPROM
    TRISGbits.TRISG9   = OUTPUT; // STBY_RST_EV
    TRISGbits.TRISG12  = OUTPUT; // AIR_PUMP_IN
    TRISGbits.TRISG13  = INPUT;  // AIR_PUMP_F
    TRISGbits.TRISG14  = OUTPUT; // CE_TC72
    TRISGbits.TRISG15  = INPUT;  // OUT_24V_FAULT
    // -------------------------------------------------------------------------
    // Set PPS - SPI1
    RPOR5bits.RP10R   = 0x0009; //RB10->SPI1:SS1OUT;
    RPOR5bits.RP11R   = 0x0008; //RB11->SPI1:SCK1OUT;
    RPINR20bits.SDI1R = 0x000D; //RB13->SPI1:SDI1;
    RPOR6bits.RP12R   = 0x0007; //RB12->SPI1:SDO1;	
}// end IORemapping

/*******************************************************************************
 * Function:        char CheckApplPres(DWORD address)
 *
 * PreCondition:    None
 *
 * Input:           Long -> First Application Flash Address
 *
 * Output:          Char -> BootLoader Stand Alone = TRUE
 *
 * Side Effects:    None
 *
 * Overview:        CheckApplPres is a centralize initialization
 *                  routine. All required USB initialization routines
 *                  are called from here.
 *
 *                  User application initialization routine should
 *                  also be called from here.
 *
 * Note:            None
 ******************************************************************************/
char CheckApplPres(DWORD address)
{
    if (ReadProgramMemory(address) == APPL_FLASH_MEMORY_ERASED_VALUE)
      return BL_STAND_ALONE;

    return BL_NO_STAND_ALONE;
}//end CheckApplPres

static void spi_remapping(unsigned char spi_index)
{
    switch (spi_index)
    {
        case SPI_1: //DRIVER
           //SDI1 = RP13
            RPINR20bits.SDI1R = SDI1_IO; 
            TRISBbits.TRISB2 = 1;
            //RP0 = SCK1
            RPOR0bits.RP0R = SCK1_IO;
            TRISBbits.TRISB0 = 0;
            //RP1 = SDO1
            AD1PCFGLbits.PCFG1 = 1;
            AD1PCFGLbits.PCFG2 = 1;
            RPOR0bits.RP1R = SDO1_IO;
            TRISBbits.TRISB1 = 0;
            
            break;
        
        case SPI_2: // EEPROM
            //SDI2 = RP26
            RPINR22bits.SDI2R = SDI2_IO; 
            TRISGbits.TRISG7 = 1;
            //RP21 = SCK2
            RPOR10bits.RP21R = SCK2_IO;
            TRISGbits.TRISG6 = 0;
            //RP19 = SDO2
            RPOR9bits.RP19R = SDO2_IO;
            TRISGbits.TRISG8 = 0;
            break;
            
        case SPI_3: //Sensore di temperatura
            //SDI3 = RP4
            //RPINR28bits.SDI3R = SDI3_IO; 
            //RP21 = SCK3
            //RPOR11bits.RP23R = SCK3_IO;
            //RP19 = SDO3
            //RPOR5bits.RP11R = SDO3_IO;
            break;    
    }
}

static void spi_init(unsigned char spi_index)
/**/
/*===========================================================================*/
/**
**   @brief This function is used for initializing the SPI module.
**
**          Preconditions: TRIS bits of SCK and SDO should be made
**          output.  TRIS bit of SDI should be made input. TRIS bit of
**          Slave Chip Select pin (if any used) should be made output.
**
**   @param  spi_index index of SPI peripheral
**
**   @return void
**/
/*===========================================================================*/
/**/
{
  switch (spi_index)
  {
  case SPI_1:
    SPI1STAT = 0;
    SPI1CON2 = 0;

    SPI1CON1 = (SPIM_PPRE | SPIM_SPRE); 
    SPI1STATbits.SPIEN = 0;
    SPI1CON1bits.MSTEN = 1; // 1: Master mode, 0: slave mode
    SPI1CON1bits.MODE16 = 0; // 0: communication is byte-wide, 1: word-wide
    SPI1CON1bits.CKE = 0; // Clock Edge Select bit, it depends on the slave
    SPI1CON1bits.CKP = 1;  // Clock Polarity Select bit. it depends on the slave
    SPI1CON1bits.SMP = 1; // Data Input Sample Phase. it depends on the slave

    SPI1STATbits.SPIEN = 1;
    break;

  case SPI_2:
    SPI2STAT = 0;
    SPI2CON2 = 0;

    // SPI2CON1 = (SPIM_PPRE | SPIM_SPRE); 
    SPI2STATbits.SPIEN = 0;
    SPI2CON1bits.MSTEN = 1; // 1: Master mode, 0: slave mode
    SPI2CON1bits.MODE16 = 0; // 0: communication is byte-wide, 1: word-wide
    SPI2CON1bits.CKE = 0; // Clock Edge Select bit, it depends on the slave
    SPI2CON1bits.CKP = 1;  // Clock Polarity Select bit. it depends on the slave
    SPI2CON1bits.SMP = 1; // Data Input Sample Phase. it depends on the slave
    
    SPI2STATbits.SPIEN = 1;
    break;
    
  case SPI_3:
    // MSTEN Master; DISSDO disabled; PPRE 64:1; SPRE 8:1; MODE16 disabled; SMP Middle; DISSCK disabled; CKP Idle:Low, Active:High; CKE Idle to Active; SSEN disabled; 
    // SPI Clock Frequency: 7.8125kHz
    // Communication is byte wide, MODE16 disabled
    SPI3CON1 = 0x20;
    // SPIFSD disabled; SPIBEN enabled; SPIFPOL disabled; SPIFE disabled; FRMEN disabled; 
    // Enhanced Buffer Master Mode selected, SPIBEN enabled
    SPI3CON2 = 0x01;
    // SPITBF disabled; SISEL SPI_INT_SPIRBF; SPIRBF disabled; SPIROV disabled; SPIEN enabled; SRXMPT disabled; SPISIDL disabled; 
    SPI3STAT = 0x800C;
    // Al momento NON usiamo gli Interrupt
    // Enable alt 3 SPI3 Interrupts
    //    SPITXI: SPI3TX - SPI3 Transfer Done
    //    Priority: 1
//    IPC2bits.SPI3TXIP = 1;
    //    SPII: SPI3 - SPI3 General
    //    Priority: 1
//    IPC2bits.SPI3IP = 1;
    //    SPIRXI: SPI3RX - SPI3 Receive Done
    //    Priority: 1
//    IPC14bits.SPI3RXIP = 1;
    break;    
    
  }
}

static void hw_init(void)
{
    // Auto generate initialization
    __builtin_write_OSCCONL(OSCCON & 0xbf); /*UnLock IO Pin Remapping*/     
    spi_remapping(SPI_1);
    __builtin_write_OSCCONL(OSCCON | 0x40);   /*Lock IO Pin Remapping*/  
    spi_init(SPI_1);  //SPI controllo motore  
    spi_remapping(SPI_2);
    spi_init(SPI_2); //SPI controllo EEprom
    spi_init(SPI_3); //SPI Sensore Temperatura    
}

//static void jump_to_appl()
void jump_to_appl()
{
  // Disabilito periferica relativa a Timer 5, solo del Bootloader, prima del salto perchè gestito solo dalla AIVT
  T5CON = 0x0000;
  PR5 = 0xFFFF;
  _T5IF = 0;
  _T5IE = 0;
    
  // Use standard vector table
  INTCON2bits.ALTIVT = 0;

  // Jump to app code, won't return
  __asm__ volatile ("goto " __APPL_GOTO_ADDR);
}

int main(void)
{
    BL_IORemapping();
    // Hardware initialization
    hw_init();
    DISABLE_WDT();
    // BootLoader app initialization
    BL_Init();
    boot_fw_version = (unsigned long) BL_SW_VERSION;
    BL_Master_Version = 0;			  
    ENABLE_WDT();
    do {
        // Kick the dog
        ClrWdt();
        BL_GestStandAlone();
        BL_USB_ServerMg();
        BL_serialCommManager();

        BL_TimerMg();
} while (BL_StandAlone != BL_NO_STAND_ALONE);
	Nop();
	Nop();      
    jump_to_appl(); // goodbye

    return 0; // unreachable, just get rid of compiler warning
} // end main

void __attribute__((__interrupt__,auto_psv)) _DefaultInterrupt(void) {
    Nop();
    Nop();
    while(1);
}
//------------------------------------------------------------------------------
void __attribute__((address(__APPL_T1)__interrupt__,auto_psv))    _T1Interrupt(void)   { }
void __attribute__((address(__APPL_U2RX1)__interrupt__,auto_psv)) _U2RXInterrupt(void) { }
void __attribute__((address(__APPL_U2TX1)__interrupt__,auto_psv)) _U2TXInterrupt(void) { }
void __attribute__((address(__APPL_U3RX1)__interrupt__,auto_psv)) _U3RXInterrupt(void) { }
void __attribute__((address(__APPL_U3TX1)__interrupt__,auto_psv)) _U3TXInterrupt(void) { }
void __attribute__((address(__APPL_SPI1)__interrupt__,auto_psv))  _SPI1Interrupt(void) { }
void __attribute__((address(__APPL_SPI2)__interrupt__,auto_psv))  _SPI2Interrupt(void) { }
void __attribute__((address(__APPL_SPI3)__interrupt__,auto_psv))  _SPI3Interrupt(void) { }
void __attribute__((address(__APPL_I2C3)__interrupt__,auto_psv))  _MI2C3Interrupt(void){ }
// -----------------------------------------------------------------------------
