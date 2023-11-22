/******************************************************************************
Filename    : rmp_platform_rv32p_gcc.inc
Author      : pry
Date        : 10/04/2012
Description : The assembly part of the RMP RTOS. This is for RV32P, and
              contains macros to be included in all interrupt assembly code
              as needed.
******************************************************************************/

/* Begin Import **************************************************************/
    /* The stack address of current thread */
    .global             RMP_SP_Cur
    /* The interrupt active flag */
    .global             RMP_RV32P_Int_Act
    /* The yield pending flag */
    .global             _RMP_RV32P_Yield_Pend
    /* Save and load extra contexts, such as FPU, peripherals and MPU */
    .global             RMP_Ctx_Save
    .global             RMP_Ctx_Load
    /* Extract highest priority running thread */
    .global             _RMP_Run_High
    /* Handler for MSP430 timer interrupt */
    .global             _RMP_RV32P_Tim_Handler
/* End Import ****************************************************************/

/* Begin Macro ***************************************************************/
    /* Save all GP regs */
    .macro              RMP_RV32P_REG_SAVE
    ADDI                sp,sp,-32*4
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
    SW                  x1,2*4(sp)
    CSRR                a0,mepc
    SW                  a0,1*4(sp)
    CSRR                a0,mstatus
    .endm

    /* Prepare for a context switch */
    .macro              RMP_RV32P_SWITCH_PRE
    SW                  a0,0*4(sp)
    .option             push
    .option             norelax
    LA                  gp,_RMP_Global
    .option             pop
    LA                  a0,RMP_SP_Cur
    SW                  sp,(a0)
    LA                  sp,_RMP_Stack
    LA                  a0,RMP_RV32P_Int_Act
    LI                  a1,1
    SW                  a1,(a0)
    .endm

    /* Finish up the context switch */
    .macro              RMP_RV32P_SWITCH_POST
    LA                  a0,_RMP_RV32P_Yield_Pend
    LW                  a1,(a0)
    BEQZ                a1,1f
    SW                  zero,(a0)
    CALL                _RMP_Run_High
1:
    LA                  a0,RMP_RV32P_Int_Act
    SW                  zero,(a0)
    LA                  a0,RMP_SP_Cur
    LW                  sp,(a0)
    LW                  a0,0*4(sp)
    .endm

    /* Restore all GP regs */
    .macro              RMP_RV32P_REG_RESTORE
    LI                  a1,0x1880
    OR                  a0,a0,a1
    CSRW                mstatus,a0
    LW                  a0,1*4(sp)
    CSRW                mepc,a0
    LW                  x1,2*4(sp)
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
    .endm

    /* Save macro for no coprocessor case */
    .macro              RMP_RV32P_INT_SAVE_NONE
    RMP_RV32P_REG_SAVE
    RMP_RV32P_SWITCH_PRE
    .endm

    /* Load macro for no coprocessor case */
    .macro              RMP_RV32P_INT_RESTORE_NONE
    RMP_RV32P_SWITCH_POST
    RMP_RV32P_REG_RESTORE
    .endm

    /* Save macro for RVF case */
    .macro              RMP_RV32P_INT_SAVE_RVF
    RMP_RV32P_REG_SAVE
    LUI                 a1,4
    AND                 a1,a1,a0
    BEQZ                a1,1f
    ADDI                sp,sp,-33*4
    .hword              0x25F3      /* FRCSR   a1 */
    .hword              0x0030
    SW                  a1,32*4(sp)
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
1:
    RMP_RV32P_SWITCH_PRE
    .endm

    /* Load macro for RVF case */
    .macro              RMP_RV32P_INT_RESTORE_RVF
    RMP_RV32P_SWITCH_POST
    LUI                 a1,4
    AND                 a1,a1,a0
    BEQZ                a1,1f
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
    LW                  a1,32*4(sp)
    .hword              0x9073      /* FSCSR   a1 */
    .hword              0x0035
    ADDI                sp,sp,33*4
