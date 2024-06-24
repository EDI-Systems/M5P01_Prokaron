/******************************************************************************
Filename    : rmp_platform_dspic.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for DSPIC.
******************************************************************************/

/* Include *******************************************************************/
#define __HDR_DEF__
#include "Platform/DSPIC/rmp_platform_dspic.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "Platform/DSPIC/rmp_platform_dspic.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Platform/DSPIC/rmp_platform_dspic.h"

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
    struct RMP_DSPIC_Stack* Ctx;
    
    Ptr=RMP_STACK_PTR(Stack,Size);
    Ctx=RMP_STACK_CTX(Ptr);

    /* The entry - SFA bit not set */
    Ctx->PCL=Entry;
    /* Last 8 bits of status register, SRL/IPL3/PCH zero */
    Ctx->PCHSRL=0U;
    /* Initial SR - IPL=1 to avoid premature interrupt enabling */
    Ctx->SR=0x0020U;
    /* CORCON - set to whatever the boot code gives us */
    Ctx->CORCON=_RMP_DSPIC_CORCON_Kern;
    /* W0-W14 */
    Ctx->W0=Param;
    Ctx->W1=0x0101U;
    Ctx->W2=0x0202U;
    Ctx->W3=0x0303U;
    Ctx->W4=0x0404U;
    Ctx->W5=0x0505U;
    Ctx->W6=0x0606U;
    Ctx->W7=0x0707U;
    Ctx->W8=0x0808U;
    Ctx->W9=0x0909U;
    Ctx->W10=0x1010U;
    Ctx->W11=0x1111U;
    Ctx->W12=0x1212U;
    Ctx->W13=0x1313U;
    Ctx->W14=0x1414U;
    /* RCOUNT */
    Ctx->RCOUNT=0x0000U;
    /* TBLPAG - set to whatever the boot code gives us */
    Ctx->TBLPAG=_RMP_DSPIC_TBLPAG_Kern;

    /* Specific visibility registers - set to whatever the boot code gives us */
#if((RMP_DSPIC_COP_24F_24H!=0U)||(RMP_DSPIC_COP_30F_33F!=0U))
    Ctx->PSVPAG=_RMP_DSPIC_PSVDSWPAG_Kern;
#elif((RMP_DSPIC_COP_24E!=0U)||(RMP_DSPIC_COP_33E_33C!=0U))
    Ctx->DSRPAG=_RMP_DSPIC_DSRPAG_Kern;
    Ctx->DSWPAG=_RMP_DSPIC_PSVDSWPAG_Kern;
#endif
    
    /* Specific DSP/addressing registers */
#if((RMP_DSPIC_COP_30F_33F!=0U)||(RMP_DSPIC_COP_33E_33C!=0U))
    /* ACCAL,ACCAH,ACCAU,ACCBL,ACCBH,ACCBU */
    Ctx->ACCAL=0xAAAAU;
    Ctx->ACCAH=0x0A0AU;
    Ctx->ACCAU=0x000AU;
    Ctx->ACCBL=0xBBBBU;
    Ctx->ACCBH=0x0B0BU;
    Ctx->ACCBU=0x000BU;
    /* MODCON */
    Ctx->MODCON=0x0000U;
    /* XMODSRT,XMODEND,YMODSRT,YMODEND */
    Ctx->XMODSRT=0x0000U;
    Ctx->XMODEND=0x0001U;
    Ctx->YMODSRT=0x0000U;
    Ctx->YMODEND=0x0001U;
    /* XBREV */
    Ctx->XBREV=0x0000U;
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
    
    RMP_DSPIC_LOWLVL_INIT();
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
    /* Scheduler lock implemented with interrupt masking */
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
    RMP_DSPIC_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* Function:_RMP_DSPIC_Tim_Handler ********************************************
Description : Timer interrupt routine for DSPIC.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_DSPIC_Tim_Handler(void)
{
    RMP_DSPIC_TIM_CLR();

    _RMP_Tim_Handler(1U);
}
/* End Function:_RMP_DSPIC_Tim_Handler ***************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/