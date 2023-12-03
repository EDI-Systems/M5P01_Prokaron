/******************************************************************************
Filename    : rmp_platform_rv32p_gcc.s
Author      : pry
Date        : 10/04/2012
Description : The assembly part of the RMP RTOS. This is for RV32 with
              physical memory access, hence RV32P.
              As an open source processor IP, RISC-V have many configurations
              that are implementation-specific. To maximize the applicability
              of this port, certain modifications to the vendor-supplied
              firmware might be required:
              1. The linker script must export the _RMP_Global, which is
                 useful for global variable relative indexing.
              2. The linker script must export the _RMP_Stack, which is
                 useful for kernel execution in a context switch.
              3. The startup assembly file takes care of global variable and
                 chip clock initialization.
              To avoid changing these files directly, it is recommended to
              supply extra files in place of, or in addition to certain
              vendor-supplied files.
******************************************************************************/

/* The RISC-V RV32G Architecture **********************************************
x0:Hardwired register containing "0".
x1-x31:General purpose registers that can only be reached by 32-bit instructions.
pc:Program counter.
Note that all registers names are lower-case only.
Detailed usage convention for x0-x31:
No.    Name       Explanation
x0     $zero      hard-wired zero
x1     $ra        return address (caller-save)
x2     $sp        stack pointer (callee-save)
x3     $gp        global pointer
x4     $tp        thread pointer
x5     $t0        temporary (caller-save)
x6     $t1        temporary (caller-save)
x7     $t2        temporary (caller-save)
x8     $s0/fp     saved register/frame pointer (callee-save)
x9     $s1        saved register (callee-save)
x10    $a0        argument/return value (caller-save)
x11    $a1        argument/return value (caller-save)
x12    $a2        argument (caller-save)
x13    $a3        argument (caller-save)
x14    $a4        argument (caller-save)
x15    $a5        argument (caller-save)
x16    $a6        argument (caller-save)
x17    $a7        argument (caller-save)
x18    $s2        saved register (callee-save)
x19    $s3        saved register (callee-save)
x20    $s4        saved register (callee-save)
x21    $s5        saved register (callee-save)
x22    $s6        saved register (callee-save)
x23    $s7        saved register (callee-save)
x24    $s8        saved register (callee-save)
x25    $s9        saved register (callee-save)
x26    $s10       saved register (callee-save)
x27    $s11       saved register (callee-save)
x28    $t3        temporary (caller-save)
x29    $t4        temporary (caller-save)
x30    $t5        temporary (caller-save)
x31    $t6        temporary (caller-save)
pc     $pc        program counter

On chips that have a FPU, the layout of the FPU registers are:
f0     $ft0       temporary (caller-save)
f1     $ft1       temporary (caller-save)
f2     $ft2       temporary (caller-save)
f3     $ft3       temporary (caller-save)
f4     $ft4       temporary (caller-save)
f5     $ft5       temporary (caller-save)
f6     $ft6       temporary (caller-save)
f7     $ft7       temporary (caller-save)
f8     $fs0       saved register (callee-save)
f9     $fs1       saved register (callee-save)
f10    $fa0       argument/return value (caller-save)
f11    $fa1       argument/return value (caller-save)
f12    $fa2       argument (caller-save)
f13    $fa3       argument (caller-save)
f14    $fa4       argument (caller-save)
f15    $fa5       argument (caller-save)
f16    $fa6       argument (caller-save)
f17    $fa7       argument (caller-save)
f18    $fs2       saved register (callee-save)
f19    $fs3       saved register (callee-save)
f20    $fs4       saved register (callee-save)
f21    $fs5       saved register (callee-save)
f22    $fs6       saved register (callee-save)
f23    $fs7       saved register (callee-save)
f24    $fs8       saved register (callee-save)
f25    $fs9       saved register (callee-save)
f26    $fs10      saved register (callee-save)
f27    $fs11      saved register (callee-save)
f28    $ft8       temporary (caller-save)
f29    $ft9       temporary (caller-save)
f30    $ft10      temporary (caller-save)
f31    $ft11      temporary (caller-save)
******************************************************************************/

