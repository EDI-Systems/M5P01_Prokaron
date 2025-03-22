/******************************************************************************
Filename    : rmp_test_stm32l071cb_rvm.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for STM32L071CB, running in the RVM. Takes ~3min.
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
                 <Name>TIM21</Name>
                 <Number>20</Number>
              5. Place the following lines into the RME_Spurious_Handler (or a
                 dedicated handler if you'd wish to use one), so that we clear
                 the interrupt flags as soon as it fires.
                 #define TIM21_SR    *((volatile rme_u32_t*)(0x40010810U))
                 TIM21_SR=0U;
                 return RME_RVM_VCT_SIG_INIT;
              These steps can be automatically completed by the RVM generator.

ARMCC 6.19 -Oz (SysTick turned on)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 384 / 2603 / 382
Mailbox                            : 706 / 2953 / 701
Semaphore                          : 613 / 2866 / 609
FIFO                               : 304 / 2552 / 302
Message queue                      : 1013 / 3261 / 1007
Blocking message queue             : 1356 / 5102 / 1347
Alarm combination (1/2/3/5/7)      : 1224 / 4156 / 735
ISR Mailbox                        : 1401 / 5128 / 1391
ISR Semaphore                      : 1323 / 3551 / 1314
ISR Message queue                  : 1576 / 3819 / 1566
ISR Blocking message queue         : 1772 / 4015 / 1760
ISR RVM activation relay           : 2632 / 5354 / 2466

GCC 13.2.1 -Os (SysTick turned on)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 401 / 2241 / 400
Mailbox                            : 756 / 2624 / 751
Semaphore                          : 653 / 2521 / 649
FIFO                               : 322 / 2192 / 321
Message queue                      : 1070 / 2937 / 1064
Blocking message queue             : 1427 / 3293 / 1420
Alarm combination (1/2/3/5/7)      : 1299 / 6105 / 770
ISR Mailbox                        : 1458 / 4850 / 1450
ISR Semaphore                      : 1375 / 3241 / 1368
ISR Message queue                  : 1638 / 3502 / 1629
ISR Blocking message queue         : 1833 / 5236 / 1823
ISR RVM activation relay           : 2636 / 4953 / 2541
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
#define TIM21_BASE              (0x40010800U)
#define RCC_BASE                (0x40021000U)

#define RCC_APB1ENR             *((volatile rmp_u32_t*)(RCC_BASE+0x38U))
#define RCC_APB2ENR             *((volatile rmp_u32_t*)(RCC_BASE+0x34U))
#define RCC_APB1ENR_TIM2        (1U<<0)
#define RCC_APB2ENR_TIM21       (1U<<2)

#define TIM2_CR1                *((volatile rmp_u32_t*)(TIM2_BASE+0x00U))
#define TIM2_ARR                *((volatile rmp_u32_t*)(TIM2_BASE+0x2CU))
#define TIM2_PSC                *((volatile rmp_u32_t*)(TIM2_BASE+0x28U))
#define TIM2_CNT                *((volatile rmp_u32_t*)(TIM2_BASE+0x24U))

#define TIM21_CR1               *((volatile rmp_u32_t*)(TIM21_BASE+0x00U))
#define TIM21_ARR               *((volatile rmp_u32_t*)(TIM21_BASE+0x2CU))
#define TIM21_PSC               *((volatile rmp_u32_t*)(TIM21_BASE+0x28U))
#define TIM21_DIER              *((volatile rmp_u32_t*)(TIM21_BASE+0x0CU))
#define TIM21_SR                *((volatile rmp_u32_t*)(TIM21_BASE+0x10U))
    
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
    RMP_ASSERT(RVM_Hyp_Vct_Phys(20U,2U)==0U);
    /* Set the priority of the physical interrupt and enable it */
    RMP_ASSERT(RVM_A6M_Int_Local_Mod(KFN_INT_LOCAL_MOD,20U,
                                     RVM_A6M_KFN_INT_LOCAL_MOD_PRIO_SET,0xFFU)==0);
    RMP_ASSERT(RVM_A6M_Int_Local_Mod(KFN_INT_LOCAL_MOD,20U,
                                     RVM_A6M_KFN_INT_LOCAL_MOD_STATE_SET,1U)==0);
    /* Interrupt generation is initialized too, here we only register our handler */
    RVM_Virt_Vct_Reg(2U,Int_Handler);
    
    /* TIM21 clock = CPU clock */
    TIM21_CR1=TIM_COUNTERMODE_DOWN|TIM_CLOCKDIVISION_DIV1;
    TIM21_ARR=3600U*4U;
    TIM21_PSC=0U;
    RCC_APB2ENR|=RCC_APB2ENR_TIM21;
    TIM21_SR&=~0x01U;
    TIM21_DIER|=0x01U;
    TIM21_CR1|=TIM_CR1_CEN;
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
    RMP_ASSERT(RVM_A6M_Int_Local_Mod(KFN_INT_LOCAL_MOD,20U, 
                                     RVM_A6M_KFN_INT_LOCAL_MOD_STATE_SET,0U)==0);
    /* Reverse registration */
    RVM_Virt_Vct_Reg(2U,RMP_NULL);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
