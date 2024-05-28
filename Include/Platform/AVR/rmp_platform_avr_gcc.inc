/******************************************************************************
Filename    : rmp_platform_avr_gcc.inc
Author      : pry
Date        : 22/05/2024
Description : The assembly part of the RMP RTOS. This is for AVR, and
              contains macros to be included in all interrupt assembly code
              as needed.
******************************************************************************/
    
/* Header ********************************************************************/
    .section            ".text.isr"
    .align              2

    /* CPU register definitions */
    .equ                RMP_RAMPD,0x38
    .equ                RMP_RAMPX,0x39
    .equ                RMP_RAMPY,0x3A
    .equ                RMP_RAMPZ,0x3B
    .equ                RMP_EIND,0x3C
    .equ                RMP_SPL,0x3D
    .equ                RMP_SPH,0x3E
    .equ                RMP_SREG,0x3F
    .equ                RMP_PMIC_CTRL,0xA2
/* End Header ****************************************************************/

/* Import ********************************************************************/
    /* The kernel stack and table */
    .extern             _RMP_AVR_SP_Kern
    /* The current thread stack */
    .extern             RMP_SP_Cur
    /* The interrupt active flag */
    .extern             RMP_AVR_Int_Act
    /* The yield pending flag */
    .extern             _RMP_AVR_Yield_Pend
    /* Extract highest priority running thread */
    .extern             _RMP_Run_High
    /* Handler for AVR timer interrupt */
    .extern             _RMP_AVR_Tim_Handler
/* End Import ****************************************************************/

/* Macro *********************************************************************/
/* Save all GP regs **********************************************************/
    .macro              RMP_AVR_SAVE
    PUSH                R31
    PUSH                R30
    PUSH                R29
    IN                  R29,RMP_SREG
    PUSH                R29
    PUSH                R28
    PUSH                R27
    PUSH                R26
    PUSH                R25
    PUSH                R24
    PUSH                R23
    PUSH                R22
    PUSH                R21
    PUSH                R20
    PUSH                R19
    PUSH                R18
    PUSH                R17
    PUSH                R16
    PUSH                R15
    PUSH                R14
    PUSH                R13
    PUSH                R12
    PUSH                R11
    PUSH                R10
    PUSH                R9
    PUSH                R8
    PUSH                R7
    PUSH                R6
    PUSH                R5
    PUSH                R4
    PUSH                R3
    PUSH                R2
    PUSH                R1
    PUSH                R0
    .endm

/* Actual context switch *****************************************************/
    .macro              RMP_AVR_SWITCH_PRE
    IN                  R19,RMP_SPH                 /* Save the SP to control block */
    IN                  R18,RMP_SPL
    STS                 RMP_SP_Cur+1,R19
    STS                 RMP_SP_Cur,R18
    LDS                 R19,_RMP_AVR_SP_Kern+1      /* Load SP for kernel  */
    LDS                 R18,_RMP_AVR_SP_Kern
    OUT                 RMP_SPH,R19
    OUT                 RMP_SPL,R18
    LDI                 R18,1                       /* Indicate interrupt active */
    STS                 RMP_AVR_Int_Act,R18
    CLR                 R1                          /* Clear implicit zero register */
    .endm

/* Actual context switch *****************************************************/
    .macro              RMP_AVR_SWITCH_POST
    LDS                 R18,_RMP_AVR_Yield_Pend
    CPI                 R18,0
    BREQ                1f
    CLR                 R18
    STS                 _RMP_AVR_Yield_Pend,R18
    CALL                _RMP_Run_High               /* No need to clean R1 again */
1:
    CLR                 R18
    STS                 RMP_AVR_Int_Act,R18
    LDS                 R19,RMP_SP_Cur+1            /* Load the SP from control block */
    LDS                 R18,RMP_SP_Cur
    OUT                 RMP_SPH,R19
    OUT                 RMP_SPL,R18
    .endm

/* Restore all GP regs *******************************************************/
    .macro              RMP_AVR_LOAD
    POP                 R0
    POP                 R1
    POP                 R2
    POP                 R3
    POP                 R4
    POP                 R5
    POP                 R6
    POP                 R7
    POP                 R8
    POP                 R9
    POP                 R10
    POP                 R11
    POP                 R12
    POP                 R13
    POP                 R14
    POP                 R15
    POP                 R16
    POP                 R17
    POP                 R18
    POP                 R19
    POP                 R20
    POP                 R21
    POP                 R22
    POP                 R23
    POP                 R24
    POP                 R25
    POP                 R26
    POP                 R27
    POP                 R28
    POP                 R29
    OUT                 RMP_SREG,R29
    POP                 R29
    POP                 R30
    POP                 R31
    RETI
    .endm

