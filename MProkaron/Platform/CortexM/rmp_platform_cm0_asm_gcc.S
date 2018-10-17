;/*****************************************************************************
;Filename    : rmp_platform_cm0_asm.s
;Author      : pry
;Date        : 10/04/2012
;Description : The assembly part of the RMP RTOS. This is for Cortex-M0/0+/1.
;*****************************************************************************/

;/* The ARM Cortex-M0 Architecture ********************************************
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
    AREA                ARCH,CODE,READONLY,ALIGN=3                     
                
    THUMB
    REQUIRE8
    PRESERVE8
;/* End Header ***************************************************************/

;/* Begin Exports ************************************************************/
    ;Disable all interrupts
    EXPORT              RMP_Disable_Int      
    ;Enable all interrupts            
    EXPORT              RMP_Enable_Int
    ;Mask/unmask interrupt dummy
    EXPORT              RMP_Mask_Int
    ;Get the MSB                              
    EXPORT              RMP_MSB_Get
    ;Start the first thread
    EXPORT              _RMP_Start
    ;The PendSV trigger
    EXPORT              _RMP_Yield
    ;The system pending service routine              
    EXPORT              PendSV_Handler 
    ;The systick timer routine              
    EXPORT              SysTick_Handler                               
;/* End Exports **************************************************************/

;/* Begin Imports ************************************************************/
    ;The real task switch handling function
    IMPORT              _RMP_Get_High_Rdy 
    ;The real systick handler function
    IMPORT              _RMP_Tick_Handler
    ;The PID of the current thread                     
    IMPORT              RMP_Cur_Thd
    ;The stack address of current thread
    IMPORT              RMP_Cur_SP        
    ;Save and load extra contexts, such as FPU, peripherals and MPU
    IMPORT              RMP_Save_Ctx
    IMPORT              RMP_Load_Ctx
;/* End Imports **************************************************************/

;/* Begin Function:RMP_Disable_Int ********************************************
;Description : The function for disabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/    
RMP_Disable_Int
    ;Disable all interrupts (I is primask,F is Faultmask.)
    CPSID               I                                                       
    BX                  LR                                                 
;/* End Function:RMP_Disable_Int *********************************************/

;/* Begin Function:RMP_Enable_Int *********************************************
;Description : The function for enabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
RMP_Enable_Int
    ;Enable all interrupts.
    CPSIE               I                                                          
    BX                  LR
;/* End Function:RMP_Enable_Int **********************************************/

;/* Begin Function:RMP_Mask_Int ***********************************************
;Description : Cortex-M0 does not allow masking and this is provided as dummy.
;Input       : rmp_ptr_t R0 - The new BASEPRI to set.
;Output      : None.
;Return      : None.
;*****************************************************************************/
RMP_Mask_Int                                                   
    BX                  LR
;/* End Function:RMP_Mask_Int ************************************************/

;/* Begin Function:RMP_MSB_Get ************************************************
;Description : Get the MSB of the word.
;Input       : rmp_ptr_t R0 - The value.
;Output      : None.
;Return      : rmp_ptr_t R0 - The MSB position. 
;*****************************************************************************/
    MACRO
$Label CHECK_BITS       $BITS
    LSRS                R2,R1,#$BITS
    BEQ                 $Label.Skip
    ADDS                R0,#$BITS
    MOV                 R1,R2
$Label.Skip
    MEND
                
    ;Always 21 instructions no matter what
RMP_MSB_Get
    ;See if the word passed in is zero. In this case, we return -1.
    CMP                 R0,#0
    BEQ                 ZERO
    MOVS                R1,R0
    MOVS                R0,#0
HEX CHECK_BITS          16
OCT CHECK_BITS          8
QUAD CHECK_BITS         4
BIN CHECK_BITS          2
ONE CHECK_BITS          1
    BX                  LR
ZERO
    SUBS                R0,#1
    BX                  LR
;/* End Function:RMP_MSB_Get *************************************************/

;/* Begin Function:_RMP_Yield *************************************************
;Description : Trigger a yield to another thread.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_Yield
    LDR                 R0,=0xE000ED04      ;The NVIC_INT_CTRL register
    LDR                 R1,=0x10000000      ;Trigger the PendSV          
    STR                 R1,[R0]

    ISB                                     ;Instruction barrier
    BX                  LR
;/* End Function:_RMP_Yield **************************************************/

;/* Begin Function:_RMP_Start *************************************************
;Description : Jump to the user function and will never return from it.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_Start
    ;Should never reach here
    SUBS                R1,#64              ;This is how we push our registers so move forward
    MSR                 PSP,R1              ;Set the stack pointer
    MOVS                R4,#0x02            ;Previleged thread mode
    MSR                 CONTROL,R4

    ISB                                     ;Data and instruction barrier
    BLX                 R0                  ;Branch to our target
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
PendSV_Handler
    ; Make space for register list
    MRS                 R0,PSP              ;Spill all the registers onto the user stack
    SUBS                R0,#36
    MOV                 R1,R0
    STMIA               R1!,{R4-R7}         ;Save low register first due to limitation
    MOV                 R7,LR
    MOV                 R6,R11
    MOV                 R5,R10
    MOV                 R4,R9
    MOV                 R3,R8
    STMIA               R1!,{R3-R7}                
                
    BL                  RMP_Save_Ctx        ;Save extra context
                
    LDR                 R1,=RMP_Cur_SP      ;Save The SP to control block.
    STR                 R0,[R1]
                
    BL                  _RMP_Get_High_Rdy   ;Get the highest ready task.
                
    LDR                 R1,=RMP_Cur_SP      ;Load the SP.
    LDR                 R0,[R1]
                
    BL                  RMP_Load_Ctx        ;Load extra context
                
    MOV                 R1,R0                      
    ADDS                R0,#16
    LDMIA               R0!,{R3-R7}         ;Load high registers first due to limitation
    MOV                 R8,R3
    MOV                 R9,R4
    MOV                 R10,R5
    MOV                 R11,R6
    MOV                 LR,R7
    LDMIA               R1!,{R4-R7}
    MSR                 PSP,R0

    BX                  LR                  ;Cortex-M0 will never have a FPU.
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
SysTick_Handler
    PUSH                {LR}

    MOVS                R0,#0x01            ;We are not using tickless.
    BL                  _RMP_Tick_Handler
    
    POP                 {PC}
    ALIGN
;/* End Function:SysTick_Handler *********************************************/

    END
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
