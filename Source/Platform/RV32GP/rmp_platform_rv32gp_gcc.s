/******************************************************************************
Filename    : rmp_platform_rv32gp_gcc.s
Author      : pry
Date        : 10/04/2012
Description : The assembly part of the RMP RTOS. This is for RV32G with
              physical memory access.
              As an open source processor IP, RISC-V have many configurations
              that are implementation-specific. To maximize the applicability
              of this port, certain modifications to the vendor-supplied
              firmware might be required:
              1. The linker script must export the __global_pointer$, which is
                 useful for global variable relative indexing.
              2. The linker script must export the __initial_stack$, which is
                 useful for kernel execution in a context switch.
              3. The startup assembly file takes care of global variable and
                 chip clock initialization.
              To avoid changing these files directly, it is recommended to
              supply extra files in place of, or in addition to certain
              vendor-supplied files.
              Vector table handling:
              1. If the chip feature a vectored interrupt controller, make sure
                 all the OS-aware interrupt vectors are handled exclusively
                 with _RMP_RV32G_Handler. The other vectors may leverage any
                 tailored code.
              2. If the chip does not feature a vectored interrupt controller,
                 all interrupt vectors must be serviced with _RMP_RV32G_Handler.
              3. Need to define mcause values for SysTick and PendSV in the
                 chip configuration header so that the kernel know how to find
                 them.
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

/* Begin Header **************************************************************/
    .section            .reset_entry,"ax", @progbits
    .align              2
/* End Header ****************************************************************/

/* Begin Exports *************************************************************/
    /* Disable all interrupts */
    .global             RMP_Int_Disable
    /* Enable all interrupts */
    .global             RMP_Int_Enable
    /* Get the MSB */
    .global             RMP_MSB_Get
    /* Start the first thread */
    .global             _RMP_Start
    /* Get MCAUSE value */
    .global             __RMP_RV32GP_MCAUSE_Get
    /* Set MTVEC value */
    .global             __RMP_RV32GP_MTVEC_Set
    /* Get MCYCLE value */
    .global             __RMP_RV32GP_MCYCLE_Get
    /* Fencing memory */
    .global             __RMP_RV32GP_Mem_FENCE
    /* Generic interrupt handler */
    .global             __RMP_RV32GP_Handler
/* End Exports ***************************************************************/

/* Begin Imports *************************************************************/
    /* The real task switch handling function */
    .extern             _RMP_Run_High
    /* The real systick handler function */
    .extern             _RMP_Tim_Handler
    /* The current thread */
    .extern             RMP_Thd_Cur
    /* The stack address of current thread */
    .extern             RMP_SP_Cur
    /* Save and load extra contexts, such as FPU, peripherals and MPU */
    .extern             RMP_Ctx_Save
    .extern             RMP_Ctx_Load
    /* Interrupt handler */
    .extern             _RMP_RV32GP_Handler
    /* Clear timer & software interrupt flags */
    .extern             _RMP_Clear_Soft_Flag
    .extern             _RMP_Clear_Timer_Flag
/* End Imports ***************************************************************/

/* Begin Function:RMP_Int_Disable *********************************************
Description    : The function for disabling all interrupts. Does not allow nesting.
Input          : None.
Output         : None.
Register Usage : None.
******************************************************************************/
RMP_Int_Disable:
    /* Disable all interrupts */
    CSRCI                mstatus, 8
    RET
/* End Function:RMP_Int_Disable **********************************************/

/* Begin Function:RMP_Int_Enable **********************************************
Description    : The function for enabling all interrupts. Does not allow nesting.
Input          : None.
Output         : None.
Register Usage : None.
******************************************************************************/
RMP_Int_Enable:
    /* Enable all interrupts */
    CSRSI                mstatus, 8
    RET
/* End Function:RMP_Int_Enable ***********************************************/

/* Begin Function:RMP_MSB_Get *************************************************
Description    : Get the MSB of the word. RV32G doesn't always support CLZ which
                 is an RVB bit extension, so we will have to do it the hard way.
Input          : ptr_t Val - The value.
Output         : None.
Return         : ptr_t - The MSB position.
Register Usage : None.
******************************************************************************/
.macro CHECK_BITS BITS LABEL
    SRL                 a2, a1, \BITS
    BEQ                 a2, x0, \LABEL
    ADDI                a0, a0, \BITS
    ADD                 a1, a2, x0
