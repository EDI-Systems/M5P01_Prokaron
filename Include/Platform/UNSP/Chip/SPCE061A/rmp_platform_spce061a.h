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
/* Assertion */
#define RMP_ASSERT_ENABLE           (1U)
/* Invalid parameter checking */
#define RMP_CHECK_ENABLE            (1U)
/* Debug logging */
#define RMP_DBGLOG_ENABLE           (1U)

/* System */
/* The maximum number of preemption priorities */
#define RMP_PREEMPT_PRIO_NUM        (16U)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (10000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX             (1000U)
/* The stack size of the init thread */
#define RMP_INIT_STACK_SIZE         (128U)

/* GUI */
#define RMP_GUI_ENABLE              (0U)
/* Anti-aliasing */
#define RMP_GUI_ANTIALIAS_ENABLE    (0U)
/* Widgets */
#define RMP_GUI_WIDGET_ENABLE       (0U)

/* Tick timer value decided in IRQ - 1/1024s */
#define RMP_UNSP_TICK_VAL           (C_IRQ4_1KHz)
/* Is this unSP V2? */
#define RMP_UNSP_COP_SPV2           (0U)

/* Other low-level initialization stuff - clock and serial.
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_START_HOOK). */
#define RMP_UNSP_LOWLVL_INIT() \
do \
{ \
    /* System clock: Fosc=Fcpu=24.576MHz; we can't run Fosc at 49.152MHz, \
     * or the timer reads will become unreliable. This is not documented \
     * in the SPCE061A manual at all, so this is a silicon bug. */ \
    *P_SystemClock=C_Fosc_24M|C_32K_Work|C_StrongMode|C_Fosc; \
    /* UART TX on IOB10 - set attrib to avoid inversion; see datasheet */ \
    *P_IOB_Attrib=0x0400U; \
    *P_IOB_Dir=0x0400U; \
    *P_IOB_Data=0x0000U; \
    /* UART reset then TX enable, 115200-8-N-1 */ \
    *P_UART_Command1=C_UART_Reset; \
    *P_UART_Command1=0x00U; \
    *P_UART_BaudScalarHigh=0x00U; \
    *P_UART_BaudScalarLow=0x6BU; \
    *P_UART_Command2=C_UART_Tx_Pin_ENB; \
    /* Configure 1/1024s tick timer on IRQ4 */ \
    *P_INT_Clear=0xFFFFU; \
    *P_INT_Mask=RMP_UNSP_TICK_VAL; \
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
