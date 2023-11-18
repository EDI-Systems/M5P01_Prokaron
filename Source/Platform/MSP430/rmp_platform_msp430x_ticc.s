;/*****************************************************************************
;Filename    : rmp_platform_msp430x_ticc.s
;Author      : pry
;Date        : 25/02/2018
;Description : The assembly part of the RMP RTOS. This is for MSP430X.
;              This port does not support interrupt preemption, as it makes no 
;              sense on MSP430(X) because MSP430(X) interrupt priority is fixed.
;              This port assumes that no code and data is near, and thus fits
;              all MSP430(X) powered chips.
;*****************************************************************************/

;/* The MSP430X Architecture **************************************************
;R0:PC                        Program counter.
;R1:SP                        Stack pointer.
;R2:SR                        Status register.
;R3:CG2                       Constant generator.
;R4-R15:                      General purpose registers.
;Note that MSP430X's register PC,SP,R4-R15 are all 20 bits,while MSP430 is 16 bits.
;Some variants also have the low energy vector math accelerator, whose context
;we leave to the user to handle. It is recommended that the LEA is left to one
;task and not shared between tasks.
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
    RETA
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
    RETA
    .ENDASMFUNC
;/* End Function:RMP_Int_Enable **********************************************/

;/* Begin Function:_RMP_Start *************************************************
;Description : Jump to the user function and will never return from it.
;Input       : rmp_ptr_t R13:R12 - PC.
;              rmp_ptr_t R15:R14 - SP.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_Start:             .ASMFUNC
    ;Place the PC into the same word
    PUSH                R13
    PUSH                R12
    POPA                R12
    ;Place the SP into the same word
    PUSH                R15
    PUSH                R14
    POPA                R14
    ;Jump to location
    MOVA                R14,SP
    MOVA                R12,PC
    ; Dummy return
    RETA
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
    PUSH                #$LO16(_RMP_MSP430_Skip)
    PUSH                SR

    ;Save all GP regs and save SP to block
    PUSHM.A             #12,R15
    CALLA               #RMP_Ctx_Save
    MOVA                SP,&RMP_SP_Cur

    ;PC higher half, and set GIE(bit[3]) in SR
    MOV                 #$HI16(_RMP_MSP430_Skip),R15
    RLAM                #4,R15
    RLAM                #4,R15
    RLAM                #4,R15
    ADD                 #0x08,R15
    ADD                 R15,12*4(SP)

    ;Choose highest priority ready thread
    CALLA               #_RMP_Run_High

    ;Pop as if we returned from an interrupt, enabling interrupt
    MOVA                &RMP_SP_Cur,SP
    CALLA               #RMP_Ctx_Load
    POPM.A              #12,R15
    ;Make sure we don't enter LPM mode automatically on return
    BIC                 #0xF0,0(SP)
    BIS                 #0x08,0(SP)
    RETI
_RMP_MSP430_Skip:
    RETA
    .ENDASMFUNC
;/* End Function:_RMP_MSP430_Yield *******************************************/

;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/

