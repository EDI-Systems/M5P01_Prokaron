/******************************************************************************
Filename    : rmp_test_X86l.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for linux OS. This is not particularly fast due to
              system emulation, and thus suffers a long run time. It can take
              30000 clock cycles to do a context switch and send operations can
              take up to 130000 cycles. Additionall,, the itimer is not accurate
              for time as little as 1ms. Expect a run of 2-3 minutes.
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
#define RMP_CNT_READ()      RDTSC_Read()
/* Memory pool test switch */
#define TEST_MEM_POOL       (8192U)
/* Test exit switch */
#define TEST_EXIT
/* Minimal build switch */
/* #define MINIMAL_SIZE */
/* Timestamp data type */
typedef unsigned long long rmp_tim_t;

/* Number of rounds - too many is useless on this demo port */
#define ROUND_NUM           (100U)
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
rmp_ptr_t Stack_1[65536];
rmp_ptr_t Stack_2[65536];
/* End Global ****************************************************************/

/* Function:Timer_Init ********************************************************
Description : Initialize the timer for timing measurements. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
/* Counter read wrapper */
static __inline__ unsigned long long RDTSC_Read(void)
{
    unsigned long long int Value;
    
    __asm__ __volatile__ (".byte 0x0f, 0x31" : "=A" (Value));
    
    return Value;
}

void Timer_Init(void)
{
    /* Already initialized on power-on, this is x86 and we have RDTSC */
}
/* End Function:Timer_Init ***************************************************/

/* Function:Int_Init **********************************************************
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
    /* Setup the external interrupt handler */
    RMP_X86L_Eint_Handler=TIM_IRQHandler;
    RMP_DBG_S("Timer init done - testing interrupts.\r\n");
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
    /* Erase handler */
    RMP_X86L_Eint_Handler=RMP_NULL;
}
/* End Function:Int_Disable **************************************************/

/* Function:Test_Exit *********************************************************
Description : Exit the test.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Test_Exit(void)
{
    printf("All tests done - exiting process.\n");
    kill(0,SIGKILL);
    exit(0);
}
#endif
/* End Function:Test_Exit ****************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
