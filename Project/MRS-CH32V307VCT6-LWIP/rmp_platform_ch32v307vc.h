/******************************************************************************
Filename    : rmp_platform_ch32v307vc.h
Author      : pry
Date        : 24/06/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The configuration file for CH32V307VC RISC-V chip.  This chip 
              carries multiple esoteric features; see the standard configuration
              header for details. This test assumes 128k RAM/192k Flash to be
              able to accomodate the lwIP stack.
******************************************************************************/

/* Define ********************************************************************/
/* The HAL library */
#include "ch32v30x.h"
#include "debug.h"
#include "core_riscv.h"

/* Debugging */
#define RMP_ASSERT_CORRECT              (0U)
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 32 is usually sufficient */
#define RMP_PREEMPT_PRIO_NUM            (32U)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX                   (100000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX                 (1000U)
/* Are we using custom hooks? */
#define RMP_HOOK_EXTRA                  (0U)
/* The stzck size of the init thread */
#define RMP_INIT_STACK_SIZE             (8*1024U)
/* The mask/unmask interrupt operations */
#define RMP_INT_MASK()                  RMP_Int_Disable()
#define RMP_INT_UNMASK()                RMP_Int_Enable()

/* 1ms tick time for 144MHz */
#define RMP_RV32P_OSTIM_VAL             (144000U)
/* What is the FPU type? */
#define RMP_RV32P_COP_RVF               (1U)
#define RMP_RV32P_COP_RVD               (0U)

/* Reprogram the timer or clear timer interrupt flags */
#define RMP_RV32P_TIM_CLR()             (SysTick->SR=0)

/* Other low-level initialization stuff - clock and serial. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_Start_Hook). */
#define RMP_RV32P_LOWLVL_INIT() \
do \
{ \
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); \
    USART_Printf_Init(115200U); \
    RMP_Int_Disable(); \
    SysTick->CMP=RMP_RV32P_OSTIM_VAL; \
    SysTick->CTLR=0x3FU; \
    NVIC_SetPriority(SysTicK_IRQn,0xff); \
    NVIC_EnableIRQ(SysTicK_IRQn); \
} \
while(0)

/* This is for debugging output */
#define RMP_RV32P_PUTCHAR(CHAR)         putchar(CHAR)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
