/******************************************************************************
Filename    : platform_mipsm_asm.s
Author      : pry
Date        : 10/04/2012
Description : The assembly part of the RMP RTOS.
******************************************************************************/
    
/* The MIPS M-Class Structure *************************************************
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
R4     $a0	      argument 1
R5     $a1  	  argument 2
R6     $a2        argument 3
R7     $a3        argument 4
R8     $t0  	  temporary (not preserved across call)
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
                .global          RMP_Disable_Int      
                /* Enable all interrupts */        
                .global          RMP_Enable_Int   
                /* Get the MSB */
                .global          RMP_MSB_Get
                /* Start the first thread */
                .global          _RMP_Start
                /* The context switch trigger */
                .global          _RMP_Yield
                /* The system pending service routine */
                .global          PendSV_Handler 
                /* The systick timer routine */
                .global          SysTick_Handler
                /* Set timer overflow value */
                .global          _RMP_Set_Timer
/* End Exports ***************************************************************/

/* Begin Imports *************************************************************/
                /* The real task switch handling function */
                .extern         _RMP_Get_High_Rdy 
                /* The real systick handler function */
                .extern          _RMP_Tick_Handler
                /* The PID of the current thread */
                .extern          RMP_Cur_Thd
                /* The stack address of current thread */
                .extern          RMP_Cur_SP
                /* Save and load extra contexts, such as FPU, peripherals and MPU */
                .extern          RMP_Save_Ctx
                .extern          RMP_Load_Ctx
                /* The place where we store the kernel gp/sp value */
                .extern          RMP_GP_Val
                .extern          RMP_SP_Val
                /* The interrupt nesting value */
                .extern          RMP_Int_Nest
                /* Clear timer & software interrupt flags */
                .extern          _RMP_Clear_Soft_Flag
                .extern          _RMP_Clear_Timer_Flag
/* End Imports ***************************************************************/

/* Begin Macros **************************************************************/
                .equ             CP0_COUNT,$9
                .equ             CP0_COMPARE,$11
                .equ             CP0_STATUS,$12
                .equ             CP0_CAUSE,$13
                .equ             CP0_EPC,$14
                .equ             CTX_SIZE,136
                .equ             CORE_SW0,0x100
/* Interrupt context - this is for generic support. These interrupts should not use DSP & FPU */
.macro	SAVE_CONTEXT
                /* Make room for the context */
                addiu            $sp,$sp,-CTX_SIZE
                /* Save three regs first so that we can enable interrupts as soon as possible */
                sw               $23,132($sp)
                sw               $22,128($sp)
                sw               $21,124($sp)
                sw               $20,120($sp)
                /* Increase interrupt nesting count */
                lui              $23,%hi(RMP_Int_Nest)
                ori              $23,$23,%lo(RMP_Int_Nest)
                lw               $22,($23)
                nop
                addiu            $21,$22,1
                sw               $21,($23)
                /* Branch taken when already in system stack */
                bne              $22,$0,1f 
                nop
                /* Need to save the old sp first */
                lui              $23,%hi(RMP_Old_SP_Val)
                ori              $23,$23,%lo(RMP_Old_SP_Val)
                sw               $sp,($23)
                move             $20,$sp
                /* We need to swap to system stack for execution */
                lui              $23,%hi(RMP_SP_Val)
                ori              $23,$23,%lo(RMP_SP_Val)
                lw               $sp,($23)
                b                2f
                nop
1:              move             $20,$sp
                /* Save all status registers, except for CAUSE */
2:              mfc0             $23,CP0_EPC
                mfc0             $22,CP0_CAUSE
	            mfc0             $21,CP0_STATUS
                sw               $23,116($20)
                sw               $21,112($20)
                /* Now we reenable interrupts */
                ins              $21,$0,1,15             /* Clear IPL, UM, ERL, EXL from STATUS */
                ext              $23,$22,10,6            /* Extract RIPL from CAUSE */
                ins              $21,$23,10,6            /* Set current IPL */
                mtc0             $21,CP0_STATUS          /* Write status back */
                /* Save mutiply/divide registers */
                mfhi             $23
                mflo             $22
                sw               $23,108($20)
                sw               $22,104($20)
                /* Save "kernel only" registers - we do not expect the user to abide by the rule */
                sw               $28,100($20)
                sw               $27,96($20)
                sw               $26,92($20)
                /* Save everything else */
                sw               $31,88($20)
                sw               $30,84($20)
                sw               $25,80($20)
                sw               $24,76($20)
                sw               $19,72($20)
                sw               $18,68($20)
                sw               $17,64($20)
                sw               $16,60($20)
                sw               $15,56($20)
                sw               $14,52($20)
                sw               $13,48($20)
                sw               $12,44($20)
                sw               $11,40($20)
                sw               $10,36($20)
                sw               $9,32($20)
                sw               $8,28($20)
                sw               $7,24($20)
                sw               $6,20($20)
                sw               $5,16($20)
                sw               $4,12($20)
                sw               $3,8($20)
                sw               $2,4($20)
                sw               $1,0($20)
