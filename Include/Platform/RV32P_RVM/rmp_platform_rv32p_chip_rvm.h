/******************************************************************************
Filename    : rmp_platform_rv32p_chip_rvm.h
Author      : pry
Date        : 24/06/2017
Licence     : The Unlicense; see LICENSE for details.
Description: The configuration file for any virtualized RISC-V 32-bit chip.
******************************************************************************/

/* Include *******************************************************************/
#include "rvm_guest.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
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
/* The stack size of the init thread */
#define RMP_INIT_STACK_SIZE         (256U)

/* GUI */
#define RMP_GUI_ENABLE              (0U)
/* Anti-aliasing */
#define RMP_GUI_ANTIALIAS_ENABLE    (0U)
/* Widgets */
#define RMP_GUI_WIDGET_ENABLE       (0U)

/* What is the FPU type? */
#define RMP_RV32P_RVM_COP_RVF       RVM_RV32P_COP_RVF
#define RMP_RV32P_RVM_COP_RVD       RVM_RV32P_COP_RVD

/* Whether to use fast yield routine */
#define RMP_RV32P_RVM_FAST_YIELD    (1U)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
