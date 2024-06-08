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
    .equ                RMP_CP1_FSCR,$31
/* End Header ****************************************************************/

/* Import ********************************************************************/
    /* The kernel SP and GP */
    .extern             _RMP_MP32P_SP_Kern
    .extern             _RMP_MP32P_GP_Kern
    /* The current thread stack */
    .extern             RMP_SP_Cur
    /* The interrupt active flag */
    .extern             RMP_MP32P_Int_Act
    /* The yield pending flag */
    .extern             _RMP_MP32P_Yield_Pend
    /* Extract highest priority running thread */
    .extern             _RMP_Run_High
    /* Handler for MP32P timer interrupt */
    .extern             _RMP_MP32P_Tim_Handler
/* End Import ****************************************************************/

/* Macro *********************************************************************/
/* Save all GP regs **********************************************************/
    .macro              RMP_MP32P_SAVE
    ADDIU               $sp,$sp,-34*4               /* 30 GPRs, STATUS, EPC, 2 MDURs */
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
    LI                  $22,1                       /* Indicate interrupt active */
    LA                  $23,RMP_MP32P_Int_Act
    SW                  $22,($23)
    .endm

/* Finish up the context switch **********************************************/
    .macro              RMP_MP32P_SWITCH_POST
    LA                  $23,_RMP_MP32P_Yield_Pend
    LW                  $22,($23)
    NOP
    BEQ                 $22,$zero,1f
    SW                  $zero,($23)                 /* Does no harm even when this always execute */
    JAL                 _RMP_Run_High               /* Get the highest priority task */
    NOP
1:
    LA                  $23,RMP_MP32P_Int_Act
    SW                  $zero,($23)
    LA                  $23,RMP_SP_Cur              /* Load SP from control block */
    LW                  $sp,($23)
    NOP
    .endm

/* Restore all GP regs ******************************************************/
    .macro              RMP_MP32P_LOAD
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
    .macro              RMP_MP32P_DSPASE_SAVE
    ADDIU               $sp,$sp,-6*4                /* 6 extra MDURs */
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
    .macro              RMP_MP32P_DSPASE_LOAD
    LW                  $1,0*4($sp)                 /* 6 extra MDURs */
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
    .macro              RMP_MP32P_FR32_SAVE
    ADDIU               $sp,$sp,-16*8-4             /* 16 DFPRs + FSCR */
    .word               0xF7BE007C                  /* SDC1    $f30,15*8+4($sp) */
    .word               0xF7BC0074                  /* SDC1    $f28,14*8+4($sp) */
    .word               0xF7BA006C                  /* SDC1    $f26,13*8+4($sp) */
    .word               0xF7B80064                  /* SDC1    $f24,12*8+4($sp) */
    .word               0xF7B6005C                  /* SDC1    $f22,11*8+4($sp) */
    .word               0xF7B40054                  /* SDC1    $f20,10*8+4($sp) */
    .word               0xF7B2004C                  /* SDC1    $f18,9*8+4($sp) */
    .word               0xF7B00044                  /* SDC1    $f16,8*8+4($sp) */
    .word               0xF7AE003C                  /* SDC1    $f14,7*8+4($sp) */
    .word               0xF7AC0034                  /* SDC1    $f12,6*8+4($sp) */
    .word               0xF7AA002C                  /* SDC1    $f10,5*8+4($sp) */
    .word               0xF7A80024                  /* SDC1    $f8,4*8+4($sp) */
    .word               0xF7A6001C                  /* SDC1    $f6,3*8+4($sp) */
    .word               0xF7A40014                  /* SDC1    $f4,2*8+4($sp) */
    .word               0xF7A2000C                  /* SDC1    $f2,1*8+4($sp) */
    .word               0xF7A00004                  /* SDC1    $f0,0*8+4($sp) */
    .word               0x4441F800                  /* CFC1    $1,RMP_CP1_FSCR */
    SW                  $1,0*4($sp)
    .endm
    
