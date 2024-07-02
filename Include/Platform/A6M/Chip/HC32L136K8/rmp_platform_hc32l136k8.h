/******************************************************************************
Filename   : rmp_platform_hc32l163k8.h
Author     : pry
Date       : 24/06/2017
Licence    : LGPL v3+; see COPYING for details.
Description: The configuration file for HC32L136K8.
******************************************************************************/

/* Define ********************************************************************/
/* The HAL library */
#include "ddl.h"
#include "core_cm0plus.h"
#include "uart.h"
#include "gpio.h"
#include "bt.h"
#include "timer3.h"

/* Debugging */
#define RMP_ASSERT_ENABLE           (1U)
#define RMP_CHECK_ENABLE            (1U)

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

/* What is the Systick value? */
#define RMP_A6M_SYSTICK_VAL         (24000U)

/* Other low-level initialization stuff - clock and serial
 * This device requires that you switch from low to high frequency.
 * The manufacturer have defined their own delay functions - do not use
 * them, because they are not OS agnostic and will reprogram the systick 
 * to something that you possibly don't want. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_START_HOOK). */
#define RMP_A6M_LOWLVL_INIT() \
do \
{ \
    stc_gpio_cfg_t GPIO_Init_Cfg; \
    stc_uart_cfg_t UART_Init_Cfg; \
    DDL_ZERO_STRUCT(GPIO_Init_Cfg); \
    DDL_ZERO_STRUCT(UART_Init_Cfg); \
    \
    /* Switch to the target clock frequency */ \
    Sysctrl_SetRCLTrim(SysctrlRclFreq32768); \
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, TRUE); \
    Sysctrl_SysClkSwitch(SysctrlClkRCL); \
    Sysctrl_SetRCHTrim(SysctrlRchFreq24MHz); \
	Sysctrl_SysClkSwitch(SysctrlClkRCH); \
    \
    /* Turn off the low speed RC oscillator and update core clock */ \
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, FALSE); \
    SystemCoreClockUpdate(); \
    \
    /* Initialize I/O and serial */ \
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); \
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart0, TRUE); \
    \
    GPIO_Init_Cfg.enDir=GpioDirOut; \
    Gpio_Init(GpioPortA,GpioPin9,&GPIO_Init_Cfg); \
    Gpio_SetAfMode(GpioPortA,GpioPin9,GpioAf1); \
    \
    UART_Init_Cfg.enRunMode=UartMskMode3; \
    UART_Init_Cfg.enMmdorCk=UartMskEven; \
    UART_Init_Cfg.enStopBit=UartMsk1bit; \
    UART_Init_Cfg.stcBaud.enClkDiv=UartMsk8Or16Div; \
    UART_Init_Cfg.stcBaud.u32Pclk=Sysctrl_GetPClkFreq(); \
    UART_Init_Cfg.stcBaud.u32Baud=115200; \
    Uart_Init(M0P_UART0, &UART_Init_Cfg); \
    \
    /* Set the priority of timer, svc and faults to the lowest */ \
    NVIC_SetPriority(SVC_IRQn, 0xFF); \
    NVIC_SetPriority(PendSV_IRQn, 0xFF); \
    NVIC_SetPriority(SysTick_IRQn, 0xFF); \
    \
    /* Configure systick */ \
    SysTick_Config(RMP_A6M_SYSTICK_VAL); \
} \
while(0)

/* This is for debugging output */
#define RMP_A6M_PUTCHAR(CHAR)           Uart_SendDataPoll(M0P_UART0,CHAR)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
