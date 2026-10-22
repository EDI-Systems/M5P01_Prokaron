/******************************************************************************
Filename    : rmp_test_stm32g071rb.h
Author      : pry 
Date        : 19/10/2026
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for STM32G071RB.

ARMCLANG 6.23 -O3
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 264 / 364 / 260
Mailbox                            : 465 / 565 / 459
Semaphore                          : 407 / 509 / 401
FIFO                               : 209 / 313 / 206
Message queue                      : 697 / 793 / 688
Blocking message queue             : 948 / 1043 / 935
Alarm combination (1/2/3/5/7)      : 904 / 1787 / 536
ISR Mailbox                        : 463 / 566 / 455
ISR Semaphore                      : 400 / 504 / 394
ISR Message queue                  : 577 / 680 / 569
ISR Blocking message queue         : 726 / 826 / 715

GCC 13.2.1 -O3 (SysTick turned on)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 289 / 408 / 284
Mailbox                            : 536 / 675 / 525
Semaphore                          : 480 / 617 / 471
FIFO                               : 233 / 376 / 228
Message queue                      : 694 / 829 / 679
Blocking message queue             : 958 / 1088 / 936
Alarm combination (1/2/3/5/7)      : 892 / 1728 / 549
ISR Mailbox                        : 489 / 631 / 479
ISR Semaphore                      : 462 / 602 / 452
ISR Message queue                  : 599 / 737 / 586
ISR Blocking message queue         : 772 / 906 / 756

IAR 9.40.1 -O3 (SysTick turned on)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 298 / 402 / 294
Mailbox                            : 545 / 651 / 538
Semaphore                          : 480 / 584 / 474
FIFO                               : 252 / 358 / 248
Message queue                      : 762 / 863 / 752
Blocking message queue             : 1012 / 1107 / 997
Alarm combination (1/2/3/5/7)      : 1062 / 2139 / 611
ISR Mailbox                        : 532 / 636 / 524
ISR Semaphore                      : 471 / 575 / 464
ISR Message queue                  : 643 / 744 / 633
ISR Blocking message queue         : 757 / 858 / 746
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
TIM_HandleTypeDef TIM3_Handle={0};

void Timer_Init(void);
void Int_Handler(void);
void Int_Init(void);
void Int_Disable(void);
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim);
void TIM3_IRQHandler(void);
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
    /* TIM3 clock = CPU clock */
    TIM3_Handle.Instance=TIM3;
    TIM3_Handle.Init.Prescaler=0;
    TIM3_Handle.Init.CounterMode=TIM_COUNTERMODE_DOWN;
    TIM3_Handle.Init.Period=3600;
    TIM3_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM3_Handle);
    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_TIM_ENABLE(&TIM3_Handle);
    /* Clear interrupt pending bit, because we used EGR to update the registers */
    __HAL_TIM_CLEAR_IT(&TIM3_Handle, TIM_IT_UPDATE);
    HAL_TIM_Base_Start_IT(&TIM3_Handle);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3) 
    {
        /* Set the interrupt priority */
        NVIC_SetPriority(TIM3_IRQn,0xFF);
        /* Enable timer 3 interrupt */
        NVIC_EnableIRQ(TIM3_IRQn);
        /* Enable timer 3 clock */
        __HAL_RCC_TIM3_CLK_ENABLE();
    }
}

/* The interrupt handler */
void TIM3_IRQHandler(void)
{
    TIM3->SR=~TIM_FLAG_UPDATE;
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
    /* Disable timer 3 interrupt */
    NVIC_DisableIRQ(TIM3_IRQn);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
