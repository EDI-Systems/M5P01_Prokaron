/******************************************************************************
Filename    : platform_msp430.c
Author      : pry
Date        : 25/02/2018
Licence     : LGPL v3+; see COPYING for details.
Description : The platform specific file for MSP430.
******************************************************************************/

/* Includes ******************************************************************/
#define __HDR_DEFS__
#include "Platform/MSP430/platform_msp430.h"
#include "Kernel/kernel.h"
#undef __HDR_DEFS__

#define __HDR_STRUCTS__
#include "Platform/MSP430/platform_msp430.h"
#include "Kernel/kernel.h"
#undef __HDR_STRUCTS__

/* Private include */
#include "Platform/MSP430/platform_msp430.h"

#define __HDR_PUBLIC_MEMBERS__
#include "Kernel/kernel.h"
#undef __HDR_PUBLIC_MEMBERS__
/* End Includes **************************************************************/

/* Begin Function:RMP_MSB_Get *************************************************
Description : Get the MSB of the word.
Input       : ptr_t Val - The value.
Output      : None.
Return      : ptr_t - The MSB position.
******************************************************************************/
/* 2*i and 2*i+1 will correspond to the same slot in this table */
const u8 RMP_MSB_Tbl[128]=
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

ptr_t RMP_MSB_Get(ptr_t Val)
{
    /* Something at high bits */
    if((Val&0xFF00)!=0)
        return RMP_MSB_Tbl[Val>>9]+16;
    /* Something at low bits */
    else if((Val&0xFF)!=0)
        return RMP_MSB_Tbl[Val>>1];
    /* Nothing anywhere */
    return 0xFFFF;
}
/* End Function:RMP_MSB_Get **************************************************/

/* Begin Function:_RMP_Stack_Init *********************************************
Description : Initiate the process stack when trying to start a process. Never
              call this function in user application.
Input       : ptr_t Entry - The entry of the thread.
              ptr_t Stack - The stack address of the thread.
              ptr_t Arg - The argument to pass to the thread.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Stack_Init(ptr_t Entry, ptr_t Stack, ptr_t Arg)
{

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
    RMP_MSP430_LOW_LEVEL_INIT();
    
    /* Enable all fault handlers */
    
    /* Set the priority of timer, svc and faults to the lowest */
    TA0CCTL0 = CCIE;                             // CCR0 interrupt enabled
    TA0CTL = TASSEL_2 + MC_1 + ID_3;             // SMCLK/8, upmode
    TA0CCR0 =  1000;                                 // 1250 Hz
    TA0CCTL1 = CCIE;                             // CCR1 interrupt enabled

    /* Manully pend the interrupt - test successful! */
    TA0CCTL1|=CCIFG_1;

    /* Configure systick */
    
    RMP_Disable_Int();
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
    RMP_MSP430_PUTCHAR(Char);
}
/* End Function:RMP_Putchar **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
