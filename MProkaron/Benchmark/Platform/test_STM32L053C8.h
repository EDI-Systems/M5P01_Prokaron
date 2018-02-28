/******************************************************************************
Filename    : test_STM32L053C8.h
Author      : pry 
Date        : 22/07/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The testbench for STM32L053C8.
******************************************************************************/

/* Includes ******************************************************************/
#include "RMP.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Where are the initial stacks */
#define THD1_STACK        (&Stack_1[230])
#define THD2_STACK        (&Stack_2[230])
/* How to read counter */
#define COUNTER_READ()    (TIM2->CNT)
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The STM32L0 timers are all 16 bits, so */
typedef u16 tim_t;
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
ptr_t Stack_1[256];
ptr_t Stack_2[256];
TIM_HandleTypeDef TIM2_Handle={0};
TIM_HandleTypeDef TIM21_Handle={0};
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
    /* TIM2 clock = CPU clock */
    TIM2_Handle.Instance=TIM2;
    TIM2_Handle.Init.Prescaler=0;
    TIM2_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
    TIM2_Handle.Init.Period=(u16)(-1);
    TIM2_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM2_Handle);
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_TIM_ENABLE(&TIM2_Handle);
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

/* Begin Function:Int_Disable *************************************************
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
