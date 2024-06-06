;/*****************************************************************************
;Filename    : rmp_test_tms320f2812_ticc.s
;Author      : zr (ran zhang)
;Date        : 27/04/2024
;Description : The test specific assembly vectors for C28X.
;*****************************************************************************/

;/* Header *******************************************************************/
    .text
    .sect                   ".text:_isr"
    .align                  1
    .include                "Platform\C28X\rmp_platform_c28x_ticc.inc"

    ;Make things compatible for both COFF and EABI
    .if __TI_EABI__
    .asg                    INT14_ISR,_INT14_ISR
    .asg                    INT13_ISR,_INT13_ISR
    .asg                    TIM1_IRQHandler,_TIM1_IRQHandler
    .endif
;/* End Header ***************************************************************/

;/* Export *******************************************************************/
    ;The systick timer routine
    .global                 _INT14_ISR
    ;The test timer routine
    .global                 _INT13_ISR
;/* End Export ***************************************************************/

;/* Import *******************************************************************/
    ;Handler for C28X test timer interrupt
    .global                 _TIM1_IRQHandler
;/* End Import ***************************************************************/

;/* Function:_INT14_ISR *******************************************************
;Description : The timer used to generate ticks for the OS.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_INT14_ISR:                 .asmfunc
    NOP
    RMP_C28X_INT_SAVE_NONE
    ;Get the highest ready task.
    LCR                     __RMP_C28X_Tim_Handler
    RMP_C28X_INT_LOAD_NONE
    .endasmfunc
;/* End Function:_INT14_ISR **************************************************/

;/* Function:_INT13_ISR *******************************************************
;Description : The timer used to generate ticks for the interrupt latency test.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_INT13_ISR:                 .asmfunc
    RMP_C28X_INT_SAVE_NONE
    ;Get the highest ready task.
    LCR                     _TIM1_IRQHandler
    RMP_C28X_INT_LOAD_NONE
    .endasmfunc
;/* End Function:_INT13_ISR **************************************************/

;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
