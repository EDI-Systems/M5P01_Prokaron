/******************************************************************************
Filename    : test_STM32F767IG.h
Author      : pry 
Date        : 22/07/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The testbench for STM32F767IG.
******************************************************************************/

/* Includes ******************************************************************/
#include "RMP.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* How to read counter */
#define COUNTER_READ()   ((TIM2->CNT)<<1)
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The STM32F7 timers are all 32 bits, so */
typedef ptr_t tim_t;
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
ptr_t Stack_1[256];
struct RMP_Thd Thd_1={0};
ptr_t Stack_2[256];
struct RMP_Thd Thd_2={0};
struct RMP_Sem Sem_1={0};
TIM_HandleTypeDef TIM2_Handle={0};
TIM_HandleTypeDef TIM4_Handle={0};
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
    /* Initialize timer 2 to run at the same speed as the CPU */
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

/* Begin Function:Int_Init ****************************************************
Description : Initialize an periodic interrupt source. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Init(void)
{
    /* Initialize timer 2 to run at the same speed as the CPU */
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

/* Begin Function:Int_Disable *************************************************
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
