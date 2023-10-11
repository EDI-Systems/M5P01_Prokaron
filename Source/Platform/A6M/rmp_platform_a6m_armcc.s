;/*****************************************************************************
;Filename    : rmp_platform_a6m_armcc.s
;Author      : pry
;Date        : 10/04/2012
;Description : The ARMv6-M assembly file of the RMP RTOS.
;*****************************************************************************/

;/* The ARMv6-M Architecture ********************************************
;R0-R7:General purpose registers that are accessible. 
;R8-R12:General purpose registers that can only be reached by 32-bit instructions.
;R13:SP/SP_process/SP_main    Stack pointer
;R14:LR                       Link Register(used for returning from a subfunction)
;R15:PC                       Program counter.
;IPSR                         Interrupt Program Status Register.
;APSR                         Application Program Status Register.
;EPSR                         Execute Program Status Register.
;The above 3 registers are saved into the stack in combination(xPSR).
;*****************************************************************************/
            
;/* Begin Header *************************************************************/
                        ;2^3=8 byte alignment.
                        AREA                ARCH, CODE, READONLY, ALIGN=3                     
                                    
                        THUMB
                        REQUIRE8
                        PRESERVE8
;/* End Header ***************************************************************/

;/* Begin Exports ************************************************************/
                        ; Disable all interrupts
                        EXPORT              RMP_Int_Disable      
                        ; Enable all interrupts            
                        EXPORT              RMP_Int_Enable
                        ; Mask/unmask interrupt dummy
                        EXPORT              RMP_Int_Mask
                        ; Get the MSB                              
                        EXPORT              RMP_MSB_Get
                        ; Start the first thread
                        EXPORT              _RMP_Start
                        ; The PendSV trigger
                        EXPORT              _RMP_Yield
                        ; The system pending service routine              
                        EXPORT              PendSV_Handler 
                        ; The systick timer routine              
                        EXPORT              SysTick_Handler                               
;/* End Exports **************************************************************/

;/* Begin Imports ************************************************************/
                        ; The real task switch handling function
                        IMPORT              _RMP_Run_High 
                        ; The real systick handler function
                        IMPORT              _RMP_Tim_Handler
                        ; The PID of the current thread                     
                        IMPORT              RMP_Thd_Cur
                        ; The stack address of current thread
                        IMPORT              RMP_SP_Cur        
                        ; Save and load extra contexts, such as FPU, peripherals and MPU
                        IMPORT              RMP_Ctx_Save
                        IMPORT              RMP_Ctx_Load
;/* End Imports **************************************************************/

;/* Begin Function:RMP_Int_Disable ********************************************
;Description : The function for disabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/    
RMP_Int_Disable         PROC
                        CPSID               I
                        BX                  LR
                        ENDP
;/* End Function:RMP_Int_Disable *********************************************/

;/* Begin Function:RMP_Int_Enable *********************************************
;Description : The function for enabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
RMP_Int_Enable          PROC
                        CPSIE               I
                        BX                  LR
                        ENDP
;/* End Function:RMP_Int_Enable **********************************************/

;/* Begin Function:RMP_Int_Mask ***********************************************
;Description : Cortex-M0 does not allow masking and this is provided as dummy.
;Input       : rmp_ptr_t R0 - The new BASEPRI to set.
;Output      : None.
;Return      : None.
;*****************************************************************************/
RMP_Int_Mask            PROC
                        BX                  LR
                        ENDP
;/* End Function:RMP_Int_Mask ************************************************/

;/* Begin Function:RMP_MSB_Get ************************************************
;Description : Get the MSB of the word. Always 21 instructions no matter what.
;Input       : rmp_ptr_t R0 - The value.
;Output      : None.
;Return      : rmp_ptr_t R0 - The MSB position. 
;*****************************************************************************/
                        MACRO
$Label                  CHECK_BITS          $BITS
                        LSRS                R2, R1, #$BITS
                        BEQ                 $Label.Skip
                        ADDS                R0, #$BITS
                        MOV                 R1, R2
