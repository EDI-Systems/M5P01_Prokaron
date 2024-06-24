/******************************************************************************
Filename    : rmp_platform_x86_linux.c
Author      : pry wyh
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for x86 Linux port.
******************************************************************************/

/* Include *******************************************************************/
#define __HDR_DEF__
#include "Platform/X86_LINUX/rmp_platform_x86_linux.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "Platform/X86_LINUX/rmp_platform_x86_linux.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Platform/X86_LINUX/rmp_platform_x86_linux.h"

#define __HDR_PUBLIC__
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC__
/* End Include ***************************************************************/

/* Function:_RMP_Lowlvl_Init ***************************************************
Description    : Initialize the low level hardware of the system.
Input          : None.
Output         : None.
Register Usage : None.
******************************************************************************/
void _RMP_Lowlvl_Init(void)
{
    RMP_SysTick_Flag=0U;
    RMP_PendSV_Flag=0U;
    RMP_Int_Disabled=1U;

    RMP_Int_Disable();
}
/* End Function:_RMP_Lowlvl_Init **********************************************/

/* Function:RMP_Int_Disable ***************************************************
Description    : The function for disabling all interrupts. Does not allow nesting.
Input          : None.
Output         : None.
Register Usage : None.
******************************************************************************/
void RMP_Int_Disable(void)
{
    RMP_Int_Disabled=1U;
}
/* End Function:RMP_Int_Disable **********************************************/

/* Function:RMP_Int_Enable ****************************************************
Description    : The function for enabling all interrupts. Does not allow nesting.
Input          : None.
Output         : None.
Register Usage : None.
******************************************************************************/
void RMP_Int_Enable(void)
{
    RMP_Int_Disabled=0U;
}
/* End Function:RMP_Int_Enable ***********************************************/

/* Function:_RMP_X86_LINUX_Yield **********************************************
Description : Trigger a yield to another thread.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_X86_LINUX_Yield(void)
{
    RMP_PendSV_Flag=1U;

    if(RMP_Int_Disabled==0U)
        RMP_ASSERT(kill(RMP_User_PID,SIGUSR1)>=0);
}
/* End Function:_RMP_X86_LINUX_Yield *****************************************/

/* Function:_RMP_Start ********************************************************
Description : Jump to the user function and will never return from it.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
/* The timer interrupt signal processing - this will happen every second */
void SigAlrm_Handler(int Param)
{
    RMP_SysTick_Flag=1U;

    if(RMP_Int_Disabled==0U)
        kill(RMP_User_PID,SIGUSR1);
}

void _RMP_Start(rmp_ptr_t Entry, rmp_ptr_t Stack)
{
    int Status;
    struct itimerval Tick;

    /* Set up the timer */
    memset(&Tick,0U,sizeof(Tick));
    /* First timeout */
    Tick.it_value.tv_sec=1U;
    Tick.it_value.tv_usec=0U;
    /* Interval time to run function */
    Tick.it_interval.tv_sec=1U;
    Tick.it_interval.tv_usec=0U;
    RMP_ASSERT(signal(SIGALRM,SigAlrm_Handler)>=0);
    RMP_ASSERT(setitimer(ITIMER_REAL,&Tick,NULL)>=0);

    printf("The test on interrupt latency can take up to 5 minutes depending on\n"
           "your setup. The performance number printed is not accurate (usually\n"
           "only the minumum matters). Additionally, the interrupt test may fail,\n"
           "cause the Linux scheduler is not guaranteed to pick the user thread\n"
           "after the system thread have finished its signal handling. If another\n"
           "signal comes again before the user thread gets a chance to run, then\n"
           "it fails because the mailbox, etc. is not empty yet. Should this\n"
           "happen on your computer(due to performance or virtualization issues),\n"
           "increase the value of TEST_INT_INTERVAL macro(in test_x86_linux.h).\n"
           "However, interrupt latency test runtime scales linearly with this.\n");

    /* Fork user thread */
    RMP_Sys_PID=syscall(SYS_gettid);
    RMP_User_PID=clone((int (*)(void*))(Entry),(void*)Stack,CLONE_VM|SIGCHLD,0);
    printf("\nSys PID is %d, User PID is %d.\n",RMP_Sys_PID,RMP_User_PID);

    /* If needed, set priority - may help you in some cases
     * struct sched_param Param;
     * Param.sched_priority=sched_get_priority_max(SCHED_FIFO);
     * RMP_ASSERT(Param.sched_priority>=0);
     * RMP_ASSERT(sched_setscheduler(RMP_Sys_PID,SCHED_FIFO,&Param)>=0);
     * RMP_ASSERT(sched_setscheduler(RMP_User_PID,SCHED_FIFO,&Param)>=0); */
    
    while(1)
    {
        /* Wait for the thread to receive the signal */
        RMP_ASSERT(wait(&Status)>0);
        
        /* Handle system interrupts */
        if(RMP_SysTick_Flag!=0U)
        {
            RMP_SysTick_Flag=0U;
            SysTick_Handler();
        }
        else if(RMP_PendSV_Flag!=0U)
        {
            RMP_PendSV_Flag=0U;
            PendSV_Handler();
        }
        /* Must be an external interrupt */
        else
        {
            if(RMP_Eint_Handler!=0U)
                RMP_Eint_Handler();
        }
        
        /* Resume execution */
        RMP_ASSERT(ptrace(PTRACE_CONT,RMP_User_PID,0,0)>=0);
    }
}
/* End Function:_RMP_Start ***************************************************/

