;/*****************************************************************************
;Filename    : platform_cmx_RVM.s
;Author      : pry
;Date        : 09/02/2018
;Description : The assembly part of the RMP RTOS for the RVM virtual machine.
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
;The ARM Cortex-M4 also include a single-accuracy FPU.
;*****************************************************************************/
            
;/* Begin Header *************************************************************/
                ;The align is "(2^3)/8=1(Byte)." In fact it does not take effect.            
                AREA            INIT,CODE,READONLY,ALIGN=3
                
                THUMB
                REQUIRE8
                PRESERVE8
;/* End Header ***************************************************************/

;/* Begin Exports ************************************************************/
                ;Get the MSB                              
                EXPORT          RMP_MSB_Get
                ;Start the first thread
                EXPORT          _RMP_Start          
;/* End Exports **************************************************************/

;/* Begin Imports ************************************************************/

;/* End Imports **************************************************************/

;/* Begin Function:RMP_MSB_Get ************************************************
;Description    : Get the MSB of the word.
;Input          : ptr_t Val - The value.
;Output         : None.
;Return         : ptr_t - The MSB position.   
;Register Usage : None. 
;*****************************************************************************/
RMP_MSB_Get
                CLZ      R1,R0
                MOV      R0,#31
                SUB      R0,R1
                BX       LR
;/* End Function:RMP_MSB_Get *************************************************/

;/* Begin Function:_RMP_Start *************************************************
;Description : Jump to the user function and will never return from it.
;Input       : None.
;Output      : None.                                      
;*****************************************************************************/
_RMP_Start
                ;Should never reach here
                SUB       R1,#16                       ; This is how we push our registers so move forward
                MOV       SP,R1                        ; Set the stack pointer
                
                DSB                                    ; Data and instruction barrier
                ISB
                
                BLX       R0                           ; Branch to our target
;/* End Function:_RMP_Start **************************************************/

                END
;/* End Of File **************************************************************/

;/* Copyright (C) 2011-2013 Evo-Devo Instrum. All rights reserved ************/
