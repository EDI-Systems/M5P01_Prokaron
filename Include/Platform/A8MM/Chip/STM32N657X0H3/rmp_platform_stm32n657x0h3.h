/******************************************************************************
Filename   : rmp_platform_stm32n657x0h3.h
Author     : pry
Date       : 02/11/2026
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for STM32N657X0H3 (Cortex-M55, no internal
             flash - the image is downloaded into AXISRAM1 and launched via
             the debugger, see the GCCMF-STM32N657X0Q project).
******************************************************************************/

/* Define********************************************************************/
/* The HAL library */
#include "stm32n657xx.h"
#include "core_cm55.h"
#include "stm32n6xx_hal.h"

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
#define RMP_A8MM_NVIC_GROUPING       RMP_A8MM_NVIC_GROUPING_P2S6
/* What is the Systick value? CPU clock = 600MHz, 1ms tick */
#define RMP_A8MM_SYSTICK_VAL         (600000U)
/* What is the interrupt masking level? */
#define RMP_A8MM_INT_MASK_LVL        (0xFFU)
/* What is the FPU type? The Cortex-M55 on STM32N6 has FPv5 with double
 * precision plus the MVE (Helium) extension with single precision vectors. */
#define RMP_A8MM_COP_FPV5_SP         (1U)
#define RMP_A8MM_COP_FPV5_DP         (1U)
#define RMP_A8MM_COP_MVE_SP          (1U)

/* Other low-level initialization stuff - clock and serial
 * STM32N657X0H3 CPU @600MHz(HSI 64MHz -> PLL1: M=4,N=75,VCO 1200MHz -> IC1/2),
 * SYSCLK/HCLK @100MHz(IC2/3, AHB/4), APB1=APB2=100MHz, from the on-board
 * SMPS(external supply) with VOS1 - same tree as the official ST Nucleo
 * examples. The chip has no internal flash and runs from AXISRAM1 downloaded
 * by the debugger, thus no flash wait states are involved. USART1 kernel
 * clock=PCLK1(reset default), USART1 is the STLINK-V3EC VCP on
 * NUCLEO-N657X0-Q (PE5=TX).
 * This is the default initialization sequence. If you wish to supply your
 * own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_START_HOOK). */
