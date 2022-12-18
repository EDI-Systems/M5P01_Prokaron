/******************************************************************************
Filename    : rmp_platform_x86_linux.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for x86 Linux port.
******************************************************************************/

/* Includes ******************************************************************/
#define __HDR_DEFS__
#include "Platform/X86_LINUX/rmp_platform_x86_linux.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEFS__

#define __HDR_STRUCTS__
#include "Platform/X86_LINUX/rmp_platform_x86_linux.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCTS__

/* Private include */
#include "Platform/X86_LINUX/rmp_platform_x86_linux.h"

#define __HDR_PUBLIC_MEMBERS__
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC_MEMBERS__
/* End Includes **************************************************************/

/* Begin Function:RMP_Int_Disable *********************************************
Description    : The function for disabling all interrupts. Does not allow nesting.
Input          : None.
Output         : None.
Register Usage : None.
******************************************************************************/
void RMP_Int_Disable(void)
{
	RMP_Int_Disabled=1;
}
/* End Function:RMP_Int_Disable **********************************************/

/* Begin Function:RMP_Int_Enable **********************************************
Description    : The function for enabling all interrupts. Does not allow nesting.
Input          : None.
Output         : None.
Register Usage : None.
******************************************************************************/
void RMP_Int_Enable(void)
{
	RMP_Int_Disabled=0;
}
/* End Function:RMP_Int_Enable ***********************************************/

/* Begin Function:RMP_MSB_Get *************************************************
Description : Get the MSB of the word.
Input       : rmp_ptr_t Val - The value.
Output      : None.
Return      : rmp_ptr_t - The MSB position.
******************************************************************************/
/* 2*i and 2*i+1 will correspond to the same slot in this table */
const rmp_u8_t RMP_MSB_Tbl[128]=
{
	/* 0-1 */
	0x00,
	/* 2-3 */
	0x01,
	/* 4-7 */
	0x02,0x02,
	/* 8-15 */
	0x03,0x03,0x03,0x03,
	/* 16-31 */
	0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,
	/* 32-63 */
	0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,
	/* 64-127 */
	0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,
	0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,
	/* 128-255 */
	0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,
	0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,
	0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,
	0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07
};

rmp_ptr_t RMP_MSB_Get(rmp_ptr_t Val)
{
	/* Scan from high bits to low bits */
	if((Val&0xFF000000)!=0)
		return RMP_MSB_Tbl[Val>>25]+24;
	else if((Val&0xFF0000)!=0)
		return RMP_MSB_Tbl[Val>>17]+16;
	else if((Val&0xFF00)!=0)
		return RMP_MSB_Tbl[Val>>9]+8;
	else if((Val&0xFF)!=0)
		return RMP_MSB_Tbl[Val>>1];

	/* Nothing anywhere */
	return 0xFFFFFFFF;
}
/* End Function:RMP_MSB_Get **************************************************/

/* Begin Function:_RMP_Yield **************************************************
Description : Trigger a yield to another thread.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Yield(void)
{
	RMP_PendSV_Flag=1;

	if(RMP_Int_Disabled==0)
		RMP_ASSERT(kill(RMP_User_PID,SIGUSR1)>=0);
}
/* End Function:_RMP_Yield ***************************************************/

/* Begin Function:_RMP_Start **************************************************
Description : Jump to the user function and will never return from it.
Input       : None.
Output      : None.
Return       : None.
******************************************************************************/
/* The signal processing function for invoking the timer interrupts - this will happen per 1 sec */
void SigAlrm_Handler(int Param)
{
	RMP_SysTick_Flag=1;

	if(RMP_Int_Disabled==0)
		kill(RMP_User_PID,SIGUSR1);
}

