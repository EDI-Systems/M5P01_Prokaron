/******************************************************************************
Filename    : rmp_test_ch32v307vc_gcc.s
Author      : pry
Date        : 10/04/2012
Description : The test specific assembly vectors for CH32V307.
******************************************************************************/

/* Header ********************************************************************/
    .section            .text.isr
    .include            "Platform/RV32P/rmp_platform_rv32p_gcc.inc"
/* End Header ****************************************************************/

/* Import ********************************************************************/
    .extern             _TIM4_IRQHandler
/* End Import ****************************************************************/

/* Export ********************************************************************/
    .global             SysTick_Handler
    .global             TIM4_IRQHandler
/* End Export ****************************************************************/

/* Function:SysTick_Handler ***************************************************
Description : System tick timer handler routine.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
    .align              2
SysTick_Handler:
    RMP_RV32P_INT_SAVE_RVF
    CALL                _RMP_RV32P_Tim_Handler
    RMP_RV32P_INT_RESTORE_RVF
/* End Function:SysTick_Handler **********************************************/

/* Function:TIM4_IRQHandler ***************************************************
Description : Timer 4 handler routine.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
    .align              2
TIM4_IRQHandler:
    RMP_RV32P_INT_SAVE_RVF
    LI                  a0,1
    CALL                _TIM4_IRQHandler
    RMP_RV32P_INT_RESTORE_RVF
/* End Function:TIM4_IRQHandler **********************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
