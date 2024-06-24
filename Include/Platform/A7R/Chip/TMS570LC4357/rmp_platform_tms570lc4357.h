/******************************************************************************
Filename   : rmp_platform_tms570lc4357.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for TMS570LC4357.
******************************************************************************/

/* Define ********************************************************************/
/* The HAL library */
#include "HL_sys_common.h"
#include "HL_system.h"
#include "HL_rti.h"
#include "HL_sci.h"

/* Debugging */
#define RMP_ASSERT_ENABLE           (1U)
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 32 is usually sufficient */
#define RMP_PREEMPT_PRIO_NUM        (32U)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (100000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX             (1000U)
/* Are we using custom hooks? */
#define RMP_HOOK_EXTRA              (0U)
/* The stzck size of the init thread */
#define RMP_INIT_STACK_SIZE         (1024U)
/* The mask/unmask interrupt operations */
#define RMP_INT_MASK()              RMP_Int_Disable()
#define RMP_INT_UNMASK()            RMP_Int_Enable()

#define RMP_A7R_SYSTICK_VAL         (3750U)

/* Other low-level initialization stuff - clock and serial. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_Start_Hook). */
#define RMP_A7R_LOWLVL_INIT() \
do \
{ \
    /* The TI library is in charge of all the initialization of interrupts and clocks */\
    rtiInit(); \
    rtiSetPeriod(rtiREG1, rtiCOMPARE0, RMP_A7R_SYSTICK_VAL); \
    rtiEnableNotification(rtiREG1, rtiNOTIFICATION_COMPARE0); \
    rtiStartCounter(rtiREG1, rtiCOUNTER_BLOCK0); \
    /* Enable SCI */ \
    sciInit(); \
} \
while(0)

/* This is for debugging output */
#define RMP_A7R_PUTCHAR(CHAR) \
do \
{ \
    sciSendByte(sciREG1,(CHAR)); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
