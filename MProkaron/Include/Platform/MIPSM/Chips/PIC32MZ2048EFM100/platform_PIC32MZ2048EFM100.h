/******************************************************************************
Filename   : platform_PIC32MZ2048EFM100.h
Author     : pry
Date       : 24/06/2017
Licence    : LGPL v3+; see COPYING for details.
Description: The configuration file for PIC32MZ2048EFM100.
******************************************************************************/

/* Defines *******************************************************************/
/* The MPLAB library */
#include <xc.h>
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

/* What is the NVIC priority grouping? */
/* Which timer to use for ticking? */
#define RMP_MIPSM_TICK_TIMER         3
/* What is the tick timer value? */
#define RMP_MIPSM_TICK_VAL           20000

/* Other low-level initialization stuff - clock and serial */
#define RMP_MIPSM_LOW_LEVEL_INIT() \
do \
{ \
    /* set PBCLK2 to deliver 40Mhz clock for PMP/I2C/UART/SPI. */ \
	SYSKEY=0xAA996655UL; \
	SYSKEY=0x556699AAUL; \
	/* 200MHz/5=40MHz */ \
	PB2DIVbits.PBDIV=0b100; \
	/* Timers use clock PBCLK3, set this to 40MH. */ \
	PB3DIVbits.PBDIV=0b100; \
	/* Ports use PBCLK4 */ \
	PB4DIVbits.PBDIV=0b000; \
	SYSKEY=0; \
	/* Disable interrupts */ \
	__builtin_disable_interrupts(); \
	/* Enable multi-vector interrupts. */ \
	_CP0_BIS_CAUSE(0x00800000U); \
	INTCONSET=_INTCON_MVEC_MASK; \
	__builtin_enable_interrupts(); \
    /* We always use the timer 1 for interrupts */ \
} \
while(0)

/* This is for debugging output */
#define RMP_MIPSM_PUTCHAR(CHAR) \
do \
{ \
} \
while(0)
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