/* Import ********************************************************************/
    /* The real task switch handling function */
    .extern             _RMP_Run_High
    /* The stack address of current thread */
    .extern             RMP_SP_Cur
/* End Import ****************************************************************/

/* Export ********************************************************************/
    /* Disable all interrupts */
    .global             RMP_Int_Disable
    /* Enable all interrupts */
    .global             RMP_Int_Enable
    /* Start the first thread */
    .global             _RMP_Start
    /* Get MCYCLE value */
    .global             RMP_RV32P_MCYCLE_Get
    /* Get MCAUSE value */
    .global             RMP_RV32P_MCAUSE_Get
    /* Set MTVEC value */
    .global             _RMP_RV32P_MTVEC_Set
    /* Fast yield */
    .global             _RMP_RV32P_Yield_NONE
    .global             _RMP_RV32P_Yield_RVF
    .global             _RMP_RV32P_Yield_RVD
/* End Export ****************************************************************/

/* Header ********************************************************************/
    .section            ".text.arch"
    .align              3
/* End Header ****************************************************************/

/* Function:RMP_Int_Disable ***************************************************
Description    : The function for disabling all interrupts. Does not allow nesting.
Input          : None.
Output         : None.
Register Usage : None.
******************************************************************************/
RMP_Int_Disable:
    /* Disable all interrupts */
    CSRCI                mstatus,8
    RET
/* End Function:RMP_Int_Disable **********************************************/

/* Function:RMP_Int_Enable ****************************************************
Description    : The function for enabling all interrupts. Does not allow nesting.
Input          : None.
Output         : None.
Register Usage : None.
******************************************************************************/
RMP_Int_Enable:
    /* Enable all interrupts */
    CSRSI                mstatus,8
    RET
/* End Function:RMP_Int_Enable ***********************************************/

/* Function:_RMP_Start ********************************************************
Description : Jump to the user function and will never return from it.
Input       : $a0 - The address to branch to.
              $a1 - The stack to use.
Output      : None.
Return      : None.
******************************************************************************/
_RMP_Start:
    ADD                 sp,x0,a1
    JR                  a0
    RET
/* End Function:_RMP_Start ***************************************************/

/* Function:RMP_RV32P_MCYCLE_Get **********************************************
Description : Set the mcycle register content, could be useful for debugging.
Input       : None.
Output      : None.
Return      : $a0 - MCYCLE value.
******************************************************************************/
RMP_RV32P_MCYCLE_Get:
    CSRR                a0,mcycle
    RET
/* End Function:RMP_RV32P_MCYCLE_Get *****************************************/

/* Function:RMP_RV32P_MCAUSE_Get **********************************************
Description : Set the mcause register content, could be useful for debugging.
Input       : None.
Output      : None.
Return      : $a0 - MCYCLE value.
******************************************************************************/
RMP_RV32P_MCAUSE_Get:
    CSRR                a0,mcause
    RET
/* End Function:RMP_RV32P_MCAUSE_Get *****************************************/

/* Function:_RMP_RV32P_MTVEC_Set **********************************************
Description : Set the mtvec register content.
Input       : None.
Output      : None.
Return      : $a0 - MCYCLE value.
******************************************************************************/
_RMP_RV32P_MTVEC_Set:
    CSRW                mtvec,a0
    RET
/* End Function:_RMP_RV32P_MTVEC_Set *****************************************/

