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
    .DEF                TIMER0_A0_VECTOR
;/* End Exports **************************************************************/

;/* Begin Imports ************************************************************/
    ;Tailored tickless handler
    .GLOBAL             _RMP_MSP430_Tickless_Handler
;/* End Imports **************************************************************/

;/* Begin Function:TIMERA0_VECTOR *********************************************
;Description : The timer used to generate ticks for the OS.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
TIMER0_A0_VECTOR:       .ASMFUNC
    RMP_MSP430_INT_SAVE
    ;Get the highest ready task.
    CALL                #_RMP_MSP430_Tickless_Handler
    RMP_MSP430_INT_RESTORE
    .ENDASMFUNC
;/* End Function:TIMERA0_VECTOR **********************************************/

;/* Need to tailor these to specific microcontrollers - asm not portable */
    .sect                 ".int09"
    .short                TIMER0_A0_VECTOR
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
