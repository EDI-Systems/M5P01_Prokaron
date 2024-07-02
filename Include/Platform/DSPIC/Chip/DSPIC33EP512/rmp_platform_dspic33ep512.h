/******************************************************************************
Filename   : rmp_platform_dspic33ep512.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for DSPIC33EP512.
******************************************************************************/

/* Define ********************************************************************/
/* The MPLAB XC16 libraries */
#include <xc.h>
#include <stddef.h>

/* Debugging */
#define RMP_ASSERT_ENABLE           (1U)
#define RMP_CHECK_ENABLE            (1U)

/* System */
/* The maximum number of preemption priorities */
#define RMP_PREEMPT_PRIO_NUM        (16U)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (10000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX             (1000U)
/* The stack size of the init thread */
#define RMP_INIT_STACK_SIZE         (1024U)

/* GUI */
#define RMP_GUI_ENABLE              (0U)
/* Anti-aliasing */
#define RMP_GUI_ANTIALIAS_ENABLE    (0U)
/* Widgets */
#define RMP_GUI_WIDGET_ENABLE       (0U)

/* What is the tick timer tick value? */
#define RMP_DSPIC_TICK_VAL          (60000U)
/* The exact CPU type */
#define RMP_DSPIC_COP_24F_24H       (0U)
#define RMP_DSPIC_COP_24E           (0U)
#define RMP_DSPIC_COP_30F_33F       (0U)
#define RMP_DSPIC_COP_33E_33C       (1U)

/* Timer constants */
#define RMP_DSPIC_TIM_ON            (0x8000U)
#define RMP_DSPIC_TIM_PRESC1        (0x0000U)
#define RMP_DSPIC_TIM_PRESC8        (0x0010U)
#define RMP_DSPIC_TIM_PRESC64       (0x0020U)
#define RMP_DSPIC_TIM_PRESC256      (0x0030U)
/* Tick timer prescaler value  */
#define RMP_DSPIC_TIM_PRESC         RMP_DSPIC_TIM_PRESC8

/* Other low-level initialization stuff - clock and serial. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_START_HOOK). */
#define RMP_DSPIC_LOWLVL_INIT() \
do \
{ \
    /* Configure PLL to pump device up to Fcpu=Fperi=Fcy=60MHz */ \
    PLLFBD=238; \
    CLKDIVbits.PLLPOST=0; \
    CLKDIVbits.PLLPRE=6; \
    while(OSCCONbits.COSC!=0b011); \
    while(OSCCONbits.LOCK!=1); \
    /* Set the timer with IPL=1 */ \
    T1CON=0; \
    IPC0bits.T1IP=1; \
    IFS0bits.T1IF=0; \
    TMR1=0; \
    PR1=RMP_DSPIC_TICK_VAL; \
    T1CON=RMP_DSPIC_TIM_ON|RMP_DSPIC_TIM_PRESC; \
    IEC0bits.T1IE=1; \
} \
while(0)

#define RMP_DSPIC_TIM_CLR()         (IFS0bits.T1IF=0U)

/* This is for debugging output */
#define RMP_DSPIC_PUTCHAR(CHAR) \
do \
{ \
    /*  DSPIC33 starter board have no UART, print to an array instead */ \
    static rmp_ptr_t Ptr=0; \
    volatile static rmp_s8_t Output[4096]; \
    Output[Ptr++]=(CHAR); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
