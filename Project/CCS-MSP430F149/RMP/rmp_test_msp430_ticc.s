;/*****************************************************************************
;Filename    : rmp_test_msp430_ticc.s
;Author      : pry
;Date        : 25/02/2018
;Description : The test specific assembly vectors for MSP430.
;*****************************************************************************/

;/* Begin Header *************************************************************/
    .text
    .sect               ".text:_isr"
    .ALIGN              2
    .include            "Platform/MSP430/rmp_platform_msp430_ticc.inc"
;/* End Header ***************************************************************/

;/* Begin Exports ************************************************************/
    ;The systick timer routine
    .DEF                TIMERA0_VECTOR
    ;The test timer routine
    .DEF                TIMERB0_VECTOR
;/* End Exports **************************************************************/

;/* Begin Imports ************************************************************/
    ;Handler for MSP430 timer interrupt
    .GLOBAL             TIM1_IRQHandler
;/* End Imports **************************************************************/

;/* Begin Function:TIMERA0_VECTOR *********************************************
;Description : The timer used to generate ticks for the OS.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
TIMERA0_VECTOR:         .ASMFUNC
    RMP_MSP430_INT_SAVE
    ;Get the highest ready task.
    CALL                #_RMP_MSP430_Tim_Handler
    RMP_MSP430_INT_RESTORE
    .ENDASMFUNC
;/* End Function:TIMERA0_VECTOR **********************************************/

;/* Begin Function:TIMERB0_VECTOR *********************************************
;Description : The timer used to generate ticks for the interrupt latency test.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
TIMERB0_VECTOR:         .ASMFUNC
    RMP_MSP430_INT_SAVE
    ;Get the highest ready task.
    CALL                #TIM1_IRQHandler
    RMP_MSP430_INT_RESTORE
    .ENDASMFUNC
;/* End Function:TIMERB0_VECTOR **********************************************/

;/* Need to tailor these to specific microcontrollers - asm not portable */
    .sect                 ".int06"
    .short                TIMERA0_VECTOR
    .sect                 ".int13"
    .short                TIMERB0_VECTOR
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
