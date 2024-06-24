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
    rmp_u8_t Count;
    rmp_ptr_t Ptr;
    struct RMP_M6502_Stack* Ctx;
    
    Ptr=RMP_STACK_PTR(Stack,Size);
    Ctx=RMP_STACK_CTX(Ptr);
    
    /* Pass entry and parameter - program space is in words instead of bytes */
    Ctx->PCH=Entry>>8;
    Ctx->PCL=Entry&0xFFU;
    Ctx->X=Param>>8;
    Ctx->A=Param&0xFFU;

    /* Enable interrupts */
    Ctx->PF=0x04U;

    /* RSP always at 0xF9: 0xFF-PCH, 0xFE-PCL, 0xFD-PF, 0xFC-A. 0xFB-X, 0xFA-Y */
    Ctx->RSP=0xF9U;

    /* Fill register for ease of identification */
    Ctx->Y=0xAAU;

    /* Fill zeropage with sequence numbers for ease of identification */
    for(Count=0U;Count<RMP_M6502_ZP_SIZE;Count++)
        Ctx->ZP[Count]=Count;

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
    
    RMP_M6502_LOWLVL_INIT();
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

/* Function:_RMP_M6502_Tim_Handler ********************************************
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
/* End Function:_RMP_M6502_Tim_Handler ***************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
