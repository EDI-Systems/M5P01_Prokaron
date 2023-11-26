/******************************************************************************
Filename    : rmp_platform_a7m_asm_gcc.s
Author      : songleifeng
Date        : 07/19/2018
Description : The assembly part of the RMP RTOS. This is for ARMv7-M.
******************************************************************************/

/* The ARM Cortex-M Architecture **********************************************
R0-R7:General purpose registers that are accessible.
R8-R12:general purpose registers that can only be reached by 32-bit instructions.
R13:SP/SP_process/SP_main    Stack pointer
R14:LR                       Link Register(used for returning from a subfunction)
R15:PC                       Program counter.
IPSR                         Interrupt Program Status Register.
APSR                         Application Program Status Register.
EPSR                         Execute Program Status Register.
The above 3 registers are saved into the stack in combination(xPSR).

The ARM Cortex-M4/7 also include a FPU.
******************************************************************************/

/* Import ********************************************************************/
    /* The real task switch handling function */
    .global             _RMP_Run_High
    /* The real systick handler function */
    .global             _RMP_Tim_Handler
    /* The PID of the current thread */
    .global             RMP_Thd_Cur
    /* The stack address of current thread */
    .global             RMP_SP_Cur
/* End Import ****************************************************************/
            
/* Export ********************************************************************/
    /* Disable all interrupts */
    .global             RMP_Int_Disable
    /* Enable all interrupts */
    .global             RMP_Int_Enable
    /* Mask/unmask some interrupts */
    .global             RMP_Int_Mask
    /* Get the MSB/LSB */
    .global             _RMP_A7M_MSB_Get
    .global             _RMP_A7M_LSB_Get
    /* Start the first thread */
    .global             _RMP_Start
    /* The PendSV trigger */
    .global             _RMP_Yield
    /* The system pending service routine */
    .global             PendSV_Handler
    /* The systick timer routine */
    .global             SysTick_Handler
/* End Export ****************************************************************/

/* Header ********************************************************************/
    .section            ".text.arch"
    .align              3
    .syntax             unified
    .thumb
/* End Header ****************************************************************/

/* Function:RMP_Int_Disable ***************************************************
Description : The function for disabling all interrupts. Does not allow nesting.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
    .thumb_func
RMP_Int_Disable:
    .fnstart
    .cantunwind
    CPSID               I                                                       
    BX                  LR       
    .fnend
/* End Function:RMP_Int_Disable **********************************************/

/* Function:RMP_Int_Enable ****************************************************
Description : The function for enabling all interrupts. Does not allow nesting.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
    .thumb_func
RMP_Int_Enable:
    .fnstart
    .cantunwind
    CPSIE               I               
    BX                  LR
    .fnend				
/* End Function:RMP_Int_Enable ***********************************************/

/* Function:RMP_Int_Mask ******************************************************
Description : The function for masking & unmasking interrupts. Does not allow nesting.
Input       : rmp_ptr_t R0 - The new BASEPRI to set.
Output      : None.
Return      : None.
******************************************************************************/
    .thumb_func
RMP_Int_Mask:
    .fnstart
    .cantunwind
    MSR                 BASEPRI,R0
    /* We are not influenced by errata #837070 as the next */
    /* instruction is BX LR. Thus we have a free window. */
    BX                  LR
    .fnend
/* End Function:RMP_Int_Mask *************************************************/

/* Function:_RMP_A7M_MSB_Get **************************************************
Description : Get the MSB of the word.
Input       : rmp_ptr_t R0 - The value.
Output      : None.
Return      : rmp_ptr_t R0 - The MSB position.
******************************************************************************/
    .thumb_func
_RMP_A7M_MSB_Get:
    .fnstart
    .cantunwind
    CLZ                 R1,R0
    MOVS                R0,#31
    SUBS                R0,R1
    BX                  LR
    .fnend
/* End Function:_RMP_A7M_MSB_Get *********************************************/

