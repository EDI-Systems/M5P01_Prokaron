/******************************************************************************
Filename    : rmp_test_dspic33ep512.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for DSPIC33EP512MU810.

XC16 2.10 -O3 (--spec hack enabled)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 214 / 367 / 214
Mailbox                            : 447 / 601 / 447
Semaphore                          : 353 / 505 / 353
FIFO                               : 219 / 373 / 219
Message queue                      : 608 / 762 / 608
Blocking message queue             : 851 / 1005 / 851
Memory allocation/free pair        : 448 / 468 / 438
ISR Mailbox                        : 368 / 519 / 368
ISR Semaphore                      : 278 / 431 / 278
ISR Message queue                  : 455 / 609 / 455
ISR Blocking message queue         : 602 / 756 / 602
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Counter read wrapper */
#define RMP_CNT_READ()          (TMR2)
/* Memory pool test switch */
#define TEST_MEM_POOL           (8192U)
/* Minimal build switch */
/* #define MINIMAL_SIZE */
/* Pointer size switch */
#define PTR_16_BIT
/* Timestamp data type */
typedef rmp_u16_t rmp_tim_t;

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
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
rmp_ptr_t Stack_1[512];
rmp_ptr_t Stack_2[512];

void Int_Handler(void);
/* End Global ****************************************************************/

/* Function:Timer_Init ********************************************************
Description : Initialize the timer for timing measurements. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Timer_Init(void)
{
    /* TIM2 clock = CPU clock */
    T2CON=0;
    /* Clear counter */
    TMR2=0;
    PR2=0xFFFFU;
    T2CON=RMP_DSPIC_TIM_ON|RMP_DSPIC_TIM_PRESC1;
}
/* End Function:Timer_Init ***************************************************/

/* Function:Int_Init **********************************************************
Description : Initialize an periodic interrupt source. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Init(void)
{
    /* TIM3 clock = CPU clock */
    T3CON=0U;
    /* Clear interrupt flag and set priority to lowest */
    IPC2bits.T3IP=1U;
    IFS0bits.T3IF=0U;
    /* Clear counter, period 25000 - 1ms per interrupt */
    TMR3=0U;
    PR3=25000U;
    T3CON=RMP_DSPIC_TIM_ON|RMP_DSPIC_TIM_PRESC1;
    /* Enable interrupt */
    IEC0bits.T3IE=1;
}

/* Timer 3 interrupt handler */
void Tim3_Interrupt(void)
{
    /* Clear flag */
    IFS0bits.T3IF=0U;
    Int_Handler();
}
/* End Function:Int_Init *****************************************************/

/* Function:Int_Disable *******************************************************
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
