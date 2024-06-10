;/*****************************************************************************
;Filename    : rmp_platform_m6502_cc65.s
;Author      : pry
;Date        : 10/04/2012
;Description : The assembly part of the RMP RTOS. This is for MOS 6502.
;*****************************************************************************/

;/* The MOS 6502 Architecture *************************************************
;PSP        : Zeropage software stack pointer, for parameter passing.
;Z0-ZX      : Zeropage "registers" whose number can be configured.
;A          : Accumulator.
;X          : X index - less capable.
;Y          : Y index - more capable.
;SP         : Stack pointer - $100 to $1FF, for call return addresses.
;PC         : Program counter - 16 bits.
;PF         : Processor flags - NV1BDIZC.
;*****************************************************************************/

;/* Import *******************************************************************/
    ; The real task switch handling function
    .import             __RMP_Run_High
    ; The PID of the current thread
    .import             _RMP_Thd_Cur
    ; The stack address of current thread
    .import             _RMP_SP_Cur
    ; The kernel stack
    .import             __RMP_M6502_SP_Kern
    ; Zeropage "registers" provided by cc65
    .import             __ZP_START__
    .import             __ZP_SIZE__
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
    ; Place thread PSP on tmps - safe because we're leaving context
    STA                 tmp1
    STX                 tmp2
    ; Rewind RSTK to top location
    LDX                 #$FF
    TXS
    ; Extract entry from PSTK, with X lower A higher
    LDY                 #0
    LDA                 (sp),Y
    TAX
    INY
    LDA                 (sp),Y
    ; Need to -1 to compensate for RTS +1 behavior
    DEX
    BNE                 __RMP_Start_Entry_Skip
    SEC                 ; SBC interprets borrow as !carry bit
    SBC                 #1
__RMP_Start_Entry_Skip:
    ; Push entry onto RSTK, make sure it is little-endian
    PHA
    TXA
    PHA
    ; Increase PSP by 2 to pop the parameter away, X lower A higher
    LDX                 sp
    LDA                 sp+1
    INX
    BEQ                 __RMP_Start_SP_Skip1
    INX
    BEQ                 __RMP_Start_SP_Skip2
    JMP                 __RMP_Start_SP_Skip3
__RMP_Start_SP_Skip1:
    INX
__RMP_Start_SP_Skip2:
    CLC
    ADC                 #1
__RMP_Start_SP_Skip3:
    ; Save current PSP as kernel PSP
    STX                 __RMP_M6502_SP_Kern
    STA                 __RMP_M6502_SP_Kern+1
    ; Set thread PSP from tmps
    LDA                 tmp1
    LDX                 tmp2
    STA                 sp
    STX                 sp+1
    ; Return to the entry - don't use JMP indirect to avoid cross-page bugs
    RTS
    ; Should not reach here
;/* End Function:_RMP_Start **************************************************/

;/* Function:_RMP_Yield *******************************************************
;Description : Trigger a yield to another thread. 6502 is not fast, so we're
;              trying to maximally exploit what we can get. A context switch
;              consists of 7 chapters that cost ~4000 cycles in total, which
;              is about 15% CPU power (1.8 MHz) when it happens at 60Hz. Stack
;              manipulation alone is 1600 cycles.
;              This is probably the most phenomenal RMP context switch routine.
;              Note that the RTS will +1 but RTI will not, hence we must 
;              increase the PC pushed onto the stack by 1 so that the stack
;              frame is restorable with RTI. This is faster than the dedicated
;              jump label approach. We usually won't exploit such features to
;              keep a clean mental model, but here we're forced to gain more
;              performance. We usually do this only if the call and interrupt
;              entry share something exactly common, i.e. pushing the same PC.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
__RMP_M6502_Yield:
;/* Prologue *****************************************************************/
    ; Disable interrupts
    SEI
    ; Save PF to RSTK (empty descending)
    PHP
    ; Clear decimal mode as the kernel never use it
    CLD
    ; Save other registers to RSTK (empty descending)
    PHA
    TXA
    PHA
    TYA
    PHA
    ; Make sure interrupts are enabled in the pushed PF
    TSX
    LDA                 #$FB
    AND                 $104,X
    STA                 $104,X
    ; Increase the PC pushed by the RTS by 1 so that it is restorable with RTI
    INC                 $105,X
    BNE                 __RMP_M6502_Yield_PC_Skip
    INC                 $105,X
