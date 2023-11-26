/******************************************************************************
Filename   : rmp_platform_stm32f767ig_rvm.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for STM32F767IG, on RVM.
******************************************************************************/

/* Defines *******************************************************************/
/* Debugging */
#define RMP_ASSERT_CORRECT          (0U)
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 32 is usually sufficient */
#define RMP_PREEMPT_PRIO_NUM        (32U)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (100000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX             (1000)
/* Are we using custom hooks? */
#define RMP_HOOK_EXTRA              (0U)
/* The stzck size of the init thread */
#define RMP_INIT_STACK_SIZE         (1024U)

/* The mask/unmask interrupt operations */
#define RMP_INT_MASK()              RMP_Int_Mask()
#define RMP_INT_UNMASK()            RMP_Int_Unmask()
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
