/******************************************************************************
Filename    : rmp_test_atmega1284p.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for ATMEGA1284P.
              This test takes 1 min @16 MHz. Just observe how slow the AVR is.

GCC 4.7.4 (Atmel Studio GNU 5.4.0) -O3
   ___   __  ___ ___
  / _ \ /  |/  // _ \       Simple real-time kernel
 / , _// /|_/ // ___/       Standard benchmark test
/_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 437 / 739 / 437
Mailbox                            : 751 / 1036 / 734
Semaphore                          : 717 / 1003 / 701
FIFO                               : 314 / 609 / 307
Message queue                      : 1098 / 1375 / 1073
Blocking message queue             : 1352 / 1623 / 1321
Memory allocation/free pair        : 1680 / 1809 / 1558
ISR Mailbox                        : 637 / 922 / 620
ISR Semaphore                      : 639 / 924 / 622
ISR Message queue                  : 921 / 1198 / 896
ISR Blocking message queue         : 1087 / 1361 / 1059

GCC 4.7.4 (Atmel Studio GNU 5.4.0) -Os -mcall-prologues
   ___   __  ___ ___
  / _ \ /  |/  // _ \       Simple real-time kernel
 / , _// /|_/ // ___/       Standard benchmark test
/_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 428 / 705 / 428
Mailbox                            : 793 / 1054 / 777
Semaphore                          : 740 / 1002 / 725
FIFO                               : 314 / 585 / 308
Message queue                      : 1193 / 1445 / 1168
Blocking message queue             : 1522 / 1767 / 1490
Memory allocation/free pair        : 2220 / 2354 / 2097
ISR Mailbox                        : 671 / 931 / 654
ISR Semaphore                      : 634 / 895 / 618
ISR Message queue                  : 944 / 1198 / 921
ISR Blocking message queue         : 1132 / 1382 / 1105
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