/* Function:_RMP_RV32P_Yield **************************************************
Description : Yield from one thread to another without an interrupt.
              This function has 3 versions:
              1. no coprocessor;
              2. single-precision FPU;
              3. double-precision FPU coexisting with the single-precision FPU.
              In cases 2. and 3., the "Clean" state cannot be used for speedup
              because we put the FPU context on stack. The "Clean" state assumes
              a persistent context storage across context switches which is
              usually a separate struct rather than the thread stack, but RMP
              doesn't fall into this category (RME does). Of course, it is the
              user's freedom to rewrite this routine and enable RMP_Ctx_Save and
              RMP_Ctx_Load to save/restore FPU to/from standalone structs, but
              this is out of the scope of a standard RMP port.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
/* Save all GP regs **********************************************************/
    .macro              RMP_RV32P_SAVE LABEL
    /* RISC-V does not support interrupt nesting, as the current specification says.
     * Its interrupt controller does not accept new ones before the old one gets
     * done; and to make things worse, unlike MIPS, it doesn't have IPL field,
     * thus the interrupt nesting must be implemented with different machine modes
     * (harts). This is a restriction that likely can't be overcome with software.
     * From a programming perspective, this is not too bad; it simplifies the
     * mental model. */
    ADDI                sp,sp,-31*4         /* Save registers */
    SW                  x31,30*4(sp)
    SW                  x30,29*4(sp)
    SW                  x29,28*4(sp)
    SW                  x28,27*4(sp)
    SW                  x27,26*4(sp)
    SW                  x26,25*4(sp)
    SW                  x25,24*4(sp)
    SW                  x24,23*4(sp)
    SW                  x23,22*4(sp)
    SW                  x22,21*4(sp)
    SW                  x21,20*4(sp)
    SW                  x20,19*4(sp)
    SW                  x19,18*4(sp)
    SW                  x18,17*4(sp)
    SW                  x17,16*4(sp)
    SW                  x16,15*4(sp)
    SW                  x15,14*4(sp)
    SW                  x14,13*4(sp)
    SW                  x13,12*4(sp)
    SW                  x12,11*4(sp)
    SW                  x11,10*4(sp)
    SW                  x10,9*4(sp)
    SW                  x9,8*4(sp)
    SW                  x8,7*4(sp)
    SW                  x7,6*4(sp)
    SW                  x6,5*4(sp)
    SW                  x5,4*4(sp)
    SW                  x4,3*4(sp)
    SW                  x3,2*4(sp)
    SW                  x1,1*4(sp)
    LA                  a0,\LABEL           /* Save pc - use exit address */
    SW                  a0,0*4(sp)
    CSRR                a0,mstatus          /* Read mstatus to decide FPU status, but don't save yet */
    .endm

    /* Actual context switch */
    .macro              RMP_RV32P_SWITCH
    ADDI                sp,sp,-4            /* Save mstatus */
    SW                  a0,0*4(sp)
    .option             push
    .option             norelax
    LA                  gp,_RMP_Global      /* Load gp for kernel - defined by linker script */
    .option             pop
    LA                  a0,RMP_SP_Cur       /* Save the sp to control block */
    SW                  sp,(a0)
    LA                  sp,_RMP_Stack       /* Load sp for kernel - defined by linker script */
    CALL                _RMP_Run_High       /* Get the highest ready task */
    LA                  a0,RMP_SP_Cur       /* Load the sp from control block */
    LW                  sp,(a0)
    LW                  a0,0*4(sp)          /* Read mstatus to decide FPU status, but don't load yet */
    ADDI                sp,sp,4
    .endm

