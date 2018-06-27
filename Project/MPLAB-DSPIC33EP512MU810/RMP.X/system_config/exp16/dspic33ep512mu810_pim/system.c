/*******************************************************************************
 System Initialization File

  File Name:
    sys_init.c

  Summary:
    System Initialization.

  Description:
    This file contains source code necessary to initialize the system.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END

// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************
#include <xc.h>
#include "app.h"

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GSS = OFF                // General Segment Code-Protect bit (General Segment Code protect is disabled)
#pragma config GSSK = OFF               // General Segment Key bits (General Segment Write Protection and Code Protection is Disabled)

// FOSCSEL
#pragma config FNOSC = FRC              // Initial Oscillator Source Selection Bits (Internal Fast RC (FRC))
#pragma config IESO = ON                // Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source)

// FOSC
#pragma config POSCMD = XT              // Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config IOL1WAY = OFF            // Peripheral pin select configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSECMD           // Clock Switching Mode bits (Clock switching is enabled,Fail-safe Clock Monitor is disabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler Bits (1:32,768)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON              // PLL Lock Wait Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR128           // Power-on Reset Timer Value Select bits (128ms)
#pragma config BOREN = ON               // Brown-out Reset (BOR) Detection Enable bit (BOR is enabled)
#pragma config ALTI2C1 = ON             // Alternate I2C pins for I2C1 (ASDA1/ASCK1 pins are selected as the I/O pins for I2C1)
#pragma config ALTI2C2 = ON             // Alternate I2C pins for I2C2 (ASDA2/ASCK2 pins are selected as the I/O pins for I2C2)

// FICD
#pragma config ICS = PGD1               // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config RSTPRI = PF              // Reset Target Vector Select bit (Device will obtain reset instruction from Primary flash)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FAS
#pragma config AWRP = OFF               // Auxiliary Segment Write-protect bit (Auxiliary program memory is not write-protected)
#pragma config APL = OFF                // Auxiliary Segment Code-protect bit (Aux Flash Code protect is disabled)
#pragma config APLK = OFF               // Auxiliary Segment Key bits (Aux Flash Write Protection and Code Protection is Disabled)


// ****************************************************************************
// ****************************************************************************
// Section: File Scope Functions
// ****************************************************************************
// ****************************************************************************

void SOSC_Configuration ( void ) ;
void __attribute__ ( ( __interrupt__ , auto_psv ) ) _OscillatorFail ( void ) ;
void __attribute__ ( ( __interrupt__ , auto_psv ) ) _AddressError ( void ) ;
void __attribute__ ( ( __interrupt__ , auto_psv ) ) _StackError ( void ) ;
void __attribute__ ( ( __interrupt__ , auto_psv ) ) _MathError ( void ) ;

// ****************************************************************************
// ****************************************************************************
// Section: System Initialization
// ****************************************************************************
// ****************************************************************************
/*******************************************************************************
  Function:
    void SYS_Initialize ( void )

  Summary:
    Initializes the Explorer 16 board peripherals

  Description:
    This routine initializes the Explorer 16 board peripherals for the demo.
    In a bare-metal environment (where no OS is supported), this routine should
    be called almost immediately after entering the "main" routine.

  Precondition:
    The C-language run-time environment and stack must have been initialized.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    SYS_INT_Initialize(NULL);
    </code>

  Remarks:
    Basic System Initialization Sequence:

    1.  Initilize minimal board services and processor-specific items
        (enough to use the board to initialize drivers and services)
    2.  Initialize all supported system services
    3.  Initialize all supported modules
        (libraries, drivers, middleware, and application-level modules)
    4.  Initialize the main (static) application, if present.

    The order in which services and modules are initialized and started may be
    important.

 */

void SYS_Initialize ( void )
{
    /* Enable LEDs*/
    LED_Enable ( LED_D9 ) ;
    LED_Enable ( LED_D10 ) ;

    /* Turn On LEDs*/
    LED_On ( LED_D9 ) ;
    LED_On ( LED_D10 ) ;

    /* Enable Switch S3*/
    BUTTON_Enable ( BUTTON_S3 ) ;

    /* Enable ADC to the Potentiometer channel */ 
     
    ADC_ChannelEnable ( ADC_CHANNEL_POTENTIOMETER ) ;


    /* Initialize LCD*/
    PRINT_SetConfiguration ( PRINT_CONFIGURATION_LCD ) ;

    /* Configure Secondary Ocillator for Timer 1 to work as RTC counter*/
    SOSC_Configuration ( ) ;
}

void SOSC_Configuration ( void )
{
    char a , b , c , *p ;

    a = 2 ;
    b = 0x46 ;
    c = 0x57 ;
    p = (char *) &OSCCON ;

    asm volatile ("mov.b %1,[%0] \n"
                "mov.b %2,[%0] \n"
                "mov.b %3,[%0] \n" : /* no outputs */ : "r"( p ) , "r"( b ) , "r"( c ) ,
                "r"( a ) ) ;
}

