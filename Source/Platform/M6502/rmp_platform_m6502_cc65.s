;/*****************************************************************************
;Filename    : rmp_platform_m6502_cc65.s
;Author      : pry
;Date        : 10/04/2012
;Description : The assembly part of the RMP RTOS. This is for MOS 6502.
;*****************************************************************************/

;/* The MOS 6502 Architecture *************************************************
;Z0-ZX      : Zero page "registers" whose number can be configured.
;A          : Accumulator.
;X          : X index.
;Y          : Y index.
;SP         : Stack pointer.
;PC         : Program counter.
;PF         : Processor flags.
;*****************************************************************************/

;/* Import *******************************************************************/
    ; The real task switch handling function
    .import             _RMP_Run_High
    ; The PID of the current thread
    .import             _RMP_Thd_Cur
    ; The stack address of current thread
    .import             _RMP_SP_Cur
    ; The kernel stack
    .import             __RMP_M6502_SP_Kern
    ; Utilities provided by cc65
    .import             pusha
    .import             popa
    .import             pushax
    .import             popax
    .import             decax1
    .include            "zeropage.inc"
;/* End Import ***************************************************************/

;/* Export *******************************************************************/
    ; Disable all interrupts
    .export             _RMP_Int_Disable
    ; Enable all interrupts
    .export             _RMP_Int_Enable
    ; Start the first thread
    .export             __RMP_Start
    ; Trigger a context switch
    .export             __RMP_M6502_Yield
;/* End Export ***************************************************************/

;/* Header *******************************************************************/
    .CODE
;/* End Header ***************************************************************/

;/* Function:RMP_Int_Disable **************************************************
;Description : The function for disabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_Int_Disable:
    SEI
    RTS
;/* End Function:RMP_Int_Disable *********************************************/

;/* Function:RMP_Int_Enable ***************************************************
;Description : The function for enabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_Int_Enable:
    CLI
    RTS
;/* End Function:RMP_Int_Enable **********************************************/

;/* Function:_RMP_Start *******************************************************
;Description : Jump to the user function and will never return from it.
;Input       : [Stack] - The entry of the first task.
;              [X:A] - The stack of the first task.
;Output      : None.
;Return      : None.
;*****************************************************************************/
__RMP_Start:
    ; Place thread SP on tmps
    STA                 tmp1
    STX                 tmp2
    ; Rewind call stack to top location
    LDX                 #$FF
    TXS
    ; Push entry onto call stack - need to -1 due to RTS +1 behavior
    JSR                 popax
    JSR                 decax1
    ; Swap high and low bytes - little endian
    STA                 tmp3
    TXA
    PHA
    LDA                 tmp3
    PHA
    ; Save current SP as kernel SP
    LDA                 sp
    LDX                 sp+1
    STA                 __RMP_M6502_SP_Kern
    STX                 __RMP_M6502_SP_Kern+1
    ; Set thread SP
    LDA                 tmp1
    LDX                 tmp2
    STA                 sp
    STX                 sp+1
    ; Return to the entry - don't use JMP indirect to avoid cross-page bugs
    RTS
    ; Should not reach here
;/* End Function:_RMP_Start **************************************************/

;/* Function:_RMP_Yield *******************************************************
;Description : Trigger a yield to another thread. 6502 will never be fast, so
;              we focus on RAM footprint here rather than speed.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
    .macro              PHW arg
    LDA                 arg
    LDX                 arg+1
    JSR                 pushax
    .endmacro

    .macro              PHDB arg1,arg2
    LDA                 arg1
    LDX                 arg2
    JSR                 pushax
    .endmacro

    ; The RTS will +1 but RTI will not, hence we must use a skip label.
__RMP_M6502_Yield:
    ; Disable interrupts and decimal mode
    SEI
    CLD
    ; Leave call stack hole for PC which we will fill later on
    PHA
    PHA
    ; Push PF,A,X,Y to stack
    PHP
    PHA
    TXA
    PHA
    TYA
    PHA
    ; Save all zero page registers that cc65 uses to parameter stack - could optimize somehow, eliminate the jumps later, perhaps execute some +26 on the SP to avoid this mess.
    PHW         sreg                    ; sreg
    PHW         regsave                 ; regsave
    PHW         regsave+2
    PHW         ptr1                    ; ptr1
    PHW         ptr2                    ; ptr2
    PHW         ptr3                    ; ptr3
    PHW         ptr4                    ; ptr4
    PHDB        tmp1,tmp2               ; tmp1-2
    PHDB        tmp3,tmp4               ; tmp3-4
    PHW         regbank                 ; regbank
    PHW         regbank+2
    PHW         regbank+4
    ; Save call stack to parameter stack, including SP at the end
    TSX
    STX         tmp1
    LDX         #$FF
__RMP_M6502_Save: ; - could use indirection to speed up quite a bit
    LDA         $0100,X
    JSR         pusha
    DEX
    CPX         tmp1
    BNE         __RMP_M6502_Save
    ; Save done, place SP onto the stack as well
    LDA         tmp1
    JSR         pusha
    ; Fix up the hole that we intentionally left - same, do later. just write a working one now and try to shred some cycles

    ; Save current parameter stack pointer

    ; Load next parameter stack pointer

    ; Restore the call stack

    ; Restore the zero page registers

    ; Restore the rest of the registers
    
    RTI
__RMP_M6502_Yield_Skip:
    RTS
;/* End Function:_RMP_Yield **************************************************/
    .end
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
