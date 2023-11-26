;/*****************************************************************************
;Filename    : rmp_test_msp430fr5994_ticc.s
;Author      : pry
;Date        : 25/02/2018
;Description : The test specific assembly vectors for MSP430FR5994.
;*****************************************************************************/

;/* Header *******************************************************************/
    .text
    .sect               ".text:_isr"
    .align              2
    .include            "Platform/MSP430/rmp_platform_msp430x_ticc.inc"
;/* End Header ***************************************************************/

;/* Export *******************************************************************/
    ;The systick timer routine
    .def                TIMER0_A0_VECTOR
    ;The test timer routine
    .def                TIMER1_A0_VECTOR
;/* End Export ***************************************************************/

;/* Import *******************************************************************/
    ;Handler for MSP430 timer interrupt
    .global             TIM1_IRQHandler
;/* End Import ***************************************************************/

;/* Function:TIMER0_A0_VECTOR *************************************************
;Description : The timer used to generate ticks for the OS.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
TIMER0_A0_VECTOR:       .asmfunc
    RMP_MSP430_INT_SAVE
    ;Get the highest ready task.
    CALLA               #_RMP_MSP430_Tim_Handler
    RMP_MSP430_INT_RESTORE
    .endasmfunc
;/* End Function:TIMER0_A0_VECTOR ********************************************/

;/* Function:TIMER1_A0_VECTOR *************************************************
;Description : The timer used to generate ticks for the interrupt latency test.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
TIMER1_A0_VECTOR:       .asmfunc
    RMP_MSP430_INT_SAVE
    ;Get the highest ready task.
    CALLA               #TIM1_IRQHandler
    RMP_MSP430_INT_RESTORE
    .endasmfunc
;/* End Function:TIMER0_A0_VECTOR ********************************************/

;/* Need to tailor these to specific microcontrollers - asm not portable */
    .sect                 ".int41"
    .short                TIMER1_A0_VECTOR
    .sect                 ".int45"
    .short                TIMER0_A0_VECTOR
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
