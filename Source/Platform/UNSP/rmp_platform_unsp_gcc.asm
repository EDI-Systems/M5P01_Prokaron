//*****************************************************************************
//Filename    : rmp_platform_unsp_gcc.s
//Author      : pry
//Date        : 10/04/2012
//Description : The assembly part of the RMP RTOS. This is for unSP.
//              The unSP assembly is unusually strong, which allows manipulation
//              of variables and sensitive registers directly. This strongly
//              suggests that it is more of a CISC than a RISC.
//*****************************************************************************

// The Sunplus (Now Generalplus) unSP Architecture ****************************
//R1-R3       : General purpose registers.
//R5          : Base pointer.
//R8-R15      : General purpose register (only present in unSP V2).
//SR1-SR4     : Secondary register bank (only present in unSP V2).
//SR          : Status register (includes CS and DS).
//SP          : Stack pointer.
//PC          : Program counter.
//FR          : Function control register, where
//              [13]   - BNK - this port will NOT switch register banks
//              [12]   - FRA
//              [11]   - FIR_MOVE
//              [10:7] - SB
//              [6]    - ENA_FIQ
//              [5]    - ENQ_IRQ
//              [4]    - INE
//              [3:0]  - PRI
//*****************************************************************************

// Import *********************************************************************
    // The real task switch handling function
    .external           __RMP_Run_High
    // The real systick handler function
    .external         	__RMP_Tim_Handler
    // The PID of the current thread
    .external           _RMP_Thd_Cur
    // The stack address of current thread
    .external          	_RMP_SP_Cur
    // Kernel stack and table
    .external         	__RMP_UNSP_SP_Kern
// End Import *****************************************************************

// Export *********************************************************************
    // Disable all interrupts
    .public             _RMP_Int_Disable
    // Enable all interrupts
    .public             _RMP_Int_Enable
    // Mask/unmask interrupt
    .public             _RMP_Int_Mask
    // Start the first thread
    .public             __RMP_Start
    // Trigger a context switch
    .public             __RMP_UNSP_Yield_SPV1
    .public             __RMP_UNSP_Yield_SPV2
// End Export *****************************************************************

// Header *********************************************************************
    .text
// End Header *****************************************************************

// Function:RMP_Int_Disable ***************************************************
//Description : The function for disabling all IRQ interrupts, but not FIQ 
//              (interrupt masking). Does not allow nesting.
//Input       : None.
//Output      : None.
//Return      : None.
//*****************************************************************************
_RMP_Int_Disable:
    INT                 FIQ,IRQ
    RETF
// End Function:RMP_Int_Disable ***********************************************

// Function:RMP_Int_Enable ****************************************************
//Description : The function for enabling all interrupts. Does not allow nesting.
//Input       : None.
//Output      : None.
//Return      : None.
//*****************************************************************************
_RMP_Int_Enable:
    INT                 FIQ,IRQ
    RETF
// End Function:RMP_Int_Enable ************************************************

// Function:RMP_Int_Mask ******************************************************
//Description : Disable/enable IRQ but leave FIQ alone.
//Input       : [BP] - Whether to mask IRQ.
//Output      : None.
//Return      : None.
//*****************************************************************************
_RMP_Int_Mask:
    BP                  =SP+3
    R1                  =[BP]
    CMP                 R1,0
    JZ                  _RMP_Int_Mask_Enable
    INT                 FIQ
    RETF
_RMP_Int_Mask_Enable:
    INT                 FIQ,IRQ
    RETF
// End Function:RMP_Int_Mask **************************************************

// Function:_RMP_Start ********************************************************
//Description : Jump to the user function and will never return from it.
//Input       : [BP+0] - The entry of the first task.
//              [BP+1] - The stack of the first task.
//Output      : None.
//Return      : None.
//*****************************************************************************
__RMP_Start:
    BP                  =SP+3
    // Save the current kernel SP address - use BP to gain more space
    R1                  =BP+2
    [__RMP_UNSP_SP_Kern]=R1
    // Load SP for the first task
    SP                  =[BP+1]
    // Jump to the entry - note that the function pointer actually points to the
    // location of the true double word entry address in flash (do note that this
    // is very different from other architectures which generate stubs at that
    // address which does a long jump), so we have to fiddle with the address a
    // little bit. SR value is unimportant here (we're leaving main() anyway), so
    // lets destroy DS and flags here. In other words, we assume DS=0 upon RMP_Init
    // entry. It can be observed that unSP is not function pointers friendly, as it
    // lacks true long jumps.
    BP                  =[BP+0]
    R1                  =[BP]
    R2                  =[BP+1]
    PUSH                R1,R2 TO [SP]
    POP                 SR,PC FROM [SP]
    // Should not reach here
// End Function:_RMP_Start ****************************************************

// Function:_RMP_Yield ********************************************************
//Description : Trigger a yield to another thread.
//Input       : None.
//Output      : None.
//Return      : None.
//*****************************************************************************
// Save all GP regs ***********************************************************
    // unSP call is special in the sense that the CALL and interrupt entry
    // perform exactly the same pushing behavior, hence we can omit the PC 
    // push/skip altogether. IRQ nesting is impossible, and FIQ is left to
    // cater kernel agnostic interrupts.
RMP_UNSP_SAVE:          .macro
    INT                 FIQ
    PUSH                R1,R5 TO [SP]
    .endm

// Actual context switch ******************************************************
RMP_UNSP_SWITCH:        .macro
    R1                  =0
    SR                  =R1
    [_RMP_SP_Cur]       =SP
    SP                  =[__RMP_UNSP_SP_Kern]
    CALL                __RMP_Run_High
    SP                  =[_RMP_SP_Cur]
    .endm

// Restore all GP regs ********************************************************
RMP_UNSP_LOAD:          .macro
    POP                 R1,R5 FROM [SP]
    INT                 FIQ,IRQ
    RETI
    .endm

// unSP V1.x ******************************************************************
__RMP_UNSP_Yield_SPV1:  .proc
    RMP_UNSP_SAVE
    RMP_UNSP_SWITCH
    RMP_UNSP_LOAD
    .endp
    
// unSP V2.x ******************************************************************
__RMP_UNSP_Yield_SPV2:  .proc
    RMP_UNSP_SAVE
    .dw                 0xFF80      // PUSH R8,R15 TO [SP]
    .dw                 0x8E20
    RMP_UNSP_SWITCH
    .dw                 0xFF80      // POP R8,R15 FROM [SP]
    .dw                 0x0E20
    RMP_UNSP_LOAD
    .endp
// End Function:_RMP_Yield ****************************************************
    .end
// End Of File ****************************************************************

// Copyright (C) Evo-Devo Instrum. All rights reserved ************************
