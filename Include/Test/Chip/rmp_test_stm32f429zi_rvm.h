/******************************************************************************
Filename    : rmp_test_stm32f429zi_rvm.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for STM32F429ZI, running in the RVM. 
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

GCC 14.2.1 -O3 (SysTick turned on, w/FPU context)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 416 / 2396 / 416
Mailbox                            : 709 / 2840 / 708
Semaphore                          : 552 / 2696 / 552
FIFO                               : 212 / 2252 / 212
Message queue                      : 827 / 2892 / 824
Blocking message queue             : 1130 / 3196 / 1128
Alarm combination (1/2/3/5/7)      : 760 / 3040 / 476
Memory allocation/free pair        : 382 / 646 / 371
ISR Mailbox                        : 1454 / 3356 / 1452
ISR Semaphore                      : 1364 / 1364 / 1364
ISR Message queue                  : 1543 / 4980 / 1540
ISR Blocking message queue         : 1719 / 3676 / 1716
ISR RVM activation relay           : 2778 / 4596 / 2356

ARMCC 6.23 -O3 (SysTick turned on, w/FPU context)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 336 / 2240 / 336
Mailbox                            : 516 / 2496 / 516
Semaphore                          : 452 / 2484 / 452
FIFO                               : 200 / 2092 / 200
Message queue                      : 824 / 2832 / 824
Blocking message queue             : 1121 / 3104 / 1120
Alarm combination (1/2/3/5/7)      : 809 / 3048 / 484
Memory allocation/free pair        : 391 / 645 / 377
ISR Mailbox                        : 1314 / 3160 / 1312
ISR Semaphore                      : 1224 / 1224 / 1224
ISR Message queue                  : 1418 / 3320 / 1412
ISR Blocking message queue         : 1592 / 1592 / 1592
ISR RVM activation relay           : 2602 / 4364 / 2412

GCC 14.2.1 -O3 (SysTick turned on, w/o FPU context)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 408 / 2092 / 408
Mailbox                            : 704 / 2472 / 704
Semaphore                          : 548 / 2336 / 544
FIFO                               : 212 / 1952 / 212
Message queue                      : 840 / 2612 / 840
Blocking message queue             : 1145 / 2976 / 1144
Alarm combination (1/2/3/5/7)      : 737 / 2960 / 448
Memory allocation/free pair        : 383 / 607 / 371
ISR Mailbox                        : 1350 / 3060 / 1348
ISR Semaphore                      : 1268 / 1268 / 1268
ISR Message queue                  : 1438 / 3128 / 1436
ISR Blocking message queue         : 1608 / 1608 / 1608
ISR RVM activation relay           : 2486 / 4128 / 2420

ARMCC 6.23 -O3 (SysTick turned on, w/o FPU context)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 340 / 1944 / 340
Mailbox                            : 534 / 2344 / 516
Semaphore                          : 456 / 2084 / 456
FIFO                               : 200 / 1840 / 200
Message queue                      : 820 / 2524 / 820
Blocking message queue             : 1145 / 2828 / 1144
Alarm combination (1/2/3/5/7)      : 809 / 2980 / 484
Memory allocation/free pair        : 392 / 606 / 378
ISR Mailbox                        : 1188 / 1628 / 1188
ISR Semaphore                      : 1081 / 2680 / 1080
ISR Message queue                  : 1280 / 1280 / 1280
ISR Blocking message queue         : 1466 / 3064 / 1464
ISR RVM activation relay           : 2209 / 3720 / 2144
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
#define RMP_CNT_READ()          ((rmp_tim_t)((TIM2_CNT)<<1))
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
    RVM_Virt_Vct_Reg(2U,Int_Handler,RVM_VCT_MASKABLE);
    
    /* TIM4 clock = 1/2 CPU clock */
    TIM4_CR1=TIM_COUNTERMODE_DOWN|TIM_CLOCKDIVISION_DIV1;
    TIM4_ARR=18000U;
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
    RMP_ASSERT(RVM_A7M_Int_Local_Mod(KFN_INT_LOCAL_MOD,30U,RVM_A7M_KFN_INT_LOCAL_MOD_STATE_SET,0U)==0);
    /* Reverse registration */
    RVM_Virt_Vct_Reg(2U,RMP_NULL,RVM_VCT_MASKABLE);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
