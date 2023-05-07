/******************************************************************************
Filename    : rmp_platform_a6m.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for ARMv6-M.
******************************************************************************/

/* Includes ******************************************************************/
#define __HDR_DEFS__
#include "Platform/A6M/rmp_platform_a6m.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEFS__

#define __HDR_STRUCTS__
#include "Platform/A6M/rmp_platform_a6m.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCTS__

/* Private include */
#include "Platform/A6M/rmp_platform_a6m.h"

#define __HDR_PUBLIC_MEMBERS__
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC_MEMBERS__
/* End Includes **************************************************************/

/* Begin Function:_RMP_Stack_Init *********************************************
Description : Initiate the process stack when trying to start a process. Never
              call this function in user application.
              Special note must be taken if you are using va_list, va_arg, va_start
              and va_end (variable argument related functions), the stack passed
              in must be 8n+4 byte aligned instead of 4n byte aligned. If this is not
              adhered to, the va_arg can return wrong results, especially for 64-bit
              arguments (long long, int64, double). 
              The reason why we require 8n+4 is that the context size is 17 words,
              and after popping the context will be aligned to 8n bytes.
Input       : rmp_ptr_t Entry - The entry address of the thread.
              rmp_ptr_t Stack - The stack address of the thread.
              rmp_ptr_t Arg - The argument to pass to the thread.
Output      : None.
Return      : None.
Other       : When the system stack safe redundancy is set to zero, the stack 
              looks like this when we try to step into the next process by 
              context switch: (context size is 17 words)
              HIGH-->  XPSR PC LR(1) R12 R3-R0 LR R11-R4 -->LOW 
              We need to set the stack correctly pretending that we are 
              returning from an systick timer interrupt. Thus, we set the XPSR
              to avoid INVSTATE; set PC to the pseudo-process entrance; set LR
              (1) to 0 because the process does not return to anything; set the 
              R12,R3-R0 to 0; set R11-R4 to 0.
******************************************************************************/
void _RMP_Stack_Init(rmp_ptr_t Entry, rmp_ptr_t Stack, rmp_ptr_t Arg)
{
    /* This is the LR value indicating that we never used the FPU */
    ((rmp_ptr_t*)Stack)[0+8]=0xFFFFFFFD;    
    /* Cortex-M:Pass the parameter */                            
    ((rmp_ptr_t*)Stack)[0+9]=Arg;       
    /* Cortex-M:Set the process entry */
    ((rmp_ptr_t*)Stack)[6+9]=Entry;
    /* Cortex-M:Set the T bit or an INVSTATE will happen; don't set STKALIGN, 
     * and there is no stack padding. The effect of STKALIGN is, if it is set
     * and the (hardware-pushed) interrupt stack is 8n byte aligned, it will
     * pop an extra word (to restore the stack to an previous unaligned state);
     * if the interrupt stack is 8n+4 byte aligned, even if STKALIGN is set, it
     * will not pop this extra word. The result is, if STKALIGN is set, we will
     * never be able to get a 8n byte aligned stack, so we do not set it. */                            
    ((rmp_ptr_t*)Stack)[7+9]=0x01000000;
}
/* End Function:_RMP_Stack_Init **********************************************/

/* Begin Function:_RMP_Low_Level_Init *****************************************
Description : Initialize the low level hardware of the system.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Low_Level_Init(void)
{
    RMP_A6M_LOW_LEVEL_INIT();
  
    RMP_Int_Disable();
}
/* End Function:_RMP_Low_Level_Init ******************************************/

/* Begin Function:_RMP_Plat_Hook **********************************************
Description : Platform-specific hook for system initialization.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Plat_Hook(void)
{
    RMP_Int_Enable();
}
/* End Function:_RMP_Plat_Hook ***********************************************/

/* Begin Function:RMP_Putchar *************************************************
Description : Print a character to the debug console.
Input       : char Char - The character to print.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Putchar(char Char)
{
    RMP_A6M_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
