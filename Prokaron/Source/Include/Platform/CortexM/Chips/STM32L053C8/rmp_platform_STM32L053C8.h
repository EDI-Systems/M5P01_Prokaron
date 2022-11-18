/******************************************************************************
Filename   : rmp_platform_STM32L053C8.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for STM32L053C8.
******************************************************************************/

/* Defines *******************************************************************/
/* The HAL library */
#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_conf.h"
#include "core_cm0plus.h"
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
/* The mask/unmask interrupt operations */
#define RMP_MASK_INT()               RMP_Disable_Int()
#define RMP_UNMASK_INT()             RMP_Enable_Int()

/* What is the NVIC priority grouping? */
#define RMP_CMX_NVIC_GROUPING        RMP_CMX_NVIC_GROUPING_P2S6
/* What is the Systick value? */
#define RMP_CMX_SYSTICK_VAL          3600

/* Other low-level initialization stuff - clock and serial
 * STM32L0xx APB1<32MHz, APB2<32MHz. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_Start_Hook). */
#define RMP_CMX_LOW_LEVEL_INIT() \
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
    /* Enable Power Control clock */ \
    __HAL_RCC_PWR_CLK_ENABLE(); \
    /* The voltage scaling allows optimizing the power consumption when the device is  \
     * clocked below the maximum system frequency, to update the voltage scaling value  \
     * regarding system frequency refer to product datasheet.  */ \
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1); \
    /* Enable HSI Oscillator and activate PLL with HSI as source */ \
    RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_HSI; \
    RCC_OscInitStruct.HSEState=RCC_HSE_OFF; \
    RCC_OscInitStruct.HSIState=RCC_HSI_ON; \
    RCC_OscInitStruct.PLL.PLLState=RCC_PLL_ON; \
    RCC_OscInitStruct.PLL.PLLSource=RCC_PLLSOURCE_HSI; \
    RCC_OscInitStruct.PLL.PLLMUL=RCC_PLL_MUL4; \
    RCC_OscInitStruct.PLL.PLLDIV=RCC_PLL_DIV2; \
    RCC_OscInitStruct.HSICalibrationValue=0x10; \
    HAL_RCC_OscConfig(&RCC_OscInitStruct); \
    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clock dividers */ \
    RCC_ClkInitStruct.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2); \
    RCC_ClkInitStruct.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK; \
    RCC_ClkInitStruct.AHBCLKDivider=RCC_SYSCLK_DIV1; \
    RCC_ClkInitStruct.APB1CLKDivider=RCC_HCLK_DIV1; \
    RCC_ClkInitStruct.APB2CLKDivider=RCC_HCLK_DIV1; \
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1); \
    \
    /* Enable USART 1 for user-level operations */ \
    /* Clock enabling */ \
    __HAL_RCC_GPIOA_CLK_ENABLE(); \
    __HAL_RCC_USART1_CLK_ENABLE(); \
    /* UART IO initialization */ \
    GPIO_Init.Pin=GPIO_PIN_9; \
    GPIO_Init.Mode=GPIO_MODE_AF_PP; \
    GPIO_Init.Pull=GPIO_PULLUP; \
    GPIO_Init.Speed=GPIO_SPEED_HIGH; \
    GPIO_Init.Alternate=GPIO_AF4_USART1; \
    HAL_GPIO_Init(GPIOA,&GPIO_Init); \
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
    /* Set the priority of timer, svc and faults to the lowest */ \
    NVIC_SetPriority(SVC_IRQn, 0xFF); \
    NVIC_SetPriority(PendSV_IRQn, 0xFF); \
    NVIC_SetPriority(SysTick_IRQn, 0xFF); \
    /* Configure systick */ \
    SysTick_Config(RMP_CMX_SYSTICK_VAL); \
} \
while(0)

/* This is for debugging output */
#define RMP_CMX_PUTCHAR(CHAR) \
do \
{ \
    USART1->TDR=CHAR; \
    while((USART1->ISR&0x40)==0); \
} \
while(0)
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
