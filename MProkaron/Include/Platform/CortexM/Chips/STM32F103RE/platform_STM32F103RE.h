/******************************************************************************
Filename   : platform_STM32F103RE.h
Author     : pry
Date       : 24/06/2017
Licence    : LGPL v3+; see COPYING for details.
Description: The configuration file for STM32F103RE.
******************************************************************************/

/* Defines *******************************************************************/
/* The HAL library */
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "core_cm3.h"
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
/* The mask/unmask interrupt operations - can be adjusted to fit your own levels */
#define RMP_MASK_INT()               RMP_Mask_Int(0xFF)
#define RMP_UNMASK_INT()             RMP_Mask_Int(0x00)

/* What is the NVIC priority grouping? */
#define RMP_CMX_NVIC_GROUPING        RMP_CMX_NVIC_GROUPING_P2S6
/* What is the Systick value? */
#define RMP_CMX_SYSTICK_VAL          7200

/* Other low-level initialization stuff - clock and serial
 * STM32F10x APB1<36MHz, APB2<72MHz */
#define RMP_CMX_LOW_LEVEL_INIT() \
do \
{ \
    RCC_OscInitTypeDef Osc_Init; \
    RCC_ClkInitTypeDef Clk_Init; \
    UART_HandleTypeDef USART1_Handle; \
    GPIO_InitTypeDef GPIOA_Init; \
    HAL_Init(); \
    RMP_Clear(&Osc_Init, sizeof(RCC_OscInitTypeDef)); \
    RMP_Clear(&Clk_Init, sizeof(RCC_ClkInitTypeDef)); \
    RMP_Clear(&USART1_Handle, sizeof(UART_HandleTypeDef)); \
    RMP_Clear(&GPIOA_Init, sizeof(GPIO_InitTypeDef)); \
    \
    /* Enable HSE Oscillator and activate PLL with HSE as source */ \
    Osc_Init.OscillatorType=RCC_OSCILLATORTYPE_HSE; \
    Osc_Init.HSEState=RCC_HSE_ON; \
    Osc_Init.HSEPredivValue=RCC_HSE_PREDIV_DIV1; \
    Osc_Init.PLL.PLLState=RCC_PLL_ON; \
    Osc_Init.PLL.PLLSource=RCC_PLLSOURCE_HSE; \
    Osc_Init.PLL.PLLMUL=RCC_PLL_MUL9; \
    RMP_ASSERT(HAL_RCC_OscConfig(&Osc_Init)==HAL_OK); \
    \
    /* Secect PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */ \
    Clk_Init.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2); \
    Clk_Init.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK; \
    Clk_Init.AHBCLKDivider=RCC_SYSCLK_DIV1; \
    Clk_Init.APB2CLKDivider=RCC_HCLK_DIV1; \
    Clk_Init.APB1CLKDivider=RCC_HCLK_DIV2; \
    RMP_ASSERT(HAL_RCC_ClockConfig(&Clk_Init, FLASH_LATENCY_2)==HAL_OK); \
     __HAL_FLASH_PREFETCH_BUFFER_ENABLE(); \
    \
    __HAL_RCC_GPIOA_CLK_ENABLE(); \
	__HAL_RCC_USART1_CLK_ENABLE(); \
    GPIOA_Init.Mode=GPIO_MODE_AF_PP; \
    GPIOA_Init.Pin=GPIO_PIN_9; \
    GPIOA_Init.Speed=GPIO_SPEED_FREQ_HIGH; \
    HAL_GPIO_Init(GPIOA, &GPIOA_Init); \
    \
    USART1_Handle.Instance=USART1; \
    USART1_Handle.Init.BaudRate=115200; \
    USART1_Handle.Init.WordLength=UART_WORDLENGTH_8B; \
    USART1_Handle.Init.StopBits=UART_STOPBITS_1; \
    USART1_Handle.Init.Parity=UART_PARITY_NONE; \
    USART1_Handle.Init.HwFlowCtl=UART_HWCONTROL_NONE; \
    USART1_Handle.Init.Mode=UART_MODE_TX; \
    HAL_UART_Init(&USART1_Handle); \
    RMP_CMX_PUTCHAR('\r'); \
    RMP_CMX_PUTCHAR('\n'); \
    /* Enable all fault handlers */ \
    SCB->SHCSR|=RMP_CMX_SHCSR_USGFAULTENA|RMP_CMX_SHCSR_BUSFAULTENA|RMP_CMX_SHCSR_MEMFAULTENA; \
    \
    /* Set the priority of timer, svc and faults to the lowest */ \
    NVIC_SetPriorityGrouping(RMP_CMX_NVIC_GROUPING); \
    NVIC_SetPriority(SVCall_IRQn, 0xFF); \
    NVIC_SetPriority(PendSV_IRQn, 0xFF); \
    NVIC_SetPriority(SysTick_IRQn, 0xFF); \
    NVIC_SetPriority(BusFault_IRQn, 0xFF); \
    NVIC_SetPriority(UsageFault_IRQn, 0xFF); \
    NVIC_SetPriority(DebugMonitor_IRQn, 0xFF); \
    SysTick_Config(RMP_CMX_SYSTICK_VAL); \
} \
while(0)

/* This is for debugging output */
#define RMP_CMX_PUTCHAR(CHAR) \
do \
{ \
    USART1->DR=CHAR; \
    while((USART1->SR&0x40)==0); \
} \
while(0)
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
