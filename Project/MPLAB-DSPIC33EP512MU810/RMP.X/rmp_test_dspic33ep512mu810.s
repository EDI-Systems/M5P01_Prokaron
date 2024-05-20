/******************************************************************************
Filename    : rmp_test_dspic33ep512mu810.s
Author      : pry
Date        : 10/04/2012
Description : The extra testing file for this chip.
******************************************************************************/
    
/* Header ********************************************************************/
    .text
    .include            "Platform/DSPIC/rmp_platform_dspic_gcc.inc"
/* End Header ****************************************************************/
    
/* Import ********************************************************************/
    .extern             _Tim3_Interrupt
/* End Import ****************************************************************/

/* Export ********************************************************************/
    .global             __T1Interrupt
    .global             __T3Interrupt
/* End Export ****************************************************************/

/* Function:__T1Interrupt *****************************************************
Description : Timer 1 interrupt routine for tick timer.
Input       : None.
Output      : None.                                      
******************************************************************************/
__T1Interrupt:
    RMP_DSPIC_INT_SAVE
    CALL                __RMP_DSPIC_Tim_Handler
    RMP_DSPIC_INT_RESTORE
/* End Function:__T1Interrupt ************************************************/

/* Function:__T3Interrupt *****************************************************
Description : Timer 3 interrupt routine for ISR latency testing.
Input       : None.
Output      : None.                                      
******************************************************************************/
__T3Interrupt:
    RMP_DSPIC_INT_SAVE
    CALL                _Tim3_Interrupt
    RMP_DSPIC_INT_RESTORE
/* End Function:__T3Interrupt ************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
