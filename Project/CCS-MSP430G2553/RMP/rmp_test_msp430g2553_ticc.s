;/*****************************************************************************
;Filename    : rmp_test_msp430g2553_ticc.s
;Author      : pry
;Date        : 25/02/2018
;Description : The test specific assembly vectors for MSP430G2553.
;*****************************************************************************/

;/* Header *******************************************************************/
    .text
    .sect               ".text:_isr"
    .align              2
    .include            "Platform/MSP430/rmp_platform_msp430_none_ticc.inc"
;/* End Header ***************************************************************/

;/* Import *******************************************************************/
    ;Tailored tickless handler
    .global             _RMP_MSP430_Tickless_Handler
;/* End Import ***************************************************************/

;/* Export *******************************************************************/
    ;The systick timer routine
    .def                TIMER0_A0_VECTOR
;/* End Export ***************************************************************/

;/* Function:TIMERA0_VECTOR ***************************************************
;Description : The timer used to generate ticks for the OS.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
TIMER0_A0_VECTOR:       .asmfunc
    RMP_MSP430_INT_SAVE_NONE
    ;Get the highest ready task.
    CALL                #_RMP_MSP430_Tickless_Handler
    RMP_MSP430_INT_LOAD_NONE
    .endasmfunc
;/* End Function:TIMERA0_VECTOR **********************************************/

;/* Need to tailor these to specific microcontrollers - asm not portable */
    .sect                 ".int09"
    .short                TIMER0_A0_VECTOR
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
