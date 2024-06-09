/******************************************************************************
Filename    : rmp_platform_dspic_gcc.s
Author      : pry
Date        : 10/04/2012
Description : The assembly part of the RMP RTOS, for DSPIC architectures.
              The DSPIC GCC toolchain mangles all symbols with an extra '_'.
******************************************************************************/
    
/* The DSPIC Architecture *****************************************************
Common registers:
No.         Bits        Name & Explanation
PC          24          Program counter.
SR          16          Status register.
CORCON      16          Core control register.
W0          16          Working register.
W1-W13      16          General purpose registers.
W14         16          Frame pointer.
W15         16          Stack pointer. Always word aligned.
SPLIM       16          Stack limit register (global - unswitched).
RCOUNT      16          Repeat loop counter.
TBLPAG      8           Data table page address.

Model-specific registers:
No.         Bits        Name & Explanation
ACCA        40          Accumulator A.
ACCB        40          Accumulator B.
PSVPAG      10          Program space read page address (older models).
DSRPAG      10          Data space read page address (newer models).
DSWPAG      9           Data space write page address (newer models).
DCOUNT      16          DO loop counter and stack (hardware - unswitched).
DOSTART     24          DO loop start address and stack (hardware - unswitched).
DOEND       24          DO loop end address and stack (hardware - unswitched).

Difference between CPUs: (all have PC, WREG, SR, SPLIM, RCOUNT, TBLPAG, CORCON)
24F_24H               PSVPAG
24E                          DSRPAG DSWPAG
30F_33F     ACCA ACCB PSVPAG               DCOUNT DOSTART DOEND
33E_33C     ACCA ACCB        DSRPAG DSWPAG DCOUNT DOSTART DOEND
******************************************************************************/

/* Import ********************************************************************/
    /* The real task switch handling function */
    .extern             __RMP_Run_High 
    /* The real systick handler function */
    .extern             __RMP_Tim_Handler
    /* The PID of the current thread */
    .extern             _RMP_Thd_Cur
    /* The stack address of current thread */
    .extern             _RMP_SP_Cur
    /* Kernel stack and table */
    .extern             __RMP_DSPIC_SP_Kern
    .extern             __RMP_DSPIC_CORCON_Kern
    .extern             __RMP_DSPIC_TBLPAG_Kern
    .extern             __RMP_DSPIC_DSRPAG_Kern
    .extern             __RMP_DSPIC_PSVDSWPAG_Kern
/* End Import ****************************************************************/

/* Export ********************************************************************/
    /* Disable all interrupts */
    .global             _RMP_Int_Disable      
    /* Enable all interrupts */
    .global             _RMP_Int_Enable
    /* Mask interrupts */
    .global             _RMP_Int_Mask   
    /* Get the MSB/LSB */
    .global             _RMP_DSPIC_MSB_Get
    .global             _RMP_DSPIC_LSB_Get
    /* Start the first thread */
    .global             __RMP_Start
    /* Yield to another thread */
    .global             __RMP_DSPIC_Yield_24F_24H
    .global             __RMP_DSPIC_Yield_24E
    .global             __RMP_DSPIC_Yield_30F_33F
    .global             __RMP_DSPIC_Yield_33E_33C
/* End Export ****************************************************************/

/* Header ********************************************************************/
    .text
    .align              4
/* End Header ****************************************************************/

/* Function:RMP_Int_Disable ***************************************************
Description    : The function for disabling all interrupts. This does not allow
                 nesting, and only disables interrupt for limited time; this is
                 a limitation of the DSPIC architecture. Considering the common
                 usage pattern of this function, this should be sufficient.
Input          : None.
Output         : None.    
Register Usage : None.                                  
******************************************************************************/
_RMP_Int_Disable:
    /* Disable all interrupts (for limited time) */
    DISI                #16383
    RETURN
/* End Function:RMP_Int_Disable **********************************************/

