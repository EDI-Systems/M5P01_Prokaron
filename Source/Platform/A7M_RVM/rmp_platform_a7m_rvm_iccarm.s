;/*****************************************************************************
;Filename    : rmp_platform_a7m_rvm_iccarm.s
;Author      : pry ; Cline (ported from the armcc variant)
;Date        : 22/10/2026
;Licence     : The Unlicense; see LICENSE for details.
;Description : The assembly part of the RMP RTOS for the RVM virtual machine.
;              This is the IAR (iasmarm) port.
;*****************************************************************************/

;/* The ARMv7-M Architecture **************************************************
;R0-R7:General purpose registers that are accessible. 
;R8-R12:general purpose registers that can only be reached by 32-bit instructions.
;R13:SP/SP_process/SP_main    Stack pointer
;R14:LR                       Link Register(used for returning from a subfunction)
;R15:PC                       Program counter.
;IPSR                         Interrupt Program Status Register.
;APSR                         Application Program Status Register.
;EPSR                         Execute Program Status Register.
;The above 3 registers are saved into the stack in combination(xPSR).
;The ARM Cortex-M4/7 also include a FPU.
;*****************************************************************************/

;/* Import *******************************************************************/
        ;The real task switch handling function
        EXTERN              _RMP_Run_High
        ;The stack address of current thread
        EXTERN              RMP_SP_Cur
        ;Mask/unmask interrupts
        EXTERN              RVM_Virt_Int_Mask
        EXTERN              RVM_Virt_Int_Unmask
        ;Hypercall parameter space
        EXTERN              RVM_Usr_Param
        ;Wrong ICI/IT bits in fast context switch
        EXTERN              _RMP_A7M_RVM_Yield_Err
;/* End Import ***************************************************************/

;/* Export *******************************************************************/
        ;Start the first thread
        PUBLIC              _RMP_Start
        ;Get the MSB/LSB in the word
        PUBLIC              _RMP_A7M_RVM_MSB_Get
        PUBLIC              _RMP_A7M_RVM_LSB_Get
        ;Fast-path context switching without invoking the RVM
        PUBLIC              _RMP_A7M_RVM_Yield_Swt
;/* End Export ***************************************************************/

;/* Header *******************************************************************/
        MODULE              RMP_A7M_RVM
        SECTION             .arch:CODE:NOROOT(3)
        THUMB
        REQUIRE8
        PRESERVE8
;/* End Header ***************************************************************/

;/* Function:_RMP_Start *******************************************************
;Description : Jump to the user function and will never return from it.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_Start
        MOV                 SP,R1               ;Set the stack pointer
        ISB
        BX                  R0                  ;Branch to target
;/* End Function:_RMP_Start **************************************************/

;/* Function:_RMP_A7M_RVM_MSB_Get *********************************************
;Description : Get the MSB of the word.
;Input       : rvm_ptr_t Val - The value.
;Output      : None.
;Return      : rvm_ptr_t - The MSB position.   
;*****************************************************************************/
_RMP_A7M_RVM_MSB_Get
        CLZ                 R1,R0
        MOV                 R0,#31
        SUB                 R0,R0,R1
        BX                  LR
;/* End Function:_RMP_A7M_RVM_MSB_Get ****************************************/

;/* Function:_RMP_A7M_RVM_LSB_Get *********************************************
;Description : Get the LSB of the word.
;Input       : rvm_ptr_t Val - The value.
;Output      : None.
;Return      : rvm_ptr_t - The LSB position.   
;*****************************************************************************/
_RMP_A7M_RVM_LSB_Get
        RBIT                R0,R0
        CLZ                 R0,R0
        BX                  LR
;/* End Function:_RMP_A7M_RVM_LSB_Get ****************************************/

;/* Exception Entry Stacking *************************************************/
        ;Alignment detection
ALIGN_PUSH MACRO skipTo                         ;Cannot TST SP, unpredictable
        MOV                 R0,SP               ;Is SP aligned to 8 bytes?
        TST                 R0,#0x00000007
        BNE                 skipTo              ;We pushed two words before, thus NE
        ENDM

        ;Exception stacking for basic frame
