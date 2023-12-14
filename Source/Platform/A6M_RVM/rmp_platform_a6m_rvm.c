/******************************************************************************
Filename    : rmp_platform_a6m_rvm.c
Author      : pry
Date        : 09/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for Cortex-M on RVM hypervisor.
******************************************************************************/

/* Include *******************************************************************/
#define __HDR_DEF__
#include "Platform/A6M_RVM/rmp_platform_a6m_rvm.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "Platform/A6M_RVM/rmp_platform_a6m_rvm.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Platform/A6M_RVM/rmp_platform_a6m_rvm.h"

#define __HDR_PUBLIC__
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC__

/* The virtual machine configs are here */
#include "rvm_guest.h"
/* End Include ***************************************************************/

/* Function:_RMP_Stack_Init ***************************************************
Description : Initiate the process stack when trying to start a process. Never
              call this function in user application.
              Need to pretend that we're returning from a context switch: 
                  21  20  19    18 17-14 13-10  9   8-5     4-1       0
              H> XPSR PC LR(1) R12 R3-R0 R7-R4 LR R11-R8 Param[3-0] Number >L
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
    struct RMP_A6M_RVM_Stack* Ptr;
    
    /* Compute & align stack */
    End=RMP_ROUND_DOWN(Stack+Size, 3U);
    Ptr=(struct RMP_A6M_RVM_Stack*)(End-sizeof(struct RMP_A6M_RVM_Stack));
    
    /* Set LR_EXC and xPSR accordingly to avoid INVSTATE */
    Ptr->LR_EXC=0xFFFFFFFDU;
    Ptr->XPSR=0x01000000U;
    
    /* Pass entry and parameter */
    Ptr->PC=Entry;
    Ptr->R0=Param;

    /* Hypercall not active */
    Ptr->Number=RVM_HYP_INVALID;

    /* Fill the rest for ease of identification */
    Ptr->R1=0x01010101U;
    Ptr->R2=0x02020202U;
    Ptr->R3=0x03030303U;
    Ptr->R4=0x04040404U;
    Ptr->R5=0x05050505U;
    Ptr->R6=0x06060606U;
    Ptr->R7=0x07070707U;
    Ptr->R8=0x08080808U;
    Ptr->R9=0x09090909U;
    Ptr->R10=0x10101010U;
    Ptr->R11=0x11111111U;
    Ptr->R12=0x12121212U;
    Ptr->LR=0x14141414U;
    
    return (rmp_ptr_t)Ptr;
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
    RVM_Virt_Tim_Reg(RMP_SysTick_Handler);
    RVM_Virt_Ctx_Reg(RMP_PendSV_Handler);
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
    /* Check header validity - guarantees that the header is not optimized out.
     * ALL VMs are guaranteed to have three entries: Vector, User and Stub */
    RVM_ASSERT(RVM_Desc[0]==RVM_MAGIC_VIRTUAL);
    RVM_ASSERT(RVM_Desc[1]==3U);
    /* Enable interrupt, we've finished all initialization */
    RVM_Hyp_Int_Ena();
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
#if(RVM_DEBUG_PRINT!=0U)
    RVM_Putchar(Char);
#endif
}
/* End Function:RMP_Putchar **************************************************/

/* Function:RMP_Int_Enable ****************************************************
Description : Enable interrupts.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Int_Enable(void)
{
    RVM_Hyp_Int_Ena();
}
/* End Function:RMP_Int_Enable ***********************************************/

/* Function:RMP_Int_Disable ***************************************************
Description : Disable interrupts.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Int_Disable(void)
{
    RVM_Hyp_Int_Dis();
}
/* End Function:RMP_Int_Disable **********************************************/

/* Function:RMP_Int_Mask ******************************************************
Description : Mask interrupts that may do sends.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Int_Mask(void)
{
    RVM_Virt_Int_Mask();
}
/* End Function:RMP_Int_Mask *************************************************/

/* Function:RMP_Int_Unmask ****************************************************
Description : Unmask interrupts that may do sends.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Int_Unmask(void)
{
    RVM_Virt_Int_Unmask();
}
/* End Function:RMP_Int_Unmask ***********************************************/

/* Function:_RMP_Yield ********************************************************
Description : Trigger a yield to a different thread.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
/* Use "const" to make sure this initializer is in code flash - this will
 * be optimized out when fast context switching is not enabled */
