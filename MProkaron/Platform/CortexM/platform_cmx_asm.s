;/*****************************************************************************
;Filename    : platform_cmx_asm.s
;Author      : pry
;Date        : 10/04/2012
;Description : The assembly part of the RMP RTOS. This is for Cortex-M3/4/7.
;*****************************************************************************/

;/* The ARM Cortex-M Structure ************************************************
;R0-R7:General purpose registers that are accessible. 
;R8-R12:general purpose registers that can only be reached by 32-bit instructions.
;R13:SP/SP_process/SP_main    Stack pointer
;R14:LR                       Link Register(used for returning from a subfunction)
;R15:PC                       Program counter.
;IPSR                         Interrupt Program Status Register.
;APSR                         Application Program Status Register.
;EPSR                         Execute Program Status Register.
;The above 3 registers are saved into the stack in combination(xPSR).
;
;The ARM Cortex-M4/7 also include a single-accuracy FPU.
;*****************************************************************************/
            
;/* Begin Header *************************************************************/
                ;The align is "(2^3)/8=1(Byte)." In fact it does not take effect.            
                AREA            ARCH,CODE,READONLY,ALIGN=3                     
                
                THUMB
                REQUIRE8
                PRESERVE8
;/* End Header ***************************************************************/

;/* Begin Exports ************************************************************/
                ;Disable all interrupts
                EXPORT          RMP_Disable_Int      
                ;Enable all interrupts            
                EXPORT          RMP_Enable_Int
                ;Mask/unmask some interrupts
                EXPORT          RMP_Mask_Int
                ;Get the MSB                              
                EXPORT          RMP_MSB_Get
                ;Start the first thread
                EXPORT          _RMP_Start
                ;The PendSV trigger
                EXPORT          _RMP_Yield
                ;The system pending service routine              
                EXPORT          PendSV_Handler 
                ;The systick timer routine              
                EXPORT          SysTick_Handler                               
;/* End Exports **************************************************************/

;/* Begin Imports ************************************************************/
                ;The real task switch handling function
                IMPORT          _RMP_Get_High_Rdy 
                ;The real systick handler function
                IMPORT          _RMP_Tick_Handler
                ;The PID of the current thread                     
                IMPORT          RMP_Cur_Thd
                ;The stack address of current thread
                IMPORT          RMP_Cur_SP        
                ;Save and load extra contexts, such as FPU, peripherals and MPU
                IMPORT          RMP_Save_Ctx
                IMPORT          RMP_Load_Ctx
;/* End Imports **************************************************************/

;/* Begin Function:RMP_Disable_Int ********************************************
;Description    : The function for disabling all interrupts. Does not allow nesting.
;Input          : None.
;Output         : None.    
;Register Usage : None.                                  
;*****************************************************************************/    
RMP_Disable_Int
                ;Disable all interrupts (I is primask,F is Faultmask.)
                CPSID           I                                                       
                BX              LR                                                 
;/* End Function:RMP_Disable_Int *********************************************/

;/* Begin Function:RMP_Enable_Int *********************************************
;Description    : The function for enabling all interrupts. Does not allow nesting.
;Input          : None.
;Output         : None.    
;Register Usage : None.                                  
;*****************************************************************************/
RMP_Enable_Int
                ;Enable all interrupts.
                CPSIE           I               
                BX              LR
;/* End Function:RMP_Enable_Int **********************************************/

;/* Begin Function:RMP_Mask_Int ***********************************************
;Description    : The function for masking & unmasking interrupts. Does not allow masking.
;Input          : R0 - The new basepri to set.
;Output         : None.    
;Register Usage : None.                                  
;*****************************************************************************/
RMP_Mask_Int
                ;Mask some interrupts.
                MSR             BASEPRI,R0                                                        
                BX              LR
;/* End Function:RMP_Mask_Int ************************************************/

;/* Begin Function:RMP_MSB_Get ************************************************
;Description    : Get the MSB of the word.
;Input          : ptr_t Val - The value.
;Output         : None.
;Return         : ptr_t - The MSB position.   
;Register Usage : None. 
;*****************************************************************************/
RMP_MSB_Get
                CLZ             R1,R0
                MOVS            R0,#31
                SUBS            R0,R1
                BX              LR
;/* End Function:RMP_MSB_Get *************************************************/

;/* Begin Function:_RMP_Yield *************************************************
;Description : Trigger a yield to another thread.
;Input       : None.
;Output      : None.                                      
;*****************************************************************************/
_RMP_Yield
                LDR             R0,=0xE000ED04         ;The NVIC_INT_CTRL register
                LDR             R1,=0x10000000         ;Trigger the PendSV          
                STR             R1,[R0]

                DSB                                    ;Data and instruction barrier
                ISB
                
                BX              LR                                                   
;/* End Function:_RMP_Yield **************************************************/

;/* Begin Function:_RMP_Start *************************************************
;Description : Jump to the user function and will never return from it.
;Input       : None.
;Output      : None.                                      
;*****************************************************************************/
_RMP_Start
                ;Should never reach here
                SUBS            R1,#64                 ;This is how we push our registers so move forward
                MSR             PSP,R1                 ;Set the stack pointer
                MOVS            R4,#0x02               ;Previleged thread mode
                MSR             CONTROL,R4
                
                DSB                                    ;Data and instruction barrier
                ISB
                
                BLX             R0                     ;Branch to our target
                B               .                      ;Capture faults      
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
PendSV_Handler
                MRS       R0,PSP                     ;Spill all the registers onto the user stack
                STMDB     R0!,{R4-R11,LR}
                
                BL        RMP_Save_Ctx               ;Save extra context
                
                LDR       R1,=RMP_Cur_SP             ;Save The SP to control block.
                STR       R0,[R1]
                
                BL        _RMP_Get_High_Rdy          ;Get the highest ready task.
                
                LDR       R1,=RMP_Cur_SP             ;Load the SP.
                LDR       R0,[R1]
                
                BL        RMP_Load_Ctx               ;Load extra context

                LDMIA     R0!,{R4-R11,LR}
                MSR       PSP,R0
                
                BX        LR                         ;The LR will indicate whether we are using FPU.     
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
SysTick_Handler
                PUSH      {LR}
                
                MOVS      R0,#0x01                     ;We are not using tickless.
                BL        _RMP_Tick_Handler
                
                POP       {PC}
                NOP
;/* End Function:SysTick_Handler *********************************************/

                END
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
