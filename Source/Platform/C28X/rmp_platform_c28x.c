/******************************************************************************
Filename    : rmp_platform_c28x.c
Author      : zr (ran zhang)
Date        : 27/04/2024
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for C28X. Note that this architecture
              is 16-bit word addressing, where rmp_u8_t is in fact 16 bits.
******************************************************************************/

/* Include *******************************************************************/
#define __HDR_DEF__
#include "Platform/C28X/rmp_platform_c28x.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "Platform/C28X/rmp_platform_c28x.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Platform/C28X/rmp_platform_c28x.h"

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
    rmp_ptr_t Ptr;
    struct RMP_C28X_Stack* Ctx;

    Ptr=RMP_STACK_PTR(Stack,Size);
    Ctx=RMP_STACK_CTX(Ptr);

    /* Pass entry and parameter */
    Ctx->PC=Entry;
    Ctx->ACC=Param;

    /* No product shift, sign extend, aux ptr 0, interrupt enabled upon entry */
    Ctx->T_ST0=0x00000081U;
    Ctx->DP_ST1=0x00000A08U;

    /* Enable interrupt for all sources, by default, for each thread */
    Ctx->DBGSTAT_IER=0x0000FFFFU;

    /* Initialize the rest of the registers */
    Ctx->AR1_AR0=0x01010000U;
    Ctx->AR1H_AR0H=0x01010000U;
    Ctx->XAR2=0x02020202U;
    Ctx->XAR3=0x03030303U;
    Ctx->XAR4=0x04040404U;
    Ctx->XAR5=0x05050505U;
    Ctx->XAR6=0x06060606U;
    Ctx->XAR7=0x07070707U;

    /* Selecting FPU64 implies FPU32 */
#if(RMP_C28X_COP_FPU64!=0U)
    /* Rounding mode - RND32 */
    Ctx->STF=0x00000200U;
    Ctx->RB=0x00000000U;
    Ctx->R0L=0x00000000U;
    Ctx->R0H=0x00000000U;
    Ctx->R1L=0x01010101U;
    Ctx->R1H=0x01010101U;
    Ctx->R2L=0x02020202U;
    Ctx->R2H=0x02020202U;
    Ctx->R3L=0x03030303U;
    Ctx->R3H=0x03030303U;
    Ctx->R4L=0x04040404U;
    Ctx->R4H=0x04040404U;
    Ctx->R5L=0x05050505U;
    Ctx->R5H=0x05050505U;
    Ctx->R6L=0x06060606U;
    Ctx->R6H=0x06060606U;
    Ctx->R7L=0x07070707U;
    Ctx->R7H=0x07070707U;
#elif(RMP_C28X_COP_FPU32!=0U)
    /* Rounding mode - RND32 */
    Ctx->STF=0x00000200U;
    Ctx->RB=0x00000000U;
    Ctx->R0H=0x00000000U;
    Ctx->R1H=0x01010101U;
    Ctx->R2H=0x02020202U;
    Ctx->R3H=0x03030303U;
    Ctx->R4H=0x04040404U;
    Ctx->R5H=0x05050505U;
    Ctx->R6H=0x06060606U;
    Ctx->R7H=0x07070707U;
#endif

    /* Compensate for the "-1" when "IRET"ing */
    return Ptr+1U;
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
    
    RMP_C28X_LOWLVL_INIT();
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
    RMP_C28X_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* Function:_RMP_C28X_Tim_Handler *********************************************
Description : Timer interrupt routine for C28X; this is for normal kernel.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_C28X_Tim_Handler(void)
{
    RMP_C28X_TIM_CLR();

    _RMP_Tim_Handler(1U);
}
/* End Function:_RMP_C28X_Tim_Handler ****************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
