/******************************************************************************
Filename    : rmp_test_stm32n657x0h3.h
Author      : pry
Date        : 02/11/2026
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for STM32N657X0H3.

GCC 13.2.1 -O3 (SysTick turned on, CPU @600MHz HSI-PLL1/SMPS/VOS1,
L1 caches enabled, image in AXISRAM1)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 163 / 233 / 160
Mailbox                            : 298 / 804 / 298
Semaphore                          : 252 / 605 / 252
FIFO                               : 135 / 467 / 132
Message queue                      : 391 / 838 / 391
Blocking message queue             : 524 / 965 / 524
Alarm combination (1/2/3/5/7)      : 527 / 1061 / 307
Memory allocation/free pair        : 307 / 447 / 296
ISR Mailbox                        : 289 / 525 / 289
ISR Semaphore                      : 256 / 510 / 256
ISR Message queue                  : 336 / 723 / 336
ISR Blocking message queue         : 427 / 530 / 427

ARMCLANG 6.23 -O3 (SysTick turned on, CPU @600MHz HSI-PLL1/SMPS/VOS1,
L1 caches enabled, image in AXISRAM1)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 155 / 220 / 155
Mailbox                            : 281 / 704 / 281
Semaphore                          : 228 / 449 / 228
FIFO                               : 130 / 369 / 130
Message queue                      : 421 / 704 / 421
Blocking message queue             : 571 / 689 / 571
Alarm combination (1/2/3/5/7)      : 555 / 1064 / 327
Memory allocation/free pair        : 309 / 471 / 298
ISR Mailbox                        : 282 / 509 / 282
ISR Semaphore                      : 246 / 506 / 246
ISR Message queue                  : 364 / 491 / 364
ISR Blocking message queue         : 454 / 565 / 454
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Counter read wrapper - DWT cycle counter, CPU clock = 600MHz */
#define RMP_CNT_READ()    (DWT->CYCCNT)
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
    /* Enable the cycle counter (DWT is unlocked out of reset on Cortex-M55) */
    CoreDebug->DEMCR|=CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT=0U;
    DWT->CTRL|=DWT_CTRL_CYCCNTENA_Msk;
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
    /* TIM4 clock = 100MHz PCLK1, period = 20000 counts = 5kHz */
    TIM4_Handle.Instance=TIM4;
    TIM4_Handle.Init.Prescaler=0;
    TIM4_Handle.Init.CounterMode=TIM_COUNTERMODE_DOWN;
    TIM4_Handle.Init.Period=20000;
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
    /* Feed the factory-demo watchdog every millisecond; see the chip header */
    IWDG->KR=0xAAAAU;
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