/* Restore all GP regs and simulate a MRET ***********************************/
    .macro              RMP_RV32P_RESTORE
    LI                  a1,0x1880           /* Load mstatus - force M mode with enabled interrupt */
    OR                  a0,a0,a1
    CSRW                mstatus,a0
    LW                  a0,0*4(sp)          /* Load pc */
    CSRW                mepc,a0
    LW                  x1,1*4(sp)          /* Load registers */
    LW                  x3,2*4(sp)
    LW                  x4,3*4(sp)
    LW                  x5,4*4(sp)
    LW                  x6,5*4(sp)
    LW                  x7,6*4(sp)
    LW                  x8,7*4(sp)
    LW                  x9,8*4(sp)
    LW                  x10,9*4(sp)
    LW                  x11,10*4(sp)
    LW                  x12,11*4(sp)
    LW                  x13,12*4(sp)
    LW                  x14,13*4(sp)
    LW                  x15,14*4(sp)
    LW                  x16,15*4(sp)
    LW                  x17,16*4(sp)
    LW                  x18,17*4(sp)
    LW                  x19,18*4(sp)
    LW                  x20,19*4(sp)
    LW                  x21,20*4(sp)
    LW                  x22,21*4(sp)
    LW                  x23,22*4(sp)
    LW                  x24,23*4(sp)
    LW                  x25,24*4(sp)
    LW                  x26,25*4(sp)
    LW                  x27,26*4(sp)
    LW                  x28,27*4(sp)
    LW                  x29,28*4(sp)
    LW                  x30,29*4(sp)
    LW                  x31,30*4(sp)
    ADDI                sp,sp,31*4
    MRET
    .endm

/* No coprocessor ************************************************************/
    .section            .text._rmp_rv32p_yield_none
    .align              3

_RMP_RV32P_Yield_NONE:
    CSRCI               mstatus,8           /* Disable interrupt and save registers */
    RMP_RV32P_SAVE      _RMP_RV32P_Yield_NONE_Exit
    RMP_RV32P_SWITCH                        /* Do context switch */
    RMP_RV32P_RESTORE                       /* Enable interrupt and restore registers */
_RMP_RV32P_Yield_NONE_Exit:
    RET

/* Single-precision FPU coprocessor ******************************************/
    .section            .text._rmp_rv32p_yield_rvf
    .align              3

_RMP_RV32P_Yield_RVF:
    CSRCI               mstatus,8           /* Disable interrupt and save registers */
    RMP_RV32P_SAVE      _RMP_RV32P_Yield_RVF_Exit
    LUI                 a1,4                /* See if FPU is used (mstatus.fs[1]==1) */
    AND                 a1,a1,a0
    BEQZ                a1,_RMP_RV32P_Yield_RVF_Save_Skip
    ADDI                sp,sp,-33*4         /* FPU active, saving context - .hword for compatibility */
    .hword              0x25F3              /* FRCSR   a1 */
    .hword              0x0030
    SW                  a1,32*4(sp)
    .hword              0xFEFE              /* FSW     f31, 31*4(sp) */
    .hword              0xFCFA              /* FSW     f30, 30*4(sp) */
    .hword              0xFAF6              /* FSW     f29, 29*4(sp) */
    .hword              0xF8F2              /* FSW     f28, 28*4(sp) */
    .hword              0xF6EE              /* FSW     f27, 27*4(sp) */
    .hword              0xF4EA              /* FSW     f26, 26*4(sp) */
    .hword              0xF2E6              /* FSW     f25, 25*4(sp) */
    .hword              0xF0E2              /* FSW     f24, 24*4(sp) */
    .hword              0xEEDE              /* FSW     f23, 23*4(sp) */
    .hword              0xECDA              /* FSW     f22, 22*4(sp) */
    .hword              0xEAD6              /* FSW     f21, 21*4(sp) */
    .hword              0xE8D2              /* FSW     f20, 20*4(sp) */
    .hword              0xE6CE              /* FSW     f19, 19*4(sp) */
    .hword              0xE4CA              /* FSW     f18, 18*4(sp) */
    .hword              0xE2C6              /* FSW     f17, 17*4(sp) */
    .hword              0xE0C2              /* FSW     f16, 16*4(sp) */
    .hword              0xFE3E              /* FSW     f15, 15*4(sp) */
    .hword              0xFC3A              /* FSW     f14, 14*4(sp) */
    .hword              0xFA36              /* FSW     f13, 13*4(sp) */
    .hword              0xF832              /* FSW     f12, 12*4(sp) */
    .hword              0xF62E              /* FSW     f11, 11*4(sp) */
    .hword              0xF42A              /* FSW     f10, 10*4(sp) */
    .hword              0xF226              /* FSW     f9, 9*4(sp) */
    .hword              0xF022              /* FSW     f8, 8*4(sp) */
    .hword              0xEE1E              /* FSW     f7, 7*4(sp) */
    .hword              0xEC1A              /* FSW     f6, 6*4(sp) */
    .hword              0xEA16              /* FSW     f5, 5*4(sp) */
    .hword              0xE812              /* FSW     f4, 4*4(sp) */
    .hword              0xE60E              /* FSW     f3, 3*4(sp) */
    .hword              0xE40A              /* FSW     f2, 2*4(sp) */
    .hword              0xE206              /* FSW     f1, 1*4(sp) */
    .hword              0xE002              /* FSW     f0, 0*4(sp) */
