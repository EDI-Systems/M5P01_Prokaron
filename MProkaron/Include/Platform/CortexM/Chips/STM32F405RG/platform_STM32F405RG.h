/******************************************************************************
Filename   : platform_STM32F767IG.h
Author     : pry
Date       : 24/06/2017
Licence    : LGPL v3+; see COPYING for details.
Description: The configuration file for STM32F767IG.
******************************************************************************/

/* Defines *******************************************************************/
/* The HAL library */
#include "stm32f4xx.h"
#include "core_cm4.h"
#include "stm32f4xx.h"
/* The virtual memory start address for the kernel objects */
#define RME_KMEM_VA_START            0x10002000
/* The size of the kernel object virtual memory */
#define RME_KMEM_SIZE                0x6000
/* The granularity of kernel memory allocation, in bytes */
#define RME_KMEM_SLOT_ORDER          4
/* Kernel stack size and address */
#define RME_KMEM_STACK_ADDR          0x10000FF0
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 32 is usually sufficient */
#define RME_MAX_PREEMPT_PRIO         32

/* Initial kenel object frontier limit */
#define RME_CMX_KMEM_BOOT_FRONTIER   0x10003000
/* Number of MPU regions available */
#define RME_CMX_MPU_REGIONS          8
/* Init process's first thread's entry point address */
#define RME_CMX_INIT_ENTRY           (0x08004000|0x01)
/* Init process's first thread's stack address */
#define RME_CMX_INIT_STACK           0x2001FFF0
/* What is the FPU type? */
#define RME_CMX_FPU_TYPE             RME_CMX_FPU_VFPV4
/* What is the NVIC priority grouping? */
#define RME_CMX_NVIC_GROUPING        RME_CMX_NVIC_GROUPING_P2S6
/* What is the Systick value? */
#define RME_CMX_SYSTICK_VAL          16800

/* Other low-level initialization stuff - The serial port */
#define RME_CMX_LOW_LEVEL_INIT() \
do \
{ \
    \
} \
while(0)

/* This is for debugging output */
#define RME_CMX_PUTCHAR(CHAR) \
do \
{ \
    ITM_SendChar((s8)(CHAR)); \
} \
while(0)
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
