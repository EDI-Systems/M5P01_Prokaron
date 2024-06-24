/******************************************************************************
Filename    : rmp_platform_mp32p_gcc.inc
Author      : pry
Date        : 22/05/2024
Description : The assembly part of the RMP RTOS. This is for MIPS, and
              contains macros to be included in all interrupt assembly code
              as needed.
******************************************************************************/
    
/* Header ********************************************************************/
    .text
    .align              4

    /* CPU register definitions */
    .equ                RMP_CP0_COUNT,$9
    .equ                RMP_CP0_COMPARE,$11
    .equ                RMP_CP0_STATUS,$12
    .equ                RMP_CP0_CAUSE,$13
    .equ                RMP_CP0_EPC,$14
    .equ                RMP_CP1_FCSR,$31
/* End Header ****************************************************************/

/* Import ********************************************************************/
    /* The kernel SP and GP */
    .extern             _RMP_MP32P_SP_Kern
    .extern             _RMP_MP32P_GP_Kern
    /* The current thread stack */
    .extern             RMP_SP_Cur
    /* The yield pending flag */
    .extern             RMP_Sched_Pend
    /* Extract highest priority running thread */
    .extern             _RMP_Run_High
    /* Handler for MP32P timer interrupt */
    .extern             _RMP_MP32P_Tim_Handler
/* End Import ****************************************************************/

/* Macro *********************************************************************/
/* Save all GP regs **********************************************************/
    .macro              RMP_MP32P_SAVE              /* 30 GPRs, STATUS, EPC, 2 MDURs */
    ADDIU               $sp,$sp,-34*4
    SW                  $23,33*4($sp)
    SW                  $22,32*4($sp)
    MFC0                $23,RMP_CP0_EPC             /* Save PC here */
    SW                  $23,31*4($sp)
    MFC0                $22,RMP_CP0_STATUS          /* Save STATUS with IPL=0, EXL=1 */
    SW                  $22,30*4($sp)
    LI                  $23,1                       /* Mask IPL=1 system interrupts */
    INS                 $22,$23,10,6
    INS                 $22,$zero,1,1               /* Reenable transparent interrupts, EXL=0 */
    MTC0                $22,RMP_CP0_STATUS
    SW                  $31,29*4($sp)               /* GPRs except for $sp and $zero */
    SW                  $30,28*4($sp)
    SW                  $28,27*4($sp)
    SW                  $27,26*4($sp)
    SW                  $26,25*4($sp)
    SW                  $25,24*4($sp)
    SW                  $24,23*4($sp)
    SW                  $21,22*4($sp)
    SW                  $20,21*4($sp)
    SW                  $19,20*4($sp)
    SW                  $18,19*4($sp)
    SW                  $17,18*4($sp)
    SW                  $16,17*4($sp)
    SW                  $15,16*4($sp)
    SW                  $14,15*4($sp)
    SW                  $13,14*4($sp)
    SW                  $12,13*4($sp)
    SW                  $11,12*4($sp)
    SW                  $10,11*4($sp)
    SW                  $9,10*4($sp)
    SW                  $8,9*4($sp)
    SW                  $7,8*4($sp)
    SW                  $6,7*4($sp)
    SW                  $5,6*4($sp)
    SW                  $4,5*4($sp)
    SW                  $3,4*4($sp)
    SW                  $2,3*4($sp)
    SW                  $1,2*4($sp)
    MFHI                $2                          /* Standard HI and LO registers */
    MFLO                $1
    SW                  $2,1*4($sp)
    SW                  $1,0*4($sp)
    .endm

/* Prepare for the context switch ********************************************/
    .macro              RMP_MP32P_SWITCH_PRE
    LA                  $23,RMP_SP_Cur              /* Save SP to control block */
    SW                  $sp,($23)
    LA                  $23,_RMP_MP32P_SP_Kern      /* Load SP for kernel */
    LW                  $sp,($23)
    LA                  $gp,_RMP_MP32P_GP_Kern      /* Load GP for kernel */
    LW                  $gp,($gp)
    .endm

/* Finish up the context switch **********************************************/
    .macro              RMP_MP32P_SWITCH_POST
    LA                  $23,RMP_Sched_Pend
    LW                  $22,($23)
    NOP
    BEQ                 $22,$zero,1f
    NOP
    JAL                 _RMP_Run_High               /* Get the highest priority task */
    NOP