__RMP_M6502_Yield_PC_Skip:

;/* Save zeropage ************************************************************/
    ; Decrease PSP to prepare for ZP save
    LDA                 sp
    SEC                 ; SBC interprets borrow as !carry bit
    SBC                 #.LOBYTE(__ZP_SIZE__)
    STA                 sp
    BCS                 __RMP_M6502_Yield_ZP_Save_PSP_Skip
    DEC                 sp+1
__RMP_M6502_Yield_ZP_Save_PSP_Skip:
    ; Save ZP to PSTK (full descending)
    LDX                 #0
    LDY                 #0
__RMP_M6502_Yield_ZP_Save:
    LDA                 .LOBYTE(__ZP_START__),X
    STA                 (sp),Y
    INX
    INY
    CPX                 #.LOBYTE(__ZP_SIZE__)
    BNE                 __RMP_M6502_Yield_ZP_Save

;/* Save return stack ********************************************************/
    ; Decrease PSP to prepare for RSTK/RSP save
    TSX
    TXA                 ; PSP-((0xFF-RSP)+1)=PSP+RSP=RSP+PSP, but borrow=!carry, use BCS
    CLC
    ADC                 sp
    STA                 sp
    BCS                 __RMP_M6502_Yield_RSTK_Save_PSP_Skip
    DEC                 sp+1
__RMP_M6502_Yield_RSTK_Save_PSP_Skip:
    ; Save current thread RSP to PSTK (full descending)
    LDY                 #0
    TSX                 ; Not really necessary, but we want to make things clear
    TXA
    STA                 (sp),Y
    ; Save RSTK (must have something; empty descending) to PSTK (full descending)
    INX
    INY
__RMP_M6502_Yield_RSTK_Save:
    LDA                 $100,X
    STA                 (sp),Y
    INX
    INY
    CPX                 #0
    BNE                 __RMP_M6502_Yield_RSTK_Save

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
__RMP_M6502_Yield_RSTK_Load:
    LDA                 (sp),Y
    STA                 $100,X
    INX
    INY
    CPX                 #0
    BNE                 __RMP_M6502_Yield_RSTK_Load
    ; Increase PSP to finish up RSTK/RSP restore
    TSX
    TXA                 ; PSP+((0xFF-RSP))+1)=PSP-RSP=(-RSP)+PSP, use BCC
    EOR                 #$FF
    SEC
    ADC                 sp
    STA                 sp
    BCC                 __RMP_M6502_Yield_RSTK_Load_PSP_Skip
    INC                 sp+1
__RMP_M6502_Yield_RSTK_Load_PSP_Skip:

;/* Restore zeropage *********************************************************/
    ; Restore ZP from PSTK (full descending)
    LDX                 #0
    LDY                 #0
__RMP_M6502_Yield_ZP_Load:
    LDA                 (sp),Y
    STA                 .LOBYTE(__ZP_START__),X
    INX
    INY
    CPX                 #.LOBYTE(__ZP_SIZE__)
    BNE                 __RMP_M6502_Yield_ZP_Load
    ; Increase PSP to finish up ZP restore
    LDA                 sp
    CLC
    ADC                 #.LOBYTE(__ZP_SIZE__)
    STA                 sp
    BCC                 __RMP_M6502_Yield_ZP_Load_PSP_Skip
    INC                 sp+1
__RMP_M6502_Yield_ZP_Load_PSP_Skip:

;/* Epilogue *****************************************************************/
    ; Restore other registers from RSTK (empty descending)
    PLA
    TAY
    PLA
    TAX
    PLA
    ; Enable interrupts and return
    RTI
;/* End Function:_RMP_Yield **************************************************/
    .end
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
