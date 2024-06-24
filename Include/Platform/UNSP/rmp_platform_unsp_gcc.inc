//*****************************************************************************
//Filename    : rmp_platform_unsp_gcc.inc
//Author      : pry
//Date        : 22/05/2024
//Description : The assembly part of the RMP RTOS. This is for unSP, and
//              contains macros to be included in all interrupt assembly code
//              as needed.
//*****************************************************************************
    
// Header *********************************************************************
    .text
// End Header *****************************************************************

// Import *********************************************************************
    // The kernel stack and table
    .external           __RMP_UNSP_SP_Kern
    // The current thread stack
    .external           _RMP_SP_Cur
    // The yield pending flag
    .external           _RMP_Sched_Pend
    // Extract highest priority running thread
    .external           __RMP_Run_High
    // Handler for UNSP timer interrupt
    .external           __RMP_UNSP_Tim_Handler
// End Import *****************************************************************

// Macro **********************************************************************
// Save all GP regs ***********************************************************
RMP_UNSP_SAVE:          .macro
    PUSH                R1,R5 TO [SP]
    .endm

// Prepare for the context switch *********************************************
RMP_UNSP_SWITCH_PRE:    .macro
    R1                  =0
    SR                  =R1
    [_RMP_SP_Cur]       =SP
    SP                  =[__RMP_UNSP_SP_Kern]
    .endm

// Finish up the context switch ***********************************************
RMP_UNSP_SWITCH_POST:   .macro
    R1                  =[_RMP_Sched_Pend]
    CMP                 R1,0
    JZ                  ?RMP_UNSP_SWITCH_POST_SKIP  //GCC 2.x does not support 1f yet
    CALL                __RMP_Run_High
?RMP_UNSP_SWITCH_POST_SKIP:
    SP                  =[_RMP_SP_Cur]
    .endm

// Restore all GP regs ********************************************************
RMP_UNSP_LOAD:          .macro
    POP                 R1,R5 FROM [SP]
    RETI
    .endm
    
// Save macro for unSP V1 *****************************************************
RMP_UNSP_INT_SAVE_SPV1: .macro
    RMP_UNSP_SAVE
    RMP_UNSP_SWITCH_PRE
    .endm

// Restore macro for unSP V1 **************************************************
RMP_UNSP_INT_LOAD_SPV1: .macro
    RMP_UNSP_SWITCH_POST
    RMP_UNSP_LOAD
    .endm
    
// Save macro for unSP V2 *****************************************************
RMP_UNSP_INT_SAVE_SPV2: .macro
    RMP_UNSP_SAVE
    .dw                 0xFF80      // PUSH R8,R15 TO [SP]
    .dw                 0x8E20
    RMP_UNSP_SWITCH_PRE
    .endm

// Restore macro for unSP V2 **************************************************
RMP_UNSP_INT_LOAD_SPV2: .macro
    RMP_UNSP_SWITCH_POST
    .dw                 0xFF80      // POP R8,R15 FROM [SP]
    .dw                 0x0E20
    RMP_UNSP_LOAD
    .endm
// End Macro ******************************************************************

// End Of File ****************************************************************

// Copyright (C) Evo-Devo Instrum. All rights reserved ************************
