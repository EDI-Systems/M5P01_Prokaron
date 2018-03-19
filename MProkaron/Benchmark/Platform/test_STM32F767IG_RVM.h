/******************************************************************************
Filename    : test_STM32F767IG_RVM.h
Author      : pry 
Date        : 22/07/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The testbench for STM32F767IG, running in the RVM.
******************************************************************************/

/* Includes ******************************************************************/
#include "RMP.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* How to read counter */
#define COUNTER_READ()   ((TIM2->CNT)<<1)
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The STM32F7 timers are all 32 bits, so */
typedef ptr_t tim_t;
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
ptr_t Stack_1[256];
ptr_t Stack_2[256];

/* Page table stuff */
const struct RVM_Hdr_Pgtbl RMP_Pgtbl[1]=
{
    /* The first page table */
    {
        0                                                                      /* ptr_t Parent */,
        0x00000000                                                             /* ptr_t Addr */,
        RME_PGTBL_ORDER(RME_PGTBL_SIZE_512M,RME_PGTBL_NUM_8)                   /* ptr_t Order */,
        {                                                                      /* u8 Flags[8] */
            RME_PGTBL_ALL_PERM,RME_PGTBL_ALL_PERM,
            RME_PGTBL_ALL_PERM,RME_PGTBL_ALL_PERM,
            RME_PGTBL_ALL_PERM,RME_PGTBL_ALL_PERM,
            RME_PGTBL_ALL_PERM,RME_PGTBL_ALL_PERM
        }
    }
};

const struct RVM_Image RMP_Image=
{
    RVM_MAGIC                                                                  /* const ptr_t Magic */,
    "VM Domain 01"                                                             /* s8 Name[16] */,
    _RMP_Entry                                                                 /* void* User_Entry */,
    &RMP_User_Stack                                                            /* void* User_Stack */,
    0x100                                                                      /* ptr_t User_Size */,
    _RMP_Int                                                                   /* void* Int_Entry */,
    &RMP_Int_Stack                                                             /* void* Int_Stack */,
    0x400                                                                      /* ptr_t Int_Size */,
    
    &RMP_Param                                                                 /* struct RVM_Param* Param */,
    &RMP_Regs                                                                  /* struct RVM_Regs* Regs */,
    &RMP_Flag                                                                  /* struct RVM_Int_Flag* Int_Flags */,
    
    RMP_Console                                                                /* void* Console_Buf */,
    RMP_KERNEL_DEBUG_MAX_STR                                                   /* ptr_t Console_Size */,
    
    1                                                                          /* ptr_t Prio */,
    10                                                                         /* ptr_t Slices */,
    
    1                                                                          /* ptr_t Pgtbl_Num */,
    RMP_Pgtbl                                                                  /* const struct RVM_Hdr_Pgtbl* Pgtbl */,
    0                                                                          /* const struct RVM_Image* const * const Next_Image; */
};
/* End Globals ***************************************************************/

/* Begin Function:Timer_Init **************************************************
Description : Initialize the timer for timing measurements. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Timer_Init(void)
{
    /* Timer is initialized on startup by the M7M1 kernel */
}
/* End Function:Timer_Init ***************************************************/

/* Begin Function:Int_Init ****************************************************
Description : Initialize an periodic interrupt source. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Init(void)
{
    /* Interrupt generation is initialized too, here we only register our handler */
    RMP_Vect[2]=(ptr_t)Int_Handler;
}
/* End Function:Int_Init *****************************************************/

/* Begin Function:Int_Disable *************************************************
Description : Disable the periodic interrupt source. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Disable(void)
{
    /* Reverse registration */
    RMP_Vect[2]=0;
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/

