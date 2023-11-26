/******************************************************************************
Filename    : rmp_test_ch32v307vc.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for CH32V307VC.

GCC 12.2.0 -O3 (FPU disabled)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 246 / 256 / 246
Mailbox                            : 426 / 451 / 426
Semaphore                          : 386 / 409 / 386
FIFO                               : 179 / 196 / 179
Message queue                      : 605 / 652 / 605
Blocking message queue             : 767 / 789 / 767
ISR Mailbox                        : 359 / 382 / 287
ISR Semaphore                      : 321 / 352 / 321
ISR Message queue                  : 466 / 499 / 466
ISR Blocking message queue         : 593 / 609 / 593

GCC 12.2.0 -O3 (FPU context active)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 318 / 330 / 318
Mailbox                            : 495 / 522 / 495
Semaphore                          : 457 / 478 / 457
FIFO                               : 182 / 204 / 182
Message queue                      : 674 / 718 / 674
Blocking message queue             : 836 / 865 / 836
ISR Mailbox                        : 405 / 430 / 330
ISR Semaphore                      : 366 / 390 / 366
ISR Message queue                  : 500 / 523 / 500
ISR Blocking message queue         : 624 / 647 / 624
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
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
/* End Define ****************************************************************/

/* Global ********************************************************************/
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
void _TIM4_IRQHandler(void);
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
    /* TIM1 clock = CPU clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_CounterModeConfig(TIM1, TIM_CounterMode_Up);
    TIM_InternalClockConfig(TIM1);
    TIM_Cmd(TIM1, ENABLE);
}
/* End Function:Timer_Init ***************************************************/

/* Function:Int_Init **********************************************************
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
    TIM4_Handle.TIM_Period = 14400*4;
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

void _TIM4_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
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
    NVIC_DisableIRQ(TIM4_IRQn);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
