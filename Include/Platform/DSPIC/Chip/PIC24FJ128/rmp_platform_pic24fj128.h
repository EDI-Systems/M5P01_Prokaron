/******************************************************************************
Filename   : rmp_platform_pic24fj128.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for PIC24FJ128.
******************************************************************************/

/* Define ********************************************************************/
/* The MPLAB XC16 libraries */
#include <xc.h>
#include <stddef.h>

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
#define RMP_INIT_STACK_SIZE         (256U)

/* GUI */
#define RMP_GUI_ENABLE              (0U)
/* Anti-aliasing */
#define RMP_GUI_ANTIALIAS_ENABLE    (0U)
/* Widgets */
#define RMP_GUI_WIDGET_ENABLE       (0U)

/* What is the tick timer tick value? */
#define RMP_DSPIC_TICK_VAL          (16000U)
/* The exact CPU type */
#define RMP_DSPIC_COP_24F_24H       (1U)
#define RMP_DSPIC_COP_24E           (0U)
#define RMP_DSPIC_COP_30F_33F       (0U)
#define RMP_DSPIC_COP_33E_33C       (0U)

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
    /* No need to configure PLL at all */ \
    /* Set the timer with IPL=1 */ \
    T1CON=0U; \
    IPC0bits.T1IP=1U; \
    IFS0bits.T1IF=0U; \
    TMR1=0U; \
    PR1=RMP_DSPIC_TICK_VAL; \
    T1CON=RMP_DSPIC_TIM_ON|RMP_DSPIC_TIM_PRESC; \
    IEC0bits.T1IE=1U; \
    /* UART2 RF5 115200-8-N-1 */ \
    U2MODE=0U; \
    U2STA=0U; \
    U2MODEbits.BRGH=1U; \
    /* Don't be fooled by PIC24 manual: Fcy=Fosc/2, Fcpu=Fperi=Fcy */ \
    U2BRG=16000000UL/4UL/115200UL-1U; \
    U2MODEbits.UARTEN=1U; \
    U2STAbits.UTXEN=1U; \
} \
while(0)

#define RMP_DSPIC_TIM_CLR()         (IFS0bits.T1IF=0U)

/* This is for debugging output */
#define RMP_DSPIC_PUTCHAR(CHAR) \
do \
{ \
    while(U2STAbits.UTXBF!=0U); \
    U2TXREG=(CHAR); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
