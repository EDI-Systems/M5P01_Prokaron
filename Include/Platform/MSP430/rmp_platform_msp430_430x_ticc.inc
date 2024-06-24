;/*****************************************************************************
;Filename    : rmp_platform_msp430_430x_ticc.inc
;Author      : pry
;Date        : 25/02/2018
;Description : The assembly part of the RMP RTOS. This is for MSP430X, and
;              contains macros to be included in all interrupt assembly code
;              as needed.
;*****************************************************************************/

;/* Import *******************************************************************/
    ;The kernel stack
    .global             _RMP_MSP430_SP_Kern
    ;The current thread stack
    .global             RMP_SP_Cur
    ;The yield pending flag
    .global             RMP_Sched_Pend
    ;Extract highest priority running thread
    .global             _RMP_Run_High
    ;Handler for MSP430 timer interrupt
    .global             _RMP_MSP430_Tim_Handler
;/* End Import ***************************************************************/

;/* Macro ********************************************************************/
;/* Push everything to stack - for flags, 16-bit operations are sufficient ***/
RMP_MSP430_INT_SAVE_430X    .macro
    PUSHM.A             #12,R15
    MOVA                SP,&RMP_SP_Cur
    MOVA                &_RMP_MSP430_SP_Kern,SP
    .endm

;/* Load everything from stack, and deal with pending yields if there is *****/
RMP_MSP430_INT_LOAD_430X    .macro
    CMP                 #0,&RMP_Sched_Pend
    JEQ                 RMP_MSP430_YIELD_SKIP?
    CALLA               #_RMP_Run_High
RMP_MSP430_YIELD_SKIP?
    MOVA                &RMP_SP_Cur,SP
    POPM.A              #12,R15
    BIC                 #0xF0,0(SP)
    BIS                 #0x08,0(SP)
    RETI
    .endm
;/* End Macro ****************************************************************/

;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/

