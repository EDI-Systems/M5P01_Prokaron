/******************************************************************************
Filename    : rmp_test_stm32f405rg.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for STM32F405RG.

ARMCC 6.18 -O3
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 180 / 260 / 180
Mailbox                            : 345 / 444 / 344
Semaphore                          : 321 / 412 / 320
FIFO                               : 180 / 264 / 180
Message queue                      : 667 / 768 / 664
Blocking message queue             : 886 / 1016 / 880
Memory allocation/free pair        : 455 / 488 / 448
ISR Mailbox                        : 309 / 416 / 308
ISR Semaphore                      : 302 / 388 / 300
ISR Message queue                  : 498 / 584 / 496
ISR Blocking message queue         : 626 / 720 / 624

GCC 13.2.0 -O3 (SysTick turned on)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 196 / 280 / 196
Mailbox                            : 388 / 568 / 384
Semaphore                          : 345 / 436 / 344
FIFO                               : 192 / 276 / 192
Message queue                      : 677 / 808 / 672
Blocking message queue             : 953 / 1064 / 948
Memory allocation/free pair        : 411 / 426 / 404
ISR Mailbox                        : 381 / 512 / 380
ISR Semaphore                      : 349 / 444 / 348
ISR Message queue                  : 566 / 660 / 556
ISR Blocking message queue         : 743 / 856 / 740
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Counter read wrapper */
#define RMP_CNT_READ()      ((rmp_tim_t)((TIM2->CNT)<<1))
/* Memory pool test switch */
#define TEST_MEM_POOL       (8192U)
/* Minimal build switch */
/* #define MINIMAL_SIZE */
/* Timestamp data type */
typedef rmp_u16_t           rmp_tim_t;
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
    TIM4_Handle.Init.Period=16800;
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
