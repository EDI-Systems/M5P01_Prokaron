/******************************************************************************
Filename   : platform_PIC32MZ2048EFM100.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for PIC32MZ2048EFM100.
******************************************************************************/

/* Defines *******************************************************************/
/* The MPLAB libraries */
#include <xc.h>
#include <stddef.h>
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 16 is usually sufficient */
#define RMP_MAX_PREEMPT_PRIO         16
/* The maximum number of slices allowed */
#define RMP_MAX_SLICES               10000
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_MAX_NUM              1000
/* Are we using custom hooks? */
#define RMP_USE_HOOKS                RMP_FALSE
/* The stack size of the init thread */
#define RMP_INIT_STACK_SIZE          1024
/* The mask/unmask interrupt operations */
#define RMP_MASK_INT()               RMP_Disable_Int()
#define RMP_UNMASK_INT()             RMP_Enable_Int()

/* Timer constants */
#define RMP_DSPIC_TIMER_ON           0x8000
#define RMP_DSPIC_TIMER_PRESC1       0x0000
#define RMP_DSPIC_TIMER_PRESC8       0x0010
#define RMP_DSPIC_TIMER_PRESC64      0x0020
#define RMP_DSPIC_TIMER_PRESC256     0x0030

/* What is the tick timer tick value? */
#define RMP_DSPIC_TICK_VAL           60000
/* What is the prescaler value? - prescaled by 8. Input is Fcpu/2, thus this is
 * dividing it by 16 with regards to CPU clock rate */
#define RMP_DSPIC_TICK_PRESC         RMP_DSPIC_TIMER_PRESC8

/* Other low-level initialization stuff - clock and serial. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_Start_Hook). */
#define RMP_DSPIC_LOW_LEVEL_INIT() \
do \
{ \
    RMP_Disable_Int(); \
    /* Enable external interrupt 0 which is used for PendSV */ \
    IPC0bits.INT0IP=1; \
    IFS0bits.INT0IF=0; \
    IEC0bits.INT0IE=1; \
    /* Set the timer timeout value */ \
    _RMP_Set_Timer(RMP_DSPIC_TICK_VAL/2); \
} \
while(0)

/* Because we need to choose an unused timer as our system timer, we make it flexible 
 * here. You may choose one timer that is never used. By default, we use timer 1. */
#define RMP_DSPIC_SET_TIMER(TICKS) \
do \
{ \
    /* Stop counter */ \
    T1CON=0; \
    /* Clear interrupt flag and set priority to lowest */ \
    IPC0bits.T1IP=1; \
    IFS0bits.T1IF=0; \
    /* Clear counter */ \
    TMR1=0; \
    PR1=(TICKS); \
    T1CON=RMP_DSPIC_TIMER_ON|RMP_DSPIC_TICK_PRESC; \
    IEC0bits.T1IE=1; \
} \
while(0)

#define RMP_DSPIC_CLEAR_TIMER_FLAG() \
do \
{ \
    IFS0bits.T1IF=0; \
} \
while(0)

/* Because we need to choose an unused peripheral to trigger the software interrupt,
 * we make it flexible here. You may choose one interrupt source that is never used. */
#define RMP_DSPIC_YIELD() \
do \
{ \
    /* We default to external interrupt 0 */ \
    IFS0bits.INT0IF=1; \
} \
while(0)

#define RMP_DSPIC_CLEAR_SOFT_FLAG() \
do \
{ \
    IFS0bits.INT0IF=0; \
} \
while(0)

/* This is for debugging output */
#define RMP_DSPIC_PUTCHAR(CHAR) \
do \
{ \
    /* By default we don't provide this cause DSPIC33 starter board have no UART */ \
} \
while(0)
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
