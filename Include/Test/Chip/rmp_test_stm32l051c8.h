/******************************************************************************
Filename    : rmp_test_stm32l051c8.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for STM32L051C8.
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* How to read counter */
#define RMP_CNT_READ()    ((rmp_tim_t)(TIM2->CNT))
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The STM32L0 timers are all 16 bits, so */
typedef rmp_u16_t rmp_tim_t;
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
rmp_ptr_t Stack_1[256];
rmp_ptr_t Stack_2[256];
TIM_HandleTypeDef TIM2_Handle={0};
TIM_HandleTypeDef TIM21_Handle={0};

void Timer_Init(void);
void Int_Init(void);
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim);
void TIM21_IRQHandler(void);
void Int_Disable(void);
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
