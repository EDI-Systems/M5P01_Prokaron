/******************************************************************************
Filename    : test_DSPIC33EP512MU810.h
Author      : pry 
Date        : 22/07/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The testbench for DSPIC33EP512MU810.
******************************************************************************/

/* Includes ******************************************************************/
#include "RMP.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Where are the initial stacks */
#define THD1_STACK          (&Stack_1[300])
#define THD2_STACK          (&Stack_2[300])
/* How to read counter */
#define COUNTER_READ()      ((TMR2)<<1)
/* Are we testing the memory pool? */
#define TEST_MEM_POOL       8192
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* Because on this platform, a pointer is 16-bit, thus we need to define this */
#define PTR_16_BIT
/* The PIC33 timers 2-x are all 16 bits, so */
typedef u16 tim_t;

/* The pragmas for PIC16 fuse */
/* FGS */
#pragma config GWRP=OFF               /* General Segment Write-Protect bit (General Segment may be written) */
#pragma config GSS=OFF                /* General Segment Code-Protect bit (General Segment Code protect is disabled) */
#pragma config GSSK=OFF               /* General Segment Key bits (General Segment Write Protection and Code Protection is Disabled) */

/* FOSCSEL */
#pragma config FNOSC=PRIPLL           /* Initial Oscillator Source Selection Bits (External Oscillator with PLL) */
#pragma config IESO=ON                /* Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source) */

/* FOSC */
#pragma config POSCMD=XT              /* Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode). By default this is 50MHz */
#pragma config OSCIOFNC=OFF           /* OSC2 Pin Function bit (OSC2 is clock output) */
#pragma config IOL1WAY=OFF            /* Peripheral pin select configuration (Allow multiple reconfigurations) */
#pragma config FCKSM=CSECMD           /* Clock Switching Mode bits (Clock switching is enabled,Fail-safe Clock Monitor is disabled) */

/* FWDT */
#pragma config WDTPOST=PS32768        /* Watchdog Timer Postscaler Bits (1:32,768) */
#pragma config WDTPRE=PR128           /* Watchdog Timer Prescaler bit (1:128) */
#pragma config PLLKEN=ON              /* PLL Lock Wait Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.) */
#pragma config WINDIS=OFF             /* Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode) */
#pragma config FWDTEN=OFF             /* Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software) */

/* FPOR */
#pragma config FPWRT=PWR128           /* Power-on Reset Timer Value Select bits (128ms) */
#pragma config BOREN=ON               /* Brown-out Reset (BOR) Detection Enable bit (BOR is enabled) */
#pragma config ALTI2C1=ON             /* Alternate I2C pins for I2C1 (ASDA1/ASCK1 pins are selected as the I/O pins for I2C1) */
#pragma config ALTI2C2=ON             /* Alternate I2C pins for I2C2 (ASDA2/ASCK2 pins are selected as the I/O pins for I2C2) */

/* FICD */
#pragma config ICS=PGD1               /* ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1) */
#pragma config RSTPRI=PF              /* Reset Target Vector Select bit (Device will obtain reset instruction from Primary flash) */
#pragma config JTAGEN=OFF             /* JTAG Enable bit (JTAG is disabled) */

/* FAS */
#pragma config AWRP=OFF               /* Auxiliary Segment Write-protect bit (Auxiliary program memory is not write-protected) */
#pragma config APL=OFF                /* Auxiliary Segment Code-protect bit (Aux Flash Code protect is disabled) */
#pragma config APLK=OFF               /* Auxiliary Segment Key bits (Aux Flash Write Protection and Code Protection is Disabled) */
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
ptr_t Stack_1[512];
ptr_t Stack_2[512];
/* End Globals ***************************************************************/

/* Begin Function:Timer_Init **************************************************
Description : Initialize the timer for timing measurements. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Timer_Init(void)
{
    /* TIM2 clock = 1/2 CPU clock */
    T2CON=0;
    /* Clear counter */
    TMR2=0;
    PR2=0xFFFFUL;
    T2CON=RMP_DSPIC_TIMER_ON;
}
/* End Function:Timer_Init ***************************************************/

/* Begin Function:Int_Init ****************************************************
Description : Initialize an periodic interrupt source. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Init(void)
{
    /* TIM3 clock = 1/2 CPU clock */
    T3CON=0;
    /* Clear interrupt flag and set priority to lowest */
    IPC2bits.T3IP=1;
    IFS0bits.T3IF=0;
    /* Clear counter, period 25000 - 1ms per interrupt */
    TMR3=0;
    PR3=25000;
    T3CON=RMP_DSPIC_TIMER_ON;
    /* Enable interrupt */
    IEC0bits.T3IE=1;
}

/* The interrupt handler with shadow register sets */
void Tim3_Interrupt(void)
{
    Int_Handler();
    /* Clear flags */
    IFS0bits.T3IF=0;
}
/* End Function:Int_Init *****************************************************/

/* Begin Function:Int_Disable *************************************************
Description : Disable the periodic interrupt source. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Disable(void)
{
    /* Disable timer 3 interrupt */
    T3CON=0;
    IEC0bits.T3IE=0;
    IPC2bits.T3IP=0;
    IFS0bits.T3IF=0;
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