/* Function:RMP_Int_Enable ****************************************************
Description    : The function for enabling all interrupts. Can't directly 
                 operate on DISICNT; see errata. Have to use DISI #0.
Input          : None.
Output         : None.    
Register Usage : None.                                  
******************************************************************************/
_RMP_Int_Enable:
    /* Enable all interrupts */
    DISI                #0
    RETURN
/* End Function:RMP_Int_Enable ***********************************************/
    
/* Function:RMP_Int_Mask ******************************************************
Description    : Mask interrupts to a priority level.
Input          : [W0] - The priority level, 0 to 7.
Output         : None.    
Register Usage : None.                                  
******************************************************************************/
_RMP_Int_Mask:
    /* Mask interrupts below a certain IPL */
    MOV                 SR,W1
    SL			W0,#5,W0
    MOV                 #0xFF1F,W2
    AND                 W1,W2,W1
    IOR                 W1,W0,W1
    MOV                 W1,SR
    RETURN
/* End Function:RMP_Int_Mask *************************************************/

/* Function:_RMP_DSPIC_MSB_Get ************************************************
Description    : Get the MSB of the word.
Input          : ptr_t Value - The value.
Output         : None.
Return         : ptr_t - The MSB position.   
Register Usage : None. 
******************************************************************************/
_RMP_DSPIC_MSB_Get:
    FF1L                W0,W0
    SUBR                W0,#16,W0
    RETURN
/* End Function:_RMP_DSPIC_MSB_Get *******************************************/

/* Function:_RMP_DSPIC_LSB_Get ************************************************
Description    : Get the LSB of the word.
Input          : ptr_t Value - The value.
Output         : None.
Return         : ptr_t - The LSB position.   
Register Usage : None. 
******************************************************************************/
_RMP_DSPIC_LSB_Get:
    /* This word is non-zero, we need to find LSB */
    FF1R                W0,W0
    SUB                 #1,W0
    RETURN
/* End Function:_RMP_DSPIC_LSB_Get *******************************************/

/* Function:_RMP_Start ********************************************************
Description : Jump to the user function and will never return from it. This is
              compatible with both PSV and EDS, taking advantage of the fact
              that PSVPAG and DSWPAG are at the same address.
Input       : [W0]: Entry of the thread.
              [W1]: Stack of the thread.
Output      : None.
Return      : None.
******************************************************************************/
__RMP_Start:
    /* Save startup kernel states */
    MOV                 CORCON,W2
    MOV                 W2,__RMP_DSPIC_CORCON_Kern
    MOV                 TBLPAG,W2
    MOV                 W2,__RMP_DSPIC_TBLPAG_Kern
    /* Save startup page registers */
    MOV                 0x0032,W2   /* DSRPAG */
    MOV                 W2,__RMP_DSPIC_DSRPAG_Kern
    MOV                 0x0034,W2   /* PSVPAG or DSWPAG */
    MOV                 W2,__RMP_DSPIC_PSVDSWPAG_Kern
    /* Save startup kernel SP */
    MOV                 W15,__RMP_DSPIC_SP_Kern
    MOV                 W1,W15
    GOTO                W0
    /* Should not reach here */
/* End Function:_RMP_Start ***************************************************/

/* Function:_RMP_DSPIC_Yield **************************************************
Description : Yield from one thread to another without an interrupt.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
/* Save all GP regs **********************************************************/
    .macro              RMP_DSPIC_SAVE LABEL
    /* Mask IPL up to 1 which is the default */
    PUSH.D              W0
    MOV                 SR,W0
    MOV                 #0xFF1F,W1
    AND                 W0,W1,W0
    BSET                W0,#5
    MOV                 W0,SR
    /* Push SR with IPL=1 */
    PUSH                W0
    /* Restore W0/W1 but keep the hole for PC/SRL */
    MOV                 [W15-3*2],W0
    MOV                 [W15-2*2],W1
    /* Push GP regs to stack */
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
    /* Hack in the PC/SRL double word mimicking an interrupt entry */
    MOV                 #handle(\LABEL),W0
    MOV                 [W15-(18+1)*2],W1
    SL                  W1,#11,W1
    LSR                 W1,#3,W1
    MOV                 CORCON,W2
    /* PCL:SFA */
    CLR                 W3
    BTST.C              W2,#2
    BSW.C               W0,W3
    MOV                 W0,[W15-(2+18+1)*2]
    /* SRL:IPL3:PCH, where IPL=0 in the SRL */
    MOV                 #7,W3
    BTST.C              W2,#3
    BSW.C               W1,W3
    MOV                 W1,[W15-(1+18+1)*2]
    .endm

