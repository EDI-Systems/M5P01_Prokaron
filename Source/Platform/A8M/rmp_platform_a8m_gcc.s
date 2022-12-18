/******************************************************************************
Filename    : rmp_platform_cmx_asm_gcc.S
Author      : songleifeng
Date        : 07/19/2018
Description : The assembly part of the RMP RTOS. This is for Cortex-M3/4/7, and
              is used on gcc. This is derived from the MDK version.
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
            
/* Begin Header **************************************************************/
    .syntax             unified
    .thumb
    .section            ".text"
    .align              3
/* End Header ****************************************************************/

/* Begin Exports *************************************************************/
    .global             RMP_Int_Disable       
    .global             RMP_Int_Enable
    .global             RMP_Int_Mask     
    .global             RMP_MSB_Get
    .global             _RMP_Start
    .global             _RMP_Yield        
    .global             PendSV_Handler 
    .global             SysTick_Handler                               
/* End Exports ***************************************************************/

/* Begin Imports *************************************************************/
    .extern             _RMP_Rdy_High 
    .extern             _RMP_Tick_Handler     
    .extern             RMP_Thd_Cur
    .extern             RMP_SP_Cur        
    .extern             RMP_Ctx_Save
    .extern             RMP_Ctx_Load
/* End Imports ***************************************************************/

/* Begin Function:RMP_Int_Disable *********************************************
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

/* Begin Function:RMP_Int_Enable **********************************************
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

/* Begin Function:RMP_Int_Mask ************************************************
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
    BX                  LR
    .fnend
/* End Function:RMP_Int_Mask *************************************************/

/* Begin Function:RMP_MSB_Get *************************************************
Description : Get the MSB of the word.
Input       : rmp_ptr_t R0 - The value.
Output      : None.
Return      : ptr_t - The MSB position.
******************************************************************************/
    .thumb_func
RMP_MSB_Get:
    .fnstart
    .cantunwind
    CLZ                 R1,R0
    MOVS                R0,#31
    SUBS                R0,R1
    BX                  LR
    .fnend
/* End Function:RMP_MSB_Get **************************************************/

/* Begin Function:_RMP_Yield **************************************************
Description : Trigger a yield to another thread.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
    .thumb_func
_RMP_Yield:
    .fnstart
    .cantunwind
    LDR                 R0,=0xE000ED04        
    LDR                 R1,=0x10000000        
    STR                 R1,[R0]
                   
    ISB 
    BX                  LR         
    .fnend
/* End Function:_RMP_Yield ***************************************************/

/* Begin Function:_RMP_Start **************************************************
Description : Jump to the user function and will never return from it.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
    .thumb_func
_RMP_Start:
    .fnstart
    .cantunwind
    SUBS                R1,#64               
    MSR                 PSP,R1                
    MOVS                R4,#0x02              
    MSR                 CONTROL,R4
                                 
    ISB
    BLX                 R0              
    .fnend
/* End Function:_RMP_Start ***************************************************/

/* Begin Function:PendSV_Handler **********************************************
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
    MRS                 R0,PSP
    TST                 LR,#0x10            /* Are we using the FPU or not at all? */
    .hword              0xBF08              /* IT EQ (.hword for compatibility with no FPU support) */
    .hword              0xED20              /* VSTMDBEQ R0!,{S16-S31} */
    .hword              0x8A10              /* Save FPU registers not saved by lazy stacking. */
    STMDB               R0!,{R4-R11,LR}
    
    BL                  RMP_Ctx_Save               
    
    LDR                 R1,=RMP_SP_Cur          
    STR                 R0,[R1]
    
    BL                  _RMP_Rdy_High        
    
    LDR                 R1,=RMP_SP_Cur           
    LDR                 R0,[R1]
    
    BL                  RMP_Ctx_Load             
    
    LDMIA               R0!,{R4-R11,LR}
    TST                 LR,#0x10            /* Are we using the FPU or not at all? */
    .hword              0xBF08              /* IT EQ (.hword for compatibility with no FPU support) */
    .hword              0xECB0              /* VLDMIAEQ R0!,{S16-S31} */
    .hword              0x8A10              /* Load FPU registers not loaded by lazy stacking. */
    MSR                 PSP,R0
    
    BX                  LR                       
    .fnend				
/* End Function:PendSV_Handler ***********************************************/

/* Begin Function:SysTick_Handler *********************************************
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
    
    MOVS                R0,#0x01                    
    BL                  _RMP_Tick_Handler
    
    POP                 {PC}
    NOP
    .fnend
/* End Function:SysTick_Handler **********************************************/

    .end
/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
