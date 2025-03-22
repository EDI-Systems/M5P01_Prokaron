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
              These steps can be automatically completed by the RVM generator.

ARMCC 6.19 -O3 (SysTick turned on, w/FPU context)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 178 / 1488 / 176
Mailbox                            : 305 / 1692 / 284
Semaphore                          : 267 / 1536 / 256
FIFO                               : 148 / 1484 / 148
Message queue                      : 509 / 1760 / 480
Blocking message queue             : 722 / 748 / 696
Alarm combination (1/2/3/5/7)      : 571 / 2128 / 340
Memory allocation/free pair        : 275 / 521 / 263
ISR Mailbox                        : 864 / 2228 / 824
ISR Semaphore                      : 802 / 892 / 768
ISR Message queue                  : 928 / 2292 / 876
ISR Blocking message queue         : 1021 / 2324 / 984
ISR RVM activation relay           : 1846 / 3124 / 1776

GCC 13.2.1 -O3 (SysTick turned on, w/FPU context)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 184 / 1464 / 184
Mailbox                            : 366 / 1860 / 316
Semaphore                          : 308 / 1740 / 292
FIFO                               : 157 / 364 / 148
Message queue                      : 470 / 1908 / 444
Blocking message queue             : 644 / 2032 / 608
Alarm combination (1/2/3/5/7)      : 523 / 1912 / 304
Memory allocation/free pair        : 271 / 543 / 259
ISR Mailbox                        : 957 / 1164 / 872
ISR Semaphore                      : 951 / 2372 / 848
ISR Message queue                  : 1083 / 1220 / 992
ISR Blocking message queue         : 1166 / 2612 / 1072
ISR RVM activation relay           : 1797 / 2144 / 1720

ARMCC 6.19 -O3 (SysTick turned on, w/o FPU context)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 179 / 1400 / 176
Mailbox                            : 315 / 1508 / 284
Semaphore                          : 271 / 1536 / 256
FIFO                               : 148 / 252 / 148
Message queue                      : 503 / 1680 / 496
Blocking message queue             : 697 / 1948 / 676
Alarm combination (1/2/3/5/7)      : 572 / 2188 / 340
Memory allocation/free pair        : 275 / 461 / 263
ISR Mailbox                        : 819 / 960 / 784
ISR Semaphore                      : 776 / 2948 / 736
ISR Message queue                  : 908 / 996 / 856
ISR Blocking message queue         : 1003 / 2160 / 952
ISR RVM activation relay           : 1709 / 1964 / 1616

GCC 13.2.1 -O3 (SysTick turned on, w/o FPU context)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 184 / 1356 / 184
Mailbox                            : 356 / 1580 / 316
Semaphore                          : 309 / 1628 / 288
FIFO                               : 148 / 1300 / 148
Message queue                      : 469 / 1640 / 448
Blocking message queue             : 637 / 1848 / 608
Alarm combination (1/2/3/5/7)      : 525 / 1728 / 312
Memory allocation/free pair        : 272 / 520 / 261
ISR Mailbox                        : 808 / 2076 / 772
ISR Semaphore                      : 796 / 880 / 760
ISR Message queue                  : 987 / 1120 / 920
ISR Blocking message queue         : 1068 / 2392 / 984
ISR RVM activation relay           : 1525 / 1972 / 1480
******************************************************************************/

/* Include *******************************************************************/
#include "rvm.h"
#include "rmp.h"

#define RVM_TEST_VIRTUAL_INC
#include "rvm_test.h"
#undef RVM_TEST_VIRTUAL_INC

#define RVM_BENCHMARK_VIRTUAL_INC
#include "Test/rvm_benchmark.h"
#undef RVM_BENCHMARK_VIRTUAL_INC
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Counter read wrapper */
#define RMP_CNT_READ()          ((TIM2_CNT)<<1)
/* Memory pool test switch */
#define TEST_MEM_POOL           (8192U)
/* Minimal build switch */
/* #define MINIMAL_SIZE */
/* Timestamp data type */
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
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
rmp_ptr_t Stack_1[256];
rmp_ptr_t Stack_2[256];

void Timer_Init(void);
void Int_Init(void);
void Int_Handler(void);
void Int_Disable(void);
/* End Global ****************************************************************/

/* Function:Timer_Init ********************************************************
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

/* Function:Int_Init **********************************************************
Description : Initialize an periodic interrupt source. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Init(void)
{
    /* Connect the physical interrupt to our machine */
    RMP_ASSERT(RVM_Hyp_Vct_Phys(30U,2U)==0U);
    /* Set the priority of the physical interrupt and enable it */
    RMP_ASSERT(RVM_A7M_Int_Local_Mod(KFN_INT_LOCAL_MOD,30U,
                                     RVM_A7M_KFN_INT_LOCAL_MOD_STATE_SET,1U)==0);
    RMP_ASSERT(RVM_A7M_Int_Local_Mod(KFN_INT_LOCAL_MOD,30U,
                                     RVM_A7M_KFN_INT_LOCAL_MOD_PRIO_SET,0xFFU)==0);
    /* Interrupt generation is initialized too, here we only register our handler */
    RVM_Virt_Vct_Reg(2U,Int_Handler);
    
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

/* Function:Int_Disable *******************************************************
Description : Disable the periodic interrupt source. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Disable(void)
{
    /* Disable interrupt */
    RMP_ASSERT(RVM_A7M_Int_Local_Mod(KFN_INT_LOCAL_MOD,30U,
                                     RVM_A7M_KFN_INT_LOCAL_MOD_STATE_SET,0U)==0);
    /* Reverse registration */
    RVM_Virt_Vct_Reg(2U,RMP_NULL);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/

