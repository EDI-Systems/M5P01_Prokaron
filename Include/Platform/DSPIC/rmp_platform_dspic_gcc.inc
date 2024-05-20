/******************************************************************************
Filename    : rmp_platform_dspic_gcc.inc
Author      : pry
Date        : 10/04/2012
Description : The assembly part of the RMP RTOS. This is for DSPIC, and
              contains macros to be included in all interrupt assembly code
              as needed.
******************************************************************************/
    
/* Header ********************************************************************/
    .text
    .align 4
/* End Header ****************************************************************/

/* Import ********************************************************************/
    /* The kernel stack and variables */
    .extern             __RMP_DSPIC_SP_Kern
    .extern             __RMP_DSPIC_TBLPAG_Kern
    .extern             __RMP_DSPIC_DSRPAG_Kern
    .extern             __RMP_DSPIC_DSWPAG_Kern
    /* The current thread stack */
    .extern             _RMP_SP_Cur
    ;The interrupt active flag
    .extern             _RMP_DSPIC_Int_Act
    ;The yield pending flag
    .extern             __RMP_DSPIC_Yield_Pend
    ;Extract highest priority running thread
    .extern             __RMP_Run_High
    ;Handler for DSPIC timer interrupt
    .extern             __RMP_DSPIC_Tim_Handler
/* End Import ****************************************************************/

/* Macro *********************************************************************/
/* Push everything to stack **************************************************/
.macro RMP_DSPIC_INT_SAVE
    /* Push GP regs - SR should have IPL=1 already */
    PUSH                SR
    PUSH.D              W0
    PUSH.D              W2
    PUSH.D              W4
    PUSH.D              W6
    PUSH.D              W8
    PUSH.D              W10
    PUSH.D              W12
    PUSH                W14
    /* Push extended regs */
    PUSH                ACCAL
    PUSH                ACCAH
    PUSH                ACCAU
    PUSH                ACCBL
    PUSH                ACCBH
    PUSH                ACCBU
    PUSH                DSRPAG
    PUSH                DSWPAG
    PUSH                RCOUNT
    PUSH                DCOUNT
    PUSH                DOSTARTL
    PUSH                DOSTARTH
    PUSH                DOENDL
    PUSH                DOENDH
    PUSH                CORCON
    PUSH                MODCON
    PUSH                XMODSRT
    PUSH                XMODEND
    PUSH                YMODSRT
    PUSH                YMODEND
    PUSH                XBREV
    PUSH                TBLPAG
    
    /* Switch to kernel stack */
    MOV                 W15,_RMP_SP_Cur
    MOV                 __RMP_DSPIC_SP_Kern,W15
    /* Load kernel global pointers */
    MOV                 __RMP_DSPIC_TBLPAG_Kern,W2
    MOV                 W2,TBLPAG
    MOV                 __RMP_DSPIC_DSRPAG_Kern,W2
    MOV                 W2,DSRPAG
    MOV                 __RMP_DSPIC_DSWPAG_Kern,W2
    MOV                 W2,DSWPAG
    /* Indicate interrupt active */
    MOV                 #1,W0
    MOV                 W0,_RMP_DSPIC_Int_Act
.endm
    
/* Load everything from stack, and deal with pending yields if there is ******/
.macro RMP_DSPIC_INT_RESTORE
    CP0                 __RMP_DSPIC_Yield_Pend
    BRA                 Z,1f
    CLR                 __RMP_DSPIC_Yield_Pend
    CALL                __RMP_Run_High
1:
    CLR                 _RMP_DSPIC_Int_Act
    /* Switch back to user stack */
    MOV                 _RMP_SP_Cur,W15
    /* Pop extended regs */
    POP                 TBLPAG
    POP                 XBREV
    POP                 YMODEND
    POP                 YMODSRT
    POP                 XMODEND
    POP                 XMODSRT
    POP                 MODCON
    POP                 CORCON
    POP                 DOENDH
    POP                 DOENDL
    POP                 DOSTARTH
    POP                 DOSTARTL
    POP                 DCOUNT
    POP                 RCOUNT
    POP                 DSWPAG
    POP                 DSRPAG
    POP                 ACCBU
    POP                 ACCBH
    POP                 ACCBL
    POP                 ACCAU
    POP                 ACCAH
    POP                 ACCAL
    /* Pop GP regs */
    POP                 W14
    POP.D               W12
    POP.D               W10
    POP.D               W8
    POP.D               W6
    POP.D               W4
    POP.D               W2
    POP.D               W0
    POP                 SR
    RETFIE
.endm
/* End Macro *****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