// *****************************************************************************
// *****************************************************************************
// Section: Interrupt Handlers
// *****************************************************************************
// *****************************************************************************
/*******************************************************************************

  Function:
   void __attribute__((__interrupt__)) _AD1Interrupt( void ))

  Summary:
    ISR routine for the ADC1 Interrupt.

  Description:
    This is the raw Interrupt Service Routine (ISR) for the ADC 1 interrupt.
    This routine calls the ADC's interrupt routine to service the
    interrupt.

  Precondition:
    The ADC peripheral must have been initialized for ADC 1.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    This routine must not be "called" as a C-language function is called.  It
    is "vectored" to by the processor when the interrupt occurs.
 */

void __attribute__ ( ( __interrupt__ , auto_psv ) ) _AD1Interrupt ( void )
{
    static int count = 0 ;

    /* Simple "I am here" indicator */
    if ( count++ == 2000 )
    {
        LED_Toggle ( LED_D9 ) ;
        count = 0 ;
    }

    /* Save the Potentiometer data */
    appData.temp1 = ADC1BUF0 ;

    /* set flag to update LCD */
    appData.adc_lcd_update = 1 ;

    /* reset ADC interrupt flag */
    IFS0bits.AD1IF = 0 ;
}
/*******************************************************************************
  Function:
   void __attribute__((__interrupt__)) _T1Interrupt( void ))

  Summary:
    ISR routine for the T1 Interrupt.

  Description:
    This is the raw Interrupt Service Routine (ISR) for the Timer 3 interrupt.
    This routine calls the Timer1's interrupt routine to service the
    interrupt.

  Precondition:
    The Timer peripheral must have been initialized for Timer 3.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    This routine must not be "called" as a C-language function is called.  It
    is "vectored" to by the processor when the interrupt occurs.
 */
void __attribute__ ( ( __interrupt__ , auto_psv ) ) _T1Interrupt ( void )
{
    /* update seconds */
    if ( appData.seconds < 59 )
    {
        appData.seconds++ ;
    }
        /* update minutes */
    else
    {
        appData.seconds = 0x00 ;
        if ( appData.minutes < 59 )
        {
            appData.minutes++ ;
        }
            /* update hours */
        else
        {
            appData.minutes = 0x00 ;
            if ( appData.hours < 23 )
            {
                appData.hours ++ ;
            }
            else
            {
                appData.hours = 0x00 ;
            }
        }
    }

    /* set flag to update LCD */
    appData.rtc_lcd_update = 1 ;

    /* Toggle LED at 1 Hz rate */
    LED_Toggle ( LED_D10 ) ;

    /* reset Timer 1 interrupt flag */
    IFS0bits.T1IF = 0 ;
}

// *****************************************************************************
// *****************************************************************************
// Section: Primary Exception Vector handlers
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/*  void __attribute__((__interrupt__,auto_psv)) _OscillatorFail(void)

  Summary:
    Provides the required exception vector handlers for Oscillator trap

  Description:
   This routine is used if INTCON2bits.ALTIVT = 0 and it handles the oscillator
 trap.

  Remarks:
   All trap service routines in this file simply ensure that device
   continuously executes code within the trap service routine. Users
   may modify the basic framework provided here to suit to the needs
   of their application.
 */

void __attribute__ ( ( __interrupt__ , auto_psv ) ) _OscillatorFail ( void )
{
    INTCON1bits.OSCFAIL = 0 ;        //Clear the trap flag
    while (1) ;
}

// *****************************************************************************
/*  void __attribute__((__interrupt__,auto_psv)) _AddressError(void)

  Summary:
    Provides the required exception vector handlers for Address Error trap

  Description:
   This routine is used if INTCON2bits.ALTIVT = 0 and it handles the address
 error trap.

  Remarks:
   All trap service routines in this file simply ensure that device
   continuously executes code within the trap service routine. Users
   may modify the basic framework provided here to suit to the needs
   of their application.
 */

void __attribute__ ( ( __interrupt__ , auto_psv ) ) _AddressError ( void )
{
    INTCON1bits.ADDRERR = 0 ;        //Clear the trap flag
    while (1) ;
}

// *****************************************************************************
/*  void __attribute__((__interrupt__,auto_psv)) _StackError(void))

  Summary:
    Provides the required exception vector handlers for Stack Error trap

  Description:
   This routine is used if INTCON2bits.ALTIVT = 0 and it handles the stack
 error trap.

  Remarks:
   All trap service routines in this file simply ensure that device
   continuously executes code within the trap service routine. Users
   may modify the basic framework provided here to suit to the needs
   of their application.
 */

void __attribute__ ( ( __interrupt__ , auto_psv ) ) _StackError ( void )
{
    INTCON1bits.STKERR = 0 ;         //Clear the trap flag
    while (1) ;
}

// *****************************************************************************
/*  void __attribute__((__interrupt__,auto_psv)) _MathError(void))

  Summary:
    Provides the required exception vector handlers for Math Error trap

  Description:
   This routine is used if INTCON2bits.ALTIVT = 0 and it handles the math
 error trap.

  Remarks:
   All trap service routines in this file simply ensure that device
   continuously executes code within the trap service routine. Users
   may modify the basic framework provided here to suit to the needs
   of their application.
 */

void __attribute__ ( ( __interrupt__ , auto_psv ) ) _MathError ( void )
{
    INTCON1bits.MATHERR = 0 ;        //Clear the trap flag
    while (1) ;
}
