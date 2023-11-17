;/*****************************************************************************
;Filename    : rmp_platform_msp430x_ticc.s
;Author      : pry
;Date        : 25/02/2018
;Description : The assembly part of the RMP RTOS. This is for MSP430X, and
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
; Push everything to stack - for flags, 16-bit operations are sufficient
RMP_MSP430_INT_SAVE     .MACRO
    PUSHM.A             #12,R15
    CALLA               #RMP_Ctx_Save
    MOVA                SP,&RMP_SP_Cur
    MOV                 #1,&RMP_MSP430_Int_Act
    .ENDM

; Load everything from stack, and deal with pending yields if there is
RMP_MSP430_INT_RESTORE  .MACRO
    CMP                 #0,&RMP_MSP430_Yield_Pend
    JEQ                 RMP_MSP430_YIELD_SKIP?
    CALLA               #_RMP_Run_High
    MOV                 #0,&RMP_MSP430_Yield_Pend
RMP_MSP430_YIELD_SKIP?
    MOV                 #0,&RMP_MSP430_Int_Act
    MOVA                &RMP_SP_Cur,SP
    CALLA               #RMP_Ctx_Load
    POPM.A              #12,R15
    BIC                 #0xF0,0(SP)
    BIS					#0x08,0(SP)
    RETI
    .ENDM
;/* End Macros ***************************************************************/

;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
