/******************************************************************************
Filename    : rmp_platform_rv32p.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for RV32P with physical address space.
              Any subsets that include RV32I are supported by this port, and this
              is also compatible with processors that support RV32C in addition
              to RV32P.
              Note that processors only supporting RV32C are not covered by
              this port and should use the RV32CP port instead.
******************************************************************************/

/* Include *******************************************************************/
#define __HDR_DEF__
#include "Platform/RV32P/rmp_platform_rv32p.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "Platform/RV32P/rmp_platform_rv32p.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Platform/RV32P/rmp_platform_rv32p.h"

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
    struct RMP_RV32P_Stack* Ctx;
    
    RMP_STACK_CALC(Ptr,Ctx,Stack,Size);

    /* This is where PC is saved */
    Ctx->PC=Entry;
    /* We always initialize the mstatus register for FPU when it has one */
#if((RMP_RV32P_COP_RVF!=0U)||(RMP_RV32P_COP_RVD!=0U))
    Ctx->MSTATUS=0x1880U|0x2000U;
#else
    Ctx->MSTATUS=0x1880U;
#endif

    /* We always initialize GP to a known value.
     * If a thread modifies this later (by itself), it is fine */
    Ctx->X3_GP=(rmp_ptr_t)(&_RMP_Global);
    /* x10 for arguments */
    Ctx->X10_A0=Param;

    /* Fill the rest for ease of identification */
    Ctx->X1_RA=0x01010101U;
    Ctx->X4_TP=0x04040404U;
    Ctx->X5_T0=0x05050505U;
    Ctx->X6_T1=0x06060606U;
    Ctx->X7_T2=0x07070707U;
    Ctx->X8_S0_FP=0x08080808U;
    Ctx->X9_S1=0x09090909U;
    Ctx->X11_A1=0x11111111U;
    Ctx->X12_A2=0x12121212U;
    Ctx->X13_A3=0x13131313U;
    Ctx->X14_A4=0x14141414U;
    Ctx->X15_A5=0x15151515U;
    Ctx->X16_A6=0x16161616U;
    Ctx->X17_A7=0x17171717U;
    Ctx->X18_S2=0x18181818U;
    Ctx->X19_S3=0x19191919U;
    Ctx->X20_S4=0x20202020U;
    Ctx->X21_S5=0x21212121U;
    Ctx->X22_S6=0x22222222U;
    Ctx->X23_S7=0x23232323U;
    Ctx->X24_S8=0x24242424U;
    Ctx->X25_S9=0x25252525U;
    Ctx->X26_S10=0x26262626U;
    Ctx->X27_S11=0x27272727U;
    Ctx->X28_T3=0x28282828U;
    Ctx->X29_T4=0x29292929U;
    Ctx->X30_T5=0x30303030U;
    Ctx->X31_T6=0x31313131U;

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
    
    RMP_RV32P_LOWLVL_INIT();
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
    RMP_RV32P_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* Function:_RMP_RV32P_Tim_Handler ********************************************
Description : The Tick interrupt routine.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_RV32P_Tim_Handler(void)
{
    /* Update or reset the next tick */
    RMP_RV32P_TIM_CLR();

    _RMP_Tim_Handler(1U);
}
/* End Function:_RMP_RV32P_Tim_Handler ***************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
