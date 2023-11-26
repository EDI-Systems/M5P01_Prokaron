/******************************************************************************
Filename    : rmp_test_fe310g000.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The test specific C vectors for FE310-G000.
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Function:__RMP_RV32P_Handler ***********************************************
Description : Generic interrupt handler in C.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
extern void WDT_Interrupt(void);
void __RMP_RV32P_Handler(void)
{
    rmp_ptr_t Mcause;

    Mcause=RMP_RV32P_MCAUSE_Get();

    /* If this is an exception, die here */
    RMP_ASSERT((Mcause&0x80000000U)!=0U);

    switch(Mcause&0x7FFFFFFFU)
    {
        /* Machine software interrupt */
        case 12U:_RMP_RV32P_Tim_Handler();break;
        /* Other OS-aware cases */
        default:WDT_Interrupt();break;
    }
}
/* End Function:__RMP_RV32P_Handler ******************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
