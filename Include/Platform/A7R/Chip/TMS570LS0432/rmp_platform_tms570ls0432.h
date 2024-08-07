/******************************************************************************
Filename   : rmp_platform_tms570ls0432.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for TMS570LS0432.
******************************************************************************/

/* Define ********************************************************************/
/* The HAL library */
#include "sys_common.h"
#include "system.h"
#include "rti.h"
#include "sci.h"

/* Debugging */
/* Assertion */
#define RMP_ASSERT_ENABLE           (1U)
/* Invalid parameter checking */
#define RMP_CHECK_ENABLE            (1U)
/* Debug logging */
#define RMP_DBGLOG_ENABLE           (1U)

/* System */
/* The maximum number of preemption priorities */
#define RMP_PREEMPT_PRIO_NUM        (32U)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (100000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX             (1000U)
/* Are we using custom hooks? */
#define RMP_HOOK_EXTRA              (0U)
/* The stack size of the init thread */
#define RMP_INIT_STACK_SIZE         (1024U)
/* The mask/unmask interrupt operations */
#define RMP_INT_MASK()              RMP_Int_Disable()
#define RMP_INT_UNMASK()            RMP_Int_Enable()

/* GUI */
#define RMP_GUI_ENABLE              (0U)
/* Anti-aliasing */
#define RMP_GUI_ANTIALIAS_ENABLE    (0U)
/* Widgets */
#define RMP_GUI_WIDGET_ENABLE       (0U)

#define RMP_A7R_SYSTICK_VAL         (10000U)

/* Other low-level initialization stuff - clock and serial. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_START_HOOK). */
#define RMP_A7R_LOWLVL_INIT() \
do \
{ \
    /* The TI library is in charge of all the initialization of interrupts and clocks */ \
    rtiInit(); \
    rtiSetPeriod(rtiCOMPARE0, RMP_A7R_SYSTICK_VAL); \
    rtiEnableNotification(rtiNOTIFICATION_COMPARE0); \
    rtiStartCounter(rtiCOUNTER_BLOCK0); \
    /* Enable SCI */ \
    sciInit(); \
} \
while(0)

/* This is for debugging output */
#define RMP_A7R_PUTCHAR(CHAR) \
do \
{ \
    sciSendByte(scilinREG,(CHAR)); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
