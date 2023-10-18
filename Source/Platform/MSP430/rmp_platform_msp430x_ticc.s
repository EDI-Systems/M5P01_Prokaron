;/*****************************************************************************
;Filename    : rmp_platform_msp430x_ticc.s
;Author      : pry
;Date        : 25/02/2018
;Description : The assembly part of the RMP RTOS. This is for MSP430X.
;              This port does not support interrupt preemption, as it makes no 
;              sense on MSP430(X) because MSP430(X) interrupt priority is fixed.
;*****************************************************************************/

;/* The MSP430X Architecture **************************************************
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
    .def                RMP_Int_Disable      
    ;Enable all interrupts            
    .def                RMP_Int_Enable
    ;Start the first thread
    .def                _RMP_Start
    ;The system pending service routine              
    .def                _RMP_MSP430_Ctx_Handler
    ;The systick timer routine              
    .def                _RMP_MSP430_Tim_Handler
;/* End Exports **************************************************************/

;/* Begin Imports ************************************************************/
    ;The real task switch handling function
    .global             _RMP_Run_High 
    ;The real systick handler function
    .global             _RMP_Tim_Handler
    ;The PID of the current thread                     
    .global             RMP_Thd_Cur
    ;The stack address of current thread
    .global             RMP_SP_Cur        
    ;Save and load extra contexts, such as FPU, peripherals and MPU
    .global             RMP_Ctx_Save
    .global             RMP_Ctx_Load
    ;Clear flags
    .global             _RMP_MSP430_Ctx_Clr
    .global             _RMP_MSP430_Tim_Clr
;/* End Imports **************************************************************/

;/* Begin Macros *************************************************************/
; Push everything to stack
SAVE_CONTEXT 	        .macro
    push                SR
    pushm.a             #12, R15
    .endm
LOAD_CONTEXT            .macro
    popm.a              #12, R15
    pop                 SR
    nop
    reti
    .endm
;/* End Macros ***************************************************************/

;/* Begin Function:RMP_Int_Disable ********************************************
;Description : The function for disabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
    .text
    .align              2
RMP_Int_Disable:        .asmfunc
    ;Disable all interrupts
    nop
    dint
    nop
    reta
    .endasmfunc
;/* End Function:RMP_Int_Disable *********************************************/

;/* Begin Function:RMP_Int_Enable *********************************************
;Description : The function for enabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
    .text
    .align              2
RMP_Int_Enable:         .asmfunc
    ;Enable all interrupts
    nop
    eint
    nop
    reta
    .endasmfunc               
;/* End Function:RMP_Int_Enable **********************************************/

;/* Begin Function:_RMP_Start *************************************************
;Description : Jump to the user function and will never return from it.
;Input       : rmp_ptr_t R13:R12 - PC.
;              rmp_ptr_t R15:R14 - SP.
;Output      : None.
;Return      : None.
;*****************************************************************************/
    .text
    .align              2
_RMP_Start:             .asmfunc
    ;Place the PC into the same word
    push                R13
    push                R12
    popa                R12
    ;Place the SP into the same word
    push                R15
    push                R14
    popa                R14
    ;Jump to location
    mova                R14, SP
    mova                R12, PC
    ; Dummy return
    reta
    .endasmfunc       
;/* End Function:_RMP_Start **************************************************/

;/* Begin Function:_RMP_MSP430_Ctx_Handler ************************************
;Description : The ctxsw interrupt routine. In fact, it will call a C function
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
_RMP_MSP430_Ctx_Handler: .asmfunc
    ;Spill all the registers onto the user stack
    SAVE_CONTEXT
                
    ;Clear the interrupt flag
    calla               #_RMP_MSP430_Ctx_Clr
                
    ;Save extra context
    calla               #RMP_Ctx_Save
                
    ;Save The SP to control block.
    mova                SP, &RMP_SP_Cur
                
    ;Get the highest ready task.
    calla               #_RMP_Run_High
                
    ;Load the SP.
    mova                &RMP_SP_Cur, SP
                
    ;Load extra context
    calla               #RMP_Ctx_Load

    LOAD_CONTEXT
    .endasmfunc       
;/* End Function:_RMP_MSP430_Ctx_Handler *************************************/

;/* Begin Function:_RMP_MSP430_Tim_Handler ************************************
;Description : The OS timer interrupt routine. In fact, it will call a C function
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
_RMP_MSP430_Tim_Handler: .asmfunc
    ;Spill all the registers onto the user stack
    SAVE_CONTEXT
                
    ;Clear the interrupt flag
    calla               #_RMP_MSP430_Tim_Clr
                
    ;Note the system that we have entered an interrupt. We are not using tickless.
    mova                #0x01, R12
    calla               #_RMP_Tim_Handler

    LOAD_CONTEXT
    .endasmfunc
;/* End Function:_RMP_MSP430_Tim_Handler *************************************/

;/* Need to tailor these to specific microcontrollers - asm not portable */
	.sect         ".int44"
	.short        _RMP_MSP430_Ctx_Handler
	.sect         ".int45"
	.short        _RMP_MSP430_Tim_Handler
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
