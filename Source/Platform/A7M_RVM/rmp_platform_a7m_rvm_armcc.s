;/*****************************************************************************
;Filename    : rmp_platform_a7m_rvm_armcc.s
;Author      : pry
;Date        : 09/02/2018
;Description : The assembly part of the RMP RTOS for the RVM virtual machine.
;*****************************************************************************/

;/* The ARMv7-M Architecture **************************************************
;R0-R7:General purpose registers that are accessible. 
;R8-R12:General purpose registers that can only be reached by 32-bit instructions.
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
    IMPORT              _RMP_Run_High
    ;The stack address of current thread
    IMPORT              RMP_SP_Cur
    ;Mask/unmask interrupts
    IMPORT              RVM_Virt_Int_Mask
    IMPORT              RVM_Virt_Int_Unmask
    ;Hypercall parameter space
    IMPORT              RVM_Usr_Param
;/* End Import ***************************************************************/

;/* Export *******************************************************************/
    ;Start the first thread
    EXPORT              _RMP_Start
    ;Get the MSB/LSB in the word
    EXPORT              _RMP_A7M_RVM_MSB_Get
    EXPORT              _RMP_A7M_RVM_LSB_Get
    ;Fast-path context switching without invoking the RVM
    EXPORT              _RMP_A7M_RVM_Yield
;/* End Export ***************************************************************/
            
;/* Header *******************************************************************/
    AREA                ARCH,CODE,READONLY,ALIGN=3
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
_RMP_Start              PROC
    MOV                 SP,R1               ;Set the stack pointer
    ISB
    BX                  R0                  ;Branch to target
    ENDP
;/* End Function:_RMP_Start **************************************************/

;/* Function:_RMP_A7M_RVM_MSB_Get *********************************************
;Description : Get the MSB of the word.
;Input       : rvm_ptr_t Val - The value.
;Output      : None.
;Return      : rvm_ptr_t - The MSB position.   
;*****************************************************************************/
_RMP_A7M_RVM_MSB_Get    PROC
    CLZ                 R1,R0
    MOV                 R0,#31
    SUB                 R0,R1
    BX                  LR
    ENDP
;/* End Function:_RMP_A7M_RVM_MSB_Get ****************************************/

;/* Function:_RMP_A7M_RVM_LSB_Get *********************************************
;Description : Get the LSB of the word.
;Input       : rvm_ptr_t Val - The value.
;Output      : None.
;Return      : rvm_ptr_t - The LSB position.   
;*****************************************************************************/
_RMP_A7M_RVM_LSB_Get    PROC
    RBIT                R0,R0
    CLZ                 R0,R0
    BX                  LR
    ENDP
;/* End Function:_RMP_A7M_RVM_LSB_Get ****************************************/