#define RMP_A8MM_LOWLVL_INIT() \
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
    /* Initialize the HAL: this sets up "uwTickPrio=TICK_INT_PRIORITY" */ \
    /* (the reset value is intentionally invalid), without which the HAL */ \
    /* tick related functions would fail. */ \
    HAL_Init(); \
    \
    /* The factory demo of the NUCLEO-N657X0-Q keeps the independent */ \
    /* watchdog running. Feed it here and in the test bench timer so our */ \
    /* image survives on the board (IWDG_KR = 0xAAAA is a no-op when the */ \
    /* watchdog is disabled). */ \
    IWDG->KR=0xAAAAU; \
    \
    /* Cache enabling: the L1 caches of the Cortex-M55 */ \
    SCB_EnableICache(); \
    SCB_EnableDCache(); \
    \
    /* Power supply: the NUCLEO-N657X0-Q core rail comes from the on-board */ \
    /* SMPS (external source); VOS1 is required for the 600MHz CPU clock. */ \
    RMP_ASSERT(HAL_PWREx_ConfigSupply(PWR_EXTERNAL_SOURCE_SUPPLY)==HAL_OK); \
    RMP_ASSERT(HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)==HAL_OK); \
    \
    /* Enable the HSI (explicitly; it is the reset default anyway) */ \
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSI; \
    RCC_OscInitStructure.HSIState=RCC_HSI_ON; \
    RCC_OscInitStructure.HSIDiv=RCC_HSI_DIV1; \
    RCC_OscInitStructure.HSICalibrationValue=RCC_HSICALIBRATION_DEFAULT; \
    RCC_OscInitStructure.PLL1.PLLState=RCC_PLL_NONE; \
    RCC_OscInitStructure.PLL2.PLLState=RCC_PLL_NONE; \
    RCC_OscInitStructure.PLL3.PLLState=RCC_PLL_NONE; \
    RCC_OscInitStructure.PLL4.PLLState=RCC_PLL_NONE; \
    RMP_ASSERT(HAL_RCC_OscConfig(&RCC_OscInitStructure)==HAL_OK); \
    \
    /* Switch to HSI intermediate clocks if the PLL outputs are already */ \
    /* selected (not the case right after reset, but harmless to check) */ \
    HAL_RCC_GetClockConfig(&RCC_ClkInitStructure); \
    if((RCC_ClkInitStructure.CPUCLKSource==RCC_CPUCLKSOURCE_IC1)|| \
       (RCC_ClkInitStructure.SYSCLKSource==RCC_SYSCLKSOURCE_IC2_IC6_IC11)) \
    { \
        RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_CPUCLK|RCC_CLOCKTYPE_SYSCLK); \
        RCC_ClkInitStructure.CPUCLKSource=RCC_CPUCLKSOURCE_HSI; \
        RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_HSI; \
        RMP_ASSERT(HAL_RCC_ClockConfig(&RCC_ClkInitStructure)==HAL_OK); \
    } \
    \
    /* PLL1: HSI 64MHz / M4 x N75 = 1200MHz VCO, P1=P2=1 */ \
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_NONE; \
    RCC_OscInitStructure.PLL1.PLLState=RCC_PLL_ON; \
    RCC_OscInitStructure.PLL1.PLLSource=RCC_PLLSOURCE_HSI; \
    RCC_OscInitStructure.PLL1.PLLM=4; \
    RCC_OscInitStructure.PLL1.PLLN=75; \
    RCC_OscInitStructure.PLL1.PLLFractional=0; \
    RCC_OscInitStructure.PLL1.PLLP1=1; \
    RCC_OscInitStructure.PLL1.PLLP2=1; \
    RCC_OscInitStructure.PLL2.PLLState=RCC_PLL_NONE; \
    RCC_OscInitStructure.PLL3.PLLState=RCC_PLL_NONE; \
    RCC_OscInitStructure.PLL4.PLLState=RCC_PLL_NONE; \
    RMP_ASSERT(HAL_RCC_OscConfig(&RCC_OscInitStructure)==HAL_OK); \
    \
    /* CPUCLK=IC1(PLL1/2)=600MHz, SYSCLK=IC2(PLL1/3)=400MHz, */ \
    /* HCLK=AHB/4=100MHz, APB1=APB2=100MHz, TIM4 clock=PCLK1=100MHz */ \
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_CPUCLK|RCC_CLOCKTYPE_HCLK| \
                                    RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1| \
                                    RCC_CLOCKTYPE_PCLK2|RCC_CLOCKTYPE_PCLK5| \
                                    RCC_CLOCKTYPE_PCLK4); \
    RCC_ClkInitStructure.CPUCLKSource=RCC_CPUCLKSOURCE_IC1; \
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_IC2_IC6_IC11; \
    RCC_ClkInitStructure.AHBCLKDivider=RCC_HCLK_DIV4; \
    RCC_ClkInitStructure.APB1CLKDivider=RCC_APB1_DIV1; \
    RCC_ClkInitStructure.APB2CLKDivider=RCC_APB2_DIV1; \
    RCC_ClkInitStructure.APB4CLKDivider=RCC_APB4_DIV1; \
    RCC_ClkInitStructure.APB5CLKDivider=RCC_APB5_DIV1; \
    RCC_ClkInitStructure.IC1Selection.ClockSelection=RCC_ICCLKSOURCE_PLL1; \
    RCC_ClkInitStructure.IC1Selection.ClockDivider=2; \
    RCC_ClkInitStructure.IC2Selection.ClockSelection=RCC_ICCLKSOURCE_PLL1; \
    RCC_ClkInitStructure.IC2Selection.ClockDivider=3; \
    RCC_ClkInitStructure.IC6Selection.ClockSelection=RCC_ICCLKSOURCE_PLL1; \
    RCC_ClkInitStructure.IC6Selection.ClockDivider=4; \
    RCC_ClkInitStructure.IC11Selection.ClockSelection=RCC_ICCLKSOURCE_PLL1; \
    RCC_ClkInitStructure.IC11Selection.ClockDivider=3; \
    RMP_ASSERT(HAL_RCC_ClockConfig(&RCC_ClkInitStructure)==HAL_OK); \
    \
    IWDG->KR=0xAAAAU; \
    \
    /* Enable lazy FPU/MVE state preservation: the hardware stacks the */ \
    /* extended frame (S0-S15, FPSCR and the MVE VPR) for FP/MVE threads, */ \
    /* and the kernel only has to save S16-S31 in the PendSV handler. */ \
    FPU->FPCCR|=(FPU_FPCCR_ASPEN_Msk|FPU_FPCCR_LSPEN_Msk); \
    \
    /* Enable USART 1 for user-level operations - PE5 is the VCP TX on */ \
    /* NUCLEO-N657X0-Q */ \
    /* Clock enabling */ \
    __HAL_RCC_GPIOE_CLK_ENABLE(); \
    __HAL_RCC_USART1_CLK_ENABLE(); \
    /* UART IO initialization */ \
    GPIO_Init.Pin=GPIO_PIN_5; \
    GPIO_Init.Mode=GPIO_MODE_AF_PP; \
    GPIO_Init.Pull=GPIO_PULLUP; \
    GPIO_Init.Speed=GPIO_SPEED_FREQ_VERY_HIGH; \
    GPIO_Init.Alternate=GPIO_AF7_USART1; \
    HAL_GPIO_Init(GPIOE,&GPIO_Init); \
    /* UART initialization */ \
    UART1_Handle.Instance=USART1; \
    UART1_Handle.Init.BaudRate=115200; \
    UART1_Handle.Init.WordLength=UART_WORDLENGTH_8B; \
    UART1_Handle.Init.StopBits=UART_STOPBITS_1; \
    UART1_Handle.Init.Parity=UART_PARITY_NONE; \
    UART1_Handle.Init.HwFlowCtl=UART_HWCONTROL_NONE; \
    UART1_Handle.Init.Mode=UART_MODE_TX; \
    RMP_ASSERT(HAL_UART_Init(&UART1_Handle)==HAL_OK); \
    RMP_A8MM_PUTCHAR('\r'); \
    RMP_A8MM_PUTCHAR('\n'); \
    \
    /* Set the priority of timer, svc and faults to the lowest */ \
    NVIC_SetPriorityGrouping(RMP_A8MM_NVIC_GROUPING); \
    NVIC_SetPriority(SVCall_IRQn, 0xFF); \
    NVIC_SetPriority(PendSV_IRQn, 0xFF); \
    NVIC_SetPriority(SysTick_IRQn, 0xFF); \
    NVIC_SetPriority(MemoryManagement_IRQn, 0xFF); \
    NVIC_SetPriority(BusFault_IRQn, 0xFF); \
    NVIC_SetPriority(UsageFault_IRQn, 0xFF); \
    NVIC_SetPriority(DebugMonitor_IRQn, 0xFF); \
    SysTick_Config(RMP_A8MM_SYSTICK_VAL); \
} \
while(0)

/* This is for debugging output */
#define RMP_A8MM_PUTCHAR(CHAR) \
do \
{ \
    USART1->TDR=(rmp_ptr_t)(CHAR); \
    while((USART1->ISR&0x40U)==0U); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
