;/*****************************************************************************
;Filename    : rmp_platform_rl78_ccrl.inc
;Author      : pry
;Date        : 25/02/2018
;Description : The assembly part of the RMP RTOS. This is for RL78, and
;              contains macros to be included in all interrupt assembly code
;              as needed.
;*****************************************************************************/

;/* Import *******************************************************************/
    ;/* The kernel stack */
    .EXTERN             __RMP_RL78_SP_Kern
    ;/* The kernel CS/ES */
    .EXTERN             __RMP_RL78_CSES_Kern
    ;/* The current thread stack */
    .EXTERN             _RMP_SP_Cur
    ;/* The yield pending flag */
    .EXTERN             _RMP_Sched_Pend
    ;/* Extract highest priority running thread */
    .EXTERN             __RMP_Run_High
    ;/* Handler for RL78 timer interrupt */
    .EXTERN             __RMP_RL78_Tim_Handler
;/* End Import ***************************************************************/

;/* Macro ********************************************************************/
;/* Push everything to stack *************************************************/
RMP_RL78_INT_SAVE       .MACRO
    PUSH                AX
    PUSH                BC
    PUSH                DE
    PUSH                HL
    MOV                 A,ES
    MOV                 X,A
    MOV                 A,CS
    PUSH                AX
    MOVW                AX,SP
    MOVW                !_RMP_SP_Cur,AX
    MOVW                AX,!__RMP_RL78_CSES_Kern
    MOV                 CS,A
    MOV                 A,X
    MOV                 ES,A
    MOVW                AX,!__RMP_RL78_SP_Kern
    MOVW                SP,AX
    .ENDM
;/* Load everything from stack, and deal with pending yields if there is *****/
RMP_RL78_INT_RESTORE    .MACRO
    .LOCAL              __RMP_RL78_Yield_Skip
    CMP0                !_RMP_Sched_Pend
    BZ                  $__RMP_RL78_Yield_Skip
    CALL                !!__RMP_Run_High
__RMP_RL78_Yield_Skip:
    MOVW                AX,!_RMP_SP_Cur
    MOVW                SP,AX
    POP                 AX
    MOV                 CS,A
    MOV                 A,X
    MOV                 ES,A
    POP                 HL
    POP                 DE
    POP                 BC
    POP                 AX
    RETI
    .ENDM
;/* End Macro ****************************************************************/

;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/

