/******************************************************************************
Filename    : rmp_platform_a7m_rvm.c
Author      : pry
Date        : 09/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for Cortex-M on RVM hypervisor.
******************************************************************************/

/* Includes ******************************************************************/
#define __HDR_DEFS__
#include "Platform/A7M_RVM/rmp_platform_a7m_rvm.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEFS__

#define __HDR_STRUCTS__
#include "Platform/A7M_RVM/rmp_platform_a7m_rvm.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCTS__

/* Private include */
#include "Platform/A7M_RVM/rmp_platform_a7m_rvm.h"

#define __HDR_PUBLIC_MEMBERS__
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC_MEMBERS__

/* The virtual machine configs are here */
#include "rvm_guest_a7m.h"
#include "rvm_guest.h"
/* End Includes **************************************************************/

/* Begin Function:_RMP_Stack_Init *********************************************
Description : Initiate the process stack when trying to start a process. Never
              call this function in user application.
Input       : rmp_ptr_t Entry - The entry address of the thread.
              rmp_ptr_t Stack - The stack address of the thread.
              rmp_ptr_t Arg - The argument to pass to the thread.
Output      : None.
Return      : None.
Other       : When the system stack safe redundancy is set to zero, the stack 
              looks like this when we try to step into the next process by 
              context switch:
                        21  20  19    18 17-14 13  12-5     4      3-0
              HIGH-->  XPSR PC LR(1) R12 R3-R0 LR R11-R4 Number Param[0-3] -->LOW 
              We need to set the stack correctly pretending that we are 
              returning from an systick timer interrupt. Thus, we set the XPSR
              to avoid INVSTATE; set PC to the pseudo-process entrance; set LR
              (1) to 0 because the process does not return to anything; set the 
              R12,R3-R0 to 0; set R11-R4 to 0.
******************************************************************************/
void _RMP_Stack_Init(rmp_ptr_t Entry, rmp_ptr_t Stack, rmp_ptr_t Arg)
{
    /* This is the LR value indicating that we never used the FPU */
    ((rmp_ptr_t*)Stack)[0+8+5]=0xFFFFFFFD;       
    /* Pass the parameter */                            
    ((rmp_ptr_t*)Stack)[0+9+5]=Arg;
    /* Set the process entry */
    ((rmp_ptr_t*)Stack)[6+9+5]=Entry;
    /* For xPSR. Fill the T bit,or an INVSTATE will happen */                          
    ((rmp_ptr_t*)Stack)[7+9+5]=0x01000200;
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
    RVM_Virt_Reg_Timer(RMP_SysTick_Handler);
    RVM_Virt_Reg_Ctxsw(RMP_PendSV_Handler);
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
    RVM_Hyp_Ena_Int();
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

}
/* End Function:RMP_Putchar **************************************************/

/* Begin Function:RMP_Enable_Int **********************************************
Description : Enable interrupts.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Enable_Int(void)
{
    RVM_Hyp_Ena_Int();
}
/* End Function:RMP_Enable_Int ***********************************************/

/* Begin Function:RMP_Disable_Int *********************************************
Description : Disable interrupts.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Disable_Int(void)
{
    RVM_Hyp_Dis_Int();
}
/* End Function:RMP_Disable_Int **********************************************/

/* Begin Function:RMP_Mask_Int ************************************************
Description : Mask interrupts that may do sends.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Mask_Int(void)
{
    RVM_Virt_Mask_Int();
}
/* End Function:RMP_Mask_Int *************************************************/

/* Begin Function:RMP_Unmask_Int **********************************************
Description : Unmask interrupts that may do sends.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unmask_Int(void)
{
    RVM_Virt_Unmask_Int();
}
/* End Function:RMP_Unmask_Int ***********************************************/

/* Begin Function:_RMP_Yield **************************************************
Description : Trigger a yield to a different thread.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Yield(void)
{
    RVM_Virt_Yield();
}
/* End Function:_RMP_Yield ***************************************************/

