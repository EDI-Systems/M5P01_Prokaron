/******************************************************************************
Filename    : rmp_platform_ch32v307vc_rvm.h
Author      : pry
Date        : 24/06/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The configuration file for CH32V307VC, on RVM.
******************************************************************************/

/* Define ********************************************************************/
/* Debugging */
#define RMP_ASSERT_ENABLE           (1U)
#define RMP_CHECK_ENABLE            (1U)

/* System */
/* The maximum number of preemption priorities */
#define RMP_PREEMPT_PRIO_NUM        (32U)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (100000U)
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

/* What is the FPU type? */
#define RMP_RV32P_RVM_COP_RVF       (1U)
#define RMP_RV32P_RVM_COP_RVD       (0U)

/* Whether to use fast yield routine */
#define RMP_RV32P_RVM_FAST_YIELD    (1U)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
