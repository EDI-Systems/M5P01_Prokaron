/******************************************************************************
Filename    : rmp_test_fe310g000_gcc.s
Author      : pry
Date        : 10/04/2012
Description : The test specific assembly vectors for FE310-G000.
******************************************************************************/

/* Header ********************************************************************/
    .section            .text.isr
    .include            "Platform/RV32P/rmp_platform_rv32p_gcc.inc"
/* End Header ****************************************************************/

/* Import ********************************************************************/
    .extern             __RMP_RV32P_Handler
/* End Export ****************************************************************/

/* Export ********************************************************************/
    .global             _RMP_RV32P_Handler
/* End Export ****************************************************************/

/* Function:_RMP_RV32P_Handler ************************************************
Description : Generic interrupt handler routine.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
    .align              2
_RMP_RV32P_Handler:
    RMP_RV32P_INT_SAVE_NONE
    CALL                __RMP_RV32P_Handler
    RMP_RV32P_INT_LOAD_NONE
/* End Function:_RMP_RV32P_Handler *******************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
