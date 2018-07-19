/******************************************************************************
Filename    : rmp_platform_mipsm.c
Author      : pry
Date        : 04/02/2018
Licence     : LGPL v3+; see COPYING for details.
Description : The platform specific file for MIPS M-class. 
              Currently M4k and M14kF is supported.
              Beware that sometimes GCC-based compilers will use the LWX 
              instruction, which belongs to the DSP ASE expansion. We do not
              enable DSP ASE by default however, so passing no ASE flags to the
              compiler is necessary.
              Add -mno-dsp and -mno-dspr2
******************************************************************************/

/* Includes ******************************************************************/
#define __HDR_DEFS__
#include "Platform/MIPSM/rmp_platform_mipsm.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEFS__

#define __HDR_STRUCTS__
#include "Platform/MIPSM/rmp_platform_mipsm.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCTS__

/* Private include */
#include "Platform/MIPSM/rmp_platform_mipsm.h"

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
    RMP_MIPSM_CLEAR_SOFT_FLAG();
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
    _RMP_Set_Timer(RMP_MIPSM_TICK_VAL/2);
    RMP_MIPSM_CLEAR_TIMER_FLAG();
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
    
    Stack_Ptr=(rmp_ptr_t*)Stack;
    /* General purpose registers */
#if(RMP_MIPSM_INIT_EXTRA==RMP_TRUE)
    Stack_Ptr[0]=0x01010101;                                    /* R1 */
    Stack_Ptr[1]=0x02020202;                                    /* R2 */
    Stack_Ptr[2]=0x03030303;                                    /* R3 */
#endif
    Stack_Ptr[3]=Arg;                                           /* R4 */
#if(RMP_MIPSM_INIT_EXTRA==RMP_TRUE)
    Stack_Ptr[4]=0x05050505;                                    /* R5 */
    Stack_Ptr[5]=0x06060606;                                    /* R6 */
    Stack_Ptr[6]=0x07070707;                                    /* R7 */
    Stack_Ptr[7]=0x08080808;                                    /* R8 */
    Stack_Ptr[8]=0x09090909;                                    /* R9 */
    Stack_Ptr[9]=0x10101010;                                    /* R10 */
    Stack_Ptr[10]=0x11111111;                                   /* R11 */
    Stack_Ptr[11]=0x12121212;                                   /* R12 */
    Stack_Ptr[12]=0x13131313;                                   /* R13 */
    Stack_Ptr[13]=0x14141414;                                   /* R14 */
    Stack_Ptr[14]=0x15151515;                                   /* R15 */
    Stack_Ptr[15]=0x16161616;                                   /* R16 */
    Stack_Ptr[16]=0x17171717;                                   /* R17 */
    Stack_Ptr[17]=0x18181818;                                   /* R18 */
    Stack_Ptr[18]=0x19191919;                                   /* R19 */
    Stack_Ptr[19]=0x24242424;                                   /* R24 */
    Stack_Ptr[20]=0x25252525;                                   /* R25 */
    Stack_Ptr[21]=0x30303030;                                   /* R30 */
    Stack_Ptr[22]=0x31313131;                                   /* R31 */
    /* Kernel registers */
    Stack_Ptr[23]=0x26262626;                                   /* R26 */
    Stack_Ptr[24]=0x27272727;                                   /* R27 */
#endif
    Stack_Ptr[25]=RMP_GP_Val;                                   /* R28 */
#if(RMP_MIPSM_INIT_EXTRA==RMP_TRUE)
    /* Multiply/divide */
    Stack_Ptr[26]=0x15151515;                                    /* LO */
    Stack_Ptr[27]=0x51515151;                                    /* HI */
#endif
    /* Status registers - The EXL prevents premature interrupt enabling */
    Stack_Ptr[28]=RMP_MIPSM_STATUS_IE|RMP_MIPSM_STATUS_EXL;     /* CP0_STATUS */
    Stack_Ptr[29]=Entry;                                        /* CP0_EPC */
#if(RMP_MIPSM_INIT_EXTRA==RMP_TRUE)
    /* Some general-purpose scratch regs */
    Stack_Ptr[30]=0x20202020;                                   /* R20 */
    Stack_Ptr[31]=0x21212121;                                   /* R21 */
    Stack_Ptr[32]=0x22222222;                                   /* R22 */
    Stack_Ptr[33]=0x23232323;                                   /* R23 */
#endif
}
/* End Function:_RMP_Stack_Init **********************************************/

/* Begin Function:_RMP_Low_Level_Init *****************************************
Description : Initialize the low level hardware of the system.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Low_Level_Init(void)
{
    RMP_MIPSM_LOW_LEVEL_INIT();
    
    RMP_Disable_Int();
    RMP_Int_Nest=0;
    
    /* Set the timer timeout value */
    _RMP_Set_Timer(RMP_MIPSM_TICK_VAL/2);
}
/* End Function:_RMP_Low_Level_Init ******************************************/

/* Begin Function:_RMP_Plat_Hook **********************************************
Description : Platform-specific hook for system initialization.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Plat_Hook(void)
{
    RMP_Enable_Int();
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
    RMP_MIPSM_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
