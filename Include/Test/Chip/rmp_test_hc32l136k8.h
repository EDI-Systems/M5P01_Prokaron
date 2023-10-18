/******************************************************************************
Filename    : rmp_test_hc32l136k8.h
Author      : pry 
Date        : 22/07/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The testbench for HC32L136K8.
******************************************************************************/

/* Includes ******************************************************************/
#include "rmp.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Where are the initial stacks */
#define THD1_STACK        (&Stack_1[230])
#define THD2_STACK        (&Stack_2[230])
/* How to read counter */
/* 你需要把这个宏给改成读取计时定时器的计时寄存器，并且还要将定时器给换算成机器周期 */
#define RMP_CNT_READ()    (0)
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The HC32L136K8 timers are all 16 bits, so */
typedef rmp_u16_t rmp_tim_t;
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
rmp_ptr_t Stack_1[256];
rmp_ptr_t Stack_2[256];
/* 你需要在这里放置定时器的初始化句柄们 */
//TIM_HandleTypeDef TIM2_Handle={0};
//TIM_HandleTypeDef TIM21_Handle={0};
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
    /* 在这里你需要初始化计时用定时器 */
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
    /* 在这里你需要初始化用作中断源的定时器的溢出中断并启动该定时器 */
}

/* The interrupt handler */
/* 这个函数是该定时器的溢出中断向量。函数名要改成合适的函数名。*/
void TIMXX_IRQHandler(void)
{   
    /* 在这里响应中断，做一些清除中断Flag的事情， */
    
    /* 然后调用Int_Handler函数。 */
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
    /* 在这里你需要除能用作中断源的定时器的溢出中断并停止该定时器 */
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