/* Actual context switch *****************************************************/
    .macro              RMP_DSPIC_SWITCH
    /* Switch to kernel stack */
    MOV                 W15,_RMP_SP_Cur
    MOV                 __RMP_DSPIC_SP_Kern,W15
    /* Get the highest priority ready task */
    CALL                __RMP_Run_High
    /* Switch back to user stack */
    MOV                 _RMP_SP_Cur,W15
    .endm
    
/* Restore all GP regs and simulate a RETFIE *********************************/
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

/* 24F & 24H *****************************************************************/
    .section            .text.__rmp_dspic_yield_24f_24h,code
__RMP_DSPIC_Yield_24F_24H:
    RMP_DSPIC_SAVE      __RMP_DSPIC_Yield_24F_24H_Skip
    PUSH                0x0034      /* PSVPAG */
    RMP_DSPIC_KERN_PSV
    RMP_DSPIC_SWITCH
    POP                 0x0034      /* PSVPAG */
    RMP_DSPIC_LOAD
__RMP_DSPIC_Yield_24F_24H_Skip:
    RETURN
    
/* 24E ***********************************************************************/
    .section            .text.__rmp_dspic_yield_24e,code
__RMP_DSPIC_Yield_24E:
    RMP_DSPIC_SAVE      __RMP_DSPIC_Yield_24E_Skip
    PUSH                0x0032      /* DSRPAG */
    PUSH                0x0034      /* DSWPAG */
    RMP_DSPIC_KERN_EDS
    RMP_DSPIC_SWITCH
    POP                 0x0034      /* DSWPAG */
    POP                 0x0032      /* DSRPAG */
    RMP_DSPIC_LOAD
__RMP_DSPIC_Yield_24E_Skip:
    RETURN

/* 30F & 33F *****************************************************************/
    .section            .text.__rmp_dspic_yield_30f_33f,code
__RMP_DSPIC_Yield_30F_33F:
    RMP_DSPIC_SAVE      __RMP_DSPIC_Yield_30F_33F_Skip
    PUSH                0x0034      /* PSVPAG */
    RMP_DSPIC_DSP_SAVE
    RMP_DSPIC_KERN_PSV
    RMP_DSPIC_SWITCH
    RMP_DSPIC_DSP_LOAD
    POP                 0x0034      /* PSVPAG */
    RMP_DSPIC_LOAD
__RMP_DSPIC_Yield_30F_33F_Skip:
    RETURN
    
/* 33E & 33C *****************************************************************/
    .section            .text.__rmp_dspic_yield_33e_33c,code
__RMP_DSPIC_Yield_33E_33C:
    RMP_DSPIC_SAVE      __RMP_DSPIC_Yield_33E_33C_Skip
    PUSH                0x0032      /* DSRPAG */
    PUSH                0x0034      /* DSWPAG */
    RMP_DSPIC_DSP_SAVE
    RMP_DSPIC_KERN_EDS
    RMP_DSPIC_SWITCH
    RMP_DSPIC_DSP_LOAD
    POP                 0x0034      /* DSWPAG */
    POP                 0x0032      /* DSRPAG */
    RMP_DSPIC_LOAD
__RMP_DSPIC_Yield_33E_33C_Skip:
    RETURN
/* End Function:_RMP_DSPIC_Yield *********************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