1:
    LA                  $23,RMP_SP_Cur              /* Load SP from control block */
    LW                  $sp,($23)
    NOP
    .endm

/* Restore all GP regs ******************************************************/
    .macro              RMP_MP32P_LOAD              /* 30 GPRs, STATUS, EPC, 2 MDURs */
    LW                  $1,0*4($sp)                 /* Standard HI and LO registers */
    LW                  $2,1*4($sp)
    MTLO                $1
    MTHI                $2
    LW                  $1,2*4($sp)                 /* GPRs except for $sp and $zero */
    LW                  $2,3*4($sp)
    LW                  $3,4*4($sp)
    LW                  $4,5*4($sp)
    LW                  $5,6*4($sp)
    LW                  $6,7*4($sp)
    LW                  $7,8*4($sp)
    LW                  $8,9*4($sp)
    LW                  $9,10*4($sp)
    LW                  $10,11*4($sp)
    LW                  $11,12*4($sp)
    LW                  $12,13*4($sp)
    LW                  $13,14*4($sp)
    LW                  $14,15*4($sp)
    LW                  $15,16*4($sp)
    LW                  $16,17*4($sp)
    LW                  $17,18*4($sp)
    LW                  $18,19*4($sp)
    LW                  $19,20*4($sp)
    LW                  $20,21*4($sp)
    LW                  $21,22*4($sp)
    LW                  $24,23*4($sp)
    LW                  $25,24*4($sp)
    LW                  $26,25*4($sp)
    LW                  $27,26*4($sp)
    LW                  $28,27*4($sp)
    LW                  $30,28*4($sp)
    LW                  $31,29*4($sp)
    LW                  $22,30*4($sp)               /* Load STATUS with IPL=0, EXL=1 */
    LW                  $23,31*4($sp)               /* Load EPC here; avoid STATUS load delay */
    MTC0                $22,RMP_CP0_STATUS
    MTC0                $23,RMP_CP0_EPC             /* Fill EPC safely here */
    LW                  $22,32*4($sp)
    LW                  $23,33*4($sp)
    ADDIU               $sp,$sp,34*4
    ERET
    .endm
    
/* Save DSPASE extra registers ***********************************************/
    .macro              RMP_MP32P_DSPASE_SAVE       /* 6 extra MDURs */
    ADDIU               $sp,$sp,-6*4
    .word               0x00201010                  /* MFHI    $2,$ac1 */
    .word               0x00200812                  /* MFLO    $1,$ac1 */
    SW                  $2,5*4($sp)
    SW                  $1,4*4($sp)
    .word               0x00401010                  /* MFHI    $2,$ac2 */
    .word               0x00400812                  /* MFLO    $1,$ac2 */
    SW                  $2,3*4($sp)
    SW                  $1,2*4($sp)
    .word               0x00601010                  /* MFHI    $2,$ac3 */
    .word               0x00600812                  /* MFLO    $1,$ac3 */
    SW                  $2,1*4($sp)
    SW                  $1,0*4($sp)
    .endm
    
/* Restore DSPASE extra registers ********************************************/
    .macro              RMP_MP32P_DSPASE_LOAD       /* 6 extra MDURs */
    LW                  $1,0*4($sp)
    LW                  $2,1*4($sp)
    .word               0x00201813                  /* MTLO    $1,$ac3 */
    .word               0x00401811                  /* MTHI    $2,$ac3 */
    LW                  $1,2*4($sp)
    LW                  $2,3*4($sp)
    .word               0x00201013                  /* MTLO    $1,$ac2 */
    .word               0x00401011                  /* MTHI    $2,$ac2 */
    LW                  $1,4*4($sp)
    LW                  $2,5*4($sp)
    .word               0x00200813                  /* MTLO    $1,$ac1 */
    .word               0x00400811                  /* MTHI    $2,$ac1 */
    ADDIU               $sp,$sp,6*4
    .endm

