#include "test.h"

void Func_1(void* Param)
{
    while(1)
    {
        RMP_Thd_Delay(3000);
        RMP_PRINTK_S("Delayed 3000 cycles\n");
        
        RMP_Thd_Snd(&Thd_2, Time, RMP_MAX_SLICES);
        Time++;
    };
}

void Func_2(void* Param)
{
    ptr_t Data;
    
    while(1)
    {
        RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
        
        RMP_PRINTK_S("Received ");
        RMP_PRINTK_I(Data);
        RMP_PRINTK_S("\n");
    };
}

void RMP_Init_Hook(void)
{
    /* Start threads */
    RMP_Thd_Crt(&Thd_1, Func_1, &Stack_1[238], (void*)0x12345678, 1, 5);
    RMP_Thd_Crt(&Thd_2, Func_2, &Stack_2[238], (void*)0x87654321, 1, 5);
}

void RMP_Init_Idle(void)
{
    return;
}

/* End Function:RMP_Init_Idle ************************************************/

/* These tests shall be conducted simutaneously to make sure that everything works */
/* Two threads yielding to each other */

/* One thread performing a delay loop flashing one variable(use tracing) */

/* One thread suspends the thread performing delay loop every 10 secs, test suspend + delay */

/* Simple send/Receive test pair */

/* Simple send/Receive test pair with different timeouts, print the log to console */

/* Pubsub semaphore test pair with different timeouts, print the log to console */

/* One thread suspending the send/receive pair with some delay */

/* Priority system */
/* Creation/Unsuspend/Delay/Rcv/Send/Semaphore @ different priority levels, see if priority system works */

/* Hooks */

/* Send from ISR with systick hook 1sec to both systems */

/* Send/Rcv chaining */

/* Measurements should be conducted according to the M7M1 manual */

/* This should be good enough, after we finish the manual we can go to deal with the macros */

/* We gonna finish the documents at home, tonight, be quick */
