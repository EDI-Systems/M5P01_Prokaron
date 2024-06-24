/******************************************************************************
Filename    : rmp_test_stm32f107vc.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for STM32F107VC.

ARMCC 6.18 -O3
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 187 / 260 / 186
Mailbox                            : 427 / 492 / 420
Semaphore                          : 359 / 432 / 356
FIFO                               : 230 / 304 / 228
Message queue                      : 656 / 730 / 650
Blocking message queue             : 906 / 978 / 898
Memory allocation/free pair        : 522 / 548 / 508
ISR Mailbox                        : 393 / 470 / 390
ISR Semaphore                      : 338 / 414 / 336
ISR Message queue                  : 536 / 608 / 528
ISR Blocking message queue         : 679 / 752 / 674

ARMCC 6.18 -O3 (RMP_ASSERT_ENABLE=0)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 187 / 264 / 186
Mailbox                            : 422 / 496 / 418
Semaphore                          : 351 / 416 / 348
FIFO                               : 204 / 282 / 202
Message queue                      : 628 / 702 / 622
Blocking message queue             : 856 / 928 / 848
Memory allocation/free pair        : 519 / 547 / 505
ISR Mailbox                        : 396 / 470 / 390
ISR Semaphore                      : 334 / 412 / 332
ISR Message queue                  : 511 / 588 / 508
ISR Blocking message queue         : 643 / 714 / 634

ARMCC 6.18 -Oz -flto
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 200 / 286 / 198
Mailbox                            : 508 / 594 / 502
Semaphore                          : 418 / 504 / 414
FIFO                               : 299 / 386 / 296
Message queue                      : 843 / 914 / 836
Blocking message queue             : 1143 / 1220 / 1130
Memory allocation/free pair        : 621 / 643 / 608
ISR Mailbox                        : 441 / 528 / 436
ISR Semaphore                      : 374 / 460 / 368
ISR Message queue                  : 607 / 692 / 602
ISR Blocking message queue         : 754 / 836 / 746
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Counter read wrapper */
#define RMP_CNT_READ()    (TIM2->CNT)
/* Memory pool test switch */
#define TEST_MEM_POOL     (8192U)
/* Minimal build switch */
/* #define MINIMAL_SIZE */
/* Timestamp data type */
typedef rmp_u16_t rmp_tim_t;
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
rmp_ptr_t Stack_1[256];
rmp_ptr_t Stack_2[256];
TIM_HandleTypeDef TIM2_Handle={0};   
TIM_HandleTypeDef TIM4_Handle={0};

void Timer_Init(void);
void Int_Init(void);
void Int_Handler(void);
void Int_Disable(void);

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim);
void TIM4_IRQHandler(void);
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
    /* TIM2 clock = CPU clock 
     * TIM2 is on APB1, APB1 clock = 1/2 CPU clock (DIV2 when running at 72MHz
     * to satisfy APB1 36MHz Fmax limit). However, when APB1DIV is not DIV1, 
     * TIM2CLK = 2 x APB1CLK (which is STM32 timer specific). */
    TIM2_Handle.Instance=TIM2;
    TIM2_Handle.Init.Prescaler=0;
    TIM2_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
    TIM2_Handle.Init.Period=65535U;
    TIM2_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM2_Handle);
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_TIM_ENABLE(&TIM2_Handle);
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
    TIM4_Handle.Instance=TIM4;
    TIM4_Handle.Init.Prescaler=0;
    TIM4_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
    TIM4_Handle.Init.Period=7200U;
    TIM4_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM4_Handle);
    __HAL_RCC_TIM4_CLK_ENABLE();
    __HAL_TIM_ENABLE(&TIM4_Handle);
    /* Clear interrupt pending bit, because we used EGR to update the registers */
    __HAL_TIM_CLEAR_IT(&TIM4_Handle, TIM_IT_UPDATE);
    HAL_TIM_Base_Start_IT(&TIM4_Handle);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM4) 
    {
        /* Set the interrupt priority */
        NVIC_SetPriority(TIM4_IRQn,0xFF);
        /* Enable timer 4 interrupt */
        NVIC_EnableIRQ(TIM4_IRQn);
        /* Enable timer 4 clock */
        __HAL_RCC_TIM4_CLK_ENABLE();
    }
}

/* The interrupt handler */
void TIM4_IRQHandler(void)
{
    TIM4->SR=~TIM_FLAG_UPDATE;
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
    /* Disable timer 4 interrupt */
    NVIC_DisableIRQ(TIM4_IRQn);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
