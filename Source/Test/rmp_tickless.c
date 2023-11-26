/******************************************************************************
Filename    : rmp_tickless.c
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The tickless test for RMP. When you enable tickless for a port,
              its functionality may be tested here for completeness. Note that
              this is not a coverage or performance test.
              This test assumes 1 tick = 10ms, and the timer used for tickless
              have at least 16 bits. Hence, the minimum MCU activation frequency
              is once per 10 minutes, which deemed sufficient for .
              The user should provide:
              1. implementation of RMP_Init_Idle,
              2. implementation of RMP_Sched_Hook,
              3. implementation of RMP_Tim_Hook,
              4. implementation of RMP_Dly_Hook, and
              5. appropriate routines for fast timer reprogramming.
******************************************************************************/

/* Include *******************************************************************/
#include "rmp_test.h"
/* End Include ***************************************************************/

/* Global ********************************************************************/
/* Kernel objects */
volatile struct RMP_Thd Thd_1;
volatile struct RMP_Thd Thd_2;

/* Test variables */
volatile rmp_ptr_t Thd_Flag;

/* Test functions */
void Func_1(void);
void Func_2(void);
/* End Global ****************************************************************/

/* Function:Func_1 ************************************************************
Description : The test function group 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Func_1(void)
{
    volatile rmp_ptr_t Count;

    Count=0U;
    while(Count<20U)
    {
        if(Thd_Flag==0U)
        {
            Thd_Flag=1U;
            RMP_DBG_S("> Thd 1: switched to.\r\n");
            Count++;
        }
    }

    for(Count=0U;Count<10U;Count++)
    {
        RMP_Thd_Delay(RMP_TICK_1SEC*5U);
        RMP_DBG_S("> Thd 1: delay complete.\r\n");
    }

    while(1)
    {
        RMP_DBG_S("> Thd 1: suspended.\r\n");
        RMP_Thd_Suspend(&Thd_1);
    }
}
/* End Function:Test_Yield ***************************************************/

/* Function:Func_2 ************************************************************
Description : The test function group 2.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Func_2(void)
{
    volatile rmp_ptr_t Count;
    volatile rmp_ptr_t Intern;

    /* Print table header */
    RMP_DBG_S("\r\n    ___   __  ___ ___\r\n");
    RMP_DBG_S("   / _ \\ /  |/  // _ \\       Simple real-time kernel\r\n");
    RMP_DBG_S("  / , _// /|_/ // ___/       Tickless function test\r\n");
    RMP_DBG_S(" /_/|_|/_/  /_//_/\r\n");
    RMP_DBG_S("====================================================\r\n");

    /* Test busy loop current consumption */
    RMP_DBG_S("Test 1: Busy Loop - measure busy current now.\r\n");
    for(Count=0U;Count<RMP_BUSY_LOOP;Count++)
    {
        for(Intern=0U;Intern<65500U;Intern++)
        {
            /* Busy loop */
        }
    }

    /* Test thread delay while the other is active */
    RMP_DBG_S("Test 2: Delay switches - should pop up every 5 secs.\r\n");
    for(Count=0U;Count<10U;Count++)
    {
        Thd_Flag=0U;
        RMP_Thd_Delay(RMP_TICK_1SEC*5U);
        RMP_DBG_S("> Thd 2: delay complete.\r\n");
    }

    /* Lower priority of thread 2 for timeout yields */
    RMP_Thd_Set(&Thd_2,1U,RMP_SLICE_MAX);

    /* Test budget exhaustion */
    RMP_DBG_S("Test 3: Budget exhaustion - should pop up every 3 secs.\r\n");
    Count=0U;
    while(Count<10U)
    {
        if(Thd_Flag!=0U)
        {
            Thd_Flag=0U;
            RMP_DBG_S("> Thd 2: switched to.\r\n");
            Count++;
        }
    }

    /* Test thread delay while no thread is active */
    RMP_DBG_S("Test 4: Pulse mode - measure pulse-activation current now.\r\n");
    for(Count=0U;Count<10U;Count++)
    {
        RMP_Thd_Delay(RMP_TICK_1SEC*5U);
        RMP_DBG_S("> Thd 2: delay complete.\r\n");
    }

    /* Test standby current */
    RMP_DBG_S("Test 5: Standby mode - measure sleep current now.\r\n");
    while(1)
    {
        RMP_DBG_S("> Thd 2: suspended.\r\n");
        RMP_Thd_Suspend(&Thd_2);
    }
}
/* End Function:Func_2 *******************************************************/

/* Function:RMP_Init_Hook *****************************************************
Description : The init thread hook functions.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Init_Hook(void)
{
    Thd_Flag=0U;

    /* Clean up the structures */
    RMP_Clear(&Thd_1, sizeof(struct RMP_Thd));
    RMP_Clear(&Thd_2, sizeof(struct RMP_Thd));
    
    /* Start threads - the two threads will exhaust timeslices */
    RMP_Thd_Crt(&Thd_2,
                (void*)Func_2, (void*)0x4321U,
                Stack_2, sizeof(Stack_2),
                2U, RMP_TICK_1SEC*3U);
    RMP_Thd_Crt(&Thd_1,
                (void*)Func_1, (void*)0x1234U,
                Stack_1, sizeof(Stack_1),
                1U, RMP_TICK_1SEC*3U);
}
/* End Function:RMP_Init_Hook ************************************************/

/* Function:RMP_Ctx_Save ******************************************************
Description : Save additional context.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Ctx_Save(void)
{
    return;
}
/* End Function:RMP_Ctx_Save *************************************************/

/* Function:RMP_Ctx_Load ******************************************************
Description : Load additional context.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Ctx_Load(void)
{
    return;
}
/* End Function:RMP_Ctx_Load *************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