void _RMP_Start(rmp_ptr_t Entry, rmp_ptr_t Stack)
{
	int Status;
	struct itimerval Tick;

	/* Set up the timer */
	memset(&Tick, 0, sizeof(Tick));
	/* First timeout */
	Tick.it_value.tv_sec=1;
	Tick.it_value.tv_usec=0;
	/* Interval time to run function */
	Tick.it_interval.tv_sec=1;
	Tick.it_interval.tv_usec=0;
	RMP_ASSERT(signal(SIGALRM, SigAlrm_Handler)>=0);
	RMP_ASSERT(setitimer(ITIMER_REAL, &Tick, NULL)>=0);

    printf("The test on interrupt response time can take up to 5 minutes. Please wait patiently.\n");
    printf("The performance number printed is not accurate. If you see 6-figure numbers, run again.\n");
    printf("Additionally, the interrupt test may fail, because the Linux scheduler is not\n");
    printf("guaranteed to pick the user thread after the system thread have finished its\n");
    printf("signal handling. If another signal comes again before the user thread gets a\n");
    printf("chance to run, then it will fail because the mailbox is not empty yet. Should\n");
    printf("this happen on your computer(due to performance issues), increase the macro\n");
    printf("TEST_INT_INTERVAL's (in test_X86_LINUX.h) value. However this macro's value\n");
    printf("have a linear relationship with interrupt response test runtime, so configure it\n");
    printf("according to your needs. The default value 10000 corresponds to 200 seconds.\n");
    printf("This test will continue to run if not killed manually (mimicking the MCUs).\n");
    printf("Remember to kill it after its completion!\n\n");


	RMP_Sys_PID=syscall(SYS_gettid);
	RMP_User_PID=clone((int (*)(void*))(Entry), (void*)Stack, CLONE_VM|SIGCHLD, 0);
    printf("Sys PID is %d, User PID is %d.\n\n",RMP_Sys_PID,RMP_User_PID);

    while(1)
    {
    	/* Wait for the thread to receive the signal */
        RMP_ASSERT(wait(&Status)>0);
        if(RMP_SysTick_Flag!=0)
        {
        	RMP_SysTick_Flag=0;
        	SysTick_Handler();
        }
        else if(RMP_PendSV_Flag!=0)
        {
        	RMP_PendSV_Flag=0;
        	PendSV_Handler();
        }
        /* Must be an external interrupt */
        else
        {
        	if(RMP_Eint_Handler!=0)
        		RMP_Eint_Handler();
        }
       	/* Resume execution */
        RMP_ASSERT(ptrace(PTRACE_CONT, RMP_User_PID,0,0)>=0);
    }
}
/* End Function:_RMP_Start ***************************************************/

/* Begin Function:PendSV_Handler **********************************************
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
	RMP_ASSERT(ptrace(PTRACE_GETREGS, RMP_User_PID, NULL, &Regs)>=0);

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

	RMP_Ctx_Save();
	RMP_SP_Cur=(rmp_ptr_t)SP;
	_RMP_Rdy_High();
	SP=(rmp_ptr_t*)RMP_SP_Cur;
	RMP_Ctx_Load();

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

	RMP_ASSERT(ptrace(PTRACE_SETREGS, RMP_User_PID, NULL, &Regs)>=0);
}
/* End Function:PendSV_Handler ***********************************************/

/* Begin Function:SysTick_Handler *********************************************
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
	_RMP_Tick_Handler(1);
}
/* End Function:SysTick_Handler **********************************************/

/* Begin Function:_RMP_Stack_Init *********************************************
Description : Initiate the process stack when trying to start a process. Never
              call this function in user application.
Input       : rmp_ptr_t Entry - The entry of the thread.
              rmp_ptr_t Stack - The stack address of the thread.
              rmp_ptr_t Arg - The argument to pass to the thread.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Stack_Init(rmp_ptr_t Entry, rmp_ptr_t Stack, rmp_ptr_t Arg)
{
	rmp_ptr_t* Stack_Addr;

	Stack_Addr=(rmp_ptr_t*)Stack;

	Stack_Addr[0]=0x0B0B0B0B;                                        /* EBX */
	Stack_Addr[1]=0x0C0C0C0C;                                        /* ECX */
	Stack_Addr[2]=0x0D0D0D0D;                                        /* EDX */
	Stack_Addr[3]=0x51515151;                                        /* ESI */
	Stack_Addr[4]=0xD1D1D1D1;                                        /* EDI */
	Stack_Addr[5]=0x69696969;                                        /* EBP */
	Stack_Addr[6]=Arg;                                               /* EAX */
	/* ptrace requires the last 2 bits of segment registers to be 1; see kernel source */
	Stack_Addr[7]=0x2B;                                              /* XDS */
	Stack_Addr[8]=0x2B;                                              /* XES */
	Stack_Addr[9]=0x2B;                                              /* XFS */
	Stack_Addr[10]=0x63;                                             /* XGS */
	Stack_Addr[11]=Arg;                                              /* ORIG_EAX */
	/* Always need to +2, kernel bug */
	Stack_Addr[12]=Entry;                                            /* EIP */
	Stack_Addr[13]=0x23;                                             /* ECS */
	Stack_Addr[14]=0x202;                                            /* EFLAGS */
	Stack_Addr[15]=0x2B;                                             /* XSS */
	Stack_Addr[16]=Arg;                                              /* Param */
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
    RMP_SysTick_Flag=0;
    RMP_PendSV_Flag=0;
    RMP_Int_Disabled=1;

    RMP_Int_Disable();
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
	/* Let the parent trace me */
	RMP_ASSERT(ptrace(PTRACE_TRACEME)>=0);

    RMP_Int_Enable();
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
    RMP_POSIX_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
