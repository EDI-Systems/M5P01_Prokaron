/******************************************************************************
Filename    : rmp_platform_lint.c
Author      : pry
Date        : 25/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for static analysis.
              This is never intended to run; just used for static analysis.
              May also be used as a template to create a new port.
******************************************************************************/

/* Include *******************************************************************/
#define __HDR_DEF__
#include "Platform/LINT/rmp_platform_lint.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "Platform/LINT/rmp_platform_lint.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Platform/LINT/rmp_platform_lint.h"

#define __HDR_PUBLIC__
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC__
/* End Include ***************************************************************/

/* Function:RMP_Int_Disable ***************************************************
Description : Disable interrupts.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Int_Disable(void)
{

}
/* End Function:RMP_Int_Disable **********************************************/

/* Function:RMP_Int_Enable ****************************************************
Description : Enable interrupts.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Int_Enable(void)
{

}
/* End Function:RMP_Int_Enable ***********************************************/

/* Function:_RMP_Start ********************************************************
Description : Start first thread.
Input       : rmp_ptr_t Entry - The entry address.
              rmp_ptr_t Stack - The stack address.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Start(rmp_ptr_t Entry, rmp_ptr_t Stack)
{

}
/* End Function:_RMP_Start ***************************************************/

/* Function:_RMP_Yield ********************************************************
Description : Trigger a yield to another thread.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Yield(void)
{

}
/* End Function:_RMP_Yield ***************************************************/

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
    rmp_ptr_t Ptr;
    struct RMP_LINT_Stack* Ctx;

    Ptr=RMP_STACK_PTR(Stack,Size);
    Ctx=RMP_STACK_CTX(Ptr);
    
    Ctx->Dummy=Entry+Param;
    
    return Ptr;
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

}
/* End Function:RMP_Putchar **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