\LABEL:
.endm

    /* Always 21 instructions no matter what */
RMP_MSB_Get:
    BEQ                 a0, x0, ZERO
    ADD                 a1, a0, x0
    LI                  a0, 0
    CHECK_BITS          BITS=16 LABEL=HEX
    CHECK_BITS          BITS=8  LABEL=OCT
    CHECK_BITS          BITS=4  LABEL=QUAD
    CHECK_BITS          BITS=2  LABEL=BIN
    CHECK_BITS          BITS=1  LABEL=ONE
    RET
ZERO:
    ADDI                a0, a0, -1
    RET
/* End Function:RMP_MSB_Get **************************************************/

/* Begin Function:_RMP_Start **************************************************
Description : Jump to the user function and will never return from it.
Input       : $a0 - The address to branch to.
              $a1 - The stack to use.
Output      : None.
Return      : None.
******************************************************************************/
_RMP_Start:
    ADD                 sp, x0, a1
    JR                  a0
    RET
/* End Function:_RMP_Start ***************************************************/

/* Begin Function:__RMP_RV32GP_MCAUSE_Get *************************************
Description : Get the mcause register content.
Input       : None.
Output      : None.
Return      : $a0 - MCAUSE value.
******************************************************************************/
__RMP_RV32GP_MCAUSE_Get:
    CSRR                a0, mcause
    RET
/* End Function:__RMP_RV32GP_MCAUSE_Get **************************************/

/* Begin Function:__RMP_RV32GP_MTVEC_Set **************************************
Description : Set the mtvec register content.
Input       : $a0 - MTVEC value.
Output      : None.
Return      : None.
******************************************************************************/
__RMP_RV32GP_MTVEC_Set:
    CSRW                mtvec, a0
    RET
/* End Function:__RMP_RV32GP_MTVEC_Set ***************************************/

/* Begin Function:__RMP_RV32GP_MCYCLE_Get *************************************
Description : Set the mtvec register content.
Input       : None.
Output      : None.
Return      : $a0 - MCYCLE value.
******************************************************************************/
__RMP_RV32GP_MCYCLE_Get:
    CSRR                a0, mcycle
    RET
/* End Function:__RMP_RV32GP_MCYCLE_Get **************************************/

/* Begin Function:__RMP_RV32GP_Mem_FENCE **************************************
Description : Fence the memory accesses to make sure that they are strongly ordered.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
__RMP_RV32GP_Mem_FENCE:
    FENCE
    RET
/* End Function:__RMP_RV32GP_Mem_FENCE ***************************************/

/* Begin Function:__RMP_RV32GP_Handler ****************************************
Description : The generic interrupt handling routine. This is the entry of all
              interrupt handlers (that are managed by the kernel); we will
              analyze mcause register to jump to the corresponding vector.
              This must be aligned to at least a word boundary.
Input       : None.
Output      : None.
******************************************************************************/
    .align              2
