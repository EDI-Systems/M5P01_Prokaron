/******************************************************************************
Filename    : rmp_platform_msp430.c
Author      : pry
Date        : 25/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for MSP430.
******************************************************************************/

/* Includes ******************************************************************/
#define __HDR_DEFS__
#include "Platform/MSP430/rmp_platform_msp430.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEFS__

#define __HDR_STRUCTS__
#include "Platform/MSP430/rmp_platform_msp430.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCTS__

/* Private include */
#include "Platform/MSP430/rmp_platform_msp430.h"

#define __HDR_PUBLIC_MEMBERS__
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC_MEMBERS__
/* End Includes **************************************************************/

/* Begin Function:_RMP_Yield **************************************************
Description : Trigger a yield to another thread.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Yield(void)
{
    if(RMP_MSP430_Int_Act!=0U)
        RMP_MSP430_Yield_Pend=1U;
    else
        _RMP_MSP430_Yield();
}
/* End Function:_RMP_Yield ***************************************************/

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
rmp_ptr_t _RMP_Stack_Init(rmp_ptr_t Stack,
                          rmp_ptr_t Size,
                          rmp_ptr_t Entry,
                          rmp_ptr_t Param)
{
    rmp_ptr_t End;
    struct RMP_MSP430_Stack* Ptr;

    /* Compute & align stack */
    End=RMP_ROUND_DOWN(Stack+Size, 2U);
    Ptr=(struct RMP_MSP430_Stack*)(End-sizeof(struct RMP_MSP430_Stack));

#if(RMP_MSP430_X!=0U)
    Ptr->R12=Param;
    Ptr->PCSR=RMP_MSP430X_PCSR(Entry, RMP_MSP430_SR_GIE);

    /* Fill the rest for ease of identification */
    Ptr->R4=0x040404UL;
    Ptr->R5=0x050505UL;
    Ptr->R6=0x060606UL;
    Ptr->R7=0x070707UL;
    Ptr->R8=0x080808UL;
    Ptr->R9=0x090909UL;
    Ptr->R10=0x101010UL;
    Ptr->R11=0x111111UL;
    Ptr->R13=0x131313UL;
    Ptr->R14=0x141414UL;
    Ptr->R15=0x151515UL;
#else
    Ptr->R12=Param;
    Ptr->SR=RMP_MSP430_SR_GIE;
    Ptr->PC=Entry;

    /* Fill the rest for ease of identification */
    Ptr->R4=0x0404U;
    Ptr->R5=0x0505U;
    Ptr->R6=0x0606U;
    Ptr->R7=0x0707U;
    Ptr->R8=0x0808U;
    Ptr->R9=0x0909U;
    Ptr->R10=0x1010U;
    Ptr->R11=0x1111U;
    Ptr->R13=0x1313U;
    Ptr->R14=0x1414U;
    Ptr->R15=0x1515U;
#endif

    return (rmp_ptr_t)Ptr;
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
    RMP_MSP430_LOWLVL_INIT();

    RMP_Int_Disable();

    /* Clear flags */
    RMP_MSP430_Int_Act=0U;
    RMP_MSP430_Yield_Pend=0U;
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
    RMP_MSP430_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* Begin Function:_RMP_MSP430_Tim_Handler *************************************
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