/* Begin Function:RMP_PendSV_Handler ******************************************
Description : The PendSV interrupt routine.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_PendSV_Handler(void)
{
    rmp_ptr_t* SP;
    /* Spill all the registers onto the user stack
     * MRS       R0,PSP */
    SP=(rmp_ptr_t*)(RVM_REGS->Reg.SP);

    /* Are we using the FPUs at all? If yes, push FPU registers onto stack */
    /* TST                 LR,#0x10            ;Are we using the FPU or not at all?
     * DCI                 0xBF08              ;IT EQ ;If yes, (DCI for compatibility with no FPU support)
     * DCI                 0xED20              ;VSTMDBEQ R0!,{S16-S31}
     * DCI                 0x8A10              ;Save FPU registers not saved by lazy stacking. */
    if((RVM_REGS->Reg.LR&0x10)==0)
    {
        *(--SP)=RVM_REGS->Cop_Reg.S31;
        *(--SP)=RVM_REGS->Cop_Reg.S30;
        *(--SP)=RVM_REGS->Cop_Reg.S29;
        *(--SP)=RVM_REGS->Cop_Reg.S28;
        *(--SP)=RVM_REGS->Cop_Reg.S27;
        *(--SP)=RVM_REGS->Cop_Reg.S26;
        *(--SP)=RVM_REGS->Cop_Reg.S25;
        *(--SP)=RVM_REGS->Cop_Reg.S24;
        *(--SP)=RVM_REGS->Cop_Reg.S23;
        *(--SP)=RVM_REGS->Cop_Reg.S22;
        *(--SP)=RVM_REGS->Cop_Reg.S21;
        *(--SP)=RVM_REGS->Cop_Reg.S20;
        *(--SP)=RVM_REGS->Cop_Reg.S19;
        *(--SP)=RVM_REGS->Cop_Reg.S18;
        *(--SP)=RVM_REGS->Cop_Reg.S17;
        *(--SP)=RVM_REGS->Cop_Reg.S16;
    }

    /* STMDB     R0!,{R4-R11,LR} */
    *(--SP)=RVM_REGS->Reg.LR;
    *(--SP)=RVM_REGS->Reg.R11;
    *(--SP)=RVM_REGS->Reg.R10;
    *(--SP)=RVM_REGS->Reg.R9;
    *(--SP)=RVM_REGS->Reg.R8;
    *(--SP)=RVM_REGS->Reg.R7;
    *(--SP)=RVM_REGS->Reg.R6;
    *(--SP)=RVM_REGS->Reg.R5;
    *(--SP)=RVM_REGS->Reg.R4;

    /* Spill all the user-accessible hypercall structure to stack */
    *(--SP)=RVM_PARAM->User.Number;
    *(--SP)=RVM_PARAM->User.Param[0];
    *(--SP)=RVM_PARAM->User.Param[1];
    *(--SP)=RVM_PARAM->User.Param[2];
    *(--SP)=RVM_PARAM->User.Param[3];

    /* Save extra context
     * BL       RMP_Save_Ctx */
    RMP_Save_Ctx();
    
    /* Save the SP to control block
     * LDR       R1,=RMP_Cur_SP
     * STR       R0,[R1] */
    RMP_Cur_SP=(rmp_ptr_t)SP;
                
    /* Get the highest ready task
     * BL        _RMP_Get_High_Rdy */
    _RMP_Get_High_Rdy();
    
    /* Load the SP
     * LDR       R1,=RMP_Cur_SP
     * LDR       R0,[R1] */
    SP=(rmp_ptr_t*)RMP_Cur_SP;
    
    /* Load extra context
     * BL        RMP_Load_Ctx */
    RMP_Load_Ctx();

    /* Load the user-accessible hypercall structure to stack */
    RVM_PARAM->User.Param[3]=*(SP++);
    RVM_PARAM->User.Param[2]=*(SP++);
    RVM_PARAM->User.Param[1]=*(SP++);
    RVM_PARAM->User.Param[0]=*(SP++);
    RVM_PARAM->User.Number=*(SP++);
     
    /* Load registers from user stack
     * LDMIA     R0!,{R4-R11,LR}
     * MSR       PSP,R0 */
    RVM_REGS->Reg.R4=*(SP++);
    RVM_REGS->Reg.R5=*(SP++);
    RVM_REGS->Reg.R6=*(SP++);
    RVM_REGS->Reg.R7=*(SP++);
    RVM_REGS->Reg.R8=*(SP++);
    RVM_REGS->Reg.R9=*(SP++);
    RVM_REGS->Reg.R10=*(SP++);
    RVM_REGS->Reg.R11=*(SP++);
    RVM_REGS->Reg.LR=*(SP++);
                
    /* If we use FPU, restore FPU context */
    /* TST                 LR,#0x10            ;Are we using the FPU or not at all?
     * DCI                 0xBF08              ;IT EQ ;If yes, (DCI for compatibility with no FPU support)
     * DCI                 0xECB0              ;VLDMIAEQ R0!,{S16-S31}
     * DCI                 0x8A10              ;Load FPU registers not loaded by lazy stacking. */
    if((RVM_REGS->Reg.LR&0x10)==0)
    {
        RVM_REGS->Cop_Reg.S16=*(SP++);
        RVM_REGS->Cop_Reg.S17=*(SP++);
        RVM_REGS->Cop_Reg.S18=*(SP++);
        RVM_REGS->Cop_Reg.S19=*(SP++);
        RVM_REGS->Cop_Reg.S20=*(SP++);
        RVM_REGS->Cop_Reg.S21=*(SP++);
        RVM_REGS->Cop_Reg.S22=*(SP++);
        RVM_REGS->Cop_Reg.S23=*(SP++);
        RVM_REGS->Cop_Reg.S24=*(SP++);
        RVM_REGS->Cop_Reg.S25=*(SP++);
        RVM_REGS->Cop_Reg.S26=*(SP++);
        RVM_REGS->Cop_Reg.S27=*(SP++);
        RVM_REGS->Cop_Reg.S28=*(SP++);
        RVM_REGS->Cop_Reg.S29=*(SP++);
        RVM_REGS->Cop_Reg.S30=*(SP++);
        RVM_REGS->Cop_Reg.S31=*(SP++);
    }

    RVM_REGS->Reg.SP=(rmp_ptr_t)SP;

    /* Return from interrupt */
    /* BX        LR */
    return;
}
/* End Function:RMP_PendSV_Handler *******************************************/

/* Begin Function:RMP_SysTick_Handler *****************************************
Description : The SysTick interrupt routine.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_SysTick_Handler(void)
{
    /* PUSH      {LR} */
    /* Note the system that we have entered an interrupt. We are not using tickless here */
    /* MOV       R0,#0x01 */
    /* BL        _RMP_Tick_Handler */
    _RMP_Tick_Handler(1);
    /* POP       {PC} */
}
/* End Function:RMP_SysTick_Handler ******************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
