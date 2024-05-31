;/**********************************************************************************************************************
; * DISCLAIMER
; * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
; * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
; * applicable laws, including copyright laws.
; * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
; * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
; * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
; * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
; * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
; * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
; * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
; * this software. By using this software, you agree to the additional terms and conditions found by accessing the
; * following link:
; * http://www.renesas.com/disclaimer
; *
; * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
; *********************************************************************************************************************/;---------------------------------------------------------------------
;	_stkinit
;
;	void _stkinit(void __near * stackbss);
;
;	input:
;		stackbss = AX (#LOWW(_stackend))
;	output:
;		NONE
;---------------------------------------------------------------------

;   NOTE       : THIS IS A TYPICAL EXAMPLE.

	.PUBLIC	_stkinit

.textf	.CSEG	TEXTF
_stkinit:
	MOVW	HL,AX		; stack_end_addr
	MOV	[SP+3],#0x00	; [SP+0]-[SP+2] for return address
	MOVW	AX,SP
	SUBW	AX,HL		; SUBW  AX,#LOWW _@STEND
	BNH	$LSTINIT3	; goto end
	SHRW	AX,5		; loop count for 32 byte transfer
	MOVW	BC,AX
	CLRW	AX
LSTINIT1:
	CMPW	AX,BC
	BZ	$LSTINIT2
	MOVW	[HL],AX
	MOVW	[HL+2],AX
	MOVW	[HL+4],AX
	MOVW	[HL+6],AX
	MOVW	[HL+8],AX
	MOVW	[HL+10],AX
	MOVW	[HL+12],AX
	MOVW	[HL+14],AX
	MOVW	[HL+16],AX
	MOVW	[HL+18],AX
	MOVW	[HL+20],AX
	MOVW	[HL+22],AX
	MOVW	[HL+24],AX
	MOVW	[HL+26],AX
	MOVW	[HL+28],AX
	MOVW	[HL+30],AX
	XCHW	AX,HL
	ADDW	AX,#0x20
	XCHW	AX,HL
	DECW	BC
	BR	$LSTINIT1
LSTINIT2:
	MOVW	AX,SP
	CMPW	AX,HL
	BZ	$LSTINIT3	; goto end
	CLRW	AX
	MOVW	[HL],AX
	INCW	HL
	INCW	HL
	BR	$LSTINIT2
LSTINIT3:
	RET
