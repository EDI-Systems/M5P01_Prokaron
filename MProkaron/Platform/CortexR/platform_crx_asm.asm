;/*****************************************************************************
;Filename    : platform_crx_asm.s
;Author      : pry
;Date        : 10/04/2012
;Description : The assembly part of the RMP RTOS. This is for Cortex-R4/5/7/8.
;*****************************************************************************/

;/* The ARM Cortex-R Structure ************************************************
; Sys/User     FIQ   Supervisor   Abort     IRQ    Undefined
;    R0        R0        R0        R0       R0        R0
;    R1        R1        R1        R1       R1        R1
;    R2        R2        R2        R2       R2        R2
;    R3        R3        R3        R3       R3        R3
;    R4        R4        R4        R4       R4        R4
;    R5        R5        R5        R5       R5        R5
;    R6        R6        R6        R6       R6        R6
;    R7        R7        R7        R7       R7        R7
;    R8        R8_F      R8        R8       R8        R8
;    R9        R9_F      R9        R9       R9        R9
;    R10       R10_F     R10       R10      R10       R10
;    R11       R11_F     R11       R11      R11       R11
;    R12       R12_F     R12       R12      R12       R12
;    SP        SP_F      SP_S      SP_A     SP_I      SP_U
;    LR        LR_F      LR_S      LR_A     LR_I      LR_U
;    PC        PC        PC        PC       PC        PC
;---------------------------------------------------------------
;    CPSR      CPSR      CPSR      CPSR     CPSR      CPSR
;              SPSR_F    SPSR_S    SPSR_A   SPSR_I    SPSR_U
;    10000     10001     10011     10111    10010     11011
;---------------------------------------------------------------
;R0-R7  : General purpose registers that are accessible
;R8-R12 : General purpose regsisters that are not accessible by 16-bit thumb
;R13    : SP, Stack pointer
;R14    : LR, Link register
;R15    : PC, Program counter
;CPSR   : Program status word
;SPSR   : Banked program status word
;The ARM Cortex-R4/5/7/8 also include a single-accuracy FPU.
;*****************************************************************************/

;/* Begin Header *************************************************************/
                .text
                .arm
;/* End Header ***************************************************************/

;/* Begin Exports ************************************************************/
                ;Disable all interrupts
                .global         RMP_Disable_Int
                ;Enable all interrupts            
                .global         RMP_Enable_Int
                ;Mask/unmask some interrupts
                .global         RMP_Mask_Int
                ;Get the MSB                              
                .global         RMP_MSB_Get
                ;Start the first thread
                .global         _RMP_Start
                ;The PendSV trigger
                .global         _RMP_Yield
                ;The system pending service routine              
                .global         _svc                   ;PendSV_Handler
                ;The systick timer routine              
                .global         SysTick_Handler
                ;Other unused error handlers
                .global        _dabort
                .global        _prefetch
                .global        _undef
                .global        phantomInterrupt
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

;/* Begin Function:RMP_Disable_Int ********************************************
;Description    : The function for disabling all interrupts. Does not allow nesting.
;                 We never mask FIQs on Cortex-R because they are not allowed to
;                 perform any non-transparent operations anyway.
;Input          : None.
;Output         : None.    
;Register Usage : None.                                  
;*****************************************************************************/
RMP_Disable_Int .asmfunc
                ;Disable all interrupts (I is primask,F is Faultmask.)
                CPSID           I
                BX              LR
                .endasmfunc                                               
;/* End Function:RMP_Disable_Int *********************************************/

;/* Begin Function:RMP_Enable_Int *********************************************
;Description    : The function for enabling all interrupts. Does not allow nesting.
;Input          : None.
;Output         : None.    
;Register Usage : None.                                  
;*****************************************************************************/
RMP_Enable_Int  .asmfunc
                ;Enable all interrupts.
                CPSIE           I               
                BX              LR
                .endasmfunc
;/* End Function:RMP_Enable_Int **********************************************/

;/* Begin Function:RMP_Mask_Int ***********************************************
;Description    : The function for masking & unmasking interrupts. Does not allow masking.
;Input          : R0 - The new basepri to set.
;Output         : None.    
;Register Usage : None.                                  
;*****************************************************************************/
RMP_Mask_Int    .asmfunc
                ;Mask some interrupts.
                ;MSR             BASEPRI,R0
                ;BX              LR
                .endasmfunc
;/* End Function:RMP_Mask_Int ************************************************/

;/* Begin Function:RMP_MSB_Get ************************************************
;Description    : Get the MSB of the word.
;Input          : ptr_t Val - The value.
;Output         : None.
;Return         : ptr_t - The MSB position.   
;Register Usage : None. 
;*****************************************************************************/
RMP_MSB_Get     .asmfunc
                CLZ             R1,R0
                MOVS            R0,#31
                SUBS            R0,R0,R1
                BX              LR
                .endasmfunc
;/* End Function:RMP_MSB_Get *************************************************/

;/* Begin Function:_RMP_Yield *************************************************
;Description : Trigger a yield to another thread.
;Input       : None.
;Output      : None.                                      
;*****************************************************************************/
_RMP_Yield      .asmfunc
                PUSH            {R0-R1}
                
                SVC             #0                     ;Trigger the SVC instruction

                DSB                                    ;Data and instruction barrier
                ISB
                
                POP             {R0-R1}                
                BX              LR   
                .endasmfunc                                                
;/* End Function:_RMP_Yield **************************************************/

;/* Begin Function:_RMP_Start *************************************************
;Description : Jump to the user function and will never return from it.
;Input       : None.
;Output      : None.                                      
;*****************************************************************************/
_RMP_Start      .asmfunc
                ;Should never reach here
                SUBS            R1,R1,#64              ;This is how we push our registers so move forward

             ;   MSR             PSP,R1                 ;Set the stack pointer
                MOVS            R4,#0x02               ;Previleged thread mode
           ;     MSR             CONTROL,R4
                
                DSB                                    ;Data and instruction barrier
                ISB
                
                BLX             R0                     ;Branch to our target
Loop:           B               Loop                      ;Capture faults
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
_svc
PendSV_Handler  .asmfunc
;                MRS            R0,PSP                     ;Spill all the registers onto the user stack
                STMDB           R0!,{R4-R11,LR}
                
                BL              RMP_Save_Ctx               ;Save extra context
                
;                LDR            R1,=RMP_Cur_SP             ;Save The SP to control block.
                STR             R0,[R1]
                
                BL              _RMP_Get_High_Rdy          ;Get the highest ready task.
                
 ;               LDR            R1,=RMP_Cur_SP             ;Load the SP.
                LDR             R0,[R1]
                
                BL              RMP_Load_Ctx               ;Load extra context

                LDMIA           R0!,{R4-R11,LR}
 ;               MSR            PSP,R0
                
                BX              LR                         ;The LR will indicate whether we are using FPU.
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
SysTick_Handler .asmfunc
                PUSH            {LR}
                
                MOVS            R0,#0x01                     ;We are not using tickless.
                BL              _RMP_Tick_Handler
                
                POP             {PC}
                .endasmfunc
;/* End Function:SysTick_Handler *********************************************/

_dabort         .asmfunc
                B               _dabort
                .endasmfunc

_prefetch       .asmfunc
                B               _prefetch
                .endasmfunc

_undef          .asmfunc
                B               _undef
                .endasmfunc

phantomInterrupt .asmfunc
                B               phantomInterrupt
                .endasmfunc
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
