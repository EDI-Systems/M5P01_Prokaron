/******************************************************************************
Filename    : rmp_test_fe310g000.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for FE310-G000.
              The numbers running from this test does not make much sense due
              to the FE310's instruction cache w/SPI flash. This combination
              easily costs tens of thousands of cycles when it misses.
              Each test needs a full power cycle - FE310 chip is seriously
              flawed in many aspects especially in its WDT timer.
		      This project only worked with the Freedom Studio Beta 3
		      (FreedomStudio-Win_x86_64-20180122) which is now deprecated
		      (in fact all releases of the FreedomStudio is as of 2023), and
		      the provided OpenOCD debugger is very sluggish. It is advised
		      to stay away from this board unless you already have one.
		      This is a typical academia chip: looks nice but is useless.

GCC 7.2.0 -O3 (Note: numbers > 10000 is due to SPI flash reads)
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 608 / 5146 / 598
Mailbox                            : 874 / 38957 / 871
Semaphore                          : 782 / 28967 / 780
FIFO                               : 158 / 31113 / 155
Message queue                      : 1027 / 29777 / 1025
Blocking message queue             : 1223 / 28692 / 1221
ISR Mailbox                        : 826 / 9596 / 825
ISR Semaphore                      : 47596 / 47666 / 18046
ISR Message queue                  : 47612 / 57111 / 32376
ISR Blocking message queue         : 47856 / 47922 / 480
******************************************************************************/

/* Includes ******************************************************************/
#include "rmp.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* How to read counter */
#define RMP_CNT_READ()      ((rmp_tim_t)RMP_RV32P_MCYCLE_Get())
/* Are we testing the memory pool? */
/* #define TEST_MEM_POOL       8192 */
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The FE310 timers are all 64 bits, however we only need last 16 bits */
typedef rmp_u16_t rmp_tim_t;
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
rmp_ptr_t Stack_1[512];
rmp_ptr_t Stack_2[512];

void Timer_Init(void);
void Int_Init(void);
void Int_Handler(void);
void Int_Disable(void);

void WDT_Interrupt(void);
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
    /* No need to init the measurement timer - we read MCYCLE register instead */
	RMP_DBG_S("Confirming serial good - connect to TXD.1 pin");
}
/* End Function:Timer_Init ***************************************************/

/* Begin Function:Int_Init ****************************************************
Description : Initialize an periodic interrupt source. This function needs
              to be adapted to your specific hardware. The stuff initialized
              here is actually the watchdog timer.
              You MAY experience very large numbers during the test set of
              interrupt response times, because this part operates on SPI flash
              which induces a huge penalty on any instruction cache miss. Just
              one or two of these misses will get you tens of thousands of
              cycles wasted. You can read 'minstret' register against 'mcycle'
              register to confirm this.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Init(void)
{
    plic_instance_t RMP_Global_PLIC;
    /* WDT clock = 32.768kHz. Setting counter to 33 to obtain 1ms intervals */
    AON_REG(AON_WDOGKEY)=AON_WDOGKEY_VALUE;
    AON_REG(AON_WDOGCMP)=33;
    /* Feed dog - cannot assume that the pin is not asserted */
    AON_REG(AON_WDOGKEY)=AON_WDOGKEY_VALUE;
    AON_REG(AON_WDOGFEED)=AON_WDOGFEED_VALUE;
	/* Disable WDT before trying to enable interrupt */
    AON_REG(AON_WDOGKEY)=AON_WDOGKEY_VALUE;
	AON_REG(AON_WDOGCFG)=0;
	/* Enable WDT interrupt */
	RMP_Global_PLIC.base_addr=PLIC_CTRL_ADDR;
	/* If there is already one pending, get rid of it */
	PLIC_complete_interrupt(&RMP_Global_PLIC, PLIC_claim_interrupt(&RMP_Global_PLIC));
    PLIC_set_priority(&RMP_Global_PLIC, INT_WDOGCMP, 1);
	PLIC_enable_interrupt(&RMP_Global_PLIC, INT_WDOGCMP);
	/* Enable WDT with automatic counter zeroing */
    AON_REG(AON_WDOGKEY)=AON_WDOGKEY_VALUE;
	AON_REG(AON_WDOGCFG)=AON_WDOGCFG_ENALWAYS|AON_WDOGCFG_ZEROCMP;
}

void WDT_Interrupt(void)
{
    plic_instance_t RMP_Global_PLIC;
    rmp_ptr_t Int_Number;

    RMP_Global_PLIC.base_addr=PLIC_CTRL_ADDR;
    Int_Number=PLIC_claim_interrupt(&RMP_Global_PLIC);

    /* Filter spurious interrupts, but ACK them as well */
    if(Int_Number==INT_WDOGCMP)
    {
        /* Clear the interrupt pending bit */
        AON_REG(AON_WDOGKEY)=AON_WDOGKEY_VALUE;
    	AON_REG(AON_WDOGCFG)&=~AON_WDOGCFG_CMPIP;
        Int_Handler();
    }

    PLIC_complete_interrupt(&RMP_Global_PLIC, Int_Number);
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
    plic_instance_t RMP_Global_PLIC;

	/* Disable WDT interrupt */
	RMP_Global_PLIC.base_addr=PLIC_CTRL_ADDR;
	PLIC_disable_interrupt(&RMP_Global_PLIC,INT_WDOGCMP);
	/* Disable WDT */
	AON_REG(AON_WDOGKEY)=AON_WDOGKEY_VALUE;
	AON_REG(AON_WDOGCFG)=0;
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
