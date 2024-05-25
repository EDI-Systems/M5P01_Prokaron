/******************************************************************************
Filename    : rmp_test_spce061a.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for SPCE061A.
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* How to read counter */
#define RMP_CNT_READ()      ((rmp_tim_t)((*P_TimerA_Data)<<1))
/* Are we testing the memory pool? */
#define TEST_MEM_POOL       1536
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The AVR timers we use is 16 bits, so */
typedef rmp_u16_t rmp_tim_t;
/* The pointer is also 16-bit, resort to 32-bit accumulators */
#define PTR_16_BIT
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
rmp_ptr_t Stack_1[80];
rmp_ptr_t Stack_2[80];

void Timer_Init(void);
void Int_Init(void);
void Int_Disable(void);
/* End Global ****************************************************************/

/* Function:Timer_Init ********************************************************
Description : Initialize the timer for timing measurements. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Timer_Init(void)
{
    /* TIMA clock = 1/2 CPU clock - Fosc/2, no gating */
    *P_TimerA_Data=0x0000U;
    *P_TimerA_Ctrl=C_SourceA_Fosc2|C_SourceB_1|C_DutyOff;
}
/* End Function:Timer_Init ***************************************************/

/* Function:Int_Init **********************************************************
Description : Initialize an periodic interrupt source. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Init(void)
{
    /* TIMB clock = 1/2 CPU clock - Fosc/2 */
    *P_TimerB_Data=0xF000U;
    *P_TimerB_Ctrl=C_SourceC_Fosc2|C_DutyOff;
    *P_INT_Ctrl|=C_IRQ2_TMB;
}

/* The interrupt handler */
void TIMB_Handler(void)
{
    /* Clear TIMB flags */
    *P_INT_Clear=C_IRQ2_TMB;
    Int_Handler();
}
/* End Function:Int_Init *****************************************************/

/* Function:Int_Disable *******************************************************
Description : Disable the periodic interrupt source. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Disable(void)
{
    *P_INT_Ctrl&=~C_IRQ2_TMB;
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
