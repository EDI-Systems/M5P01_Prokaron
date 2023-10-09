/******************************************************************************
Filename    : rmp_test_stm32f767ig_rvm.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for STM32F767IG, running in the RVM. 
              How to use this test header:
              1. Add relevant test file to the project.
              2. Add memory access permission as follows:
                 <Type>V_Device</Type>
                 <Base>0x40000000</Base>
                 <Size>0x20000000</Size>
                 <Align>Auto</Align>
                 <Type>Device</Type>
                 <Attribute>RWS</Attribute>
              3. Add kernel function as follows:
                 <Name>INT_LOCAL_MOD</Name>
                 <Start>0xF100</Start>
                 <End>0xF100</End>
              4. Add interrupt vector declaration as follows (optional):
                 <Name>TIM4</Name>
                 <Number>30</Number>
              5. Place the following lines into the RME_Spurious_Handler (or a
                 dedicated handler if you'd wish to use one), so that we clear
                 the interrupt flags as soon as it fires.
                 #define TIM4_SR    *((volatile rme_u32_t*)(0x40000810U))
                 TIM4_SR=0U;
                 return RME_RVM_VCT_SIG_INIT;

ARMCC 6.18 -O3 (SysTick turned off to avoid spurious interrupts)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 1446 / 1520 / 1412
Mailbox                            : 1636 / 1752 / 1624
Semaphore                          : 1594 / 1760 / 1560
FIFO                               : 156 / 272 / 156
Message queue                      : 1862 / 2064 / 1816
Blocking message queue             : 2092 / 2180 / 2060
Memory allocation/free pair        : 338 / 404 / 330
ISR Mailbox                        : 1314 / 1456 / 1224
ISR Semaphore                      : 1210 / 1316 / 1160
ISR Message queue                  : 1418 / 1496 / 1340
ISR Blocking message queue         : 1601 / 1716 / 1488

ARMCC 6.18 -O3 (SysTick turned on, may kick in between)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 1448 / 2712 / 1408
Mailbox                            : 1661 / 2860 / 1604
Semaphore                          : 1601 / 2832 / 1552
FIFO                               : 156 / 284 / 156
Message queue                      : 1864 / 3100 / 1816
Blocking message queue             : 2096 / 3468 / 2032
Memory allocation/free pair        : 338 / 506 / 330
ISR Mailbox                        : 1315 / 2480 / 1224
ISR Semaphore                      : 1230 / 2304 / 1148
ISR Message queue                  : 1422 / 2560 / 1316
ISR Blocking message queue         : 1607 / 2852 / 1488

ARMCC 6.18 -O3 (SysTick turned on, may kick in between, w/FPU context)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 1667 / 2988 / 1640
Mailbox                            : 1850 / 3068 / 1804
Semaphore                          : 1777 / 3104 / 1748
FIFO                               : 160 / 1612 / 160
Message queue                      : 2046 / 3340 / 2004
Blocking message queue             : 2344 / 3692 / 2264
Memory allocation/free pair        : 339 / 692 / 331
ISR Mailbox                        : 1380 / 2688 / 1296
ISR Semaphore                      : 1322 / 2604 / 1256
ISR Message queue                  : 1461 / 2708 / 1384
ISR Blocking message queue         : 1644 / 2976 / 1560

******************************************************************************/

/* Includes ******************************************************************/
#include "rmp.h"
#include "rvm_guest.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Where are the initial stacks */
#define THD1_STACK              (&Stack_1[230])
#define THD2_STACK              (&Stack_2[230])
/* How to read counter */
#define COUNTER_READ()          ((TIM2_CNT)<<1)
/* Are we testing the memory pool? */
#define TEST_MEM_POOL           (8192U)
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The STM32F7 timers are all 32 bits, so */
typedef rmp_ptr_t rmp_tim_t;

/* Hardware definitions so we don't rely on STM32 HAL */
#define TIM2_BASE               (0x40000000U)
#define TIM4_BASE               (0x40000800U)
#define RCC_BASE                (0x40023800U)

