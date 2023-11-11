/******************************************************************************
Filename    : rmp_test_msp430f149.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for MSP430F149.
              This test is very slow. It takes typ.10 min to run @ 8MHz.

TICC V21.6.1LTS -O4 -mf5 (OS timer enabled)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 312 / 476 / 306
Mailbox                            : 641 / 801 / 631
Semaphore                          : 573 / 734 / 564
FIFO                               : 312 / 477 / 307
Message queue                      : 985 / 1139 / 969
Blocking message queue             : 1278 / 1427 / 1257
ISR Mailbox                        : 528 / 688 / 518
ISR Semaphore                      : 487 / 648 / 478
ISR Message queue                  : 739 / 895 / 725
ISR Blocking message queue         : 898 / 1052 / 882
******************************************************************************/

/* Includes ******************************************************************/
#include "rmp.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* How to read counter */
#define RMP_CNT_READ()          (TBR)
/* Are we testing the memory pool? */
/* #define TEST_MEM_POOL */
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The MSP430 timers are all 16 bits, so */
typedef rmp_u16_t rmp_tim_t;
/* The pointer is also 16-bit, resort to 32-bit accumulators */
#define PTR_16_BIT
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
rmp_ptr_t Stack_1[128];
rmp_ptr_t Stack_2[128];

void Timer_Init(void);
void Int_Init(void);
void Int_Handler(void);
void Int_Disable(void);
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
    /* TIMB clock = CPU clock */
    TBCCR0=65535;
    TBCTL=TBSSEL_2|MC_1;
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
    /* Make sure that we also generate interrupts from the same timer */
    TBCCTL0=CCIE;
}

/* The interrupt handler */
void TIM1_IRQHandler(void)
{
    TBCCTL0&=~CCIFG;
    Int_Handler();
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
    /* Disable TIMB interrupt */
    TBCCTL0=~CCIE;
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
