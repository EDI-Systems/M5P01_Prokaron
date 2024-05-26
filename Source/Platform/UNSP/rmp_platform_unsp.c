/******************************************************************************
Filename    : rmp_platform_unsp.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for unSP.
******************************************************************************/

/* Include *******************************************************************/
#define __HDR_DEF__
#include "Platform/UNSP/rmp_platform_unsp.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "Platform/UNSP/rmp_platform_unsp.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Platform/UNSP/rmp_platform_unsp.h"

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
    rmp_ptr_t* Entry_Vector;
    struct RMP_UNSP_Stack* Ptr;
    
    /* Compute stack - empty descending, no alignment requirement */
    Ptr=(struct RMP_UNSP_Stack*)(Stack+Size-sizeof(struct RMP_UNSP_Stack)-1U);
    
    /* Pass entry/SR - the long entry is stored in face value at this address
     * rather than being at the address; this chip lacks true long jumps. */
    Entry_Vector=(rmp_ptr_t*)Entry;
    Ptr->SR_CSDS=Entry_Vector[0];
    Ptr->PC=Entry_Vector[1];
    
    /* Pass parameter - on stack */
    Ptr->Param=Param;
    
    /* Fill the rest for ease of identification */
    Ptr->R1=0x0001U;
    Ptr->R2=0x0002U;
    Ptr->R3_MRL=0x0003U;
    Ptr->R4_MRH=0x0004U;
    Ptr->R5_BP=0x0005U;
    
    /* Empty descending */
    return ((rmp_ptr_t)Ptr)-1U;
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
    
    RMP_UNSP_LOWLVL_INIT();
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
    /* Scheduler lock is coupled but not implemented with interrupt disabling */
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
    RMP_UNSP_PUTCHAR(Char);
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
    if(RMP_UNSP_Int_Act!=0U)
        _RMP_UNSP_Yield_Pend=1U;
    else
#if(RMP_UNSP_COP_SPV2!=0U)
        _RMP_UNSP_Yield_SPV2();
#else
        _RMP_UNSP_Yield_SPV1();
#endif
}
/* End Function:_RMP_Yield ***************************************************/

/* Function:_RMP_UNSP_Tim_Handler *********************************************
Description : Timer interrupt routine for DSPIC.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_UNSP_Tim_Handler(void)
{
    RMP_UNSP_TIM_CLR();

    _RMP_Tim_Handler(1U);
}
/* End Function:_RMP_UNSP_Tim_Handler ****************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
