/******************************************************************************
Filename    : rmp_platform_fe310g000.h
Author      : pry
Date        : 24/06/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The configuration file for FE310-G000 RISC-V chip.
******************************************************************************/

/* Define ********************************************************************/
/* The HAL library */
#include "encoding.h"
#include "platform.h"
#include "plic/plic_driver.h"
#include "fe300prci/fe300prci_driver.h"

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

/* The mtime RTC runs on a 32768kHz crystal. This means 1ms tick time */
#define RMP_RV32P_OSTIM_VAL         (33U)
/* What is the FPU type? */
#define RMP_RV32P_COP_RVF           (0U)
#define RMP_RV32P_COP_RVD           (0U)

/* Some register names */
#define MTIME                       (*((volatile uint64_t *)(CLINT_CTRL_ADDR + CLINT_MTIME)))
#define MTIMECMP                    (*((volatile uint64_t *)(CLINT_CTRL_ADDR + CLINT_MTIMECMP)))

/* Other low-level initialization stuff - clock and serial. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_START_HOOK). */
#define RMP_RV32P_LOWLVL_INIT() \
extern void _RMP_RV32P_Handler(void); \
plic_instance_t RMP_Global_PLIC; \
do \
{ \
    /* Initialize PLIC - all interrupts priority 0 */ \
    PLIC_init(&RMP_Global_PLIC,PLIC_CTRL_ADDR,PLIC_NUM_INTERRUPTS,PLIC_NUM_PRIORITIES); \
    RMP_Int_Disable(); \
    /* Initialize PLL to run the CPU at 256MHz (which is safer than 320MHz) */ \
    PRCI_use_pll(1, /* Use HFXTAL as the reference */ \
                 0, /* Bypass = 0, really use the PLL */ \
                 1, /* Set DIVR to divide-by-2 to get 8MHz frequency */ \
                 0x1F, /* Set DIVF to get 512MHz frequency */ \
                 1,    /* Set DIVQ to divide-by-2 to get 256 MHz frequency */ \
                 1, /* Set final divider to div-by-1 */ \
                 -1, /* We don't care about HFROSC */ \
                 -1); \
    /* Initialize trap vectors */ \
    _RMP_RV32P_MTVEC_Set((rmp_ptr_t)_RMP_RV32P_Handler); \
    \
    /* Initialize the serial port */ \
    /* Select IOF0 for UART0 RX & TX pins */ \
    GPIO_REG(GPIO_IOF_SEL)&=~IOF0_UART0_MASK; \
    GPIO_REG(GPIO_IOF_EN)|=IOF0_UART0_MASK; \
    /* Baud rate 115200 */ \
    UART0_REG(UART_REG_DIV)=256000000/115200-1; \
    UART0_REG(UART_REG_TXCTRL)|=UART_TXEN; \
    \
    /* Program the timer, and get rid of all pending interrupts */ \
    MTIMECMP=MTIME+RMP_RV32P_OSTIM_VAL; \
    PLIC_enable_interrupt(&RMP_Global_PLIC,INT_RTCCMP); \
    PLIC_set_priority(&RMP_Global_PLIC,INT_RTCCMP,1); \
    PLIC_complete_interrupt(&RMP_Global_PLIC, PLIC_claim_interrupt(&RMP_Global_PLIC)); \
} \
while(0)

/* Reprogram the timer and clear timer interrupt flags */
#define RMP_RV32P_TIM_CLR() \
do \
{ \
    plic_instance_t RMP_Global_PLIC; \
    \
    MTIMECMP=MTIME+RMP_RV32P_OSTIM_VAL; \
    RMP_Global_PLIC.base_addr=PLIC_CTRL_ADDR; \
    PLIC_complete_interrupt(&RMP_Global_PLIC,INT_RTCCMP); \
} \
while(0)

/* This is for debugging output */
#define RMP_RV32P_PUTCHAR(CHAR) \
do \
{ \
    while(UART0_REG(UART_REG_TXFIFO)&0x80000000U); \
    UART0_REG(UART_REG_TXFIFO)=(rmp_ptr_t)(CHAR); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
