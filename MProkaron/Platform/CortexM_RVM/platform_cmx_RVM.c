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
    cnt_t Count;
    /* Clear all VM-related flags and registration tables */
    RMP_Ctxsw=0;
    for(Count=0;Count<RVM_VECT_BITMAP;Count++)
        RMP_Flag.Flags[Count]=0;
    for(Count=0;Count<RVM_MAX_INTVECT;Count++)
        RMP_Vect[Count]=0;
    /* Install systick and pendsv handlers */
    RMP_Vect[0]=(ptr_t)RMP_SysTick_Handler;
    RMP_Vect[1]=(ptr_t)RMP_PendSV_Handler;
    
    /* Clean up the console */
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
        RMP_Console[RMP_Console_Ptr++]=Char;
    }
    else
    {
        RMP_Lock_Sched();
        RMP_Console[RMP_Console_Ptr]='\0';
        RMP_Console_Ptr=0;
        RVM_Hyp_Print();
        RMP_Unlock_Sched();
    }
}
/* End Function:RMP_Putchar **************************************************/

/* Begin Function:RMP_Hypercall ***********************************************
Description : Do a hypercall to the virtual machine.
Input       : ptr_t Number - The hypercall number.
              ptr_t Param1 - The second input.
              ptr_t Param2 - The third input.
              ptr_t Param3 - The fourth ptr_t Param4
Output      : None.
Return      : None.
******************************************************************************/
ret_t RMP_Hypercall(ptr_t Number, ptr_t Param1, ptr_t Param2, ptr_t Param3, ptr_t Param4)
{
    /* Pass the parameters */
    RMP_Param.Number=Number;
    RMP_Param.Param[0]=Param1;
    RMP_Param.Param[1]=Param2;
    RMP_Param.Param[2]=Param3;
    RMP_Param.Param[3]=Param4;
    
    /* Do the hypercall */
    _RMP_Hypercall();
    
    /* Return the result */
    return RMP_Param.Param[0];
}
/* End Function:RMP_Hypercall ************************************************/

/* Begin Function:RMP_Enable_Int **********************************************
Description : Enable interrupts.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Enable_Int(void)
{
    /* Must be successful */
    RMP_ASSERT(RMP_Hypercall(RVM_HYP_ENAINT,0,0,0,0)==0);
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
    /* Must be successful */
    RMP_ASSERT(RMP_Hypercall(RVM_HYP_DISINT,0,0,0,0)==0);
}
/* End Function:RMP_Disable_Int **********************************************/

/* Begin Function:RMP_Hyp_Reg_Evt *********************************************
Description : Register an event channel for the current virtal machine.
Input       : ptr_t Int_Num - The interrupt number for this channel.
              ptr_t VMID - The sender's VMID. Only the VM with this ID is allowed to
                           send to the channel.
Output      : None.
Return      : ret_t - If successful, 0; else an error code.
******************************************************************************/
ret_t RMP_Hyp_Reg_Evt(ptr_t Int_Num, ptr_t VMID)
{
    return RMP_Hypercall(RVM_HYP_REGEVT,Int_Num,VMID,0,0);
}
/* End Function:RMP_Hyp_Reg_Evt **********************************************/

/* Begin Function:RMP_Hyp_Del_Evt *********************************************
Description : Delete an event channel.
Input       : ptr_t Evt_ID - The event channel to delete. The event channel must be
                             created by this VM.
Output      : None.
Return      : ret_t - If successful, 0; else an error code.
******************************************************************************/
ret_t RMP_Hyp_Del_Evt(cnt_t Evt_ID)
{
    return RMP_Hypercall(RVM_HYP_DELEVT,Evt_ID,0,0,0);
}
/* End Function:RMP_Hyp_Del_Evt **********************************************/

/* Begin Function:RVM_Hyp_Wait_Evt ********************************************
Description : Wait for an event. The interrupt must be enabled for this VM.
Input       : None.
Output      : None.
Return      : ret_t - If successful, 0; else an error code.
******************************************************************************/
ret_t RVM_Hyp_Wait_Evt(void)
{
    return RMP_Hypercall(RVM_HYP_WAITEVT,0,0,0,0);
}
/* End Function:RVM_Hyp_Wait_Evt *********************************************/

/* Begin Function:RVM_Hyp_Send_Evt ********************************************
Description : Send an event to the event channel. This VM must have permissions
              to send to that channel.
Input       : ptr_t Evt_ID - The event channel to send to.
Output      : None.
Return      : ret_t - If successful, 0; else an error code.
******************************************************************************/
ret_t RVM_Hyp_Send_Evt(ptr_t Evt_ID)
{
    return RMP_Hypercall(RVM_HYP_SENDEVT,Evt_ID,0,0,0);
}
/* End Function:RVM_Hyp_Send_Evt *********************************************/

/* Begin Function:RVM_Hyp_Query_Evt *******************************************
Description : See if there is a channel in the target VM that this VM can send to.
Input       : None.
Output      : None.
Return      : ret_t - If successful, the channel ID; else an error code.
******************************************************************************/
ret_t RVM_Hyp_Query_Evt(ptr_t VMID)
{
    return RMP_Hypercall(RVM_HYP_QUERYEVT,VMID,0,0,0);
}
/* End Function:RVM_Hyp_Query_Evt ********************************************/

