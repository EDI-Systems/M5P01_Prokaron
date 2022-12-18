/******************************************************************************
Filename    : platform_rv32imac_asm.s
Author      : pry
Date        : 10/04/2012
Description : The assembly part of the RMP RTOS. This is for RV32IMAC (FE310).
******************************************************************************/

/* The RISC-V RV32IMAC Structure **********************************************
R0:Hardwired register containing "0".
R1-R31:General purpose registers that can only be reached by 32-bit instructions.
PC:Program counter.
Detailed usage convention for X0-X31:
No.    Name       Explanation
X0     $zero      hard-wired zero
X1     $ra        return address (caller-save)
X2     $sp        stack pointer (callee-save)
X3     $gp        global pointer
X4     $tp          thread pointer
X5     $t0        temporary (caller-save)
X6     $t1        temporary (caller-save)
X7     $t2        temporary (caller-save)
X8     $s0/fp        saved register/frame pointer (callee-save)
X9     $s1        saved register (callee-save)
X10    $a0        argument/return value (caller-save)
X11    $a1        argument/return value (caller-save)
X12    $a2        argument (caller-save)
X13    $a3        argument (caller-save)
X14    $a4        argument (caller-save)
X15    $a5        argument (caller-save)
X16    $a6        argument (caller-save)
X17    $a7        argument (caller-save)
X18    $s2        saved register (callee-save)
X19    $s3        saved register (callee-save)
X20    $s4        saved register (callee-save)
X21    $s5        saved register (callee-save)
X22    $s6        saved register (callee-save)
X23    $s7        saved register (callee-save)
X24    $s8        saved register (callee-save)
X25    $s9        saved register (callee-save)
X26    $s10       saved register (callee-save)
X27    $s11       saved register (callee-save)
X28    $t3        temporary (caller-save)
X29    $t4        temporary (caller-save)
X30    $t5        temporary (caller-save)
X31    $t6        temporary (caller-save)
PC     $pc        program counter

On chips that have a FPU, the layout of the FPU registers are:
F0     $ft0       temporary (caller-save)
F1     $ft1       temporary (caller-save)
F2     $ft2       temporary (caller-save)
F3     $ft3       temporary (caller-save)
F4     $ft4       temporary (caller-save)
F5     $ft5       temporary (caller-save)
F6     $ft6       temporary (caller-save)
F7     $ft7       temporary (caller-save)
F8     $fs0       saved register (callee-save)
F9     $fs1       saved register (callee-save)
F10    $fa0       argument/return value (caller-save)
F11    $fa1       argument/return value (caller-save)
F12    $fa2       argument (caller-save)
F13    $fa3       argument (caller-save)
F14    $fa4       argument (caller-save)
F15    $fa5       argument (caller-save)
F16    $fa6       argument (caller-save)
F17    $fa7       argument (caller-save)
F18    $fs2       saved register (callee-save)
F19    $fs3       saved register (callee-save)
F20    $fs4       saved register (callee-save)
F21    $fs5       saved register (callee-save)
F22    $fs6       saved register (callee-save)
F23    $fs7       saved register (callee-save)
F24    $fs8       saved register (callee-save)
F25    $fs9       saved register (callee-save)
F26    $fs10      saved register (callee-save)
F27    $fs11      saved register (callee-save)
F28    $ft8       temporary (caller-save)
F29    $ft9       temporary (caller-save)
F30    $ft10      temporary (caller-save)
F31    $ft11      temporary (caller-save)
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
    .global             _RMP_Get_MCAUSE
    /* Set MTVEC value */
    .global             _RMP_Set_MTVEC
    /* Get MCYCLE value */
    .global             _RMP_Get_MCYCLE
    /* Fencing memory */
    .global             _RMP_Mem_FENCE
    /* Generic interrupt handler */
    .global             Interrupt_Handler
/* End Exports ***************************************************************/

/* Begin Imports *************************************************************/
    /* The real task switch handling function */
    .extern             _RMP_Rdy_High
    /* The real systick handler function */
    .extern             _RMP_Tick_Handler
    /* The current thread */
    .extern             RMP_Thd_Cur
    /* The stack address of current thread */
    .extern             RMP_SP_Cur
    /* Save and load extra contexts, such as FPU, peripherals and MPU */
    .extern             RMP_Ctx_Save
    .extern             RMP_Ctx_Load
    /* Interrupt handlers */
    .extern             PendSV_Handler
    .extern             SysTick_Handler
    .extern             _RMP_Int_Handler
    /* Clear timer & software interrupt flags */
    .extern             _RMP_Clear_Soft_Flag
    .extern             _RMP_Clear_Timer_Flag
