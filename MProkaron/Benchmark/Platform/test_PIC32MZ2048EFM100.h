/******************************************************************************
Filename    : test_PIC32MZ2048EFM100.h
Author      : pry 
Date        : 22/07/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The testbench for PIC32MZ2048EFM100.
******************************************************************************/

/* Includes ******************************************************************/
#include "RMP.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Where are the initial stacks */
#define THD1_STACK          (&Stack_1[300])
#define THD2_STACK          (&Stack_2[300])
/* How to read counter */
#define COUNTER_READ()      (TMR1*5)
/* Are we testing the memory pool? */
#define TEST_MEM_POOL       8192
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The PIC32 timers are all 16 bits, so */
typedef u16 tim_t;

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
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
ptr_t Stack_1[512];
ptr_t Stack_2[512];
/* End Globals ***************************************************************/

/* Begin Function:Timer_Init **************************************************
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

/* Begin Function:Int_Init ****************************************************
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

/* Begin Function:Int_Disable *************************************************
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
