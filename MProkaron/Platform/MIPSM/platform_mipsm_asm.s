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
/* End Imports ***************************************************************/

/* Begin Macros **************************************************************/
                .equ             CP0_COUNT,$9
                .equ             CP0_COMPARE,$11
                .equ             CP0_STATUS,$12
                .equ             CP0_CAUSE,$13
                .equ             CP0_EPC,$14
                .equ             CTX_SIZE,140
                /* Context saving */
.macro	SAVE_CONTEXT
                /* Make room for the context */
                addiu            $sp,$sp,-CTX_SIZE
                /* Save two regs first so that we can enable interrupts as soon as possible */
                sw               $23,136($sp)
                sw               $22,132($sp)
                sw               $21,128($sp)
                /* Save all status registers */
                mfc0             $23,CP0_EPC
                mfc0             $22,CP0_CAUSE
	            mfc0             $21,CP0_STATUS
                sw               $23,124($sp)
                sw               $22,120($sp)
                sw               $21,116($sp)
                /* Now we reenable interrupts */
                ins              $21,$0,1,15             /* Clear IPL, UM, ERL, EXL from STATUS */
                ext              $22,$22,10,6            /* Extract RIPL from CAUSE */
                ins              $21,$22,10,6            /* Set current IPL */
                mtc0             $21,CP0_STATUS          /* Write status back */
                /* Save mutiply/divide registers */
                mfhi             $23
                mflo             $22
                sw               $23,112($sp)
                sw               $22,108($sp)
                /* Save "kernel only" registers - we do not expect the user to abide by the rule */
                sw               $28,104($sp)
                sw               $27,100($sp)
                sw               $26,96($sp)
                /* Save everything else */
                sw               $31,92($sp)
                sw               $30,88($sp)
                sw               $25,84($sp)
                sw               $24,80($sp)
                sw               $20,76($sp)
                sw               $19,72($sp)
                sw               $18,68($sp)
                sw               $17,64($sp)
                sw               $16,60($sp)
                sw               $15,56($sp)
                sw               $14,52($sp)
                sw               $13,48($sp)
                sw               $12,44($sp)
                sw               $11,40($sp)
                sw               $10,36($sp)
                sw               $9,32($sp)
                sw               $8,28($sp)
                sw               $7,24($sp)
                sw               $6,20($sp)
                sw               $5,16($sp)
                sw               $4,12($sp)
                sw               $3,8($sp)
                sw               $2,4($sp)
                sw               $1,0($sp)
                /* Save extra registers */
                jal              RMP_Save_Ctx
                nop
.endm
                /* Context restoring */
.macro	LOAD_CONTEXT
                /* Restore extra registers */
                jal              RMP_Load_Ctx
                nop
                /* Restore everything else */
                lw               $1,0($sp)
                lw               $2,4($sp)
                lw               $3,8($sp)
                lw               $4,12($sp)
                lw               $5,16($sp)
                lw               $6,20($sp)
                lw               $7,24($sp)
                lw               $8,28($sp)
                lw               $9,32($sp)
                lw               $10,36($sp)
                lw               $11,40($sp)
                lw               $12,44($sp)
                lw               $13,48($sp)
                lw               $14,52($sp)
                lw               $15,56($sp)
                lw               $16,60($sp)
                lw               $17,64($sp)
                lw               $18,68($sp)
                lw               $19,72($sp)
                lw               $20,76($sp)
                lw               $24,80($sp)
                lw               $25,84($sp)
                lw               $30,88($sp)
                lw               $31,92($sp)
                /* Restore "kernel only" registers */
                lw               $26,96($sp)
                lw               $27,100($sp)
                lw               $28,104($sp)
                /* Restore mutiply/divide registers */
                lw               $22,108($sp)
                lw               $23,112($sp)
                mtlo             $22
                mthi             $23
                /* Restore all status registers */
                lw               $21,116($sp)
                lw               $22,120($sp)
                lw               $23,124($sp)
                /* Protect access to c0 registers */
                di
                ehb
	            mtc0             $21,CP0_STATUS
                mtc0             $22,CP0_CAUSE
                mtc0             $23,CP0_EPC
                /* Restore three temporary regs */
                lw               $21,128($sp)
                lw               $22,132($sp)
                lw               $23,136($sp)
                /* Make room for the context */
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
                or               $t0,1
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
                sub              $sp,$a1,128                  /* Set the SP */
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
                SAVE_CONTEXT
                
                /* Save the SP to control block */
                lui              $a0, %hi(RMP_Cur_SP)
                ori              $a0, $a0, %lo(RMP_Cur_SP)
                sw               $sp,($a0)
                /* Get the highest priority ready task */
                jal              _RMP_Get_High_Rdy
                nop
                /* Load the SP from control block */
                lui              $a0, %hi(RMP_Cur_SP)
                ori              $a0, $a0, %lo(RMP_Cur_SP)
                lw               $sp,($a0)
                
                LOAD_CONTEXT
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
                
                LOAD_CONTEXT
                .end SysTick_Handler
/* End Function:SysTick_Handler **********************************************/
                
/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
