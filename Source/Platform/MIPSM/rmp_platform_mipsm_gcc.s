/******************************************************************************
Filename    : platform_mipsm_asm.s
Author      : pry
Date        : 10/04/2012
Description : The assembly part of the RMP RTOS.
******************************************************************************/
    
/* The MIPS M-Class Architecture **********************************************
R0:Hardwired register containing "0". 
R1-R31:General purpose registers that can only be reached by 32-bit instructions.
HI:Multiply/divide unit high register.
LO:Multiply/divide unit low register.
PC:Program counter.
The processor also include a single-accuracy FPU.
Detailed usage convention for R0-R31:
No.    Name       Explanation
R0     $zero      constant 0
R1     $at        reserved for assembler
R2     $v0        expression evaluation and results of a function
R3     $v1        expression evaluation and results of a function
R4     $a0        argument 1
R5     $a1        argument 2
R6     $a2        argument 3
R7     $a3        argument 4
R8     $t0        temporary (not preserved across call)
R9     $t1        temporary (not preserved across call)
R10    $t2        temporary (not preserved across call)
R11    $t3        temporary (not preserved across call)
R12    $t4        temporary (not preserved across call)
R13    $t5        temporary (not preserved across call)
R14    $t6        temporary (not preserved across call)
R15    $t7        temporary (not preserved across call)
R16    $s0        saved temporary (preserved across call)
R17    $s1        saved temporary (preserved across call)
R18    $s2        saved temporary (preserved across call)
R19    $s3        saved temporary (preserved across call)
R20    $s4        saved temporary (preserved across call)
R21    $s5        saved temporary (preserved across call)
R22    $s6        saved temporary (preserved across call)
R23    $s7        saved temporary (preserved across call)
R24    $t8        temporary (not preserved across call)
R25    $t9        temporary (not preserved across call)
R26    $k0        reserved for OS kernel
R27    $k1        reserved for OS kernel
R28    $gp        pointer to global area
R29    $sp        stack pointer
R30    $fp        frame pointer
R31    $ra        return address (used by function call)
******************************************************************************/
    
/* Begin Header **************************************************************/
    .text
    .align 4
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
    /* The context switch trigger */
    .global             _RMP_Yield
    /* The system pending service routine */
    .global             PendSV_Handler 
    /* The systick timer routine */
    .global             SysTick_Handler
    /* Set timer overflow value */
    .global             _RMP_Set_Timer
/* End Exports ***************************************************************/

/* Begin Imports *************************************************************/
    /* The real task switch handling function */
    .extern             _RMP_Rdy_High 
    /* The real systick handler function */
    .extern             _RMP_Tick_Handler
    /* The PID of the current thread */
    .extern             RMP_Thd_Cur
    /* The stack address of current thread */
    .extern             RMP_SP_Cur
    /* Save and load extra contexts, such as FPU, peripherals and MPU */
    .extern             RMP_Ctx_Save
    .extern             RMP_Ctx_Load
    /* The place where we store the kernel gp/sp value */
    .extern             RMP_GP_Val
    .extern             RMP_SP_Val
    /* The interrupt nesting value */
    .extern             RMP_Int_Nest
    /* Clear timer & software interrupt flags */
    .extern             _RMP_Clear_Soft_Flag
    .extern             _RMP_Clear_Timer_Flag
/* End Imports ***************************************************************/

/* Begin Macros **************************************************************/
    .equ                CP0_COUNT,$9
    .equ                CP0_COMPARE,$11
    .equ                CP0_STATUS,$12
    .equ                CP0_CAUSE,$13
    .equ                CP0_EPC,$14
    .equ                CTX_SIZE,136
    .equ                CORE_SW0,0x100
    /* Interrupt context saving. These interrupts should not use DSP & FPU */