1:
    RMP_RV32P_REG_RESTORE
    .endm

    /* Save macro for RVFD case */
    .macro              RMP_RV32P_INT_SAVE_RVFD
    RMP_RV32P_REG_SAVE
    LUI                 a1,4
    AND                 a1,a1,a0
    BEQZ                a1,1f
    ADDI                sp,sp,-65*4
    .hword              0x25F3      /* FRCSR   a1 */
    .hword              0x0030
    SW                  a1,32*8(sp)
    .hword              0x3C27      /* FSD     f31,31*8(sp) */
    .hword              0x0FF1
    .hword              0x3827      /* FSD     f30,30*8(sp) */
    .hword              0x0FE1
    .hword              0x3427      /* FSD     f29,29*8(sp) */
    .hword              0x0FD1
    .hword              0x3027      /* FSD     f28,28*8(sp) */
    .hword              0x0FC1
    .hword              0x3C27      /* FSD     f27,27*8(sp) */
    .hword              0x0DB1
    .hword              0x3827      /* FSD     f26,26*8(sp) */
    .hword              0x0DA1
    .hword              0x3427      /* FSD     f25,25*8(sp) */
    .hword              0x0D91
    .hword              0x3027      /* FSD     f24,24*8(sp) */
    .hword              0x0D81
    .hword              0x3C27      /* FSD     f23,23*8(sp) */
    .hword              0x0B71
    .hword              0x3827      /* FSD     f22,22*8(sp) */
    .hword              0x0B61
    .hword              0x3427      /* FSD     f21,21*8(sp) */
    .hword              0x0B51
    .hword              0x3027      /* FSD     f20,20*8(sp) */
    .hword              0x0B41
    .hword              0x3C27      /* FSD     f19,19*8(sp) */
    .hword              0x0931
    .hword              0x3827      /* FSD     f18,18*8(sp) */
    .hword              0x0921
    .hword              0x3427      /* FSD     f17,17*8(sp) */
    .hword              0x0911
    .hword              0x3027      /* FSD     f16,16*8(sp) */
    .hword              0x0901
    .hword              0x3C27      /* FSD     f15,15*8(sp) */
    .hword              0x06F1
    .hword              0x3827      /* FSD     f14,14*8(sp) */
    .hword              0x06E1
    .hword              0x3427      /* FSD     f13,13*8(sp) */
    .hword              0x06D1
    .hword              0x3027      /* FSD     f12,12*8(sp) */
    .hword              0x06C1
    .hword              0x3C27      /* FSD     f11,11*8(sp) */
    .hword              0x04B1
    .hword              0x3827      /* FSD     f10,10*8(sp) */
    .hword              0x04A1
    .hword              0x3427      /* FSD     f9,9*8(sp) */
    .hword              0x0491
    .hword              0x3027      /* FSD     f8,8*8(sp) */
    .hword              0x0481
    .hword              0x3C27      /* FSD     f7,7*8(sp) */
    .hword              0x0271
    .hword              0x3827      /* FSD     f6,6*8(sp) */
    .hword              0x0261
    .hword              0x3427      /* FSD     f5,5*8(sp) */
    .hword              0x0251
    .hword              0x3027      /* FSD     f4,4*8(sp) */
    .hword              0x0241
    .hword              0x3C27      /* FSD     f3,3*8(sp) */
    .hword              0x0031
    .hword              0x3827      /* FSD     f2,2*8(sp) */
    .hword              0x0021
    .hword              0x3427      /* FSD     f1,1*8(sp) */
    .hword              0x0011
    .hword              0x3027      /* FSD     f0,0*8(sp) */
    .hword              0x0001
1:
    RMP_RV32P_SWITCH_PRE
    .endm

    /* Load macro for RVFD case */
    .macro              RMP_RV32P_INT_RESTORE_RVFD
    RMP_RV32P_SWITCH_POST
    LUI                 a1,4
    AND                 a1,a1,a0
    BEQZ                a1,1f
    .hword              0x3007      /* FLD     f0,0*8(sp) */
    .hword              0x0001
    .hword              0x3087      /* FLD     f1,1*8(sp) */
    .hword              0x0081
    .hword              0x3107      /* FLD     f2,2*8(sp) */
    .hword              0x0101
    .hword              0x3187      /* FLD     f3,3*8(sp) */
    .hword              0x0181
    .hword              0x3207      /* FLD     f4,4*8(sp) */
    .hword              0x0201
    .hword              0x3287      /* FLD     f5,5*8(sp) */
    .hword              0x0281
    .hword              0x3307      /* FLD     f6,6*8(sp) */
    .hword              0x0301
    .hword              0x3387      /* FLD     f7,7*8(sp) */
    .hword              0x0381
    .hword              0x3407      /* FLD     f8,8*8(sp) */
    .hword              0x0401
    .hword              0x3487      /* FLD     f9,9*8(sp) */
    .hword              0x0481
    .hword              0x3507      /* FLD     f10,10*8(sp) */
    .hword              0x0501
    .hword              0x3587      /* FLD     f11,11*8(sp) */
    .hword              0x0581
    .hword              0x3607      /* FLD     f12,12*8(sp) */
    .hword              0x0601
    .hword              0x3687      /* FLD     f13,13*8(sp) */
    .hword              0x0681
    .hword              0x3707      /* FLD     f14,14*8(sp) */
    .hword              0x0701
    .hword              0x3787      /* FLD     f15,15*8(sp) */
    .hword              0x0781
    .hword              0x3807      /* FLD     f16,16*8(sp) */
    .hword              0x0801
    .hword              0x3887      /* FLD     f17,17*8(sp) */
    .hword              0x0881
    .hword              0x3907      /* FLD     f18,18*8(sp) */
    .hword              0x0901
    .hword              0x3987      /* FLD     f19,19*8(sp) */
    .hword              0x0981
    .hword              0x3A07      /* FLD     f20,20*8(sp) */
    .hword              0x0A01
    .hword              0x3A87      /* FLD     f21,21*8(sp) */
    .hword              0x0A81
    .hword              0x3B07      /* FLD     f22,22*8(sp) */
    .hword              0x0B01
    .hword              0x3B87      /* FLD     f23,23*8(sp) */
    .hword              0x0B81
    .hword              0x3C07      /* FLD     f24,24*8(sp) */
    .hword              0x0C01
    .hword              0x3C87      /* FLD     f25,25*8(sp) */
    .hword              0x0C81
    .hword              0x3D07      /* FLD     f26,26*8(sp) */
    .hword              0x0D01
    .hword              0x3D87      /* FLD     f27,27*8(sp) */
    .hword              0x0D81
    .hword              0x3E07      /* FLD     f28,28*8(sp) */
    .hword              0x0E01
    .hword              0x3E87      /* FLD     f29,29*8(sp) */
    .hword              0x0E81
    .hword              0x3F07      /* FLD     f30,30*8(sp) */
    .hword              0x0F01
    .hword              0x3F87      /* FLD     f31,31*8(sp) */
    .hword              0x0F81
    LW                  a1,32*8(sp)
    .hword              0x9073      /* FSCSR   a1 */
    .hword              0x0035
    ADDI                sp,sp,65*4
1:
    RMP_RV32P_REG_RESTORE
    .endm
/* End Macro *****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
