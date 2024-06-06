/******************************************************************************
Filename    : rmp_test_tms320f2812.h
Author      : rz (ran zhang)
Date        : 27/04/2024
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for TMS320F2812.
              This test is very slow; 2812 has even less RAM than 28335, which
              makes it not that great for a RTOS.

TI V22.6.1LTS -O4 -mf5 (OS timer enabled, RAM)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 217 / 333 / 217
Mailbox                            : 493 / 609 / 493
Semaphore                          : 407 / 523 / 407
FIFO                               : 229 / 345 / 229
Message queue                      : 706 / 822 / 706
Blocking message queue             : 954 / 1070 / 954
Memory allocation/free pair        : 939 / 955 / 929
ISR Mailbox                        : 436 / 552 / 436
ISR Semaphore                      : 381 / 497 / 381
ISR Message queue                  : 583 / 699 / 583
ISR Blocking message queue         : 727 / 843 / 727
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Counter read wrapper */
#define RMP_CNT_READ()          (~CpuTimer1Regs.TIM.all)
/* Memory pool test switch */
#define TEST_MEM_POOL           (2048U)
/* Minimal build switch */
/* #define MINIMAL_SIZE */
/* Timestamp data type */
typedef rmp_u16_t rmp_tim_t;
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
rmp_ptr_t Stack_1[512];
rmp_ptr_t Stack_2[512];

void Timer_Init(void);
void Int_Init(void);
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
    CpuTimer1Regs.TCR.bit.TSS=1U;
    CpuTimer1Regs.PRD.all=0xFFFFU;
    CpuTimer1Regs.TPR.all=0U;
    CpuTimer1Regs.TPRH.all=0U;
    CpuTimer1Regs.TCR.bit.TRB=1U;
    CpuTimer1Regs.TCR.bit.TIE=0U;
    CpuTimer1Regs.TCR.bit.TSS=0U;
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
    CpuTimer1Regs.TCR.bit.TIF=0U;
    CpuTimer1Regs.TCR.bit.TIE=1U;
}

/* The interrupt handler */
void TIM1_IRQHandler(void)
{
    /* Write 1 to clear - see user manual */
    CpuTimer1Regs.TCR.bit.TIF=1U;
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
    CpuTimer1Regs.TCR.bit.TIE=0U;
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