;/* Function:_RMP_A7M_RVM_Yield ***********************************************
;Description : Switch from user code to another thread, rather than from the 
;              interrupt handler. Need to masquerade the context well so that
;              it may be recovered from the interrupt handler as well.
;              Caveats: 
;              1. User-level code cannot clear CONTROL.FPCA (this is correct, 
;                 if it can then there is a serious security loophole) hence 
;                 all threads in the system will be tainted with the FPU active
;                 flag and include a full context save/restore. However this is
;                 still hugely faster than the slow path through the PendSV.
;              2. After the user have stacked up everything on its stack but
;                 not disabled its interrupt yet, an interrupt may occur, and
;                 stack again on the user stack. This is allowed, but must be
;                 taken into account when calculating stack usage.
;
;              The exception extended stack layout is as follows:
;
;               Unaligned           Aligned
;               Reserved           
;               Reserved            Reserved
;               FPSCR               FPSCR
;               S15                 S15
;               S14                 S14
;               S13                 S13
;               S12                 S12
;               S11                 S11
;               S10                 S10
;               S9                  S9
;               S8                  S8
;               S7                  S7
;               S6                  S6
;               S5                  S5
;               S4                  S4
;               S3                  S3
;               S2                  S2
;               S1                  S1
;               S0                  S0
;               XPSR                XPSR
;               PC                  PC
;               LR                  LR
;               R12                 R12
;               R3                  R3
;               R2                  R2
;               R1                  R1
;               R0                  R0
;
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
;/* Exception Entry Stacking *************************************************/
    ;Alignment detection
    MACRO
    ALIGN_PUSH          $LABEL              ;Cannot TST SP, unpredictable
    MOV                 R0,SP               ;Is SP aligned to 8 bytes?
    TST                 R0,#0x00000007
    BNE                 $LABEL              ;We pushed two words before, thus NE
    MEND

    ;Exception stacking for basic frame
    MACRO
    EXC_PUSH            $SZ,$XPSR,$LR
    LDR                 R0,[SP,#4]          ;Load real R0 value pushed at start
    IF                  (($SZ-8)!=0)        ;Adjust SP to push GP regs when needed
    SUB                 SP,#4*($SZ-8)
    ENDIF
    PUSH                {R0-R3,R12,LR}      ;Push stack frame GP regs
    LDR                 R0,[SP,#4*($SZ-2)]  ;Load real XPSR value pushed at start
    LDR                 R1,=$XPSR
    ORR                 R0,R1
    STR                 R0,[SP,#4*7]
    LDR                 R0,=_RMP_A7M_Skip   ;Push PC with[0] cleared
    AND                 R0,#0xFFFFFFFE
    STR                 R0,[SP,#4*6]
    MOV                 LR,#$LR             ;Make up the EXC_RETURN
    MEND
                        
;/* Exception Exit Unstacking ************************************************/
    ;Alignment detection
    MACRO
    ALIGN_POP           $LABEL
    LDR                 R0,[SP,#4*7]        ;Load up XPSR
    TST                 R0,#0x00000200
    BNE                 $LABEL
    MEND
                        
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
    MACRO
    EXC_POP             $SZ
    LDR                 LR,[SP,#4*5]        ;Restore LR
    POP                 {R0}                ;Load R0/XPSR/PC into R0/R1/R2
    LDR                 R1,[SP,#4*6]
    AND                 R1,#0xFFFFFDFF      ;Clear XPSR[9]
    LDR                 R2,[SP,#4*5]
    ORR                 R2,#0x00000001      ;Set PC[0]
    STR                 R1,[SP,#4*($SZ-4)]  ;Rearrange to H-PC-R0-XPSR-L
    STR                 R0,[SP,#4*($SZ-3)]
    STR                 R2,[SP,#4*($SZ-2)]
    POP                 {R1-R3,R12}         ;Pop GP regs
    IF                  (($SZ-8)!=0)        ;Skip [PAD] and [FPU] when needed
    ADD                 SP,#4*($SZ-8)
    ENDIF
    POP                 {R0}                ;Pop XPSR through R0
    MSR                 XPSR,R0
    POP                 {R0,PC}             ;Pop R0 and PC
    MEND
                        
;/* User-level Context Switch ************************************************/
    AREA                FASTYIELD,CODE,READONLY,ALIGN=3
_RMP_A7M_RVM_Yield      PROC
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
    B                   Stk_Basic_Done
                        
Stk_Extend                                  ;Extended frame
    ALIGN_PUSH          Stk_Extend_Unalign
Stk_Extend_Align                            ;Aligned stack
    EXC_PUSH            8+17+1,0x01000000,0xFFFFFFED
    B                   Stk_Extend_Done
Stk_Extend_Unalign                          ;Unaligned stack
    EXC_PUSH            8+17+2,0x01000200,0xFFFFFFED
Stk_Extend_Done                             ;Extended frame extra pushing
    ADD                 R0,SP,#4*8          ;Locate FPU stacking area
    DCI                 0xECA0              ;Push FPU regs
    DCI                 0x0A10              ;VSTMIA R0!,{S0-S15}
    DCI                 0xEEF1              ;Push FPSCR
    DCI                 0x1A10              ;VMRS R1,FPSCR
    STR                 R1,[R0]
    DCI                 0xED2D              ;Push OS-managed FPU regs
    DCI                 0x8A10              ;VPUSH {S16-S31}

Stk_Basic_Done       
    PUSH                {R4-R11,LR}         ;Push GP regs
    LDR                 R0,=RVM_Usr_Param
    LDR                 R0,[R0]             ;Push hypercall parameters
    LDMIA               R0,{R1-R5}
    PUSH                {R1-R5}

    BL                  RVM_Virt_Int_Mask   ;Mask interrupts
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
    DCI                 0xECBD              ;Pop OS-managed FPU regs
    DCI                 0x8A10              ;VPOP {S16-S31}           
    ADD                 R0,SP,#4*(8+16)
    LDR                 R1,[R0]
    DCI                 0xEEE1              ;Pop FPSCR
    DCI                 0x1A10              ;VMSR FPSCR,R1
    DCI                 0xED30              ;Pop FPU regs
    DCI                 0x0A10              ;VLDMDB R0!,{S0-S15}
    ALIGN_POP           Uns_Extend_Unalign
Uns_Extend_Align                            ;Aligned stack
    EXC_POP             8+17+1
Uns_Extend_Unalign                          ;Unaligned stack
    EXC_POP             8+17+2

_RMP_A7M_Skip                               ;Exit location
    BX                  LR
    ENDP
;/* End Function:_RMP_A7M_RVM_Yield ******************************************/
    ALIGN
    LTORG
    END
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
