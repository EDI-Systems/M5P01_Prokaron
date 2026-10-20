/******************************************************************************
Filename    : rmp_test_stm32h743zi_rvm.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for STM32H743ZI, running in the RVM. 
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

GCC 9.3.1 -O3 (SysTick turned on, w/FPU context)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 265 / 1680 / 256
Mailbox                            : 448 / 664 / 420
Semaphore                          : 398 / 1884 / 372
FIFO                               : 156 / 348 / 156
Message queue                      : 527 / 1776 / 512
Blocking message queue             : 708 / 2116 / 704
Alarm combination (1/2/3/5/7)      : 534 / 2048 / 324
Memory allocation/free pair        : 272 / 512 / 260
ISR Mailbox                        : 896 / 2204 / 856
ISR Semaphore                      : 915 / 1052 / 844
ISR Message queue                  : 1006 / 1096 / 924
ISR Blocking message queue         : 1136 / 1228 / 1056
ISR RVM activation relay           : 1869 / 3788 / 1736

ARMCC 6.23 -O3 (SysTick turned on, w/FPU context)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 244 / 1568 / 244
Mailbox                            : 389 / 1848 / 376
Semaphore                          : 368 / 1772 / 348
FIFO                               : 160 / 268 / 160
Message queue                      : 595 / 1996 / 580
Blocking message queue             : 803 / 2240 / 780
Alarm combination (1/2/3/5/7)      : 618 / 1840 / 380
Memory allocation/free pair        : 284 / 489 / 271
ISR Mailbox                        : 940 / 1144 / 840
ISR Semaphore                      : 828 / 1000 / 760
ISR Message queue                  : 957 / 2356 / 880
ISR Blocking message queue         : 1066 / 1228 / 984
ISR RVM activation relay           : 1878 / 3404 / 1740

GCC 9.3.1 -O3 (SysTick turned on, w/o FPU context)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 253 / 332 / 252
Mailbox                            : 429 / 1820 / 416
Semaphore                          : 376 / 1580 / 356
FIFO                               : 156 / 1424 / 156
Message queue                      : 499 / 676 / 492
Blocking message queue             : 716 / 1984 / 704
Alarm combination (1/2/3/5/7)      : 534 / 2176 / 316
Memory allocation/free pair        : 272 / 473 / 260
ISR Mailbox                        : 887 / 1128 / 828
ISR Semaphore                      : 891 / 988 / 788
ISR Message queue                  : 915 / 1020 / 840
ISR Blocking message queue         : 1015 / 2148 / 944
ISR RVM activation relay           : 1671 / 3624 / 1576

ARMCC 6.23 -O3 (SysTick turned on, w/o FPU context)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 244 / 344 / 244
Mailbox                            : 389 / 1624 / 376
Semaphore                          : 376 / 1432 / 352
FIFO                               : 160 / 264 / 160
Message queue                      : 594 / 680 / 580
Blocking message queue             : 788 / 1920 / 768
Alarm combination (1/2/3/5/7)      : 614 / 2008 / 388
Memory allocation/free pair        : 284 / 537 / 272
ISR Mailbox                        : 815 / 1004 / 764
ISR Semaphore                      : 735 / 1888 / 708
ISR Message queue                  : 878 / 948 / 840
ISR Blocking message queue         : 1023 / 1112 / 952
ISR RVM activation relay           : 1607 / 3044 / 1540
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
#define RCC_BASE                (0x58024400U)

#define RCC_APB1ENR             *((volatile rmp_u32_t*)(RCC_BASE+0xE8U))
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
    RVM_Virt_Vct_Reg(2U,Int_Handler,RVM_VCT_MASKABLE);
    
    /* TIM4 clock = 1/2 CPU clock */
    TIM4_CR1=TIM_COUNTERMODE_DOWN|TIM_CLOCKDIVISION_DIV1;
    TIM4_ARR=40000U;
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
    RVM_Virt_Vct_Reg(2U,RMP_NULL,RVM_VCT_MASKABLE);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/

