;/*****************************************************************************
;Filename    : rmp_platform_c28x_ticc.s
;Author      : zr (ran zhang)
;Date        : 27/04/2024
;Description : The assembly part of the RMP RTOS. This is for C28X with FPU64.
;              The TI C compiler, when working under COFF mode, will automatically
;              mangle symbols and add "_"  before it, whereas the EABI mode does
;              not. This file is compatible with both modes.
;              Note that the C28X's FPU does not have a bit indicating whether
;              it is in use, thus we cannot have a lazy stacking feature like
;              Cortex-M or RISC-V without nontrivial effort.
;*****************************************************************************/

;/* The C28X Architecture *****************************************************
;ACC                        Accumulator.
;XAR0-XAR7:                 Auxiliary registers.
;XT                         Multiplicand register.
;PC                         Program counter.
;RPC                        Return program counter.
;SP                         Stack pointer.
;DP                         Data Page Pointer.
;ST0/ST1                    Status Registers.
;IER                        Interrupt enable register.
;*****************************************************************************/

;/* Header *******************************************************************/
    .text
    .align                  1

    ;Make things compatible for both COFF and EABI
    .if __TI_EABI__
    .asg                    _RMP_Run_High,__RMP_Run_High
    .asg                    RMP_SP_Cur,_RMP_SP_Cur
    .asg                    _RMP_C28X_SP_Kern,__RMP_C28X_SP_Kern
    .asg                    RMP_Int_Disable,_RMP_Int_Disable
    .asg                    RMP_Int_Enable,_RMP_Int_Enable
    .asg                    _RMP_Start,__RMP_Start
    .asg                    _RMP_C28X_Yield_NONE,__RMP_C28X_Yield_NONE
    .asg                    _RMP_C28X_Yield_FPU32,__RMP_C28X_Yield_FPU32
    .asg                    _RMP_C28X_Yield_FPU64,__RMP_C28X_Yield_FPU64
    .endif
;/* End Header ***************************************************************/

;/* Import *******************************************************************/
    ;The real task switch handling function
    .global                 __RMP_Run_High
    ;The current thread stack
    .global                 _RMP_SP_Cur
    ;The kernel stack
    .global                 __RMP_C28X_SP_Kern
;/* End Import ***************************************************************/

;/* Export *******************************************************************/
    ;Disable all interrupts
    .global                 _RMP_Int_Disable
    ;Enable all interrupts
    .global                 _RMP_Int_Enable
    ;Start the first thread
    .global                 __RMP_Start
    ;Fast yield
    .global                 __RMP_C28X_Yield_NONE
    .global                 __RMP_C28X_Yield_FPU32
    .global                 __RMP_C28X_Yield_FPU64
;/* End Export ***************************************************************/

;/* Function:_RMP_Int_Disable *************************************************
;Description : The function for disabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_Int_Disable:           .asmfunc
    ;Disable all interrupts
    DINT
    LRETR
    .endasmfunc
;/* End Function:_RMP_Int_Disable ********************************************/

;/* Function:_RMP_Int_Enable **************************************************
;Description : The function for enabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_Int_Enable:            .asmfunc
    ;Enable all interrupts
    EINT
    LRETR
    .endasmfunc
;/* End Function:_RMP_Int_Enable *********************************************/

;/* Function:__RMP_Start ******************************************************
;Description : Jump to the user function and will never return from it.
;              Note that all thread function entries should locate at <64kB.
;Input       : rmp_ptr_t ACC - PC.
;              rmp_ptr_t stack - SP.
;Output      : None.
;Return      : None.
;*****************************************************************************/
__RMP_Start:                .asmfunc
    ;Save kernel SP
    MOVL                    XAR7,ACC
    MOV                     AL,SP
    XOR                     AH,AH
    MOV                     DP,#__RMP_C28X_SP_Kern
    MOVL                    @__RMP_C28X_SP_Kern,ACC
    ;Set user SP
    MOVL                    ACC,*-SP[4]
    MOV                     SP,AL
    ;Jump tp user code
    LB                      *XAR7
    .endasmfunc
;/* End Function:__RMP_Start *************************************************/

