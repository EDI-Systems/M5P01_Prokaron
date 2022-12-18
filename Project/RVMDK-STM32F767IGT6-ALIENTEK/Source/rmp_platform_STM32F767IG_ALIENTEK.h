/******************************************************************************
Filename   : rmp_platform_STM32F767IG_FPU.h
Author     : pry
Date       : 24/06/2017
Licence    : LGPL v3+; see COPYING for details.
Description: The configuration file for STM32F767IG using the FPU.
******************************************************************************/

/* Defines *******************************************************************/
/* The HAL library */
#include "stm32f7xx.h"
#include "core_cm7.h"
#include "stm32f7xx_hal.h"
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 32 is usually sufficient */
#define RMP_PREEMPT_PRIO_NUM         32
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               100000
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX              1000
/* Are we using custom hooks? */
#define RMP_HOOK_EXTRA                0U
/* The stzck size of the init thread */
#define RMP_INIT_STACK_SIZE          256
/* The mask/unmask interrupt operations - can be adjusted to fit your own levels */
#define RMP_INT_MASK()               RMP_Int_Mask(0xFF)
#define RMP_INT_UNMASK()             RMP_Int_Mask(0x00)

/* Graphics */
#define RMP_POINT                    RMP_Point
/* If you want to use anti-aliasing, these color mixing macros must be provided */
#define R(C)                         ((C)>>11)
#define G(C)                         (((C)>>5)&0x3F)
#define B(C)                         ((C)&0x1F)
#define RGB(R,G,B)                   (((R)<<11)|((G)<<5)|(B))
#define RMP_COLOR_25P(C1,C2)         RGB((R(C1)+R(C2)*3)>>2,(G(C1)+G(C2)*3)>>2,(B(C1)+B(C2)*3)>>2)
#define RMP_COLOR_50P(C1,C2)         RGB((R(C1)+R(C2))>>1,(G(C1)+G(C2))>>1,(B(C1)+B(C2))>>1)
#define RMP_COLOR_75P(C1,C2)         RGB((R(C1)*3+R(C2))>>2,(G(C1)*3+G(C2))>>2,(B(C1)*3+B(C2))>>2)
/* If you want to use the control drawing library, these color macros must be provided */
#define RMP_CTL_WHITE                0xFFFF
#define RMP_CTL_LGREY                (28<<11)|(56<<5)|(28)
#define RMP_CTL_GREY                 (23<<11)|(46<<5)|(23)
#define RMP_CTL_DGREY                (20<<11)|(40<<5)|(20)
#define RMP_CTL_DARK                 (15<<11)|(31<<5)|(15)
#define RMP_CTL_DDARK                (13<<11)|(26<<5)|(13)
#define RMP_CTL_BLACK                0x0000

/* What is the NVIC priority grouping? */
#define RMP_CMX_NVIC_GROUPING        RMP_CMX_NVIC_GROUPING_P2S6
/* What is the Systick value? */
#define RMP_CMX_SYSTICK_VAL          21600

/* Other low-level initialization stuff - clock and serial
 * STM32F7xx APB1<45MHz, APB2<90MHz. When running at 216MHz,
 * actually we are overdriving the bus a little, which might
 * be fine.
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_Start_Hook). */
#define RMP_CMX_LOW_LEVEL_INIT() \
do \
{ \
    RCC_OscInitTypeDef RCC_OscInitStructure; \
    RCC_ClkInitTypeDef RCC_ClkInitStructure; \
    GPIO_InitTypeDef GPIO_Init; \
    UART_HandleTypeDef UART1_Handle; \
    RMP_Clear(&RCC_OscInitStructure, sizeof(RCC_OscInitTypeDef)); \
    RMP_Clear(&RCC_ClkInitStructure, sizeof(RCC_ClkInitTypeDef)); \
    RMP_Clear(&GPIO_Init, sizeof(GPIO_InitTypeDef)); \
    RMP_Clear(&UART1_Handle, sizeof(UART_HandleTypeDef)); \
    \
    /* Set the clock tree in the system */ \
    /* Enble power regulator clock, and configure voltage scaling function */ \
    __HAL_RCC_PWR_CLK_ENABLE(); \
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1); \
    /* Initialize the oscillator */ \
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE; \
    RCC_OscInitStructure.HSEState=RCC_HSE_ON; \
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON; \
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE; \
    /* Fpll=Fin/PLLM*PLLN, Fsys=Fpll/PLLP, Fperiph=Fpll/PLLQ */ \
    RCC_OscInitStructure.PLL.PLLM=25; \
    RCC_OscInitStructure.PLL.PLLN=432; \
    RCC_OscInitStructure.PLL.PLLP=2; \
    RCC_OscInitStructure.PLL.PLLQ=9; \
    RMP_ASSERT(HAL_RCC_OscConfig(&RCC_OscInitStructure)==HAL_OK); \
    /* Overdrive to 216MHz */ \
    RMP_ASSERT(HAL_PWREx_EnableOverDrive()==HAL_OK); \
    \
    /* HCLK,PCLK1 & PCLK2 configuration */ \
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2); \
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK; \
    RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1; \
    RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV4; \
    RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV2; \
    /* Flash latency = 7us, 8 CPU cycles */ \
    RMP_ASSERT(HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_7)==HAL_OK); \
    \
    /* Cache/Flash ART enabling */ \
    SCB_EnableICache(); \
    SCB_EnableDCache(); \
    __HAL_FLASH_ART_ENABLE(); \
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
    GPIO_Init.Alternate=GPIO_AF7_USART1; \
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
    USART1->TDR=CHAR; \
    while((USART1->ISR&0x40)==0); \
} \
while(0)
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