/* Save all RAMPs ************************************************************/
    .macro              RMP_AVR_RAMP_SAVE
    IN                  R21,RMP_RAMPZ
    IN                  R20,RMP_RAMPY
    IN                  R19,RMP_RAMPX
    IN                  R18,RMP_RAMPD
    PUSH                R21
    PUSH                R20
    PUSH                R19
    PUSH                R18
    CLR                 R18
    OUT                 RMP_RAMPD,R18
    OUT                 RMP_RAMPX,R18
    OUT                 RMP_RAMPY,R18
    OUT                 RMP_RAMPZ,R18
    .endm

/* Restore all RAMPs *********************************************************/
    .macro              RMP_AVR_RAMP_LOAD
    POP                 R18
    POP                 R19
    POP                 R20
    POP                 R21
    OUT                 RMP_RAMPD,R18
    OUT                 RMP_RAMPX,R19
    OUT                 RMP_RAMPY,R20
    OUT                 RMP_RAMPZ,R21
    .endm

/* Save EIND *****************************************************************/
    .macro              RMP_AVR_EIND_SAVE
    IN                  R18,RMP_EIND
    PUSH                R18
    CLR                 R18
    OUT                 RMP_EIND,R18
    .endm

/* Restore EIND **************************************************************/
    .macro              RMP_AVR_EIND_LOAD
    POP                 R18
    OUT                 RMP_EIND,R18
    .endm

/* Save macro for MegaAVR ****************************************************/
    .macro              RMP_AVR_INT_SAVE_MEGA
    RMP_AVR_SAVE
    RMP_AVR_SWITCH_PRE
    .endm

/* Restore macro for MegaAVR *************************************************/
    .macro              RMP_AVR_INT_LOAD_MEGA
    RMP_AVR_SWITCH_POST
    RMP_AVR_LOAD
    .endm

/* Save macro for MegaAVR with RAMP ******************************************/
    .macro              RMP_AVR_INT_SAVE_MEGA_RAMP
    RMP_AVR_SAVE
    RMP_AVR_RAMP_SAVE
    RMP_AVR_SWITCH_PRE
    .endm

/* Restore macro for MegaAVR with RAMP ***************************************/
    .macro              RMP_AVR_INT_LOAD_MEGA_RAMP
    RMP_AVR_SWITCH_POST
    RMP_AVR_RAMP_LOAD
    RMP_AVR_LOAD
    .endm

/* Save macro for MegaAVR with RAMP and EIND *********************************/
    .macro              RMP_AVR_INT_SAVE_MEGA_EIND
    RMP_AVR_SAVE
    RMP_AVR_RAMP_SAVE
    RMP_AVR_EIND_SAVE
    RMP_AVR_SWITCH_PRE
    .endm

/* Restore macro for MegaAVR with RAMP and EIND ******************************/
    .macro              RMP_AVR_INT_LOAD_MEGA_EIND
    RMP_AVR_SWITCH_POST
    RMP_AVR_EIND_LOAD
    RMP_AVR_RAMP_LOAD
    RMP_AVR_LOAD
    .endm

/* XMegaAVR macros are the same as MegaAVR macros ****************************/
    .macro              RMP_AVR_INT_SAVE_XMEGA
    RMP_AVR_INT_SAVE_MEGA
    .endm

    .macro              RMP_AVR_INT_LOAD_XMEGA
    RMP_AVR_INT_LOAD_MEGA
    .endm

    .macro              RMP_AVR_INT_SAVE_XMEGA_RAMP
    RMP_AVR_INT_SAVE_MEGA_RAMP
    .endm

    .macro              RMP_AVR_INT_LOAD_XMEGA_RAMP
    RMP_AVR_INT_LOAD_MEGA_RAMP
    .endm

    .macro              RMP_AVR_INT_SAVE_XMEGA_EIND
    RMP_AVR_INT_SAVE_MEGA_EIND
    .endm

    .macro                RMP_AVR_INT_LOAD_XMEGA_EIND
    RMP_AVR_INT_LOAD_MEGA_EIND
    .endm
/* End Macro *****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
