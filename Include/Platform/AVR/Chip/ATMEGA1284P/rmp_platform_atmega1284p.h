/******************************************************************************
Filename   : rmp_platform_atmega1284p.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for ATMEGA1284P.
******************************************************************************/

/* Define ********************************************************************/
/* The AVR I/O library */
#include "avr/io.h"

/* Debugging */
#define RMP_ASSERT_CORRECT          (0U)
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 16 is usually sufficient */
#define RMP_PREEMPT_PRIO_NUM        (16U)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (10000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX             (1000U)
/* Are we using custom hooks? */
#define RMP_HOOK_EXTRA              (0U)
/* The stack size of the init thread */
#define RMP_INIT_STACK_SIZE         (256U)
/* The mask/unmask interrupt operations */
#define RMP_INT_MASK()              RMP_Int_Disable()
#define RMP_INT_UNMASK()            RMP_Int_Enable()

/* What is the Systick value? 50U = 12800 cycles = 0.8ms */
#define RMP_AVR_TICK_VAL            (50U)
/* Does the chip have RAMP, EIND, and is it XMEGA? */
#define RMP_AVR_COP_RAMP            (1U)
#define RMP_AVR_COP_EIND            (0U)
#define RMP_AVR_COP_XMEGA           (0U)

/* Other low-level initialization stuff - clock and serial.
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_Start_Hook). */
#define RMP_AVR_LOWLVL_INIT() \
do \
{ \
    /* USART0 TX pin - PD1 */ \
    DDRD=0x02U; \
    /* USART0 - double speed, TX only, 115200-8-N-1 */ \
    UCSR0A=0x02U; \
    UCSR0B=0x08U; \
    UCSR0C=0x06U; \
    UBRR0=16U; \
    /* Timer 0 - CTC mode, UP counter, prescaler 256 */ \
    TCNT0=0x00U; \
    OCR0A=RMP_AVR_TICK_VAL; \
    TIFR0=0x00U; \
    TCCR0A=0x02U; \
    TCCR0B=0x04U; \
    TIMSK0=0x02U; \
} \
while(0)

#define RMP_AVR_TIM_CLR()           (TIFR0=0x00U)

/* This is for debugging output */
#define RMP_AVR_PUTCHAR(CHAR) \
do \
{ \
    /* Wait for transmit buffer to be empty */ \
    while((UCSR0A&0x20U)==0U); \
    UDR0=(CHAR); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
