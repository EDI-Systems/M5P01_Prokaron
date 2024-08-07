/******************************************************************************
Filename    : rmp_platform_avr.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for AVR.
******************************************************************************/

/* Include *******************************************************************/
#define __HDR_DEF__
#include "Platform/AVR/rmp_platform_avr.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "Platform/AVR/rmp_platform_avr.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Platform/AVR/rmp_platform_avr.h"

#define __HDR_PUBLIC__
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC__
/* End Include ***************************************************************/

/* Function:_RMP_AVR_MSB_Get **************************************************
Description : Get the MSB of the word. AVR is slow anyway, so we avoid the
              generic version to save up some code and data space.
Input       : rmp_ptr_t Value - The value.
Output      : None.
Return      : rmp_ptr_t - The MSB position.
******************************************************************************/
rmp_ptr_t _RMP_AVR_MSB_Get(rmp_ptr_t Value)
{
    rmp_u8_t Half;
    rmp_u8_t Pos;
    
    Pos=0U;
    Half=Value>>8;
    
    if(Half!=0U)
        Pos+=8U;
    else
        Half=(rmp_u8_t)Value;
    
    if(Half>0x0FU)
    {
        Half>>=4U;
        Pos+=4U;
    }
    
    if(Half>0x03U)
    {
        Half>>=2U;
        Pos+=2U;
    }
    
    if(Half>0x01U)
        Pos++;
    
    return (rmp_ptr_t)Pos;
}
/* End Function:_RMP_AVR_MSB_Get *********************************************/

/* Function:_RMP_AVR_LSB_Get **************************************************
Description : Get the LSB of the word. AVR is slow anyway, so we avoid the 
              generic version to save up some code and data space.
Input       : rmp_ptr_t Value - The value.
Output      : None.
Return      : rmp_ptr_t - The LSB position.
******************************************************************************/
rmp_ptr_t _RMP_AVR_LSB_Get(rmp_ptr_t Value)
{
    rmp_u8_t Half;
    rmp_u8_t Pos;
    
    Pos=0U;
    Half=(rmp_u8_t)Value;
    
    if(Half==0U)
    {
        Half=Value>>8;
        Pos+=8U;
    }
    
    if((Half&0x0FU)==0U)
    {
        Half>>=4U;
        Pos+=4U;
    }
    
    if((Half&0x03U)==0U)
    {
        Half>>=2U;
        Pos+=2U;
    }
    
    if((Half&0x01U)==0U)
        Pos++;
    
    return (rmp_ptr_t)Pos;
}
/* End Function:_RMP_AVR_LSB_Get *********************************************/

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
    struct RMP_AVR_Stack* Ctx;
    
    RMP_STACK_CALC(Ptr,Ctx,Stack,Size);
    
    /* Set SREG to all zero for MegaAVR which uses RETI to set GIE, but set
     * GIE on XMegaAVR which uses PMIC to take care of interrupt nesting */
#if(RMP_AVR_COP_XMEGA!=0U)
    Ctx->SREG_SR=0x80U;
#else
    Ctx->SREG_SR=0x00U;
#endif
    
    /* Pass entry and parameter - program space is in words instead of bytes */
    Ctx->PCH=Entry>>8;
    Ctx->PCL=Entry&0xFFU;
    Ctx->R25=Param>>8;
    Ctx->R24=Param&0xFFU;
    
    /* Fill the rest for ease of identification - R1 is implicitly zero as required 
     * by GCC, but we still save/restore it in case the program includes assembly */
    Ctx->R0=0x00U;
    Ctx->R1=0x00U;
    Ctx->R2=0x02U;
    Ctx->R3=0x03U;
    Ctx->R4=0x04U;
    Ctx->R5=0x05U;
    Ctx->R6=0x06U;
    Ctx->R7=0x07U;
    Ctx->R8=0x08U;
    Ctx->R9=0x09U;
    Ctx->R10=0x10U;
    Ctx->R11=0x11U;
    Ctx->R12=0x12U;
    Ctx->R13=0x13U;
    Ctx->R14=0x14U;
    Ctx->R15=0x15U;
    Ctx->R16=0x16U;
    Ctx->R17=0x17U;
    Ctx->R18=0x18U;
    Ctx->R19=0x19U;
    Ctx->R20=0x20U;
    Ctx->R21=0x21U;
    Ctx->R22=0x22U;
    Ctx->R23=0x23U;
    Ctx->R26_XL=0x00U;
    Ctx->R27_XH=0x00U;
    Ctx->R28_YL=0x00U;
    Ctx->R29_YH=0x00U;
    Ctx->R30_ZL=0x00U;
    Ctx->R31_ZH=0x00U;

    /* EIND implies RAMP */
#if((RMP_AVR_COP_RAMP!=0U)||(RMP_AVR_COP_EIND!=0U))
    Ctx->RAMPD_ZU=0x00U;
    Ctx->RAMPX_XU=0x00U;
    Ctx->RAMPY_YU=0x00U;
    Ctx->RAMPZ_ZU=0x00U;
#endif

#if(RMP_AVR_COP_EIND!=0U)
    Ctx->EIND_ZU=0x00U;
    Ctx->PCU=0x00U;
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
    
    RMP_AVR_LOWLVL_INIT();
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
#if(RMP_AVR_COP_XMEGA!=0U)
    /* Scheduler lock implemented with interrupt masking */
    RMP_Int_Enable();
#else
    /* Scheduler lock implemented with interrupt disabling */
#endif
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
    RMP_AVR_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* Function:_RMP_AVR_Tim_Handler **********************************************
Description : Timer interrupt routine for AVR.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_AVR_Tim_Handler(void)
{
    RMP_AVR_TIM_CLR();

    _RMP_Tim_Handler(1U);
}
/* End Function:_RMP_AVR_Tim_Handler *****************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