#define RCC_APB1ENR             *((volatile rmp_u32_t*)(RCC_BASE+0x40U))
#define RCC_APB1ENR_TIM2        (1U<<0)
#define RCC_APB1ENR_TIM4        (1U<<2)

#define TIM2_CR1                *((volatile rmp_u32_t*)(TIM2_BASE+0x00U))
#define TIM2_ARR                *((volatile rmp_u32_t*)(TIM2_BASE+0x2CU))
#define TIM2_PSC                *((volatile rmp_u32_t*)(TIM2_BASE+0x28U))
#define TIM2_CNT                *((volatile rmp_u32_t*)(TIM2_BASE+0x24U))

#define TIM4_CR1                *((volatile rmp_u32_t*)(TIM4_BASE+0x00U))
#define TIM4_ARR                *((volatile rmp_u32_t*)(TIM4_BASE+0x2CU))
#define TIM4_PSC                *((volatile rmp_u32_t*)(TIM4_BASE+0x28U))
#define TIM4_DIER               *((volatile rmp_u32_t*)(TIM4_BASE+0x0CU))
#define TIM4_SR                 *((volatile rmp_u32_t*)(TIM4_BASE+0x10U))
    
#define TIM_CR1_CEN             (1U<<0)
#define TIM_COUNTERMODE_UP      (0U)
#define TIM_COUNTERMODE_DOWN    (1U<<4)
#define TIM_CLOCKDIVISION_DIV1  (0U)
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
rmp_ptr_t Stack_1[256];
rmp_ptr_t Stack_2[256];

void Timer_Init(void);
void Int_Init(void);
void Int_Handler(void);
void Int_Disable(void);
/* End Globals ***************************************************************/

/* Begin Function:Timer_Init **************************************************
Description : Initialize the timer for timing measurements. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Timer_Init(void)
{
    /* TIM2 clock = 1/2 CPU clock */
    TIM2_CR1=TIM_COUNTERMODE_UP|TIM_CLOCKDIVISION_DIV1;
    TIM2_ARR=(unsigned int)(-1);
    TIM2_PSC=0U;
    RCC_APB1ENR|=RCC_APB1ENR_TIM2;
    TIM2_CR1|=TIM_CR1_CEN;
}
/* End Function:Timer_Init ***************************************************/

/* Begin Function:Int_Init ****************************************************
Description : Initialize an periodic interrupt source. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Init(void)
{
    /* Connect the physical interrupt to our machine */
    RMP_ASSERT(RVM_Hyp_Vct_Phys(30U, 2U)==0U);
    /* Set the priority of the physical interrupt and enable it */
    RMP_ASSERT(RVM_A7M_Int_Local_Mod(KFN_INT_LOCAL_MOD, 30U, RVM_A7M_KFN_INT_LOCAL_MOD_SET_STATE, 1U)==0);
    RMP_ASSERT(RVM_A7M_Int_Local_Mod(KFN_INT_LOCAL_MOD, 30U, RVM_A7M_KFN_INT_LOCAL_MOD_SET_PRIO, 0xFFU)==0);
    /* Interrupt generation is initialized too, here we only register our handler */
    RVM_Virt_Vct_Reg(2U, Int_Handler);
    
    /* TIM4 clock = 1/2 CPU clock */
    TIM4_CR1=TIM_COUNTERMODE_DOWN|TIM_CLOCKDIVISION_DIV1;
    TIM4_ARR=21600U;
    TIM4_PSC=0U;
    RCC_APB1ENR|=RCC_APB1ENR_TIM4;
    TIM4_SR&=~0x01U;
    TIM4_DIER|=0x01U;
    TIM4_CR1|=TIM_CR1_CEN;
}
/* End Function:Int_Init *****************************************************/

/* Begin Function:Int_Disable *************************************************
Description : Disable the periodic interrupt source. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Disable(void)
{
    /* Disable interrupt */
    RMP_ASSERT(RVM_A7M_Int_Local_Mod(KFN_INT_LOCAL_MOD, 30U, RVM_A7M_KFN_INT_LOCAL_MOD_SET_STATE, 0U)==0);
    /* Reverse registration */
    RVM_Virt_Vct_Reg(2U,RMP_NULL);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/

