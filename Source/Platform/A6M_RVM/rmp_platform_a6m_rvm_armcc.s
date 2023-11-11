;/*****************************************************************************
;Filename    : rmp_platform_a6m_rvm_armcc.s
;Author      : pry
;Date        : 09/02/2018
;Description : The assembly part of the RMP RTOS for the RVM virtual machine.
;*****************************************************************************/

;/* The ARMv6-M Architecture **************************************************
;R0-R7:General purpose registers that are accessible. 
;R8-R12:General purpose registers that can only be reached by 32-bit instructions.
;R13:SP/SP_process/SP_main    Stack pointer
;R14:LR                       Link Register(used for returning from a subfunction)
;R15:PC                       Program counter.
;IPSR                         Interrupt Program Status Register.
;APSR                         Application Program Status Register.
;EPSR                         Execute Program Status Register.
;The above 3 registers are saved into the stack in combination(xPSR).
;The ARM Cortex-M4/7 also include a FPU.
;*****************************************************************************/
            
;/* Begin Header *************************************************************/
                        ; 2^3=8 byte alignment.        
                        AREA                INIT, CODE, READONLY, ALIGN=3
                                    
                        THUMB
                        REQUIRE8
                        PRESERVE8
;/* End Header ***************************************************************/

;/* Begin Exports ************************************************************/
                        ; Start the first thread
                        EXPORT              _RMP_Start      
                        ;Fast-path context switching without invoking the RVM
                        EXPORT              _RMP_A6M_RVM_Yield    
;/* End Exports **************************************************************/

;/* Begin Imports ************************************************************/
                        ; The real task switch handling function
                        IMPORT              _RMP_Run_High
                        ; The stack address of current thread
                        IMPORT              RMP_SP_Cur        
                        ; Save and load extra contexts, such as FPU, peripherals and MPU
                        IMPORT              RMP_Ctx_Save
                        IMPORT              RMP_Ctx_Load  
                        ; Mask/unmask interrupts
                        IMPORT              RMP_Int_Mask
                        IMPORT              RMP_Int_Unmask
                        ; Hypercall parameter space
                        IMPORT              RMP_A6M_RVM_Usr_Param
;/* End Imports **************************************************************/

;/* Begin Function:_RMP_Start *************************************************
;Description : Jump to the user function and will never return from it.
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
_RMP_Start              PROC
                        SUBS                R1, #16             ; This is how we push our registers so move forward
                        MOV                 SP, R1              ; Set the stack pointer
                        ISB
                        BX                  R0                  ; Branch to our target
                        ENDP
;/* End Function:_RMP_Start **************************************************/

