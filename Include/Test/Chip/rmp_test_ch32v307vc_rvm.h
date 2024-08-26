/******************************************************************************
Filename    : rmp_test_ch32v307vc_rvm.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for CH32V307VC, running in the RVM. 
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
                 <Number>46</Number>
              5. Place the following lines into the RME_Spurious_Handler (or a
                 dedicated handler if you'd wish to use one), so that we clear
                 the interrupt flags as soon as it fires.
                 Note that these addresses coincide with the STM32F405; this is
                 not a copy-paste error!
                 #define TIM4_INTFR     *((volatile rme_u16_t*)(0x40000810U))
                 TIM4_INTFR=0U;
                 return RME_RVM_VCT_SIG_INIT;
              
GCC 12.2.0 -O3 (FPU disabled, fastpath turned on)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 247 / 255 / 247
Mailbox                            : 419 / 457 / 419
Semaphore                          : 361 / 376 / 361
FIFO                               : 169 / 195 / 169
Message queue                      : 538 / 585 / 538
Blocking message queue             : 689 / 738 / 689
Alarm combination (1/2/3/5/7)      : 605 / 1135 / 364
Memory allocation/free pair        : 390 / 400 / 379
ISR Mailbox                        : 1399 / 1436 / 994
ISR Semaphore                      : 1328 / 1349 / 1328
ISR Message queue                  : 1437 / 1451 / 1437
ISR Blocking message queue         : 1545 / 1553 / 1545

GCC 12.2.0 -O3 (FPU enabled, fastpath turned on)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 325 / 340 / 325
Mailbox                            : 497 / 537 / 497
Semaphore                          : 436 / 457 / 436
FIFO                               : 169 / 185 / 169
Message queue                      : 616 / 668 / 616
Blocking message queue             : 767 / 816 / 767
Alarm combination (1/2/3/5/7)      : 608 / 1149 / 367
Memory allocation/free pair        : 390 / 405 / 379
ISR Mailbox                        : 1789 / 1825 / 1162
ISR Semaphore                      : 1742 / 1756 / 1740
ISR Message queue                  : 1857 / 1883 / 1855
ISR Blocking message queue         : 1951 / 1967 / 1951
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Counter read wrapper */
#define RMP_CNT_READ()          (TIM1_CNT)
/* Memory pool test switch */
#define TEST_MEM_POOL           (4096U)
/* Minimal build switch */
/* #define MINIMAL_SIZE */
/* Timestamp data type */
typedef rmp_u16_t rmp_tim_t;

/* Hardware definitions so we don't rely on WCH HAL */
#define TIM1_BASE               (0x40012C00U)
#define TIM4_BASE               (0x40000800U)
#define RCC_BASE                (0x40021000U)

#define RCC_APB2PCENR           *((volatile rmp_u32_t*)(RCC_BASE+0x18U))
#define RCC_APB2PCENR_TIM1      (1U<<11)

#define RCC_APB1PCENR           *((volatile rmp_u32_t*)(RCC_BASE+0x1CU))
#define RCC_APB1PCENR_TIM4      (1U<<2)

#define TIM1_CTLR1              *((volatile rmp_u16_t*)(TIM1_BASE+0x00U))
#define TIM1_SMCFGR             *((volatile rmp_u16_t*)(TIM1_BASE+0x08U))
#define TIM1_CNT                *((volatile rmp_u16_t*)(TIM1_BASE+0x24U))

#define TIM4_CTLR1              *((volatile rmp_u16_t*)(TIM4_BASE+0x00U))
#define TIM4_ATRLR              *((volatile rmp_u16_t*)(TIM4_BASE+0x2CU))
#define TIM4_PSC                *((volatile rmp_u16_t*)(TIM4_BASE+0x28U))
#define TIM4_DMAINTENR          *((volatile rmp_u16_t*)(TIM4_BASE+0x0CU))
#define TIM4_INTFR              *((volatile rmp_u16_t*)(TIM4_BASE+0x10U))
    
#define TIM_CTLR1_CEN           (1U<<0)
#define TIM_COUNTERMODE_UP      (0U)
#define TIM_COUNTERMODE_DOWN    (1U<<4)
#define TIM_CKD_DIV1            (0U)
#define TIM_IT_UPDATE           (1U<<0)
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
rmp_ptr_t Stack_1[1024];
rmp_ptr_t Stack_2[1024];

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
    /* TIM1 clock = CPU clock */
    RCC_APB2PCENR|=RCC_APB2PCENR_TIM1;
    TIM1_CTLR1=TIM_COUNTERMODE_UP;
    TIM1_SMCFGR=0U;
    TIM1_CTLR1|=TIM_CTLR1_CEN;
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
    RMP_ASSERT(RVM_Hyp_Vct_Phys(46U,2U)==0U);
    /* Set the priority of the physical interrupt and enable it */
    RMP_ASSERT(RVM_RV32P_Int_Local_Mod(KFN_INT_LOCAL_MOD,46U,
                                       RVM_RV32P_KFN_INT_LOCAL_MOD_STATE_SET,1U)==0);
    RMP_ASSERT(RVM_RV32P_Int_Local_Mod(KFN_INT_LOCAL_MOD,46U,
                                       RVM_RV32P_KFN_INT_LOCAL_MOD_PRIO_SET,0xFFU)==0);
    /* Interrupt generation is initialized too, here we only register our handler */
    RVM_Virt_Vct_Reg(2U,Int_Handler);
    
    /* TIM4 clock = CPU clock */
    RCC_APB1PCENR|=RCC_APB1PCENR_TIM4;
    
    TIM4_PSC=0U;
    TIM4_ATRLR=14400U*4U;
    TIM4_CTLR1=TIM_COUNTERMODE_DOWN|TIM_CKD_DIV1;
    TIM4_INTFR=0U;
    TIM4_DMAINTENR=TIM_IT_UPDATE;
    TIM4_CTLR1|=TIM_CTLR1_CEN;
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
    RMP_ASSERT(RVM_RV32P_Int_Local_Mod(KFN_INT_LOCAL_MOD,46U,
                                       RVM_RV32P_KFN_INT_LOCAL_MOD_STATE_SET,0U)==0);
    /* Reverse registration */
    RVM_Virt_Vct_Reg(2U,RMP_NULL);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
