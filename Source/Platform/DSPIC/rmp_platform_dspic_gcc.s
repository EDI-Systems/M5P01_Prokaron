/******************************************************************************
Filename    : rmp_platform_dspic_gcc.s
Author      : pry
Date        : 10/04/2012
Description : The assembly part of the RMP RTOS, for DSPIC architectures.
******************************************************************************/
    
/* The DSPIC 33E Architecture *************************************************
No.         Bits        Name & Explanation
W0          16          Working register.
W1-W13      16          General purpose registers.
W14         16          Frame pointer.
W15         16          Stack pointer. Always word aligned.
SPLIM       16          Stack limit register.
ACCA        40          Accumulator A.
ACCB        40          Accumulator B.
PC          24          Program counter.
TBLPAG      8           Data table page address.
DSRPAG      10          Data space read page address. (DSP24 parts have PSVPAG)
DSWPAG      9           Data space write page address.
RCOUNT      16          Repeat loop counter.
DCOUNT      16          DO loop counter and stack.(DSP33EPxx806/810/814 only)
DOSTART     24          DO loop start address and stack.(DSP33EPxx806/810/814 only)
DOEND       24          DO loop end address and stack.(DSP33EPxx806/810/814 only)
CORCON      16          Core control register.
STATUS      16          Status register.
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
    /* Kernel values */
    .extern             __RMP_DSPIC_SP_Kern
    .extern             __RMP_DSPIC_TBLPAG_Kern
    .extern             __RMP_DSPIC_DSRPAG_Kern
    .extern             __RMP_DSPIC_DSWPAG_Kern
/* End Import ****************************************************************/

/* Export ********************************************************************/
    /* The DSPIC toolchain mangles all symbols with an extra '_' */
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
    .global             __RMP_DSPIC_Yield
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
Description : Jump to the user function and will never return from it.
Input       : [W0]: Entry of the thread.
              [W1]: Stack of the thread.
Output      : None.
Return      : None.
******************************************************************************/
__RMP_Start:
    MOV                 TBLPAG,W2
    MOV                 W2,__RMP_DSPIC_TBLPAG_Kern
    MOV                 DSRPAG,W2
    MOV                 W2,__RMP_DSPIC_DSRPAG_Kern
    MOV                 DSWPAG,W2
    MOV                 W2,__RMP_DSPIC_DSWPAG_Kern
    /* Save this SP for kernel use */
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
__RMP_DSPIC_Yield:
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
    PUSH.D              W0
    PUSH.D              W2
    PUSH.D              W4
    PUSH.D              W6
    PUSH.D              W8
    PUSH.D              W10
    PUSH.D              W12
    PUSH                W14
    /* Hack in the PC/SRL double word mimicking an interrupt entry */
    MOV                 #handle(__RMP_DSPIC_Skip),W0
    MOV                 [W15-16*2],W1
    SL                  W1,#11,W1
    LSR                 W1,#3,W1
    MOV                 CORCON,W2
    /* PCL:SFA */
    CLR                 W3
    BTST.C              W2,#2
    BSW.C               W0,W3
    MOV                 W0,[W15-18*2]
    /* SRL:IPL3:PCH, where IPL=0 in the SRL */
    MOV                 #7,W3
    BTST.C              W2,#3
    BSW.C               W1,W3
    MOV                 W1,[W15-17*2]
    /* Push extended regs */
    PUSH                ACCAL
    PUSH                ACCAH
    PUSH                ACCAU
    PUSH                ACCBL
    PUSH                ACCBH
    PUSH                ACCBU
    PUSH                DSRPAG      /* Was PSVPAG on PIC24 parts */
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
    /* Get the highest priority ready task */
    CALL                __RMP_Run_High
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
__RMP_DSPIC_Skip:
    RETURN
/* End Function:_RMP_DSPIC_Yield *********************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
