/******************************************************************************
Filename   : platform_MSP430FR5994.h
Author     : pry
Date       : 24/06/2017
Licence    : LGPL v3+; see COPYING for details.
Description: The configuration file for MSP43FR5994.
******************************************************************************/

/* Defines *******************************************************************/
/* The TI-supplied header */
#include <msp430.h>

/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 16 is usually sufficient */
#define RMP_MAX_PREEMPT_PRIO         16
/* The maximum number of slices allowed */
#define RMP_MAX_SLICES               10000
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_MAX_NUM              1000
/* Are we using custom hooks? */
#define RMP_USE_HOOKS                RMP_FALSE
/* The stzck size of the init thread */
#define RMP_INIT_STACK_SIZE          128

/* We are using MSP430X */
#define RMP_MSP430_X                 RMP_TRUE

/* Other low-level initialization stuff - clock and serial */
#define RMP_MSP430_LOW_LEVEL_INIT() \
do \
{ \
    /* Stop the WDT */ \
    WDTCTL = WDTPW | WDTHOLD; \
} \
while(0)

/* This is for debugging output */
#define RMP_MSP430_PUTCHAR(CHAR) \
do \
{ \
} \
while(0)
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
