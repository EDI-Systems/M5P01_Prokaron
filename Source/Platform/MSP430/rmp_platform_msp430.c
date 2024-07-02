/******************************************************************************
Filename    : rmp_platform_msp430.c
Author      : pry
Date        : 25/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for MSP430.
******************************************************************************/

/* Include *******************************************************************/
#define __HDR_DEF__
#include "Platform/MSP430/rmp_platform_msp430.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "Platform/MSP430/rmp_platform_msp430.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Platform/MSP430/rmp_platform_msp430.h"

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
    struct RMP_MSP430_Stack* Ctx;
    
    RMP_STACK_CALC(Ptr,Ctx,Stack,Size);

#if(RMP_MSP430_COP_430X!=0U)
    Ctx->R12=Param;
    Ctx->PCSR=RMP_MSP430X_PCSR(Entry, RMP_MSP430_SR_GIE);

    /* Fill the rest for ease of identification */
    Ctx->R4=0x040404UL;
    Ctx->R5=0x050505UL;
    Ctx->R6=0x060606UL;
    Ctx->R7=0x070707UL;
    Ctx->R8=0x080808UL;
    Ctx->R9=0x090909UL;
    Ctx->R10=0x101010UL;
    Ctx->R11=0x111111UL;
    Ctx->R13=0x131313UL;
    Ctx->R14=0x141414UL;
    Ctx->R15=0x151515UL;
#else
    Ctx->R12=Param;
    Ctx->SR=RMP_MSP430_SR_GIE;
    Ctx->PC=Entry;

    /* Fill the rest for ease of identification */
    Ctx->R4=0x0404U;
    Ctx->R5=0x0505U;
    Ctx->R6=0x0606U;
    Ctx->R7=0x0707U;
    Ctx->R8=0x0808U;
    Ctx->R9=0x0909U;
    Ctx->R10=0x1010U;
    Ctx->R11=0x1111U;
    Ctx->R13=0x1313U;
    Ctx->R14=0x1414U;
    Ctx->R15=0x1515U;
#endif

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
    
    RMP_MSP430_LOWLVL_INIT();
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
    RMP_MSP430_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* Function:_RMP_MSP430_Tim_Handler *******************************************
Description : Timer interrupt routine for MSP430; this is for normal kernel.
              For a tickless kernel, the user should supply this handler.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_MSP430_Tim_Handler(void)
{
    RMP_MSP430_TIM_CLR();

    _RMP_Tim_Handler(1U);
}
/* End Function:_RMP_MSP430_Tim_Handler **************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