/* Function:PendSV_Handler ****************************************************
Description : The PendSV interrupt routine. In fact, it will call a C function
              directly. The reason why the interrupt routine must be an assembly
              function is that the compiler may deal with the stack in a different
              way when different optimization level is chosen. An assembly function
              can make way around this problem.
              However, if your compiler support inline assembly functions, this
              can also be written in C.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void PendSV_Handler(void)
{
    rmp_ptr_t* SP;
    struct pt_regs Regs;

    /* Read the register contents and push to stack */
    RMP_ASSERT(ptrace(PTRACE_GETREGS,RMP_User_PID,NULL,&Regs)>=0);

    /* Is it in the kernel? if yes... well, we gonna skip it this time until it exits */
    /* Now push everything to its stack */
    SP=(rmp_ptr_t*)(Regs.esp);
    *(--SP)=Regs.xss;
    *(--SP)=Regs.eflags;
    *(--SP)=Regs.xcs;
    *(--SP)=Regs.eip;
    *(--SP)=Regs.orig_eax;
    *(--SP)=Regs.xgs;
    *(--SP)=Regs.xfs;
    *(--SP)=Regs.xes;
    *(--SP)=Regs.xds;
    *(--SP)=Regs.eax;
    *(--SP)=Regs.ebp;
    *(--SP)=Regs.edi;
    *(--SP)=Regs.esi;
    *(--SP)=Regs.edx;
    *(--SP)=Regs.ecx;
    *(--SP)=Regs.ebx;

    /* Context saved to stack */
    RMP_SP_Cur=(rmp_ptr_t)SP;
    _RMP_Run_High();
    SP=(rmp_ptr_t*)RMP_SP_Cur;
    
    /* Reload register contents from stack */
    Regs.ebx=*(SP++);
    Regs.ecx=*(SP++);
    Regs.edx=*(SP++);
    Regs.esi=*(SP++);
    Regs.edi=*(SP++);
    Regs.ebp=*(SP++);
    Regs.eax=*(SP++);
    Regs.xds=*(SP++);
    Regs.xes=*(SP++);
    Regs.xfs=*(SP++);
    Regs.xgs=*(SP++);
    Regs.orig_eax=*(SP++);
    Regs.eip=*(SP++);
    Regs.xcs=*(SP++);
    Regs.eflags=*(SP++);
    Regs.xss=*(SP++);
    Regs.esp=(rmp_ptr_t)SP;

    RMP_ASSERT(ptrace(PTRACE_SETREGS,RMP_User_PID,NULL,&Regs)>=0);
}
/* End Function:PendSV_Handler ***********************************************/

/* Function:SysTick_Handler ***************************************************
Description : The SysTick interrupt routine. In fact, it will call a C function
              directly. The reason why the interrupt routine must be an assembly
              function is that the compiler may deal with the stack in a different
              way when different optimization level is chosen. An assembly function
              can make way around this problem.
              However, if your compiler support inline assembly functions, this
              can also be written in C.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void SysTick_Handler(void)
{
    _RMP_Tim_Handler(1U);
}
/* End Function:SysTick_Handler **********************************************/

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
    struct RMP_X86_LINUX_Stack* Ctx;

    Ptr=RMP_STACK_PTR(Stack,Size);
    Ctx=RMP_STACK_CTX(Ptr);

    Ctx->REG_EBX=0x0B0B0B0BU;
    Ctx->REG_ECX=0x0C0C0C0CU;
    Ctx->REG_EDX=0x0D0D0D0DU;
    Ctx->REG_ESI=0x51515151U;
    Ctx->REG_EDI=0xD1D1D1D1U;
    Ctx->REG_EBP=0x69696969U;
    Ctx->REG_EAX=Param;
    /* ptrace requires the last 2 bits of segment registers to be 1; see kernel source */
    Ctx->REG_XDS=0x2BU;
    Ctx->REG_XES=0x2BU;
    Ctx->REG_XFS=0x2BU;
    Ctx->REG_XGS=0x63U;
    Ctx->REG_ORIG_EAX=Param;
    /* Always need to +2, kernel bug */
    Ctx->REG_EIP=Entry;
    Ctx->REG_ECS=0x23U;
    Ctx->REG_EFLAGS=0x202U;
    Ctx->REG_XSS=0x2BU;
    Ctx->REG_Param=Param;

    return Ptr;
}
/* End Function:_RMP_Stack_Init **********************************************/

/* Function:_RMP_Plat_Hook ****************************************************
Description : Platform-specific hook for system initialization.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Plat_Hook(void)
{
    /* Let the parent trace me */
    RMP_ASSERT(ptrace(PTRACE_TRACEME)>=0);
    
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
    RMP_POSIX_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
