/******************************************************************************
Filename    : rmp_test_TMS570LC4357.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for TMS570LC4357.

TICC V20.2.7LTS -O4 -mf5
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 306 / 520 / 304
Mailbox                            : 493 / 688 / 472
Semaphore                          : 460 / 696 / 456
FIFO                               : 193 / 656 / 192
Message queue                      : 686 / 904 / 672
Blocking message queue             : 897 / 1128 / 880
Memory allocation/free pair        : 533 / 632 / 520
ISR Mailbox                        : 480 / 592 / 480
ISR Semaphore                      : 464 / 704 / 464
ISR Message queue                  : 592 / 720 / 592
ISR Blocking message queue         : 736 / 856 / 736
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* How to read counter */
#define RMP_CNT_READ()    ((rtiREG1->CNT[0].FRCx)<<3)
/* Are we testing the memory pool? */
#define TEST_MEM_POOL     8192
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The TMS570LC timers are all 32 bits, so */
typedef rmp_ptr_t rmp_tim_t;
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
rmp_ptr_t Stack_1[256];
rmp_ptr_t Stack_2[256];

void Timer_Init(void);
void Int_Handler(void);
void Int_Disable(void);
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
    /* RTI/FRC0 clock = 1/8 CPU clock, already initialized. Disable compare 1 */
    rtiREG1->INTFLAG = 2U;
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
    /* RTI/FRC1 clock = 1/8 CPU clock */
    rtiREG1->INTFLAG = 2U;
    rtiStopCounter(rtiREG1, rtiCOUNTER_BLOCK1);
    rtiSetPeriod(rtiREG1, rtiCOMPARE1, 3750);
    rtiEnableNotification(rtiREG1, rtiNOTIFICATION_COMPARE1);
    rtiStartCounter(rtiREG1, rtiCOUNTER_BLOCK1);
}
volatile int Acc_Count=0;
/* The interrupt handler */
void rtiInterrupt(void)
{
    Acc_Count++;
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
    /* Disable channel 1 interrupt */
    rtiDisableNotification(rtiREG1, rtiNOTIFICATION_COMPARE1);
    rtiStopCounter(rtiREG1, rtiCOUNTER_BLOCK1);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
