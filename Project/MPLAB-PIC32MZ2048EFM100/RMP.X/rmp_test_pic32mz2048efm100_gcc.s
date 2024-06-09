/******************************************************************************
Filename    : rmp_test_pic32mz2048efm100_gcc.s
Author      : pry
Date        : 10/04/2012
Description : The extra testing file for this PIC32MZ2048EFM100.
******************************************************************************/
    
/* Header ********************************************************************/
    .text
    .include            "Platform/MP32P/rmp_platform_mp32p_gcc.inc"
/* End Header ****************************************************************/

/* Import ********************************************************************/
    /* The extra routine */
    .extern             Tim2_Interrupt
/* End Import ****************************************************************/
    
/* Import ********************************************************************/
    /* Program core timer */
    .global             _RMP_Set_Timer
/* End Import ****************************************************************/
    
/* Function:_RMP_Set_Timer ****************************************************
Description    : The function for setting the timer.
Input          : $a0 - Timer overflow value.
Output         : None.    
Register Usage : None.                                  
******************************************************************************/
    .set                nomips16
    .set                nomicromips
    .set                noreorder
    .set                noat
    .ent                _RMP_Set_Timer
_RMP_Set_Timer:
    MTC0                $a0,RMP_CP0_COMPARE
    LI                  $a0,0
    MTC0                $a0,RMP_CP0_COUNT
    JR                  $ra
    NOP
    .end                _RMP_Set_Timer
/* End Function:_RMP_Set_Timer ***********************************************/

/* Function:__vector_dispatch_0 ***********************************************
Description : MIPS core timer interrupt routine.
Input       : None.
Output      : None.                                      
******************************************************************************/
    .set                nomips16
    .set                nomicromips
    .set                noreorder
    .set                noat
    .global             __vector_dispatch_0
    .section            .vector_0,code,keep
    .ent                __vector_dispatch_0
__vector_dispatch_0:
    RMP_MP32P_INT_SAVE_DSPASE_FR64
    JAL                 _RMP_MP32P_Tim_Handler
    NOP
    RMP_MP32P_INT_LOAD_DSPASE_FR64
    .end                __vector_dispatch_0
/* End Function:__vector_dispatch_0 ******************************************/

/* Function:__vector_dispatch_9 ***********************************************
Description : The timer 2 interrupt routine.
Input       : None.
Output      : None.                                      
******************************************************************************/
    .set                nomips16
    .set                nomicromips
    .set                noreorder
    .set                noat
    .global             __vector_dispatch_9
    .section            .vector_9,code,keep
    .ent                __vector_dispatch_9
__vector_dispatch_9:
    RMP_MP32P_INT_SAVE_DSPASE_FR64
    JAL                 Tim2_Interrupt
    NOP
    RMP_MP32P_INT_LOAD_DSPASE_FR64
    .end                __vector_dispatch_9
/* End Function:__vector_dispatch_9 ******************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
