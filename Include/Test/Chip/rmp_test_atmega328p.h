/******************************************************************************
Filename    : rmp_test_atmega328p.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for ATMEGA328P.
              This test takes 1 min @16 MHz. Just observe how slow the AVR is.
              This part is particularly meager with just 32KiB flash and 2 KiB
              SRAM. Should your compilation fail altogether or encounters weird
              bugs that fail asserts, check the compiler or runtime use more
              RAM than the version that was used to run the figures below.
              Also, if you're using Arduino and want to debugWIRE the board,
              make sure to remove the 100nF capacitor between the nRESET pin
              and the USB-Serial or the debugWIRE will refuse to connect.
              Please refer to the schematic for more details.

GCC 4.7.4 (Atmel Studio GNU 5.4.0) -O3
   ___   __  ___ ___
  / _ \ /  |/  // _ \       Simple real-time kernel
 / , _// /|_/ // ___/       Standard benchmark test
/_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 408 / 682 / 408
Mailbox                            : 719 / 979 / 705
Semaphore                          : 686 / 946 / 672
FIFO                               : 313 / 581 / 307
Message queue                      : 1065 / 1317 / 1043
Blocking message queue             : 1318 / 1565 / 1291
ISR Mailbox                        : 624 / 882 / 608
ISR Semaphore                      : 626 / 884 / 610
ISR Message queue                  : 905 / 1158 / 884
ISR Blocking message queue         : 1073 / 1321 / 1047

GCC 4.7.4 (Atmel Studio GNU 5.4.0) -Os -mcall-prologues
   ___   __  ___ ___
  / _ \ /  |/  // _ \       Simple real-time kernel
 / , _// /|_/ // ___/       Standard benchmark test
/_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 399 / 647 / 399
Mailbox                            : 761 / 995 / 747
Semaphore                          : 709 / 944 / 696
FIFO                               : 309 / 552 / 304
Message queue                      : 1154 / 1381 / 1133
Blocking message queue             : 1483 / 1703 / 1455
ISR Mailbox                        : 656 / 890 / 642
ISR Semaphore                      : 620 / 854 / 606
ISR Message queue                  : 926 / 1155 / 907
ISR Blocking message queue         : 1110 / 1339 / 1091
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Counter read wrapper */
#define RMP_CNT_READ()      ((rmp_tim_t)(TCNT1))
/* Memory pool test switch */
/* #define TEST_MEM_POOL */
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
rmp_ptr_t Stack_1[80];
rmp_ptr_t Stack_2[80];

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