/* Restore FR32 extra registers **********************************************/
    .macro              RMP_MP32P_FR32_LOAD
    LW                  $1,0*4($sp)                 /* 16 DFPRs + FSCR */
    .word               0xD7A00004                  /* LDC1    $f0,0*8+4($sp) */
    .word               0x44C1F800                  /* CTC1    $1,RMP_CP1_FSCR - Avoid load delay */
    .word               0xD7A2000C                  /* LDC1    $f2,1*8+4($sp) */
    .word               0xD7A40014                  /* LDC1    $f4,2*8+4($sp) */
    .word               0xD7A6001C                  /* LDC1    $f6,3*8+4($sp) */
    .word               0xD7A80024                  /* LDC1    $f8,4*8+4($sp) */
    .word               0xD7AA002C                  /* LDC1    $f10,5*8+4($sp) */
    .word               0xD7AC0034                  /* LDC1    $f12,6*8+4($sp) */
    .word               0xD7AE003C                  /* LDC1    $f14,7*8+4($sp) */
    .word               0xD7B00044                  /* LDC1    $f16,8*8+4($sp) */
    .word               0xD7B2004C                  /* LDC1    $f18,9*8+4($sp) */
    .word               0xD7B40054                  /* LDC1    $f20,10*8+4($sp) */
    .word               0xD7B6005C                  /* LDC1    $f22,11*8+4($sp) */
    .word               0xD7B80064                  /* LDC1    $f24,12*8+4($sp) */
    .word               0xD7BA006C                  /* LDC1    $f26,13*8+4($sp) */
    .word               0xD7BC0074                  /* LDC1    $f28,14*8+4($sp) */
    .word               0xD7BE007C                  /* LDC1    $f30,15*8+4($sp) */
    ADDIU               $sp,$sp,16*8+4
    .endm
    
/* Save FR64 extra registers *************************************************/
    .macro              RMP_MP32P_FR64_SAVE
    ADDIU               $sp,$sp,-32*8-4             /* 32 DFPRs + FSCR */
    .word               0xF7BF00FC                  /* SDC1    $f31,31*8+4($sp) */
    .word               0xF7BE00F4                  /* SDC1    $f30,30*8+4($sp) */
    .word               0xF7BD00EC                  /* SDC1    $f29,29*8+4($sp) */
    .word               0xF7BC00E4                  /* SDC1    $f28,28*8+4($sp) */
    .word               0xF7BB00DC                  /* SDC1    $f27,27*8+4($sp) */
    .word               0xF7BA00D4                  /* SDC1    $f26,26*8+4($sp) */
    .word               0xF7B900CC                  /* SDC1    $f25,25*8+4($sp) */
    .word               0xF7B800C4                  /* SDC1    $f24,24*8+4($sp) */
    .word               0xF7B700BC                  /* SDC1    $f23,23*8+4($sp) */
    .word               0xF7B600B4                  /* SDC1    $f22,22*8+4($sp) */
    .word               0xF7B500AC                  /* SDC1    $f21,21*8+4($sp) */
    .word               0xF7B400A4                  /* SDC1    $f20,20*8+4($sp) */
    .word               0xF7B3009C                  /* SDC1    $f19,19*8+4($sp) */
    .word               0xF7B20094                  /* SDC1    $f18,18*8+4($sp) */
    .word               0xF7B1008C                  /* SDC1    $f17,17*8+4($sp) */
    .word               0xF7B00084                  /* SDC1    $f16,16*8+4($sp) */
    .word               0xF7AF007C                  /* SDC1    $f15,15*8+4($sp) */
    .word               0xF7AE0074                  /* SDC1    $f14,14*8+4($sp) */
    .word               0xF7AD006C                  /* SDC1    $f13,13*8+4($sp) */
    .word               0xF7AC0064                  /* SDC1    $f12,12*8+4($sp) */
    .word               0xF7AB005C                  /* SDC1    $f11,11*8+4($sp) */
    .word               0xF7AA0054                  /* SDC1    $f10,10*8+4($sp) */
    .word               0xF7A9004C                  /* SDC1    $f9,9*8+4($sp) */
    .word               0xF7A80044                  /* SDC1    $f8,8*8+4($sp) */
    .word               0xF7A7003C                  /* SDC1    $f7,7*8+4($sp) */
    .word               0xF7A60034                  /* SDC1    $f6,6*8+4($sp) */
    .word               0xF7A5002C                  /* SDC1    $f5,5*8+4($sp) */
    .word               0xF7A40024                  /* SDC1    $f4,4*8+4($sp) */
    .word               0xF7A3001C                  /* SDC1    $f3,3*8+4($sp) */
    .word               0xF7A20014                  /* SDC1    $f2,2*8+4($sp) */
    .word               0xF7A1000C                  /* SDC1    $f1,1*8+4($sp) */
    .word               0xF7A10004                  /* SDC1    $f1,0*8+4($sp) */
    .word               0x4441F800                  /* CFC1    $1,RMP_CP1_FSCR */
    SW                  $1,0*4($sp)
    .endm
    
