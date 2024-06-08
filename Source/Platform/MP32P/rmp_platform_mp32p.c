/******************************************************************************
Filename    : rmp_platform_mp32p.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for MIPS.
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
    rmp_ptr_t Ptr;
    struct RMP_MP32P_Stack* Ctx;
    
    Ptr=RMP_STACK_PTR(Stack,Size);
    Ctx=RMP_STACK_CTX(Ptr);
    
    /* Entry and parameter */
    Ctx->PC=Entry;
    Ctx->R4_A0=Param;
    
    /* General purpose registers */
    Ctx->R1_AT=0x01010101U;
    Ctx->R2_V0=0x02020202U;
    Ctx->R3_V1=0x03030303U;
    Ctx->R5_A1=0x05050505U;
    Ctx->R6_A2=0x06060606U;
    Ctx->R7_A3=0x07070707U;
    Ctx->R8_T0=0x08080808U;
    Ctx->R9_T1=0x09090909U;
    Ctx->R10_T2=0x10101010U;
    Ctx->R11_T3=0x11111111U;
    Ctx->R12_T4=0x12121212U;
    Ctx->R13_T5=0x13131313U;
    Ctx->R14_T6=0x14141414U;
    Ctx->R15_T7=0x15151515U;
    Ctx->R16_S0=0x16161616U;
    Ctx->R17_S1=0x17171717U;
    Ctx->R18_S2=0x18181818U;
    Ctx->R19_S3=0x19191919U;
    Ctx->R20_S4=0x20202020U;
    Ctx->R21_S5=0x21212121U;
    Ctx->R22_S6=0x22222222U;
    Ctx->R23_S7=0x23232323U;
    Ctx->R24_T8=0x24242424U;
    Ctx->R25_T9=0x25252525U;
    Ctx->R26_K0=0x26262626U;
    Ctx->R27_K1=0x27272727U;
    Ctx->R28_GP=_RMP_MP32P_GP_Kern;
    Ctx->R30_FP=0x30303030U;
    Ctx->R31_RA=0x31313131U;
    Ctx->LO=0x15151515U;
    Ctx->HI=0x51515151U;
 
    /* Status register - The EXL prevents premature interrupt enabling */
    Ctx->STATUS=RMP_MP32P_STATUS_CU0|RMP_MP32P_STATUS_IE|RMP_MP32P_STATUS_EXL;
    
    /* DSPASE extensions */
#if(RMP_MP32P_COP_DSPASE!=0U)
    Ctx->A1_LO=0x01010101U;
    Ctx->A1_HI=0x10101010U;
    Ctx->A2_LO=0x02020202U;
    Ctx->A2_HI=0x20202020U;
    Ctx->A3_LO=0x03030303U;
    Ctx->A3_HI=0x30303030U;
    Ctx->STATUS|=RMP_MP32P_STATUS_MX;
#endif
    
#if(RMP_MP32P_COP_FR64!=0U)
    Ctx->FSCR=RMP_MP32P_FCSR_MAC2008|RMP_MP32P_FCSR_ABS2008|RMP_MP32P_FCSR_NAN2008;
    Ctx->F0[0]=0x00000000U;
    Ctx->F0[1]=0x00000001U;
    Ctx->F1[0]=0x00010000U;
    Ctx->F1[1]=0x00010001U;
    Ctx->F2[0]=0x00020000U;
    Ctx->F2[1]=0x00020001U;
    Ctx->F3[0]=0x00030000U;
    Ctx->F3[1]=0x00030001U;
    Ctx->F4[0]=0x00040000U;
    Ctx->F4[1]=0x00040001U;
    Ctx->F5[0]=0x00050000U;
    Ctx->F5[1]=0x00050001U;
    Ctx->F6[0]=0x00060000U;
    Ctx->F6[1]=0x00060001U;
    Ctx->F7[0]=0x00070000U;
    Ctx->F7[1]=0x00070001U;
    Ctx->F8[0]=0x00080000U;
    Ctx->F8[1]=0x00080001U;
    Ctx->F9[0]=0x00090000U;
    Ctx->F9[1]=0x00090001U;
    Ctx->F10[0]=0x00100000U;
    Ctx->F10[1]=0x00100001U;
    Ctx->F11[0]=0x00110000U;
    Ctx->F11[1]=0x00110001U;
    Ctx->F12[0]=0x00120000U;
    Ctx->F12[1]=0x00120001U;
    Ctx->F13[0]=0x00130000U;
    Ctx->F13[1]=0x00130001U;
    Ctx->F14[0]=0x00140000U;
    Ctx->F14[1]=0x00140001U;
    Ctx->F15[0]=0x00150000U;
    Ctx->F15[1]=0x00150001U;
    Ctx->F16[0]=0x00160000U;
    Ctx->F16[1]=0x00160001U;
    Ctx->F17[0]=0x00170000U;
    Ctx->F17[1]=0x00170001U;
    Ctx->F18[0]=0x00180000U;
    Ctx->F18[1]=0x00180001U;
    Ctx->F19[0]=0x00190000U;
    Ctx->F19[1]=0x00190001U;
    Ctx->F20[0]=0x00200000U;
    Ctx->F20[1]=0x00200001U;
    Ctx->F21[0]=0x00210000U;
    Ctx->F21[1]=0x00210001U;
    Ctx->F22[0]=0x00220000U;
    Ctx->F22[1]=0x00220001U;
    Ctx->F23[0]=0x00230000U;
    Ctx->F23[1]=0x00230001U;
    Ctx->F24[0]=0x00240000U;
    Ctx->F24[1]=0x00240001U;
    Ctx->F25[0]=0x00250000U;
    Ctx->F25[1]=0x00250001U;
    Ctx->F26[0]=0x00260000U;
    Ctx->F26[1]=0x00260001U;
    Ctx->F27[0]=0x00270000U;
    Ctx->F27[1]=0x00270001U;
    Ctx->F28[0]=0x00280000U;
    Ctx->F28[1]=0x00280001U;
    Ctx->F29[0]=0x00290000U;
    Ctx->F29[1]=0x00290001U;
    Ctx->F30[0]=0x00300000U;
    Ctx->F30[1]=0x00300001U;
    Ctx->F31[0]=0x00310000U;
    Ctx->F31[1]=0x00310001U;
    Ctx->STATUS|=RMP_MP32P_STATUS_CU1|RMP_MP32P_STATUS_FR;
