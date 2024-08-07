/******************************************************************************
Filename    : rmp_test_stm32l071cb.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for STM32L071CB.

ARMCC 6.18 -O3
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 335 / 432 / 326
Mailbox                            : 581 / 672 / 567
Semaphore                          : 532 / 625 / 519
FIFO                               : 253 / 352 / 247
Message queue                      : 892 / 974 / 871
Blocking message queue             : 1167 / 1249 / 1139
ISR Mailbox                        : 554 / 650 / 542
ISR Semaphore                      : 524 / 620 / 511
ISR Message queue                  : 756 / 845 / 735
ISR Blocking message queue         : 945 / 1028 / 918

GCC 13.2.0 -O3 (SysTick turned on)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 337 / 460 / 327
Mailbox                            : 656 / 768 / 633
Semaphore                          : 600 / 718 / 580
FIFO                               : 284 / 673 / 274
Message queue                      : 947 / 1055 / 916
Blocking message queue             : 1260 / 1353 / 1217
ISR Mailbox                        : 578 / 696 / 556
ISR Semaphore                      : 602 / 719 / 580
ISR Message queue                  : 794 / 911 / 771
ISR Blocking message queue         : 1003 / 1111 / 971
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Counter read wrapper */
#define RMP_CNT_READ()    ((rmp_tim_t)(TIM2->CNT))
/* Memory pool test switch */
/* #define TEST_MEM_POOL */
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
TIM_HandleTypeDef TIM21_Handle={0};

void Timer_Init(void);
void Int_Handler(void);
void Int_Init(void);
void Int_Disable(void);
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim);
void TIM21_IRQHandler(void);
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
    /* TIM2 clock = CPU clock */
    TIM2_Handle.Instance=TIM2;
    TIM2_Handle.Init.Prescaler=0;
    TIM2_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
    TIM2_Handle.Init.Period=(rmp_tim_t)(-1);
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
    /* TIM21 clock = CPU clock */
    TIM21_Handle.Instance=TIM21;
    TIM21_Handle.Init.Prescaler=0;
    TIM21_Handle.Init.CounterMode=TIM_COUNTERMODE_DOWN;
    TIM21_Handle.Init.Period=3600;
    TIM21_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM21_Handle);
    __HAL_RCC_TIM21_CLK_ENABLE();
    __HAL_TIM_ENABLE(&TIM21_Handle);
    /* Clear interrupt pending bit, because we used EGR to update the registers */
    __HAL_TIM_CLEAR_IT(&TIM21_Handle, TIM_IT_UPDATE);
    HAL_TIM_Base_Start_IT(&TIM21_Handle);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM21) 
    {
        /* Set the interrupt priority */
        NVIC_SetPriority(TIM21_IRQn,0xFF);
        /* Enable timer 21 interrupt */
        NVIC_EnableIRQ(TIM21_IRQn);
        /* Enable timer 21 clock */
        __HAL_RCC_TIM21_CLK_ENABLE();
    }
}

/* The interrupt handler */
void TIM21_IRQHandler(void)
{
    TIM21->SR=~TIM_FLAG_UPDATE;
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
    /* Disable timer 21 interrupt */
    NVIC_DisableIRQ(TIM21_IRQn);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