.macro    SAVE_CONTEXT
    /* Make room for the context */
    ADDIU               $sp,$sp,-CTX_SIZE
    /* Save 4 regs first so that we can enable interrupts as soon as possible */
    SW                  $23,132($sp)
    SW                  $22,128($sp)
    SW                  $21,124($sp)
    SW                  $20,120($sp)
    /* Increase interrupt nesting count */
    LUI                 $23,%hi(RMP_Int_Nest)
    ORI                 $23,$23,%lo(RMP_Int_Nest)
    LW                  $22,($23)
    NOP
    ADDIU               $21,$22,1
    SW                  $21,($23)
    /* Temporarily save current stack */
    MOVE                $20,$sp
    /* Branch taken when already in system stack */
    BNE                 $22,$0,1f 
    NOP
    /* Need to save the old sp first, then switch to system stack to continue execution */
    LUI                 $23,%hi(RMP_SP_Cur)
    ORI                 $23,$23,%lo(RMP_SP_Cur)
    SW                  $sp,($23)
    /* We need to swap to system stack for execution */
    LUI                 $23,%hi(RMP_SP_Val)
    ORI                 $23,$23,%lo(RMP_SP_Val)
    LW                  $sp,($23)
    /* Save all status registers, except for CAUSE */
1:  MFC0                $23,CP0_EPC
    MFC0                $21,CP0_STATUS
    SW                  $23,116($20)
    SW                  $21,112($20)
    /* See if it is safe to reenable interrupt on register popping. If yes, we 
     * allow that by storing a new version of STATUS that does not have UM,ERL,EXL 
     * bits set. This can only happen when this is already a nested interrupt (
     * we are already on kernel stack so this is fine) */
    BEQ                 $22,$0,2f
    NOP
    INS                 $21,$0,1,7                  /* Clear UM, ERL, EXL from STATUS */
    SW                  $21,112($20)
    /* Now we reenable interrupts - only IPL[10:15] is supported; IPL[17:16] kept at 0 */
2:  INS                 $21,$0,1,15                 /* Clear IPL from STATUS */
    MFC0                $22,CP0_CAUSE
    EXT                 $23,$22,10,6                /* Extract RIPL from CAUSE */
    INS                 $21,$23,10,6                /* Set current IPL */
    MTC0                $21,CP0_STATUS              /* Write status back */
    /* Save mutiply/divide registers */
    MFHI                $23
    MFLO                $22
    SW                  $23,108($20)
    SW                  $22,104($20)
    /* Save "kernel only" registers - we do not expect the user to abide by the rule */
    SW                  $28,100($20)
    SW                  $27,96($20)
    SW                  $26,92($20)
    /* Save everything else */
    SW                  $31,88($20)
    SW                  $30,84($20)
    SW                  $25,80($20)
    SW                  $24,76($20)
    SW                  $19,72($20)
    SW                  $18,68($20)
    SW                  $17,64($20)
    SW                  $16,60($20)
    SW                  $15,56($20)
    SW                  $14,52($20)
    SW                  $13,48($20)
    SW                  $12,44($20)
    SW                  $11,40($20)
    SW                  $10,36($20)
    SW                  $9,32($20)
    SW                  $8,28($20)
    SW                  $7,24($20)
    SW                  $6,20($20)
    SW                  $5,16($20)
    SW                  $4,12($20)
    SW                  $3,8($20)
    SW                  $2,4($20)
    SW                  $1,0($20)
    /* Load kernel GP for kernel execution */
    LUI                 $gp,%hi(RMP_GP_Val)
    ORI                 $gp,$gp,%lo(RMP_GP_Val)
    LW                  $gp,($gp)
.endm
                
    /* Context restoring */
.macro LOAD_CONTEXT
    /* Protect access to c0 registers */
    DI
    EHB
    /* Decrease interrupt nesting count */
    LUI                 $23,%hi(RMP_Int_Nest)
    ORI                 $23,$23,%lo(RMP_Int_Nest)
    LW                  $22,($23)
    NOP
    ADDIU               $21,$22,-1
    SW                  $21,($23)
    /* Branch taken when going to return to user stack */
    BNE                 $21,$0,1f 
    NOP
    /* Need to load back the old sp */
    LUI                 $23,%hi(RMP_SP_Cur)
    ORI                 $23,$23,%lo(RMP_SP_Cur)
    LW                  $sp,($23)
    NOP
