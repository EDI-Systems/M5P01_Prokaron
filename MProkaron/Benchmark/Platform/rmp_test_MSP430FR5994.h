/******************************************************************************
Filename    : rmp_test_MSP430FR5994.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for MSP430FR5994.
******************************************************************************/

/* Includes ******************************************************************/
#include "rmp.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Where are the initial stacks */
#define THD1_STACK        (&Stack_1[100])
#define THD2_STACK        (&Stack_2[100])
/* This test is really slow. It takes 1-2 minutes(typ.1 min 30 sec) to run through @ 16MHz */
/* How to read counter */
#define COUNTER_READ()    (TA1R)
/* Are we testing the memory pool? */
#define TEST_MEM_POOL     8192
#pragma PERSISTENT(Pool)
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The MSP430 timers are all 16 bits, so */
typedef rmp_u16_t rmp_tim_t;
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
rmp_ptr_t Stack_1[128];
rmp_ptr_t Stack_2[128];
struct Timer_A_initUpModeParam TIM1_Handle={0};
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
    /* TIM1 clock = CPU clock */
    TIM1_Handle.clockSource=TIMER_A_CLOCKSOURCE_SMCLK;
    TIM1_Handle.clockSourceDivider=TIMER_A_CLOCKSOURCE_DIVIDER_1;
    TIM1_Handle.timerPeriod=0xFFFF;
    TIM1_Handle.timerInterruptEnable_TAIE=TIMER_A_TAIE_INTERRUPT_DISABLE;
    TIM1_Handle.captureCompareInterruptEnable_CCR0_CCIE=TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE;
    TIM1_Handle.timerClear=TIMER_A_SKIP_CLEAR;
    TIM1_Handle.startTimer=1;
    Timer_A_initUpMode(TA1_BASE,&TIM1_Handle);
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
    /* Make sure that we also generate interrupts from the same timer */
    TA1CCTL0|=CCIE;
}


/* The interrupt handler */
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIM1_IRQHandler(void)
{
    TA1CCTL0&=~CCIFG;
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
    /* Disable timer 1 interrupt */
    TA1CCTL0&=~CCIE;
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