;/* Begin Function:_RMP_A6M_RVM_Yield *****************************************
;Description : Switch from user code to another thread, rather than from the 
;              interrupt handler. Need to masquerade the context well so that
;              it may be recovered from the interrupt handler as well.
;              Caveats: 
;              1. User-level code cannot clear CONTROL.FPCA hence all threads
;                 in the system will be tainted with the FPU flag and include
;                 a full context save/restore. Yet this is still much faster
;                 than the traditional slow path through the PendSV.
;              2. After the user have stacked up everything on its stack but
;                 not disabled its interrupt yet, an interrupt may occur, and
;                 stack again on the user stack. This is allowed, but must be
;                 taken into account when calculating stack usage.
;
;              The exception stack layout is as follows:
;
;               Unaligned           Aligned
;               Reserved
;               XPSR                XPSR
;               PC                  PC
;               LR                  LR
;               R12                 R12
;               R3                  R3
;               R2                  R2
;               R1                  R1
;               R0                  R0
;
;Input       : None.
;Output      : None.
;Return      : None.
;*****************************************************************************/
;/* Exception Entry Stacking *************************************************/
                        ; Alignment detection
                        MACRO
                        ALIGN_PUSH          $LABEL              ; Cannot TST SP, unpredictable
                        MOV                 R0, SP              ; Is SP aligned to 8 bytes?
                        LDR                 R1, =0x00000007
                        TST                 R0, R1
                        BEQ                 $LABEL              ; We pushed three words before, thus EQ
                        MEND

                        ; Exception stacking
                        MACRO
                        EXC_PUSH            $SZ, $XPSR, $LR
                        SUB                 SP, #4*($SZ-7)      ; Adjust SP to before pushing R0-R3
                        LDR                 R0, [SP, #4*($SZ-5)]; Load real R0/R1 value pushed at start
                        LDR                 R1, [SP, #4*($SZ-7)]
                        PUSH                {R0-R3}             ; Push GP regs without overwrites
                        MOV                 R0, R12
                        MOV                 R1, LR
                        STR                 R0, [SP, #4*4]
                        STR                 R1, [SP, #4*5]
                        LDR                 R0, [SP, #4*($SZ-2)]; Load real XPSR value pushed at start
                        LDR                 R1, =$XPSR
                        ORRS                R0, R1
                        STR                 R0, [SP, #4*7]
                        LDR                 R0, =_RMP_A6M_Skip  ; Push PC with[0] cleared
                        LDR                 R1, =0xFFFFFFFE
                        ANDS                R0, R1
                        STR                 R0, [SP, #4*6]
                        LDR                 R1, =$LR            ; Make up the EXC_RETURN
                        MOV                 LR, R1
                        MEND
                        
;/* Exception Exit Unstacking ************************************************/
                        ; Alignment detection
                        MACRO
                        ALIGN_POP           $LABEL
                        LDR                 R0, [SP, #4*7]      ; Load up XPSR
                        LDR                 R1, =0x00000200
                        TST                 R0, R1
                        BNE                 $LABEL
                        MEND
                        
                        ; Exception unstacking for basic frame:
                        ; The original consists of [PAD] [FPU] XPSR PC LR R12 R3-R0. 
                        ; This is not very ideal for restoring the context without
                        ; touching the XPSR, which is susceptible to changes.
                        ; PC R0 XPSR Temp .... is far more ideal. This snippet does 
                        ; the manipulations accordingly. It
                        ; (1) transforms the stack into         PC XPSR LR ... R0
                        ; (2) pops off GP regs and result in    PC XPSR
                        ; (3) makes more space on stack         PC XPSR ---- Temp
                        ; (4) stores R0 to temp                 PC XPSR ---- R0
                        ; (5) moves XPSR to back                PC ---- XPSR R0
                        ; (6) moves R0 to front                 PC R0   XPSR ----
                        ; (7) restores xpsr through R0          PC R0
                        ; (8) restores R0 and PC                PC R0
                        ; Note that in the transformation we never place variables 
                        ; at lower addresses than the current SP, as this will run 
                        ; the risk of a racing interrupt erasing the variable.
                        MACRO
                        EXC_POP             $SZ
                        LDR                 R1, [SP, #4*6]      ; Move PC to the last
                        LDR                 R2, =0x00000001
                        ORRS                R1, R2
                        STR                 R1, [SP, #4*($SZ-1)]
                        LDR                 R2, =0xFFFFFDFF     ; Clear the XPSR[9]
                        ANDS                R0, R2
                        STR                 R0, [SP, #4*($SZ-2)]; Move XPSR to the second to the last
                        LDR                 R0, [SP, #4*4]      ; Pop GP regs without overwrites
                        LDR                 R1, [SP, #4*5]
                        MOV                 R12, R0
                        MOV                 LR, R1
                        POP                 {R0-R3}
                        ADD                 SP, #4*($SZ-8)      ; Make room for manipulations
                        STR                 R0, [SP]            ; Keep R0 original value
                        LDR                 R0, [SP, #4*2]      ; Move XPSR back
                        STR                 R0, [SP, #4*1]
                        LDR                 R0, [SP]            ; Move R0 to front
                        STR                 R0, [SP, #4*2]
                        ADD                 SP, #4
                        POP                 {R0}
                        MSR                 XPSR, R0
                        POP                 {R0}
                        POP                 {PC}
                        MEND
                        
;/* User-level Context Switch ************************************************/
                        AREA                FASTYIELD, CODE, READONLY, ALIGN=3
_RMP_A6M_RVM_Yield      PROC
                        PUSH                {R0}                ; Protect R0, XPSR and R1
                        MRS                 R0, XPSR
                        PUSH                {R0}
                        PUSH                {R1}
                        
                        ALIGN_PUSH          Stk_Unalign
Stk_Align               ; Aligned stack
                        EXC_PUSH            8, 0x01000000, 0xFFFFFFFD
                        B                   Stk_Done
Stk_Unalign             ; Unaligned stack
                        EXC_PUSH            8+1, 0x01000200, 0xFFFFFFFD

Stk_Done
                        MOV                 R3, R11             ; Push GP regs
                        MOV                 R2, R10
                        MOV                 R1, R9
                        MOV                 R0, R8
                        PUSH                {R0-R3, LR}
                        PUSH                {R4-R7}
                        
                        LDR                 R0, =RMP_A6M_RVM_Usr_Param
                        LDR                 R0, [R0]            ; Push hypercall parameters
                        LDMIA               R0!, {R1-R5}
                        PUSH                {R1-R5}

                        BL                  RMP_Int_Mask        ; Mask interrupts
                        BL                  RMP_Ctx_Save        ; Save extra context

                        LDR                 R1, =RMP_SP_Cur     ; Save the SP to control block.
                        MOV                 R0, SP
                        STR                 R0, [R1]

                        BL                  _RMP_Run_High       ; Get the highest ready task.

                        LDR                 R1, =RMP_SP_Cur     ; Load the SP.
                        LDR                 R0, [R1]
                        MOV                 SP, R0

                        BL                  RMP_Ctx_Load        ; Load extra context
                        BL                  RMP_Int_Unmask      ; Unmask interrupts

                        LDR                 R0, =RMP_A6M_RVM_Usr_Param
                        LDR                 R0, [R0]            ; Pop hypercall parameters
                        POP                 {R1-R5}
                        STMIA               R0!, {R1-R5}
                        
                        POP                 {R4-R7}             ; Pop GP regs
                        POP                 {R0-R3}
                        MOV                 R8, R0
                        MOV                 R9, R1
                        MOV                 R10, R2
                        MOV                 R11, R3
                        POP                 {R0}
                        MOV                 LR, R0

                        ALIGN_POP           Uns_Unalign
Uns_Align               ; Aligned stack
                        EXC_POP             8
Uns_Unalign             ; Unaligned stack
                        EXC_POP             8+1
                        ; Exit location
_RMP_A6M_Skip
                        BX                  LR
                        ENDP
;/* End Function:_RMP_A6M_RVM_Yield ******************************************/
                        ALIGN
                        END
;/* End Of File **************************************************************/

;/* Copyright (C) Evo-Devo Instrum. All rights reserved **********************/
