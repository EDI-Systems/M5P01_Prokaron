/******************************************************************************
Filename    : rmp_platform_dspic.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for DSPIC 33E. This also works for PIC24.
******************************************************************************/

/* Includes ******************************************************************/
#define __HDR_DEFS__
#include "Platform/DSPIC/rmp_platform_dspic.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEFS__

#define __HDR_STRUCTS__
#include "Platform/DSPIC/rmp_platform_dspic.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCTS__

/* Private include */
#include "Platform/DSPIC/rmp_platform_dspic.h"

#define __HDR_PUBLIC_MEMBERS__
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC_MEMBERS__
/* End Includes **************************************************************/

/* Begin Function:_RMP_Clear_Soft_Flag ****************************************
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

/* Begin Function:_RMP_Clear_Timer_Flag ***************************************
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
    rmp_ptr_t* Stack_Ptr;
    Stack_Ptr=(rmp_ptr_t*)(Stack-sizeof(rmp_ptr_t)*41);
    
    /* The entry - SFA bit not set */
    Stack_Ptr[0]=Entry;
    /* Last 8 bits of status register, IPL3, PC residue - all zero*/
    Stack_Ptr[1]=0U;
    /* Initial SR - all zero except for IPL, set to 2, because lowest actual level is 1 */
    Stack_Ptr[2]=((rmp_ptr_t)2U)<<5;
    /* W0-W14 */
    Stack_Ptr[3]=Arg;
    Stack_Ptr[4]=0x0101U;
    Stack_Ptr[5]=0x0202U;
    Stack_Ptr[6]=0x0303U;
    Stack_Ptr[7]=0x0404U;
    Stack_Ptr[8]=0x0505U;
    Stack_Ptr[9]=0x0606U;
    Stack_Ptr[10]=0x0707U;
    Stack_Ptr[11]=0x0808U;
    Stack_Ptr[12]=0x0909U;
    Stack_Ptr[13]=0x1010U;
    Stack_Ptr[14]=0x1111U;
    Stack_Ptr[15]=0x1212U;
    Stack_Ptr[16]=0x1313U;
    Stack_Ptr[17]=0x1414U;
    /* We don't use SPLIM with OS, it is always untouched */
    /* ACCAL,ACCAH,ACCAU,ACCBL,ACCBH,ACCBU */
    Stack_Ptr[18]=0U;
    Stack_Ptr[19]=0U;
    Stack_Ptr[20]=0U;
    Stack_Ptr[21]=0U;
    Stack_Ptr[22]=0U;
    Stack_Ptr[23]=0U;
    /* DSRPAG,DSWPAG - reset to what the toolkit initialized it to be */
    Stack_Ptr[24]=RMP_DSRPAG_Val;
    Stack_Ptr[25]=RMP_DSWPAG_Val;
    /* RCOUNT,DCOUNT,DOSTARTL,DOSTARTH,DOENDL,DOENDH */
    Stack_Ptr[26]=0U;
    Stack_Ptr[27]=0U;
    Stack_Ptr[28]=0U;
    Stack_Ptr[29]=0U;
    Stack_Ptr[30]=0U;
    Stack_Ptr[31]=0U;
    /* CORCON */
    Stack_Ptr[32]=0x0020U;
    /* MODCON */
    Stack_Ptr[33]=0x0000U;
    /* XMODSRT,XMODEND,YMODSRT,YMODEND */
    Stack_Ptr[34]=0x0000U;
    Stack_Ptr[35]=0x0001U;
    Stack_Ptr[36]=0x0000U;
    Stack_Ptr[37]=0x0001U;
    /* XBREV */
    Stack_Ptr[38]=0x0000U;
    /* TBLPAG */
    Stack_Ptr[39]=RMP_TBLPAG_Val;
    /* MSTRPR */
    Stack_Ptr[40]=0x0000U;
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
    RMP_DSPIC_LOWLVL_INIT();
}
/* End Function:_RMP_Lowlvl_Init *********************************************/

/* Begin Function:_RMP_Set_Timer **********************************************
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

/* Begin Function:_RMP_Yield **************************************************
Description : Trigger a yield to another thread.
Input       : None.
Output      : None.                                      
******************************************************************************/
void _RMP_Yield(void)
{
    RMP_DSPIC_YIELD();
}                                 
/* End Function:_RMP_Yield ***************************************************/

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
    RMP_DSPIC_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/