/* Save FR32 extra registers *************************************************/
    .macro              RMP_MP32P_FR32_SAVE         /* 16 DFPRs, FCSR, DUMMY */
    ADDIU               $sp,$sp,-17*8
    .word               0x4441F800                  /* CFC1    $1,RMP_CP1_FCSR */
    .word               0xF7BE0078                  /* SDC1    $f30,15*8($sp) */
    SW                  $1,16*8($sp)                /* Avoid FPU move delay */
    .word               0xF7BC0070                  /* SDC1    $f28,14*8($sp) */
    .word               0xF7BA0068                  /* SDC1    $f26,13*8($sp) */
    .word               0xF7B80060                  /* SDC1    $f24,12*8($sp) */
    .word               0xF7B60058                  /* SDC1    $f22,11*8($sp) */
    .word               0xF7B40050                  /* SDC1    $f20,10*8($sp) */
    .word               0xF7B20048                  /* SDC1    $f18,9*8($sp) */
    .word               0xF7B00040                  /* SDC1    $f16,8*8($sp) */
    .word               0xF7AE0038                  /* SDC1    $f14,7*8($sp) */
    .word               0xF7AC0030                  /* SDC1    $f12,6*8($sp) */
    .word               0xF7AA0028                  /* SDC1    $f10,5*8($sp) */
    .word               0xF7A80020                  /* SDC1    $f8,4*8($sp) */
    .word               0xF7A60018                  /* SDC1    $f6,3*8($sp) */
    .word               0xF7A40010                  /* SDC1    $f4,2*8($sp) */
    .word               0xF7A20008                  /* SDC1    $f2,1*8($sp) */
    .word               0xF7A00000                  /* SDC1    $f0,0*8($sp) */
    .endm
    
/* Restore FR32 extra registers **********************************************/
    .macro              RMP_MP32P_FR32_LOAD         /* 16 DFPRs, FCSR, DUMMY */
    .word               0xD7A00000                  /* LDC1    $f0,0*8($sp) */
    .word               0xD7A20008                  /* LDC1    $f2,1*8($sp) */
    .word               0xD7A40010                  /* LDC1    $f4,2*8($sp) */
    .word               0xD7A60018                  /* LDC1    $f6,3*8($sp) */
    .word               0xD7A80020                  /* LDC1    $f8,4*8($sp) */
    .word               0xD7AA0028                  /* LDC1    $f10,5*8($sp) */
    .word               0xD7AC0030                  /* LDC1    $f12,6*8($sp) */
    .word               0xD7AE0038                  /* LDC1    $f14,7*8($sp) */
    .word               0xD7B00040                  /* LDC1    $f16,8*8($sp) */
    .word               0xD7B20048                  /* LDC1    $f18,9*8($sp) */
    .word               0xD7B40050                  /* LDC1    $f20,10*8($sp) */
    .word               0xD7B60058                  /* LDC1    $f22,11*8($sp) */
    .word               0xD7B80060                  /* LDC1    $f24,12*8($sp) */
    .word               0xD7BA0068                  /* LDC1    $f26,13*8($sp) */
    .word               0xD7BC0070                  /* LDC1    $f28,14*8($sp) */
    LW                  $1,16*8($sp)                /* Avoid load delay */
    .word               0xD7BE0078                  /* LDC1    $f30,15*8($sp) */
    .word               0x44C1F800                  /* CTC1    $1,RMP_CP1_FCSR */
    ADDIU               $sp,$sp,17*8
    .endm
    
