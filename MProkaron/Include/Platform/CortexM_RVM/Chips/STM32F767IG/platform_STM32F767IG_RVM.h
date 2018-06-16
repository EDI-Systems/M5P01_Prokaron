/******************************************************************************
Filename   : platform_STM32F767IG_RVM.h
Author     : pry
Date       : 24/06/2017
Licence    : LGPL v3+; see COPYING for details.
Description: The configuration file for STM32F767IG.
******************************************************************************/

/* Defines *******************************************************************/
/* The HAL library */
#include "stm32f7xx.h"
#include "core_cm7.h"
#include "stm32f7xx_hal.h"
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

/* The mask/unmask interrupt operations - do not need on RVM */
#define RMP_MASK_INT()               RVM_Mask_Int()
#define RMP_UNMASK_INT()             RVM_Unmask_Int()
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
