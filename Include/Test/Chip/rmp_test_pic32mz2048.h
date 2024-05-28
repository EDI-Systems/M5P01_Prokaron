/******************************************************************************
Filename    : rmp_test_pic32mz2048efm100.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for PIC32MZ2048EFM100.
              This port is considered experimental and without FPU support.

XC32-GCC 4.35 O3(--specs hack disabled)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 285 / 435 / 285
Mailbox                            : 402 / 820 / 395
Semaphore                          : 373 / 745 / 370
FIFO                               : 151 / 590 / 150
Message queue                      : 579 / 815 / 575
Blocking message queue             : 735 / 970 / 730
Memory allocation/free pair        : 471 / 721 / 458
ISR Mailbox                        : 445 / 1190 / 440
ISR Semaphore                      : 425 / 720 / 420
ISR Message queue                  : 550 / 795 / 545
ISR Blocking message queue         : 645 / 825 / 640

XC32-GCC 4.35 O3(--specs hack enabled)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 260 / 440 / 255
Mailbox                            : 392 / 1135 / 390
Semaphore                          : 370 / 935 / 370
FIFO                               : 146 / 650 / 145
Message queue                      : 540 / 1195 / 535
Blocking message queue             : 672 / 1015 / 665
Memory allocation/free pair        : 364 / 665 / 352
ISR Mailbox                        : 440 / 1230 / 435
ISR Semaphore                      : 420 / 925 / 415
ISR Message queue                  : 530 / 1350 / 525
ISR Blocking message queue         : 620 / 840 / 615
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Counter read wrapper */
#define RMP_CNT_READ()      (TMR1*5U)
/* Memory pool test switch */
#define TEST_MEM_POOL       (8192U)
/* Minimal build switch */
/* #define MINIMAL_SIZE */
/* Timestamp data type */
typedef rmp_u16_t rmp_tim_t;

/* The pragmas for PIC32 fuse */
/*** DEVCFG0 ***/
#pragma config DEBUG =      OFF
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx2
#pragma config TRCEN =      OFF
#pragma config BOOTISA =    MIPS32
#pragma config FECCCON =    OFF_UNLOCKED
#pragma config FSLEEP =     OFF
#pragma config DBGPER =     ALLOW_PG2
#pragma config SMCLR =      MCLR_NORM
#pragma config SOSCGAIN =   GAIN_2X
#pragma config SOSCBOOST =  ON
#pragma config POSCGAIN =   GAIN_2X
#pragma config POSCBOOST =  ON
#pragma config EJTAGBEN =   NORMAL
#pragma config CP =         OFF

/*** DEVCFG1 ***/
#pragma config FNOSC =      SPLL
#pragma config DMTINTV =    WIN_127_128
#pragma config FSOSCEN =    OFF
#pragma config IESO =       OFF
#pragma config POSCMOD =    EC
#pragma config OSCIOFNC =   OFF
#pragma config FCKSM =      CSECME
#pragma config WDTPS =      PS1048576
#pragma config WDTSPGM =    STOP
#pragma config FWDTEN =     OFF
#pragma config WINDIS =     NORMAL
#pragma config FWDTWINSZ =  WINSZ_25
#pragma config DMTCNT =     DMT9
#pragma config FDMTEN =     OFF

/*** DEVCFG2 ***/
#pragma config FPLLIDIV =   DIV_3
#pragma config FPLLRNG =    RANGE_8_16_MHZ
#pragma config FPLLICLK =   PLL_POSC
#pragma config FPLLMULT =   MUL_50
#pragma config FPLLODIV =   DIV_2
#pragma config UPLLFSEL =   FREQ_24MHZ

/*** DEVCFG3 ***/
#pragma config USERID =     0xffff
#pragma config FMIIEN =     OFF
#pragma config FETHIO =     ON
#pragma config PGL1WAY =    ON
#pragma config PMDL1WAY =   ON
#pragma config IOL1WAY =    ON
#pragma config FUSBIDIO =   OFF

/*** BF1SEQ0 ***/
#pragma config TSEQ =       0x0000
#pragma config CSEQ =       0xffff
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
rmp_ptr_t Stack_1[512];
rmp_ptr_t Stack_2[512];

void Timer_Init(void);
void Int_Init(void);
void Int_Handler(void);
void Int_Disable(void);
void Tim2_Interrupt(void);
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
    /* TIM1 clock = 1/5 CPU clock */
    T1CON=0;
    TMR1=0;
    PR1=0xFFFF;
    /* Start the timer */
    T1CONSET=(1<<15);
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
    /* TIM2 clock = 1/5 CPU clock */
    T2CON=0;
    TMR2=0;
    PR2=20000;
    /* Lowest interrupt level */
    IPC2SET=(1<<_IPC2_T2IP_POSITION)|(0<<_IPC2_T2IS_POSITION);
    IFS0CLR=_IFS0_T2IF_MASK;
    IEC0SET=(1<<_IEC0_T2IE_POSITION);
    /* Start the timer */
    T2CONSET=(1<<15);
}

/* The interrupt handler with shadow register sets */
void Tim2_Interrupt(void)
{
    Int_Handler();
    /* Clear flags */
    IFS0CLR=_IFS0_T2IF_MASK;
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
    /* Disable timer 2 interrupt */
    IEC0CLR=_IEC0_T2IE_MASK;
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
