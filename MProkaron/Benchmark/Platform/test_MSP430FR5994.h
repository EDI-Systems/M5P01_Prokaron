/******************************************************************************
Filename    : test_MSP430FR5994.h
Author      : pry 
Date        : 22/07/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The testbench for MSP430FR5994.
******************************************************************************/

/* Includes ******************************************************************/
#include "RMP.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* How to read counter */
#define COUNTER_READ()   0 //((TIM2->CNT)<<1)
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The MSP430 timers are all 16 bits, so */
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
//TIM_HandleTypeDef TIM2_Handle={0};
//TIM_HandleTypeDef TIM4_Handle={0};
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

}


/* The interrupt handler */
void TIM4_IRQHandler(void)
{
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

}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
