/******************************************************************************
Filename    : rmp_platform_mp32p.c
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

/* Include *******************************************************************/
#define __HDR_DEF__
#include "Platform/MP32P/rmp_platform_mp32p.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "Platform/MP32P/rmp_platform_mp32p.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Platform/MP32P/rmp_platform_mp32p.h"

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
    rmp_ptr_t End;
    struct RMP_MP32P_Stack* Ptr;
    
    /* Compute & align stack - full descending */
    End=RMP_ROUND_DOWN(Stack+Size, 3U);
    Ptr=(struct RMP_MP32P_Stack*)(End-sizeof(struct RMP_MP32P_Stack));
    
    /* General purpose registers */
    Ptr->R1_AT=0x01010101U;
    Ptr->R2_V0=0x02020202U;
    Ptr->R3_V1=0x03030303U;
    Ptr->R4_A0=Param;
    Ptr->R5_A1=0x05050505U;
    Ptr->R6_A2=0x06060606U;
    Ptr->R7_A3=0x07070707U;
    Ptr->R8_T0=0x08080808U;
    Ptr->R9_T1=0x09090909U;
    Ptr->R10_T2=0x10101010U;
    Ptr->R11_T3=0x11111111U;
    Ptr->R12_T4=0x12121212U;
    Ptr->R13_T5=0x13131313U;
    Ptr->R14_T6=0x14141414U;
    Ptr->R15_T7=0x15151515U;
    Ptr->R16_S0=0x16161616U;
    Ptr->R17_S1=0x17171717U;
    Ptr->R18_S2=0x18181818U;
    Ptr->R19_S3=0x19191919U;
    Ptr->R20_S4=0x20202020U;
    Ptr->R21_S5=0x21212121U;
    Ptr->R22_S6=0x22222222U;
    Ptr->R23_S7=0x23232323U;
    Ptr->R24_T8=0x24242424U;
    Ptr->R25_T9=0x25252525U;
    Ptr->R26_K0=0x26262626U;
    Ptr->R27_K1=0x27272727U;
    Ptr->R28_GP=RMP_GP_Val;
    Ptr->R30_FP=0x30303030U;
    Ptr->R31_RA=0x31313131U;
    Ptr->LO=0x15151515U;
    Ptr->HI=0x51515151U;
    
    /* Status registers - The EXL prevents premature interrupt enabling */
    Ptr->STATUS=RMP_MP32P_STATUS_IE|RMP_MP32P_STATUS_EXL;
    Ptr->PC=Entry;
    
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
    RMP_MP32P_CLEAR_SOFT_FLAG();
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
    _RMP_Set_Timer(RMP_MP32P_CORETIM_VAL/2);
    RMP_MP32P_CLEAR_TIMER_FLAG();
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
    RMP_MP32P_LOWLVL_INIT();
    
    RMP_Int_Disable();
    RMP_Int_Nest=0;
    
    /* Set the timer timeout value */
    _RMP_Set_Timer(RMP_MP32P_CORETIM_VAL/2);
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
    RMP_MP32P_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
