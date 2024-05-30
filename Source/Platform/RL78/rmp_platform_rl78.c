/******************************************************************************
Filename    : rmp_platform_rl78.c
Author      : lht
Date        : 04/24/2024
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for RL78.
******************************************************************************/

/* Include *******************************************************************/
#define __HDR_DEF__
#include "Platform/RL78/rmp_platform_rl78.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "Platform/RL78/rmp_platform_rl78.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Platform/RL78/rmp_platform_rl78.h"

#define __HDR_PUBLIC__
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC__
/* End Include ***************************************************************/

/* Function:_RMP_Stack_Init ***************************************************
Description : Initiate the process stack when trying to start a process. Never
              call this function in user application.
              Need to pretend that we're returning from a context switch:
              HL,DE,BC,AX,
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
    struct RMP_RL78_Stack* Ptr;
    
    /* Compute & align stack */
    End=RMP_ROUND_DOWN(Stack+Size, 1U);
    Ptr = (struct RMP_RL78_Stack*)(End-sizeof(struct RMP_RL78_Stack));
    
    /* Pass entry */
    Ptr->PC=Entry&0xFFFFU;
    /* High 8bits-PSW; Low 8bits-PCH, all function stubs within first 64k */
    Ptr->PSWPCH=0x0100U;

    /* Pass parameter */
    Ptr->AX=Param&0xFFFFU;
    Ptr->BC=0x0101U;
    /* Initialize CS/ES with default parameters */
    Ptr->CSES=0x000FU;

    /* Fill the rest for ease of identification */
    Ptr->DE=0x0202U;
    Ptr->HL=0x0303U;

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
    RMP_RL78_LOWLVL_INIT();
  
    RMP_Int_Disable();
    RMP_RL78_Int_Act=0U;
    _RMP_RL78_Yield_Pend=0U;
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
	RMP_RL78_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* Function:_RMP_Yield ********************************************************
Description : Trigger a yield to another thread.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Yield(void)
{	if(RMP_RL78_Int_Act!=0U)
        _RMP_RL78_Yield_Pend=1U;
    else
        _RMP_RL78_Yield();
}
/* End Function:_RMP_Yield ***************************************************/

/* Function:_RMP_RL78_Tim_Handler *******************************************
Description : Timer interrupt routine for RL78;
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_RL78_Tim_Handler(void)
{
    RMP_RL78_TIM_CLR();

    _RMP_Tim_Handler(1U);
}
/* End Function:_RMP_RL78_Tim_Handler **************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
