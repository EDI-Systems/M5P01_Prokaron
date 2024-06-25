/******************************************************************************
Filename   : rmp_platform_msp430f149.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for MSP430F149.
             This port is hardly useful due to severe RAM restrictions, but it
             is an existence proof of how small the RMP's footprint can be.
******************************************************************************/

/* Define ********************************************************************/
/* The TI-supplied headers */
#include "msp430f149.h"

/* Debugging */
#define RMP_ASSERT_ENABLE           (1U)
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 16 is usually sufficient */
#define RMP_PREEMPT_PRIO_NUM        (16U)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (10000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX             (1000U)
/* The stack size of the init thread */
#define RMP_INIT_STACK_SIZE         (128U)

/* GUI */
#define RMP_GUI_ENABLE              (0U)
/* Anti-aliasing */
#define RMP_GUI_ANTIALIAS_ENABLE    (0U)
/* Widgets */
#define RMP_GUI_WIDGET_ENABLE       (0U)

/* The tick timer value */
#define RMP_MSP430_TICK_VAL         (8000U)
/* We are using MSP430X or not */
#define RMP_MSP430_COP_430X         (0U)

/* Other low-level initialization stuff - clock and serial. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_START_HOOK). */
#define RMP_MSP430_LOWLVL_INIT() \
do \
{ \
    /* Stop the WDT */ \
    WDTCTL=WDTPW+WDTHOLD; \
    P6DIR|=BIT2; \
    P6OUT|=BIT2; \
    /* CPU @ 8MHz on external OSC */ \
    BCSCTL1=0U; \
    /* There is no OSC ready flag, so we'll have to resort to interrupt flags, hence the delay */ \
    do \
    { \
        rmp_ptr_t Delay; \
        IFG1&=~OFIFG; \
        for(Delay=0xFF;Delay>0;Delay--); \
    } \
    while((IFG1&OFIFG)!=0U); \
    BCSCTL2=SELM_2|SELS; \
    /* Enable TIMA for interrupts - use SMCLK */ \
    TACCR0=RMP_MSP430_TICK_VAL; \
    TACTL=TASSEL_2|MC_1; \
    TACCTL0=CCIE; \
    /* Enable serial & GPIO - P3.4,5 = USART0 TXD/RXD */ \
    P3SEL|=0x30U; \
    ME1|=UTXE0|URXE0; \
    UCTL0|=CHAR; \
    UTCTL0|=SSEL1; \
    UBR00=0x45U; \
    UBR10=0x00U; \
    UMCTL0=0x00U; \
    UCTL0&=~SWRST; \
} \
while(0)

#define RMP_MSP430_TIM_CLR()        (TACCTL0&=~CCIFG)

/* This is for debugging output */
#define RMP_MSP430_PUTCHAR(CHAR) \
do \
{ \
    while((IFG1&UTXIFG0)==0U); \
    TXBUF0=(rmp_u8_t)(CHAR); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
