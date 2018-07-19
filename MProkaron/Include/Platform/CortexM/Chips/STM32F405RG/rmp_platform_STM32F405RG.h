/******************************************************************************
Filename   : rmp_platform_STM32F405RG.h
Author     : pry
Date       : 24/06/2017
Licence    : LGPL v3+; see COPYING for details.
Description: The configuration file for STM32F405RG.
******************************************************************************/

/* Defines *******************************************************************/
/* The HAL library */
#include "stm32f4xx.h"
#include "core_cm4.h"
#include "stm32f4xx_hal.h"
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
#define RMP_CMX_SYSTICK_VAL          16800

/* Other low-level initialization stuff - clock and serial
 * STM32F405 APB1<42MHz, APB2<84MHz */
#define RMP_CMX_LOW_LEVEL_INIT() \
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
    RCC_OscInitStruct.PLL.PLLM=8; \
    RCC_OscInitStruct.PLL.PLLN=336; \
    RCC_OscInitStruct.PLL.PLLP=RCC_PLLP_DIV2; \
    RCC_OscInitStruct.PLL.PLLQ=7; \
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
    RMP_CMX_PUTCHAR('\r'); \
    RMP_CMX_PUTCHAR('\n'); \
    \
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
