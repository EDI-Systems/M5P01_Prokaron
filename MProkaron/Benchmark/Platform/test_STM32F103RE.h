/******************************************************************************
Filename    : test_STM32F103RE.h
Author      : pry 
Date        : 22/07/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The testbench for STM32F103RE.
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
/* The STM32F1 timers are all 16 bits, so */
typedef u16 tim_t;
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
ptr_t Stack_1[256];
ptr_t Stack_2[256];
TIM_TimeBaseInitTypeDef TIM2_Handle={0};   
TIM_TimeBaseInitTypeDef TIM4_Handle={0};
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
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_DeInit(TIM2);
    TIM2_Handle.TIM_Period=65535; 
    TIM2_Handle.TIM_Prescaler=0;
    TIM2_Handle.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM2_Handle.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM2_Handle);     
    TIM_ClearFlag(TIM2, TIM_FLAG_Update); 
    TIM_Cmd(TIM2, ENABLE);
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
    /* TIM4 clock = CPU clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_DeInit(TIM4);
    TIM4_Handle.TIM_Period=7200; 
    TIM4_Handle.TIM_Prescaler=0;
    TIM4_Handle.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM4_Handle.TIM_CounterMode=TIM_CounterMode_Down;
    TIM_TimeBaseInit(TIM4, &TIM4_Handle);     
    TIM_ClearFlag(TIM4, TIM_FLAG_Update); 
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM4, ENABLE);
    
    /* Enable Timer 4 IRQ */
    NVIC_SetPriority(TIM4_IRQn, 0xFF);
    NVIC_EnableIRQ(TIM4_IRQn);
}

/* The interrupt handler */
void TIM4_IRQHandler(void)
{
    TIM4->SR=~TIM_FLAG_Update;
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