/* Restore FR64 extra registers **********************************************/
    .macro              RMP_MP32P_FR64_LOAD
    .word               0xD7A00004                  /* LDC1    $f0,0*8+4($sp) */
    .word               0x44C1F800                  /* CTC1    $1,RMP_CP1_FSCR - Avoid load delay */
    .word               0xD7A1000C                  /* LDC1    $f1,1*8+4($sp) */
    .word               0xD7A20014                  /* LDC1    $f2,2*8+4($sp) */
    .word               0xD7A3001C                  /* LDC1    $f3,3*8+4($sp) */
    .word               0xD7A40024                  /* LDC1    $f4,4*8+4($sp) */
    .word               0xD7A5002C                  /* LDC1    $f5,5*8+4($sp) */
    .word               0xD7A60034                  /* LDC1    $f6,6*8+4($sp) */
    .word               0xD7A7003C                  /* LDC1    $f7,7*8+4($sp) */
    .word               0xD7A80044                  /* LDC1    $f8,8*8+4($sp) */
    .word               0xD7A9004C                  /* LDC1    $f9,9*8+4($sp) */
    .word               0xD7AA0054                  /* LDC1    $f10,10*8+4($sp) */
    .word               0xD7AB005C                  /* LDC1    $f11,11*8+4($sp) */
    .word               0xD7AC0064                  /* LDC1    $f12,12*8+4($sp) */
    .word               0xD7AD006C                  /* LDC1    $f13,13*8+4($sp) */
    .word               0xD7AE0074                  /* LDC1    $f14,14*8+4($sp) */
    .word               0xD7AF007C                  /* LDC1    $f15,15*8+4($sp) */
    .word               0xD7B00084                  /* LDC1    $f16,16*8+4($sp) */
    .word               0xD7B1008C                  /* LDC1    $f17,17*8+4($sp) */
    .word               0xD7B20094                  /* LDC1    $f18,18*8+4($sp) */
    .word               0xD7B3009C                  /* LDC1    $f19,19*8+4($sp) */
    .word               0xD7B400A4                  /* LDC1    $f20,20*8+4($sp) */
    .word               0xD7B500AC                  /* LDC1    $f21,21*8+4($sp) */
    .word               0xD7B600B4                  /* LDC1    $f22,22*8+4($sp) */
    .word               0xD7B700BC                  /* LDC1    $f23,23*8+4($sp) */
    .word               0xD7B800C4                  /* LDC1    $f24,24*8+4($sp) */
    .word               0xD7B900CC                  /* LDC1    $f25,25*8+4($sp) */
    .word               0xD7BA00D4                  /* LDC1    $f26,26*8+4($sp) */
    .word               0xD7BB00DC                  /* LDC1    $f27,27*8+4($sp) */
    .word               0xD7BC00E4                  /* LDC1    $f28,28*8+4($sp) */
    .word               0xD7BD00EC                  /* LDC1    $f29,29*8+4($sp) */
    .word               0xD7BE00F4                  /* LDC1    $f30,30*8+4($sp) */
    .word               0xD7BF00FC                  /* LDC1    $f31,31*8+4($sp) */
    ADDIU               $sp,$sp,32*8+4
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
