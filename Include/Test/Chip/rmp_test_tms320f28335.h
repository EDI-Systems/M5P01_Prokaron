/******************************************************************************
Filename    : rmp_test_tms320f28335.h
Author      : rz (ran zhang)
Date        : 27/04/2024
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for TMS320F28335.
              This test is very slow.

TI V22.6.1LTS -O4 -mf5 (OS timer enabled, RAM)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 246 / 369 / 246
Mailbox                            : 513 / 636 / 513
Semaphore                          : 440 / 563 / 440
FIFO                               : 235 / 358 / 235
Message queue                      : 751 / 874 / 751
Blocking message queue             : 1001 / 1124 / 1001
Memory allocation/free pair        : 946 / 965 / 935
ISR Mailbox                        : 440 / 563 / 440
ISR Semaphore                      : 413 / 536 / 413
ISR Message queue                  : 622 / 745 / 622
ISR Blocking message queue         : 770 / 893 / 770

TI V22.6.1LTS -O4 -mf5 (OS timer/FPU32 enabled, RAM)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 270 / 413 / 270
Mailbox                            : 542 / 685 / 542
Semaphore                          : 467 / 610 / 467
FIFO                               : 237 / 380 / 237
Message queue                      : 774 / 917 / 774
Blocking message queue             : 1025 / 1168 / 1025
Memory allocation/free pair        : 946 / 965 / 935
ISR Mailbox                        : 462 / 605 / 462
ISR Semaphore                      : 429 / 572 / 429
ISR Message queue                  : 635 / 778 / 635
ISR Blocking message queue         : 786 / 929 / 786

TI V22.6.1LTS -O4 -mf5 (OS timer/FPU64 enabled, RAM)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 286 / 445 / 286
Mailbox                            : 558 / 717 / 558
Semaphore                          : 483 / 642 / 483
FIFO                               : 237 / 396 / 237
Message queue                      : 790 / 949 / 790
Blocking message queue             : 1042 / 1200 / 1041
Memory allocation/free pair        : 946 / 965 / 935
ISR Mailbox                        : 470 / 629 / 470
ISR Semaphore                      : 437 / 596 / 437
ISR Message qu?ue                  : 643 / 802 / 643
ISR Blocking message queue         : 794 / 953 / 794

TI V22.6.1LTS -O4 -mf5 (OS timer enabled, Flash)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 392 / 615 / 392
Mailbox                            : 912 / 1134 / 912
Semaphore                          : 774 / 1102 / 773
FIFO                               : 469 / 694 / 469
Message queue                      : 1374 / 1597 / 1373
Blocking message queue             : 1909 / 2130 / 1907
ISR Mailbox                        : 802 / 1030 / 801
ISR Semaphore                      : 744 / 966 / 743
ISR Message queue                  : 1106 / 1329 / 1105
ISR Blocking message queue         : 1416 / 1643 / 1414
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* How to read counter */
#define RMP_CNT_READ()          (~CpuTimer1Regs.TIM.all)
/* Are we testing the memory pool? */
#define TEST_MEM_POOL           (2048U)
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The C28X timers are all 16 bits, so */
typedef rmp_u16_t rmp_tim_t;
/* The pointer is also 16-bit, resort to 32-bit accumulators */
/* #define PTR_16_BIT */
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
