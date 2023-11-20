/******************************************************************************
Filename    : rmp_platform_dspic_gcc.s
Author      : pry
Date        : 10/04/2012
Description : The assembly part of the RMP RTOS. Different from most architectures,
              this architecture have an empty ascending stack(grows to high address,
              and where the SP points to is always empty).
              In this port, we save all the registers to stack and then enable
              interrupt. This is because the CPU can push memory efficiently only
              with SP.
              Limitations of this port:
              Enabling & Disabling of interrupts only disables interrupt level 0-6.
              Enabling & Disabling of interrupts can only persist 16383 cycles.
              Stack limit feature is never enabled.
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
DSRPAG      10          Data space read page address.
DSWPAG      9           Data space write page address.
RCOUNT      16          Repeat loop counter.
DCOUNT      16          DO loop counter and stack.(DSP33EPxx806/810/814 only)
DOSTART     24          DO loop start address and stack.(DSP33EPxx806/810/814 only)
DOEND       24          DO loop end address and stack.(DSP33EPxx806/810/814 only)
CORCON      16          Core control register.
STATUS      16          Status register.
******************************************************************************/
    
/* Begin Header **************************************************************/
    .text
    .align 2
/* End Header ****************************************************************/

/* Begin Exports *************************************************************/
    /* The DSPIC toolchain mangles all symbols with an extra '_' */
    /* Disable all interrupts */
    .global             _RMP_Int_Disable      
    /* Enable all interrupts */        
    .global             _RMP_Int_Enable   
    /* Get the MSB/LSB */
    .global             _RMP_DSPIC_MSB_Get
    .global             _RMP_DSPIC_LSB_Get
    /* Start the first thread */
    .global             __RMP_Start
    /* The system pending service routine */
    .global             __INT0Interrupt 
    /* The systick timer routine */
    .global             __T1Interrupt
/* End Exports ***************************************************************/

/* Begin Imports *************************************************************/
    /* The real task switch handling function */
    .extern             __RMP_Run_High 
    /* The real systick handler function */
    .extern             __RMP_Tim_Handler
    /* The PID of the current thread */
    .extern             _RMP_Thd_Cur
    /* The stack address of current thread */
    .extern             _RMP_SP_Cur
    /* The place where we store the kernel sp value */
    .extern             _RMP_SP_Val
    /* Kernel constant pool global pointers */
    .extern             _RMP_TBLPAG_Val;
    .extern             _RMP_DSRPAG_Val;
    .extern             _RMP_DSWPAG_Val;
    /* The interrupt nesting value */
    .extern             _RMP_Int_Nest
    /* Clear timer & software interrupt flags */
    .extern             __RMP_Clear_Soft_Flag
    .extern             __RMP_Clear_Timer_Flag
/* End Imports ***************************************************************/

/* Begin Macros **************************************************************/
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

/* Begin Function:RMP_Int_Disable *********************************************
Description    : The function for disabling all interrupts. Does not allow nesting.
                 This macro only disables interrupt for a short time; this is a 
                 limitation of DSPIC33 architecture. Considering the common usage
                 pattern of this function, 16383 cycles should be far more than 
                 sufficient for the operations we perform.
Input          : None.
Output         : None.    
Register Usage : None.                                  
******************************************************************************/
_RMP_Int_Disable:
    /* Reliably disable all interrupts */
    DISI                #16383
    RETURN
/* End Function:RMP_Int_Disable **********************************************/

/* Begin Function:RMP_Int_Enable **********************************************
Description    : The function for enabling all interrupts. Does not allow nesting.
                 Can't directly operate on DISICNT; see errata. Have to use DISI #0.
Input          : None.
Output         : None.    
Register Usage : None.                                  
******************************************************************************/
_RMP_Int_Enable:
    /* Enable all interrupts */
    DISI                #0
    RETURN
/* End Function:RMP_Int_Enable ***********************************************/

/* Begin Function:_RMP_DSPIC_MSB_Get ******************************************
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

/* Begin Function:_RMP_DSPIC_LSB_Get ******************************************
Description    : Get the LSB of the word.
Input          : ptr_t Value - The value.
Output         : None.
Return         : ptr_t - The LSB position.   
Register Usage : None. 
******************************************************************************/
_RMP_DSPIC_LSB_Get:
    /* This word is non-zero, we need to find MSB */
    FF1R                W0,W0
    SUB                 #1,W0
    RETURN
/* End Function:_RMP_DSPIC_LSB_Get *******************************************/

/* Begin Function:_RMP_Start **************************************************
Description : Jump to the user function and will never return from it.
Input       : [W0]: Entry of the thread.
              [W1]: Stack of the thread.
Output      : None.
******************************************************************************/
__RMP_Start:
    MOV                 TBLPAG,W2
    MOV                 W2,_RMP_TBLPAG_Val
    MOV                 DSRPAG,W2
    MOV                 W2,_RMP_DSRPAG_Val
    MOV                 DSWPAG,W2
    MOV                 W2,_RMP_DSWPAG_Val
    /* Save this SP for kernel use */
    MOV                 W15,_RMP_SP_Val
    MOV                 W1,W15
    GOTO                W0
    /* Should not reach here */
    RETURN
/* End Function:_RMP_Start ***************************************************/

/* Begin Function:__INT0Interrupt *********************************************
Description : The switch interrupt routine. In fact, it will call a C function
              directly. The reason why the interrupt routine must be an assembly
              function is that the compiler may deal with the stack in a different 
              way when different optimization level is chosen. An assembly function
              can make way around this problem.
              However, if your compiler support inline assembly functions, this
              can also be written in C.
Input       : None.
Output      : None.                                      
******************************************************************************/
__INT0Interrupt:
    SAVE_CONTEXT

    /* Get the highest priority ready task */
    CALL                __RMP_Run_High
    /* Clear software interrupt flag */
    CALL                __RMP_Clear_Soft_Flag

    LOAD_CONTEXT
/* End Function:__INT0Interrupt **********************************************/

/* Begin Function:__T1Interrupt ***********************************************
Description : The timer 1 interrupt routine. In fact, it will call a C function
              directly. The reason why the interrupt routine must be an assembly
              function is that the compiler may deal with the stack in a different 
              way when different optimization level is chosen. An assembly function
              can make way around this problem.
              However, if your compiler support inline assembly functions, this
              can also be written in C.
Input       : None.
Output      : None.                                      
******************************************************************************/
__T1Interrupt:
    SAVE_CONTEXT
    
    /* We are not using tickless */
    CLR                 W1
    MOV                 #1,W0
    CALL                __RMP_Tim_Handler
    CALL                __RMP_Clear_Timer_Flag
    
    LOAD_CONTEXT
    .end
/* End Function:__T1Interrupt ************************************************/
                
/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
