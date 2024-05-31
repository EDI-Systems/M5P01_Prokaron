/******************************************************************************
Filename    : rmp_test_r5f104pj.h
Author      : lht
Date        : 24/04/2024
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for R5F104PJ.
              How the timers are used:
              TDR00 - System tick timer, @ Fcpu, producing 1ms interrupts.
              TDR01 - Generate interrupts, @ Fcpu, about every 10000 clock cycles.
              TDR02 - Clock counting timer, @ Fcpu, free running.
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 322 / 322 / 322
Mailbox                            : 631 / 692 / 594
Semaphore                          : 647 / 707 / 609
FIFO                               : 368 / 690 / 347
Message queue                      : 1149 / 1180 / 1082
Blocking message queue             : 1515 / 1524 / 1426
ISR Mailbox                        : 600 / 668 / 570
ISR Semaphore                      : 636 / 697 / 599
ISR Message queue                  : 1034 / 1072 / 974
ISR Blocking message queue         : 1262 / 1293 / 1195
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
#include "iodefine.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* How to read counter */
#define RMP_CNT_READ()      (~TCR02)
/* Are we testing the memory pool? */
/* #define TEST_MEM_POOL */
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* RL78 timers are 16 bits */
typedef rmp_u16_t rmp_tim_t;
/* The pointer is also 16-bit, resort to 32-bit accumulators */
#define PTR_16_BIT
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
{   /* supplies input clock */
    TAU0EN=1U;
    TPS0=0x0100U|0x8000U;
    TT0=0x0001U|0x0002U|0x0004U|0x0008U|0x0200U|0x0800U;
    /* Mask channel 0 and 1 and 2 interrupt */
    TMMK00=1U;
    TMIF00=0U;
    TMMK01=1U;
    TMIF01=0U;
    TMMK01=1U;
    TMIF01=0U;
    TMMK02=1U;
    TMIF02=0U;
    /* Set INTTM00 INTTM01 high priority */
    TMPR101=0U;
    TMPR001=0U;
    TMPR100=0U;
    TMPR000=0U;
    /* Channel 0 used as interval timer */
    TMR00=0x0000U;
    TDR00=0x067FU;
    TOM0&=~0x0001U;
    TOL0&=~0x0001U;
    TO0&=~0x0001U;
    TOE0&=~0x0001U;
    /* Channel 1 used as interval timer */
    TMR01=0x0000U;
    TDR01=0x0C7FU;
    TOM0&=~0x0002U;
    TOL0&=~0x0002U;
    TO0&=~0x0002U;
    TOE0&=~0x0002U;
    /* Channel 2 used as interval timer */
    TMR02=0x0000U;
    TDR02=0xFFFFU;
    TCR02=0xFFFFU;
    TOM0&=~0x0004U;
    TOL0&=~0x0004U;
    TO0&=~0x0004U;
    TOE0&=~0x0004U;
    /* start Channel 0 and 1 and 2 */
    TS0|=0x0001U;
    TS0|=0x0002U;
    TS0|=0x0004U;
    /* enable INTTM00 interrupt */
    TMIF00=0U;
    TMMK00=0U;
}

/* End Function:Timer_Init ***************************************************/

/* Function:Int_Init **********************************************************
Description : Initialize a periodic interrupt source. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Init(void)
{   /* clear INTTM01 interrupt flag and enable INTTM01 interrupt */
    TMIF01=0U;
    TMMK01=0U;
}
/* The interrupt handler */
void TDR01_IRQHandler(void)
{   /* Clear interrupt request flag */
    TMIF01=0;
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
    /* Disable TAU0 channel 1 interrupt */
    TMIF01=0U;
    TMMK01=1U;
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
