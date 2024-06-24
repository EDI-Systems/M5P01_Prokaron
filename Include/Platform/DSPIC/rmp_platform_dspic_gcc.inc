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
    .align              4
/* End Header ****************************************************************/

/* Import ********************************************************************/
    /* The kernel stack and table */
    .extern             __RMP_DSPIC_SP_Kern
    .extern             __RMP_DSPIC_CORCON_Kern
    .extern             __RMP_DSPIC_TBLPAG_Kern
    .extern             __RMP_DSPIC_DSRPAG_Kern
    .extern             __RMP_DSPIC_PSVDSWPAG_Kern
    /* The current thread stack */
    .extern             _RMP_SP_Cur
    ;The yield pending flag
    .extern             _RMP_Sched_Pend
    ;Extract highest priority running thread
    .extern             __RMP_Run_High
    ;Handler for DSPIC timer interrupt
    .extern             __RMP_DSPIC_Tim_Handler
/* End Import ****************************************************************/

/* Macro *********************************************************************/
/* Save all GP regs **********************************************************/
    .macro              RMP_DSPIC_SAVE
    /* Push GP regs - SR should have IPL=1 already */
    PUSH                SR
    PUSH                CORCON
    PUSH.D              W0
    PUSH.D              W2
    PUSH.D              W4
    PUSH.D              W6
    PUSH.D              W8
    PUSH.D              W10
    PUSH.D              W12
    PUSH                W14
    PUSH                RCOUNT
    PUSH                TBLPAG
    .endm
    
/* Prepare for the context switch ********************************************/
    .macro              RMP_DSPIC_SWITCH_PRE
    /* Switch to kernel stack and table */
    MOV                 W15,_RMP_SP_Cur
    MOV                 __RMP_DSPIC_SP_Kern,W15
    MOV                 __RMP_DSPIC_TBLPAG_Kern,W2
    MOV                 W2,TBLPAG
    .endm

/* Finish up the context switch **********************************************/
    .macro              RMP_DSPIC_SWITCH_POST
    CP0                 _RMP_Sched_Pend
    BRA                 Z,1f
    CALL                __RMP_Run_High
1:
    /* Switch back to user stack */
    MOV                 _RMP_SP_Cur,W15
    .endm
    
/* Restore all GP regs *******************************************************/
    .macro              RMP_DSPIC_LOAD
    /* Pop GP regs */
    POP                 TBLPAG
    POP                 RCOUNT
    POP                 W14
    POP.D               W12
    POP.D               W10
    POP.D               W8
    POP.D               W6
    POP.D               W4
    POP.D               W2
    POP.D               W0
    POP                 CORCON
    POP                 SR
    RETFIE
    .endm

/* Kernel table loading for PSV **********************************************/
    .macro              RMP_DSPIC_KERN_PSV
    MOV                 __RMP_DSPIC_CORCON_Kern,W2
    MOV                 W2,CORCON
    MOV                 __RMP_DSPIC_TBLPAG_Kern,W2
    MOV                 W2,TBLPAG
    MOV                 __RMP_DSPIC_PSVDSWPAG_Kern,W2
    MOV                 W2,0x0034   /* PSVPAG */
    .endm
    
/* Kernel table loading for EDS **********************************************/
    .macro              RMP_DSPIC_KERN_EDS
    MOV                 __RMP_DSPIC_CORCON_Kern,W2
    MOV                 W2,CORCON
    MOV                 __RMP_DSPIC_TBLPAG_Kern,W2
    MOV                 W2,TBLPAG
    MOV                 __RMP_DSPIC_DSRPAG_Kern,W2
    MOV                 W2,0x0032   /* DSRPAG */
    MOV                 __RMP_DSPIC_PSVDSWPAG_Kern,W2
    MOV                 W2,0x0034   /* DSWPAG */
    .endm
    
