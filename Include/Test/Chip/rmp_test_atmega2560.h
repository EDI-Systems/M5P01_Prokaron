/******************************************************************************
Filename    : rmp_test_atmega2560.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for ATMEGA2560.
              This test takes 1 min @16 MHz. Just observe how slow the AVR is.

GCC 4.7.4 (Atmel Studio GNU 5.4.0) -O3
   ___   __  ___ ___
  / _ \ /  |/  // _ \       Simple real-time kernel
 / , _// /|_/ // ___/       Standard benchmark test
/_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 449 / 763 / 449
Mailbox                            : 774 / 1070 / 756
Semaphore                          : 736 / 1033 / 719
FIFO                               : 326 / 633 / 319
Message queue                      : 1131 / 1418 / 1104
Blocking message queue             : 1396 / 1677 / 1363
Memory allocation/free pair        : 1686 / 1809 / 1565
ISR Mailbox                        : 656 / 952 / 638
ISR Semaphore                      : 654 / 950 / 636
ISR Message queue                  : 942 / 1230 / 916
ISR Blocking message queue         : 1117 / 1401 / 1087

GCC 4.7.4 (Atmel Studio GNU 5.4.0) -Os -mcall-prologues
   ___   __  ___ ___
  / _ \ /  |/  // _ \       Simple real-time kernel
 / , _// /|_/ // ___/       Standard benchmark test
/_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 442 / 731 / 442
Mailbox                            : 825 / 1096 / 807
Semaphore                          : 768 / 1041 / 752
FIFO                               : 331 / 613 / 324
Message queue                      : 1246 / 1508 / 1219
Blocking message queue             : 1596 / 1850 / 1561
Memory allocation/free pair        : 2253 / 2395 / 2128
ISR Mailbox                        : 694 / 966 / 677
ISR Semaphore                      : 655 / 927 / 638
ISR Message queue                  : 977 / 1242 / 953
ISR Blocking message queue         : 1176 / 1437 / 1148
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Counter read wrapper */
#define RMP_CNT_READ()      ((rmp_tim_t)(TCNT1))
/* Memory pool test switch */
#define TEST_MEM_POOL       (2048U)
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

void Timer_Init(void);
void Int_Init(void);
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
    TCCR1A=0x00U;
    TCCR1B=0x01U;
    TCCR1C=0x00U;
    TCNT1=0x0000U;
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
    /* TIM2 clock = 1/256 CPU clock */
    TCNT2=0x00U;
    OCR2A=64U;
    TIFR2=0x00U;
    TCCR2A=0x02U;
    TCCR2B=0x06U;
    TIMSK2=0x02U;
}

/* The interrupt handler */
void TIM2_Handler(void)
{
    /* Flag is auto cleared upon entry so this is not really needed */
    TIFR2=0xFFU;
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
    TIMSK2=0x00U;
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
