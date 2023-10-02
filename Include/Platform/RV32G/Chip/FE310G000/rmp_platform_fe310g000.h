/******************************************************************************
Filename    : rmp_platform_fe310g000.h
Author      : pry
Date        : 24/06/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The configuration file for FE310-G000 RISC-V chip.
******************************************************************************/

/* Defines *******************************************************************/
/* The HAL library */
#include "encoding.h"
#include "platform.h"
#include "plic/plic_driver.h"
#include "fe300prci/fe300prci_driver.h"

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

#define RMP_RV32G_INT_NUMBER     	PLIC_NUM_INTERRUPTS
/* The mtime RTC runs on a 32768kHz crystal. This means 1ms tick time */
#define RMP_RV32G_TICK_COUNT        (33U)
#define RMP_RV32G_TICK_MCAUSE       (7U)
#define RMP_RV32G_SWITCH_MCAUSE     (3U)

/* Some register names */
#define MTIME                       (*((volatile uint64_t *)(CLINT_CTRL_ADDR + CLINT_MTIME)))
#define MTIMECMP                    (*((volatile uint64_t *)(CLINT_CTRL_ADDR + CLINT_MTIMECMP)))

/* Other low-level initialization stuff - clock and serial. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_Start_Hook). */
#define RMP_RV32G_LOWLVL_INIT() \
extern void _RMP_RV32G_Handler(void); \
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
    _RMP_MTVEC_Set((rmp_ptr_t)_RMP_RV32G_Handler); \
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
    MTIMECMP=MTIME+RMP_RV32G_TICK_COUNT; \
    PLIC_enable_interrupt(&RMP_Global_PLIC,INT_RTCCMP); \
    PLIC_set_priority(&RMP_Global_PLIC,INT_RTCCMP,1); \
	PLIC_complete_interrupt(&RMP_Global_PLIC, PLIC_claim_interrupt(&RMP_Global_PLIC)); \
} \
while(0)

/* Reprogram the timer and clear timer interrupt flags */
#define RMP_RV32G_TICK_RESET() \
do \
{ \
	plic_instance_t RMP_Global_PLIC; \
    \
    MTIMECMP=MTIME+RMP_RV32G_TICK_COUNT; \
    RMP_Global_PLIC.base_addr=PLIC_CTRL_ADDR; \
    PLIC_complete_interrupt(&RMP_Global_PLIC,INT_RTCCMP); \
} \
while(0)

/* Trigger/clear software interrupt */
#define RMP_RV32G_SWITCH_SET()          (CLINT_REG(CLINT_MSIP)=0xFFFFFFFF)
#define RMP_RV32G_SWITCH_CLR()          (CLINT_REG(CLINT_MSIP)=0)

/* Peripheral handler hook */
#define RMP_RV32G_PERIPH_HANDLER()		WDT_Interrupt()

/* This is for debugging output */
#define RMP_RV32G_PUTCHAR(CHAR) \
do \
{ \
    while(UART0_REG(UART_REG_TXFIFO)&0x80000000U); \
    UART0_REG(UART_REG_TXFIFO)=(rmp_ptr_t)(CHAR); \
} \
while(0)

extern void WDT_Interrupt(void);
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
