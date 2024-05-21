/******************************************************************************
Filename   : rmp_platform_msp430fr5994.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for MSP430FR5994.
******************************************************************************/

/* Define ********************************************************************/
/* The TI-supplied headers */
#include "msp430fr5994.h"
#include "driverlib.h"

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
/* The stzck size of the init thread */
#define RMP_INIT_STACK_SIZE         (128U)
/* The mask/unmask interrupt operations */
#define RMP_INT_MASK()              RMP_Int_Disable()
#define RMP_INT_UNMASK()            RMP_Int_Enable()

/* The tick timer value */
#define RMP_MSP430_TICK_VAL         (10000U)
/* We are using MSP430X or not */
#define RMP_MSP430_COP_430X         (1U)

/* Other low-level initialization stuff - clock and serial. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_Start_Hook). */
#define RMP_MSP430_LOWLVL_INIT() \
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
    /* Set the TIMA0 and its interrupts */ \
    TIM0_Handle.clockSource=TIMER_A_CLOCKSOURCE_SMCLK; \
    TIM0_Handle.clockSourceDivider=TIMER_A_CLOCKSOURCE_DIVIDER_16; \
    TIM0_Handle.timerPeriod=RMP_MSP430_TICK_VAL; \
    TIM0_Handle.timerInterruptEnable_TAIE=TIMER_A_TAIE_INTERRUPT_DISABLE; \
    TIM0_Handle.captureCompareInterruptEnable_CCR0_CCIE=TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE; \
    TIM0_Handle.timerClear=TIMER_A_SKIP_CLEAR; \
    TIM0_Handle.startTimer=1; \
    Timer_A_initUpMode(TA0_BASE,&TIM0_Handle); \
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

#define RMP_MSP430_TIM_CLR()        {TA0CCTL0&=~CCIFG;}

/* This is for debugging output */
#define RMP_MSP430_PUTCHAR(CHAR) \
do \
{ \
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE,(CHAR)); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
