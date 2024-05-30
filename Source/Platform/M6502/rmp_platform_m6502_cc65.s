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
;Input       : [R25:R24] - The entry of the first task.
;              [R23:R22] - The stack of the first task.
;Output      : None.
;Return      : None.
;*****************************************************************************/
__RMP_Start:
    ; Load SP for the first task
    ; Jump to the entry
    ; Should not reach here
;/* End Function:_RMP_Start **************************************************/

;/* Function:_RMP_Yield *******************************************************
;Description : Trigger a yield to another thread.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
__RMP_M6502_Yield:
    CLI
    RTI
;/* End Function:_RMP_Yield **************************************************/
    .end
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
