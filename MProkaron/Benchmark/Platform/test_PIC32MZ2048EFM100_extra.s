/******************************************************************************
Filename    : test_PIC32MZ2048EFM100_extra.s
Author      : pry
Date        : 10/04/2012
Description : The extra testing file for this chip.
******************************************************************************/
    
/* Begin Header **************************************************************/
                .text
                .align 4
/* End Header ****************************************************************/

/* Begin Imports *************************************************************/
                /* The place where we store the kernel gp/sp value */
                .extern          RMP_GP_Val
                .extern          RMP_SP_Val
                /* The interrupt nesting value */
                .extern          RMP_Int_Nest
                /* The extra routine */
                .extern          Tim2_Interrupt    
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

/* Begin Function:Tim2_Handler ************************************************
Description : The timer 2 interrupt routine.
Input       : None.
Output      : None.                                      
******************************************************************************/
                .set             nomips16
                .set             nomicromips
                .set             noreorder
                .set             noat
                .equ             __vector_dispatch_9,Tim2_Handler
                .global          __vector_dispatch_9
                .section         .vector_9,code,keep
                .ent             Tim2_Handler
Tim2_Handler:
                /* Note the system that we have entered an interrupt. We are not using tickless */
                SAVE_CONTEXT
                
                jal              Tim2_Interrupt
                nop
                
                LOAD_CONTEXT
                .end Tim2_Handler
/* End Function:Tim2_Handler *************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
