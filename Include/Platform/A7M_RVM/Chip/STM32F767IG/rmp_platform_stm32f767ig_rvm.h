/******************************************************************************
Filename   : rmp_platform_stm32f767ig_rvm.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for STM32F767IG, on RVM.
******************************************************************************/

/* Define ********************************************************************/
/* Debugging */
#define RMP_ASSERT_ENABLE           (1U)
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 32 is usually sufficient */
#define RMP_PREEMPT_PRIO_NUM        (32U)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (100000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX             (1000)
/* Are we using custom hooks? */
#define RMP_HOOK_EXTRA              (0U)
/* The stack size of the init thread */
#define RMP_INIT_STACK_SIZE         (1024U)

/* GUI */
#define RMP_GUI_ENABLE              (0U)
/* Anti-aliasing */
#define RMP_GUI_ANTIALIAS_ENABLE    (0U)
/* Widgets */
#define RMP_GUI_WIDGET_ENABLE       (0U)

/* What is the FPU type? */
#define RMP_A7M_RVM_COP_FPV4_SP     (1U)
#define RMP_A7M_RVM_COP_FPV5_SP     (1U)
#define RMP_A7M_RVM_COP_FPV5_DP     (1U)
/* Whether to use fast yield routine */
#define RMP_A7M_RVM_FAST_YIELD      (1U)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
