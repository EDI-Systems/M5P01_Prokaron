/******************************************************************************
Filename   : platform_STM32F103RE.h
Author     : pry
Date       : 24/06/2017
Licence    : LGPL v3+; see COPYING for details.
Description: The configuration file for STM32F103RE.
******************************************************************************/

/* Defines *******************************************************************/
/* The HAL library */
#include "stm32f10x_conf.h"
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

/* What is the NVIC priority grouping? */
#define RMP_CMX_NVIC_GROUPING        RMP_CMX_NVIC_GROUPING_P2S6
/* What is the Systick value? */
#define RMP_CMX_SYSTICK_VAL          7200

/* Other low-level initialization stuff - clock and serial
 * STM32F10x APB1<36MHz, APB2<72MHz */
#define RMP_CMX_LOW_LEVEL_INIT() \
do \
{ \
    USART_InitTypeDef USART1_Init; \
    GPIO_InitTypeDef GPIOA_Init; \
    RMP_Clear(&USART1_Init, sizeof(USART_InitTypeDef)); \
    RMP_Clear(&GPIOA_Init, sizeof(GPIO_InitTypeDef)); \
    \
    GPIOA_Init.GPIO_Mode=GPIO_Mode_AF_PP; \
    GPIOA_Init.GPIO_Pin=GPIO_Pin_9; \
    GPIOA_Init.GPIO_Speed=GPIO_Speed_50MHz; \
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); \
    GPIO_Init(GPIOA, &GPIOA_Init); \
    \
    USART1_Init.USART_BaudRate=115200; \
    USART1_Init.USART_WordLength=USART_WordLength_8b; \
    USART1_Init.USART_StopBits=USART_StopBits_1; \
    USART1_Init.USART_Parity=USART_Parity_No; \
    USART1_Init.USART_HardwareFlowControl=USART_HardwareFlowControl_None; \
    USART1_Init.USART_Mode=USART_Mode_Tx; \
    \
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); \
    USART_Init(USART1, &USART1_Init); \
    USART_Cmd(USART1, ENABLE); \
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
