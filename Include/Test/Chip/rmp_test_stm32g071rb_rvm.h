/******************************************************************************
Filename    : rmp_test_stm32g071rb_rvm.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for STM32G071RB, running in the RVM. Takes ~3min.
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
                 <Name>TIM3</Name>
                 <Number>20</Number>
              5. Place the following lines into the RME_Spurious_Handler (or a
                 dedicated handler if you'd wish to use one), so that we clear
                 the interrupt flags as soon as it fires.
                 #define TIM3_SR    *((volatile rme_u32_t*)(0x40000410U))
                 TIM3_SR=0U;
                 return RME_RVM_VCT_SIG_INIT;
              These steps can be automatically completed by the RVM generator.

GCC 9.3.1 -Os (SysTick turned on)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 463 / 2498 / 462
Mailbox                            : 887 / 2974 / 884
Semaphore                          : 753 / 2788 / 752
FIFO                               : 378 / 2415 / 377
Message queue                      : 1225 / 6742 / 1224
Blocking message queue             : 1650 / 3736 / 1645
Alarm combination (1/2/3/5/7)      : 1530 / 4472 / 922
ISR Mailbox                        : 1637 / 3721 / 1631
ISR Semaphore                      : 1523 / 3610 / 1517
ISR Message queue                  : 1822 / 3890 / 1817
ISR Blocking message queue         : 2038 / 4123 / 2037
ISR RVM activation relay           : 2971 / 5035 / 2952

ARMCC 6.23 -Oz (SysTick turned on)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 427 / 2806 / 426
Mailbox                            : 804 / 3133 / 801
Semaphore                          : 689 / 3066 / 687
FIFO                               : 349 / 2729 / 348
Message queue                      : 1143 / 3464 / 1139
Blocking message queue             : 1546 / 3917 / 1540
Alarm combination (1/2/3/5/7)      : 1385 / 4517 / 846
ISR Mailbox                        : 1540 / 2166 / 1540
ISR Semaphore                      : 1444 / 4941 / 1438
ISR Message queue                  : 1729 / 4098 / 1722
ISR Blocking message queue         : 1956 / 5940 / 1947
ISR RVM activation relay           : 2897 / 5247 / 2887
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
#define RMP_CNT_READ()          ((rmp_tim_t)(TIM2_CNT))
/* Memory pool test switch */
/* #define TEST_MEM_POOL */
/* Minimal build switch */
/* #define MINIMAL_SIZE */
/* Timestamp data type - STM32L0 timers are mixed, we treat them as 16-bits */
typedef rmp_u16_t rmp_tim_t;

/* Hardware definitions so we don't rely on STM32 HAL */
#define TIM2_BASE               (0x40000000U)
#define TIM3_BASE               (0x40000400U)
#define RCC_BASE                (0x40021000U)

#define RCC_APBENR1             *((volatile rmp_u32_t*)(RCC_BASE+0x3CU))
#define RCC_APBENR1             *((volatile rmp_u32_t*)(RCC_BASE+0x3CU))
#define RCC_APBENR1_TIM2        (1U<<0)
#define RCC_APBENR1_TIM3        (1U<<1)

#define TIM2_CR1                *((volatile rmp_u32_t*)(TIM2_BASE+0x00U))
#define TIM2_ARR                *((volatile rmp_u32_t*)(TIM2_BASE+0x2CU))
#define TIM2_PSC                *((volatile rmp_u32_t*)(TIM2_BASE+0x28U))
#define TIM2_CNT                *((volatile rmp_u32_t*)(TIM2_BASE+0x24U))

#define TIM3_CR1               *((volatile rmp_u32_t*)(TIM3_BASE+0x00U))
#define TIM3_ARR               *((volatile rmp_u32_t*)(TIM3_BASE+0x2CU))
#define TIM3_PSC               *((volatile rmp_u32_t*)(TIM3_BASE+0x28U))
#define TIM3_DIER              *((volatile rmp_u32_t*)(TIM3_BASE+0x0CU))
#define TIM3_SR                *((volatile rmp_u32_t*)(TIM3_BASE+0x10U))
    
#define TIM_CR1_CEN             (1U<<0)
#define TIM_COUNTERMODE_UP      (0U)
#define TIM_COUNTERMODE_DOWN    (1U<<4)
#define TIM_CLOCKDIVISION_DIV1  (0U)
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
rmp_ptr_t Stack_1[128];
rmp_ptr_t Stack_2[128];

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
    /* TIM2 clock = CPU clock */
    TIM2_CR1=TIM_COUNTERMODE_UP|TIM_CLOCKDIVISION_DIV1;
    TIM2_ARR=(unsigned int)(-1);
    TIM2_PSC=0U;
    RCC_APBENR1|=RCC_APBENR1_TIM2;
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
    RMP_ASSERT(RVM_Hyp_Vct_Phys(16U,2U)==0U);
    /* Set the priority of the physical interrupt and enable it */
    RMP_ASSERT(RVM_A6M_Int_Local_Mod(KFN_INT_LOCAL_MOD,16U,
                                     RVM_A6M_KFN_INT_LOCAL_MOD_PRIO_SET,0xFFU)==0);
    RMP_ASSERT(RVM_A6M_Int_Local_Mod(KFN_INT_LOCAL_MOD,16U,
                                     RVM_A6M_KFN_INT_LOCAL_MOD_STATE_SET,1U)==0);
    /* Interrupt generation is initialized too, here we only register our handler */
    RVM_Virt_Vct_Reg(2U,Int_Handler,RVM_VCT_MASKABLE);
    
    /* TIM3 clock = CPU clock */
    TIM3_CR1=TIM_COUNTERMODE_DOWN|TIM_CLOCKDIVISION_DIV1;
    TIM3_ARR=12800U;
    TIM3_PSC=0U;
    RCC_APBENR1|=RCC_APBENR1_TIM3;
    TIM3_SR&=~0x01U;
    TIM3_DIER|=0x01U;
    TIM3_CR1|=TIM_CR1_CEN;
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
    RMP_ASSERT(RVM_A6M_Int_Local_Mod(KFN_INT_LOCAL_MOD,16U, 
                                     RVM_A6M_KFN_INT_LOCAL_MOD_STATE_SET,0U)==0);
    /* Reverse registration */
    RVM_Virt_Vct_Reg(2U,RMP_NULL,RVM_VCT_MASKABLE);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
