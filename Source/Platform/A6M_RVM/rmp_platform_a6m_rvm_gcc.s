/******************************************************************************
Filename    : rmp_platform_cmx_rvm_asm.S
Author      : pry
Date        : 09/02/2018
Description : The assembly part of the RMP RTOS for the RVM virtual machine.
******************************************************************************/

/* The ARMv6-M Architecture ***************************************************
R0-R7:General purpose registers that are accessible. 
R8-R12:General purpose registers that can only be reached by 32-bit instructions.
R13:SP/SP_process/SP_main    Stack pointer
R14:LR                       Link Register(used for returning from a subfunction)
R15:PC                       Program counter.
IPSR                         Interrupt Program Status Register.
APSR                         Application Program Status Register.
EPSR                         Execute Program Status Register.
The above 3 registers are saved into the stack in combination(xPSR).
The ARM Cortex-M4/7 also include a FPU.
******************************************************************************/
    .syntax             unified
    .arch               armv6s-m
    .thumb
/* Import ********************************************************************/
    /* The real task switch handling function */
    .extern             _RMP_Run_High
    /* The stack address of current thread */
    .extern             RMP_SP_Cur
    /* Mask/unmask interrupts */
    .extern             RMP_Int_Mask
    .extern             RMP_Int_Unmask
    /* Hypercall parameter space */
    .extern             RMP_A6M_RVM_Usr_Param
/* End Import ****************************************************************/

/* Export ********************************************************************/
    /* Start the first thread */
    .global             _RMP_Start
    /* Fast-path context switching without invoking the RVM */
    .global             _RMP_A6M_RVM_Yield
/* End Export ****************************************************************/

/* Header ********************************************************************/
    .section            ".text.arch"
    .align              3
/* End Header ****************************************************************/

/* Function:_RMP_Start ********************************************************
Description : Jump to the user function and will never return from it.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
    .thumb_func
_RMP_Start:
    MOV                 SP,R1               /* Set the stack pointer */                
    ISB
    BLX                 R0                  /* Branch to our target */
/* End Function:_RMP_Start ***************************************************/