/* End Imports ***************************************************************/

/* Startup & Init ************************************************************/
    .global             _Reset_Entry
_Reset_Entry:
    .option             push
    /* Ensure the instruction is not optimized, since gp is not yet set. GP
     * will be supplied by the linker to provide the position of global variables */
    .option             norelax
    /* Load global pointer */
    LA                  gp,__global_pointer$
    .option             pop
    /* Load stack pointer */
    LA                  sp,__stack
    /* Go into C to continue our initialization of .data and .bss */
    J                   _start
/* End Startup & Init ********************************************************/

/* Begin Function:RMP_Int_Disable *********************************************
Description    : The function for disabling all interrupts. Does not allow nesting.
Input          : None.
Output         : None.
Register Usage : None.
******************************************************************************/
RMP_Int_Disable:
    /* Disable all interrupts */
    CSRRCI              a0,mstatus,8
    CSRWI               mie,0
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
    CSRRSI              a0,mstatus,8
    LI                  a0,0x888
    CSRW                mie,a0
    RET
/* End Function:RMP_Int_Enable ***********************************************/

/* Begin Function:RMP_MSB_Get *************************************************
Description    : Get the MSB of the word. RV32IMAC does not support CLZ, so we will
                 have to do it the hard way.
Input          : ptr_t Val - The value.
Output         : None.
Return         : ptr_t - The MSB position.
Register Usage : None.
******************************************************************************/
.macro CHECK_BITS BITS LABEL
    SRL                 a2,a1,\BITS
    BEQ                 a2,x0,\LABEL
    ADDI                a0,a0,\BITS
    ADD                 a1,a2,x0
\LABEL:
.endm

    /* Always 21 instructions no matter what */
RMP_MSB_Get:
    BEQ                 a0,x0,ZERO
    ADD                 a1,a0,x0
    LI                  a0,0
    CHECK_BITS          BITS=16 LABEL=HEX
    CHECK_BITS          BITS=8  LABEL=OCT
    CHECK_BITS          BITS=4  LABEL=QUAD
    CHECK_BITS          BITS=2  LABEL=BIN
    CHECK_BITS          BITS=1  LABEL=ONE
    RET
ZERO:
    ADDI                a0,a0,-1
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
    ADD                 sp,x0,a1
    JR                  a0
    RET
/* End Function:_RMP_Start ***************************************************/

/* Begin Function:_RMP_Get_MCAUSE *********************************************
Description : Get the mcause register content.
Input       : None.
Output      : None.
Return      : $a0 - MCAUSE value.
******************************************************************************/
_RMP_Get_MCAUSE:
    CSRR                a0,mcause
    RET
/* End Function:_RMP_Get_MCAUSE **********************************************/

/* Begin Function:_RMP_Set_MTVEC **********************************************
Description : Set the mtvec register content.
Input       : $a0 - MTVEC value.
Output      : None.
Return      : None.
******************************************************************************/
_RMP_Set_MTVEC:
    CSRW                mtvec,a0
    RET
/* End Function:_RMP_Set_MTVEC ***********************************************/

/* Begin Function:_RMP_Mem_FENCE **********************************************
Description : Fence the memory accesses to make sure that they are strongly ordered.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
_RMP_Mem_FENCE:
    FENCE
    RET
/* End Function:_RMP_Mem_FENCE ***********************************************/

/* Begin Function:_RMP_Get_MCYCLE *********************************************
Description : Set the mtvec register content.
Input       : None.
Output      : None.
Return      : $a0 - MCYCLE value.
******************************************************************************/
_RMP_Get_MCYCLE:
    CSRR                a0,mcycle
    RET
/* End Function:_RMP_Get_MCYCLE **********************************************/

/* Begin Function:Interrupt_Handler *******************************************
Description : The interrupt handling routine. This is the entry of all interrupt
              handlers; we will analyze mcause register to jump to the corresponding
              vector. This must be aligned to at least a word boundary.
Input       : None.
Output      : None.
******************************************************************************/
    .align              2
