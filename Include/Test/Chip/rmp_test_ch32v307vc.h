/******************************************************************************
Filename    : rmp_test_ch32v307vc.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for CH32V307VC.

GCC 8.2.0 -O3 (FPU disabled)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 301 / 312 / 301
Mailbox                            : 470 / 489 / 470
Semaphore                          : 418 / 430 / 418
FIFO                               : 180 / 218 / 180
Message queue                      : 636 / 676 / 636
Blocking message queue             : 831 / 853 / 831
ISR Mailbox                        : 472 / 497 / 258
ISR Semaphore                      : 424 / 448 / 424
ISR Message queue                  : 576 / 605 / 576
ISR Blocking message queue         : 695 / 706 / 695

GCC 8.2.0 -O3 (FPU context active)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 378 / 385 / 378
Mailbox                            : 546 / 576 / 546
Semaphore                          : 492 / 518 / 492
FIFO                               : 177 / 198 / 177
Message queue                      : 719 / 757 / 719
Blocking message queue             : 909 / 957 / 909
ISR Mailbox                        : 559 / 585 / 271
ISR Semaphore                      : 518 / 536 / 518
ISR Message queue                  : 669 / 707 / 669
ISR Blocking message queue         : 789 / 815 / 789

GCC 12.2.0 -O3 (FPU disabled)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 259 / 281 / 259
Mailbox                            : 438 / 471 / 438
Semaphore                          : 394 / 421 / 394
FIFO                               : 180 / 199 / 180
Message queue                      : 607 / 647 / 607
Blocking message queue             : 769 / 802 / 769
ISR Mailbox                        : 430 / 452 / 238
ISR Semaphore                      : 387 / 418 / 387
ISR Message queue                  : 541 / 564 / 541
ISR Blocking message queue         : 653 / 672 / 653

GCC 12.2.0 -O3 (FPU context active)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 334 / 349 / 334
Mailbox                            : 510 / 541 / 510
Semaphore                          : 474 / 493 / 470
FIFO                               : 180 / 202 / 180
Message queue                      : 672 / 707 / 672
Blocking message queue             : 828 / 858 / 828
ISR Mailbox                        : 521 / 540 / 259
ISR Semaphore                      : 483 / 507 / 483
ISR Message queue                  : 618 / 652 / 618
ISR Blocking message queue         : 733 / 759 / 733
******************************************************************************/

/* Includes ******************************************************************/
#include "rmp.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Where are the initial stacks */
#define THD1_STACK          (&Stack_1[1024])
#define THD2_STACK          (&Stack_2[1024])
/* How to read counter */
#define RMP_CNT_READ()      (TIM1->CNT)
/* Are we testing the memory pool? */
/* #define TEST_MEM_POOL       8192 */
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* CH32V307 timers are all 16-bit */
typedef rmp_u16_t rmp_tim_t;
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
rmp_ptr_t Stack_1[2048];
rmp_ptr_t Stack_2[2048];
TIM_TimeBaseInitTypeDef TIM4_Handle = {0};
NVIC_InitTypeDef NVIC_InitStruture = {0};

void Timer_Init(void);
void Int_Init(void);
void Int_Handler(void);
void Int_Disable(void);

/* Software-handled stack pushing/popping */
void TIM4_IRQHandler(void) __attribute__((interrupt()));
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
    /* TIM1 clock = CPU clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_CounterModeConfig(TIM1, TIM_CounterMode_Up);
    TIM_InternalClockConfig(TIM1);
    TIM_Cmd(TIM1, ENABLE);
}
/* End Function:Timer_Init ***************************************************/

/* Begin Function:Int_Init ****************************************************
Description : Initialize an periodic interrupt source. This function needs
              to be adapted to your specific hardware. The stuff initialized
              here is actually the watchdog timer.
              You MAY experience very large numbers during the test set of interrupt
              response times, because this part operates on SPI flash which have
              a huge penalty on any instruction cache miss. Just one or two of these
              misses will get you tens of thousands of cycles wasted.
              You can read 'minstret' register against 'mcycle' register to confirm this.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Init(void)
{
    /* TIM4 clock = CPU clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM4_Handle.TIM_Prescaler = 0;
    TIM4_Handle.TIM_CounterMode = TIM_CounterMode_Down;
    TIM4_Handle.TIM_Period = 14400;
    TIM4_Handle.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM4_Handle.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4,&TIM4_Handle);
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

    NVIC_InitStruture.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStruture.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruture.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruture.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruture);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
}

void TIM4_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
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
    NVIC_DisableIRQ(TIM4_IRQn);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