/* Begin Function:RVM_Hyp_Query ***********************************************
Description : See if there is a VM with this name. If there is, return its VM ID.
Input       : s8* Name - The name of the VM.
Output      : None.
Return      : ret_t - If successful, 0; else an error code.
******************************************************************************/
ret_t RVM_Hyp_Query(s8* Name)
{
    cnt_t Count;
    s8* Name_Array;
    
    /* Pass the parameters */
    RMP_Param.Number=RVM_HYP_SENDEVT;
    Name_Array=(s8*)(RMP_Param.Param);
    for(Count=0;Count<16;Count++)
    {
        if(Name[Count]=='\0')
            break;
        Name_Array[Count]=Name[Count];
    }
    
    /* Do the hypercall */
    _RMP_Hypercall();
    
    /* Return the result */
    return RMP_Param.Param[0];
}
/* End Function:RVM_Hyp_Query ************************************************/

/* Begin Function:RVM_Hyp_Tim_Prog ********************************************
Description : See if there is a VM with this name. If there is, return its VM ID.
Input       : ptr_t Period - The new period of timer interrupts.
Output      : None.
Return      : ret_t - If successful, 0; else an error code.
******************************************************************************/
ret_t RVM_Hyp_Tim_Prog(ptr_t Period)
{
    return RMP_Hypercall(RVM_HYP_TIMPROG,Period,0,0,0);
}
/* End Function:RVM_Hyp_Tim_Prog *********************************************/

/* Begin Function:RVM_Hyp_Print ***********************************************
Description : Trigger a print to the console. The print address is predetermined.
Input       : None.
Output      : None.
Return      : ret_t - If successful, 0; else -1.
******************************************************************************/
ret_t RVM_Hyp_Print(void)
{
    return RMP_Hypercall(RVM_HYP_PRINT,0,0,0,0);
}
/* End Function:RVM_Hyp_Print ************************************************/

/* Begin Function:_RMP_Get_Int ************************************************
Description : Get the interrupt number to handle. After returning the vector, clean
              up the corresponding bit.
Input       : None.
Output      : None.
Return      : ret_t - If there is interrupt pending, the interrupt number; else -1.
******************************************************************************/
ret_t _RMP_Get_Int(void)
{
    cnt_t Count;
    cnt_t Pos;
    
    /* See which one is ready, and pick it */
    Pos=-1;
    for(Count=RVM_VECT_BITMAP-1;Count>=0;Count--)
    {
        if(RMP_Flag.Flags[Count]==0)
            continue;
        
        Pos=RMP_MSB_Get(RMP_Flag.Flags[Count]);
        Pos+=(Count<<RMP_WORD_ORDER);
        break;
    }
    
    /* Now kill the bit */
    if(Pos>=0)
    {
        /* See if context switch required */
        if((Pos>1)&&(RMP_Ctxsw!=0))
        {
                RMP_Ctxsw=0;
                Pos=1;
        }
        _RMP_Fetch_And(&RMP_Flag.Flags[Count],~(((ptr_t)1)<<Pos));
    }
    else 
    {
        if(RMP_Ctxsw!=0)
        {
                RMP_Ctxsw=0;
                Pos=1;
        }
    }
    
    return Pos;
}
/* End Function:_RMP_Get_Int *************************************************/

/* Begin Function:_RMP_Int ****************************************************
Description : The interrupt entry of RMP.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Int(void)
{
    cnt_t Int_Num;
    
    while(1)
    {
        _RMP_Int_Rcv();
        /* Look for interrupts to handle from the first */
        Int_Num=_RMP_Get_Int();
        /* Handle the interrupt here - the interrupt is tail-chained */
        while(Int_Num>=0)
        {
            if(RMP_Vect[Int_Num]!=0)
                ((void(*)(void))RMP_Vect[Int_Num])();
            Int_Num=_RMP_Get_Int();
        }
    }
}
/* End Function:_RMP_Int *****************************************************/

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
    SP=(ptr_t*)(RMP_Regs.Reg.SP);    
    *(--SP)=RMP_Regs.Reg.LR;
    *(--SP)=RMP_Regs.Reg.R11;
    *(--SP)=RMP_Regs.Reg.R10;
    *(--SP)=RMP_Regs.Reg.R9;
    *(--SP)=RMP_Regs.Reg.R8;
    *(--SP)=RMP_Regs.Reg.R7;
    *(--SP)=RMP_Regs.Reg.R6;
    *(--SP)=RMP_Regs.Reg.R5;
    *(--SP)=RMP_Regs.Reg.R4;
    
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
    RMP_Regs.Reg.R4=*(SP++);
    RMP_Regs.Reg.R5=*(SP++);
    RMP_Regs.Reg.R6=*(SP++);
    RMP_Regs.Reg.R7=*(SP++);
    RMP_Regs.Reg.R8=*(SP++);
    RMP_Regs.Reg.R9=*(SP++);
    RMP_Regs.Reg.R10=*(SP++);
    RMP_Regs.Reg.R11=*(SP++);
    RMP_Regs.Reg.LR=*(SP++);
    RMP_Regs.Reg.SP=(ptr_t)SP;
                
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
