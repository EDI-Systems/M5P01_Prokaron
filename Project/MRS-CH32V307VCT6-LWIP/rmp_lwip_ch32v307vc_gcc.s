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
    .extern             _ETH_IRQHandler
/* End Import ****************************************************************/

/* Export ********************************************************************/
    .global             SysTick_Handler
    .global             ETH_IRQHandler
/* End Export ****************************************************************/

/* Function:SysTick_Handler ***************************************************
Description : System tick timer handler routine.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
    .align              3
SysTick_Handler:
    CSRCI               mstatus,8
    RMP_RV32P_INT_SAVE_RVF
    CALL                _RMP_RV32P_Tim_Handler
    RMP_RV32P_INT_LOAD_RVF
/* End Function:SysTick_Handler **********************************************/

/* Function:ETH_IRQHandler ****************************************************
Description : Ethernet handler routine.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
    .align              3
ETH_IRQHandler:
    CSRCI               mstatus,8
    RMP_RV32P_INT_SAVE_RVF
    CALL                _ETH_IRQHandler
    RMP_RV32P_INT_LOAD_RVF
/* End Function:ETH_IRQHandler ***********************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
