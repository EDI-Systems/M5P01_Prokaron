/******************************************************************************
Filename   : rmp_platform_spce061a.h
Author     : pry
Date       : 23/05/2024
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for SPCE061A.
******************************************************************************/

/* Define ********************************************************************/
/* The UNSP I/O library */
#include "SPCE061V004.H"

/* Debugging */
#define RMP_ASSERT_CORRECT          (0U)
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 16 is usually sufficient */
#define RMP_PREEMPT_PRIO_NUM        (16U)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (10000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX             (1000U)
/* Are we using custom hooks? */
#define RMP_HOOK_EXTRA              (0U)
/* The stack size of the init thread */
#define RMP_INIT_STACK_SIZE         (96U)
/* The mask/unmask interrupt operations */
#define RMP_INT_MASK()              RMP_Int_Mask(0x01U)
#define RMP_INT_UNMASK()            RMP_Int_Mask(0x00U)

/* Tick timer value decided in IRQ - 1/1024s */
#define RMP_UNSP_TICK_VAL           (C_IRQ4_1KHz)
/* Is this unSP V2? */
#define RMP_UNSP_COP_SPV2           (0U)

/* Other low-level initialization stuff - clock and serial.
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_Start_Hook). */
#define RMP_UNSP_LOWLVL_INIT() \
do \
{ \
    /* System clock: Fosc=Fcpu=49.152MHz */ \
    *P_SystemClock=C_Fosc_49M|C_Fosc; \
    /* UART TX on IOB10, configured as output */ \
    *P_IOB_Dir=0x0400U; \
    /* UART reset then TX enable, 115200-8-N-1 */ \
    *P_UART_Command1=C_UART_Reset; \
    *P_UART_Command1=0x00U; \
    *P_UART_BaudScalarHigh=0x00U; \
    *P_UART_BaudScalarLow=0x6BU; \
    *P_UART_Command2=C_UART_Tx_Pin_ENB; \
    /* Configure 1/1024s tick timer on IRQ4 */ \
    *P_INT_Clear=0xFFFFU; \
    *P_INT_Ctrl=RMP_UNSP_TICK_VAL; \
} \
while(0)

/* Feed the dog and clear the flag */
#define RMP_UNSP_TIM_CLR() \
do \
{ \
    *P_Watchdog_Clear=C_WDTCLR; \
    *P_INT_Clear=RMP_UNSP_TICK_VAL; \
} \
while(0)

/* This is for debugging output */
#define RMP_UNSP_PUTCHAR(CHAR) \
do \
{ \
    while(((*P_UART_Command2)&C_UART_Tx_RDY)==0U); \
    *P_UART_Data=(CHAR); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
