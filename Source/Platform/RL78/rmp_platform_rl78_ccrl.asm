;/*****************************************************************************
;Filename    : rmp_platform_rl78_ccrl.asm
;Author      : lht
;Date        : 23/04/2024
;Description : The assembly part of the RMP RTOS. This is for RL78/G14.
;*****************************************************************************/

;/* Import *******************************************************************/
    ;/* The real task switch handling function */
    .EXTERN             __RMP_Run_High
    ;/* The real systick handler function */
    .EXTERN             __RMP_Tim_Handler
    ;/* The PID of the current thread */
    .EXTERN             _RMP_Thd_Cur
    ;/* The stack address of current thread */
    .EXTERN             _RMP_SP_Cur
    ;The kernel s
    .EXTERN             __RMP_RL78_SP_Kern
;/* End Import ***************************************************************/
            
;/* Export *******************************************************************/
    ;/* Disable all interrupts */
    .PUBLIC             _RMP_Int_Disable
    ;/* Enable all interrupts */
    .PUBLIC             _RMP_Int_Enable
    ;/* Start the first thread */
    .PUBLIC             __RMP_Start
    ;/* yield the thread */
    .PUBLIC             __RMP_RL78_Yield
;/* End Export ***************************************************************/

;/* Function:RMP_Int_Disable **************************************************
;Description : The function for disabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_Int_Disable:
    DI
    RET
;/* End Function:RMP_Int_Disable *********************************************/

;/* Function:RMP_Int_Enable ***************************************************
;Description : The function for enabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_Int_Enable:
    EI
    RET
;/* End Function:RMP_Int_Enable **********************************************/

;/* Function:_RMP_Start *******************************************************
;Description : Jump to the user function and will never return from it.
;Input       : rmp_ptr_t Entry - PC , rmp_ptr_t Stack - SP
;Output      : None.
;Return      : None.
;*****************************************************************************/
__RMP_Start:
    ;rmp_ptr_t Entry in AX   rmp_ptr_t Stack in BC
   ;Save kernel stack
    MOVW                DE,AX
    MOVW                AX,SP
    MOVW                !__RMP_RL78_SP_Kern,AX
    ;Make up return address
    MOVW                AX,BC
    MOVW                SP,AX
    MOVW                AX,DE
    BR                  AX
    ; Dummy return
    RET
;/* End Function:_RMP_Start **************************************************/

;/* Function:__RMP_RL78_Yield *************************************************
;Description : Trigger a yield to another thread.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
__RMP_RL78_Yield:
    ;Disable all interrupts
    DI
    ;Leave hole for PSW/PC to fill in later on
    PUSH                AX
    PUSH                AX
    ;Push all GP regs
    PUSH                AX
    PUSH                BC
    PUSH                DE
    PUSH                HL
    ;Push segment regs
    MOV                 A,ES
    MOV                 X,A
    MOV                 A,CS
    PUSH                AX

    ;Fill PC/PSW back in with interrupt enabled
    MOVW                DE, SP
    MOVW                AX, #LOWW(__RMP_RL78_Yield_Skip)
    MOVW                [DE+10],AX
    MOV                 A,PSW
    AND                 A,#0x7F
    MOV                 X,#LOW(HIGHW(__RMP_RL78_Yield_Skip))
    MOVW                [DE+12],AX

    ;Choose highest priority ready thread with kernel stack
    MOVW                AX,SP
    ;Load kernel SP
    MOVW                !_RMP_SP_Cur, AX
    MOVW                AX,!__RMP_RL78_SP_Kern
    MOVW                SP, AX
    CALL                !!__RMP_Run_High
    MOVW                AX,!_RMP_SP_Cur
    MOVW                SP, AX

    ;Pop segment regs
    POP                 AX
    MOV                 CS,A
    MOV                 A,X
    MOV                 ES,A
    ;Pop all GP regs
    POP                 HL
    POP                 DE
    POP                 BC
    POP                 AX
    ;Return from simulated interrupt
    RETI
__RMP_RL78_Yield_Skip:
    RET
;/* End Function:__RMP_RL78_Yield ********************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
