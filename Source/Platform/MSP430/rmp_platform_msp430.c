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

/* Begin Function:_RMP_MSP430_Ctx_Clr *****************************************
Description : Clear the software interrupt flag in the interrupt controller.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_MSP430_Ctx_Clr(void)
{
    RMP_MSP430_CTX_CLR();
}
/* End Function:_RMP_MSP430_Ctx_Clr ******************************************/

/* Begin Function:_RMP_MSP430_Tim_Clr *****************************************
Description : Clear the timer interrupt flag in the interrupt controller.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_MSP430_Tim_Clr(void)
{
    RMP_MSP430_TIM_CLR();
}
/* End Function:_RMP_MSP430_Tim_Clr ******************************************/

/* Begin Function:_RMP_Yield **************************************************
Description : Trigger a yield to another thread. This will always trigger the
              timer 0 compare vector on MSP430.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Yield(void)
{
    /* Manully pend the interrupt */
    RMP_MSP430_CTX_SET();
}
/* End Function:_RMP_Yield ***************************************************/

/* Begin Function:_RMP_Stack_Init *********************************************
Description : Initiate the process stack when trying to start a process. Never
              call this function in user application.
Input       : rmp_ptr_t Entry - The entry of the thread.
              rmp_ptr_t Stack - The stack address of the thread.
              rmp_ptr_t Arg - The argument to pass to the thread.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Stack_Init(rmp_ptr_t Entry, rmp_ptr_t Stack, rmp_ptr_t Arg)
{
    rmp_ptr_t* Stack_Ptr=(rmp_ptr_t*)Stack;

    /* General purpose registers */
#if(RMP_MSP430_INIT_EXTRA==1U)
    Stack_Ptr[0]=0x0404;                                    /* R4 */
    Stack_Ptr[1]=0x0505;                                    /* R5 */
    Stack_Ptr[2]=0x0606;                                    /* R6 */
    Stack_Ptr[3]=0x0707;                                    /* R7 */
    Stack_Ptr[4]=0x0808;                                    /* R8 */
    Stack_Ptr[5]=0x0909;                                    /* R9 */
    Stack_Ptr[6]=0x1010;                                    /* R10 */
    Stack_Ptr[7]=0x1111;                                    /* R11 */
#endif
    Stack_Ptr[8]=Arg;                                       /* R12 */
#if(RMP_MSP430_INIT_EXTRA==1U)
    Stack_Ptr[9]=0x1313;                                    /* R13 */
    Stack_Ptr[10]=0x1414;                                   /* R14 */
    Stack_Ptr[11]=0x1515;                                   /* R15 */
#endif

#if(RMP_MSP430_X!=0U)
    ((rmp_u16_t*)Stack_Ptr)[24]=0;
    ((rmp_u16_t*)Stack_Ptr)[25]=((Entry>>4)&0xF000)|RMP_MSP430_SR_GIE;
    ((rmp_u16_t*)Stack_Ptr)[26]=Entry&0xFFFF;
#else
    Stack_Ptr[12]=RMP_MSP430_SR_GIE;                        /* Status */
    Stack_Ptr[13]=Entry;                                    /* PC */
#endif
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

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
