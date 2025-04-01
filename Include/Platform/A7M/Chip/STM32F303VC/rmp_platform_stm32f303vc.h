/******************************************************************************
Filename   : rmp_platform_stm32f303vc.h
Author     : zzx
Date       : 30/03/2025
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for STM32F303VC.
******************************************************************************/

/* Define ********************************************************************/
/* The HAL library */
#include "stm32f3xx.h"
#include "core_cm4.h"
#include "stm32f3xx_hal.h"

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

/* What is the NVIC priority grouping? */
#define RMP_A7M_NVIC_GROUPING       RMP_A7M_NVIC_GROUPING_P2S6
/* What is the Systick value? */
#define RMP_A7M_SYSTICK_VAL         (7200U)
/* What is the interrupt masking level? */
#define RMP_A7M_INT_MASK_LVL        (0xFFU)
/* What is the FPU type? */
#define RMP_A7M_COP_FPV4_SP         (1U)
#define RMP_A7M_COP_FPV5_SP         (0U)
#define RMP_A7M_COP_FPV5_DP         (0U)

/* Other low-level initialization stuff - clock and serial
 * STM32F303 APB1<36MHz, APB2<72MHz.
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_START_HOOK). */
#define RMP_A7M_LOWLVL_INIT() \
do \
{ \
    RCC_ClkInitTypeDef RCC_ClkInitStruct; \
    RCC_OscInitTypeDef RCC_OscInitStruct; \
    UART_HandleTypeDef UART1_Handle; \
    GPIO_InitTypeDef GPIO_Init; \
	HAL_Init(); \
    RMP_Clear(&RCC_ClkInitStruct, sizeof(RCC_ClkInitStruct)); \
    RMP_Clear(&RCC_OscInitStruct, sizeof(RCC_OscInitStruct)); \
    RMP_Clear(&UART1_Handle, sizeof(UART_HandleTypeDef)); \
    RMP_Clear(&GPIO_Init, sizeof(GPIO_InitTypeDef)); \
    \
    /* Enable HSE Oscillator and activate PLL with HSE as source */ \
    RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_HSE; \
    RCC_OscInitStruct.HSEState=RCC_HSE_ON; \
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1; \
    RCC_OscInitStruct.PLL.PLLState=RCC_PLL_ON; \
    RCC_OscInitStruct.PLL.PLLSource=RCC_PLLSOURCE_HSE; \
    RCC_OscInitStruct.PLL.PLLMUL=RCC_PLL_MUL9; \
    HAL_RCC_OscConfig(&RCC_OscInitStruct); \
    \
    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */ \
    RCC_ClkInitStruct.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2); \
    RCC_ClkInitStruct.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK; \
    RCC_ClkInitStruct.AHBCLKDivider=RCC_SYSCLK_DIV1; \
    RCC_ClkInitStruct.APB1CLKDivider=RCC_HCLK_DIV2; \
    RCC_ClkInitStruct.APB2CLKDivider=RCC_HCLK_DIV1; \
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2); \
	__HAL_FLASH_PREFETCH_BUFFER_ENABLE(); \
    /* Enable USART 1 for user-level operations */ \
    /* Clock enabling */ \
    __HAL_RCC_GPIOC_CLK_ENABLE(); \
    __HAL_RCC_USART1_CLK_ENABLE(); \
    /* UART IO initialization */ \
    GPIO_Init.Pin=GPIO_PIN_4; \
    GPIO_Init.Mode=GPIO_MODE_AF_PP; \
    GPIO_Init.Pull=GPIO_PULLUP; \
    GPIO_Init.Speed=GPIO_SPEED_FREQ_HIGH; \
    GPIO_Init.Alternate=GPIO_AF7_USART1; \
    HAL_GPIO_Init(GPIOC, &GPIO_Init); \
    /* UART initialization */ \
    UART1_Handle.Instance=USART1; \
    UART1_Handle.Init.BaudRate=115200; \
    UART1_Handle.Init.WordLength=UART_WORDLENGTH_8B; \
    UART1_Handle.Init.StopBits=UART_STOPBITS_1; \
    UART1_Handle.Init.Parity=UART_PARITY_NONE; \
    UART1_Handle.Init.HwFlowCtl=UART_HWCONTROL_NONE; \
    UART1_Handle.Init.Mode=UART_MODE_TX; \
    HAL_UART_Init(&UART1_Handle); \
    RMP_A7M_PUTCHAR('\r'); \
    RMP_A7M_PUTCHAR('\n'); \
    \
    /* Enable all fault handlers */ \
    SCB->SHCSR|=RMP_A7M_SHCSR_USGFAULTENA|RMP_A7M_SHCSR_BUSFAULTENA|RMP_A7M_SHCSR_MEMFAULTENA; \
    \
    /* Set the priority of timer, svc and faults to the lowest */ \
    NVIC_SetPriorityGrouping(RMP_A7M_NVIC_GROUPING); \
    NVIC_SetPriority(SVCall_IRQn, 0xFF); \
    NVIC_SetPriority(PendSV_IRQn, 0xFF); \
    NVIC_SetPriority(SysTick_IRQn, 0xFF); \
    NVIC_SetPriority(BusFault_IRQn, 0xFF); \
    NVIC_SetPriority(UsageFault_IRQn, 0xFF); \
    NVIC_SetPriority(DebugMonitor_IRQn, 0xFF); \
    SysTick_Config(RMP_A7M_SYSTICK_VAL); \
} \
while(0)

/* This is for debugging output */
#define RMP_A7M_PUTCHAR(CHAR) \
do \
{ \
    USART1->TDR=(rmp_ptr_t)(CHAR); \
    while((USART1->ISR&0x40U)==0U); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
