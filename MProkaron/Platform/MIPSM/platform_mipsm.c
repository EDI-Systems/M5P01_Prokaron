/******************************************************************************
Filename    : platform_mipsm.c
Author      : pry
Date        : 04/02/2018
Licence     : LGPL v3+; see COPYING for details.
Description : The platform specific file for MIPS M-class. 
              Currently M4k and M14kF is supported.
******************************************************************************/

/* Includes ******************************************************************/
#define __HDR_DEFS__
#include "Platform/MIPSM/platform_mipsm.h"
#include "Kernel/kernel.h"
#undef __HDR_DEFS__

#define __HDR_STRUCTS__
#include "Platform/MIPSM/platform_mipsm.h"
#include "Kernel/kernel.h"
#undef __HDR_STRUCTS__

/* Private include */
#include "Platform/MIPSM/platform_mipsm.h"

#define __HDR_PUBLIC_MEMBERS__
#include "Kernel/kernel.h"
#undef __HDR_PUBLIC_MEMBERS__
/* End Includes **************************************************************/

/* Begin Function:_RMP_Stack_Init *********************************************
Description : Initiate the process stack when trying to start a process. Never
              call this function in user application.
Input       : ptr_t Entry - The entry of the thread.
              ptr_t Stack - The stack address of the thread.
              ptr_t Arg - The argument to pass to the thread.
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
    RMP_MIPSM_LOW_LEVEL_INIT();
    
    /* Core Timer Interrupt _CORE_TIMER_VECTOR 0 OFF000<17:1> IFS0<0> IEC0<0> IPC0<4:2> IPC0<1:0> No
     * Core Software Interrupt 0 _CORE_SOFTWARE_0_VECTOR 1 OFF001<17:1> IFS0<1> IEC0<1> IPC0<12:10> IPC0<9:8> No */
    /* Clear the software interrupt flags */
	IFS0CLR=_IFS0_CTIF_MASK|_IFS0_CS0IF_MASK;
    
	/* Set both interrupt priority - priority 1, subpriority 3, lowest allowed */
	IPC0CLR=_IPC0_CTIP_MASK|_IPC0_CTIS_MASK|
            _IPC0_CS0IP_MASK|_IPC0_CS0IS_MASK;
	IPC0SET=(1<<_IPC0_CTIP_POSITION)|(0<<_IPC0_CTIS_POSITION)|
            (1<<_IPC0_CS0IP_POSITION)|(0<<_IPC0_CS0IS_POSITION);

	IEC0CLR=_IEC0_CTIE_POSITION|_IEC0_CS0IE_MASK;
	IEC0SET=(1<<_IEC0_CTIE_POSITION)|(1<<_IEC0_CS0IE_POSITION);
    
    /* Set the timer timeout value */
    _RMP_Set_Timer(RMP_MIPSM_TICK_VAL/2);
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
    RMP_MIPSM_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/