/* Function:_RMP_A6M_RVM_Yield ************************************************
Description : Switch from user code to another thread, rather than from the 
              interrupt handler. Need to masquerade the context well so that
              it may be recovered from the interrupt handler as well.
              Caveats: 
              1. User-level code cannot clear CONTROL.FPCA (this is correct, 
                 if it can then there is a serious security loophole) hence 
                 all threads in the system will be tainted with the FPU active
                 flag and include a full context save/restore. However this is
                 still hugely faster than the slow path through the PendSV.
              2. After the user have stacked up everything on its stack but
                 not disabled its interrupt yet, an interrupt may occur, and
                 stack again on the user stack. This is allowed, but must be
                 taken into account when calculating stack usage.

              The exception extended stack layout is as follows:

               Unaligned           Aligned
               Reserved           
               XPSR                XPSR
               PC                  PC
               LR                  LR
               R12                 R12
               R3                  R3
               R2                  R2
               R1                  R1
               R0                  R0

Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
/* Exception Entry Stacking **************************************************/
    /* Alignment detection */
    .macro              ALIGN_PUSH,LABEL    /* Cannot TST SP, unpredictable */
    MOV                 R0,SP               /* Is SP aligned to 8 bytes? */
    LDR                 R1,=0x00000007
    TST                 R0,R1
    BEQ                 \LABEL              /* We pushed three words before, thus EQ */
    .endm

    /* Exception stacking */
    .macro              EXC_PUSH,SZ,XPSR,LR
    LDR                 R0,[SP,#4*2]        /* Load real R0/R1 value pushed at start */
    LDR                 R1,[SP]
    SUB                 SP,#4*(\SZ-7)       /* Adjust SP to push GP regs */
    PUSH                {R0-R3}             /* Push stack frame GP regs */
    MOV                 R0,R12
    MOV                 R1,LR
    STR                 R0,[SP,#4*4]
    STR                 R1,[SP,#4*5]
    LDR                 R0,[SP,#4*(\SZ-2)]  /* Load real XPSR value pushed at start */
    LDR                 R1,=\XPSR
    ORRS                R0,R1
    STR                 R0,[SP,#4*7]
    LDR                 R0,=_RMP_A6M_Skip   /* Push PC with[0] cleared */
    LDR                 R1,=0xFFFFFFFE
    ANDS                R0,R1
    STR                 R0,[SP,#4*6]
    LDR                 R1,=\LR             /* Make up the EXC_RETURN */
    MOV                 LR,R1
    .endm

/* Exception Exit Unstacking *************************************************/
    /* Alignment detection */
    .macro              ALIGN_POP,LABEL  
    LDR                 R0,[SP,#4*7]        /* Load up XPSR */
    LDR                 R1,=0x00000200
    TST                 R0,R1 
    BNE                 \LABEL  
    .endm

    /* Exception unstacking for basic frame:
     * The original sequence is [PAD] XPSR PC LR R12 R3-R0. This is not
     * ideal for manual restoring, because restoring PC and SP simulaneously
     * must be the last step. Thus, we need to transform it.
     * 1. When there is [PAD]:
     *    Original    : [PAD] XPSR PC   LR      R12 R3  - R0
     *    Transformed : PC    R0   XPSR [EMPTY] R12 R3-R1 LR
     * 2. When there is no [PAD]:
     *    Original    : XPSR PC LR   R12 R3  - R0
     *    Transformed : PC   R0 XPSR R12 R3-R1 LR
     * This is done in 4 steps:
     * 1. Restore LR from the stack, so it won't be overwritten;
     * 2. Pop off R0, load XPSR, load PC, and rearrange them at the end;
     * 3. Pop off R1-R3, pop off LR, then skip the middle;
     * 4. Restore XPSR through R0, then restore R0 and PC.
     * Note that in the transformation we never place variables at lower 
     * addresses than the current SP, as this will run the risk of a racing
     * interrupt erasing the variable. */
    .macro              EXC_POP,SZ
    LDR                 R0,[SP,#4*4]        /* Restore LR/R12 */
    LDR                 R1,[SP,#4*5]
    MOV                 R12,R0
    MOV                 LR,R1
    POP                 {R0}                /* Load R0/XPSR/PC into R0/R1/R2 */
    LDR                 R1,[SP,#4*6]
    LDR                 R3,=0xFFFFFDFF      /* Clear XPSR[9] */
    ANDS                R1,R3
    LDR                 R2,[SP,#4*5]
    LDR                 R3,=0x00000001      /* Set PC[0] */
    ORRS                R2,R3
    STR                 R1,[SP,#4*(\SZ-4)]  /* Rearrange to H-PC-R0-XPSR-L */
    STR                 R0,[SP,#4*(\SZ-3)]
    STR                 R2,[SP,#4*(\SZ-2)]
    POP                 {R1-R3}             /* Pop GP regs */
    ADD                 SP,#4*(\SZ-7)       /* Skip R12/[PAD] */
    POP                 {R0}                /* Pop XPSR through R0 */
    MSR                 XPSR_NZCVQ,R0
    POP                 {R0,PC}             /* Pop R0 and PC */
    .endm

/* User-level Context Switch *************************************************/
    .section            ".text.fastyield"
    .align              3

    .thumb_func
_RMP_A6M_RVM_Yield:
    PUSH                {R0}                /* Protect R0, XPSR and R1 */
    MRS                 R0,XPSR
    PUSH                {R0}
    PUSH                {R1}

    ALIGN_PUSH          Stk_Unalign
Stk_Align:                                  /* Aligned stack */
    EXC_PUSH            8,0x01000000,0xFFFFFFFD
    B                   Stk_Done
Stk_Unalign:                                /* Unaligned stack */
    EXC_PUSH            8+1,0x01000200,0xFFFFFFFD

Stk_Done:
    MOV                 R3,R11              /* Push GP regs */
    MOV                 R2,R10
    MOV                 R1,R9
    MOV                 R0,R8
    PUSH                {R0-R3,LR}
    PUSH                {R4-R7}

    LDR                 R0,=RMP_A6M_RVM_Usr_Param
    LDR                 R0,[R0]             /* Push hypercall parameters */
    LDMIA               R0!,{R1-R5}
    PUSH                {R1-R5}

    BL                  RMP_Int_Mask        /* Mask interrupts */
    LDR                 R1,=RMP_SP_Cur      /* Save the SP to control block */
    MOV                 R0,SP
    STR                 R0,[R1]
    BL                  _RMP_Run_High       /* Get the highest ready task */
    LDR                 R1,=RMP_SP_Cur      /* Load the SP */
    LDR                 R0,[R1]
    MOV                 SP,R0
    BL                  RMP_Int_Unmask      /* Unmask interrupts */

    LDR                 R0,=RMP_A6M_RVM_Usr_Param
    LDR                 R0,[R0]             /* Pop hypercall parameters */
    POP                 {R1-R5}
    STMIA               R0!,{R1-R5}

    POP                 {R4-R7}             /* Pop GP regs */
    POP                 {R0-R3}
    MOV                 R8,R0
    MOV                 R9,R1
    MOV                 R10,R2
    MOV                 R11,R3
    POP                 {R0}
    MOV                 LR,R0

    ALIGN_POP           Uns_Unalign
Uns_Align:                                  /* Aligned stack */
    EXC_POP             8
Uns_Unalign:                                /* Unaligned stack */
    EXC_POP             8+1

_RMP_A6M_Skip:                              /* Exit location */
    BX                  LR
/* End Function:_RMP_A6M_RVM_Yield *******************************************/
    .end
/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/

