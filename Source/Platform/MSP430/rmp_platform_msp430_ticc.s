;/*****************************************************************************
;Filename    : rmp_platform_msp430_ticc.s
;Author      : pry
;Date        : 25/02/2018
;Description : The assembly part of the RMP RTOS. This is for MSP430X.
;              This port does not support interrupt preemption, as it makes no 
;              sense on MSP430 because MSP430 interrupt priority is fixed.
;*****************************************************************************/

;/* The MSP430 Architecture ***************************************************
;R0:PC                        Program counter.
;R1:SP                        Stack pointer.
;R2:SR                        Status register.
;R3:CG2                       Constant generator.
;R4-R15:                      General purpose registers.
;*****************************************************************************/

;/* Begin Header *************************************************************/
    .text
    .ALIGN              2
;/* End Header ***************************************************************/

;/* Begin Exports ************************************************************/
    ;Disable all interrupts
    .DEF                RMP_Int_Disable
    ;Enable all interrupts            
    .DEF                RMP_Int_Enable
    ;Start the first thread
    .DEF                _RMP_Start
    ;Start the first thread
    .DEF                _RMP_MSP430_Yield
;/* End Exports **************************************************************/

;/* Begin Imports ************************************************************/
    ;The stack address of current thread
    .GLOBAL             RMP_SP_Cur
    ;Save and load extra contexts, such as FPU, peripherals and MPU
    .GLOBAL             RMP_Ctx_Save
    .GLOBAL             RMP_Ctx_Load
    .GLOBAL             _RMP_Run_High
;/* End Imports **************************************************************/

;/* Begin Function:RMP_Int_Disable ********************************************
;Description : The function for disabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
RMP_Int_Disable:        .ASMFUNC
    ;Disable all interrupts
    NOP
    DINT
    NOP
    RET
    .ENDASMFUNC
;/* End Function:RMP_Int_Disable *********************************************/

;/* Begin Function:RMP_Int_Enable *********************************************
;Description : The function for enabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
RMP_Int_Enable:         .ASMFUNC
    ;Enable all interrupts
    NOP
    EINT
    NOP
    RET
    .ENDASMFUNC
;/* End Function:RMP_Int_Enable **********************************************/

;/* Begin Function:_RMP_Start *************************************************
;Description : Jump to the user function and will never return from it.
;              Note that all thread function entries should locate at <64kB.
;Input       : rmp_ptr_t R12 - PC.
;              rmp_ptr_t R13 - SP.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_Start:             .ASMFUNC
    MOV                 R13,SP
    MOV                 R12,PC
    ; Dummy return
    RET
    .ENDASMFUNC
;/* End Function:_RMP_Start **************************************************/

;/* Begin Function:_RMP_MSP430_Yield ******************************************
;Description : Yield from one thread to another without an interrupt.
;              Note that MSP430's "rotate" is VERY different from regular CPUs.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_MSP430_Yield:      .ASMFUNC
    ;Disable all interrupts
    NOP
    DINT
    NOP
    ;Push as if we entered from an interrupt, PC higher half not filled yet
    PUSH                #_RMP_MSP430_Skip
    PUSH                SR

    ;Save all GP regs and save SP to block
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

    ;Set GIE(bit[3]) in SR
    ADD                 #0x08,12*2(SP)

    ;Choose highest priority ready thread
    CALL                #_RMP_Run_High

    ;Pop as if we returned from an interrupt, enabling interrupt
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
    ;Make sure we don't enter LPM mode automatically on return
    BIC                 #0xF0,0(SP)
    BIS                 #0x08,0(SP)
    RETI
_RMP_MSP430_Skip:
    RET
    .ENDASMFUNC
;/* End Function:_RMP_MSP430_Yield *******************************************/

;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/