__RMP_RV32GP_Handler:
    /* RISC-V does not support interrupt nesting, as the current specification says.
     * Its interrupt controller does not accept new ones before the old one gets
     * done; and to make things worse, unlike MIPS, it doesn't have IPL field,
     * thus the interrupt nesting must be implemented with different machine modes
     * (harts). This is a restriction that likely can't be overcome with software.
     * From a programming perspective, this is not too bad; it simplifies the
     * mental model. */
    ADDI                sp, sp, -32*4
    /* Save three registers so that we can reenable interrupt as quickly as possible */
    SW                  x31, 31*4(sp)
    SW                  x30, 30*4(sp)
    SW                  x29, 29*4(sp)
    SW                  x28, 28*4(sp)
    SW                  x27, 27*4(sp)
    SW                  x26, 26*4(sp)
    SW                  x25, 25*4(sp)
    SW                  x24, 24*4(sp)
    SW                  x23, 23*4(sp)
    SW                  x22, 22*4(sp)
    SW                  x21, 21*4(sp)
    SW                  x20, 20*4(sp)
    SW                  x19, 19*4(sp)
    SW                  x18, 18*4(sp)
    SW                  x17, 17*4(sp)
    SW                  x16, 16*4(sp)
    SW                  x15, 15*4(sp)
    SW                  x14, 14*4(sp)
    SW                  x13, 13*4(sp)
    SW                  x12, 12*4(sp)
    SW                  x11, 11*4(sp)
    SW                  x10, 10*4(sp)
    SW                  x9, 9*4(sp)
    SW                  x8, 8*4(sp)
    SW                  x7, 7*4(sp)
    SW                  x6, 6*4(sp)
    SW                  x5, 5*4(sp)
    SW                  x4, 4*4(sp)
    SW                  x3, 3*4(sp)

    /* See if we have used FPU (mstatus.fs[1]==1) */
    CSRR                a0, mstatus
    LUI					x3, 4
    AND                 x3, x3, a0
    BEQZ                x3, SAVESKIP
	/* FPU active, saving context - for compatibility with non-FPU cases */
    ADDI                sp, sp, -33*4
    .hword              0x21F3      /* FRCSR   x3 */
    .hword              0x0030
    SW                  x3, 32*4(sp)
    .hword              0xFEFE      /* FSW     f31, 31*4(sp) */
    .hword              0xFCFA      /* FSW     f30, 30*4(sp) */
    .hword              0xFAF6      /* FSW     f29, 29*4(sp) */
    .hword              0xF8F2      /* FSW     f28, 28*4(sp) */
    .hword              0xF6EE      /* FSW     f27, 27*4(sp) */
    .hword              0xF4EA      /* FSW     f26, 26*4(sp) */
    .hword              0xF2E6      /* FSW     f25, 25*4(sp) */
    .hword              0xF0E2      /* FSW     f24, 24*4(sp) */
    .hword              0xEEDE      /* FSW     f23, 23*4(sp) */
    .hword              0xECDA      /* FSW     f22, 22*4(sp) */
    .hword              0xEAD6      /* FSW     f21, 21*4(sp) */
    .hword              0xE8D2      /* FSW     f20, 20*4(sp) */
    .hword              0xE6CE      /* FSW     f19, 19*4(sp) */
    .hword              0xE4CA      /* FSW     f18, 18*4(sp) */
    .hword              0xE2C6      /* FSW     f17, 17*4(sp) */
    .hword              0xE0C2      /* FSW     f16, 16*4(sp) */
    .hword              0xFE3E      /* FSW     f15, 15*4(sp) */
    .hword              0xFC3A      /* FSW     f14, 14*4(sp) */
    .hword              0xFA36      /* FSW     f13, 13*4(sp) */
    .hword              0xF832      /* FSW     f12, 12*4(sp) */
    .hword              0xF62E      /* FSW     f11, 11*4(sp) */
    .hword              0xF42A      /* FSW     f10, 10*4(sp) */
    .hword              0xF226      /* FSW     f9, 9*4(sp) */
    .hword              0xF022      /* FSW     f8, 8*4(sp) */
    .hword              0xEE1E      /* FSW     f7, 7*4(sp) */
    .hword              0xEC1A      /* FSW     f6, 6*4(sp) */
    .hword              0xEA16      /* FSW     f5, 5*4(sp) */
    .hword              0xE812      /* FSW     f4, 4*4(sp) */
    .hword              0xE60E      /* FSW     f3, 3*4(sp) */
    .hword              0xE40A      /* FSW     f2, 2*4(sp) */
    .hword              0xE206      /* FSW     f1, 1*4(sp) */
    .hword              0xE002      /* FSW     f0, 0*4(sp) */

