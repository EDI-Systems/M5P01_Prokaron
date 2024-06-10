;/*****************************************************************************
;Filename    : rmp_test_famicom_cc65.s
;Author      : pry
;Date        : 10/04/2012
;Description : The test specific assembly vectors for FAMICOM.
;*****************************************************************************/

;/* Header *******************************************************************/
    .include            "Platform/M6502/rmp_platform_m6502_cc65.inc"
;/* End Header ***************************************************************/

;/* Import *******************************************************************/
    .import             _NAMCO_IRQ_Handler
;/* End Import ***************************************************************/

;/* Export *******************************************************************/
    .export             IRQ_Handler
;/* End Export ***************************************************************/

;/* Function:IRQ_Handler ******************************************************
;Description : Handle all IRQs in the system. We leave IRQ identification to
;              the C code which makes things easier.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
IRQ_Handler:
    RMP_M6502_INT_SAVE
    ; Read the NAMCO IRQ counter and see if it came from the mapper
    LDA                 $5000
    CMP                 #$FF
    BNE                 IRQ_Handler_Timer
    ; Some simulators disagree on what should be read from the high register.
    ; MESEN thinks that the interrupt enable bit should be read, but FCEUX
    ; does not think so. Thus, thanks to the fact that the enable bit being
    ; the MSB, if the value is >= $7F, the NAMCO IRQ must have fired.
    LDA                 $5800
    AND                 #$7F
    CMP                 #$7F
    BNE                 IRQ_Handler_Timer
    ; Yes, the NAMCO counter fired (if the APU is firing too, handle it later)
    JSR                 _NAMCO_IRQ_Handler
    JMP                 IRQ_Handler_Timer_Skip
IRQ_Handler_Timer:
    ; No, it must be from the APU frame counter
    JSR                 __RMP_M6502_Tim_Handler
IRQ_Handler_Timer_Skip:
    RMP_M6502_INT_LOAD
;/* End Function:IRQ_Handler *************************************************/

;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
