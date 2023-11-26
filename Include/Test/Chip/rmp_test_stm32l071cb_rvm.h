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

ARMCC 6.18 -O3  (SysTick turned off to avoid spurious interrupts)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 1708 / 1708 / 1708
Mailbox                            : 2030 / 2030 / 2030
Semaphore                          : 1931 / 1931 / 1931
FIFO                               : 325 / 326 / 325
Message queue                      : 2318 / 2318 / 2318
Blocking message queue             : 2661 / 2661 / 2661
ISR Mailbox                        : 1372 / 1372 / 1372
ISR Semaphore                      : 1289 / 1289 / 1289
ISR Message queue                  : 1528 / 1528 / 1528
ISR Blocking message queue         : 1735 / 1735 / 1735

ARMCC 6.18 -O3 (SysTick turned on, may kick in between)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 1713 / 3448 / 1712
Mailbox                            : 2037 / 3772 / 2036
Semaphore                          : 1937 / 3673 / 1936
FIFO                               : 327 / 2007 / 327
Message queue                      : 2325 / 4075 / 2324
Blocking message queue             : 2670 / 4351 / 2669
ISR Mailbox                        : 1377 / 3040 / 1377
ISR Semaphore                      : 1293 / 2956 / 1293
ISR Message queue                  : 1533 / 3196 / 1533
ISR Blocking message queue         : 1741 / 3477 / 1741

ARMCC 6.18 -O3 (SysTick turned on, fastpath enabled)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 469 / 2145 / 467
Mailbox                            : 841 / 2569 / 837
Semaphore                          : 717 / 5154 / 711
FIFO                               : 391 / 2121 / 389
Message queue                      : 1143 / 2870 / 1138
Blocking message queue             : 1529 / 3257 / 1521
ISR Mailbox                        : 1484 / 5909 / 1476
ISR Semaphore                      : 1376 / 3104 / 1371
ISR Message queue                  : 1646 / 4815 / 1638
ISR Blocking message queue         : 1867 / 4979 / 1859
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
#include "rvm_guest.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* How to read counter */
#define RMP_CNT_READ()          ((rmp_tim_t)(TIM2_CNT))
/* Are we testing the memory pool? */
/* #define TEST_MEM_POOL     8192 */
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The STM32L0 timers are mixed and we treat them as 16-bits */
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
               RVM_A6M_KFN_INT_LOCAL_MOD_SET_PRIO,0xFFU)==0);
    RMP_ASSERT(RVM_A6M_Int_Local_Mod(KFN_INT_LOCAL_MOD,20U,
               RVM_A6M_KFN_INT_LOCAL_MOD_SET_STATE,1U)==0);
    /* Interrupt generation is initialized too, here we only register our handler */
    RVM_Virt_Vct_Reg(2U, Int_Handler);
    
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
    RMP_ASSERT(RVM_A6M_Int_Local_Mod(KFN_INT_LOCAL_MOD, 20U, RVM_A6M_KFN_INT_LOCAL_MOD_SET_STATE, 0U)==0);
    /* Reverse registration */
    RVM_Virt_Vct_Reg(2U,RMP_NULL);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
