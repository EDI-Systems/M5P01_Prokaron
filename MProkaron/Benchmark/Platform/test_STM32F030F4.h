/******************************************************************************
Filename    : test_STM32F030F4.h
Author      : pry 
Date        : 22/07/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The testbench for STM32F030F4.
******************************************************************************/

/* Includes ******************************************************************/
#include "RMP.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Where are the initial stacks */
#define THD1_STACK        (&Stack_1[100])
#define THD2_STACK        (&Stack_2[100])
/* How to read counter */
#define COUNTER_READ()    (TIM3->CNT)
/* Are we doing minimal measurements? */
#define MINIMAL_SIZE
/* The STM32F0 timers are all 16 bits, so */
typedef u16 tim_t;
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
ptr_t Stack_1[128];
struct RMP_Thd Thd_1={0};
ptr_t Stack_2[128];
struct RMP_Thd Thd_2={0};
struct RMP_Sem Sem_1={0};
TIM_HandleTypeDef TIM3_Handle={0};
TIM_HandleTypeDef TIM14_Handle={0};
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
    /* TIM3 clock = CPU clock */
    TIM3_Handle.Instance=TIM3;
    TIM3_Handle.Init.Prescaler=0;
    TIM3_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
    TIM3_Handle.Init.Period=(u16)(-1);
    TIM3_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM3_Handle);
    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_TIM_ENABLE(&TIM3_Handle);
}
/* End Function:Timer_Init ***************************************************/

/* Begin Function:Int_Init ****************************************************
Description : Initialize an periodic interrupt source. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Init(void)
{
    /* TIM14 clock = CPU clock */
    TIM14_Handle.Instance=TIM14;
    TIM14_Handle.Init.Prescaler=0;
    TIM14_Handle.Init.CounterMode=TIM_COUNTERMODE_DOWN;
    TIM14_Handle.Init.Period=3600;
    TIM14_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM14_Handle);
    __HAL_RCC_TIM14_CLK_ENABLE();
    __HAL_TIM_ENABLE(&TIM14_Handle);
	/* Clear interrupt pending bit, because we used EGR to update the registers */
	__HAL_TIM_CLEAR_IT(&TIM14_Handle, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT(&TIM14_Handle);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM14) 
    {
		/* Set the interrupt priority */
		NVIC_SetPriority(TIM14_IRQn,0xFF);
		/* Enable timer 21 interrupt */
		NVIC_EnableIRQ(TIM14_IRQn);
		/* Enable timer 21 clock */
		__HAL_RCC_TIM14_CLK_ENABLE();
	}
}

/* The interrupt handler */
void TIM14_IRQHandler(void)
{
    TIM14->SR=~TIM_FLAG_UPDATE;
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
    /* Disable timer 14 interrupt */
    NVIC_DisableIRQ(TIM14_IRQn);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
