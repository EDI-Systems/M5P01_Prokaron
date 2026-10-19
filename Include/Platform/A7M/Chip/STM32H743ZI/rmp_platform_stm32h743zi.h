/******************************************************************************
Filename   : rmp_platform_stm32h743zi.h
Author     : pry
Date       : 19/10/2026
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for STM32H743ZI.
******************************************************************************/

/* Define********************************************************************/
/* The HAL library */
#include "stm32h7xx.h"
#include "core_cm7.h"
#include "stm32h7xx_hal.h"

/* Debugging */
/* Assertion */
#define RMP_ASSERT_ENABLE           (1U)
/* Invalid parameter checking */
#define RMP_CHECK_ENABLE            (1U)
/* Debug logging */
#define RMP_DBGLOG_ENABLE           (1U)

/* System */
/* The maximum number of preemption priorities */
#define RMP_PREEMPT_PRIO_NUM        (64U)
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
#define RMP_A7M_SYSTICK_VAL         (40000U)
/* What is the interrupt masking level? */
#define RMP_A7M_INT_MASK_LVL        (0xFFU)
/* What is the FPU type? */
#define RMP_A7M_COP_FPV4_SP         (1U)
#define RMP_A7M_COP_FPV5_SP         (1U)
#define RMP_A7M_COP_FPV5_DP         (1U)

/* Other low-level initialization stuff - clock and serial
 * STM32H743 D1 core @400MHz(VOS1), HCLK=200MHz, APB1/APB2=100MHz,
 * APB1 timer clock=200MHz(cpu/2), from HSE(8MHz MCO from ST-LINK).
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_START_HOOK). */
#define RMP_A7M_LOWLVL_INIT() \
do \
{ \
    RCC_OscInitTypeDef RCC_OscInitStructure; \
    RCC_ClkInitTypeDef RCC_ClkInitStructure; \
    GPIO_InitTypeDef GPIO_Init; \
    UART_HandleTypeDef UART3_Handle; \
    RMP_Clear(&RCC_OscInitStructure, sizeof(RCC_OscInitTypeDef)); \
    RMP_Clear(&RCC_ClkInitStructure, sizeof(RCC_ClkInitStructure)); \
    RMP_Clear(&GPIO_Init, sizeof(GPIO_InitTypeDef)); \
    RMP_Clear(&UART3_Handle, sizeof(UART_HandleTypeDef)); \
    \
    /* Initialize the HAL: this sets up "uwTickPrio=TICK_INT_PRIORITY" */ \
    /* (the reset value is intentionally invalid), without which */ \
    /* "HAL_RCC_ClockConfig" would fail in "HAL_InitTick" and abort. */ \
    HAL_Init(); \
    \
    /* Set the clock tree in the system */ \
    /* Power supply configuration: use the internal LDO, VOS1 for 400MHz. */ \
    /* The PWR clock is enabled by default on STM32H7 (RCC_APB4ENR.PWREN). */ \
    RMP_ASSERT(HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY)==HAL_OK); \
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1); \
    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)); \
    \
    /* Enable HSE oscillator(MCO from ST-LINK, thus bypass mode) and activate */ \
    /* PLL1 with HSE as source: 8MHz/4x400/2 = 400MHz CPU clock */ \
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE; \
    RCC_OscInitStructure.HSEState=RCC_HSE_BYPASS; \
    RCC_OscInitStructure.HSIState=RCC_HSI_OFF; \
    RCC_OscInitStructure.CSIState=RCC_CSI_OFF; \
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON; \
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE; \
    RCC_OscInitStructure.PLL.PLLM=4; \
    RCC_OscInitStructure.PLL.PLLN=400; \
    RCC_OscInitStructure.PLL.PLLP=2; \
    RCC_OscInitStructure.PLL.PLLQ=4; \
    RCC_OscInitStructure.PLL.PLLR=2; \
    RCC_OscInitStructure.PLL.PLLFRACN=0; \
    RCC_OscInitStructure.PLL.PLLVCOSEL=RCC_PLL1VCOWIDE; \
    RCC_OscInitStructure.PLL.PLLRGE=RCC_PLL1VCIRANGE_1; \
    RMP_ASSERT(HAL_RCC_OscConfig(&RCC_OscInitStructure)==HAL_OK); \
    \
    /* Select PLL1 as system clock source and configure the bus clock dividers */ \
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK| \
                                    RCC_CLOCKTYPE_D1PCLK1|RCC_CLOCKTYPE_PCLK1| \
                                    RCC_CLOCKTYPE_PCLK2|RCC_CLOCKTYPE_D3PCLK1); \
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK; \
    RCC_ClkInitStructure.SYSCLKDivider=RCC_SYSCLK_DIV1; \
    RCC_ClkInitStructure.AHBCLKDivider=RCC_HCLK_DIV2; \
    RCC_ClkInitStructure.APB3CLKDivider=RCC_APB3_DIV2; \
    RCC_ClkInitStructure.APB1CLKDivider=RCC_APB1_DIV2; \
    RCC_ClkInitStructure.APB2CLKDivider=RCC_APB2_DIV2; \
    RCC_ClkInitStructure.APB4CLKDivider=RCC_APB4_DIV2; \
    RMP_ASSERT(HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_4)==HAL_OK); \
    \
    /* Cache enabling: the L1 caches of the Cortex-M7 core */ \
    SCB_EnableICache(); \
    SCB_EnableDCache(); \
    \
    /* Enable USART 3 for user-level operations - PD8 is the VCP TX on NUCLEO-H743ZI */ \
    /* Clock enabling */ \
    __HAL_RCC_GPIOD_CLK_ENABLE(); \
    __HAL_RCC_USART3_CLK_ENABLE(); \
    /* UART IO initialization */ \
    GPIO_Init.Pin=GPIO_PIN_8; \
    GPIO_Init.Mode=GPIO_MODE_AF_PP; \
    GPIO_Init.Pull=GPIO_PULLUP; \
    GPIO_Init.Speed=GPIO_SPEED_FREQ_VERY_HIGH; \
    GPIO_Init.Alternate=GPIO_AF7_USART3; \
    HAL_GPIO_Init(GPIOD,&GPIO_Init); \
    /* UART initialization */ \
    UART3_Handle.Instance=USART3; \
    UART3_Handle.Init.BaudRate=115200; \
    UART3_Handle.Init.WordLength=UART_WORDLENGTH_8B; \
    UART3_Handle.Init.StopBits=UART_STOPBITS_1; \
    UART3_Handle.Init.Parity=UART_PARITY_NONE; \
    UART3_Handle.Init.HwFlowCtl=UART_HWCONTROL_NONE; \
    UART3_Handle.Init.Mode=UART_MODE_TX; \
    RMP_ASSERT(HAL_UART_Init(&UART3_Handle)==HAL_OK); \
    RMP_A7M_PUTCHAR('\r'); \
    RMP_A7M_PUTCHAR('\n'); \
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
    USART3->TDR=(rmp_ptr_t)(CHAR); \
    while((USART3->ISR&0x40U)==0U); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/