.endm
                
                /* Context restoring */
.macro LOAD_CONTEXT
                /* Protect access to c0 registers */
                di
                ehb
                /* Decrease interrupt nesting count */
                lui              $23,%hi(RMP_Int_Nest)
                ori              $23,$23,%lo(RMP_Int_Nest)
                lw               $22,($23)
                nop
                addiu            $21,$22,-1
                sw               $21,($23)
                /* Branch taken when going to return to user stack */
                bne              $21,$0,1f 
                nop
                /* Need to load back the old sp */
                lui              $23,%hi(RMP_Old_SP_Val)
                ori              $23,$23,%lo(RMP_Old_SP_Val)
                lw               $sp,($23)
                nop
1:              move             $20,$sp
                /* Restore all status registers, except for CAUSE */
                lw               $21,112($20)
                lw               $23,116($20)
                mtc0             $23,CP0_EPC
                /* Make sure status is recovered at last */
	            mtc0             $21,CP0_STATUS
                /* Restore everything else */
                lw               $1,0($20)
                lw               $2,4($20)
                lw               $3,8($20)
                lw               $4,12($20)
                lw               $5,16($20)
                lw               $6,20($20)
                lw               $7,24($20)
                lw               $8,28($20)
                lw               $9,32($20)
                lw               $10,36($20)
                lw               $11,40($20)
                lw               $12,44($20)
                lw               $13,48($20)
                lw               $14,52($20)
                lw               $15,56($20)
                lw               $16,60($20)
                lw               $17,64($20)
                lw               $18,68($20)
                lw               $19,72($20)
                lw               $24,76($20)
                lw               $25,80($20)
                lw               $30,84($20)
                lw               $31,88($20)
                /* Restore "kernel only" registers */
                lw               $26,92($20)
                lw               $27,96($20)
                lw               $28,100($20)
                /* Restore mutiply/divide registers */
                lw               $22,104($20)
                lw               $23,108($20)
                mtlo             $22
                mthi             $23
                /* Restore four temporary regs */
                lw               $20,120($sp)
                lw               $21,124($sp)
                lw               $22,128($sp)
                lw               $23,132($sp)
                /* Restore the SP */
                addiu            $sp,$sp,CTX_SIZE
                eret
                nop
.endm
/* End Macros ****************************************************************/

/* Begin Function:RMP_Disable_Int *********************************************
Description    : The function for disabling all interrupts. Does not allow nesting.
Input          : None.
Output         : None.    
Register Usage : None.                                  
******************************************************************************/
               	.set             nomips16
                .set             nomicromips
                .set             noreorder
                .set             noat
                .ent             RMP_Disable_Int
RMP_Disable_Int:
                /* Disable all interrupts */
                di
                jr               $ra
                nop
                .end             RMP_Disable_Int
/* End Function:RMP_Disable_Int **********************************************/

/* Begin Function:RMP_Enable_Int **********************************************
Description    : The function for enabling all interrupts. Does not allow nesting.
Input          : None.
Output         : None.    
Register Usage : None.                                  
******************************************************************************/
               	.set             nomips16
                .set             nomicromips
                .set             noreorder
                .set             noat
                .ent             RMP_Enable_Int
RMP_Enable_Int:
                /* Enable all interrupts */
                ei
                jr               $ra
                nop
                .end             RMP_Enable_Int
/* End Function:RMP_Enable_Int ***********************************************/

/* Begin Function:_RMP_Set_Timer **********************************************
Description    : The function for setting the timer.
Input          : $a0 - Timer overflow value.
Output         : None.    
Register Usage : None.                                  
******************************************************************************/
               	.set             nomips16
                .set             nomicromips
                .set             noreorder
                .set             noat
                .ent             _RMP_Set_Timer
_RMP_Set_Timer:
                mtc0             $a0,CP0_COMPARE
                li               $a0,0
                mtc0             $a0,CP0_COUNT
                jr               $ra
                nop
                .end             _RMP_Set_Timer
/* End Function:_RMP_Set_Timer ***********************************************/

/* Begin Function:RMP_MSB_Get *************************************************
Description    : Get the MSB of the word.
Input          : ptr_t Val - The value.
Output         : None.
Return         : ptr_t - The MSB position.   
Register Usage : None. 
******************************************************************************/
               	.set             nomips16
                .set             nomicromips
                .set             noreorder
                .set             noat
                .ent             RMP_MSB_Get
