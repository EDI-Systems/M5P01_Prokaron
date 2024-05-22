/******************************************************************************
Filename    : rmp_platform_avr_gcc.s
Author      : pry
Date        : 10/04/2012
Description : The assembly part of the RMP RTOS. This is for Cortex-M0/0+/1.
******************************************************************************/

/* The Atmel (Microchip) AVR Architecture *************************************
R0-R31  : General purpose registers that are accessible. 
RAMPD   : D address upper register.
RAMPX   : X address upper register.
RAMPY   : Y address upper register.
RAMPZ   : Z address upper register.
SP      : Stack pointer
PC      : Program counter.
SREG    : Status Register.
******************************************************************************/

/* Import ********************************************************************/
    /* The real task switch handling function */
    .extern             _RMP_Run_High
    /* The real systick handler function */
    .extern             _RMP_Tim_Handler
    /* The PID of the current thread */
    .extern             RMP_Thd_Cur
    /* The stack address of current thread */
    .extern             RMP_SP_Cur
    /* Kernel stack and table */
    .extern             _RMP_AVR_SP_Kern
/* End Import ****************************************************************/

/* Export ********************************************************************/
    /* Disable all interrupts */
    .global             RMP_Int_Disable
    /* Enable all interrupts */
    .global             RMP_Int_Enable
    /* Mask/unmask interrupt dummy */
    .global             RMP_Int_Mask
    /* Start the first thread */
    .global             _RMP_Start
    /* The PendSV trigger */
    .global             _RMP_AVR_Yield_NONE
    .global             _RMP_AVR_Yield_RAMPZ
    .global             _RMP_AVR_Yield_XMEGA
/* End Export ****************************************************************/

/* Header ********************************************************************/
    .section            ".text.arch"
    .align              2

    /* CPU register definitions */
    .equ                RAMPD,0x38
    .equ                RAMPX,0x39
    .equ                RAMPY,0x3A
    .equ                RAMPZ,0x3B
    .equ                EIND,0x3C
    .equ                SPL,0x3D
    .equ                SPH,0x3E
    .equ                SREG,0x3F
    .equ                PMIC_CTRL,0xA2
/* End Header ****************************************************************/

/* Function:RMP_Int_Disable ***************************************************
Description : The function for disabling all interrupts. Does not allow nesting.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
RMP_Int_Disable:
    CLI
    RET
/* End Function:RMP_Int_Disable **********************************************/

/* Function:RMP_Int_Enable ****************************************************
Description : The function for enabling all interrupts. Does not allow nesting.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
RMP_Int_Enable:
    SEI
    RET
/* End Function:RMP_Int_Enable ***********************************************/

/* Function:RMP_Int_Mask ******************************************************
Description : Set the PMIC mask for XMEGA.
Input       : R24 - The last 3 bit mask for the PMIC.
Output      : None.
Return      : None.
******************************************************************************/
RMP_Int_Mask:
    LDI                 R30,hi8(PMIC_CTRL)
    LDI                 R31,lo8(PMIC_CTRL)
    LD                  R18,Z
    ANDI                R18,0xF8
    OR                  R18,R24
    ST                  Z,R18
    RET
/* End Function:RMP_Int_Enable ***********************************************/

/* Function:_RMP_Start ********************************************************
Description : Jump to the user function and will never return from it.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
_RMP_Start:
/* End Function:_RMP_Start ***************************************************/

/* Function:_RMP_Yield ********************************************************
Description : Trigger a yield to another thread.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
/* Save all GP regs **********************************************************/
    /* AVR is special in the sense that the CALL and interrupt entry perform
     * exactly the same pushing behavior, hence we can omit the PC push/skip
     * altogether. The interrupt entry-exit behavior is also interesting: the
     * MEGA disables interrupt upon entry and enables it upon RETI, while the
     * XMEGA uses a more modern interrupt prioritization scheme where the global
     * interrupt stays enabled and the PMIC tracks the interrupt status. The 
     * interrupt is not disabled upon entry, and is not enabled upon RETI;
     * The RETI will restore PMIC state in addition to the RET behavior.
     * Hence, for MEGA, we use CLI/RETI pair, while for XMEGA we leverage
     * its PMIC masking features, and we assume all kernel-aware interrupts
     * are low-level interrupts in XMEGA. */
    .macro              RMP_AVR_SAVE
    /* R31-R29 are handled elsewhere because we need some temporaries */
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
    IN                  R18,SREG
    PUSH                R18
    .endm