/* Save FR64 extra registers *************************************************/
    .macro              RMP_MP32P_FR64_SAVE         /* 32 DFPRs, FCSR, DUMMY */
    ADDIU               $sp,$sp,-33*8
    .word               0x4441F800                  /* CFC1    $1,RMP_CP1_FCSR */
    .word               0xF7BF00F8                  /* SDC1    $f31,31*8($sp) */
    SW                  $1,32*8($sp)                /* Avoid FPU move delay */
    .word               0xF7BE00F0                  /* SDC1    $f30,30*8($sp) */
    .word               0xF7BD00E8                  /* SDC1    $f29,29*8($sp) */
    .word               0xF7BC00E0                  /* SDC1    $f28,28*8($sp) */
    .word               0xF7BB00D8                  /* SDC1    $f27,27*8($sp) */
    .word               0xF7BA00D0                  /* SDC1    $f26,26*8($sp) */
    .word               0xF7B900C8                  /* SDC1    $f25,25*8($sp) */
    .word               0xF7B800C0                  /* SDC1    $f24,24*8($sp) */
    .word               0xF7B700B8                  /* SDC1    $f23,23*8($sp) */
    .word               0xF7B600B0                  /* SDC1    $f22,22*8($sp) */
    .word               0xF7B500A8                  /* SDC1    $f21,21*8($sp) */
    .word               0xF7B400A0                  /* SDC1    $f20,20*8($sp) */
    .word               0xF7B30098                  /* SDC1    $f19,19*8($sp) */
    .word               0xF7B20090                  /* SDC1    $f18,18*8($sp) */
    .word               0xF7B10088                  /* SDC1    $f17,17*8($sp) */
    .word               0xF7B00080                  /* SDC1    $f16,16*8($sp) */
    .word               0xF7AF0078                  /* SDC1    $f15,15*8($sp) */
    .word               0xF7AE0070                  /* SDC1    $f14,14*8($sp) */
    .word               0xF7AD0068                  /* SDC1    $f13,13*8($sp) */
    .word               0xF7AC0060                  /* SDC1    $f12,12*8($sp) */
    .word               0xF7AB0058                  /* SDC1    $f11,11*8($sp) */
    .word               0xF7AA0050                  /* SDC1    $f10,10*8($sp) */
    .word               0xF7A90048                  /* SDC1    $f9,9*8($sp) */
    .word               0xF7A80040                  /* SDC1    $f8,8*8($sp) */
    .word               0xF7A70038                  /* SDC1    $f7,7*8($sp) */
    .word               0xF7A60030                  /* SDC1    $f6,6*8($sp) */
    .word               0xF7A50028                  /* SDC1    $f5,5*8($sp) */
    .word               0xF7A40020                  /* SDC1    $f4,4*8($sp) */
    .word               0xF7A30018                  /* SDC1    $f3,3*8($sp) */
    .word               0xF7A20010                  /* SDC1    $f2,2*8($sp) */
    .word               0xF7A10008                  /* SDC1    $f1,1*8($sp) */
    .word               0xF7A00000                  /* SDC1    $f0,0*8($sp) */
    .endm
    
/* Restore FR64 extra registers **********************************************/
    .macro              RMP_MP32P_FR64_LOAD         /* 32 DFPRs, FCSR, DUMMY */
    .word               0xD7A00000                  /* LDC1    $f0,0*8($sp) */
    .word               0xD7A10008                  /* LDC1    $f1,1*8($sp) */
    .word               0xD7A20010                  /* LDC1    $f2,2*8($sp) */
    .word               0xD7A30018                  /* LDC1    $f3,3*8($sp) */
    .word               0xD7A40020                  /* LDC1    $f4,4*8($sp) */
    .word               0xD7A50028                  /* LDC1    $f5,5*8($sp) */
    .word               0xD7A60030                  /* LDC1    $f6,6*8($sp) */
    .word               0xD7A70038                  /* LDC1    $f7,7*8($sp) */
    .word               0xD7A80040                  /* LDC1    $f8,8*8($sp) */
    .word               0xD7A90048                  /* LDC1    $f9,9*8($sp) */
    .word               0xD7AA0050                  /* LDC1    $f10,10*8($sp) */
    .word               0xD7AB0058                  /* LDC1    $f11,11*8($sp) */
    .word               0xD7AC0060                  /* LDC1    $f12,12*8($sp) */
    .word               0xD7AD0068                  /* LDC1    $f13,13*8($sp) */
    .word               0xD7AE0070                  /* LDC1    $f14,14*8($sp) */
    .word               0xD7AF0078                  /* LDC1    $f15,15*8($sp) */
    .word               0xD7B00080                  /* LDC1    $f16,16*8($sp) */
    .word               0xD7B10088                  /* LDC1    $f17,17*8($sp) */
    .word               0xD7B20090                  /* LDC1    $f18,18*8($sp) */
    .word               0xD7B30098                  /* LDC1    $f19,19*8($sp) */
    .word               0xD7B400A0                  /* LDC1    $f20,20*8($sp) */
    .word               0xD7B500A8                  /* LDC1    $f21,21*8($sp) */
    .word               0xD7B600B0                  /* LDC1    $f22,22*8($sp) */
    .word               0xD7B700B8                  /* LDC1    $f23,23*8($sp) */
    .word               0xD7B800C0                  /* LDC1    $f24,24*8($sp) */
    .word               0xD7B900C8                  /* LDC1    $f25,25*8($sp) */
    .word               0xD7BA00D0                  /* LDC1    $f26,26*8($sp) */
    .word               0xD7BB00D8                  /* LDC1    $f27,27*8($sp) */
    .word               0xD7BC00E0                  /* LDC1    $f28,28*8($sp) */
    .word               0xD7BD00E8                  /* LDC1    $f29,29*8($sp) */
    .word               0xD7BE00F0                  /* LDC1    $f30,30*8($sp) */
    LW                  $1,32*8($sp)                /* Avoid load delay */
    .word               0xD7BF00F8                  /* LDC1    $f31,31*8($sp) */
    .word               0x44C1F800                  /* CTC1    $1,RMP_CP1_FCSR */
    ADDIU               $sp,$sp,33*8
    .endm

