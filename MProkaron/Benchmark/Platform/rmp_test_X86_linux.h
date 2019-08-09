/******************************************************************************
Filename    : rmp_test_X86_linux.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for linux OS. This is not particularly fast due to
              system emulation, and thus suffers a long run time. It can take
              30000 clock cycles to do a context switch and send operations can
              take up to 130000 cycles. Additionall,, the itimer is not accurate
              for time as little as 1ms. Expect a run of 2-3 minutes.
******************************************************************************/

/* Includes ******************************************************************/
#include "rmp.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Where are the initial stacks */
#define THD1_STACK        (&Stack_1[60000])
#define THD2_STACK        (&Stack_2[60000])
/* How to read counter */
static __inline__ unsigned long long rdtsc(void)
{
    unsigned long long int x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
}

#define COUNTER_READ()    rdtsc()
/* Are we testing the memory pool? */
#define TEST_MEM_POOL     8192
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The POSIX timers are all 64 bits, so */
typedef unsigned long long rmp_tim_t;

/* Interrupt interval in usec */
#define TEST_INT_INTERVAL 10000
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
rmp_ptr_t Stack_1[65536];
rmp_ptr_t Stack_2[65536];
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
    /* Already initialized on power-on, this is x86 and we have RDTSC */
}
/* End Function:Timer_Init ***************************************************/

/* Begin Function:Int_Init ****************************************************
Description : Initialize an periodic interrupt source. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
/* The interrupt handler */
void TIM_IRQHandler(void)
{
    Int_Handler();
}

/* This is used to send interrupt signal to the parent thread */
void Int_Init(void)
{
    struct itimerval Tick;
    /* Setup the external interrupt handler */
    RMP_Eint_Handler=TIM_IRQHandler;

    /* Set up the timer */
    memset(&Tick, 0, sizeof(Tick));
    /* First timeout */
    Tick.it_value.tv_sec=0;
    Tick.it_value.tv_usec=TEST_INT_INTERVAL;
    /* Interval time to run function */
    Tick.it_interval.tv_sec=0;
    Tick.it_interval.tv_usec=TEST_INT_INTERVAL;
    RMP_ASSERT(setitimer(ITIMER_REAL, &Tick, NULL)>=0);
    RMP_PRINTK_S("Timer init done - testing interrupts.\r\n");
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
    struct itimerval Tick;
    /* Erase handler */
    RMP_Eint_Handler=0;

    /* Set up the timer */
    memset(&Tick, 0, sizeof(Tick));
    /* Stop the itimer */
    RMP_ASSERT(setitimer(ITIMER_REAL, &Tick, NULL)>=0);
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