RMP_MSB_Get:
                clz              $a0,$a0
                li               $v0,31
                sub              $v0,$a0
                jr               $ra
                nop
                .end             RMP_MSB_Get
/* End Function:RMP_MSB_Get **************************************************/

/* Begin Function:_RMP_Yield **************************************************
Description : Trigger a yield to another thread.
Input       : None.
Output      : None.                                      
******************************************************************************/
               	.set             nomips16
                .set             nomicromips
                .set             noreorder
                .set             noat
                .ent             _RMP_Yield
_RMP_Yield:
                /* Get the cause for this */
                mfc0             $t0,$13
                or               $t0,CORE_SW0
                /* Now write back to trigger the software interrupt */
                mtc0             $t0,$13
                jr               $ra
                nop
                .end             _RMP_Yield                                           
/* End Function:_RMP_Yield ***************************************************/

;/* Begin Function:_RMP_Start *************************************************
;Description : Jump to the user function and will never return from it.
;Input       : R0 - The address to branch to.
               R1 - The stack to use.
;Output      : None.                                      
;*****************************************************************************/
               	.set             nomips16
                .set             nomicromips
                .set             noreorder
                .set             noat
                .ent             _RMP_Start
_RMP_Start:
                lui              $a2,%hi(RMP_SP_Val)          /* Store the SP to a global */
                ori              $a2,$a2,%lo(RMP_SP_Val)
                sw               $sp,($a2)
                
                lui              $a2,%hi(RMP_GP_Val)          /* Store the GP to a global */
                ori              $a2,$a2,%lo(RMP_GP_Val)
                sw               $gp,($a2)
                
                sub              $sp,$a1,128                  /* Set the new SP */
                jal              $a0                          /* Branch to target */
                nop
__Loop:         j                __Loop                       /* Dead loop to capture faults */
                nop
                .end             _RMP_Start
/* End Function:_RMP_Start ***************************************************/

/* Begin Function:PendSV_Handler **********************************************
Description : The PendSV interrupt routine. In fact, it will call a C function
              directly. The reason why the interrupt routine must be an assembly
              function is that the compiler may deal with the stack in a different 
              way when different optimization level is chosen. An assembly function
              can make way around this problem.
              However, if your compiler support inline assembly functions, this
              can also be written in C.
Input       : None.
Output      : None.                                      
******************************************************************************/
               	.set             nomips16
                .set             nomicromips
                .set             noreorder
                .set             noat
                .equ             __vector_dispatch_1,PendSV_Handler
                .global          __vector_dispatch_1
                .section         .vector_1,code
                .ent             PendSV_Handler
