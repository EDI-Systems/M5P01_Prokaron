/******************************************************************************
Filename   : rmp_platform_HC32L136K8.h
Author     : pry
Date       : 24/06/2017
Licence    : LGPL v3+; see COPYING for details.
Description: The configuration file for HC32L136K8.
******************************************************************************/

/* Defines *******************************************************************/
/* The HAL library */
#include "HC32L136K8TA.h"
#include "system_hc32l13x.h"
#include "ddl.h"
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
#define RMP_CMX_SYSTICK_VAL          24000

/* Other low-level initialization stuff - clock and serial
 * This device requires that you switch from low to high frequency.
 * The manufacturer have defined their own delay functions - do not use
 * them, because they are not OS agnostic and will reprogram the systick 
 * to something that you possibly don't want. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_Start_Hook). */
#define RMP_CMX_LOW_LEVEL_INIT() \
do \
{ \
    /* Switch to the target clock frequency */ \
    Sysctrl_SetRCLTrim(SysctrlRclFreq32768); \
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, TRUE); \
    Sysctrl_SysClkSwitch(SysctrlClkRCL); \
    Sysctrl_SetRCHTrim(SysctrlRchFreq24MHz); \
	Sysctrl_SysClkSwitch(SysctrlClkRCH); \
    /* Turn off the low speed RC oscillator and update core clock */ \
	Sysctrl_ClkSourceEnable(SysctrlClkRCL, FALSE); \
    SystemCoreClockUpdate(); \
    /* ����������IO�ʹ��ڳ�ʼ������Ϊ��ӡ�п����õ� */\
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
    /* ������Ӵ��ڴ�ӡ��ʹ����ѯ��ʽ���һ���ַ���*/\
} \
while(0)
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
