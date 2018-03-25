/******************************************************************************
Filename    : test_STM32F767IG_RVM.h
Author      : pry 
Date        : 22/07/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The testbench for STM32F767IG, running in the RVM.
******************************************************************************/

/* Includes ******************************************************************/
#include "RMP.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Where are the initial stacks */
#define THD1_STACK        (&Stack_1[230])
#define THD2_STACK        (&Stack_2[230])
/* How to read counter */
#define COUNTER_READ()    ((TIM2->CNT)<<1)
/* Are we testing the memory pool? */
#define TEST_MEM_POOL     8192
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The STM32F7 timers are all 32 bits, so */
typedef ptr_t tim_t;
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
ptr_t Stack_1[256];
ptr_t Stack_2[256];
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
void TIM_Base_SetConfig(TIM_TypeDef *TIMx, TIM_Base_InitTypeDef *Structure)
{
  uint32_t tmpcr1 = 0;
  tmpcr1 = TIMx->CR1;
  
  /* Set TIM Time Base Unit parameters ---------------------------------------*/
  if(IS_TIM_CC3_INSTANCE(TIMx) != RESET)   
  {
    /* Select the Counter Mode */
    tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    tmpcr1 |= Structure->CounterMode;
  }
 
  if(IS_TIM_CC1_INSTANCE(TIMx) != RESET)  
  {
    /* Set the clock division */
    tmpcr1 &= ~TIM_CR1_CKD;
    tmpcr1 |= (uint32_t)Structure->ClockDivision;
  }

  /* Set the auto-reload preload */
  MODIFY_REG(tmpcr1, TIM_CR1_ARPE, Structure->AutoReloadPreload);

  TIMx->CR1 = tmpcr1;

  /* Set the Auto-reload value */
  TIMx->ARR = (uint32_t)Structure->Period ;
 
  /* Set the Prescaler value */
  TIMx->PSC = (uint32_t)Structure->Prescaler;
    
  if(IS_TIM_ADVANCED_INSTANCE(TIMx) != RESET)  
  {
    /* Set the Repetition Counter value */
    TIMx->RCR = Structure->RepetitionCounter;
  }

  /* Generate an update event to reload the Prescaler 
     and the repetition counter(only for TIM1 and TIM8) value immediately */
  TIMx->EGR = TIM_EGR_UG;
}

void Timer_Init(void)
{
    /* TIM2 clock = 1/2 CPU clock */
    TIM2_Handle.Instance=TIM2;
    TIM2_Handle.Init.Prescaler=0;
    TIM2_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
    TIM2_Handle.Init.Period=(unsigned int)(-1);
    TIM2_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    /* Set the Time Base configuration */
    TIM_Base_SetConfig(TIM2_Handle.Instance, &(TIM2_Handle.Init)); 
    TIM2_Handle.State=HAL_TIM_STATE_READY;
  
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
    /* Connect the physical interrupt to our machine */
    RMP_ASSERT(RVM_Hyp_Reg_Int(RVM_TIM4_INT_IRQ30, 2)==0);
    /* Set the priority of the physical interrupt and enable it */
    RMP_ASSERT(RVM_HW_Int_Enable(RVM_TIM4_INT_IRQ30)==0);
    RMP_ASSERT(RVM_HW_Int_Prio(RVM_TIM4_INT_IRQ30,0xFF)==0);
    /* Interrupt generation is initialized too, here we only register our handler */
    RVM_Vect_Init(2,Int_Handler);
    
    /* TIM4 clock = 1/2 CPU clock */
    TIM4_Handle.Instance=TIM4;
    TIM4_Handle.Init.Prescaler=0;
    TIM4_Handle.Init.CounterMode=TIM_COUNTERMODE_DOWN;
    TIM4_Handle.Init.Period=21600;
    TIM4_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	TIM4_Handle.Init.RepetitionCounter=0;
    TIM_Base_SetConfig(TIM4_Handle.Instance, &(TIM4_Handle.Init)); 
    TIM4_Handle.State=HAL_TIM_STATE_READY;
    __HAL_RCC_TIM4_CLK_ENABLE();
	/* Clear interrupt pending bit, because we used EGR to update the registers */
	__HAL_TIM_CLEAR_IT(&TIM4_Handle, TIM_IT_UPDATE);
      /* Enable the TIM Update interrupt */
    __HAL_TIM_ENABLE_IT(&TIM4_Handle, TIM_IT_UPDATE);
    /* Enable the Peripheral */
    __HAL_TIM_ENABLE(&TIM4_Handle);
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
    /* Disable interrupt */
    RMP_ASSERT(RVM_HW_Int_Enable(RVM_TIM4_INT_IRQ30)==0);
    /* Reverse registration */
    RVM_Vect_Init(2,0);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/