#elif(RMP_MP32P_COP_FR32!=0U)
    Ctx->FSCR=RMP_MP32P_FCSR_MAC2008|RMP_MP32P_FCSR_ABS2008|RMP_MP32P_FCSR_NAN2008;
    Ctx->F0=0x00000000U;
    Ctx->F1=0x00010000U;
    Ctx->F2=0x00020000U;
    Ctx->F3=0x00030000U;
    Ctx->F4=0x00040000U;
    Ctx->F5=0x00050000U;
    Ctx->F6=0x00060000U;
    Ctx->F7=0x00070000U;
    Ctx->F8=0x00080000U;
    Ctx->F9=0x00090000U;
    Ctx->F10=0x00000000U;
    Ctx->F11=0x00110000U;
    Ctx->F12=0x00120000U;
    Ctx->F13=0x00130000U;
    Ctx->F14=0x00140000U;
    Ctx->F15=0x00150000U;
    Ctx->F16=0x00160000U;
    Ctx->F17=0x00170000U;
    Ctx->F18=0x00180000U;
    Ctx->F19=0x00190000U;
    Ctx->F20=0x00200000U;
    Ctx->F21=0x00210000U;
    Ctx->F22=0x00220000U;
    Ctx->F23=0x00230000U;
    Ctx->F24=0x00240000U;
    Ctx->F25=0x00250000U;
    Ctx->F26=0x00260000U;
    Ctx->F27=0x00270000U;
    Ctx->F28=0x00280000U;
    Ctx->F29=0x00290000U;
    Ctx->F30=0x00300000U;
    Ctx->F31=0x00310000U;
    Ctx->STATUS|=RMP_MP32P_STATUS_CU1;
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
    RMP_MP32P_LOWLVL_INIT();
    
    RMP_Int_Disable();
    
    RMP_MP32P_Int_Act=0U;
    _RMP_MP32P_Yield_Pend=0U;
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
    RMP_MP32P_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* Function:_RMP_Yield ********************************************************
Description : Trigger a yield to another thread.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Yield(void)
{
    if(RMP_MP32P_Int_Act!=0U)
        _RMP_MP32P_Yield_Pend=1U;
    else
#if(RMP_MP32P_COP_DSPASE==0U)
#if(RMP_MP32P_COP_FR64!=0U)
        _RMP_MP32P_Yield_FR64();
#elif(RMP_MP32P_COP_FR32!=0U)
        _RMP_MP32P_Yield_FR32();
#else
        _RMP_MP32P_Yield_NONE();
#endif
#else
#if(RMP_MP32P_COP_FR64!=0U)
        _RMP_MP32P_Yield_DSPASE_FR64();
#elif(RMP_MP32P_COP_FR32!=0U)
        _RMP_MP32P_Yield_DSPASE_FR32();
#else
        _RMP_MP32P_Yield_DSPASE();
#endif
#endif
}
/* End Function:_RMP_Yield ***************************************************/

/* Function:_RMP_MP32P_Tim_Handler ********************************************
Description : Clear the timer interrupt flag in the interrupt controller.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_MP32P_Tim_Handler(void)
{
    RMP_MP32P_TIM_CLR();
    
    _RMP_Tim_Handler(1U);
}
/* End Function:_RMP_MP32P_Tim_Handler ***************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