;/* Function:__RMP_C28X_Yield *************************************************
;Description : Yield from one thread to another without an interrupt.
;              This function has 3 versions:
;              1. no coprocessor;
;              2. single-precision FPU;
;              3. double-precision FPU coexisting with the single-precision FPU.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
;/* Save all GP regs *********************************************************/
RMP_C28X_SAVE               .macro LABEL
    ;Disable interrupt upon entry
    DINT
    ;Meant to increase SP by 1, simulating "interrupt" behavior
    PUSH                    ST0
    ;Main push sequence simulating an interrupt entry
    PUSH                    T:ST0
    PUSH                    ACC
    PUSH                    P
    PUSH                    AR1:AR0
    ;Push ST1 and DP - ST1 must have interrupt enabled upon return
    PUSH                    DP:ST1
    POP                     ACC
    AND                     AL,#0xFFFEU
    PUSH                    ACC
    ;Push 0 for DBGSTAT; clear emulation context
    XOR                     AH,AH
    MOV                     AL,IER
    PUSH                    ACC
    ;Push _RMP_C28X_Skip as PC
    PUSH                    XAR2
    MOVL                    XAR2,#LABEL
    MOVL                    ACC,XAR2
    POP                     XAR2
    PUSH                    ACC
    ;Push regular registers
    PUSH                    RPC
    PUSH                    AR1H:AR0H
    PUSH                    XAR2
    PUSH                    XAR3
    PUSH                    XAR4
    PUSH                    XAR5
    PUSH                    XAR6
    PUSH                    XAR7
    PUSH                    XT
    .endm

;/* Actual context switch ****************************************************/
RMP_C28X_SWITCH             .macro
    ;Choose highest priority ready thread with kernel stack
    MOV                     AH,SP
    MOV                     DP,#_RMP_SP_Cur
    MOV                     @_RMP_SP_Cur,AH
    MOV                     DP,#__RMP_C28X_SP_Kern
    MOV                     AH,@__RMP_C28X_SP_Kern
    MOV                     SP,AH
    LCR                     __RMP_Run_High
    MOV                     DP,#_RMP_SP_Cur
    MOV                     AH,@_RMP_SP_Cur
    MOV                     SP,AH
    .endm

;/* Restore all GP regs and simulate an IRET *********************************/
RMP_C28X_RESTORE            .macro
    ;Pop regular registers
    POP                     XT
    POP                     XAR7
    POP                     XAR6
    POP                     XAR5
    POP                     XAR4
    POP                     XAR3
    POP                     XAR2
    POP                     AR1H:AR0H
    POP                     RPC
    ;Return from "interrupt"
    IRET
    .endm

;/* No coprocessor ***********************************************************/
    .sect                   ".text:_RMP_C28X_Yield_NONE"
    .align                  1
__RMP_C28X_Yield_NONE:      .asmfunc
    RMP_C28X_SAVE           __RMP_C28X_Yield_NONE_Skip
    RMP_C28X_SWITCH
    RMP_C28X_RESTORE
__RMP_C28X_Yield_NONE_Skip:
    LRETR
    .endasmfunc

;/* Single-precision FPU coprocessor *****************************************/
    .sect                   ".text:__RMP_C28X_Yield_FPU32"
    .align                  1
__RMP_C28X_Yield_FPU32:     .asmfunc
    RMP_C28X_SAVE           __RMP_C28X_Yield_FPU32_Skip
    ;Push FPU registers
    .word                   0xE200      ;MOV32   *SP++, STF
    .word                   0x00BD
    .word                   0xFFF0      ;PUSH    RB
    .word                   0xE203      ;MOV32   *SP++, R0H
    .word                   0x00BD
    .word                   0xE203      ;MOV32   *SP++, R1H
    .word                   0x01BD
    .word                   0xE203      ;MOV32   *SP++, R2H
    .word                   0x02BD
    .word                   0xE203      ;MOV32   *SP++, R3H
    .word                   0x03BD
    .word                   0xE203      ;MOV32   *SP++, R4H
    .word                   0x04BD
    .word                   0xE203      ;MOV32   *SP++, R5H
    .word                   0x05BD
    .word                   0xE203      ;MOV32   *SP++, R6H
    .word                   0x06BD
    .word                   0xE203      ;MOV32   *SP++, R7H
    .word                   0x07BD
    RMP_C28X_SWITCH
    ;Pop FPU registers
    .word                   0xE2AF      ;MOV32   R7H,*--SP
    .word                   0x07BE
    .word                   0xE2AF      ;MOV32   R6H,*--SP
    .word                   0x06BE
    .word                   0xE2AF      ;MOV32   R5H,*--SP
    .word                   0x05BE
    .word                   0xE2AF      ;MOV32   R4H,*--SP
    .word                   0x04BE
    .word                   0xE2AF      ;MOV32   R3H,*--SP
    .word                   0x03BE
    .word                   0xE2AF      ;MOV32   R2H,*--SP
    .word                   0x02BE
    .word                   0xE2AF      ;MOV32   R1H,*--SP
    .word                   0x01BE
    .word                   0xE2AF      ;MOV32   R0H,*--SP
    .word                   0x00BE
    .word                   0xFFF1      ;POP     RB
    .word                   0xE280      ;MOV32   STF,*--SP
    .word                   0x00BE
    RMP_C28X_RESTORE
