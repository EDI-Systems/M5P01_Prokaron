/******************************************************************************
Filename    : rmp_platform_rv32imac.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for RV32IMAC (FE310). This port is for
              experimental purposes only and not fit for production right now.
              The real-time performance of this port is not great due to intermittent
              instruction cache misses. This can cause a huge penalty (~40000) cycles
              if there is one. You will observe the effect of this when measuring the
              number of instruction executed (minstret) vs. machine cycles (mcycle) in
              a slightly larger loop. By the way, the RAM/ROM ratio of FE310 is
              completely ridiculous, about 16kB vs. 16MB, 1:1000.
              This chip also contains various bugs that make it unfit for production
              as well, such as SPI, AON, etc. We need more serious future versions.
******************************************************************************/

/* Includes ******************************************************************/
#define __HDR_DEFS__
#include "Platform/RV32IMAC/rmp_platform_rv32imac.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEFS__

#define __HDR_STRUCTS__
#include "Platform/RV32IMAC/rmp_platform_rv32imac.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCTS__

/* Private include */
#include "Platform/RV32IMAC/rmp_platform_rv32imac.h"

#define __HDR_PUBLIC_MEMBERS__
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC_MEMBERS__
/* End Includes **************************************************************/

/* Entry *********************************************************************/
/* This portion is completely GCC specific - if you need to move this to other
 * compilers, this needs to be changed. The linker script should also be changed
 * as well. */
/* Address of the data section */
extern rmp_ptr_t  __data_load_addr__;
/* Begin address for the .data section */
extern rmp_ptr_t __data_begin__;
/* End address for the .data section */
extern rmp_ptr_t __data_end__;
/* Begin address for the .bss section */
extern rmp_ptr_t __bss_begin__;
/* End address for the .bss section */
extern rmp_ptr_t __bss_end__;
/* Global pointer address */
extern rmp_ptr_t __global_pointer$;
/* The entry address */
extern int main(void);
void _start(void);
void _start(void)
{
    rmp_u8_t* Src;
    rmp_u8_t* Dst;

    Src=(rmp_u8_t*)&__data_load_addr__;
    Dst=(rmp_u8_t*)&__data_begin__;
    while((rmp_ptr_t)Dst<(rmp_ptr_t)&__data_end__)
        *Dst++=*Src++;

    Dst=(rmp_u8_t*)&__bss_begin__;
    while((rmp_ptr_t)Dst<(rmp_ptr_t)&__bss_end__)
        *Dst++=0;

    /* Call main function */
    main();
}
/* End Entry *****************************************************************/

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
    rmp_ptr_t* Stack_Ptr;

    Stack_Ptr=(rmp_ptr_t*)Stack;

    /* This is where PC is saved */
    Stack_Ptr[0]=Entry;
    Stack_Ptr[1]=0x01010101;
    /* We always initialize the mstatus register to this */
    Stack_Ptr[2]=0x1880;
    /* We always initialize GP to a known value. If the thread modifies this later, it is fine */
    Stack_Ptr[3]=(rmp_ptr_t)(&__global_pointer$);
    Stack_Ptr[4]=0x04040404;
    Stack_Ptr[5]=0x05050505;
    Stack_Ptr[6]=0x06060606;
    Stack_Ptr[7]=0x07070707;
    Stack_Ptr[8]=0x08080808;
    Stack_Ptr[9]=0x09090909;
    /* x10 for arguments */
    Stack_Ptr[10]=Arg;
    Stack_Ptr[11]=0x11111111;
    Stack_Ptr[12]=0x12121212;
    Stack_Ptr[13]=0x13131313;
    Stack_Ptr[14]=0x14141414;
    Stack_Ptr[15]=0x15151515;
    Stack_Ptr[16]=0x16161616;
    Stack_Ptr[17]=0x17171717;
    Stack_Ptr[18]=0x18181818;
    Stack_Ptr[19]=0x19191919;
    Stack_Ptr[20]=0x20202020;
    Stack_Ptr[21]=0x21212121;
    Stack_Ptr[22]=0x22222222;
    Stack_Ptr[23]=0x23232323;
    Stack_Ptr[24]=0x24242424;
    Stack_Ptr[25]=0x25252525;
    Stack_Ptr[26]=0x26262626;
    Stack_Ptr[27]=0x27272727;
    Stack_Ptr[28]=0x28282828;
    Stack_Ptr[29]=0x29292929;
    Stack_Ptr[30]=0x30303030;
    Stack_Ptr[31]=0x31313131;
}
/* End Function:_RMP_Stack_Init **********************************************/

/* Begin Function:_RMP_Low_Level_Init *****************************************
Description : Initialize the low level hardware of the system. This is adapted from
              Arduino and FE310 examples.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Low_Level_Init(void)
{
    RMP_RV32IMAC_LOW_LEVEL_INIT();
    RMP_Int_Enable();
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
    RMP_RV32IMAC_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* Begin Function:_RMP_Yield **************************************************
Description : Trigger a yield to another thread. This will trigger the software
              interrupt in RISC-V.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Yield(void)
{
    CLINT_REG(CLINT_MSIP)=0xFFFFFFFF;
    _RMP_Mem_FENCE();
}
/* End Function:_RMP_Yield ***************************************************/

/* Begin Function:PendSV_Handler **********************************************
Description : The PendSV interrupt routine. This is used to switch contexts.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void PendSV_Handler(void)
{
    /* Clear the software interrupt, if still pending */
    CLINT_REG(CLINT_MSIP)=0;
    RMP_Ctx_Save();
    _RMP_High_Rdy_Get();
    RMP_Ctx_Load();
}
/* End Function:PendSV_Handler ***********************************************/

/* Begin Function:SysTick_Handler *********************************************
Description : The SysTick interrupt routine.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void SysTick_Handler(void)
{
    /* Update the next tick */
    MTIMECMP=MTIME+RMP_RISCV_TICK_COUNT;
    _RMP_Tick_Handler(1);
}
/* End Function:SysTick_Handler **********************************************/

/* Begin Function:Periph_Handler **********************************************
Description : The handler routine for peripherals.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Periph_Handler(void)
{
    plic_instance_t RMP_Global_PLIC;
    rmp_ptr_t Int_Number;

    RMP_Global_PLIC.base_addr=PLIC_CTRL_ADDR;
    /* Read PLIC for the interrupt number, and try to dispatch it to there */
    Int_Number=PLIC_claim_interrupt(&RMP_Global_PLIC);

    if(Int_Number<RMP_RV32IMAC_INT_NUMBER)
    {
        if(RMP_Periph_Vect_Table[Int_Number]!=0)
            ((void(*)(void))RMP_Periph_Vect_Table[Int_Number])();
    }

    PLIC_complete_interrupt(&RMP_Global_PLIC,Int_Number);
}
/* End Function:Periph_Handler ***********************************************/

/* Begin Function:_RMP_Int_Handler ********************************************
Description : The interrupt handler routine.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Int_Handler(void)
{
    rmp_ptr_t MCAUSE;
    MCAUSE=_RMP_Get_MCAUSE();

    /* If this is an exception, die here */
    if((MCAUSE&0x80000000)==0)
        while(1);

    switch(MCAUSE&0x7FFFFFFF)
    {
        /* Machine software interrupt */
        case 3:PendSV_Handler();break;
        case 7:SysTick_Handler();break;
        case 11:Periph_Handler();break;
        /* Should not be other cases */
        default:while(1);
    }
}
/* End Function:_RMP_Int_Handler *********************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
