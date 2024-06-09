/******************************************************************************
Filename    : rmp_test_hc32l136k8.h
Author      : hrs 
Date        : 31/05/2024
Licence     : LGPL v3+; see COPYING for details.
Description : The testbench for HC32L136K8.

ARMCC V6.19 -O3 
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 211 / 292 / 211
Mailbox                            : 422 / 500 / 421
Semaphore                          : 370 / 451 / 369
FIFO                               : 219 / 302 / 219
Message queue                      : 646 / 728 / 645
Blocking message queue             : 873 / 956 / 871
Memory allocation/free pair        : 542 / 576 / 532
ISR Mailbox                        : 403 / 486 / 402
ISR Semaphore                      : 350 / 435 / 349
ISR Message queue                  : 532 / 617 / 531
ISR Blocking message queue         : 673 / 757 / 671
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Counter read wrapper */
#define RMP_CNT_READ()      (M0P_TIM0_MODE0->CNT_f.CNT)
/* Memory pool test switch */
#define TEST_MEM_POOL       (1024U)
/* Minimal build switch */
/* #define MINIMAL_SIZE */
/* Pointer size switch */
#define PTR_16_BIT
/* Timestamp data type */
typedef rmp_u16_t rmp_tim_t;
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
rmp_ptr_t Stack_1[256];
rmp_ptr_t Stack_2[256];
stc_bt_mode0_cfg_t TIM0_Init_Struct;
stc_tim3_mode0_cfg_t TIM3_Init_Struct;

void Timer_Init(void);
void Int_Init(void);
void Int_Handler(void);
void Int_Disable(void);
void Tim0_IRQHandler(void);
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
    /* Initialize TIM init struct. */
    DDL_ZERO_STRUCT(TIM0_Init_Struct);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralBaseTim,TRUE);
    
    /* Fill base timer init struct. */
    TIM0_Init_Struct.enWorkMode=BtWorkMode0;
    TIM0_Init_Struct.enCT=BtTimer;
    TIM0_Init_Struct.enPRS=BtPCLKDiv1;
    TIM0_Init_Struct.enCntMode=Bt16bitArrMode;
    TIM0_Init_Struct.bEnTog=FALSE;
    TIM0_Init_Struct.bEnGate=FALSE;
    TIM0_Init_Struct.enGateP=BtGatePositive;
    Bt_Mode0_Init(TIM0,&TIM0_Init_Struct);
    
    Bt_M0_ARRSet(TIM0,0U);
    Bt_M0_Cnt16Set(TIM0,0U);
    
    Bt_M0_Run(TIM0); 
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
    /* Initialize TIM3 init struct and enable peripheral clock. */
    DDL_ZERO_STRUCT(TIM3_Init_Struct);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3,TRUE);

    /* Fill timer3 init struct. */
    TIM3_Init_Struct.enWorkMode=Tim3WorkMode0;
    TIM3_Init_Struct.enCT=Tim3Timer;
    TIM3_Init_Struct.enPRS=Tim3PCLKDiv1;
    TIM3_Init_Struct.enCntMode=Bt16bitArrMode;
    TIM3_Init_Struct.bEnTog=FALSE;
    TIM3_Init_Struct.bEnGate=FALSE;
    TIM3_Init_Struct.enGateP=Tim3GatePositive;
    Tim3_Mode0_Init(&TIM3_Init_Struct);
    
    Tim3_M0_ARRSet(0xFFFFU-2400U);
    Tim3_M0_Cnt16Set(0xFFFFU-2400U);
    
    Tim3_ClearIntFlag(Tim3UevIrq);
    Tim3_Mode0_EnableIrq();
    NVIC_ClearPendingIRQ(TIM3_IRQn);
    NVIC_SetPriority(TIM3_IRQn, 0xFFU);
    NVIC_EnableIRQ(TIM3_IRQn);
    
    Tim3_M0_Run();
}

/* The interrupt handler */
void TIM3_IRQHandler(void)
{
    Tim3_ClearIntFlag(Tim3UevIrq);
    Int_Handler();
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
    /* Disable timer 3 interrupt */
    Tim3_Mode0_DisableIrq();
    NVIC_DisableIRQ(TIM3_IRQn);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
