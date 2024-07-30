/******************************************************************************
Filename    : rmp_test_ch32v307vc.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for CH32V307VC. This chip carries multiple esoteric
              features; see the standard configuration header for details. This
              test assumes 32k RAM/192k Flash to be compatible with all memory
              configurations.
              
GCC 12.2.0 -O3 (FPU disabled)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 209 / 396 / 209
Mailbox                            : 386 / 592 / 386
Semaphore                          : 336 / 517 / 333
FIFO                               : 172 / 363 / 172
Message queue                      : 538 / 738 / 537
Blocking message queue             : 698 / 895 / 692
Memory allocation/free pair        : 433 / 460 / 428
ISR Mailbox                        : 350 / 379 / 287
ISR Semaphore                      : 306 / 336 / 305
ISR Message queue                  : 436 / 484 / 436
ISR Blocking message queue         : 555 / 752 / 551

GCC 12.2.0 -O3 (FPU context active)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 217 / 410 / 217
Mailbox                            : 398 / 601 / 394
Semaphore                          : 341 / 531 / 341
FIFO                               : 172 / 379 / 172
Message queue                      : 557 / 758 / 554
Blocking message queue             : 705 / 913 / 703
Memory allocation/free pair        : 433 / 461 / 428
ISR Mailbox                        : 358 / 556 / 294
ISR Semaphore                      : 307 / 335 / 306
ISR Message queue                  : 444 / 469 / 443
ISR Blocking message queue         : 556 / 582 / 555
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Counter read wrapper */
#define RMP_CNT_READ()      (TIM1->CNT)
/* Memory pool test switch */
#define TEST_MEM_POOL       (4096U)
/* Minimal build switch */
/* #define MINIMAL_SIZE */
/* Timestamp data type */
typedef rmp_u16_t rmp_tim_t;
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
rmp_ptr_t Stack_1[1024];
rmp_ptr_t Stack_2[1024];
TIM_TimeBaseInitTypeDef TIM4_Handle={0};
NVIC_InitTypeDef NVIC_InitStruture={0};

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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
    TIM_CounterModeConfig(TIM1, TIM_CounterMode_Up);
    TIM_InternalClockConfig(TIM1);
    TIM_Cmd(TIM1, ENABLE);
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
    /* TIM4 clock = CPU clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    TIM4_Handle.TIM_Prescaler=0U;
    TIM4_Handle.TIM_CounterMode=TIM_CounterMode_Down;
    TIM4_Handle.TIM_Period=14400U*4U;
    TIM4_Handle.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM4_Handle.TIM_RepetitionCounter=0U;
    TIM_TimeBaseInit(TIM4,&TIM4_Handle);
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);

    NVIC_InitStruture.NVIC_IRQChannel=TIM4_IRQn;
    NVIC_InitStruture.NVIC_IRQChannelPreemptionPriority=0U;
    NVIC_InitStruture.NVIC_IRQChannelSubPriority=0U;
    NVIC_InitStruture.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStruture);
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM4,ENABLE);
}

void _TIM4_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
    TIM_ClearFlag(TIM4,TIM_FLAG_Update);
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
