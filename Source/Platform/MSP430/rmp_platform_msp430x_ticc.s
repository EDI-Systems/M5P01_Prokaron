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

;/* Import *******************************************************************/
    ;The real task switch handling function
    .global             _RMP_Run_High
    ;The current thread stack
    .global             RMP_SP_Cur
    ;The kernel stack
    .global             _RMP_MSP430_SP_Kern
;/* End Import ***************************************************************/

;/* Export *******************************************************************/
    ;Disable all interrupts
    .def                RMP_Int_Disable
    ;Enable all interrupts            
    .def                RMP_Int_Enable
    ;Start the first thread
    .def                _RMP_Start
    ;Yield to another thread
    .def                _RMP_MSP430_Yield
;/* End Export ***************************************************************/

;/* Header *******************************************************************/
    .text
    .align              2
;/* End Header ***************************************************************/

;/* Function:RMP_Int_Disable **************************************************
;Description : The function for disabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
RMP_Int_Disable:        .asmfunc
    ;Disable all interrupts
    NOP
    DINT
    NOP
    RETA
    .endasmfunc
;/* End Function:RMP_Int_Disable *********************************************/

;/* Function:RMP_Int_Enable ***************************************************
;Description : The function for enabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
RMP_Int_Enable:         .asmfunc
    ;Enable all interrupts
    NOP
    EINT
    NOP
    RETA
    .endasmfunc
;/* End Function:RMP_Int_Enable **********************************************/

;/* Function:_RMP_Start *******************************************************
;Description : Jump to the user function and will never return from it.
;Input       : rmp_ptr_t R13:R12 - PC.
;              rmp_ptr_t R15:R14 - SP.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_Start:             .asmfunc
    ;Save kernel stack
    MOVA                SP,&_RMP_MSP430_SP_Kern
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
    .endasmfunc
;/* End Function:_RMP_Start **************************************************/

;/* Function:_RMP_MSP430_Yield ************************************************
;Description : Yield from one thread to another without an interrupt.
;              Note that MSP430's "rotate" is VERY different from regular CPUs.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_MSP430_Yield:      .asmfunc
    ;Disable all interrupts
    NOP
    DINT
    NOP
    ;Push as if we entered from an interrupt, PC higher half not filled yet
    PUSH                #$LO16(_RMP_MSP430_Skip)
    PUSH                SR

    ;Save all GP regs and save SP to block
    PUSHM.A             #12,R15

    ;Populate PC higher half
    MOV                 #$HI16(_RMP_MSP430_Skip),R15
    RLAM                #4,R15
    RLAM                #4,R15
    RLAM                #4,R15
    ADD                 R15,12*4(SP)

    ;Choose highest priority ready thread with kernel stack
    MOVA                SP,&RMP_SP_Cur
    MOVA                &_RMP_MSP430_SP_Kern,SP
    CALLA               #_RMP_Run_High
    MOVA                &RMP_SP_Cur,SP

    ;Pop as if we returned from an interrupt, enabling interrupt
    POPM.A              #12,R15
    ;Make sure we don't enter LPM mode automatically on return
    BIC                 #0xF0,0(SP)
    BIS                 #0x08,0(SP)
    RETI
_RMP_MSP430_Skip:
    RETA
    .endasmfunc
;/* End Function:_RMP_MSP430_Yield *******************************************/

;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/