_RMP_RV32P_Yield_RVF_Save_Skip:
    RMP_RV32P_SWITCH                        /* Do context switch */
    LUI                 a1,4                /* See if FPU is used (mstatus.fs[1]==1) */
    AND                 a1,a1,a0            /* FPU active, saving context - .hword for compatibility */
    BEQZ                a1,_RMP_RV32P_Yield_RVF_Restore_Skip
    .hword              0x6002              /* FLW     f0, 0*4(sp) */
    .hword              0x6092              /* FLW     f1, 1*4(sp) */
    .hword              0x6122              /* FLW     f2, 2*4(sp) */
    .hword              0x61B2              /* FLW     f3, 3*4(sp) */
    .hword              0x6242              /* FLW     f4, 4*4(sp) */
    .hword              0x62D2              /* FLW     f5, 5*4(sp) */
    .hword              0x6362              /* FLW     f6, 6*4(sp) */
    .hword              0x63F2              /* FLW     f7, 7*4(sp) */
    .hword              0x7402              /* FLW     f8, 8*4(sp) */
    .hword              0x7492              /* FLW     f9, 9*4(sp) */
    .hword              0x7522              /* FLW     f10, 10*4(sp) */
    .hword              0x75B2              /* FLW     f11, 11*4(sp) */
    .hword              0x7642              /* FLW     f12, 12*4(sp) */
    .hword              0x76D2              /* FLW     f13, 13*4(sp) */
    .hword              0x7762              /* FLW     f14, 14*4(sp) */
    .hword              0x77F2              /* FLW     f15, 15*4(sp) */
    .hword              0x6806              /* FLW     f16, 16*4(sp) */
    .hword              0x6896              /* FLW     f17, 17*4(sp) */
    .hword              0x6926              /* FLW     f18, 18*4(sp) */
    .hword              0x69B6              /* FLW     f19, 19*4(sp) */
    .hword              0x6A46              /* FLW     f20, 20*4(sp) */
    .hword              0x6AD6              /* FLW     f21, 21*4(sp) */
    .hword              0x6B66              /* FLW     f22, 22*4(sp) */
    .hword              0x6BF6              /* FLW     f23, 23*4(sp) */
    .hword              0x7C06              /* FLW     f24, 24*4(sp) */
    .hword              0x7C96              /* FLW     f25, 25*4(sp) */
    .hword              0x7D26              /* FLW     f26, 26*4(sp) */
    .hword              0x7DB6              /* FLW     f27, 27*4(sp) */
    .hword              0x7E46              /* FLW     f28, 28*4(sp) */
    .hword              0x7ED6              /* FLW     f29, 29*4(sp) */
    .hword              0x7F66              /* FLW     f30, 30*4(sp) */
    .hword              0x7FF6              /* FLW     f31, 31*4(sp) */
    LW                  a1,32*4(sp)
    .hword              0x9073              /* FSCSR   a1 */
    .hword              0x0035
    ADDI                sp,sp,33*4
_RMP_RV32P_Yield_RVF_Restore_Skip:
    RMP_RV32P_RESTORE                       /* Enable interrupt and restore registers */
_RMP_RV32P_Yield_RVF_Exit:
    RET

