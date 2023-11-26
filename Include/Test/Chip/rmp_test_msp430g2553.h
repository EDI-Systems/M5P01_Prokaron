/******************************************************************************
Filename    : rmp_test_msp430g2553.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for MSP430G2553.
              This test header is only used with the tickless benchmark to show
              how RMP perfectly fits battery-powered devices. Best used with
              MSP-EXP430G2ET for ease of real-time power minitoring.
              Possible execution paths for the hooks:
              1. Tim - Tim
              2. Tim - Ctx
              2. Tim - Dly
              3. Dly - Ctx
              5. Ctx - Ctx

TICC V21.6.1LTS -O4 -mf5 (OS timer enabled)
              AP         : 5mA   - 10 days  AAx2
              LPM3       : 140uA - 350 days AAx2
              LPM4       : 77uA  - 650 days AAx2
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
#define RMP_TICK_1SEC           (512U)
#define RMP_BUSY_LOOP           (256U)
/* End Define ****************************************************************/

/* Global ********************************************************************/
rmp_ptr_t Stack_1[48];
rmp_ptr_t Stack_2[48];
rmp_ptr_t Slice_Last;

void RMP_MSP430_Tim_Off(void);
void RMP_MSP430_Tim_Set(rmp_ptr_t Slice);
rmp_ptr_t RMP_MSP430_Tim_Get(void);
void _RMP_MSP430_Tickless_Handler(void);

void RMP_Tim_Hook(rmp_ptr_t Slice);
void RMP_Sched_Hook(void);
void RMP_Dly_Hook(rmp_ptr_t Slice);
void RMP_Start_Hook(void);
void RMP_Init_Idle(void);
/* End Global ****************************************************************/

/* Function:RMP_MSP430_Tim_Off ************************************************
Description : Turn off the timer altogether.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_MSP430_Tim_Off(void)
{
    /* Just clear everything */
    TA0CCTL0=0U;
    TA0CTL=0U;
    TA0R=0U;
    Slice_Last=0U;
}
/* End Function:RMP_MSP430_Tim_Off *******************************************/

/* Function:RMP_MSP430_Tim_Set ************************************************
Description : Reprogram the timer for MSP430 series.
              TA0 clock = LFXT1/64 = 512Hz.
Input       : rmp_ptr_t Slice - The tick value to set.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_MSP430_Tim_Set(rmp_ptr_t Slice)
{
    /* Stop the timer first, and clear all pending interrupts */
    TA0CCTL0=0U;
    TA0CTL=0U;

    /* Clear timer */
    Slice_Last=Slice;
    TA0R=0U;

    /* Program and reenable timer if we do have counts */
    if(Slice!=0U)
    {
        TA0CCR0=Slice;
        TA0CTL=TASSEL_1|ID_3|MC_1;
        TA0CCTL0=CCIE;
    }
    /* Trigger an pending interrupt immediately if we don't have counts */
    else
    {
        TA0CCTL0=CCIE|CCIFG;
    }
}
/* End Function:RMP_MSP430_Tim_Set *******************************************/

/* Function:RMP_MSP430_Tim_Get ************************************************
Description : Get the elapsed time. The difficulty with MSP430 is that
              1. it lacks one-shot mode, 2. it lacks countdown mode.
Input       : None.
Output      : None.
Return      : rmp_ptr_t - The current counter value.
******************************************************************************/
rmp_ptr_t RMP_MSP430_Tim_Get(void)
{
    rmp_ptr_t Elapse;

    Elapse=TAR;

    /* An interrupt pending, all time must be expended */
    if((TA0CCTL0&CCIFG)!=0U)
        return Slice_Last;

    Slice_Last-=Elapse;

    return Elapse;
}
/* End Function:RMP_MSP430_Tim_Get *******************************************/

/* Function:_RMP_MSP430_Tickless_Handler **************************************
Description : Tickless interrupt routine for MSP430.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_MSP430_Tickless_Handler(void)
{
    RMP_MSP430_TIM_CLR();

    /* All time must have been expended for this to be triggered */
    _RMP_Tim_Handler(Slice_Last);
}
/* End Function:_RMP_MSP430_Tickless_Handler *********************************/

/* Function:RMP_Tim_Hook ******************************************************
Description : RMP timer hook.
Input       : rmp_ptr_t Slice - Number of slices passed.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Tim_Hook(rmp_ptr_t Slice)
{
    /* If we have a pending context switch, turn off timer and don't do anything */
    if(RMP_Sched_Pend!=0U)
    {
        RMP_MSP430_Tim_Off();
        return;
    }

    /* Reprogram the timer otherwise */
    RMP_MSP430_Tim_Set(_RMP_Tim_Future());
}
/* End Function:RMP_Tim_Hook *************************************************/

/* Function:RMP_Sched_Hook *****************************************************
Description : RMP scheduler hook.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Sched_Hook(void)
{
    /* See how much time is left, and register whatever has passed */
    _RMP_Tim_Elapse(RMP_MSP430_Tim_Get());

    /* Program a new future shot */
    RMP_MSP430_Tim_Set(_RMP_Tim_Future());
}
/* End Function:RMP_Sched_Hook ***********************************************/

/* Function:RMP_Dly_Hook ******************************************************
Description : RMP timer hook.
Input       : rmp_ptr_t Slice - Number of slices to delay.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Dly_Hook(rmp_ptr_t Slice)
{
    /* Just update the interval, this is always followed by a context switch */
    _RMP_Tim_Elapse(RMP_MSP430_Tim_Get());
}
/* End Function:RMP_Dly_Hook *************************************************/

/* Function:RMP_Start_Hook ****************************************************
Description : RMP start hook. Puts all unused GPIO port input pull-down.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Start_Hook(void)
{
    /* P1.2 - UART TXD */
    P1DIR=1U<<2;
    P1OUT=0x00U;
    P1REN=0xFBU;

    /* P2.6 - XIN, P2.7 - XOUT */
    P2DIR=0x80U;
    P2OUT=0x00U;
    P2REN=0x3FU;

    P3DIR=0x00U;
    P3REN=0xFFU;
    P3OUT=0x00U;

    /* Timer turned off initially */
    RMP_MSP430_Tim_Off();
}
/* End Function:RMP_Start_Hook ***********************************************/

/* Function:RMP_Init_Idle *****************************************************
Description : RMP idle hook.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Init_Idle(void)
{
    /* Put CPU in LPM3 if there are timer pending, LPM4 if nothing */
    RMP_INT_MASK();

    if(_RMP_Tim_Idle()==0)
    {
        RMP_DBG_S("> Init: entering LPM4 idle mode.\r\n");
        __bis_SR_register(LPM4_bits|GIE);
    }
    else
    {
        RMP_DBG_S("> Init: entering LPM3 idle mode.\r\n");
        __bis_SR_register(LPM3_bits|GIE);
    }

}
/* End Function:RMP_Init_Idle ************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