SAVESKIP:
    /* Save MSTATUS */
    SW                  a0, 2*4(sp)
    /* x2 not saved on stack */
    SW                  x1, 1*4(sp)
    /* Save PC before the exception */
    CSRR                a0, mepc
    SW                  a0, 0*4(sp)
    /* x0 is always zero thus not saved */

    /* Load gp for kernel - defined by linker script */
    .option             push
    .option             norelax
    LA                  gp, __global_pointer$
    .option             pop
    /* Save the sp to control block */
    LA                  a0, RMP_SP_Cur
    SW                  sp, (a0)
    /* Load sp for kernel - defined by linker script */
    LA                  sp, __initial_stack$
    /* Call system main interrupt handler */
    CALL                _RMP_RV32GP_Handler
    /* Load the sp from control block */
    LA                  a0, RMP_SP_Cur
    LW                  sp, (a0)

    /* Load pc */
    LW                  a0, 0*4(sp)
    CSRW                mepc, a0
    /* x0 is always zero thus not restored */
    LW                  x1, 1*4(sp)
    /* Load MSTATUS - we force M mode after interrupt return */
    LW                  a0, 2*4(sp)
    LI                  a1, 0x1800
    OR                  a0, a0, a1
    CSRW                mstatus, a0

    /* See if this task uses FPU */
    LUI					x3, 4
    AND                 x3, x3, a0
    BEQZ                x3, RESTORESKIP
	/* FPU active, saving context - for compatibility with non-FPU cases */
    .hword              0x6002      /* FLW     f0, 0*4(sp) */
    .hword              0x6092      /* FLW     f1, 1*4(sp) */
    .hword              0x6122      /* FLW     f2, 2*4(sp) */
    .hword              0x61B2      /* FLW     f3, 3*4(sp) */
    .hword              0x6242      /* FLW     f4, 4*4(sp) */
    .hword              0x62D2      /* FLW     f5, 5*4(sp) */
    .hword              0x6362      /* FLW     f6, 6*4(sp) */
    .hword              0x63F2      /* FLW     f7, 7*4(sp) */
    .hword              0x7402      /* FLW     f8, 8*4(sp) */
    .hword              0x7492      /* FLW     f9, 9*4(sp) */
    .hword              0x7522      /* FLW     f10, 10*4(sp) */
    .hword              0x75B2      /* FLW     f11, 11*4(sp) */
    .hword              0x7642      /* FLW     f12, 12*4(sp) */
    .hword              0x76D2      /* FLW     f13, 13*4(sp) */
    .hword              0x7762      /* FLW     f14, 14*4(sp) */
    .hword              0x77F2      /* FLW     f15, 15*4(sp) */
    .hword              0x6806      /* FLW     f16, 16*4(sp) */
    .hword              0x6896      /* FLW     f17, 17*4(sp) */
    .hword              0x6926      /* FLW     f18, 18*4(sp) */
    .hword              0x69B6      /* FLW     f19, 19*4(sp) */
    .hword              0x6A46      /* FLW     f20, 20*4(sp) */
    .hword              0x6AD6      /* FLW     f21, 21*4(sp) */
    .hword              0x6B66      /* FLW     f22, 22*4(sp) */
    .hword              0x6BF6      /* FLW     f23, 23*4(sp) */
    .hword              0x7C06      /* FLW     f24, 24*4(sp) */
    .hword              0x7C96      /* FLW     f25, 25*4(sp) */
    .hword              0x7D26      /* FLW     f26, 26*4(sp) */
    .hword              0x7DB6      /* FLW     f27, 27*4(sp) */
    .hword              0x7E46      /* FLW     f28, 28*4(sp) */
    .hword              0x7ED6      /* FLW     f29, 29*4(sp) */
    .hword              0x7F66      /* FLW     f30, 30*4(sp) */
    .hword              0x7FF6      /* FLW     f31, 31*4(sp) */
	LW					x3, 32*4(sp)
    .hword              0x9073      /* FSCSR   x3 */
    .hword              0x0031
	ADDI                sp, sp, 33*4

RESTORESKIP:
    /* x2(sp) not restored from stack */
    LW                  x3, 3*4(sp)
    LW                  x4, 4*4(sp)
    LW                  x5, 5*4(sp)
    LW                  x6, 6*4(sp)
    LW                  x7, 7*4(sp)
    LW                  x8, 8*4(sp)
    LW                  x9, 9*4(sp)
    LW                  x10, 10*4(sp)
    LW                  x11, 11*4(sp)
    LW                  x12, 12*4(sp)
    LW                  x13, 13*4(sp)
    LW                  x14, 14*4(sp)
    LW                  x15, 15*4(sp)
    LW                  x16, 16*4(sp)
    LW                  x17, 17*4(sp)
    LW                  x18, 18*4(sp)
    LW                  x19, 19*4(sp)
    LW                  x20, 20*4(sp)
    LW                  x21, 21*4(sp)
    LW                  x22, 22*4(sp)
    LW                  x23, 23*4(sp)
    LW                  x24, 24*4(sp)
    LW                  x25, 25*4(sp)
    LW                  x26, 26*4(sp)
    LW                  x27, 27*4(sp)
    LW                  x28, 28*4(sp)
    LW                  x29, 29*4(sp)
    LW                  x30, 30*4(sp)
    LW                  x31, 31*4(sp)
    ADDI                sp, sp, 32*4
    MRET
/* End Function:__RMP_RV32GP_Handler *****************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
