/******************************************************************************
Filename    : rmp_test_TMS570LC4357.h
Author      : pry 
Date        : 22/07/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The testbench for TMS570LC4357.
******************************************************************************/

/* Includes ******************************************************************/
#include "rmp.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Where are the initial stacks */
#define THD1_STACK        (&Stack_1[215])
#define THD2_STACK        (&Stack_2[215])
/* How to read counter */
#define COUNTER_READ()    ((rtiREG1->CNT[0].FRCx)<<3)
/* Are we testing the memory pool? */
#define TEST_MEM_POOL     8192
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The TMS570LC timers are all 32 bits, so */
typedef rmp_ptr_t rmp_tim_t;
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
rmp_ptr_t Stack_1[256];
rmp_ptr_t Stack_2[256];
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
    /* RTI/FRC0 clock = 1/8 CPU clock, already initialized. Disable compare 1 */
    rtiREG1->INTFLAG = 2U;
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
    /* RTI/FRC1 clock = 1/8 CPU clock */
    rtiREG1->INTFLAG = 2U;
    rtiStopCounter(rtiREG1, rtiCOUNTER_BLOCK1);
    rtiSetPeriod(rtiREG1, rtiCOMPARE1, 3750);
    rtiEnableNotification(rtiREG1, rtiNOTIFICATION_COMPARE1);
    rtiStartCounter(rtiREG1, rtiCOUNTER_BLOCK1);
}
volatile int Acc_Count=0;
/* The interrupt handler */
void rtiInterrupt(void)
{
    Acc_Count++;
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
    /* Disable channel 1 interrupt */
    rtiDisableNotification(rtiREG1, rtiNOTIFICATION_COMPARE1);
    rtiStopCounter(rtiREG1, rtiCOUNTER_BLOCK1);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