/* Actual context switch *****************************************************/
    .macro              RMP_AVR_SWITCH
    IN                  R18,SPL             /* Save the sp to control block */
    IN                  R19,SPH
    LDI                 R28,lo8(RMP_SP_Cur) /* Y[29:28] is Callee-save */
    LDI                 R29,hi8(RMP_SP_Cur)
    ST                  Y,R0
    STD                 Y+1,R1
    LDI                 R30,lo8(_RMP_AVR_SP_Kern)   /* Load sp for kernel  */
    LDI                 R31,hi8(_RMP_AVR_SP_Kern)
    LD                  R18,Z
    LDD                 R19,Z+1
    OUT                 SPL,R18
    OUT                 SPH,R19
    LDI                 R30,lo8(_RMP_Run_High)  /* Get the highest ready task */
    LDI                 R31,hi8(_RMP_Run_High)
    ICALL                                   /* Use ICALL to remain compatible */
    LD                  R18,Y               /* Y[29:28] is Callee-save */
    LDD                 R19,Y+1
    OUT                 SPL,R18
    OUT                 SPH,R19
    .endm

/* Restore all GP regs *******************************************************/
    .macro              RMP_AVR_LOAD
    POP                 R18
    OUT                 SREG,R18
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
    .endm

/* Normal MegaAVR ************************************************************/
    .section            .text._rmp_avr_yield_none
    .align              2
_RMP_AVR_Yield_NONE:
    CLI
    PUSH                R31
    PUSH                R30
    PUSH                R29
    RMP_AVR_SAVE
    RMP_AVR_SWITCH
    RMP_AVR_LOAD
    POP                 R29
    POP                 R30
    POP                 R31
    RETI

/* MegaAVR that have RAMPZ register  ****************************************/
    .section            .text._rmp_avr_yield_rampz
    .align              2
_RMP_AVR_Yield_RAMPZ:
    CLI
    PUSH                R31
    PUSH                R30
    PUSH                R29
    RMP_AVR_SAVE
    IN                  R18,RAMPZ
    PUSH                R18
    RMP_AVR_SWITCH
    POP                 R18
    OUT                 RAMPZ,R18
    RMP_AVR_LOAD
    POP                 R29
    POP                 R30
    POP                 R31
    RETI
    
/* XMegaAVR that have PMIC and loads of RAMP registers  **********************/
    .section            .text._rmp_avr_yield_xmega
    .align              2
_RMP_AVR_Yield_XMEGA:
    /* Push temporaries */
    PUSH                R31
    PUSH                R30
    PUSH                R29
    /* Mask all low-level interrupts */
    LDI                 R30,hi8(PMIC_CTRL)
    LDI                 R31,lo8(PMIC_CTRL)
    LD                  R29,Z
    ANDI                R29,0xFE
    ST                  Z,R29
    RMP_AVR_SAVE
    IN                  R22,EIND
    IN                  R21,RAMPZ
    IN                  R20,RAMPY
    IN                  R19,RAMPX
    IN                  R18,RAMPD
    PUSH                R22
    PUSH                R21
    PUSH                R20
    PUSH                R19
    PUSH                R18
    RMP_AVR_SWITCH
    POP                 R18
    POP                 R19
    POP                 R20
    POP                 R21
    POP                 R22
    OUT                 RAMPD,R18
    OUT                 RAMPX,R19
    OUT                 RAMPY,R20
    OUT                 RAMPZ,R21
    OUT                 EIND,R22
    RMP_AVR_LOAD
    /* Unmask all low-level interrupts */
    LDI                 R30,hi8(PMIC_CTRL)
    LDI                 R31,lo8(PMIC_CTRL)
    LD                  R29,Z
    ORI                 R29,0x01
    ST                  Z,R29
    /* Pop temporaries */
    POP                 R29
    POP                 R30
    POP                 R31
    /* Use RET instead because we don't want to mess with PMIC */
    RET
/* End Function:_RMP_Yield ***************************************************/
    .end
/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
