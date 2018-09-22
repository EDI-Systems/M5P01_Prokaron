;/*****************************************************************************
;Filename    : rmp_platform_cmx_rvm_asm.s
;Author      : pry
;Date        : 09/02/2018
;Description : The assembly part of the RMP RTOS for the RVM virtual machine.
;*****************************************************************************/

;/* The ARM Cortex-M Architecture *********************************************
;R0-R7:General purpose registers that are accessible. 
;R8-R12:General purpose registers that can only be reached by 32-bit instructions.
;R13:SP/SP_process/SP_main    Stack pointer
;R14:LR                       Link Register(used for returning from a subfunction)
;R15:PC                       Program counter.
;IPSR                         Interrupt Program Status Register.
;APSR                         Application Program Status Register.
;EPSR                         Execute Program Status Register.
;The above 3 registers are saved into the stack in combination(xPSR).
;The ARM Cortex-M4/7 also include a FPU.
;*****************************************************************************/
            
;/* Begin Header *************************************************************/
    ;2^3=8 byte alignment.        
    AREA                INIT,CODE,READONLY,ALIGN=3
                
    THUMB
    REQUIRE8
    PRESERVE8
;/* End Header ***************************************************************/

;/* Begin Exports ************************************************************/
    ;Get the MSB                              
    EXPORT              RMP_MSB_Get
    ;Start the first thread
    EXPORT              _RMP_Start          
;/* End Exports **************************************************************/

;/* Begin Function:RMP_MSB_Get ************************************************
;Description : Get the MSB of the word.
;Input       : rmp_ptr_t R0 - The value.
;Output      : None.
;Return      : rmp_ptr_t R0 - The MSB position.
;*****************************************************************************/
RMP_MSB_Get
    CLZ                 R1,R0
    MOV                 R0,#31
    SUB                 R0,R1
    BX                  LR
;/* End Function:RMP_MSB_Get *************************************************/

;/* Begin Function:_RMP_Start *************************************************
;Description : Jump to the user function and will never return from it.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_Start
    ;Should never reach here
    SUB                 R1,#16              ; This is how we push our registers so move forward
    MOV                 SP,R1               ; Set the stack pointer
                
    ISB                                     ; Instruction barrier
    BLX                 R0                  ; Branch to our target
    ALIGN
;/* End Function:_RMP_Start **************************************************/

    END
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