/* Double precision FPU coprocessor ******************************************/
    .section            .text._rmp_rv32p_yield_rvd
    .align              3

_RMP_RV32P_Yield_RVD:
    CSRCI               mstatus,8           /* Disable interrupt and save registers */
    RMP_RV32P_SAVE      _RMP_RV32P_Yield_RVD_Exit
    LUI                 a1,4                /* See if FPU is used (mstatus.fs[1]==1) */
    AND                 a1,a1,a0
    BEQZ                a1,_RMP_RV32P_Yield_RVD_Save_Skip
    ADDI                sp,sp,-65*4         /* FPU active, saving context - .hword for compatibility */
    .hword              0x25F3              /* FRCSR   a1 */
    .hword              0x0030
    SW                  a1,32*8(sp)
    .hword              0x3C27              /* FSD     f31,31*8(sp) */
    .hword              0x0FF1
    .hword              0x3827              /* FSD     f30,30*8(sp) */
    .hword              0x0FE1
    .hword              0x3427              /* FSD     f29,29*8(sp) */
    .hword              0x0FD1
    .hword              0x3027              /* FSD     f28,28*8(sp) */
    .hword              0x0FC1
    .hword              0x3C27              /* FSD     f27,27*8(sp) */
    .hword              0x0DB1
    .hword              0x3827              /* FSD     f26,26*8(sp) */
    .hword              0x0DA1
    .hword              0x3427              /* FSD     f25,25*8(sp) */
    .hword              0x0D91
    .hword              0x3027              /* FSD     f24,24*8(sp) */
    .hword              0x0D81
    .hword              0x3C27              /* FSD     f23,23*8(sp) */
    .hword              0x0B71
    .hword              0x3827              /* FSD     f22,22*8(sp) */
    .hword              0x0B61
    .hword              0x3427              /* FSD     f21,21*8(sp) */
    .hword              0x0B51
    .hword              0x3027              /* FSD     f20,20*8(sp) */
    .hword              0x0B41
    .hword              0x3C27              /* FSD     f19,19*8(sp) */
    .hword              0x0931
    .hword              0x3827              /* FSD     f18,18*8(sp) */
    .hword              0x0921
    .hword              0x3427              /* FSD     f17,17*8(sp) */
    .hword              0x0911
    .hword              0x3027              /* FSD     f16,16*8(sp) */
    .hword              0x0901
    .hword              0x3C27              /* FSD     f15,15*8(sp) */
    .hword              0x06F1
    .hword              0x3827              /* FSD     f14,14*8(sp) */
    .hword              0x06E1
    .hword              0x3427              /* FSD     f13,13*8(sp) */
    .hword              0x06D1
    .hword              0x3027              /* FSD     f12,12*8(sp) */
    .hword              0x06C1
    .hword              0x3C27              /* FSD     f11,11*8(sp) */
    .hword              0x04B1
    .hword              0x3827              /* FSD     f10,10*8(sp) */
    .hword              0x04A1
    .hword              0x3427              /* FSD     f9,9*8(sp) */
    .hword              0x0491
    .hword              0x3027              /* FSD     f8,8*8(sp) */
    .hword              0x0481
    .hword              0x3C27              /* FSD     f7,7*8(sp) */
    .hword              0x0271
    .hword              0x3827              /* FSD     f6,6*8(sp) */
    .hword              0x0261
    .hword              0x3427              /* FSD     f5,5*8(sp) */
    .hword              0x0251
    .hword              0x3027              /* FSD     f4,4*8(sp) */
    .hword              0x0241
    .hword              0x3C27              /* FSD     f3,3*8(sp) */
    .hword              0x0031
    .hword              0x3827              /* FSD     f2,2*8(sp) */
    .hword              0x0021
    .hword              0x3427              /* FSD     f1,1*8(sp) */
    .hword              0x0011
    .hword              0x3027              /* FSD     f0,0*8(sp) */
    .hword              0x0001