1:  MOVE                $20,$sp
    /* Restore all status registers, except for CAUSE */
    LW                  $21,112($20)
    LW                  $23,116($20)
    MTC0                $23,CP0_EPC
    /* Interrupts NOT enabled by this status restoration if this is the last interrupt
     * nesting level. This is because the EXL, etc. bit is present when the first
     * interrupt comes in (but not in nested interrupts), and we saved it before we clear
     * it out. We also set EXL in the task initialization, and all this will keep
     * the interrupt disabled when we are exiting the last context restoration section.
     * READ THE COMMENT ABOVE BEFORE REPORTING IT AS A BUG! */
    MTC0                $21,CP0_STATUS
    /* Restore everything else */
    LW                  $1,0($20)
    LW                  $2,4($20)
    LW                  $3,8($20)
    LW                  $4,12($20)
    LW                  $5,16($20)
    LW                  $6,20($20)
    LW                  $7,24($20)
    LW                  $8,28($20)
    LW                  $9,32($20)
    LW                  $10,36($20)
    LW                  $11,40($20)
    LW                  $12,44($20)
    LW                  $13,48($20)
    LW                  $14,52($20)
    LW                  $15,56($20)
    LW                  $16,60($20)
    LW                  $17,64($20)
    LW                  $18,68($20)
    LW                  $19,72($20)
    LW                  $24,76($20)
    LW                  $25,80($20)
    LW                  $30,84($20)
    LW                  $31,88($20)
    /* Restore "kernel only" registers */
    LW                  $26,92($20)
    LW                  $27,96($20)
    LW                  $28,100($20)
    /* Restore mutiply/divide registers */
    LW                  $22,104($20)
    LW                  $23,108($20)
    MTLO                $22
    MTHI                $23
    /* Restore four temporary regs */
    LW                  $20,120($sp)
    LW                  $21,124($sp)
    LW                  $22,128($sp)
    LW                  $23,132($sp)
    /* Restore the SP */
    ADDIU               $sp,$sp,CTX_SIZE
    ERET
    NOP
.endm
/* End Macros ****************************************************************/

/* Begin Function:RMP_Int_Disable *********************************************
Description    : The function for disabling all interrupts. Does not allow nesting.
Input          : None.
Output         : None.    
Register Usage : None.                                  
******************************************************************************/
    .set                nomips16
    .set                nomicromips
    .set                noreorder
    .set                noat
    .ent                RMP_Int_Disable
RMP_Int_Disable:
    /* Disable all interrupts */
    DI
    JR                  $ra
    NOP
    .end                RMP_Int_Disable
/* End Function:RMP_Int_Disable **********************************************/

/* Begin Function:RMP_Int_Enable **********************************************
Description    : The function for enabling all interrupts. Does not allow nesting.
Input          : None.
Output         : None.    
Register Usage : None.                                  
******************************************************************************/
    .set                nomips16
    .set                nomicromips
    .set                noreorder
    .set                noat
    .ent                RMP_Int_Enable
RMP_Int_Enable:
    /* Enable all interrupts */
    EI
    JR                  $ra
    NOP
    .end                RMP_Int_Enable
/* End Function:RMP_Int_Enable ***********************************************/

/* Begin Function:_RMP_Set_Timer **********************************************
Description    : The function for setting the timer.
Input          : $a0 - Timer overflow value.
Output         : None.    
Register Usage : None.                                  
******************************************************************************/
    .set                nomips16
    .set                nomicromips
    .set                noreorder
    .set                noat
    .ent                _RMP_Set_Timer
_RMP_Set_Timer:
    MTC0                $a0,CP0_COMPARE
    LI                  $a0,0
    MTC0                $a0,CP0_COUNT
    JR                  $ra
    NOP
    .end                _RMP_Set_Timer
/* End Function:_RMP_Set_Timer ***********************************************/

/* Begin Function:RMP_MSB_Get *************************************************
Description    : Get the MSB of the word.
Input          : ptr_t Val - The value.
Output         : None.
Return         : ptr_t - The MSB position.   
Register Usage : None. 
******************************************************************************/
    .set                nomips16
    .set                nomicromips
    .set                noreorder
    .set                noat
    .ent                RMP_MSB_Get
RMP_MSB_Get:
    CLZ                 $a0,$a0
    LI                  $v0,31
    SUB                 $v0,$a0
    JR                  $ra
    NOP
    .end                RMP_MSB_Get
/* End Function:RMP_MSB_Get **************************************************/

/* Begin Function:_RMP_Yield **************************************************
Description : Trigger a yield to another thread.
Input       : None.
Output      : None.                                      
******************************************************************************/
    .set                nomips16
    .set                nomicromips
    .set                noreorder
    .set                noat
    .ent                _RMP_Yield
