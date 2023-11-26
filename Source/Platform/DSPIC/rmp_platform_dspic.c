/******************************************************************************
Filename    : rmp_platform_dspic.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for DSPIC 33E. This also works for PIC24.
              The DSPIC compilers are special in the sense that they put all 
              data in the first 64K, and all code points which could be jumped
              to in the first 64K. Thus 16-bit rmp_ptr_t is sufficient, yet 
              when switching context we need to handle 24-bit code pointers.
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
    rmp_ptr_t Start;
    struct RMP_DSPIC_Stack* Ptr;
    
    /* Compute & align stack */
    Start=RMP_ROUND_UP(Stack, 2U);
    Ptr=(struct RMP_DSPIC_Stack*)Start;

    /* The entry - SFA bit not set */
    Ptr->PCL=Entry;
    /* Last 8 bits of status register, IPL3, PC residue - all zero*/
    Ptr->PCH=0U;
    /* Initial SR - all zero except for IPL, set to 2, because lowest actual level is 1 */
    Ptr->SR=((rmp_ptr_t)2U)<<5;
    /* W0-W14 */
    Ptr->W0=Param;
    Ptr->W1=0x0101U;
    Ptr->W2=0x0202U;
    Ptr->W3=0x0303U;
    Ptr->W4=0x0404U;
    Ptr->W5=0x0505U;
    Ptr->W6=0x0606U;
    Ptr->W7=0x0707U;
    Ptr->W8=0x0808U;
    Ptr->W9=0x0909U;
    Ptr->W10=0x1010U;
    Ptr->W11=0x1111U;
    Ptr->W12=0x1212U;
    Ptr->W13=0x1313U;
    Ptr->W14=0x1414U;
    /* We don't use SPLIM with OS, it is always untouched */
    /* ACCAL,ACCAH,ACCAU,ACCBL,ACCBH,ACCBU */
    Ptr->ACCAL=0U;
    Ptr->ACCAH=0U;
    Ptr->ACCAU=0U;
    Ptr->ACCBL=0U;
    Ptr->ACCBH=0U;
    Ptr->ACCBU=0U;
    /* DSRPAG,DSWPAG - reset to what the toolkit initialized it to be */
    Ptr->DSRPAG=RMP_DSRPAG_Val;
    Ptr->DSWPAG=RMP_DSWPAG_Val;
    /* RCOUNT,DCOUNT,DOSTARTL,DOSTARTH,DOENDL,DOENDH */
    Ptr->RCOUNT=0U;
    Ptr->DCOUNT=0U;
    Ptr->DOSTARTL=0U;
    Ptr->DOSTARTH=0U;
    Ptr->DOENDL=0U;
    Ptr->DOENDH=0U;
    /* CORCON */
    Ptr->CORCON=0x0020U;
    /* MODCON */
    Ptr->MODCON=0x0000U;
    /* XMODSRT,XMODEND,YMODSRT,YMODEND */
    Ptr->XMODSRT=0x0000U;
    Ptr->XMODEND=0x0001U;
    Ptr->YMODSRT=0x0000U;
    Ptr->YMODEND=0x0001U;
    /* XBREV */
    Ptr->XBREV=0x0000U;
    /* TBLPAG */
    Ptr->TBLPAG=RMP_TBLPAG_Val;
    /* MSTRPR */
    Ptr->MSTRPR=0x0000U;

    return (rmp_ptr_t)Ptr;
}
/* End Function:_RMP_Stack_Init **********************************************/

/* Function:_RMP_Clear_Soft_Flag **********************************************
Description : Clear the software interrupt flag in the interrupt controller.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Clear_Soft_Flag(void)
{
    RMP_DSPIC_CLEAR_SOFT_FLAG();
}
/* End Function:_RMP_Clear_Soft_Flag *****************************************/

/* Function:_RMP_Clear_Timer_Flag *********************************************
Description : Clear the timer interrupt flag in the interrupt controller.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Clear_Timer_Flag(void)
{
    RMP_DSPIC_CLEAR_TIMER_FLAG();
}
/* End Function:_RMP_Clear_Timer_Flag ****************************************/

/* Function:_RMP_Lowlvl_Init **************************************************
Description : Initialize the low level hardware of the system.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Lowlvl_Init(void)
{
    RMP_DSPIC_LOWLVL_INIT();
}
/* End Function:_RMP_Lowlvl_Init *********************************************/

/* Function:_RMP_Set_Timer ****************************************************
Description    : The function for setting the timer.
Input          : rmp_ptr_t Ticks - Timer overflow value.
Output         : None.    
Register Usage : None.                                  
******************************************************************************/
void _RMP_Set_Timer(rmp_ptr_t Ticks)
{
    RMP_DSPIC_SET_TIMER(Ticks);
}
/* End Function:_RMP_Set_Timer ***********************************************/

/* Function:_RMP_Yield ********************************************************
Description : Trigger a yield to another thread.
Input       : None.
Output      : None.                                      
******************************************************************************/
void _RMP_Yield(void)
{
    RMP_DSPIC_YIELD();
}                                 
/* End Function:_RMP_Yield ***************************************************/

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
    RMP_DSPIC_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/