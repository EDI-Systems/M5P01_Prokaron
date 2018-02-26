;/*****************************************************************************
;Filename    : platform_msp430_asm.s
;Author      : pry
;Date        : 25/02/2018
;Description : The assembly part of the RMP RTOS.
;*****************************************************************************/

;/* The MSP430 Structure ******************************************************
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
                .align          2
;/* End Header ***************************************************************/

;/* Begin Exports ************************************************************/
                ;Disable all interrupts
                .def            RMP_Disable_Int      
                ;Enable all interrupts            
                .def            RMP_Enable_Int
                ;Start the first thread
                .def            _RMP_Start
                ;The PendSV trigger
                .def            _RMP_Yield
                ;The system pending service routine              
                .def            PendSV_Handler 
                ;The systick timer routine              
                .def            SysTick_Handler                               
;/* End Exports **************************************************************/

;/* Begin Imports ************************************************************/
                ;The real task switch handling function
                .global         _RMP_Get_High_Rdy 
                ;The real systick handler function
                .global         _RMP_Tick_Handler
                ;The PID of the current thread                     
                .global         RMP_Cur_Thd
                ;The stack address of current thread
                .global         RMP_Cur_SP        
                ;Save and load extra contexts, such as FPU, peripherals and MPU
                .global         RMP_Save_Ctx
                .global         RMP_Load_Ctx
;/* End Imports **************************************************************/

;/* Begin Macros *************************************************************/
; Push everything to stack
SAVE_CONTEXT 	.macro
                push            SR
                pushm.a         #12,R15
	            .endm
LOAD_CONTEXT .macro
                popm.a          #12,R15
	            pop             SR
	            nop
	            reti
				.endm
;/* End Macros ***************************************************************/

;/* Begin Function:RMP_Disable_Int ********************************************
;Description    : The function for disabling all interrupts. Does not allow nesting.
;Input          : None.
;Output         : None.    
;Register Usage : None.                                  
;*****************************************************************************/
                .text
                .align          2
RMP_Disable_Int:.asmfunc
                ;Disable all interrupts
                nop
                dint
                nop
                reta
                .endasmfunc
;/* End Function:RMP_Disable_Int *********************************************/

;/* Begin Function:RMP_Enable_Int *********************************************
;Description    : The function for enabling all interrupts. Does not allow nesting.
;Input          : None.
;Output         : None.    
;Register Usage : None.                                  
;*****************************************************************************/
                .text
                .align          2
RMP_Enable_Int:.asmfunc
                ;Enable all interrupts
                nop
                eint
                nop
                reta
                .endasmfunc               
;/* End Function:RMP_Enable_Int **********************************************/

;/* Begin Function:_RMP_Yield *************************************************
;Description : Trigger a yield to another thread. This will always trigger the
;              timer 0 compare vector on MSP430.
;Input       : None.
;Output      : None.                                      
;*****************************************************************************/
                .text
                .align          2
_RMP_Yield:     .asmfunc
                dint
                nop
                ; Atomically trigger the timer 0 compare isr
                
                eint
                nop
                reta
                .endasmfunc                                                
;/* End Function:_RMP_Yield **************************************************/

;/* Begin Function:_RMP_Start *************************************************
;Description : Jump to the user function and will never return from it.
;              Note that all thread function entries should locate at <64kB.
;Input       : R13:R12 - PC.
;              R14 - SP.
;Output      : None.                                      
;*****************************************************************************/
                .text
                .align          2
_RMP_Start:     .asmfunc
                ;Place the entry into the same word
                push            R13
                push            R12
                popa            R13
                mova            R14,SP
                mova            R13,PC
                ; Dummy return
                reta
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
;*****************************************************************************/
                .sect ".text:_isr"
                .align          2
PendSV_Handler: .asmfunc
                ;Spill all the registers onto the user stack
                SAVE_CONTEXT
                
                ;Save extra context
                calla     #RMP_Save_Ctx
                
                ;Save The SP to control block.
                mova      SP,&RMP_Cur_SP
                
                ;Get the highest ready task.
                calla     #_RMP_Get_High_Rdy
                
                ;Load the SP.
                mova      &RMP_Cur_SP,SP
                
                ;Load extra context
                calla     #RMP_Load_Ctx

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
;*****************************************************************************/
                .sect ".text:_isr"
                .align          2
SysTick_Handler:.asmfunc
                ;Spill all the registers onto the user stack
                SAVE_CONTEXT
                
                ;Note the system that we have entered an interrupt. We are not using tickless.
                mova      #0x01,R12
                calla     #_RMP_Tick_Handler

                LOAD_CONTEXT
                .endasmfunc
;/* End Function:SysTick_Handler *********************************************/

            	.sect   ".int45"
            	.short  SysTick_Handler
            	.sect   ".int44"
            	.short  PendSV_Handler
;/* End Of File **************************************************************/

;/* Copyright (C) 2011-2013 Evo-Devo Instrum. All rights reserved ************/
