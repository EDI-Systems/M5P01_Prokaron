/******************************************************************************
Filename    : platform_cmx_RVM.c
Author      : pry
Date        : 09/02/2018
Licence     : LGPL v3+; see COPYING for details.
Description : The platform specific file for M7M1.
******************************************************************************/

/* Includes ******************************************************************/
#define __HDR_DEFS__
#include "Platform/CortexM_RVM/platform_cmx_RVM.h"
#include "Kernel/kernel.h"
#undef __HDR_DEFS__

#define __HDR_STRUCTS__
#include "Platform/CortexM_RVM/platform_cmx_RVM.h"
#include "Kernel/kernel.h"
#undef __HDR_STRUCTS__

/* Private include */
#include "Platform/CortexM_RVM/platform_cmx_RVM.h"

#define __HDR_PUBLIC_MEMBERS__
#include "Kernel/kernel.h"
#undef __HDR_PUBLIC_MEMBERS__
/* End Includes **************************************************************/

/* Begin Function:_RMP_Stack_Init *********************************************
Description : Initiate the process stack when trying to start a process. Never
              call this function in user application.
Input       : pid_t PID - The PID of the process.
Output      : None.
Return      : None.
Other       : When the system stack safe redundancy is set to zero, the stack 
              looks like this when we try to step into the next process by 
              context switch:
              HIGH-->  XPSR PC LR(1) R12 R3-R0 LR R11-R4 -->LOW 
              We need to set the stack correctly pretending that we are 
              returning from an systick timer interrupt. Thus, we set the XPSR
              to avoid INVSTATE; set PC to the pseudo-process entrance; set LR
              (1) to 0 because the process does not return to anything; set the 
              R12,R3-R0 to 0; set R11-R4 to 0.
******************************************************************************/
void _RMP_Stack_Init(ptr_t Entry, ptr_t Stack, ptr_t Arg)
{
    /* The "9" here is because we also pushed other registers to PSP */
    /* This is the LR value indicating that we never used the FPU */
    ((ptr_t*)Stack)[0+8]=0xFFFFFFFD;       
    /* CM3:Pass the parameter */                            
    ((ptr_t*)Stack)[0+9]=Arg;       
    /* CM3:for xPSR. fill the T bit,or an INVSTATE will happen */
    ((ptr_t*)Stack)[6+9]=Entry;
    /* CM3:Set the process entrance */                            
    ((ptr_t*)Stack)[7+9]=0x01000200;      
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
    RVM_Init();
    RVM_Handler_Install(0, RMP_SysTick_Handler);
    RVM_Handler_Install(1, RMP_PendSV_Handler);
    RMP_Console_Ptr=0;
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
    /* If we are not changing lines, and buffer not full */
    if((Char!='\r')&&(Char!='\n')&&(Char!='\0')&&(RMP_Console_Ptr<RMP_KERNEL_DEBUG_MAX_STR-1))
    {
        RVM_Console[RMP_Console_Ptr++]=Char;
    }
    else
    {
        RMP_Lock_Sched();
        RVM_Console[RMP_Console_Ptr]='\0';
        RMP_Console_Ptr=0;
        RVM_Print();
        RMP_Unlock_Sched();
    }
}
/* End Function:RMP_Putchar **************************************************/

void RMP_Disable_Int(void)
{
    RVM_Disable_Int();
}

void RMP_Enable_Int(void)
{
    RVM_Enable_Int();
}

void _RMP_Yield(void)
{
    RVM_Yield();
}

/* Begin Function:RMP_PendSV_Handler ******************************************
Description : The PendSV interrupt routine.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_PendSV_Handler(void)
{
    ptr_t* SP;
    
    /* Spill all the registers onto the user stack
     * MRS       R0,PSP
     * STMDB     R0!,{R4-R11,LR} */
    SP=(ptr_t*)(RVM_Regs.Reg.SP);    
    *(--SP)=RVM_Regs.Reg.LR;
    *(--SP)=RVM_Regs.Reg.R11;
    *(--SP)=RVM_Regs.Reg.R10;
    *(--SP)=RVM_Regs.Reg.R9;
    *(--SP)=RVM_Regs.Reg.R8;
    *(--SP)=RVM_Regs.Reg.R7;
    *(--SP)=RVM_Regs.Reg.R6;
    *(--SP)=RVM_Regs.Reg.R5;
    *(--SP)=RVM_Regs.Reg.R4;
    
    /* Save extra context
     * BL       RMP_Save_Ctx */
    RMP_Save_Ctx();
    
    /* Save the SP to control block
     * LDR       R1,=RMP_Cur_SP
     * STR       R0,[R1] */
    RMP_Cur_SP=(ptr_t)SP;
                
    /* Get the highest ready task
     * BL        _RMP_Get_High_Rdy */
    _RMP_Get_High_Rdy();
    
    /* Load the SP
     * LDR       R1,=RMP_Cur_SP
     * LDR       R0,[R1] */
    SP=(ptr_t*)RMP_Cur_SP;
    
    /* Load extra context
     * BL        RMP_Load_Ctx */
    RMP_Load_Ctx();
     
    /* Load registers from user stack
     * LDMIA     R0!,{R4-R11,LR}
     * MSR       PSP,R0 */
    RVM_Regs.Reg.R4=*(SP++);
    RVM_Regs.Reg.R5=*(SP++);
    RVM_Regs.Reg.R6=*(SP++);
    RVM_Regs.Reg.R7=*(SP++);
    RVM_Regs.Reg.R8=*(SP++);
    RVM_Regs.Reg.R9=*(SP++);
    RVM_Regs.Reg.R10=*(SP++);
    RVM_Regs.Reg.R11=*(SP++);
    RVM_Regs.Reg.LR=*(SP++);
    RVM_Regs.Reg.SP=(ptr_t)SP;
                
    /* Here the LR will indicate whether we are using FPU
     * BX        LR */
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
