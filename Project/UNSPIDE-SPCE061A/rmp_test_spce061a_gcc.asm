//*****************************************************************************
//Filename    : rmp_test_spce061a_gcc.s
//Author      : pry
//Date        : 10/04/2012
//Description : The extra testing file for this chip.
//*****************************************************************************
    
// Header *********************************************************************
    .text
    .include            Platform\UNSP\rmp_platform_unsp_gcc.inc
// End Header *****************************************************************
    
// Import *********************************************************************
    .external           _TIMB_Handler
// End Import *****************************************************************

// Export *********************************************************************
    .public             _BREAK
    .public             _FIQ
    .public             _IRQ0
    .public             _IRQ1
    .public             _IRQ2
    .public             _IRQ3
    .public             _IRQ4
    .public             _IRQ5
    .public             _IRQ6
    .public             _IRQ7
// End Export *****************************************************************

// Function:Loop_Forever ******************************************************
//Description : A dead loop that never exits when entered.
//Input       : None.
//Output      : None.                                      
//*****************************************************************************
Loop_Forever:           .proc
_BREAK:
    NOP
_FIQ:
    NOP
_IRQ0:
    NOP
_IRQ1:
    NOP
_IRQ3:
    NOP
_IRQ5:
    NOP
_IRQ6:
    NOP
_IRQ7:
    NOP
    GOTO                _IRQ7             
    .endp
// End Function:Loop_Forever **************************************************

// Function:_IRQ4 *************************************************************
//Description : 1024Hz signal interrupt routine for tick timer.
//Input       : None.
//Output      : None.                                      
//*****************************************************************************
_IRQ4:                  .proc
    RMP_UNSP_INT_SAVE_SPV1
    CALL                __RMP_UNSP_Tim_Handler
    RMP_UNSP_INT_LOAD_SPV1
    .endp
// End Function:_IRQ4 *********************************************************

// Function:_IRQ2 *************************************************************
//Description : Timer B interrupt routine for ISR latency testing.
//Input       : None.
//Output      : None.                                      
//*****************************************************************************
_IRQ2:                  .proc
    RMP_UNSP_INT_SAVE_SPV1
    CALL                _TIMB_Handler
    RMP_UNSP_INT_LOAD_SPV1
    .endp
// End Function:_IRQ2 *********************************************************

// End Of File ****************************************************************

// Copyright (C) Evo-Devo Instrum. All rights reserved ************************
