;/*****************************************************************************
;Filename    : rmp_platform_a8m_iccarm.s
;Author      : pry
;Date        : 03/11/2026
;Licence     : The Unlicense; see LICENSE for details.
;Description : The assembly part of the RMP RTOS. This is for ARMv8-M Mainline
;              (Cortex-M33/M35P/M55/M85). This is the IAR (iasmarm) port.
;
;The MVE (Helium) extension requires no extra software work: the VPR is
;stacked by hardware in the extended stack frame, exactly like S0-S15/FPSCR,
;and we only save the remaining S16-S31 conditionally on EXC_RETURN bit 4.
;*****************************************************************************/

;/* The ARMv8-M Mainline Architecture *****************************************
;R0-R7:General purpose registers that are accessible.
;R8-R12:general purpose registers that can only be reached by 32-bit instructions.
;R13:SP/SP_process/SP_main    Stack pointer (SP_limit registers are available).
;R14:LR                       Link Register(used for returning from a subfunction).
;R15:PC                       Program counter.
;IPSR                         Interrupt Program Status Register.
;APSR                         Application Program Status Register.
;EPSR                         Execute Program Status Register.
;The above 3 registers are saved into the stack in combination(xPSR).
;
;The Cortex-M33/M35P/M55/M85 also include a FPU, and the M55/M85 may include
;the MVE (Helium) extension.
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
        ;Get the MSB/LSB
        PUBLIC              _RMP_A8M_MSB_Get
        PUBLIC              _RMP_A8M_LSB_Get
        ;Start the first thread
        PUBLIC              _RMP_Start
        ;Yield to another thread
        PUBLIC              _RMP_A8M_Yield
        ;The system pending service routine
        PUBLIC              PendSV_Handler
        ;The systick timer routine
        PUBLIC              SysTick_Handler
;/* End Export ***************************************************************/

;/* Header *******************************************************************/
        MODULE              RMP_A8M
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
;Description : The function for masking & unmasking interrupts. Does not allow nesting.
;Input       : rmp_ptr_t R0 - The new BASEPRI to set.
;Output      : None.
;Return      : None.
;*****************************************************************************/
RMP_Int_Mask
        MSR                 BASEPRI,R0
        ;We are not influenced by errata #837070 as the next
        ;instruction is BX LR. Thus we have a free window.
        BX                  LR
;/* End Function:RMP_Int_Mask ************************************************/

;/* Function:_RMP_A8M_MSB_Get *************************************************
;Description : Get the MSB of the word.
;Input       : rmp_ptr_t R0 - The value.
;Output      : None.
;Return      : rmp_ptr_t R0 - The MSB position.
;*****************************************************************************/
_RMP_A8M_MSB_Get
        CLZ                 R1,R0
        MOVS                R0,#31
        SUBS                R0,R1
        BX                  LR
;/* End Function:_RMP_A8M_MSB_Get ********************************************/

;/* Function:_RMP_A8M_LSB_Get *************************************************
;Description : Get the LSB of the word.
;Input       : rmp_ptr_t R0 - The value.
;Output      : None.
;Return      : rmp_ptr_t R0 - The LSB position.
;*****************************************************************************/
_RMP_A8M_LSB_Get
        RBIT                R0,R0
        CLZ                 R0,R0
        BX                  LR
;/* End Function:_RMP_A8M_LSB_Get ********************************************/

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

;/* Function:_RMP_A8M_Yield ***************************************************
;Description : Trigger a yield to another thread.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_A8M_Yield
        LDR                 R0,=0xE000ED04      ;The NVIC_INT_CTRL register
        LDR                 R1,=0x10000000      ;Trigger the PendSV
        STR                 R1,[R0]
        DSB
        BX                  LR
;/* End Function:_RMP_A8M_Yield **********************************************/

;/* Function:PendSV_Handler ***************************************************
;Description : The PendSV interrupt handler.
;              EXC_RETURN bit 4 (FType) tells whether the thread owns FP/MVE
;              state. If it does, the hardware has already stacked the extended
;              frame (S0-S15, FPSCR and the MVE VPR on Helium cores) below the
;              PSP, so we only add S16-S31 with the conditionally executed
;              VSTMDB. The DC16 words encode "IT EQ" plus the conditional
;              VSTMDBEQ/VLDMIAEQ by hand, so that this same source assembles
;              for cores without a FPU as well; those instructions are never
;              executed there, because FType is always one when no FP
;              extension exists.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
PendSV_Handler
        MRS                 R0,PSP
        TST                 LR,#0x10            ;Save FPU/MVE registers
        DC16                0xBF08              ;IT EQ ;If FP/MVE used,
        DC16                0xED20              ;VSTMDBEQ R0!,{S16-S31}
        DC16                0x8A10              ;Save FPU/MVE registers not saved by lazy stacking
        STMDB               R0!,{R4-R11,LR}     ;Save the general purpose registers

        LDR                 R1,=RMP_SP_Cur      ;Store The SP to control block
        STR                 R0,[R1]
        BL                  _RMP_Run_High       ;Get the highest ready task
        LDR                 R1,=RMP_SP_Cur      ;Load the SP from control block
        LDR                 R0,[R1]

        LDMIA               R0!,{R4-R11,LR}     ;Restore the general purpose registers
        TST                 LR,#0x10            ;Restore FPU/MVE registers
        DC16                0xBF08              ;IT EQ ;If FP/MVE used,
        DC16                0xECB0              ;VLDMIAEQ R0!,{S16-S31}
        DC16                0x8A10              ;Restore FPU/MVE registers not loaded by lazy stacking
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