EXC_PUSH MACRO EXCSZ, EXCXPSR, EXCLR
        LDR                 R0,[SP,#4]          ;Load real R0 value pushed at start
        IF                  ((EXCSZ-8)!=0)      ;Adjust SP to push GP regs when needed
        SUB                 SP,SP,#4*(EXCSZ-8)
        ENDIF
        PUSH                {R0-R3,R12,LR}      ;Push stack frame GP regs
        LDR                 R0,[SP,#4*(EXCSZ-2)];Load real XPSR value pushed at start
        LDR                 R1,=EXCXPSR
        ORR                 R0,R1
        STR                 R0,[SP,#4*7]
        LDR                 R0,=_RMP_A7M_Skip   ;Push PC with[0] cleared
        AND                 R0,R0,#0xFFFFFFFE
        STR                 R0,[SP,#4*6]
        MOV                 LR,#EXCLR           ;Make up the EXC_RETURN
        ENDM
                        
;/* Exception Exit Unstacking ************************************************/
        ;Alignment detection
ALIGN_POP MACRO skipTo
        LDR                 R0,[SP,#4*7]        ;Load up XPSR
        TST                 R0,#0x00000200
        BNE                 skipTo
        ENDM
                        
        ;Exception unstacking for basic frame:
        ;The original sequence is [PAD] [FPU] XPSR PC LR R12 R3-R0. This is not
        ;ideal for manual restoring, because restoring PC and SP simultaneously
        ;must be the last step. Thus, we need to transform it.
        ;1. When there is [PAD] or [FPU]:
        ;   Original    : [PAD] [FPU]  XPSR PC LR   R12 R3  - R0
        ;   Transformed : PC   R0 XPSR   [EMPTY]    R12 R3-R1 LR
        ;2. When there is neither [PAD] nor [FPU]:
        ;   Original    : XPSR PC LR   R12 R3  - R0
        ;   Transformed : PC   R0 XPSR R12 R3-R1 LR
        ;This is done in 4 steps:
        ;1. Restore LR from the stack, so it won't be overwritten;
        ;2. Pop off R0, load XPSR, load PC, and rearrange them at the end;
        ;3. Pop off R1-R3, pop off LR, then skip the middle;
        ;4. Restore XPSR through R0, then restore R0 and PC.
        ;Note that in the transformation we never place variables at lower 
        ;addresses than the current SP, as this will run the risk of a racing
        ;interrupt erasing the variable.
EXC_POP MACRO EXCSZ
        LDR                 LR,[SP,#4*5]        ;Restore LR
        POP                 {R0}                ;Load R0/XPSR/PC into R0/R1/R2
        LDR                 R1,[SP,#4*6]
        LDR                 R12,=0x0600FC00     ;Detect dirty XPSR ICI/IT on the fly
        TST                 R1,R12
        BNE                 _RMP_A7M_RVM_Yield_Err
        AND                 R1,R1,#0xFFFFFDFF   ;Clear XPSR[9]
        LDR                 R2,[SP,#4*5]
        ORR                 R2,R2,#0x00000001   ;Set PC[0]
        STR                 R1,[SP,#4*(EXCSZ-4)];Rearrange to H-PC-R0-XPSR-L
        STR                 R0,[SP,#4*(EXCSZ-3)]
        STR                 R2,[SP,#4*(EXCSZ-2)]
        POP                 {R1-R3,R12}         ;Pop GP regs
        IF                  ((EXCSZ-8)!=0)      ;Skip [PAD] and [FPU] when needed
        ADD                 SP,SP,#4*(EXCSZ-8)
        ENDIF
        POP                 {R0}                ;Pop XPSR through R0
        MSR                 XPSR,R0
        POP                 {R0,PC}             ;Pop R0 and PC
        ENDM
                        
;/* User-level Context Switch ************************************************/
        SECTION             .fastyield:CODE:NOROOT(3)
        THUMB
        REQUIRE8
        PRESERVE8

_RMP_A7M_RVM_Yield_Swt
        PUSH                {R0}                ;Protect R0 and XPSR
        MRS                 R0,XPSR
        PUSH                {R0}
        MRS                 R0,CONTROL
        TST                 R0,#0x00000004      ;CONTROL.FPCA
        BNE                 Stk_Extend
                        
Stk_Basic                                   ;Basic frame
        ALIGN_PUSH          Stk_Basic_Unalign
Stk_Basic_Align                             ;Aligned stack
        EXC_PUSH            8,0x01000000,0xFFFFFFFD
        B                   Stk_Basic_Done
Stk_Basic_Unalign                           ;Unaligned stack
        EXC_PUSH            8+1,0x01000200,0xFFFFFFFD
                        
Stk_Extend                                  ;Extended frame
        ALIGN_PUSH          Stk_Extend_Unalign
Stk_Extend_Align                            ;Aligned stack
        EXC_PUSH            8+17+1,0x01000000,0xFFFFFFED
        B                   Stk_Extend_Done
Stk_Extend_Unalign                          ;Unaligned stack
        EXC_PUSH            8+17+2,0x01000200,0xFFFFFFED
Stk_Extend_Done                             ;Extended frame extra pushing
        ADD                 R0,SP,#4*8          ;Locate FPU stacking area
        DC16                0xECA0              ;Push FPU regs
        DC16                0x0A10              ;VSTMIA R0!,{S0-S15}
        DC16                0xEEF1              ;Push FPSCR
        DC16                0x1A10              ;VMRS R1,FPSCR
        STR                 R1,[R0]
        DC16                0xED2D              ;Push OS-managed FPU regs
        DC16                0x8A10              ;VPUSH {S16-S31}

Stk_Basic_Done
        PUSH                {R4-R11,LR}         ;Push GP regs
        LDR                 R0,=RVM_Usr_Param
        LDR                 R0,[R0]             ;Push hypercall parameters
        LDMIA               R0,{R1-R5}
        PUSH                {R1-R5}

        ;BL                  RVM_Virt_Int_Mask   Mask interrupts - masked externally
        LDR                 R1,=RMP_SP_Cur      ;Save the SP to control block
        STR                 SP,[R1]
        BL                  _RMP_Run_High       ;Get the highest ready task
        LDR                 R1,=RMP_SP_Cur      ;Load the SP
        LDR                 SP,[R1]
        BL                  RVM_Virt_Int_Unmask ;Unmask interrupts

        LDR                 R0,=RVM_Usr_Param
        LDR                 R0,[R0]             ;Pop hypercall parameters
        POP                 {R1-R5}
        STMIA               R0,{R1-R5}
        POP                 {R4-R11,LR}         ;Pop GP regs

        TST                 LR,#0x00000010      ;LR.EXTENDED
        BEQ                 Uns_Extend

Uns_Basic                                   ;Basic frame
        ALIGN_POP           Uns_Basic_Unalign
Uns_Basic_Align                             ;Aligned stack
        EXC_POP             8
Uns_Basic_Unalign                           ;Unaligned stack
        EXC_POP             8+1
                        
Uns_Extend                                  ;Extended frame extra popping
        DC16                0xECBD              ;Pop OS-managed FPU regs
        DC16                0x8A10              ;VPOP {S16-S31}           
        ADD                 R0,SP,#4*(8+16)
        LDR                 R1,[R0]
        DC16                0xEEE1              ;Pop FPSCR
        DC16                0x1A10              ;VMSR FPSCR,R1
        DC16                0xED30              ;Pop FPU regs
        DC16                0x0A10              ;VLDMDB R0!,{S0-S15}
        ALIGN_POP           Uns_Extend_Unalign
Uns_Extend_Align                            ;Aligned stack
        EXC_POP             8+17+1
Uns_Extend_Unalign                          ;Unaligned stack
        EXC_POP             8+17+2

_RMP_A7M_Skip                               ;Exit location
        BX                  LR
        LTORG
;/* End Function:_RMP_A7M_RVM_Yield_Swt **************************************/

        END
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
