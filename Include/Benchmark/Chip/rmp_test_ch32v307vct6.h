/******************************************************************************
Filename    : rmp_test_ch32v307vct6.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for CH32V307VCT6.
******************************************************************************/

/* Includes ******************************************************************/
#include "rmp.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Where are the initial stacks */
#define THD1_STACK          (&Stack_1[1024])
#define THD2_STACK          (&Stack_2[1024])
/* How to read counter */
#define COUNTER_READ()      (TIM_GetCounter(TIM1))
/* Are we testing the memory pool? */
/* #define TEST_MEM_POOL       8192 */
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
typedef rmp_u16_t rmp_tim_t;
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
rmp_ptr_t Stack_1[2048];
rmp_ptr_t Stack_2[2048];
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
    /* This processor does not have MCYCLE. Initializing TIM1 for this */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_CounterModeConfig(TIM1, TIM_CounterMode_Up);
    TIM_InternalClockConfig(TIM1);
    TIM_Cmd(TIM1, ENABLE);
}
/* End Function:Timer_Init ***************************************************/

/* Begin Function:Int_Init ****************************************************
Description : Initialize an periodic interrupt source. This function needs
              to be adapted to your specific hardware. The stuff initialized
              here is actually the watchdog timer.
              You MAY experience very large numbers during the test set of interrupt
              response times, because this part operates on SPI flash which have
              a huge penalty on any instruction cache miss. Just one or two of these
              misses will get you tens of thousands of cycles wasted.
              You can read 'minstret' register against 'mcycle' register to confirm this.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Test_Handler(void)
{

}

void Int_Init(void)
{

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

}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
