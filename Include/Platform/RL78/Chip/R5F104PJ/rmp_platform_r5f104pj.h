/******************************************************************************
Filename   : rmp_platform_r5f104pj.h
Author     : lht 
Date       : 24/04/2024
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for R5F104PJ.
******************************************************************************/
#include "iodefine.h"
/* Define ********************************************************************/
/* The HAL library */

/* Debugging */
#define RMP_ASSERT_CORRECT          (0U)
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 32 is usually sufficient */
#define RMP_PREEMPT_PRIO_NUM        (16U)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (32000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX             (100U)
/* Are we using custom hooks? */
#define RMP_HOOK_EXTRA              (0U)
/* The stzck size of the init thread */
#define RMP_INIT_STACK_SIZE         (256U)
/* The mask/unmask interrupt operations - can be adjusted to fit your own levels */
#define RMP_INT_MASK()              RMP_Int_Disable()
#define RMP_INT_UNMASK()            RMP_Int_Enable()
/* What is the Systick value? */
#define RMP_RL78_SYSTICK_VAL        (1000U)

/*lowlevel init mainly in r_systemint.c */
#define RMP_RL78_LOWLVL_INIT()

#define RMP_RL78_TIM_CLR()          (TMIF00=0U)

#define RMP_RL78_PUTCHAR(CHAR) \
{ \
    TXD1=CHAR; \
    while(STIF1==0U); \
    STIF1=0U; \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
