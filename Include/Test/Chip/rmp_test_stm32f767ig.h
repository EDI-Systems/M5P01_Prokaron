/******************************************************************************
Filename    : rmp_test_stm32f767ig.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for STM32F767IG.

ARMCC 6.18 -O3
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 176 / 224 / 176
Mailbox                            : 329 / 396 / 324
Semaphore                          : 277 / 384 / 276
FIFO                               : 174 / 232 / 168
Message queue                      : 510 / 612 / 492
Blocking message queue             : 694 / 764 / 692
Memory allocation/free pair        : 334 / 368 / 321
ISR Mailbox                        : 328 / 400 / 308
ISR Semaphore                      : 259 / 320 / 252
ISR Message queue                  : 413 / 492 / 400
ISR Blocking message queue         : 516 / 596 / 492

GCC 13.2.0 -O3 (SysTick turned on)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 182 / 264 / 176
Mailbox                            : 335 / 520 / 324
Semaphore                          : 288 / 380 / 288
FIFO                               : 156 / 336 / 144
Message queue                      : 473 / 620 / 444
Blocking message queue             : 643 / 880 / 640
Memory allocation/free pair        : 332 / 372 / 315
ISR Mailbox                        : 313 / 404 / 312
ISR Semaphore                      : 264 / 372 / 248
ISR Message queue                  : 375 / 544 / 372
ISR Blocking message queue         : 514 / 616 / 484
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Counter read wrapper */
#define RMP_CNT_READ()    ((TIM2->CNT)<<1)
/* Memory pool test switch */
#define TEST_MEM_POOL     (8192U)
/* Minimal build switch */
/* #define MINIMAL_SIZE */
/* Timestamp data type */
typedef rmp_ptr_t rmp_tim_t;
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
rmp_ptr_t Stack_1[256];
rmp_ptr_t Stack_2[256];
TIM_HandleTypeDef TIM2_Handle={0};
TIM_HandleTypeDef TIM4_Handle={0};
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
    /* TIM2 clock = 1/2 CPU clock */
    TIM2_Handle.Instance=TIM2;
    TIM2_Handle.Init.Prescaler=0;
    TIM2_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
    TIM2_Handle.Init.Period=(unsigned int)(-1);
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
    /* TIM4 clock = 1/2 CPU clock */
    TIM4_Handle.Instance=TIM4;
    TIM4_Handle.Init.Prescaler=0;
    TIM4_Handle.Init.CounterMode=TIM_COUNTERMODE_DOWN;
    TIM4_Handle.Init.Period=21600;
    TIM4_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    TIM4_Handle.Init.RepetitionCounter=0;
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
