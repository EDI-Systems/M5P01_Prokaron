/******************************************************************************
Filename    : rmp_test_PIC32MZ2048EFM100_extra.s
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
    .extern             RMP_GP_Val
    .extern             RMP_SP_Val
    /* The interrupt nesting value */
    .extern             RMP_Int_Nest
    /* The extra routine */
    .extern             Tim2_Interrupt    
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
.macro	SAVE_CONTEXT
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
    LUI                 $23,%hi(RMP_Cur_SP)
    ORI                 $23,$23,%lo(RMP_Cur_SP)
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
    LUI                 $23,%hi(RMP_Cur_SP)
    ORI                 $23,$23,%lo(RMP_Cur_SP)
    LW                  $sp,($23)
    NOP
1:  MOVE                $20,$sp
    /* Restore all status registers, except for CAUSE */
    LW                  $21,112($20)
    LW                  $23,116($20)
    MTC0                $23,CP0_EPC
    /* Interrupts NOT enabled by this status restoration. This is because the EXL
     * bit is present when the interrupt comes in, and we saved it before we clear.
     * it out. We also set EXL in the task initialization, and all this will keep
     * the interrupt disabled when we are exiting the context restoration section. */
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

/* Begin Function:Tim2_Handler ************************************************
Description : The timer 2 interrupt routine.
Input       : None.
Output      : None.                                      
******************************************************************************/
    .set                nomips16
    .set                nomicromips
    .set                noreorder
    .set                noat
    .equ                __vector_dispatch_9,Tim2_Handler
    .global             __vector_dispatch_9
    .section            .vector_9,code,keep
    .ent                Tim2_Handler
Tim2_Handler:
    SAVE_CONTEXT
                
    JAL                 Tim2_Interrupt
    NOP
                
    LOAD_CONTEXT
    .end                Tim2_Handler
/* End Function:Tim2_Handler *************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