_RMP_Yield:
    /* Get the cause for this */
    MFC0                $t0,$13
    OR                  $t0,CORE_SW0
    /* Now write back to trigger the software interrupt */
    MTC0                $t0,$13
    JR                  $ra
    NOP
    .end                _RMP_Yield                                           
/* End Function:_RMP_Yield ***************************************************/

/* Begin Function:_RMP_Start **************************************************
Description : Jump to the user function and will never return from it.
Input       : $a0 - The address to branch to.
              $a1 - The stack to use.
Output      : None.
******************************************************************************/
    .set                nomips16
    .set                nomicromips
    .set                noreorder
    .set                noat
    .ent                _RMP_Start
_RMP_Start:
    LUI                 $a2,%hi(RMP_SP_Val)         /* Store the SP to a global */
    ORI                 $a2,$a2,%lo(RMP_SP_Val)
    SW                  $sp,($a2)
                
    LUI                 $a2,%hi(RMP_GP_Val)         /* Store the GP to a global */
    ORI                 $a2,$a2,%lo(RMP_GP_Val)
    SW                  $gp,($a2)
                
    SUB                 $sp,$a1,128                 /* Set the new SP */
    JAL                 $a0                         /* Branch to target */
    NOP
__Loop: 
    J                   __Loop                      /* Dead loop to capture faults */
    NOP
    .end                _RMP_Start
/* End Function:_RMP_Start ***************************************************/

/* Begin Function:PendSV_Handler **********************************************
Description : The PendSV interrupt routine. In fact, it will call a C function
              directly. The reason why the interrupt routine must be an assembly
              function is that the compiler may deal with the stack in a different 
              way when different optimization level is chosen. An assembly function
              can make way around this problem.
              However, if your compiler support inline assembly functions, this
              can also be written in C.
              This vector is at the bottom of all priority levels, thus we can use
              a customized prologue and epilogue; However, we don't do this here for
              maintenance reasons. A fully customized one can get you 9 cycles faster.
Input       : None.
Output      : None.                                      
******************************************************************************/
    .set                nomips16
    .set                nomicromips
    .set                noreorder
    .set                noat
    .equ                __vector_dispatch_1,PendSV_Handler
    .global             __vector_dispatch_1
    .section            .vector_1,code
    .ent                PendSV_Handler
PendSV_Handler:
    SAVE_CONTEXT
    
    /* Clear the software interrupt in the core */
    MFC0                $22,CP0_CAUSE
    INS                 $22,$0,8,1
    MTC0                $22,CP0_CAUSE
    EHB
    /* Save extra registers */
    JAL                 RMP_Ctx_Save
    NOP
    /* Get the highest priority ready task */
    JAL                 _RMP_Rdy_High
    NOP
    /* Restore extra registers */
    JAL                 RMP_Ctx_Load
    NOP
    /* Clear the interrupt flag in the chip's interrupt controller */
    JAL                 _RMP_Clear_Soft_Flag
    NOP
                
    LOAD_CONTEXT
    .end                PendSV_Handler
/* End Function:PendSV_Handler ***********************************************/

/* Begin Function:SysTick_Handler *********************************************
Description : The SysTick interrupt routine. In fact, it will call a C function
              directly. The reason why the interrupt routine must be an assembly
              function is that the compiler may deal with the stack in a different 
              way when different optimization level is chosen. An assembly function
              can make way around this problem.
              However, if your compiler support inline assembly functions, this
              can also be written in C.
              This vector is at the bottom of all priority levels, thus we can use
              a customized prologue and epilogue; However, we don't do this here for
              maintenance reasons. A fully customized one can get you 9 cycles faster.
Input       : None.
Output      : None.                                      
******************************************************************************/
    .set                nomips16
    .set                nomicromips
    .set                noreorder
    .set                noat
    .equ                __vector_dispatch_0,SysTick_Handler
    .global             __vector_dispatch_0
    .section            .vector_0,code,keep
    .ent                SysTick_Handler
SysTick_Handler:
    /* Note the system that we have entered an interrupt. We are not using tickless */
    SAVE_CONTEXT
                
    LI                  $a0,1
    JAL                 _RMP_Tick_Handler
    NOP
    /* Clear the interrupt flag */
    JAL                 _RMP_Clear_Timer_Flag
    NOP
                
    LOAD_CONTEXT
    .end SysTick_Handler
/* End Function:SysTick_Handler **********************************************/
                
/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
