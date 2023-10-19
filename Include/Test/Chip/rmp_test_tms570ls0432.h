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
Yield                              : 275 / 416 / 272
Mailbox                            : 496 / 640 / 488
Semaphore                          : 462 / 608 / 456
FIFO                               : 210 / 400 / 208
Message queue                      : 728 / 912 / 712
Blocking message queue             : 984 / 1152 / 976
Memory allocation/free pair        : 564 / 598 / 550
ISR Mailbox                        : 441 / 496 / 432
ISR Semaphore                      : 427 / 456 / 424
ISR Message queue                  : 592 / 656 / 584
ISR Blocking message queue         : 755 / 816 / 752
******************************************************************************/

/* Includes ******************************************************************/
#include "rmp.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Where are the initial stacks */
#define THD1_STACK        (&Stack_1[215])
#define THD2_STACK        (&Stack_2[215])
/* How to read counter */
#define RMP_CNT_READ()    ((rtiREG1->CNT[0].FRCx)<<3)
/* Are we testing the memory pool? */
#define TEST_MEM_POOL     6144
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The TMS570LS timers are all 32 bits, so */
typedef rmp_ptr_t rmp_tim_t;
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
rmp_ptr_t Stack_1[256];
rmp_ptr_t Stack_2[256];
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
    /* RTI/FRC0 clock = 1/8 CPU clock, already initialized. Disable compare 1 */
    rtiREG1->INTFLAG = 2U;
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
    /* RTI/FRC1 clock = 1/8 CPU clock */
    rtiREG1->INTFLAG=2U;
    rtiStopCounter(rtiCOUNTER_BLOCK1);
    rtiSetPeriod(rtiCOMPARE1, 4000);
    rtiEnableNotification(rtiNOTIFICATION_COMPARE1);
    rtiStartCounter(rtiCOUNTER_BLOCK1);
}
volatile int Acc_Count=0;
/* The interrupt handler */
void rtiInterrupt(void)
{
    Acc_Count++;
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
    /* Disable channel 1 interrupt */
    rtiDisableNotification(rtiNOTIFICATION_COMPARE1);
    rtiStopCounter(rtiCOUNTER_BLOCK1);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
