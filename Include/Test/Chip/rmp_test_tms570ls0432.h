/******************************************************************************
Filename    : rmp_test_tms570ls0432.h
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
Yield                              : 275 / 408 / 272
Mailbox                            : 479 / 624 / 472
Semaphore                          : 467 / 616 / 464
FIFO                               : 216 / 424 / 216
Message queue                      : 746 / 960 / 728
Blocking message queue             : 998 / 1208 / 984
Memory allocation/free pair        : 482 / 513 / 469
ISR Mailbox                        : 440 / 512 / 440
ISR Semaphore                      : 435 / 464 / 424
ISR Message queue                  : 595 / 648 / 592
ISR Blocking message queue         : 763 / 832 / 760
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Counter read wrapper */
#define RMP_CNT_READ()    ((rtiREG1->CNT[0].FRCx)<<3)
/* Memory pool test switch */
#define TEST_MEM_POOL     (6144U)
/* Minimal build switch */
/* #define MINIMAL_SIZE */
/* Timestamp data type */
typedef rmp_ptr_t rmp_tim_t;
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
rmp_ptr_t Stack_1[256];
rmp_ptr_t Stack_2[256];
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
    rtiDisableNotification(rtiNOTIFICATION_COMPARE1);
    rtiStopCounter(rtiCOUNTER_BLOCK1);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
