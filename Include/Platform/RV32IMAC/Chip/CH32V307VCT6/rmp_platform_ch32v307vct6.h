/******************************************************************************
Filename    : rmp_platform_ch32v307vct6.h
Author      : pry
Date        : 24/06/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The configuration file for CH32V307VCT6 RISC-V chip.
******************************************************************************/

/* Defines *******************************************************************/
/* The HAL library */
#include "ch32v30x.h"
#include "debug.h"
#include "core_riscv.h"

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

/* 1ms tick time for 96MHz */
#define RMP_RV32IMAC_TICK_COUNT     (96000U)

/* Mcause for interrupt vectors */
#define RMP_RV32IMAC_TICK_MCAUSE    (12U)
#define RMP_RV32IMAC_SWITCH_MCAUSE  (14U)

/* Other low-level initialization stuff - clock and serial. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_Start_Hook). */
#define RMP_RV32IMAC_LOW_LEVEL_INIT() \
do \
{ \
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); \
    Delay_Init(); \
    USART_Printf_Init(115200); \
    printf("SystemClk:%d\r\n",SystemCoreClock); \
    RMP_Int_Disable(); \
    SysTick->CMP=RMP_RV32IMAC_TICK_COUNT; \
    SysTick->CTLR=0x3F; \
     \
    NVIC_EnableIRQ(Software_IRQn); \
} \
while(0)
//NVIC_EnableIRQ(SysTicK_IRQn);
/* Optional platform hook */
#define RMP_RV32IMAC_PLAT_HOOK() \
extern void Test_Handler(void); \
Test_Handler()

/* Reprogram the timer or clear timer interrupt flags */
#define RMP_RV32IMAC_TICK_RESET()       SysTick->SR=0

/* Trigger/clear software interrupt */
#define RMP_RV32IMAC_SWITCH_SET()       NVIC_SetPendingIRQ(Software_IRQn)
#define RMP_RV32IMAC_SWITCH_CLR()       NVIC_ClearPendingIRQ(Software_IRQn)

/* Peripheral handler hook */
#define RMP_RV32IMAC_PERIPH_HANDLER()

/* This is for debugging output */
#define RMP_RV32IMAC_PUTCHAR(CHAR) \
do \
{ \
    printf("%c",CHAR); \
} \
while(0)
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/