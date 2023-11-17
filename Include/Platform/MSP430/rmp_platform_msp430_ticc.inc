;/*****************************************************************************
;Filename    : rmp_platform_msp430_ticc.s
;Author      : pry
;Date        : 25/02/2018
;Description : The assembly part of the RMP RTOS. This is for MSP430, and
;              contains macros to be included in all interrupt assembly code
;              as needed.
;*****************************************************************************/

;/* Begin Imports ************************************************************/
    ;The stack address of current thread
    .GLOBAL             RMP_SP_Cur
    ;The interrupt active flag
    .GLOBAL             RMP_MSP430_Int_Act
    ;The yield pending flag
    .GLOBAL             RMP_MSP430_Yield_Pend
    ;Save and load extra contexts, such as FPU, peripherals and MPU
    .GLOBAL             RMP_Ctx_Save
    .GLOBAL             RMP_Ctx_Load
    ;Extract highest priority running thread
    .GLOBAL             _RMP_Run_High
    ;Handler for MSP430 timer interrupt
    .GLOBAL             _RMP_MSP430_Tim_Handler
;/* End Imports **************************************************************/

;/* Begin Macros *************************************************************/
; Push everything to stack
RMP_MSP430_INT_SAVE     .MACRO
    PUSH                R15
    PUSH                R14
    PUSH                R13
    PUSH                R12
    PUSH                R11
    PUSH                R10
    PUSH                R9
    PUSH                R8
    PUSH                R7
    PUSH                R6
    PUSH                R5
    PUSH                R4
    CALL                #RMP_Ctx_Save
    MOV                 SP,&RMP_SP_Cur
    MOV                 #1,&RMP_MSP430_Int_Act
    .ENDM

; Load everything from stack, and deal with pending yields if there is
RMP_MSP430_INT_RESTORE  .MACRO
    CMP                 #0,&RMP_MSP430_Yield_Pend
    JEQ                 RMP_MSP430_YIELD_SKIP?
    CALL                #_RMP_Run_High
    MOV                 #0,&RMP_MSP430_Yield_Pend
RMP_MSP430_YIELD_SKIP?
    MOV                 #0,&RMP_MSP430_Int_Act
    MOV                 &RMP_SP_Cur,SP
    CALL                #RMP_Ctx_Load
    POP                 R4
    POP                 R5
    POP                 R6
    POP                 R7
    POP                 R8
    POP                 R9
    POP                 R10
    POP                 R11
    POP                 R12
    POP                 R13
    POP                 R14
    POP                 R15
    BIC                 #0xF0,0(SP)
    BIS					#0x08,0(SP)
    RETI
    .ENDM
;/* End Macros ***************************************************************/

;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
