/******************************************************************************
Filename    : rmp_platform_c28x.c
Author      : zr (ran zhang)
Date        : 27/04/2024
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for C28X. Note that this architecture
              is 16-bit word addressing, where rmp_u8_t is in fact 16 bits.
******************************************************************************/

/* Include *******************************************************************/
#define __HDR_DEF__
#include "Platform/C28X/rmp_platform_c28x.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "Platform/C28X/rmp_platform_c28x.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Platform/C28X/rmp_platform_c28x.h"

#define __HDR_PUBLIC__
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC__
/* End Include ***************************************************************/

/* Function:_RMP_Yield ********************************************************
Description : Trigger a yield to another thread.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Yield(void)
{
    if(RMP_C28X_Int_Act!=0U)
        _RMP_C28X_Yield_Pend=1U;
    else
        /* Selecting FPU64 implies FPU32 */
#if(RMP_C28X_COP_FPU64!=0U)
        _RMP_C28X_Yield_FPU64();
#elif(RMP_C28X_COP_FPU32!=0U)
        _RMP_C28X_Yield_FPU32();
#else
        _RMP_C28X_Yield_NONE();
#endif
}
/* End Function:_RMP_Yield ***************************************************/

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
    struct RMP_C28X_Stack* Ptr;

    /* Word addressing, must be aligned when passed in - empty ascending */
    Ptr=(struct RMP_C28X_Stack*)RMP_ROUND_UP(Stack,1U);

    /* Pass entry and parameter */
    Ptr->PC=Entry;
    Ptr->ACC=Param;
    Ptr->DP_ST1=0x00008A08U;

    /* Enable interrupt for all sources, by default, for each thread */
    Ptr->DBGSTAT_IER=0x0000FFFFU;

    /* Initialize the rest of the registers */
    Ptr->AR1_AR0=0x01010000U;
    Ptr->AR1H_AR0H=0x01010000U;
    Ptr->XAR2=0x02020202U;
    Ptr->XAR3=0x03030303U;
    Ptr->XAR4=0x04040404U;
    Ptr->XAR5=0x05050505U;
    Ptr->XAR6=0x06060606U;
    Ptr->XAR7=0x07070707U;

    /* Selecting FPU64 implies FPU32 */
#if(RMP_C28X_COP_FPU64!=0U)
    /* Rounding mode - RND32 */
    Ptr->STF=0x00000200U;
    Ptr->RB=0x00000000U;
    Ptr->R0L=0x00000000U;
    Ptr->R0H=0x00000000U;
    Ptr->R1L=0x01010101U;
    Ptr->R1H=0x01010101U;
    Ptr->R2L=0x02020202U;
    Ptr->R2H=0x02020202U;
    Ptr->R3L=0x03030303U;
    Ptr->R3H=0x03030303U;
    Ptr->R4L=0x04040404U;
    Ptr->R4H=0x04040404U;
    Ptr->R5L=0x05050505U;
    Ptr->R5H=0x05050505U;
    Ptr->R6L=0x06060606U;
    Ptr->R6H=0x06060606U;
    Ptr->R7L=0x07070707U;
    Ptr->R7H=0x07070707U;
#elif(RMP_C28X_COP_FPU32!=0U)
    /* Rounding mode - RND32 */
    Ptr->STF=0x00000200U;
    Ptr->RB=0x00000000U;
    Ptr->R0H=0x00000000U;
    Ptr->R1H=0x01010101U;
    Ptr->R2H=0x02020202U;
    Ptr->R3H=0x03030303U;
    Ptr->R4H=0x04040404U;
    Ptr->R5H=0x05050505U;
    Ptr->R6H=0x06060606U;
    Ptr->R7H=0x07070707U;
#endif

    /* Empty ascending stack & need to account for the "+1" when "IRET"ing */
    return Stack+sizeof(struct RMP_C28X_Stack)+1U;
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
    
    RMP_C28X_LOWLVL_INIT();

    /* Clear flags */
    RMP_C28X_Int_Act=0U;
    _RMP_C28X_Yield_Pend=0U;
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
    /* Scheduler lock implemented with interrupt disabling */
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
    RMP_C28X_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* Function:_RMP_C28X_Tim_Handler *********************************************
Description : Timer interrupt routine for C28X; this is for normal kernel.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_C28X_Tim_Handler(void)
{
    RMP_C28X_TIM_CLR();

    _RMP_Tim_Handler(1U);
}
/* End Function:_RMP_C28X_Tim_Handler ****************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