volatile struct RVM_Param* const RMP_A6M_RVM_Usr_Param=&(RVM_STATE->Usr);
void _RMP_Yield(void)
{
#if(RMP_A6M_RVM_FAST_YIELD!=0U)
    if(RVM_STATE->Vct_Act!=0U)
        RVM_Virt_Yield();
    else
        _RMP_A6M_RVM_Yield();
#else
    RVM_Virt_Yield();
#endif
}
/* End Function:_RMP_Yield ***************************************************/

/* Function:RMP_PendSV_Handler ************************************************
Description : The PendSV interrupt routine. Real Cortex-M0 does not have STMDB
              (only very restricted forms of STMIA and LDMIA exists), we're
              using it here as a comment for demonstration purposes only.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_PendSV_Handler(void)
{
    rmp_ptr_t* SP;

    /* MRS      R0,PSP */
    SP=(rmp_ptr_t*)(RVM_REG->Reg.SP);

    /* SUBS     R0,#36              ;Save the general purpose registers
     * MOV      R1, R0              ;Equivalent of STMDB R0!,{R4-R11,LR}
     * STMIA    R1!,{R4-R7}         ;Save low registers first due to limitation
     * MOV      R7,LR
     * MOV      R6,R11
     * MOV      R5,R10
     * MOV      R4,R9
     * MOV      R3,R8
     * STMIA    R1!,{R3-R7} */
    *(--SP)=RVM_REG->Reg.LR;
    *(--SP)=RVM_REG->Reg.R11;
    *(--SP)=RVM_REG->Reg.R10;
    *(--SP)=RVM_REG->Reg.R9;
    *(--SP)=RVM_REG->Reg.R8;
    *(--SP)=RVM_REG->Reg.R7;
    *(--SP)=RVM_REG->Reg.R6;
    *(--SP)=RVM_REG->Reg.R5;
    *(--SP)=RVM_REG->Reg.R4;

    /* Save all the user-accessible hypercall structure to stack */
    *(--SP)=RVM_STATE->Usr.Param[3];
    *(--SP)=RVM_STATE->Usr.Param[2];
    *(--SP)=RVM_STATE->Usr.Param[1];
    *(--SP)=RVM_STATE->Usr.Param[0];
    *(--SP)=RVM_STATE->Usr.Number;
    
    /* LDR      R1,=RMP_SP_Cur      ;Store the SP to control block
     * STR      R0,[R1] */
    RMP_SP_Cur=(rmp_ptr_t)SP;
                
    /* BL       _RMP_Run_High       ;Get the highest ready task */
    _RMP_Run_High();
    
    /* LDR      R1,=RMP_SP_Cur      ;Load the SP from control block
     * LDR      R0,[R1] */
    SP=(rmp_ptr_t*)RMP_SP_Cur;

    /* Restore the user-accessible hypercall structure to stack */
    RVM_STATE->Usr.Number=*(SP++);
    RVM_STATE->Usr.Param[0]=*(SP++);
    RVM_STATE->Usr.Param[1]=*(SP++);
    RVM_STATE->Usr.Param[2]=*(SP++);
    RVM_STATE->Usr.Param[3]=*(SP++);
     
    /* MOV      R1,R0               ;Restore the general purpose registers             
     * ADDS     R0,#16              ;Equivalent of LDMIA R0!,{R4-R11,LR}
     * LDMIA    R0!,{R3-R7}         ;Restore high registers first due to limitation
     * MOV      R8,R3
     * MOV      R9,R4
     * MOV      R10,R5
     * MOV      R11,R6
     * MOV      LR,R7
     * LDMIA    R1!,{R4-R7} */
    RVM_REG->Reg.R4=*(SP++);
    RVM_REG->Reg.R5=*(SP++);
    RVM_REG->Reg.R6=*(SP++);
    RVM_REG->Reg.R7=*(SP++);
    RVM_REG->Reg.R8=*(SP++);
    RVM_REG->Reg.R9=*(SP++);
    RVM_REG->Reg.R10=*(SP++);
    RVM_REG->Reg.R11=*(SP++);
    RVM_REG->Reg.LR=*(SP++);

    /* MSR      PSP, R0 */
    RVM_REG->Reg.SP=(rmp_ptr_t)SP;

    /* BX       LR */
    return;
}
/* End Function:RMP_PendSV_Handler *******************************************/

/* Function:RMP_SysTick_Handler ***********************************************
Description : The SysTick interrupt routine.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_SysTick_Handler(void)
{
    /* PUSH     {LR} */
    /* Note the system that we have entered an interrupt. We are not using tickless here */
    /* MOV      R0, #0x01 */
    /* BL       _RMP_Tick_Handler */
    _RMP_Tim_Handler(1U);
    /* POP      {PC} */
}
/* End Function:RMP_SysTick_Handler ******************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
