/******************************************************************************
Filename   : rmp_platform_atmega1284p.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for ATMEGA1284P.
******************************************************************************/

/* Define ********************************************************************/
/* The HAL library */
#include "avr/io.h"

/* Debugging */
#define RMP_ASSERT_CORRECT          (0U)
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 32 is usually sufficient */
#define RMP_PREEMPT_PRIO_NUM        (32U)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (10000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX             (1000U)
/* Are we using custom hooks? */
#define RMP_HOOK_EXTRA              (0U)
/* The stack size of the init thread */
#define RMP_INIT_STACK_SIZE         (256U)
/* The mask/unmask interrupt operations */
#define RMP_INT_MASK()              RMP_Int_Disable()
#define RMP_INT_UNMASK()            RMP_Int_Enable()

/* What is the Systick value? */
#define RMP_AVR_TICK_VAL            (2000U)
#define RMP_AVR_COP_256K            (0U)
#define RMP_AVR_COP_RAMPZ           (1U)
#define RMP_AVR_COP_XMEGA           (0U)

/* Other low-level initialization stuff - clock and serial.
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_Start_Hook). */
#define RMP_AVR_LOWLVL_INIT() \
do \
{ \
} \
while(0)

/* This is for debugging output */
#define RMP_AVR_PUTCHAR(CHAR) \
do \
{ \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
