/******************************************************************************
Filename    : test_TMS570LC4357.h
Author      : pry 
Date        : 22/07/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The testbench for TMS570LC4357.
******************************************************************************/

/* Includes ******************************************************************/
#include "RMP.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Where are the initial stacks */
#define THD1_STACK        (&Stack_1[230])
#define THD2_STACK        (&Stack_2[230])
/* How to read counter */
#define COUNTER_READ()    (0)
/* Are we testing the memory pool? */
#define TEST_MEM_POOL     8192
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The TMS570LC timers are all 32 bits, so */
typedef ptr_t tim_t;
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
ptr_t Stack_1[256];
ptr_t Stack_2[256];
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
    /* TIM2 clock = 1/2 CPU clock */

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
    /* TIM4 clock = 1/2 CPU clock */

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