Interrupt_Handler:
    /* RISC-V does not support interrupt nesting, as the current specification says.
     * Its interrupt controller does not accept new ones before the old one gets
     * done; and to make things worse, unlike MIPS, it doesn't have IPL field,
     * thus the interrupt nesting must be implemented with different machine modes
     * (harts). This is a restriction that likely can't be overcome with software.
     * From a programming perspective, this is not too bad; it simplifies the
     * mental model. */
    ADDI                sp,sp,-32*4
    /* Save three registers so that we can reenable interrupt as quickly as possible */
    SW                  x31,31*4(sp)
    SW                  x30,30*4(sp)
    SW                  x29,29*4(sp)
    SW                  x28,28*4(sp)
    SW                  x27,27*4(sp)
    SW                  x26,26*4(sp)
    SW                  x25,25*4(sp)
    SW                  x24,24*4(sp)
    SW                  x23,23*4(sp)
    SW                  x22,22*4(sp)
    SW                  x21,21*4(sp)
    SW                  x20,20*4(sp)
    SW                  x19,19*4(sp)
    SW                  x18,18*4(sp)
    SW                  x17,17*4(sp)
    SW                  x16,16*4(sp)
    SW                  x15,15*4(sp)
    SW                  x14,14*4(sp)
    SW                  x13,13*4(sp)
    SW                  x12,12*4(sp)
    SW                  x11,11*4(sp)
    SW                  x10,10*4(sp)
    SW                  x9,9*4(sp)
    SW                  x8,8*4(sp)
    SW                  x7,7*4(sp)
    SW                  x6,6*4(sp)
    SW                  x5,5*4(sp)
    SW                  x4,4*4(sp)
    SW                  x3,3*4(sp)
    /* Save MSTATUS here in case future implementations include FPU */
    CSRR                a0,mstatus
    SW                  a0,2*4(sp)
    /* x2 not saved */
    SW                  x1,1*4(sp)
    /* Save PC before the exception */
    CSRR                a0,mepc
    SW                  a0,0*4(sp)
    /* x0 is always zero thus not saved */

    /* Save the SP to control block */
    LA                  a0,RMP_SP_Cur
    SW                  sp,(a0)
    /* Now load our own gp for system use - we know that this is in linker script */
    LA                  gp,__global_pointer$
    /* And load our SP on the kernel stack */
    LA                  sp,__stack
    CALL                _RMP_Int_Handler
    /* Call the actual handler function to handle this */
    /* Load the SP from control block */
    LA                  a0,RMP_SP_Cur
    LW                  sp,(a0)

    /* Load PC */
    LW                  a0,0*4(sp)
    CSRW                mepc,a0
    /* X0 is always zero thus not loaded */
    LW                  x1,1*4(sp)
    /* Load MSTATUS - we force M mode after interrupt return */
    LW                  a0,2*4(sp)
    LI                  a1,0x1800
    OR                  a0,a0,a1
    CSRW                mstatus,a0
    /* X2 is SP thus not loaded again */
    LW                  x3,3*4(sp)
    LW                  x4,4*4(sp)
    LW                  x5,5*4(sp)
    LW                  x6,6*4(sp)
    LW                  x7,7*4(sp)
    LW                  x8,8*4(sp)
    LW                  x9,9*4(sp)
    LW                  x10,10*4(sp)
    LW                  x11,11*4(sp)
    LW                  x12,12*4(sp)
    LW                  x13,13*4(sp)
    LW                  x14,14*4(sp)
    LW                  x15,15*4(sp)
    LW                  x16,16*4(sp)
    LW                  x17,17*4(sp)
    LW                  x18,18*4(sp)
    LW                  x19,19*4(sp)
    LW                  x20,20*4(sp)
    LW                  x21,21*4(sp)
    LW                  x22,22*4(sp)
    LW                  x23,23*4(sp)
    LW                  x24,24*4(sp)
    LW                  x25,25*4(sp)
    LW                  x26,26*4(sp)
    LW                  x27,27*4(sp)
    LW                  x28,28*4(sp)
    LW                  x29,29*4(sp)
    LW                  x30,30*4(sp)
    LW                  x31,31*4(sp)
    ADDI                sp,sp,32*4
    MRET
/* End Function:Interrupt_Handler ********************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
