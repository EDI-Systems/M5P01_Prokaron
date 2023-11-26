/******************************************************************************
Filename    : rmp_platform_a6m.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for ARMv6-M.
******************************************************************************/

/* Include *******************************************************************/
#define __HDR_DEF__
#include "Platform/A6M/rmp_platform_a6m.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "Platform/A6M/rmp_platform_a6m.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Platform/A6M/rmp_platform_a6m.h"

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
    rmp_ptr_t End;
    struct RMP_A6M_Stack* Ptr;
    
    /* Compute & align stack */
    End=RMP_ROUND_DOWN(Stack+Size, 3U);
    Ptr=(struct RMP_A6M_Stack*)(End-sizeof(struct RMP_A6M_Stack));
    
    /* Set LR_EXC and xPSR accordingly to avoid INVSTATE */
    Ptr->LR_EXC=0xFFFFFFFDU;
    Ptr->XPSR=0x01000000U;
    
    /* Pass entry and parameter */
    Ptr->PC=Entry;
    Ptr->R0=Param;
    
    /* Fill the rest for ease of identification */
    Ptr->R1=0x01010101U;
    Ptr->R2=0x02020202U;
    Ptr->R3=0x03030303U;
    Ptr->R4=0x04040404U;
    Ptr->R5=0x05050505U;
    Ptr->R6=0x06060606U;
    Ptr->R7=0x07070707U;
    Ptr->R8=0x08080808U;
    Ptr->R9=0x09090909U;
    Ptr->R10=0x10101010U;
    Ptr->R11=0x11111111U;
    Ptr->R12=0x12121212U;
    Ptr->LR=0x13131313U;
    
    return (rmp_ptr_t)Ptr;
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
    RMP_A6M_LOWLVL_INIT();
  
    RMP_Int_Disable();
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
    RMP_A6M_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
