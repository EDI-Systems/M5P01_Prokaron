;/*****************************************************************************
;Filename    : rmp_platform_m6502_cc65.inc
;Author      : pry
;Date        : 22/05/2024
;Description : The assembly part of the RMP RTOS. This is for MOS 6502, and
;              contains macros to be included in all interrupt assembly code
;              as needed.
;*****************************************************************************/
    
;/* Header *******************************************************************/
    .CODE
;/* End Header ***************************************************************/

;/* Import *******************************************************************/
    ; The kernel stack pointer
    .import             __RMP_M6502_SP_Kern
    ; The current thread stack
    .import             _RMP_SP_Cur
    ; The interrupt active flag
    .import             _RMP_M6502_Int_Act
    ; The yield pending flag
    .import             __RMP_M6502_Yield_Pend
    ; Extract highest priority running thread
    .import             __RMP_Run_High
    ; Handler for M6502 timer interrupt
    .import             __RMP_M6502_Tim_Handler
    ; Zeropage "registers" provided by cc65
    .import             __ZP_START__
    .import             __ZP_SIZE__
    .include            "zeropage.inc"
;/* End Import ***************************************************************/

;/* Macro ********************************************************************/
;/* Push everything to stack *************************************************/
    .macro              RMP_M6502_INT_SAVE
    .local              __RMP_M6502_Int_ZP_Save_PSP_Skip
    .local              __RMP_M6502_Int_ZP_Save
;/* Prologue *****************************************************************/
    ; Clear decimal mode as the kernel never use it
    CLD
    ; Save other registers to RSTK (empty descending)
    PHA
    TXA
    PHA
    TYA
    PHA
;/* Save zeropage ************************************************************/
    ; Decrease PSP to prepare for ZP save
    LDA                 sp
    SEC                 ; SBC interprets borrow as !carry bit
    SBC                 #.LOBYTE(__ZP_SIZE__)
    STA                 sp
    BCS                 __RMP_M6502_Int_ZP_Save_PSP_Skip
    DEC                 sp+1
__RMP_M6502_Int_ZP_Save_PSP_Skip:
    ; Save ZP to PSTK (full descending)
    LDX                 #0
    LDY                 #0
__RMP_M6502_Int_ZP_Save:
    LDA                 .LOBYTE(__ZP_START__),X
    STA                 (sp),Y
    INX
    INY
    CPX                 #.LOBYTE(__ZP_SIZE__)
    BNE                 __RMP_M6502_Int_ZP_Save
;/* Indicate interrupt active ************************************************/
    ; Note that we don't save the stack pointer into RMP_SP_Cur now because
    ; (1) the context is not complete, and (2) we want more performance so we
    ; have to execute interrupt with thread local stack. We only switch to
    ; kernel stack when we really do a heavy-handed context switch.
    LDX                 #1
    STX                 _RMP_M6502_Int_Act
    .endmacro

;/* Load everything from stack, and deal with pending yields if there is *****/
    .macro              RMP_M6502_INT_LOAD
    .local              __RMP_M6502_Int_Ctxsw_Skip
    .local              __RMP_M6502_Int_RSTK_Save_PSP_Skip
    .local              __RMP_M6502_Int_RSTK_Save
    .local              __RMP_M6502_Int_RSTK_Load
    .local              __RMP_M6502_Int_RSTK_Load_PSP_Skip
    .local              __RMP_M6502_Int_ZP_Load
    .local              __RMP_M6502_Int_ZP_Load_PSP_Skip
;/* Detect potential full context switch requirements ************************/
    LDX                 __RMP_M6502_Yield_Pend
    BEQ                 __RMP_M6502_Int_Ctxsw_Skip
    LDX                 #0
    STX                 __RMP_M6502_Yield_Pend
;/* Save return stack ********************************************************/
    ; Decrease PSP to prepare for RSTK/RSP save
    TSX
    TXA                 ; PSP-((0xFF-RSP)+1)=PSP+RSP=RSP+PSP, but borrow=!carry, use BCS
    CLC
    ADC                 sp
    STA                 sp
    BCS                 __RMP_M6502_Int_RSTK_Save_PSP_Skip
    DEC                 sp+1
__RMP_M6502_Int_RSTK_Save_PSP_Skip:
    ; Save current thread RSP to PSTK (full descending)
    LDY                 #0
    TSX                 ; Not really necessary, but we want to make things clear
    TXA
    STA                 (sp),Y
    ; Save RSTK (must have something; empty descending) to PSTK (full descending)
    INX
    INY
__RMP_M6502_Int_RSTK_Save:
    LDA                 $100,X
    STA                 (sp),Y
    INX
    INY
    CPX                 #0
    BNE                 __RMP_M6502_Int_RSTK_Save
;/* Call scheduler ***********************************************************/
    ; Save current thread PSP
    LDA                 sp
    LDX                 sp+1
    STA                 _RMP_SP_Cur
    STX                 _RMP_SP_Cur+1
    ; Rewind RSP and load kernel PSP
    LDX                 #$FF
    TXS
    LDA                 __RMP_M6502_SP_Kern
    LDX                 __RMP_M6502_SP_Kern+1
    STA                 sp
    STX                 sp+1
    ; Call context switch routine
    JSR                 __RMP_Run_High
    ; Load current thread PSP
    LDA                 _RMP_SP_Cur
    LDX                 _RMP_SP_Cur+1
    STA                 sp
    STX                 sp+1
;/* Restore return stack *****************************************************/
    ; Restore current thread RSP from PSTK (full descending)
    LDY                 #0
    LDA                 (sp),Y
    TAX
    TXS
    ; Restore RSTK (must have something; empty descending) from PSTK (full descending)
    INX
    INY
__RMP_M6502_Int_RSTK_Load:
    LDA                 (sp),Y
    STA                 $100,X
    INX
    INY
    CPX                 #0
    BNE                 __RMP_M6502_Int_RSTK_Load
    ; Increase PSP to finish up RSTK/RSP restore
    TSX
    TXA                 ; PSP+((0xFF-RSP))+1)=PSP-RSP=(-RSP)+PSP, use BCC
    EOR                 #$FF
    SEC
    ADC                 sp
    STA                 sp
    BCC                 __RMP_M6502_Int_RSTK_Load_PSP_Skip
    INC                 sp+1
__RMP_M6502_Int_RSTK_Load_PSP_Skip:
;/* Indicate interrupt inactive **********************************************/
__RMP_M6502_Int_Ctxsw_Skip:
    LDX                 #0
    STX                 _RMP_M6502_Int_Act
;/* Restore zeropage *********************************************************/
    ; Restore ZP from PSTK (full descending)
    LDX                 #0
    LDY                 #0
__RMP_M6502_Int_ZP_Load:
    LDA                 (sp),Y
    STA                 .LOBYTE(__ZP_START__),X
    INX
    INY
    CPX                 #.LOBYTE(__ZP_SIZE__)
    BNE                 __RMP_M6502_Int_ZP_Load
    ; Increase PSP to finish up ZP restore
    LDA                 sp
    CLC
    ADC                 #.LOBYTE(__ZP_SIZE__)
    STA                 sp
    BCC                 __RMP_M6502_Int_ZP_Load_PSP_Skip
    INC                 sp+1
__RMP_M6502_Int_ZP_Load_PSP_Skip:
;/* Epilogue *****************************************************************/
    ; Restore other registers from RSTK (empty descending)
    PLA
    TAY
    PLA
    TAX
    PLA
    ; Enable interrupts and return
    RTI
    .endmacro
;/* End Macro ****************************************************************/

;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
