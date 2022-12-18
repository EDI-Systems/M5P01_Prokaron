/******************************************************************************
Filename    : rmp_platform_crx.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for Cortex-R.
******************************************************************************/

/* Includes ******************************************************************/
#define __HDR_DEFS__
#include "Platform/CortexR/rmp_platform_crx.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEFS__

#define __HDR_STRUCTS__
#include "Platform/CortexR/rmp_platform_crx.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCTS__

/* Private include */
#include "Platform/CortexR/rmp_platform_crx.h"

#define __HDR_PUBLIC_MEMBERS__
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC_MEMBERS__
/* End Includes **************************************************************/

/* Begin Function:_RMP_Stack_Init *********************************************
Description : Initiate the process stack when trying to start a process. Never
              call this function in user application.
Input       : rmp_ptr_t Entry - The entry of the thread.
              rmp_ptr_t Stack - The stack address of the thread.
              rmp_ptr_t Arg - The argument to pass to the thread.
Output      : None.
Return      : None.
Other       : When the system stack safe redundancy is set to zero, the stack 
              looks like this when we try to step into the next process by 
              context switch:
              HIGH-->  CPSR PC LR R12-R0 -->LOW
              We need to set the stack correctly pretending that we are 
              returning from an systick timer interrupt. Thus, we set the XPSR
              to avoid INVSTATE; set PC to the pseudo-process entrance; set LR
              (1) to 0 because the process does not return to anything; set the 
              R12,R3-R0 to 0; set R11-R4 to 0.
******************************************************************************/
void _RMP_Stack_Init(rmp_ptr_t Entry, rmp_ptr_t Stack, rmp_ptr_t Arg)
{
    rmp_ptr_t* Stack_Ptr;

    Stack_Ptr=(rmp_ptr_t*)Stack;
    Stack_Ptr[0]=Arg;
    Stack_Ptr[1]=0x01010101;
    Stack_Ptr[2]=0x02020202;
    Stack_Ptr[3]=0x03030303;
    Stack_Ptr[4]=0x04040404;
    Stack_Ptr[5]=0x05050505;
    Stack_Ptr[6]=0x06060606;
    Stack_Ptr[7]=0x07070707;
    Stack_Ptr[8]=0x08080808;
    Stack_Ptr[9]=0x09090909;
    Stack_Ptr[10]=0x10101010;
    Stack_Ptr[11]=0x11111111;
    Stack_Ptr[12]=0x12121212;
    Stack_Ptr[13]=0x14141414;
    Stack_Ptr[14]=Entry;

    /* See if the user code is thumb or ARM */
    if((Entry&0x01)!=0)
        Stack_Ptr[15]=RMP_CRX_CPSR_E|RMP_CRX_CPSR_A|RMP_CRX_CPSR_F|RMP_CRX_CPSR_T|RMP_CRX_CPSR_M(RMP_CRX_SYS);
    else
        Stack_Ptr[15]=RMP_CRX_CPSR_E|RMP_CRX_CPSR_A|RMP_CRX_CPSR_F|RMP_CRX_CPSR_M(RMP_CRX_SYS);
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
    RMP_CRX_LOW_LEVEL_INIT();
    
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
    RMP_CRX_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
