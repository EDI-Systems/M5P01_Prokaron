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
Yield                              : 347 / 354 / 347
Mailbox                            : 512 / 542 / 512
Semaphore                          : 474 / 495 / 474
FIFO                               : 179 / 192 / 179
Message queue                      : 685 / 730 / 685
Blocking message queue             : 881 / 932 / 881
ISR Mailbox                        : 506 / 521 / 251
ISR Semaphore                      : 484 / 491 / 484
ISR Message queue                  : 614 / 656 / 614
ISR Blocking message queue         : 737 / 752 / 737

GCC 8.2.0 -O3 (FPU context active)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 413 / 429 / 413
Mailbox                            : 586 / 620 / 586
Semaphore                          : 543 / 561 / 543
FIFO                               : 180 / 201 / 180
Message queue                      : 752 / 813 / 752
Blocking message queue             : 938 / 969 / 938
ISR Mailbox                        : 598 / 626 / 271
ISR Semaphore                      : 573 / 585 / 573
ISR Message queue                  : 707 / 734 / 707
ISR Blocking message queue         : 825 / 844 / 825

GCC 12.2.0 -O3 (FPU disabled)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 301 / 317 / 301
Mailbox                            : 487 / 508 / 487
Semaphore                          : 436 / 466 / 436
FIFO                               : 180 / 196 / 180
Message queue                      : 665 / 695 / 665
Blocking message queue             : 848 / 889 / 848
ISR Mailbox                        : 473 / 491 / 230
ISR Semaphore                      : 434 / 468 / 434
ISR Message queue                  : 582 / 601 / 582
ISR Blocking message queue         : 711 / 727 / 711

GCC 12.2.0 -O3 (FPU context active)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 377 / 401 / 377
Mailbox                            : 561 / 594 / 561
Semaphore                          : 508 / 536 / 508
FIFO                               : 179 / 195 / 179
Message queue                      : 731 / 777 / 731
Blocking message queue             : 916 / 947 / 916
ISR Mailbox                        : 564 / 587 / 257
ISR Semaphore                      : 529 / 546 / 529
ISR Message queue                  : 679 / 703 / 679
ISR Blocking message queue         : 800 / 814 / 800
******************************************************************************/

/* Includes ******************************************************************/
#include "rmp.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Where are the initial stacks */
#define THD1_STACK          (&Stack_1[1024])
#define THD2_STACK          (&Stack_2[1024])
/* How to read counter */
#define COUNTER_READ()      (TIM1->CNT)
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
