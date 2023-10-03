/******************************************************************************
Filename    : rmp_platform_mipsm.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
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
#if(RMP_MIPSM_INIT_EXTRA==1U)
    Stack_Ptr[0]=0x01010101U;                                   /* R1 */
    Stack_Ptr[1]=0x02020202U;                                   /* R2 */
    Stack_Ptr[2]=0x03030303U;                                   /* R3 */
#endif
    Stack_Ptr[3]=Arg;                                           /* R4 */
#if(RMP_MIPSM_INIT_EXTRA==1U)
    Stack_Ptr[4]=0x05050505U;                                   /* R5 */
    Stack_Ptr[5]=0x06060606U;                                   /* R6 */
    Stack_Ptr[6]=0x07070707U;                                   /* R7 */
    Stack_Ptr[7]=0x08080808U;                                   /* R8 */
    Stack_Ptr[8]=0x09090909U;                                   /* R9 */
    Stack_Ptr[9]=0x10101010U;                                   /* R10 */
    Stack_Ptr[10]=0x11111111U;                                  /* R11 */
    Stack_Ptr[11]=0x12121212U;                                  /* R12 */
    Stack_Ptr[12]=0x13131313U;                                  /* R13 */
    Stack_Ptr[13]=0x14141414U;                                  /* R14 */
    Stack_Ptr[14]=0x15151515U;                                  /* R15 */
    Stack_Ptr[15]=0x16161616U;                                  /* R16 */
    Stack_Ptr[16]=0x17171717U;                                  /* R17 */
    Stack_Ptr[17]=0x18181818U;                                  /* R18 */
    Stack_Ptr[18]=0x19191919U;                                  /* R19 */
    Stack_Ptr[19]=0x24242424U;                                  /* R24 */
    Stack_Ptr[20]=0x25252525U;                                  /* R25 */
    Stack_Ptr[21]=0x30303030U;                                  /* R30 */
    Stack_Ptr[22]=0x31313131U;                                  /* R31 */
    /* Kernel registers */
    Stack_Ptr[23]=0x26262626U;                                  /* R26 */
    Stack_Ptr[24]=0x27272727U;                                  /* R27 */
#endif
    Stack_Ptr[25]=RMP_GP_Val;                                   /* R28 */
#if(RMP_MIPSM_INIT_EXTRA==1U)
    /* Multiply/divide */
    Stack_Ptr[26]=0x15151515U;                                  /* LO */
    Stack_Ptr[27]=0x51515151U;                                  /* HI */
#endif
    /* Status registers - The EXL prevents premature interrupt enabling */
    Stack_Ptr[28]=RMP_MIPSM_STATUS_IE|RMP_MIPSM_STATUS_EXL;     /* CP0_STATUS */
    Stack_Ptr[29]=Entry;                                        /* CP0_EPC */
#if(RMP_MIPSM_INIT_EXTRA==1U)
    /* Some general-purpose scratch regs */
    Stack_Ptr[30]=0x20202020U;                                  /* R20 */
    Stack_Ptr[31]=0x21212121U;                                  /* R21 */
    Stack_Ptr[32]=0x22222222U;                                  /* R22 */
    Stack_Ptr[33]=0x23232323U;                                  /* R23 */
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
    RMP_MIPSM_LOWLVL_INIT();
    
    RMP_Int_Disable();
    RMP_Int_Nest=0;
    
    /* Set the timer timeout value */
    _RMP_Set_Timer(RMP_MIPSM_TICK_VAL/2);
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
    RMP_MIPSM_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
