/******************************************************************************
Filename    : rmp_platform_a7r.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for ARMv7-R.
******************************************************************************/

/* Includes ******************************************************************/
#define __HDR_DEFS__
#include "Platform/A7R/rmp_platform_a7r.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEFS__

#define __HDR_STRUCTS__
#include "Platform/A7R/rmp_platform_a7r.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCTS__

/* Private include */
#include "Platform/A7R/rmp_platform_a7r.h"

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
******************************************************************************/
void _RMP_Stack_Init(rmp_ptr_t Entry, rmp_ptr_t Stack, rmp_ptr_t Arg)
{
    rmp_ptr_t* Stack_Ptr;

    Stack_Ptr=(rmp_ptr_t*)Stack;
    Stack_Ptr[0]=Arg;
    Stack_Ptr[1]=0x01010101U;
    Stack_Ptr[2]=0x02020202U;
    Stack_Ptr[3]=0x03030303U;
    Stack_Ptr[4]=0x04040404U;
    Stack_Ptr[5]=0x05050505U;
    Stack_Ptr[6]=0x06060606U;
    Stack_Ptr[7]=0x07070707U;
    Stack_Ptr[8]=0x08080808U;
    Stack_Ptr[9]=0x09090909U;
    Stack_Ptr[10]=0x10101010U;
    Stack_Ptr[11]=0x11111111U;
    Stack_Ptr[12]=0x12121212U;
    Stack_Ptr[13]=0x14141414U;
    Stack_Ptr[14]=Entry;

    /* See if the user code is thumb or ARM */
    if((Entry&0x01U)!=0U)
        Stack_Ptr[15]=RMP_A7R_CPSR_E|RMP_A7R_CPSR_A|RMP_A7R_CPSR_F|RMP_A7R_CPSR_T|RMP_A7R_CPSR_M(RMP_A7R_SYS);
    else
        Stack_Ptr[15]=RMP_A7R_CPSR_E|RMP_A7R_CPSR_A|RMP_A7R_CPSR_F|RMP_A7R_CPSR_M(RMP_A7R_SYS);
}
/* End Function:_RMP_Stack_Init **********************************************/

/* Begin Function:_RMP_Lowlvl_Init ********************************************
Description : Initialize the low level hardware of the system.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Lowlvl_Init(void)
{   
    RMP_A7R_LOWLVL_INIT();
    
    RMP_Int_Disable();
}
/* End Function:_RMP_Lowlvl_Init *********************************************/

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
    RMP_A7R_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
