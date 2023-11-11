/******************************************************************************
Filename    : rmp_platform_rv32gp.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for RV32GP with physical address space.
              Any subsets that include RV32I are supported by this port, and this
              is also compatible with processors that support RV32C in addition
              to RV32GP.
              Note that processors only supporting RV32C are not covered by
              this port and should use the RV32CP port instead.
******************************************************************************/

/* Includes ******************************************************************/
#define __HDR_DEFS__
#include "Platform/RV32GP/rmp_platform_rv32gp.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEFS__

#define __HDR_STRUCTS__
#include "Platform/RV32GP/rmp_platform_rv32gp.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCTS__

/* Private include */
#include "Platform/RV32GP/rmp_platform_rv32gp.h"

#define __HDR_PUBLIC_MEMBERS__
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC_MEMBERS__
/* End Includes **************************************************************/

/* Begin Function:_RMP_Stack_Init *********************************************
Description : Initiate the process stack when trying to start a process. Never
              call this function in user application.
Input       : rmp_ptr_t Stack - The stack address of the thread.
              rmp_ptr_t Size - The stack size of the thread.
              rmp_ptr_t Entry - The entry address of the thread.
              rmp_ptr_t Param - The argument to pass to the thread.
Output      : None.
Return      : rmp_ptr_t - The adjusted stack location.
******************************************************************************/
extern rmp_ptr_t __global_pointer$;
rmp_ptr_t _RMP_Stack_Init(rmp_ptr_t Stack,
                          rmp_ptr_t Size,
                          rmp_ptr_t Entry,
                          rmp_ptr_t Param)
{
    rmp_ptr_t End;
    struct RMP_RV32GP_Stack* Ptr;

    /* Compute & align stack */
    End=RMP_ROUND_DOWN(Stack+Size, 4U);
    Ptr=(struct RMP_RV32GP_Stack*)(End-sizeof(struct RMP_RV32GP_Stack));

    /* This is where PC is saved */
    Ptr->PC=Entry;
    /* We always initialize the mstatus register to initialize
     * the FPU, but whether it is present depends on the processor */
    Ptr->MSTATUS=0x1880U|0x2000U;
    /* We always initialize GP to a known value.
     * If a thread modifies this later (by itself), it is fine */
    Ptr->X3=(rmp_ptr_t)(&__global_pointer$);
    /* x10 for arguments */
    Ptr->X10=Param;

    /* Fill the rest for ease of identification */
    Ptr->X1=0x01010101U;
    Ptr->X4=0x04040404U;
    Ptr->X5=0x05050505U;
    Ptr->X6=0x06060606U;
    Ptr->X7=0x07070707U;
    Ptr->X8=0x08080808U;
    Ptr->X9=0x09090909U;
    Ptr->X11=0x11111111U;
    Ptr->X12=0x12121212U;
    Ptr->X13=0x13131313U;
    Ptr->X14=0x14141414U;
    Ptr->X15=0x15151515U;
    Ptr->X16=0x16161616U;
    Ptr->X17=0x17171717U;
    Ptr->X18=0x18181818U;
    Ptr->X19=0x19191919U;
    Ptr->X20=0x20202020U;
    Ptr->X21=0x21212121U;
    Ptr->X22=0x22222222U;
    Ptr->X23=0x23232323U;
    Ptr->X24=0x24242424U;
    Ptr->X25=0x25252525U;
    Ptr->X26=0x26262626U;
    Ptr->X27=0x27272727U;
    Ptr->X28=0x28282828U;
    Ptr->X29=0x29292929U;
    Ptr->X30=0x30303030U;
    Ptr->X31=0x31313131U;

    return (rmp_ptr_t)Ptr;
}
/* End Function:_RMP_Stack_Init **********************************************/

/* Begin Function:_RMP_Lowlvl_Init ********************************************
Description : Initialize the low level hardware of the system. This is adapted from
              Arduino and FE310 examples.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Lowlvl_Init(void)
{
    RMP_RV32GP_LOWLVL_INIT();
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
    RMP_RV32GP_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* Begin Function:_RMP_Yield **************************************************
Description : Trigger a yield to another thread. This will trigger the software
              interrupt in RISC-V.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Yield(void)
{
    RMP_RV32GP_CTX_SET();

    __RMP_RV32GP_Mem_FENCE();
}
/* End Function:_RMP_Yield ***************************************************/

/* Begin Function:__RMP_RV32GP_Ctx_Handler ************************************
Description : The PendSV interrupt routine. This is used to switch contexts.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void __RMP_RV32GP_Ctx_Handler(void)
{
    RMP_RV32GP_CTX_CLR();

    RMP_Ctx_Save();
    _RMP_Run_High();
    RMP_Ctx_Load();
}
/* End Function:__RMP_RV32GP_Ctx_Handler *************************************/

/* Begin Function:__RMP_RV32GP_Tim_Handler ************************************
Description : The Tick interrupt routine.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void __RMP_RV32GP_Tim_Handler(void)
{
    /* Update or reset the next tick */
    RMP_RV32GP_TIM_CLR();

    _RMP_Tim_Handler(1U);
}
/* End Function:__RMP_RV32GP_Tim_Handler *************************************/

/* Begin Function:__RMP_RV32GP_Vct_Handler ************************************
Description : The handler routine for peripherals.
Input       : rmp_ptr_t Mcause - The mcause register value.
Output      : None.
Return      : None.
******************************************************************************/
void __RMP_RV32GP_Vct_Handler(rmp_ptr_t Mcause)
{
    RMP_RV32GP_VCT_HANDLER(Mcause);
}
/* End Function:__RMP_RV32GP_Vct_Handler *************************************/

/* Begin Function:_RMP_RV32GP_Handler *****************************************
Description : The interrupt handler routine. This exists due to some processor
              lacking vectored interrupt mechanism, and when this is the case
              we're forced to do it here.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_RV32GP_Handler(void)
{
    rmp_ptr_t Mcause;

    Mcause=__RMP_RV32GP_MCAUSE_Get();

    /* If this is an exception, die here */
    RMP_ASSERT((Mcause&0x80000000U)!=0U);

    switch(Mcause&0x7FFFFFFFU)
    {
        /* Machine software interrupt */
        case RMP_RV32GP_MCAUSE_TIM:__RMP_RV32GP_Tim_Handler();break;
        /* Machine timer interrupt */
        case RMP_RV32GP_MCAUSE_CTX:__RMP_RV32GP_Ctx_Handler();break;
        /* Other OS-aware cases */
        default:__RMP_RV32GP_Vct_Handler(Mcause);break;
    }
}
/* End Function:_RMP_Int_Handler *********************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
