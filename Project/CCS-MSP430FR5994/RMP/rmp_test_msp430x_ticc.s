;/*****************************************************************************
;Filename    : rmp_test_msp430x_ticc.s
;Author      : pry
;Date        : 25/02/2018
;Description : The test specific assembly vectors for MSP430X.
;*****************************************************************************/

;/* Begin Header *************************************************************/
    .text
    .sect               ".text:_isr"
    .ALIGN              2
    .include            "Platform/MSP430/rmp_platform_msp430x_ticc.inc"
;/* End Header ***************************************************************/

;/* Begin Exports ************************************************************/
    ;The systick timer routine
    .DEF                TIMER0_A0_VECTOR
    ;The test timer routine
    .DEF                TIMER1_A0_VECTOR
;/* End Exports **************************************************************/

;/* Begin Imports ************************************************************/
    ;Handler for MSP430 timer interrupt
    .GLOBAL             TIM1_IRQHandler
;/* End Imports **************************************************************/

;/* Begin Function:TIMER0_A0_VECTOR *******************************************
;Description : The timer used to generate ticks for the OS.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
TIMER0_A0_VECTOR:       .ASMFUNC
    RMP_MSP430_INT_SAVE
    ;Get the highest ready task.
    CALLA               #_RMP_MSP430_Tim_Handler
    RMP_MSP430_INT_RESTORE
    .ENDASMFUNC
;/* End Function:TIMER0_A0_VECTOR ********************************************/

;/* Begin Function:TIMER1_A0_VECTOR *******************************************
;Description : The timer used to generate ticks for the interrupt latency test.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
TIMER1_A0_VECTOR:       .ASMFUNC
    RMP_MSP430_INT_SAVE
    ;Get the highest ready task.
    CALLA               #TIM1_IRQHandler
    RMP_MSP430_INT_RESTORE
    .ENDASMFUNC
;/* End Function:TIMER0_A0_VECTOR ********************************************/

;/* Need to tailor these to specific microcontrollers - asm not portable */
    .sect                 ".int41"
    .short                TIMER1_A0_VECTOR
    .sect                 ".int45"
    .short                TIMER0_A0_VECTOR
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
