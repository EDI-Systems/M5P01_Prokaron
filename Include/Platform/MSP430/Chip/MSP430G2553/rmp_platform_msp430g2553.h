/******************************************************************************
Filename   : rmp_platform_msp430g2553.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for MSP430G2553.
             This port is entirely ususable due to impossible RAM restrictions,
             it merely an proof of how low the power footprint of RMP can be.
******************************************************************************/

/* Define ********************************************************************/
/* The TI-supplied headers */
#include "msp430g2553.h"

/* Debugging */
#define RMP_ASSERT_ENABLE           (1U)
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 16 is usually sufficient */
#define RMP_PREEMPT_PRIO_NUM        (16U)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (30000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX             (30000U)
/* The stzck size of the init thread */
#define RMP_INIT_STACK_SIZE         (64U)
/* Custom hooks */
extern void RMP_Start_Hook(void);
extern void RMP_Sched_Hook(void);
extern void RMP_Dly_Hook(rmp_u16_t Slice);
extern void RMP_Tim_Hook(rmp_u16_t Slice);
#define RMP_START_HOOK()            RMP_Start_Hook()
#define RMP_SCHED_HOOK()            RMP_Sched_Hook()
#define RMP_DLY_HOOK(SLICE)         RMP_Dly_Hook(Slice)
#define RMP_TIM_HOOK(SLICE)         RMP_Tim_Hook(Slice)

/* We are using MSP430X or not */
#define RMP_MSP430_COP_430X         (0U)

/* Other low-level initialization stuff - clock and serial. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_Start_Hook). */
#define RMP_MSP430_LOWLVL_INIT() \
do \
{ \
    /* Stop the WDT */ \
    WDTCTL=WDTPW+WDTHOLD; \
    /* Configure the CPU to 16MHz with DCO, ACLK DIV 8 */ \
    while(CALBC1_16MHZ==0xFF); \
    DCOCTL=0U; \
    BCSCTL1=CALBC1_16MHZ|DIVA_3; \
    DCOCTL=CALDCO_16MHZ; \
    /* Enable serial and attach to SMCLK */ \
    P1SEL=BIT2; \
    P1SEL2=BIT2; \
    UCA0CTL1=UCSSEL_2|UCSWRST; \
    UCA0BR0=139U; \
    UCA0BR1=0U; \
    UCA0MCTL=UCBRS2|UCBRS0; \
    UCA0CTL1&=~UCSWRST; \
} \
while(0)

#define RMP_MSP430_TIM_CLR()        (TACCTL0&=~CCIFG)

/* This is for debugging output */
#define RMP_MSP430_PUTCHAR(CHAR) \
do \
{ \
    while((IFG2&UCA0TXIFG)==0U); \
    UCA0TXBUF=(CHAR); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
