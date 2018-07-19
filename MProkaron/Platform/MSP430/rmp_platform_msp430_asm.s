;/*****************************************************************************
;Filename    : rmp_platform_msp430_asm.s
;Author      : pry
;Date        : 25/02/2018
;Description : The assembly part of the RMP RTOS. This is for MSP430.
;              This port does not support interrupt preemption, as it makes no 
;              sense on MSP430(X) because MSP430(X) interrupt priority is fixed.
;*****************************************************************************/

;/* The MSP430 Architecture ***************************************************
;R0:PC                        Program counter.
;R1:SP                        Stack pointer.
;R2:SR                        Status register.
;R3:CG2                       Constant generator.
;R4-R15:                      General purpose registers.
;Note that MSP430X's register PC,SP,R4-R15 are all 20 bits,while MSP430 is 16 bits.
;Some variants also have the low energy vector math accelerator.
;*****************************************************************************/

;/* Begin Header *************************************************************/
    .text
    .align              2
;/* End Header ***************************************************************/

;/* Begin Exports ************************************************************/
    ;Disable all interrupts
    .def                RMP_Disable_Int      
    ;Enable all interrupts            
    .def                RMP_Enable_Int
    ;Start the first thread
    .def                _RMP_Start
    ;The system pending service routine              
    .def                PendSV_Handler 
    ;The systick timer routine              
    .def                SysTick_Handler                               
;/* End Exports **************************************************************/

;/* Begin Imports ************************************************************/
    ;The real task switch handling function
    .global             _RMP_Get_High_Rdy 
    ;The real systick handler function
    .global             _RMP_Tick_Handler
    ;The PID of the current thread                     
    .global             RMP_Cur_Thd
    ;The stack address of current thread
    .global             RMP_Cur_SP        
    ;Save and load extra contexts, such as FPU, peripherals and MPU
    .global             RMP_Save_Ctx
    .global             RMP_Load_Ctx
    ;Clear flags
    .global             _RMP_Clear_Soft_Flag
    .global             _RMP_Clear_Timer_Flag
;/* End Imports **************************************************************/

;/* Begin Macros *************************************************************/
; Push everything to stack
SAVE_CONTEXT            .macro
    push                SR
    pushm               #12,R15
    .endm
LOAD_CONTEXT            .macro
    popm                #12,R15
    pop                 SR
    nop
    reti
    .endm
;/* End Macros ***************************************************************/

;/* Begin Function:RMP_Disable_Int ********************************************
;Description : The function for disabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
    .text
    .align              2
RMP_Disable_Int:        .asmfunc
    ;Disable all interrupts
    nop
    dint
    nop
    ret
    .endasmfunc
;/* End Function:RMP_Disable_Int *********************************************/

;/* Begin Function:RMP_Enable_Int *********************************************
;Description : The function for enabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.                                 
;*****************************************************************************/
    .text
    .align              2
RMP_Enable_Int:.asmfunc
    ;Enable all interrupts
    nop
    eint
    nop
    ret
    .endasmfunc               
;/* End Function:RMP_Enable_Int **********************************************/

;/* Begin Function:_RMP_Start *************************************************
;Description : Jump to the user function and will never return from it.
;              Note that all thread function entries should locate at <64kB.
;Input       : rmp_ptr_t R12 - PC.
;              rmp_ptr_t R13 - SP.
;Output      : None.
;Return      : None.
;*****************************************************************************/
    .text
    .align              2
_RMP_Start:             .asmfunc
    mov                 R13,SP
    mov                 R12,PC
    ; Dummy return
    ret
    .endasmfunc       
;/* End Function:_RMP_Start **************************************************/

;/* Begin Function:PendSV_Handler *********************************************
;Description : The PendSV interrupt routine. In fact, it will call a C function
;              directly. The reason why the interrupt routine must be an assembly
;              function is that the compiler may deal with the stack in a different 
;              way when different optimization level is chosen. An assembly function
;              can make way around this problem.
;              However, if your compiler support inline assembly functions, this
;              can also be written in C.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
    .sect               ".text:_isr"
    .align              2
PendSV_Handler:         .asmfunc
    ;Spill all the registers onto the user stack
    SAVE_CONTEXT
                
    ;Clear the interrupt flag
    call                #_RMP_Clear_Soft_Flag
                
    ;Save extra context
    call                #RMP_Save_Ctx
                
    ;Save The SP to control block.
    mov                 SP,&RMP_Cur_SP
                
    ;Get the highest ready task.
    call                #_RMP_Get_High_Rdy
                
    ;Load the SP.
    mov                 &RMP_Cur_SP,SP
                
    ;Load extra context
    call                #RMP_Load_Ctx

    LOAD_CONTEXT
    .endasmfunc       
;/* End Function:PendSV_Handler **********************************************/

;/* Begin Function:SysTick_Handler ********************************************
;Description : The SysTick interrupt routine. In fact, it will call a C function
;              directly. The reason why the interrupt routine must be an assembly
;              function is that the compiler may deal with the stack in a different 
;              way when different optimization level is chosen. An assembly function
;              can make way around this problem.
;              However, if your compiler support inline assembly functions, this
;              can also be written in C.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
    .sect               ".text:_isr"
    .align              2
SysTick_Handler:        .asmfunc
    ;Spill all the registers onto the user stack
    SAVE_CONTEXT
                
    ;Clear the interrupt flag
    call                #_RMP_Clear_Timer_Flag
                
    ;Note the system that we have entered an interrupt. We are not using tickless.
    mov                 #0x01,R12
    call                #_RMP_Tick_Handler

    LOAD_CONTEXT
    .endasmfunc
;/* End Function:SysTick_Handler *********************************************/

    ;Tailor these according to whatever chip you are using
    .sect         ".int45"
    .short        SysTick_Handler
    .sect         ".int44"
    .short        PendSV_Handler
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