_RMP_RV32P_Yield_RVD_Save_Skip:
    RMP_RV32P_SWITCH                        /* Do context switch */
    LUI                 a1,4                /* See if this task uses FPU */
    AND                 a1,a1,a0            /* FPU active, saving context - .hword for compatibility */
    BEQZ                a1,_RMP_RV32P_Yield_RVD_Restore_Skip
    .hword              0x3007              /* FLD     f0,0*8(sp) */
    .hword              0x0001
    .hword              0x3087              /* FLD     f1,1*8(sp) */
    .hword              0x0081
    .hword              0x3107              /* FLD     f2,2*8(sp) */
    .hword              0x0101
    .hword              0x3187              /* FLD     f3,3*8(sp) */
    .hword              0x0181
    .hword              0x3207              /* FLD     f4,4*8(sp) */
    .hword              0x0201
    .hword              0x3287              /* FLD     f5,5*8(sp) */
    .hword              0x0281
    .hword              0x3307              /* FLD     f6,6*8(sp) */
    .hword              0x0301
    .hword              0x3387              /* FLD     f7,7*8(sp) */
    .hword              0x0381
    .hword              0x3407              /* FLD     f8,8*8(sp) */
    .hword              0x0401
    .hword              0x3487              /* FLD     f9,9*8(sp) */
    .hword              0x0481
    .hword              0x3507              /* FLD     f10,10*8(sp) */
    .hword              0x0501
    .hword              0x3587              /* FLD     f11,11*8(sp) */
    .hword              0x0581
    .hword              0x3607              /* FLD     f12,12*8(sp) */
    .hword              0x0601
    .hword              0x3687              /* FLD     f13,13*8(sp) */
    .hword              0x0681
    .hword              0x3707              /* FLD     f14,14*8(sp) */
    .hword              0x0701
    .hword              0x3787              /* FLD     f15,15*8(sp) */
    .hword              0x0781
    .hword              0x3807              /* FLD     f16,16*8(sp) */
    .hword              0x0801
    .hword              0x3887              /* FLD     f17,17*8(sp) */
    .hword              0x0881
    .hword              0x3907              /* FLD     f18,18*8(sp) */
    .hword              0x0901
    .hword              0x3987              /* FLD     f19,19*8(sp) */
    .hword              0x0981
    .hword              0x3A07              /* FLD     f20,20*8(sp) */
    .hword              0x0A01
    .hword              0x3A87              /* FLD     f21,21*8(sp) */
    .hword              0x0A81
    .hword              0x3B07              /* FLD     f22,22*8(sp) */
    .hword              0x0B01
    .hword              0x3B87              /* FLD     f23,23*8(sp) */
    .hword              0x0B81
    .hword              0x3C07              /* FLD     f24,24*8(sp) */
    .hword              0x0C01
    .hword              0x3C87              /* FLD     f25,25*8(sp) */
    .hword              0x0C81
    .hword              0x3D07              /* FLD     f26,26*8(sp) */
    .hword              0x0D01
    .hword              0x3D87              /* FLD     f27,27*8(sp) */
    .hword              0x0D81
    .hword              0x3E07              /* FLD     f28,28*8(sp) */
    .hword              0x0E01
    .hword              0x3E87              /* FLD     f29,29*8(sp) */
    .hword              0x0E81
    .hword              0x3F07              /* FLD     f30,30*8(sp) */
    .hword              0x0F01
    .hword              0x3F87              /* FLD     f31,31*8(sp) */
    .hword              0x0F81
    LW                  a1,32*8(sp)
    .hword              0x9073              /* FSCSR   a1 */
    .hword              0x0035
    ADDI                sp,sp,65*4
_RMP_RV32P_Yield_RVD_Restore_Skip:
    RMP_RV32P_RESTORE                       /* Enable interrupt and restore registers */
_RMP_RV32P_Yield_RVD_Exit:
    RET
/* End Function:_RMP_RV32P_Yield *********************************************/
    .end
/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
