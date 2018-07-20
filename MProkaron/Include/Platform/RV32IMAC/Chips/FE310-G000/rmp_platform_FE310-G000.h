/******************************************************************************
Filename    : rmp_platform_FE310-G000.h
Author      : pry
Date        : 24/06/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The configuration file for FE310-G000.
******************************************************************************/

/* Defines *******************************************************************/
/* The HAL library */
#include "encoding.h"
#include "platform.h"
#include "plic/plic_driver.h"
#include "fe300prci/fe300prci_driver.h"

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
/* The mask/unmask interrupt operations */
#define RMP_MASK_INT()               RMP_Disable_Int()
#define RMP_UNMASK_INT()             RMP_Enable_Int()

#define RMP_RV32IMAC_INT_NUMBER      PLIC_NUM_INTERRUPTS
/* The mtime RTC runs on a 32768kHz crystal. This means 1ms tick time */
#define RMP_RISCV_TICK_COUNT         33

/* Some register names */
#define MTIME                        (*((volatile uint64_t *)(CLINT_CTRL_ADDR + CLINT_MTIME)))
#define MTIMECMP                     (*((volatile uint64_t *)(CLINT_CTRL_ADDR + CLINT_MTIMECMP)))

/* Other low-level initialization stuff - clock and serial */
#define RMP_RV32IMAC_LOW_LEVEL_INIT() \
rmp_cnt_t Count; \
plic_instance_t RMP_Global_PLIC; \
do \
{ \
    /* Initialize PLIC */ \
    PLIC_init(&RMP_Global_PLIC,PLIC_CTRL_ADDR,PLIC_NUM_INTERRUPTS,PLIC_NUM_PRIORITIES); \
    RMP_Disable_Int(); \
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
    _RMP_Set_MTVEC((rmp_ptr_t)Interrupt_Handler); \
    \
    /* Initialize the serial port */ \
    /* Select IOF0 for UART0 RX & TX pins */ \
    GPIO_REG(GPIO_IOF_SEL)&=~IOF0_UART0_MASK; \
    GPIO_REG(GPIO_IOF_EN)|=IOF0_UART0_MASK; \
    /* Baud rate 115200 */ \
    UART0_REG(UART_REG_DIV)=256000000/115200-1; \
    UART0_REG(UART_REG_TXCTRL)|=UART_TXEN; \
    \
    /* Clean up the interrupt vector table */ \
    for(Count=0;Count<PLIC_NUM_INTERRUPTS;Count++) \
        RMP_Periph_Vect_Table[Count]=0; \
    \
    /* Program the timer */ \
    MTIMECMP=MTIME+RMP_RISCV_TICK_COUNT; \
} \
while(0)

/* This is for debugging output */
#define RMP_RV32IMAC_PUTCHAR(CHAR) \
do \
{ \
    while(UART0_REG(UART_REG_TXFIFO)&0x80000000); \
    UART0_REG(UART_REG_TXFIFO)=(rmp_ptr_t)(CHAR); \
} \
while(0)
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