__RMP_C28X_Yield_FPU32_Skip:
    LRETR
    .endasmfunc

;/* Double-precision FPU coprocessor *****************************************/
    .sect                   ".text:__RMP_C28X_Yield_FPU64"
    .align                  1
__RMP_C28X_Yield_FPU64:     .asmfunc
    RMP_C28X_SAVE           __RMP_C28X_Yield_FPU64_Skip
    ;Push FPU registers
    .word                   0xE200      ;MOV32   *SP++, STF
    .word                   0x00BD
    .word                   0xFFF0      ;PUSH    RB
    .word                   0xE202      ;MOV32   *SP++, R0L
    .word                   0x00BD
    .word                   0xE203      ;MOV32   *SP++, R0H
    .word                   0x00BD
    .word                   0xE202      ;MOV32   *SP++, R1L
    .word                   0x01BD
    .word                   0xE203      ;MOV32   *SP++, R1H
    .word                   0x01BD
    .word                   0xE202      ;MOV32   *SP++, R2L
    .word                   0x02BD
    .word                   0xE203      ;MOV32   *SP++, R2H
    .word                   0x02BD
    .word                   0xE202      ;MOV32   *SP++, R3L
    .word                   0x03BD
    .word                   0xE203      ;MOV32   *SP++, R3H
    .word                   0x03BD
    .word                   0xE202      ;MOV32   *SP++, R4L
    .word                   0x04BD
    .word                   0xE203      ;MOV32   *SP++, R4H
    .word                   0x04BD
    .word                   0xE202      ;MOV32   *SP++, R5L
    .word                   0x05BD
    .word                   0xE203      ;MOV32   *SP++, R5H
    .word                   0x05BD
    .word                   0xE202      ;MOV32   *SP++, R6L
    .word                   0x06BD
    .word                   0xE203      ;MOV32   *SP++, R6H
    .word                   0x06BD
    .word                   0xE202      ;MOV32   *SP++, R7L
    .word                   0x07BD
    .word                   0xE203      ;MOV32   *SP++, R7H
    .word                   0x07BD
    RMP_C28X_SWITCH
    ;Pop FPU registers
    .word                   0xE2AF      ;MOV32   R7H,*--SP
    .word                   0x07BE
    .word                   0xE29F      ;MOV32   R7L,*--SP
    .word                   0x07BE
    .word                   0xE2AF      ;MOV32   R6H,*--SP
    .word                   0x06BE
    .word                   0xE29F      ;MOV32   R6L,*--SP
    .word                   0x06BE
    .word                   0xE2AF      ;MOV32   R5H,*--SP
    .word                   0x05BE
    .word                   0xE29F      ;MOV32   R5L,*--SP
    .word                   0x05BE
    .word                   0xE2AF      ;MOV32   R4H,*--SP
    .word                   0x04BE
    .word                   0xE29F      ;MOV32   R4L,*--SP
    .word                   0x04BE
    .word                   0xE2AF      ;MOV32   R3H,*--SP
    .word                   0x03BE
    .word                   0xE29F      ;MOV32   R3L,*--SP
    .word                   0x03BE
    .word                   0xE2AF      ;MOV32   R2H,*--SP
    .word                   0x02BE
    .word                   0xE29F      ;MOV32   R2L,*--SP
    .word                   0x02BE
    .word                   0xE2AF      ;MOV32   R1H,*--SP
    .word                   0x01BE
    .word                   0xE29F      ;MOV32   R1L,*--SP
    .word                   0x01BE
    .word                   0xE2AF      ;MOV32   R0H,*--SP
    .word                   0x00BE
    .word                   0xE29F      ;MOV32   R0L,*--SP
    .word                   0x00BE
    .word                   0xFFF1      ;POP     RB
    .word                   0xE280      ;MOV32   STF,*--SP
    .word                   0x00BE
    RMP_C28X_RESTORE
__RMP_C28X_Yield_FPU64_Skip:
    LRETR
    .endasmfunc
;/* End Function:__RMP_C28X_Yield ********************************************/

;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
