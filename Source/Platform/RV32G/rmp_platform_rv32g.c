/******************************************************************************
Filename    : rmp_platform_rv32g.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for RV32G. Any subsets that include
              RV32I are supported by this port, and this is also compatible
              with processors that support RV32C in addition to RV32G.
              Note that processors only supporting RV32C are not covered by
              this port and should use the RV32C port instead.
******************************************************************************/

/* Includes ******************************************************************/
#define __HDR_DEFS__
#include "Platform/RV32G/rmp_platform_rv32g.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEFS__

#define __HDR_STRUCTS__
#include "Platform/RV32G/rmp_platform_rv32g.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCTS__

/* Private include */
#include "Platform/RV32G/rmp_platform_rv32g.h"

#define __HDR_PUBLIC_MEMBERS__
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC_MEMBERS__
/* End Includes **************************************************************/

/* Begin Function:_RMP_Stack_Init *********************************************
Description : Initiate the thread stack when trying to start a thread. Never
              call this function in user application.
Input       : rmp_ptr_t Entry - The entry of the thread.
              rmp_ptr_t Stack - The stack address of the thread.
              rmp_ptr_t Arg - The argument to pass to the thread.
Output      : None.
Return      : None.
******************************************************************************/
extern rmp_ptr_t __global_pointer$;
void _RMP_Stack_Init(rmp_ptr_t Entry, rmp_ptr_t Stack, rmp_ptr_t Arg)
{
    rmp_ptr_t* Stack_Ptr;

    Stack_Ptr=(rmp_ptr_t*)Stack;

    /* This is where PC is saved */
    Stack_Ptr[0]=Entry;
    Stack_Ptr[1]=0x01010101U;
    /* We always initialize the mstatus register to initialize
     * the FPU, but whether it is present depends on the processor */
    Stack_Ptr[2]=0x1880U|0x2000U;
    /* We always initialize GP to a known value.
     * If a thread modifies this later (by itself), it is fine */
    Stack_Ptr[3]=(rmp_ptr_t)(&__global_pointer$);
    Stack_Ptr[4]=0x04040404U;
    Stack_Ptr[5]=0x05050505U;
    Stack_Ptr[6]=0x06060606U;
    Stack_Ptr[7]=0x07070707U;
    Stack_Ptr[8]=0x08080808U;
    Stack_Ptr[9]=0x09090909U;
    /* x10 for arguments */
    Stack_Ptr[10]=Arg;
    Stack_Ptr[11]=0x11111111U;
    Stack_Ptr[12]=0x12121212U;
    Stack_Ptr[13]=0x13131313U;
    Stack_Ptr[14]=0x14141414U;
    Stack_Ptr[15]=0x15151515U;
    Stack_Ptr[16]=0x16161616U;
    Stack_Ptr[17]=0x17171717U;
    Stack_Ptr[18]=0x18181818U;
    Stack_Ptr[19]=0x19191919U;
    Stack_Ptr[20]=0x20202020U;
    Stack_Ptr[21]=0x21212121U;
    Stack_Ptr[22]=0x22222222U;
    Stack_Ptr[23]=0x23232323U;
    Stack_Ptr[24]=0x24242424U;
    Stack_Ptr[25]=0x25252525U;
    Stack_Ptr[26]=0x26262626U;
    Stack_Ptr[27]=0x27272727U;
    Stack_Ptr[28]=0x28282828U;
    Stack_Ptr[29]=0x29292929U;
    Stack_Ptr[30]=0x30303030U;
    Stack_Ptr[31]=0x31313131U;
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
    RMP_RV32G_LOWLVL_INIT();
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
    RMP_RV32G_PUTCHAR(Char);
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
    RMP_RV32G_SWITCH_SET();

    _RMP_Mem_FENCE();
}
/* End Function:_RMP_Yield ***************************************************/

/* Begin Function:RMP_RV32G_Switch_Handler *********************************
Description : The PendSV interrupt routine. This is used to switch contexts.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_RV32G_Switch_Handler(void)
{
    RMP_RV32G_SWITCH_CLR();

    RMP_Ctx_Save();
    _RMP_Run_High();
    RMP_Ctx_Load();
}
/* End Function:RMP_RV32G_Switch_Handler **********************************/

/* Begin Function:RMP_RV32G_Tick_Handler ***********************************
Description : The Tick interrupt routine.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_RV32G_Tick_Handler(void)
{
    /* Update or reset the next tick */
    RMP_RV32G_TICK_RESET();

    _RMP_Tim_Handler(1U);
}
/* End Function:RMP_RV32G_Tick_Handler ************************************/

/* Begin Function:RMP_RV32G_Periph_Handler *********************************
Description : The handler routine for peripherals.
Input       : rmp_ptr_t Mcause - The mcause register value.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_RV32G_Periph_Handler(rmp_ptr_t Mcause)
{
    RMP_RV32G_PERIPH_HANDLER();
}
/* End Function:RMP_RV32G_Periph_Handler **************************************/

/* Begin Function:_RMP_Int_Handler ********************************************
Description : The interrupt handler routine. This exists due to some processor
              lacking vectored interrupt mechanism, and when this is the case
              we're forced to do it here.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Int_Handler(void)
{
    rmp_ptr_t Mcause;

    Mcause=_RMP_MCAUSE_Get();

    /* If this is an exception, die here */
    RMP_ASSERT((Mcause&0x80000000U)!=0U);

    switch(Mcause&0x7FFFFFFFU)
    {
        /* Machine software interrupt */
        case RMP_RV32G_TICK_MCAUSE:RMP_RV32G_Tick_Handler();break;
        /* Machine timer interrupt */
        case RMP_RV32G_SWITCH_MCAUSE:RMP_RV32G_Switch_Handler();break;
        /* Other OS-aware cases */
        default:RMP_RV32G_Periph_Handler(Mcause);break;
    }
}
/* End Function:_RMP_Int_Handler *********************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
