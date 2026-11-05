/******************************************************************************
Filename    : rmp_test_m2351kiaae.h
Author      : pry
Date        : 03/11/2026
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for Nuvoton M2351KIAAE (NuMaker-PFM-M2351).

GCC 13.2.1 -O3 (SysTick turned on, CPU @48MHz from HXT 12MHz -> PLL,
no caches, image in APROM, UART0 @115200 clocked from the same PLL)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 265 / 408 / 264
Mailbox                            : 554 / 699 / 552
Semaphore                          : 538 / 744 / 534
FIFO                               : 210 / 354 / 210
Message queue                      : 706 / 897 / 702
Blocking message queue             : 1008 / 1194 / 1005
Alarm combination (1/2/3/5/7)      : 763 / 1368 / 483
Memory allocation/free pair        : 674 / 737 / 646
ISR Mailbox                        : 492 / 597 / 492
ISR Semaphore                      : 444 / 561 / 444
ISR Message queue                  : 565 / 738 / 564
ISR Blocking message queue         : 751 / 870 / 750

Keil MDK 5 (ARMCLANG 6.23) -O3 -otime (SysTick turned on, CPU @48MHz from HXT 12MHz -> PLL,
no caches, image in APROM, UART0 @115200 clocked from the same PLL)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 213 / 303 / 213
Mailbox                            : 390 / 522 / 390
Semaphore                          : 349 / 444 / 348
FIFO                               : 197 / 339 / 195
Message queue                      : 675 / 807 / 672
Blocking message queue             : 957 / 1086 / 954
Alarm combination (1/2/3/5/7)      : 777 / 1584 / 462
Memory allocation/free pair        : 498 / 542 / 483
ISR Mailbox                        : 441 / 519 / 441
ISR Semaphore                      : 339 / 420 / 339
ISR Message queue                  : 535 / 588 / 534
ISR Blocking message queue         : 655 / 735 / 651

IAR 9.40.1 -Oh (SysTick turned on, CPU @48MHz from HXT 12MHz -> PLL,
no caches, image in APROM, UART0 @115200 clocked from the same PLL)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 228 / 321 / 228
Mailbox                            : 512 / 606 / 510
Semaphore                          : 387 / 471 / 387
FIFO                               : 211 / 321 / 210
Message queue                      : 642 / 735 / 639
Blocking message queue             : 887 / 978 / 885
Alarm combination (1/2/3/5/7)      : 900 / 1860 / 531
Memory allocation/free pair        : 501 / 547 / 484
ISR Mailbox                        : 444 / 528 / 444
ISR Semaphore                      : 366 / 414 / 366
ISR Message queue                  : 522 / 642 / 522
ISR Blocking message queue         : 663 / 690 / 663
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Counter read wrapper - TIMER0 free-runs at 48MHz. TIMER0 counters are 24
 * bits wide, so only the low 16 are used: differences then wrap correctly
 * modulo 65536 against the rmp_u16_t timestamp type, the same way the other
 * 16-bit-counter ports (e.g. STM32F103) do it. */
#define RMP_CNT_READ()      ((rmp_u16_t)(TIMER0->CNT))
/* Memory pool test switch */
#define TEST_MEM_POOL       (8192U)
/* Minimal build switch */
/* #define MINIMAL_SIZE */
/* Timestamp data type */
typedef rmp_u16_t rmp_tim_t;
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
    /* TIMER0 free running, clocked from PCLK0 (HCLK/1 = 48MHz) so that one count is
     * one CPU cycle */
    SYS_UnlockReg();
    CLK_EnableModuleClock(TMR0_MODULE);
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_PCLK0, 0U);
    SYS_LockReg();
    TIMER_Open(TIMER0, TIMER_CONTINUOUS_MODE, 1U);
    /* TIMER_Open picks a prescaler from the requested frequency; force 1:1 so
     * that one count is one CPU cycle, and let the counter run through its
     * whole 24-bit range so the low 16 bits wrap at a multiple of 65536 */
    TIMER0->CTL&=~TIMER_CTL_PSC_Msk;
    TIMER0->CMP=0xFFFFFFUL;
    TIMER_Start(TIMER0);
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
    /* TIMER1 periodic interrupt at 5kHz, clocked from PCLK0 as well */
    SYS_UnlockReg();
    CLK_EnableModuleClock(TMR1_MODULE);
    CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1SEL_PCLK0, 0U);
    SYS_LockReg();
    TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, 5000U);
    TIMER_EnableInt(TIMER1);
    NVIC_SetPriority(TMR1_IRQn, 3U);
    NVIC_EnableIRQ(TMR1_IRQn);
    TIMER_Start(TIMER1);
}

/* The interrupt handler */
void TMR1_IRQHandler(void)
{
    TIMER_ClearIntFlag(TIMER1);
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
    /* Disable timer 1 interrupt */
    NVIC_DisableIRQ(TMR1_IRQn);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
