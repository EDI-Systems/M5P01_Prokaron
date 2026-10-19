/******************************************************************************
Filename   : rmp_platform_stm32g071rb.h
Author     : pry
Date       : 19/10/2026
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for STM32G071RB.
******************************************************************************/

/* Define ********************************************************************/
/* The HAL library */
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_conf.h"
#include "core_cm0plus.h"

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
#define RMP_INIT_STACK_SIZE         (512U)

/* GUI */
#define RMP_GUI_ENABLE              (0U)
/* Anti-aliasing */
#define RMP_GUI_ANTIALIAS_ENABLE    (0U)
/* Widgets */
#define RMP_GUI_WIDGET_ENABLE       (0U)

/* What is the Systick value? */
#define RMP_A6M_SYSTICK_VAL         (6400U)

/* Other low-level initialization stuff - clock and serial
 * STM32G0xx HCLK<64MHz, PCLK1<64MHz. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_START_HOOK). */
#define RMP_A6M_LOWLVL_INIT() \
do \
{ \
    RCC_OscInitTypeDef RCC_OscInitStruct; \
    RCC_ClkInitTypeDef RCC_ClkInitStruct; \
    GPIO_InitTypeDef GPIO_Init; \
    UART_HandleTypeDef UART2_Handle; \
    RMP_Clear(&RCC_OscInitStruct, sizeof(RCC_OscInitTypeDef)); \
    RMP_Clear(&RCC_ClkInitStruct, sizeof(RCC_ClkInitTypeDef)); \
    RMP_Clear(&GPIO_Init, sizeof(GPIO_InitTypeDef)); \
    RMP_Clear(&UART2_Handle, sizeof(UART_HandleTypeDef)); \
    \
    /* Initialize the HAL - this also sets "uwTickPrio" to a valid value, without \
     * which "HAL_RCC_OscConfig" will fail on its internal "HAL_InitTick" call \
     * and skip the PLL configuration entirely (new G0 HAL defaults it to an \
     * intentionally invalid priority). */ \
    HAL_Init(); \
    \
    /* Enable Power Control clock */ \
    __HAL_RCC_PWR_CLK_ENABLE(); \
    /* The voltage scaling allows optimizing the power consumption when the device is  \
     * clocked below the maximum system frequency, to update the voltage scaling value  \
     * regarding system frequency refer to product datasheet.  */ \
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1); \
    /* Enable HSI Oscillator and activate PLL with HSI as source */ \
    RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_HSI; \
    RCC_OscInitStruct.HSIState=RCC_HSI_ON; \
    RCC_OscInitStruct.HSIDiv=RCC_HSI_DIV1; \
    RCC_OscInitStruct.HSICalibrationValue=RCC_HSICALIBRATION_DEFAULT; \
    RCC_OscInitStruct.PLL.PLLState=RCC_PLL_ON; \
    RCC_OscInitStruct.PLL.PLLSource=RCC_PLLSOURCE_HSI; \
    RCC_OscInitStruct.PLL.PLLM=RCC_PLLM_DIV1; \
    RCC_OscInitStruct.PLL.PLLN=8U; \
    RCC_OscInitStruct.PLL.PLLP=RCC_PLLP_DIV10; \
    RCC_OscInitStruct.PLL.PLLQ=RCC_PLLQ_DIV5; \
    RCC_OscInitStruct.PLL.PLLR=RCC_PLLR_DIV2; \
    HAL_RCC_OscConfig(&RCC_OscInitStruct); \
    /* Select PLL as system clock source and configure the HCLK and PCLK1 clock dividers */ \
    RCC_ClkInitStruct.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1); \
    RCC_ClkInitStruct.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK; \
    RCC_ClkInitStruct.AHBCLKDivider=RCC_SYSCLK_DIV1; \
    RCC_ClkInitStruct.APB1CLKDivider=RCC_HCLK_DIV1; \
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2); \
    \
    /* Enable USART 2 for user-level operations */ \
    /* Clock enabling */ \
    __HAL_RCC_GPIOA_CLK_ENABLE(); \
    __HAL_RCC_USART2_CLK_ENABLE(); \
    /* UART IO initialization - PA2 is the VCP TX on NUCLEO-G071RB */ \
    GPIO_Init.Pin=GPIO_PIN_2; \
    GPIO_Init.Mode=GPIO_MODE_AF_PP; \
    GPIO_Init.Pull=GPIO_PULLUP; \
    GPIO_Init.Speed=GPIO_SPEED_FREQ_HIGH; \
    GPIO_Init.Alternate=GPIO_AF1_USART2; \
    HAL_GPIO_Init(GPIOA,&GPIO_Init); \
    /* UART initialization */ \
    UART2_Handle.Instance=USART2; \
    UART2_Handle.Init.BaudRate=115200U; \
    UART2_Handle.Init.WordLength=UART_WORDLENGTH_8B; \
    UART2_Handle.Init.StopBits=UART_STOPBITS_1; \
    UART2_Handle.Init.Parity=UART_PARITY_NONE; \
    UART2_Handle.Init.HwFlowCtl=UART_HWCONTROL_NONE; \
    UART2_Handle.Init.Mode=UART_MODE_TX; \
    HAL_UART_Init(&UART2_Handle); \
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
    USART2->TDR=(rmp_ptr_t)(CHAR); \
    while((USART2->ISR&0x40U)==0U); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