$Label.Skip
                        MEND
                
RMP_MSB_Get             PROC
                        ;See if the word passed in is zero. In this case, we return -1.
                        CMP                 R0, #0
                        BEQ                 ZERO
                        MOVS                R1, R0
                        MOVS                R0, #0
HEX                     CHECK_BITS          16
OCT                     CHECK_BITS          8
QUAD                    CHECK_BITS          4
BIN                     CHECK_BITS          2
ONE                     CHECK_BITS          1
                        BX                  LR
ZERO
                        SUBS                R0, #1
                        BX                  LR
                        ENDP
;/* End Function:RMP_MSB_Get *************************************************/

;/* Begin Function:_RMP_Yield *************************************************
;Description : Trigger a yield to another thread.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_Yield              PROC
                        LDR                 R0, =0xE000ED04     ; The NVIC_INT_CTRL register
                        LDR                 R1, =0x10000000     ; Trigger the PendSV          
                        STR                 R1, [R0]
                        ISB                                     ; Instruction barrier
                        BX                  LR
                        ENDP
;/* End Function:_RMP_Yield **************************************************/

;/* Begin Function:_RMP_Start *************************************************
;Description : Jump to the user function and will never return from it.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_Start              PROC
                        SUBS                R1, #64             ; This is how we push our registers so move forward
                        MSR                 PSP, R1             ; Set the stack pointer
                        MOVS                R4, #0x02           ; Previleged thread mode
                        MSR                 CONTROL, R4
                        ISB                                     ; Data and instruction barrier
                        BX                  R0                  ; Branch to our target
                        ENDP
;/* End Function:_RMP_Start **************************************************/

;/* Begin Function:PendSV_Handler *********************************************
;Description : The PendSV interrupt routine. In fact, it will call a C function
;              directly. The reason why the interrupt routine must be an assembly
;              function is that the compiler may deal with the stack in a different 
;              way when different optimization level is chosen. An assembly function
;              can make way around this problem.
;              However, if your compiler support inline assembly functions, this
;              can also be written in C.
;              Cortex-M0 only have STMIA, will have to live with it.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
PendSV_Handler          PROC
                        ; Make space for register list
                        MRS                 R0, PSP             ; Spill all the registers onto the user stack
                        SUBS                R0, #36
                        MOV                 R1, R0
                        STMIA               R1!, {R4-R7}        ; Save low register first due to limitation
                        MOV                 R7, LR
                        MOV                 R6, R11
                        MOV                 R5, R10
                        MOV                 R4, R9
                        MOV                 R3, R8
                        STMIA               R1!, {R3-R7}                
                
                        BL                  RMP_Ctx_Save        ; Save extra context
                                    
                        LDR                 R1, =RMP_SP_Cur     ; Save The SP to control block.
                        STR                 R0, [R1]
                                    
                        BL                  _RMP_Run_High       ; Get the highest ready task.
                                    
                        LDR                 R1, =RMP_SP_Cur     ; Load the SP.
                        LDR                 R0, [R1]
                                    
                        BL                  RMP_Ctx_Load        ; Load extra context
                                    
                        MOV                 R1, R0                      
                        ADDS                R0, #16
                        LDMIA               R0!, {R3-R7}        ; Load high registers first due to limitation
                        MOV                 R8, R3
                        MOV                 R9, R4
                        MOV                 R10, R5
                        MOV                 R11, R6
                        MOV                 LR, R7
                        LDMIA               R1!, {R4-R7}
                        MSR                 PSP,R0

                        BX                  LR                  ; Cortex-M0 will never have a FPU.
                        ENDP
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
SysTick_Handler         PROC
                        PUSH                {LR}

                        MOVS                R0, #0x01           ; We are not using tickless.
                        BL                  _RMP_Tim_Handler
                        
                        POP                 {PC}
                        ENDP
;/* End Function:SysTick_Handler *********************************************/
                        ALIGN
                        END
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