/* Function:_RMP_A7M_LSB_Get **************************************************
Description : Get the LSB of the word.
Input       : rmp_ptr_t R0 - The value.
Output      : None.
Return      : rmp_ptr_t R0 - The LSB position.
******************************************************************************/
    .thumb_func
_RMP_A7M_LSB_Get:
    .fnstart
    .cantunwind
    RBIT                R0,R0
    CLZ                 R0,R0
    BX                  LR
    .fnend
/* End Function:_RMP_A7M_LSB_Get *********************************************/

/* Function:_RMP_Yield ********************************************************
Description : Trigger a yield to another thread.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
    .thumb_func
_RMP_Yield:
    .fnstart
    .cantunwind
    LDR                 R0,=0xE000ED04      /* The NVIC_INT_CTRL register */
    LDR                 R1,=0x10000000      /* Trigger the PendSV */
    STR                 R1,[R0]
    ISB 
    BX                  LR         
    .fnend
/* End Function:_RMP_Yield ***************************************************/

/* Function:_RMP_Start ********************************************************
Description : Jump to the user function and will never return from it.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
    .thumb_func
_RMP_Start:
    .fnstart
    .cantunwind           
    MSR                 PSP,R1              /* Set the stack pointer */
    MOVS                R4,#0x02            /* Previleged thread mode */
    MSR                 CONTROL,R4
    ISB
    BLX                 R0                  /* Branch to our target */
    .fnend
/* End Function:_RMP_Start ***************************************************/

/* Function:PendSV_Handler ****************************************************
Description : The PendSV interrupt routine. In fact, it will call a C function
              directly. The reason why the interrupt routine must be an assembly
              function is that the compiler may deal with the stack in a different
              way when different optimization level is chosen. An assembly function
              can make way around this problem.
              However, if your compiler support inline assembly functions, this
              can also be written in C.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
    .thumb_func
PendSV_Handler:
    .fnstart
    .cantunwind
    MRS                 R0,PSP              /* Save registers */
    TST                 LR,#0x10            /* Save FPU registers */
    .hword              0xBF08              /* IT EQ ;If yes, (DCI for compatibility with no FPU support) */
    .hword              0xED20              /* VSTMDBEQ R0!,{S16-S31} */
    .hword              0x8A10              /* Save FPU registers not saved by lazy stacking. */
    STMDB               R0!,{R4-R11,LR}     /* Save the general purpose registers. */
    
    LDR                 R1,=RMP_SP_Cur      /* Save The SP to control block. */
    STR                 R0,[R1]
    BL                  _RMP_Run_High       /* Get the highest ready task. */
    LDR                 R1,=RMP_SP_Cur      /* Load the SP. */
    LDR                 R0,[R1]  
    
    LDMIA               R0!,{R4-R11,LR}     /* Restore registers */
    TST                 LR,#0x10            /* Load FPU registers */
    .hword              0xBF08              /* IT EQ ;If yes, (DCI for compatibility with no FPU support) */
    .hword              0xECB0              /* VLDMIAEQ R0!,{S16-S31} */
    .hword              0x8A10              /* Load FPU registers not loaded by lazy stacking. */
    MSR                 PSP,R0
    
    BX                  LR                       
    .fnend				
/* End Function:PendSV_Handler ***********************************************/

/* Function:SysTick_Handler ***************************************************
Description : The SysTick interrupt routine. In fact, it will call a C function
              directly. The reason why the interrupt routine must be an assembly
              function is that the compiler may deal with the stack in a different
              way when different optimization level is chosen. An assembly function
              can make way around this problem.
              However, if your compiler support inline assembly functions, this
              can also be written in C.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
    .thumb_func
SysTick_Handler:
    .fnstart
    .cantunwind
    PUSH                {LR}
    
    MOVS                R0,#0x01            /* We are not using tickless. */
    BL                  _RMP_Tim_Handler
    
    POP                 {PC}
    NOP
    .fnend
/* End Function:SysTick_Handler **********************************************/

    .end
/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
