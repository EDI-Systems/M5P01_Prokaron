/******************************************************************************
Filename   : platform_MSP430FR5994.h
Author     : pry
Date       : 24/06/2017
Licence    : LGPL v3+; see COPYING for details.
Description: The configuration file for MSP43FR5994.
******************************************************************************/

/* Defines *******************************************************************/
/* The TI-supplied headers */
#include <msp430.h>
#include "driverlib.h"

/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 16 is usually sufficient */
#define RMP_MAX_PREEMPT_PRIO         16
/* The maximum number of slices allowed */
#define RMP_MAX_SLICES               10000
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_MAX_NUM              1000
/* Are we using custom hooks? */
#define RMP_USE_HOOKS                RMP_FALSE
/* The stzck size of the init thread */
#define RMP_INIT_STACK_SIZE          128
/* The mask/unmask interrupt operations */
#define RMP_MASK_INT()               RMP_Disable_Int()
#define RMP_UNMASK_INT()             RMP_Enable_Int()

/* We are using MSP430X or not */
#define RMP_MSP430_X                 RMP_TRUE
/* The tick timer value */
#define RMP_MSP430_TICK_VAL          10000

/* Other low-level initialization stuff - clock and serial */
#define RMP_MSP430_LOW_LEVEL_INIT() \
do \
{ \
    struct Timer_A_initUpModeParam TIM0_Handle; \
    struct EUSCI_A_UART_initParam UART0_Handle; \
    /* Stop the WDT */ \
    WDTCTL = WDTPW | WDTHOLD; \
    /* Enable GPIO settings */ \
    PM5CTL0&=~LOCKLPM5; \
    /* FRAM 1 wait state */ \
    FRAMCtl_A_configureWaitStateControl(NWAITS_1); \
    /* Disable write protection since we have a MPU */ \
    FRAMCtl_A_disableWriteProtection(); \
    /* 32.768kHz input */ \
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ, \
                                               GPIO_PIN4+GPIO_PIN5, \
                                               GPIO_PRIMARY_MODULE_FUNCTION); \
    /* Set DCO frequency to 16 MHz */ \
    CS_setDCOFreq(CS_DCORSEL_1, CS_DCOFSEL_4); \
    /* Set SMCLK = DCO with frequency divider of 1 */ \
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1); \
    /* Set MCLK = DCO with frequency divider of 1 */ \
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1); \
    /* Set the timer 0 and its interrupts - timer 0 will be fully occupied by the OS */\
    TIM0_Handle.clockSource=TIMER_A_CLOCKSOURCE_SMCLK; \
    TIM0_Handle.clockSourceDivider=TIMER_A_CLOCKSOURCE_DIVIDER_16; \
    TIM0_Handle.timerPeriod=RMP_MSP430_TICK_VAL; \
    TIM0_Handle.timerInterruptEnable_TAIE=TIMER_A_TAIE_INTERRUPT_DISABLE; \
    TIM0_Handle.captureCompareInterruptEnable_CCR0_CCIE=TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE; \
    TIM0_Handle.timerClear=TIMER_A_SKIP_CLEAR; \
    TIM0_Handle.startTimer=1; \
    Timer_A_initUpMode(TA0_BASE,&TIM0_Handle); \
    Timer_A_enableCaptureCompareInterrupt(TA0_BASE, OFS_TAxCCTL1); \
    /* UART init */ \
    UART0_Handle.selectClockSource=EUSCI_A_UART_CLOCKSOURCE_SMCLK; \
    UART0_Handle.clockPrescalar=8; \
    UART0_Handle.firstModReg=10; \
    UART0_Handle.secondModReg=0xF7; \
    UART0_Handle.parity=EUSCI_A_UART_NO_PARITY; \
    UART0_Handle.msborLsbFirst=EUSCI_A_UART_LSB_FIRST; \
    UART0_Handle.numberofStopBits=EUSCI_A_UART_ONE_STOP_BIT; \
    UART0_Handle.uartMode=EUSCI_A_UART_MODE; \
    UART0_Handle.overSampling=EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION; \
    EUSCI_A_UART_init(EUSCI_A0_BASE, &UART0_Handle); \
    EUSCI_A_UART_enable(EUSCI_A0_BASE); \
    /* GPIO Init */ \
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0, GPIO_SECONDARY_MODULE_FUNCTION); \
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN1, GPIO_SECONDARY_MODULE_FUNCTION); \
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE,'\r'); \
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE,'\n'); \
} \
while(0)

/* Flag operations */
#define RMP_MSP430_PEND_SOFT_FLAG()  {TA0CCTL1|=CCIFG;}
#define RMP_MSP430_CLEAR_SOFT_FLAG() {TA0CCTL1&=~CCIFG;}
#define RMP_MSP430_CLEAR_TIMER_FLAG() {TA0CCTL0&=~CCIFG;}

/* This is for debugging output */
#define RMP_MSP430_PUTCHAR(CHAR) \
do \
{ \
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE,(CHAR)); \
} \
while(0)
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
