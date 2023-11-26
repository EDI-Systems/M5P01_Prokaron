/******************************************************************************
Filename   : platform_dspic33ep512mu810.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for DSPIC33EP512MU810.
******************************************************************************/

/* Define ********************************************************************/
/* The MPLAB libraries */
#include <xc.h>
#include <stddef.h>

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
#define RMP_INIT_STACK_SIZE         (1024U)
/* The mask/unmask interrupt operations */
#define RMP_INT_MASK()              RMP_Int_Disable()
#define RMP_INT_UNMASK()            RMP_Int_Enable()

/* Timer constants */
#define RMP_DSPIC_TIMER_ON          (0x8000U)
#define RMP_DSPIC_TIMER_PRESC1      (0x0000U)
#define RMP_DSPIC_TIMER_PRESC8      (0x0010U)
#define RMP_DSPIC_TIMER_PRESC64     (0x0020U)
#define RMP_DSPIC_TIMER_PRESC256    (0x0030U)

/* What is the tick timer tick value? */
#define RMP_DSPIC_TICK_VAL          (60000U)
/* What is the prescaler value? - prescaled by 8. Input is Fcpu/2, thus this is
 * dividing it by 16 with regards to CPU clock rate */
#define RMP_DSPIC_TICK_PRESC        RMP_DSPIC_TIMER_PRESC8

/* Other low-level initialization stuff - clock and serial. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_Start_Hook). */
#define RMP_DSPIC_LOWLVL_INIT() \
do \
{ \
    RMP_Int_Disable(); \
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
    /*  DSPIC33 starter board have no UART, print to an array instead */ \
    static rmp_ptr_t Ptr=0; \
    volatile static rmp_s8_t Output[4096]; \
    Output[Ptr++]=(CHAR); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
