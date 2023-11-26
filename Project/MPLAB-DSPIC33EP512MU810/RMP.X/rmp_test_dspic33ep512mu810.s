/******************************************************************************
Filename    : rmp_test_dspic33ep512mu810.s
Author      : pry
Date        : 10/04/2012
Description : The extra testing file for this chip.
******************************************************************************/
    
/* Header ********************************************************************/
    .text
    .align 4
/* End Header ****************************************************************/

/* Import ********************************************************************/
    /* The place where we store the kernel sp value */
    .extern             _RMP_SP_Val
    /* Kernel constant pool global pointers */
    .extern             _RMP_TBLPAG_Val;
    .extern             _RMP_DSRPAG_Val;
    .extern             _RMP_DSWPAG_Val;;
    /* The interrupt nesting value */
    .extern             _RMP_Int_Nest
    /* The extra routine */
    .extern             _Tim3_Interrupt    
    .global             __T3Interrupt
/* End Import ****************************************************************/

/* Macros ********************************************************************/
    /* Context saving - not all registers are present in all series. */
.macro SAVE_CONTEXT
    /* Push everything to stack */
    PUSH                SR
    /* Disable all interrupts */
    DISI                #16383
    PUSH.D              W0
    PUSH.D              W2
    PUSH.D              W4
    PUSH.D              W6
    PUSH.D              W8
    PUSH.D              W10
    PUSH.D              W12
    PUSH                W14
    /* We don't use SPLIM with OS, it is always untouched */
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
    PUSH                MSTRPR
    /* Not present in PIC33 */
    /* PUSH                PSVPAG */
    /* See if we need to switch to kernel stack */
    MOV                 _RMP_Int_Nest,W1
    MOV                 #1,W2
    ADD                 W1,W2,W1
    MOV                 W1,_RMP_Int_Nest
    CPBNE               W1,W2,1f
    /* We have confirmed that we need to switch to kernel stack */
    MOV                 W15,_RMP_SP_Cur
    MOV                 _RMP_SP_Val,W15
    /* Enable interrupt */
1:  DISI                #0
    /* Load kernel global pointers */
    MOV                 _RMP_TBLPAG_Val,W2
    MOV                 W2,TBLPAG
    MOV                 _RMP_DSRPAG_Val,W2
    MOV                 W2,DSRPAG
    MOV                 _RMP_DSWPAG_Val,W2
    MOV                 W2,DSWPAG
.endm
    
    /* Context restoring */
.macro LOAD_CONTEXT
    /* Disable all interrupts and see if we need to switch back to user stack */
    DISI                #16383
    MOV                 _RMP_Int_Nest,W1
    MOV                 #1,W2
    CPBNE               W1,W2,1f
    /* We need to switch back to our user stack */
    MOV                 _RMP_SP_Cur,W15
1:  SUB                 W1,W2,W1
    MOV                 W1,_RMP_Int_Nest
    /* Pop everything from stack */
    /* POP                 PSVPAG */
    POP                 MSTRPR
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
    /* We don't use SPLIM with OS, it is always untouched */
    POP                 W14
    POP.D               W12
    POP.D               W10
    POP.D               W8
    POP.D               W6
    POP.D               W4
    POP.D               W2
    POP.D               W0
    /* Enable interrupt then pop SR */
    DISI                #0
    POP                 SR
    RETFIE
.endm
/* End Macros ****************************************************************/

/* Function:__T3Interrupt *****************************************************
Description : The timer 3 interrupt routine.
Input       : None.
Output      : None.                                      
******************************************************************************/
__T3Interrupt:
    SAVE_CONTEXT
                
    CALL                _Tim3_Interrupt
    
    LOAD_CONTEXT
    .end
/* End Function:__T3Interrupt ************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
