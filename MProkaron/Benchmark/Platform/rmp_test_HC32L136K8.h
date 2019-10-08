/******************************************************************************
Filename    : rmp_test_HC32L136K8.h
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
/* ����Ҫ���������ĳɶ�ȡ��ʱ��ʱ���ļ�ʱ�Ĵ��������һ�Ҫ����ʱ��������ɻ������� */
#define COUNTER_READ()    (0)
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
/* ����Ҫ��������ö�ʱ���ĳ�ʼ������� */
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
    /* ����������Ҫ��ʼ����ʱ�ö�ʱ�� */
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
    /* ����������Ҫ��ʼ�������ж�Դ�Ķ�ʱ��������жϲ������ö�ʱ�� */
}

/* The interrupt handler */
/* ��������Ǹö�ʱ��������ж�������������Ҫ�ĳɺ��ʵĺ�������*/
void TIMXX_IRQHandler(void)
{   
    /* ��������Ӧ�жϣ���һЩ����ж�Flag�����飬 */
    
    /* Ȼ�����Int_Handler������ */
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
    /* ����������Ҫ���������ж�Դ�Ķ�ʱ��������жϲ�ֹͣ�ö�ʱ�� */
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
