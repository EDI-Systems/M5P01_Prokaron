/******************************************************************************
Filename   : rmp_platform_hc32l163k8.h
Author     : pry
Date       : 24/06/2017
Licence    : LGPL v3+; see COPYING for details.
Description: The configuration file for HC32L136K8.
******************************************************************************/

/* Defines *******************************************************************/
/* The HAL library */
#include "ddl.h"
#include "core_cm0plus.h"

/* Debugging */
#define RMP_ASSERT_CORRECT          (0U)
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
#define RMP_INIT_STACK_SIZE         (256U)
/* The mask/unmask interrupt operations */
#define RMP_INT_MASK()              RMP_Int_Disable()
#define RMP_INT_UNMASK()            RMP_Int_Enable()

/* What is the NVIC priority grouping? */
#define RMP_A6M_NVIC_GROUPING       RMP_A6M_NVIC_GROUPING_P2S6
/* What is the Systick value? */
#define RMP_A6M_SYSTICK_VAL         (24000U)

/* Other low-level initialization stuff - clock and serial
 * This device requires that you switch from low to high frequency.
 * The manufacturer have defined their own delay functions - do not use
 * them, because they are not OS agnostic and will reprogram the systick 
 * to something that you possibly don't want. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_Start_Hook). */
#define RMP_A6M_LOW_LEVEL_INIT() \
do \
{ \
    /* Switch to the target clock frequency */ \
    Sysctrl_SetRCLTrim(SysctrlRclFreq32768); \
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, TRUE); \
    Sysctrl_SysClkSwitch(SysctrlClkRCL); \
    Sysctrl_SetRCHTrim(SysctrlRchFreq24MHz); \
	Sysctrl_SysClkSwitch(SysctrlClkRCH); \
    /* Turn off the low speed RC oscillator and update core clock */ \
	Sysctrl_ClkSourceEnable(SysctrlClkRCL, FALSE); \
    SystemCoreClockUpdate(); \
    /* Initialize I/O and serial */ \
    /* Set the priority of timer, svc and faults to the lowest */ \
    NVIC_SetPriority(SVC_IRQn, 0xFF); \
    NVIC_SetPriority(PendSV_IRQn, 0xFF); \
    NVIC_SetPriority(SysTick_IRQn, 0xFF); \
    /* Configure systick */ \
    SysTick_Config(RMP_A6M_SYSTICK_VAL); \
} \
while(0)

/* This is for debugging output */
#define RMP_A6M_PUTCHAR(CHAR) \
do \
{ \
    /* Need to add serial print */ \
} \
while(0)
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
