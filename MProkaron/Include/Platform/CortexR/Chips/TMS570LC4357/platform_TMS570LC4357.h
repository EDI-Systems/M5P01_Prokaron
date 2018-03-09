/******************************************************************************
Filename   : platform_TMS570LC4357.h
Author     : pry
Date       : 24/06/2017
Licence    : LGPL v3+; see COPYING for details.
Description: The configuration file for TMS570LC4357.
******************************************************************************/

/* Defines *******************************************************************/
/* The HAL library */
#include "HL_sys_common.h"
#include "HL_system.h"

/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 32 is usually sufficient */
#define RMP_MAX_PREEMPT_PRIO         32
/* The maximum number of slices allowed */
#define RMP_MAX_SLICES               100000
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_MAX_NUM              1000
/* Are we using custom hooks? */
#define RMP_USE_HOOKS                RMP_FALSE
/* The stzck size of the init thread */
#define RMP_INIT_STACK_SIZE          256
/* The mask/unmask interrupt operations */
#define RMP_MASK_INT()               RMP_Disable_Int()
#define RMP_UNMASK_INT()             RMP_Enable_Int()

/* Other low-level initialization stuff - clock and serial  */
#define RMP_CRX_LOW_LEVEL_INIT() \
do \
{ \
    /* The TI library is in charge of all the initialization of interrupts and clocks */\
} \
while(0)

/* This is for debugging output */
#define RMP_CRX_PUTCHAR(CHAR) \
do \
{ \
} \
while(0)
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
