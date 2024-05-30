/******************************************************************************
Filename    : rmp_platform_a7m.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for ARMv7-M.
******************************************************************************/

/* Include *******************************************************************/
#define __HDR_DEF__
#include "Platform/A7M/rmp_platform_a7m.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "Platform/A7M/rmp_platform_a7m.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Platform/A7M/rmp_platform_a7m.h"

#define __HDR_PUBLIC__
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC__
/* End Include ***************************************************************/

/* Function:_RMP_Stack_Init ***************************************************
Description : Initiate the process stack when trying to start a process. Never
              call this function in user application.
              Need to pretend that we're returning from a context switch:
                  16  15  14    13  12-9  8  7-0 
              H> XPSR PC LR(1) R12 R3-R0 LR R11-R4 >L 
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
    struct RMP_A7M_Stack* Ctx;
    
    Ptr=RMP_STACK_PTR(Stack,Size);
    Ctx=RMP_STACK_CTX(Ptr);
    
    /* Set LR_EXC and xPSR accordingly to avoid INVSTATE */
    Ctx->LR_EXC=0xFFFFFFFDU;
    Ctx->XPSR=0x01000000U;
    
    /* Pass entry and parameter */
    Ctx->PC=Entry;
    Ctx->R0=Param;
    
    /* Fill the rest for ease of identification */
    Ctx->R1=0x01010101U;
    Ctx->R2=0x02020202U;
    Ctx->R3=0x03030303U;
    Ctx->R4=0x04040404U;
    Ctx->R5=0x05050505U;
    Ctx->R6=0x06060606U;
    Ctx->R7=0x07070707U;
    Ctx->R8=0x08080808U;
    Ctx->R9=0x09090909U;
    Ctx->R10=0x10101010U;
    Ctx->R11=0x11111111U;
    Ctx->R12=0x12121212U;
    Ctx->LR=0x14141414U;
    
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
    RMP_Int_Disable();
    
    RMP_A7M_LOWLVL_INIT();
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
    /* Scheduler lock implemented with interrupt masking */
    RMP_Int_Enable();
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
    RMP_A7M_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
