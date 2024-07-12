;/*****************************************************************************
;Filename    : rmp_platform_rl78_ccrl.asm
;Author      : lht
;Date        : 23/04/2024
;Description : The assembly part of the RMP RTOS. This is for RL78.
;*****************************************************************************/

;/* The RL78 Architecture *****************************************************
;X      : General register (lower half of AX).
;A      : General register (higher half of AX).
;C      : General register (lower half of BC).
;B      : General register (higher half of BC).
;E      : General register (lower half of DE).
;D      : General register (higher half of DE).
;L      : General register (lower half of HL).
;H      : General register (higher half of HL).
;         Contrary to popular convention where C is higher than B, B is higher
;         than C. The machine is little-endian, and the same applies to all
;         other general registers.
;SP     : Stack pointer, 16-bit; stack in lower 64KiB.
;PC     : Program counter, 20-bit.
;PSW    : Program status word, 8-bit.
;ES     : Extra segment register, 8-bit, only used when ES: prefix is present.
;CS     : Code segment register, 8-bit, only used in register indirect jumps.
;         Note that the code segment register behaves very differently than
;         what most people will have in mind: it is not prefixed to the fetch
;         address but is only used for jumping. Due to this reason, the CS is
;         not really a part of PC and is not pushed to stack by hardware during
;         interrupt entry; the PC is long enough to address the whole program
;         space by itself.
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
    ;/* The kernel SP */
    .EXTERN             __RMP_RL78_SP_Kern
    ;/* The kernel CS/ES */
    .EXTERN             __RMP_RL78_CSES_Kern
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
;Input       : rmp_ptr_t Entry AX - PC
;              rmp_ptr_t Stack BC - SP
;Output      : None.
;Return      : None.
;*****************************************************************************/
__RMP_Start:
    ;/* Save kernel stack */
    MOVW                DE,AX
    MOVW                AX,SP
    MOVW                !__RMP_RL78_SP_Kern,AX
    ;/* Save segment registers */
    MOV                 A,ES
    MOV                 X,A
    MOV                 A,CS
    MOVW                !__RMP_RL78_CSES_Kern,AX
    ;/* Make up return address */
    MOVW                AX,BC
    MOVW                SP,AX
    MOVW                AX,DE
    BR                  AX
    ;/* Dummy return */
    RET
;/* End Function:_RMP_Start **************************************************/

;/* Function:__RMP_RL78_Yield *************************************************
;Description : Trigger a yield to another thread.
;              The RL78's call instruction actually pushes PC and PSW, hence the
;              call entry and interrupt entry is the same. However, this does
;              not appear in the official manual, and we refrain from relying on
;              such unofficial features.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
__RMP_RL78_Yield:
    ;/* Disable all interrupts */
    DI
    ;/* Leave hole for PSW/PC to fill in later on */
    PUSH                AX
    PUSH                AX
    ;/* Push all GP regs */
    PUSH                AX
    PUSH                BC
    PUSH                DE
    PUSH                HL
    ;/* Push segment registers */
    MOV                 A,ES
    MOV                 X,A
    MOV                 A,CS
    PUSH                AX

    ;/* Fill PC/PSW back in with interrupt enabled */
    MOVW                DE,SP
    MOVW                AX,#LOWW(__RMP_RL78_Yield_Skip)
    MOVW                [DE+10],AX
    MOV                 A,PSW
    AND                 A,#0x7F
    MOV                 X,#LOW(HIGHW(__RMP_RL78_Yield_Skip))
    MOVW                [DE+12],AX

    ;/* Save thread SP */
    MOVW                AX,SP
    MOVW                !_RMP_SP_Cur,AX
    ;/* Load kernel segment registers */
    MOVW                AX,!__RMP_RL78_CSES_Kern
    MOV                 CS,A
    MOV                 A,X
    MOV                 ES,A
    ;/* Load kernel SP */
    MOVW                AX,!__RMP_RL78_SP_Kern
    MOVW                SP,AX
    ;/* Choose highest priority ready thread with kernel stack */
    CALL                !!__RMP_Run_High
    ;/* Restore thread SP */
    MOVW                AX,!_RMP_SP_Cur
    MOVW                SP,AX

    ;/* Pop segment regs */
    POP                 AX
    MOV                 CS,A
    MOV                 A,X
    MOV                 ES,A
    ;/* Pop all GP regs */
    POP                 HL
    POP                 DE
    POP                 BC
    POP                 AX
    ;/* Return from simulated interrupt */
    RETI
__RMP_RL78_Yield_Skip:
    RET
;/* End Function:__RMP_RL78_Yield ********************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
