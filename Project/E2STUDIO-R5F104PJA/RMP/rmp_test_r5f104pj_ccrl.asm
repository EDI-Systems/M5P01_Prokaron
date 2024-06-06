;/*****************************************************************************
;Filename    : rmp_test_r5f104pj_ccrl.asm
;Author      : lht
;Date        : 30/04/2024
;Description : The test specific assembly vectors for RL78R5F104PJ.
;*****************************************************************************/

;/* Header *******************************************************************/
    $INCLUDE            (rmp_platform_rl78_ccrl.inc)
;/* End Header ***************************************************************/

;/* Export *******************************************************************/
    ;The systick timer routine
    .PUBLIC             _r_tau0_channel0_interrupt
    ;The test timer routine
    .PUBLIC             _r_tau0_channel1_interrupt
;/* End Export ***************************************************************/

;/* Import *******************************************************************/
    ;Handler for RL78 timer interrupt
    .EXTERN             __RMP_RL78_SP_Kern
    ;The current thread stack
    .EXTERN             _RMP_SP_Cur
    ;The interrupt active flag
    .EXTERN             _RMP_RL78_Int_Act
    ;The yield pending flag
    .EXTERN             __RMP_RL78_Yield_Pend
    ;Extract highest priority running thread
    .EXTERN             __RMP_Run_High
    ;The handler function of interrupt _r_tau0_channel1
    .EXTERN             _TDR01_IRQHandler
    ;The handler function of interrupt _r_tau0_channel0
    .EXTERN             __RMP_RL78_Tim_Handler
;/* End Import ***************************************************************/

;/* Function:_r_tau0_channel0_interrupt****************************************
;Description : The timer used to generate ticks for the OS.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_r_tau0_channel0_interrupt .vector 0x002C ;INTTM00
_r_tau0_channel0_interrupt:
    RMP_RL78_INT_SAVE
    CALL                !!__RMP_RL78_Tim_Handler
    RMP_RL78_INT_RESTORE
;/* End Function:_r_tau0_channel0_interrupt **********************************/

;/* Function:_r_tau0_channel1_interrupt ***************************************
;Description : The timer used to generate ticks for the interrupt latency test.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_r_tau0_channel1_interrupt .vector 0x002E ;INTTM01
_r_tau0_channel1_interrupt:
    RMP_RL78_INT_SAVE
    CALL                !!_TDR01_IRQHandler
    RMP_RL78_INT_RESTORE
;/* End Function:_r_tau0_channel1_interrupt **********************************/

;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
