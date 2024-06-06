/******************************************************************************
Filename    : rmp_test_pic24fj128.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for DSPIC33EP512MU810.
              This test is NOT particularly fast and can take up to one minute.

XC16 2.10 -O3 (--spec hack enabled)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 152 / 252 / 152
Mailbox                            : 334 / 434 / 334
Semaphore                          : 271 / 371 / 271
FIFO                               : 168 / 268 / 168
Message queue                      : 468 / 568 / 468
Blocking message queue             : 654 / 754 / 654
Memory allocation/free pair        : 379 / 393 / 370
ISR Mailbox                        : 274 / 374 / 274
ISR Semaphore                      : 213 / 313 / 213
ISR Message queue                  : 352 / 452 / 352
ISR Blocking message queue         : 461 / 561 / 461
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Counter read wrapper */
#define RMP_CNT_READ()          (TMR2)
/* Memory pool test switch */
#define TEST_MEM_POOL           (2048U)
/* Minimal build switch */
/* #define MINIMAL_SIZE */
/* Pointer size switch */
#define PTR_16_BIT
/* Timestamp data type */
typedef rmp_u16_t rmp_tim_t;

/* The pragmas for PIC16 fuse */
/* CONFIG2 */
#pragma config POSCMOD = XT             /* Primary Oscillator Select (HS Oscillator mode selected) */
#pragma config OSCIOFNC = OFF           /* Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2)) */
#pragma config FCKSM = CSDCMD           /* Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled) */
#pragma config FNOSC = PRIPLL           /* Oscillator Select (Primary Oscillator with PLL module (HSPLL, ECPLL)) */
#pragma config IESO = ON                /* Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled) */

/* CONFIG1 */
#pragma config WDTPS = PS32768          /* Watchdog Timer Postscaler (1:32,768) */
#pragma config FWPSA = PR128            /* WDT Prescaler (Prescaler ratio of 1:128) */
#pragma config WINDIS = ON              /* Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled)) */
#pragma config FWDTEN = OFF             /* Watchdog Timer Enable (Watchdog Timer is disabled) */
#pragma config ICS = PGx2               /* Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2) */
#pragma config GWRP = OFF               /* General Code Segment Write Protect (Writes to program memory are allowed) */
#pragma config GCP = OFF                /* General Code Segment Code Protect (Code protection is disabled) */
#pragma config JTAGEN = OFF             /* JTAG Port Enable (JTAG port is disabled) */
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
rmp_ptr_t Stack_1[128];
rmp_ptr_t Stack_2[128];

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