/* DSP context save **********************************************************/
    .macro              RMP_DSPIC_DSP_SAVE
    PUSH                0x0022      /* ACCAL */
    PUSH                0x0024      /* ACCAH */
    PUSH                0x0026      /* ACCAU */
    PUSH                0x0028      /* ACCBL */
    PUSH                0x002A      /* ACCBH */
    PUSH                0x002C      /* ACCBU */
    PUSH                0x0046      /* MODCON */
    PUSH                0x0048      /* XMODSRT */
    PUSH                0x004A      /* XMODEND */
    PUSH                0x004C      /* YMODSRT */
    PUSH                0x004E      /* YMODEND */
    PUSH                0x0050      /* XBREV */
    .endm
    
/* DSP context restore *******************************************************/
    .macro              RMP_DSPIC_DSP_LOAD
    POP                 0x0050      /* XBREV */
    POP                 0x004E      /* YMODEND */
    POP                 0x004C      /* YMODSRT */
    POP                 0x004A      /* XMODEND */
    POP                 0x0048      /* XMODSRT */
    POP                 0x0046      /* MODCON */
    POP                 0x002C      /* ACCBU */
    POP                 0x002A      /* ACCBH */
    POP                 0x0028      /* ACCBL */
    POP                 0x0026      /* ACCAU */
    POP                 0x0024      /* ACCAH */
    POP                 0x0022      /* ACCAL */
    .endm
    
/* Save macro for 24F & 24H case *********************************************/
    .macro              RMP_DSPIC_INT_SAVE_24F_24H
    RMP_DSPIC_SAVE
    PUSH                0x0034      /* PSVPAG */
    RMP_DSPIC_KERN_PSV
    RMP_DSPIC_SWITCH_PRE
    .endm

/* Load macro for 24F & 24H case *********************************************/
    .macro              RMP_DSPIC_INT_LOAD_24F_24H
    RMP_DSPIC_SWITCH_POST
    POP                 0x0034      /* PSVPAG */
    RMP_DSPIC_LOAD
    .endm

/* Load macro for 24E case ***************************************************/
    .macro              RMP_DSPIC_INT_SAVE_24E
    RMP_DSPIC_SAVE
    PUSH                0x0032      /* DSRPAG */
    PUSH                0x0034      /* DSWPAG */
    RMP_DSPIC_KERN_EDS
    RMP_DSPIC_SWITCH_PRE
    .endm

/* Load macro for 24E case ***************************************************/
    .macro              RMP_DSPIC_INT_LOAD_24E
    RMP_DSPIC_SWITCH_POST
    POP                 0x0034      /* DSWPAG */
    POP                 0x0032      /* DSRPAG */
    RMP_DSPIC_LOAD
    .endm

/* Save macro for 30F & 33F case *********************************************/
    .macro              RMP_DSPIC_INT_SAVE_30F_33F
    RMP_DSPIC_SAVE
    PUSH                0x0034      /* PSVPAG */
    RMP_DSPIC_DSP_SAVE
    RMP_DSPIC_KERN_PSV
    RMP_DSPIC_SWITCH_PRE
    .endm

/* Load macro for 30F & 33F case *********************************************/
    .macro              RMP_DSPIC_INT_LOAD_30F_33F
    RMP_DSPIC_SWITCH_POST
    RMP_DSPIC_DSP_LOAD
    POP                 0x0034      /* PSVPAG */
    RMP_DSPIC_LOAD
    .endm

/* Save macro for 33E & 33C case *********************************************/
    .macro              RMP_DSPIC_INT_SAVE_33E_33C
    RMP_DSPIC_SAVE
    PUSH                0x0032      /* DSRPAG */
    PUSH                0x0034      /* DSWPAG */
    RMP_DSPIC_DSP_SAVE
    RMP_DSPIC_KERN_EDS
    RMP_DSPIC_SWITCH_PRE
    .endm

/* Load macro for 33E & 33C case *********************************************/
    .macro              RMP_DSPIC_INT_LOAD_33E_33C
    RMP_DSPIC_SWITCH_POST
    RMP_DSPIC_DSP_LOAD
    POP                 0x0034      /* DSWPAG */
    POP                 0x0032      /* DSRPAG */
    RMP_DSPIC_LOAD
    .endm
/* End Macro *****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