/* Save macro for normal case ************************************************/
    .macro              RMP_MP32P_INT_SAVE_NONE
    RMP_MP32P_SAVE
    RMP_MP32P_SWITCH_PRE
    .endm
    
/* Restore macro for normal case *********************************************/
    .macro              RMP_MP32P_INT_LOAD_NONE
    RMP_MP32P_SWITCH_POST
    RMP_MP32P_LOAD
    .endm

/* Save macro for DSPASE *****************************************************/
    .macro              RMP_MP32P_INT_SAVE_DSPASE
    RMP_MP32P_SAVE
    RMP_MP32P_DSPASE_SAVE
    RMP_MP32P_SWITCH_PRE
    .endm
    
/* Restore macro for DSPASE **************************************************/
    .macro              RMP_MP32P_INT_LOAD_DSPASE
    RMP_MP32P_SWITCH_POST
    RMP_MP32P_DSPASE_LOAD
    RMP_MP32P_LOAD
    .endm
    
/* Save macro for FR32 *******************************************************/
    .macro              RMP_MP32P_INT_SAVE_FR32
    RMP_MP32P_SAVE
    RMP_MP32P_FR32_SAVE
    RMP_MP32P_SWITCH_PRE
    .endm
    
/* Restore macro for FR32 ****************************************************/
    .macro              RMP_MP32P_INT_LOAD_FR32
    RMP_MP32P_SWITCH_POST
    RMP_MP32P_FR32_LOAD
    RMP_MP32P_LOAD
    .endm
    
/* Save macro for FR64 *******************************************************/
    .macro              RMP_MP32P_INT_SAVE_FR64
    RMP_MP32P_SAVE
    RMP_MP32P_FR64_SAVE
    RMP_MP32P_SWITCH_PRE
    .endm
    
/* Restore macro for FR64 ****************************************************/
    .macro              RMP_MP32P_INT_LOAD_FR64
    RMP_MP32P_SWITCH_POST
    RMP_MP32P_FR64_LOAD
    RMP_MP32P_LOAD
    .endm
    
/* Save macro for DSPASE & FR32 **********************************************/
    .macro              RMP_MP32P_INT_SAVE_DSPASE_FR32
    RMP_MP32P_SAVE
    RMP_MP32P_DSPASE_SAVE
    RMP_MP32P_FR32_SAVE
    RMP_MP32P_SWITCH_PRE
    .endm
    
/* Restore macro for DSPASE & FR32 *******************************************/
    .macro              RMP_MP32P_INT_LOAD_DSPASE_FR32
    RMP_MP32P_SWITCH_POST
    RMP_MP32P_FR32_LOAD
    RMP_MP32P_DSPASE_LOAD
    RMP_MP32P_LOAD
    .endm
    
/* Save macro for DSPASE & FR64 **********************************************/
    .macro              RMP_MP32P_INT_SAVE_DSPASE_FR64
    RMP_MP32P_SAVE
    RMP_MP32P_DSPASE_SAVE
    RMP_MP32P_FR64_SAVE
    RMP_MP32P_SWITCH_PRE
    .endm
    
/* Restore macro for DSPASE & FR64 *******************************************/
    .macro              RMP_MP32P_INT_LOAD_DSPASE_FR64
    RMP_MP32P_SWITCH_POST
    RMP_MP32P_FR64_LOAD
    RMP_MP32P_DSPASE_LOAD
    RMP_MP32P_LOAD
    .endm
/* End Macro *****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
