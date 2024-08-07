/******************************************************************************
Filename   : rmp_platform_stm32f030f4.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for STM32F030F4.
******************************************************************************/

/* Define ********************************************************************/
/* The HAL library */
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_conf.h"
#include "core_cm0.h"

/* Debugging */
/* Assertion */
#define RMP_ASSERT_ENABLE           (1U)
/* Invalid parameter checking */
#define RMP_CHECK_ENABLE            (1U)
/* Debug logging */
#define RMP_DBGLOG_ENABLE           (1U)

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
#define RMP_A6M_SYSTICK_VAL         (3600U)

/* Other low-level initialization stuff - clock and serial
 * STM32F03x APB1<48MHz, APB2<48MHz.
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_START_HOOK). */
#define RMP_A6M_LOWLVL_INIT() \
do \
{ \
    RCC_OscInitTypeDef RCC_OscInitStruct; \
    RCC_ClkInitTypeDef RCC_ClkInitStruct; \
    GPIO_InitTypeDef GPIO_Init; \
    UART_HandleTypeDef UART1_Handle; \
    RMP_Clear(&RCC_OscInitStruct, sizeof(RCC_OscInitTypeDef)); \
    RMP_Clear(&RCC_ClkInitStruct, sizeof(RCC_ClkInitTypeDef)); \
    RMP_Clear(&GPIO_Init, sizeof(GPIO_InitTypeDef)); \
    RMP_Clear(&UART1_Handle, sizeof(UART_HandleTypeDef)); \
    \
    /* Activate PLL with HSI/2 as source */ \
    RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_NONE; \
    RCC_OscInitStruct.PLL.PLLState=RCC_PLL_ON; \
    RCC_OscInitStruct.PLL.PLLSource=RCC_PLLSOURCE_HSI; \
    RCC_OscInitStruct.PLL.PREDIV=RCC_PREDIV_DIV1; \
    RCC_OscInitStruct.PLL.PLLMUL=RCC_PLL_MUL12; \
    RMP_ASSERT(HAL_RCC_OscConfig(&RCC_OscInitStruct)==HAL_OK); \
    /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */ \
    RCC_ClkInitStruct.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1); \
    RCC_ClkInitStruct.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK; \
    RCC_ClkInitStruct.AHBCLKDivider=RCC_SYSCLK_DIV1; \
    RCC_ClkInitStruct.APB1CLKDivider=RCC_HCLK_DIV1; \
    RMP_ASSERT(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)==HAL_OK); \
    \
    /* Enable USART 1 for user-level operations */ \
    /* Clock enabling */ \
    __HAL_RCC_GPIOA_CLK_ENABLE(); \
    __HAL_RCC_USART1_CLK_ENABLE(); \
    /* UART IO initialization */ \
    GPIO_Init.Pin=GPIO_PIN_9; \
    GPIO_Init.Mode=GPIO_MODE_AF_PP; \
    GPIO_Init.Pull=GPIO_PULLUP; \
    GPIO_Init.Speed=GPIO_SPEED_FREQ_HIGH; \
    GPIO_Init.Alternate=GPIO_AF1_USART1; \
    HAL_GPIO_Init(GPIOA,&GPIO_Init); \
    /* UART initialization */ \
    UART1_Handle.Instance=USART1; \
    UART1_Handle.Init.BaudRate=115200U; \
    UART1_Handle.Init.WordLength=UART_WORDLENGTH_8B; \
    UART1_Handle.Init.StopBits=UART_STOPBITS_1; \
    UART1_Handle.Init.Parity=UART_PARITY_NONE; \
    UART1_Handle.Init.HwFlowCtl=UART_HWCONTROL_NONE; \
    UART1_Handle.Init.Mode=UART_MODE_TX; \
    HAL_UART_Init(&UART1_Handle); \
    RMP_A6M_PUTCHAR('\r'); \
    RMP_A6M_PUTCHAR('\n'); \
    \
    /* Set the priority of timer, svc and faults to the lowest */ \
    NVIC_SetPriority(SVC_IRQn, 0xFFU); \
    NVIC_SetPriority(PendSV_IRQn, 0xFFU); \
    NVIC_SetPriority(SysTick_IRQn, 0xFFU); \
    /* Configure systick */ \
    SysTick_Config(RMP_A6M_SYSTICK_VAL); \
} \
while(0)

/* This is for debugging output */
#define RMP_A6M_PUTCHAR(CHAR) \
do \
{ \
    USART1->TDR=CHAR; \
    while((USART1->ISR&0x40U)==0U); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
