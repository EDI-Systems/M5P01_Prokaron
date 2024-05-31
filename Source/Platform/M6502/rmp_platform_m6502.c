/******************************************************************************
Filename    : rmp_platform_m6502.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for M6502.
******************************************************************************/

/* Include *******************************************************************/
#define __HDR_DEF__
#include "Platform/M6502/rmp_platform_m6502.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "Platform/M6502/rmp_platform_m6502.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Platform/M6502/rmp_platform_m6502.h"

#define __HDR_PUBLIC__
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC__
/* End Include ***************************************************************/

/* Function:_RMP_Stack_Init ***************************************************
Description : Initiate the process stack when trying to start a process. Never
              call this function in user application.
Input       : rmp_ptr_t Stack - The stack address of the thread.
              rmp_ptr_t Size - The stack size of the thread.
              rmp_ptr_t Entry - The entry address of the thread.
              rmp_ptr_t Param - The argument to pass to the thread.
Output      : None.
Return      : rmp_ptr_t - The adjusted stack location.
******************************************************************************/
rmp_ptr_t _RMP_Stack_Init(rmp_ptr_t Stack,
                          rmp_ptr_t Size,
                          rmp_ptr_t Entry,
                          rmp_ptr_t Param)
{
    struct RMP_M6502_Stack* Ptr;
    
    /* Compute stack - empty descending, no alignment requirement */
    Ptr=(struct RMP_M6502_Stack*)(Stack+Size-sizeof(struct RMP_M6502_Stack)-1U);
    
    /* Pass entry and parameter - program space is in words instead of bytes */

    
    /* Fill the rest for ease of identification - R1 is implicitly zero as required 
     * by GCC, but we still save/restore it in case the program includes assembly */
 
    
    /* Empty descending */
    return ((rmp_ptr_t)Ptr)-1U;
}
/* End Function:_RMP_Stack_Init **********************************************/

/* Function:_RMP_Lowlvl_Init **************************************************
Description : Initialize the low level hardware of the system.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Lowlvl_Init(void)
{
    RMP_Int_Disable();
    
    RMP_M6502_LOWLVL_INIT();

    /* Clear flags */
    RMP_M6502_Int_Act=0U;
    _RMP_M6502_Yield_Pend=0U;
}
/* End Function:_RMP_Lowlvl_Init *********************************************/

/* Function:_RMP_Plat_Hook ****************************************************
Description : Platform-specific hook for system initialization.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Plat_Hook(void)
{
    /* Scheduler lock implemented with interrupt disabling */
}
/* End Function:_RMP_Plat_Hook ***********************************************/

/* Function:RMP_Putchar *******************************************************
Description : Print a character to the debug console.
Input       : char Char - The character to print.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Putchar(char Char)
{
    RMP_M6502_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* Function:_RMP_Yield ********************************************************
Description : Trigger a yield to another thread.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Yield(void)
{
    if(RMP_M6502_Int_Act!=0U)
        _RMP_M6502_Yield_Pend=1U;
    else
        _RMP_M6502_Yield();
}
/* End Function:_RMP_Yield ***************************************************/

/* Function:_RMP_M6502_Tim_Handler **********************************************
Description : Timer interrupt routine for DSPIC.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_M6502_Tim_Handler(void)
{
    RMP_M6502_TIM_CLR();

    _RMP_Tim_Handler(1U);
}
/* End Function:_RMP_M6502_Tim_Handler *****************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
