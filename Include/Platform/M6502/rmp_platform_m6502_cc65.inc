;/*****************************************************************************
;Filename    : rmp_platform_m6502_cc65.inc
;Author      : pry
;Date        : 22/05/2024
;Description : The assembly part of the RMP RTOS. This is for MOS 6502, and
;              contains macros to be included in all interrupt assembly code
;              as needed.
;*****************************************************************************/
    
;/* Header *******************************************************************/
    .CODE
;/* End Header ***************************************************************/

;/* Import *******************************************************************/
    ; The current thread stack
    .import             _RMP_SP_Cur
    ; The interrupt active flag
    .import             _RMP_M6502_Int_Act
    ; The yield pending flag
    .import             __RMP_M6502_Yield_Pend
    ; Extract highest priority running thread
    .import             __RMP_Run_High
    ; Handler for M6502 timer interrupt
    .import             __RMP_M6502_Tim_Handler
;/* End Import ***************************************************************/

;/* Macro ********************************************************************/

;/* End Macro ****************************************************************/

;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