PendSV_Handler:
                /* Make room for the context */
                addiu            $sp,$sp,-CTX_SIZE
                /* Save three regs first so that we can ena \ble interrupts as soon as possible */
                sw               $23,132($sp)
                sw               $22,128($sp)
                sw               $21,124($sp)
                sw               $20,120($sp)
                /* Increase interrupt nesting count */
                lui              $23,%hi(RMP_Int_Nest)
                ori              $23,$23,%lo(RMP_Int_Nest)
                lw               $22,($23)
                nop
                addiu            $21,$22,1
                sw               $21,($23)
                /* We always need to swap to system stack for execution */
                lui              $23,%hi(RMP_SP_Val)
                ori              $23,$23,%lo(RMP_SP_Val)
                move             $20,$sp
                lw               $sp,($23)
                /* Save all status registers, except for CAUSE */
                mfc0             $23,CP0_EPC
                mfc0             $22,CP0_CAUSE
	            mfc0             $21,CP0_STATUS
                sw               $23,116($20)
                sw               $21,112($20)
                /* Clear the software interrupt in the core */
                mfc0             $22,CP0_CAUSE
                ins              $22,$0,8,1
                mtc0             $22,CP0_CAUSE
                ehb
                /* Now we reenable interrupts */
                ins              $21,$0,1,15             /* Clear IPL, UM, ERL, EXL from STATUS */
                ext              $23,$22,10,6            /* Extract RIPL from CAUSE */
                ins              $21,$23,10,6            /* Set current IPL */
                mtc0             $21,CP0_STATUS          /* Write status back */
                ehb
                /* Save mutiply/divide registers */
                mfhi             $23
                mflo             $22
                sw               $23,108($20)
                sw               $22,104($20)
                /* Save "kernel only" registers - we do not expect the user to abide by the rule */
                sw               $28,100($20)
                sw               $27,96($20)
                sw               $26,92($20)
                /* Save everything else */
                sw               $31,88($20)
                sw               $30,84($20)
                sw               $25,80($20)
                sw               $24,76($20)
                sw               $19,72($20)
                sw               $18,68($20)
                sw               $17,64($20)
                sw               $16,60($20)
                sw               $15,56($20)
                sw               $14,52($20)
                sw               $13,48($20)
                sw               $12,44($20)
                sw               $11,40($20)
                sw               $10,36($20)
                sw               $9,32($20)
                sw               $8,28($20)
                sw               $7,24($20)
                sw               $6,20($20)
                sw               $5,16($20)
                sw               $4,12($20)
                sw               $3,8($20)
                sw               $2,4($20)
                sw               $1,0($20)
                
                /* Save the SP to control block */
                lui              $a0,%hi(RMP_Cur_SP)
                ori              $a0,$a0,%lo(RMP_Cur_SP)
                sw               $20,($a0)
                /* Now load our own gp for system use */
                lui              $gp,%hi(RMP_GP_Val)
                ori              $gp,$gp,%lo(RMP_GP_Val)
                lw               $gp,($gp)
                /* Save extra registers */
                jal              RMP_Save_Ctx
                nop
                /* Get the highest priority ready task */
                jal              _RMP_Get_High_Rdy
                nop
                /* Restore extra registers */
                jal              RMP_Load_Ctx
                nop
                /* Load the SP from control block */
                lui              $a0,%hi(RMP_Cur_SP)
                ori              $a0,$a0, %lo(RMP_Cur_SP)
                lw               $20,($a0)
                nop
                /* Clear the interrupt flag */
                jal              _RMP_Clear_Soft_Flag
                nop
                
                /* Restore everything else */
                lw               $1,0($20)
                lw               $2,4($20)
                lw               $3,8($20)
                lw               $4,12($20)
                lw               $5,16($20)
                lw               $6,20($20)
                lw               $7,24($20)
                lw               $8,28($20)
                lw               $9,32($20)
                lw               $10,36($20)
                lw               $11,40($20)
                lw               $12,44($20)
                lw               $13,48($20)
                lw               $14,52($20)
                lw               $15,56($20)
                lw               $16,60($20)
                lw               $17,64($20)
                lw               $18,68($20)
                lw               $19,72($20)
                lw               $24,76($20)
                lw               $25,80($20)
                lw               $30,84($20)
                lw               $31,88($20)
                /* Restore "kernel only" registers */
                lw               $26,92($20)
                lw               $27,96($20)
                lw               $28,100($20)
                /* Restore mutiply/divide registers */
                lw               $22,104($20)
                lw               $23,108($20)
                mtlo             $22
                mthi             $23
                /* Protect access to c0 registers */
                di
                ehb
                /* Switch back to the user stack */
                move             $sp,$20
                /* Decrease interrupt nesting count */
                lui              $23,%hi(RMP_Int_Nest)
                ori              $23,$23,%lo(RMP_Int_Nest)
                lw               $22,($23)
                nop
                addiu            $21,$22,-1
                sw               $21,($23)
                /* Restore all status registers, except for CAUSE */
                lw               $21,112($20)
                lw               $23,116($20)
                mtc0             $23,CP0_EPC
                /* Make sure status is recovered at last */
	            mtc0             $21,CP0_STATUS
                /* Restore four temporary regs */
                lw               $20,120($sp)
                lw               $21,124($sp)
                lw               $22,128($sp)
                lw               $23,132($sp)
                /* Restore the SP */
                addiu            $sp,$sp,CTX_SIZE
                eret
                nop
                .end             PendSV_Handler
/* End Function:PendSV_Handler ***********************************************/

/* Begin Function:SysTick_Handler *********************************************
Description : The SysTick interrupt routine. In fact, it will call a C function
              directly. The reason why the interrupt routine must be an assembly
              function is that the compiler may deal with the stack in a different 
              way when different optimization level is chosen. An assembly function
              can make way around this problem.
              However, if your compiler support inline assembly functions, this
              can also be written in C.
Input       : None.
Output      : None.                                      
******************************************************************************/
               	.set             nomips16
                .set             nomicromips
                .set             noreorder
                .set             noat
                .equ             __vector_dispatch_0,SysTick_Handler
                .global          __vector_dispatch_0
                .section         .vector_0,code,keep
                .ent             SysTick_Handler
SysTick_Handler:
                /* Note the system that we have entered an interrupt. We are not using tickless */
                SAVE_CONTEXT
                
                li               $a0,1
                jal              _RMP_Tick_Handler
                nop
                /* Clear the interrupt flag */
                jal              _RMP_Clear_Timer_Flag
                nop
                
                LOAD_CONTEXT
                .end SysTick_Handler
/* End Function:SysTick_Handler **********************************************/
                
/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
