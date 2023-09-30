/******************************************************************************
Filename    : rmp_test_fe310g000.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for FE310-G000.
******************************************************************************/

/* Includes ******************************************************************/
#include "rmp.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Where are the initial stacks */
#define THD1_STACK          (&Stack_1[300])
#define THD2_STACK          (&Stack_2[300])
/* How to read counter */
#define COUNTER_READ()      (_RMP_Get_MCYCLE())
/* Are we testing the memory pool? */
/* #define TEST_MEM_POOL       8192 */
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The FE310 timers are all 64 bits, however we only need last 32 bits */
typedef rmp_u32_t rmp_tim_t;
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
rmp_ptr_t Stack_1[512];
rmp_ptr_t Stack_2[512];
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
}
/* End Function:Timer_Init ***************************************************/

/* Begin Function:Int_Init ****************************************************
Description : Initialize an periodic interrupt source. This function needs
              to be adapted to your specific hardware. The stuff initialized
              here is actually the watchdog timer.
              You MAY experience very large numbers during the test set of interrupt
              response times, because this part operates on SPI flash which have
              a huge penalty on any instruction cache miss. Just one or two of these
              misses will get you tens of thousands of cycles wasted.
              You can read 'minstret' register against 'mcycle' register to confirm this.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void WDT_Interrupt(void)
{
    /* Clear the interrupt pending bit */
    AON_REG(AON_WDOGKEY)=AON_WDOGKEY_VALUE;
	AON_REG(AON_WDOGCFG)&=~AON_WDOGCFG_CMPIP;
    Int_Handler();
}

void Int_Init(void)
{
    plic_instance_t RMP_Global_PLIC;
	/* Set interrupt to the interrupt handler array */
	RMP_Periph_Vect_Table[INT_WDOGCMP]=(rmp_ptr_t)WDT_Interrupt;
    /* WDT clock = 32.768kHz. Setting counter to 16 to obtain 500us intervals */
    AON_REG(AON_WDOGKEY)=AON_WDOGKEY_VALUE;
    AON_REG(AON_WDOGCMP)=16;
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
	PLIC_set_threshold (&RMP_Global_PLIC,0);
	PLIC_enable_interrupt(&RMP_Global_PLIC,INT_WDOGCMP);
	PLIC_set_priority(&RMP_Global_PLIC,INT_WDOGCMP,2);
	/* Enable WDT with automatic counter zeroing */
    AON_REG(AON_WDOGKEY)=AON_WDOGKEY_VALUE;
	AON_REG(AON_WDOGCFG)=AON_WDOGCFG_ENALWAYS|AON_WDOGCFG_ZEROCMP;
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
