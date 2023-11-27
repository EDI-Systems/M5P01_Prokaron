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
    rmp_ptr_t End;
    struct RMP_RV32P_Stack* Ptr;

    /* Compute & align stack */
    End=RMP_ROUND_DOWN(Stack+Size, 4U);
    Ptr=(struct RMP_RV32P_Stack*)(End-sizeof(struct RMP_RV32P_Stack));

    /* This is where PC is saved */
    Ptr->PC=Entry;
    /* We always initialize the mstatus register to initialize
     * the FPU, but whether it is present depends on the processor */
    Ptr->MSTATUS=0x1880U|0x2000U;
    /* We always initialize GP to a known value.
     * If a thread modifies this later (by itself), it is fine */
    Ptr->X3_GP=(rmp_ptr_t)(&_RMP_Global);
    /* x10 for arguments */
    Ptr->X10_A0=Param;

    /* Fill the rest for ease of identification */
    Ptr->X1_RA=0x01010101U;
    Ptr->X4_TP=0x04040404U;
    Ptr->X5_T0=0x05050505U;
    Ptr->X6_T1=0x06060606U;
    Ptr->X7_T2=0x07070707U;
    Ptr->X8_S0_FP=0x08080808U;
    Ptr->X9_S1=0x09090909U;
    Ptr->X11_A1=0x11111111U;
    Ptr->X12_A2=0x12121212U;
    Ptr->X13_A3=0x13131313U;
    Ptr->X14_A4=0x14141414U;
    Ptr->X15_A5=0x15151515U;
    Ptr->X16_A6=0x16161616U;
    Ptr->X17_A7=0x17171717U;
    Ptr->X18_S2=0x18181818U;
    Ptr->X19_S3=0x19191919U;
    Ptr->X20_S4=0x20202020U;
    Ptr->X21_S5=0x21212121U;
    Ptr->X22_S6=0x22222222U;
    Ptr->X23_S7=0x23232323U;
    Ptr->X24_S8=0x24242424U;
    Ptr->X25_S9=0x25252525U;
    Ptr->X26_S10=0x26262626U;
    Ptr->X27_S11=0x27272727U;
    Ptr->X28_T3=0x28282828U;
    Ptr->X29_T4=0x29292929U;
    Ptr->X30_T5=0x30303030U;
    Ptr->X31_T6=0x31313131U;

    return (rmp_ptr_t)Ptr;
}
/* End Function:_RMP_Stack_Init **********************************************/

/* Function:_RMP_Lowlvl_Init **************************************************
Description : Initialize the low level hardware of the system. This is adapted from
              Arduino and FE310 examples.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Lowlvl_Init(void)
{
    RMP_RV32P_LOWLVL_INIT();

    /* Clear flags */
    RMP_RV32P_Int_Act=0U;
    _RMP_RV32P_Yield_Pend=0U;
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
    RMP_RV32P_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* Function:_RMP_Yield ********************************************************
Description : Trigger a yield to another thread. This will trigger the software
              interrupt in RISC-V.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Yield(void)
{
    if(RMP_RV32P_Int_Act!=0U)
        _RMP_RV32P_Yield_Pend=1U;
    else
        /* Selecting RVD implies RVF */
#if(RMP_RV32P_COP_RVD!=0U)
        _RMP_RV32P_Yield_RVD();
#elif(RMP_RV32P_COP_RVF!=0U)
        _RMP_RV32P_Yield_RVF();
#else
        _RMP_RV32P_Yield_NONE();
#endif
}
/* End Function:_RMP_Yield ***************************************************/

/* Function:_RMP_RV32P_Tim_Handler *******************************************
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
/* End Function:_RMP_RV32P_Tim_Handler **************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
