;/*****************************************************************************
;Filename    : rmp_platform_a8mb_iccarm.s
;Author      : pry
;Date        : 05/11/2026
;Licence     : The Unlicense; see LICENSE for details.
;Description : The assembly part of the RMP RTOS. This is for ARMv8-M Baseline
;              (Cortex-M23). This is the IAR (iasmarm) port.
;
;The Baseline has no BASEPRI, no CLZ, no FPU and only STMIA/LDMIA on the low
;registers, so every divergence from the A8MM port shows up here:
;  - RMP_Int_Mask degrades to PRIMASK;
;  - the MSB/LSB helpers live in the kernel (generic C), so there are no
;    _RMP_A8MB_MSB_Get/_RMP_A8MB_LSB_Get here;
;  - PendSV moves the nine software-saved words by hand.
;*****************************************************************************/

;/* The ARMv8-M Baseline Architecture *****************************************
;R0-R7:General purpose registers that are accessible.
;R8-R12:general purpose registers that can only be reached by MOV.
;R13:SP/SP_process/SP_main    Stack pointer (SPLIM registers are available).
;R14:LR                       Link Register(used for returning from a subfunction).
;R15:PC                       Program counter.
;IPSR                         Interrupt Program Status Register.
;APSR                         Application Program Status Register.
;EPSR                         Execute Program Status Register.
;The above 3 registers are saved into the stack in combination(xPSR).
;*****************************************************************************/

;/* Import *******************************************************************/
        ;The real task switch handling function
        EXTERN              _RMP_Run_High
        ;The real systick handler function
        EXTERN              _RMP_Tim_Handler
        ;The stack address of current thread
        EXTERN              RMP_SP_Cur
;/* End Import ***************************************************************/

;/* Export *******************************************************************/
        ;Disable all interrupts
        PUBLIC              RMP_Int_Disable
        ;Enable all interrupts
        PUBLIC              RMP_Int_Enable
        ;Mask/unmask some interrupts
        PUBLIC              RMP_Int_Mask
        ;Start the first thread
        PUBLIC              _RMP_Start
        ;Yield to another thread
        PUBLIC              _RMP_A8MB_Yield
        ;The system pending service routine
        PUBLIC              PendSV_Handler
        ;The systick timer routine
        PUBLIC              SysTick_Handler
;/* End Export ***************************************************************/

;/* Header *******************************************************************/
        MODULE              RMP_A8MB
        SECTION             .text:CODE:NOROOT(3)
        THUMB
        REQUIRE8
        PRESERVE8
;/* End Header ***************************************************************/

;/* Function:RMP_Int_Disable **************************************************
;Description : The function for disabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
RMP_Int_Disable
        CPSID               I
        BX                  LR
;/* End Function:RMP_Int_Disable *********************************************/

;/* Function:RMP_Int_Enable ***************************************************
;Description : The function for enabling all interrupts. Does not allow nesting.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
RMP_Int_Enable
        CPSIE               I
        BX                  LR
;/* End Function:RMP_Int_Enable **********************************************/

;/* Function:RMP_Int_Mask *****************************************************
;Description : The function for masking & unmasking interrupts. Does not allow
;              nesting. There is no BASEPRI on the Baseline, so the level is
;              ignored: any non-zero value masks everything and zero unmasks.
;Input       : rmp_ptr_t R0 - The new BASEPRI to set (non-zero = mask all).
;Output      : None.
;Return      : None.
;*****************************************************************************/
RMP_Int_Mask
        CMP                 R0,#0x00            ;Non-zero masks everything,
        BEQ                 RMP_Int_Unmask      ;zero unmasks
        CPSID               I
        BX                  LR

RMP_Int_Unmask
        CPSIE               I
        BX                  LR
;/* End Function:RMP_Int_Mask ************************************************/

;/* Function:_RMP_Start *******************************************************
;Description : Jump to the user function and will never return from it.
;              The PSPLIM is cleared before the PSP is loaded. The core
;              registers survive a system reset, so a PSPLIM left behind by
;              whatever ran before this image (ROM monitor, previous image)
;              would otherwise make the first exception taken from a thread
;              fail with UFSR.STKOF.
;Input       : rmp_ptr_t R0 - The entry of the thread.
;              rmp_ptr_t R1 - The stack address of the thread.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_Start
        MOVS                R2,#0x00            ;Clear the stale PSPLIM left by
        MSR                 PSPLIM,R2           ;any firmware that ran before us;
        MSR                 PSP,R1              ;core registers survive sysreset
        MOVS                R4,#0x02            ;Previleged thread mode
        MSR                 CONTROL,R4
        ISB
        BX                  R0                  ;Branch to our target
;/* End Function:_RMP_Start **************************************************/

;/* Function:_RMP_A8MB_Yield **************************************************
;Description : Trigger a yield to another thread.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_A8MB_Yield
        LDR                 R0,=0xE000ED04      ;The NVIC_INT_CTRL register
        LDR                 R1,=0x10000000      ;Trigger the PendSV
        STR                 R1,[R0]
        DSB
        BX                  LR
;/* End Function:_RMP_A8MB_Yield *********************************************/

;/* Function:PendSV_Handler ***************************************************
;Description : The PendSV interrupt handler.
;              No FPU means EXC_RETURN bit 4 is always one and the hardware
;              frame is always the standard eight words. The Baseline only
;              has STMIA/LDMIA on the low registers, so the nine software
;              saved words (R4-R11 plus the EXC_RETURN) are moved by hand.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
PendSV_Handler
        MRS                 R0,PSP
        SUBS                R0,#36              ;Save the general purpose registers
        MOV                 R1,R0               ;Equivalent of STMDB R0!,{R4-R11,LR}
        STMIA               R1!,{R4-R7}         ;Save low registers first due to limitation
        MOV                 R7,LR
        MOV                 R6,R11
        MOV                 R5,R10
        MOV                 R4,R9
        MOV                 R3,R8
        STMIA               R1!,{R3-R7}

        LDR                 R1,=RMP_SP_Cur      ;Store The SP to control block
        STR                 R0,[R1]
        BL                  _RMP_Run_High       ;Get the highest ready task
        LDR                 R1,=RMP_SP_Cur      ;Load the SP from control block
        LDR                 R0,[R1]

        MOV                 R1,R0               ;Restore the general purpose registers
        ADDS                R0,#16              ;Equivalent of LDMIA R0!,{R4-R11,LR}
        LDMIA               R0!,{R3-R7}         ;Restore high registers first due to limitation
        MOV                 R8,R3
        MOV                 R9,R4
        MOV                 R10,R5
        MOV                 R11,R6
        MOV                 LR,R7
        LDMIA               R1!,{R4-R7}
        MSR                 PSP,R0

        ;Some chips such as XMC4xxx step AA/step AB may corrupt on this branch.
        ;For those chips, you must manually edit this to PUSH {LR} then POP {PC}
        BX                  LR
;/* End Function:PendSV_Handler **********************************************/

;/* Function:SysTick_Handler **************************************************
;Description : The SysTick interrupt handler.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
SysTick_Handler
        PUSH                {LR}

        MOVS                R0,#0x01            ;We are not using tickless.
        BL                  _RMP_Tim_Handler

        POP                 {PC}
;/* End Function:SysTick_Handler *********************************************/

        LTORG
        END
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
