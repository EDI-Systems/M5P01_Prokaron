/******************************************************************************
Filename   : rmp_platform_stm32f405rg.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for STM32F405RG.
******************************************************************************/

/* Define ********************************************************************/
/* The HAL library */
#include "stm32f4xx.h"
#include "core_cm4.h"
#include "stm32f4xx_hal.h"

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

/* What is the NVIC priority grouping? */
#define RMP_A7M_NVIC_GROUPING       RMP_A7M_NVIC_GROUPING_P2S6
/* What is the Systick value? */
#define RMP_A7M_SYSTICK_VAL         (16800U)
/* What is the interrupt masking level? */
#define RMP_A7M_INT_MASK_LVL        (0xFFU)
/* What is the FPU type? */
#define RMP_A7M_COP_FPV4_SP         (1U)
#define RMP_A7M_COP_FPV5_SP         (0U)
#define RMP_A7M_COP_FPV5_DP         (0U)

/* Other low-level initialization stuff - clock and serial
 * STM32F405 APB1<42MHz, APB2<84MHz. 
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
    RMP_Clear(&RCC_ClkInitStruct, sizeof(RCC_ClkInitStruct)); \
    RMP_Clear(&RCC_OscInitStruct, sizeof(RCC_OscInitStruct)); \
    RMP_Clear(&UART1_Handle, sizeof(UART_HandleTypeDef)); \
    RMP_Clear(&GPIO_Init, sizeof(GPIO_InitTypeDef)); \
    \
    /* Enable Power Control clock */ \
    __HAL_RCC_PWR_CLK_ENABLE(); \
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1); \
    \
    /* Enable HSE Oscillator and activate PLL with HSE as source */ \
    RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_HSE; \
    RCC_OscInitStruct.HSEState=RCC_HSE_ON; \
    RCC_OscInitStruct.PLL.PLLState=RCC_PLL_ON; \
    RCC_OscInitStruct.PLL.PLLSource=RCC_PLLSOURCE_HSE; \
    RCC_OscInitStruct.PLL.PLLM=8U; \
    RCC_OscInitStruct.PLL.PLLN=336U; \
    RCC_OscInitStruct.PLL.PLLP=RCC_PLLP_DIV2; \
    RCC_OscInitStruct.PLL.PLLQ=7U; \
    HAL_RCC_OscConfig(&RCC_OscInitStruct); \
    \
    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */ \
    RCC_ClkInitStruct.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2); \
    RCC_ClkInitStruct.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK; \
    RCC_ClkInitStruct.AHBCLKDivider=RCC_SYSCLK_DIV1; \
    RCC_ClkInitStruct.APB1CLKDivider=RCC_HCLK_DIV4; \
    RCC_ClkInitStruct.APB2CLKDivider=RCC_HCLK_DIV2; \
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5); \
    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE(); \
    __HAL_FLASH_DATA_CACHE_ENABLE(); \
    /* STM32F405x/407x/415x/417x Revision Z/1/2 devices: prefetch is supported */ \
    if(HAL_GetREVID()!=0x1000) \
        __HAL_FLASH_PREFETCH_BUFFER_ENABLE();\
    \
    /* Enable USART 1 for user-level operations */ \
    /* Clock enabling */ \
    __HAL_RCC_GPIOB_CLK_ENABLE(); \
    __HAL_RCC_USART1_CLK_ENABLE(); \
    /* UART IO initialization */ \
    GPIO_Init.Pin=GPIO_PIN_6; \
    GPIO_Init.Mode=GPIO_MODE_AF_PP; \
    GPIO_Init.Pull=GPIO_PULLUP; \
    GPIO_Init.Speed=GPIO_SPEED_FREQ_HIGH; \
    GPIO_Init.Alternate=GPIO_AF7_USART1; \
    HAL_GPIO_Init(GPIOB,&GPIO_Init); \
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
    USART1->DR=(rmp_ptr_t)(CHAR); \
    while((USART1->SR&0x40U)==0U); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
