#include "test.h"

ptr_t Start;
ptr_t End;
ptr_t Total;

void Test_Yield_1(void)
{
    cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        /* Read counter here */
        Start=COUNTER_READ();
        RMP_Yield();
    }
}

void Test_Yield_2(void)
{
    cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Yield();
        /* Read counter here */
        End=COUNTER_READ();
        Total+=End-Start;
    };
}

void Test_Mail_1(void)
{
    cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        /* Read counter here */
        Start=COUNTER_READ();
        RMP_Thd_Snd(&Thd_2, 1, RMP_MAX_SLICES);
    }
}

void Test_Mail_2(void)
{
    ptr_t Data;
    cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
        Total+=End-Start;
    };
}

void Test_Sem_1(void)
{
    cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        /* Read counter here */
        Start=COUNTER_READ();
        RMP_Sem_Post(&Sem_1, 1);
    }
}

void Test_Sem_2(void)
{
    cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Sem_Pend(&Sem_1, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
        Total+=End-Start;
    };
}

void Test_Mail_ISR(void)
{
    ptr_t Data;
    static cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
        Total+=End-Start;
    };
}

void Test_Sem_ISR(void)
{
    static cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Sem_Pend(&Sem_1, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
        Total+=End-Start;
    };
}

void Int_Handler(void)
{
    static cnt_t Count=0;
    
    if(Count<10000)
    {
        Count++;
        Start=COUNTER_READ();
        if(RMP_Thd_Snd_ISR(&Thd_2, 1)<0)
            while(1);
    }
    else if(Count<20000)
    {
        Count++;
        Start=COUNTER_READ();
        RMP_Sem_Post_ISR(&Sem_1, 1);
    }
    else
        Int_Disable();
}



void Func_1(void* Param)
{
    Test_Yield_1();
    /* Change priority of thread 2 */
    RMP_Thd_Set(&Thd_2,2,RMP_MAX_SLICES);
    Test_Mail_1();
    Test_Sem_1();
    while(1);
}

void Func_2(void* Param)
{
    /* Yield tests */
    Total=0;
    Test_Yield_2();
    RMP_PRINTK_S("Yield: ");
    RMP_PRINTK_I(Total/10000);
    RMP_PRINTK_S(" cycles.\r\n");
    /* Change priority of thread 2, just in case */
    RMP_Thd_Set(&Thd_2,2,RMP_MAX_SLICES);
    
    /* Mailbox tests */
    Total=0;
    Test_Mail_2();
    RMP_PRINTK_S("Mailbox: ");
    RMP_PRINTK_I(Total/10000);
    RMP_PRINTK_S(" cycles.\r\n");
    
    /* Semaphore tests */
    Total=0;
    Test_Sem_2();
    RMP_PRINTK_S("Semaphore: ");
    RMP_PRINTK_I(Total/10000);
    RMP_PRINTK_S(" cycles.\r\n");
    
    /* Mailbox from interrupt tests */
    Total=0;
    Int_Init();
    Test_Mail_ISR();
    RMP_PRINTK_S("Mailbox-ISR: ");
    RMP_PRINTK_I(Total/10000);
    RMP_PRINTK_S(" cycles.\r\n");
    
    /* Semaphore from interrupt tests */
    Total=0;
    Test_Sem_ISR();
    RMP_PRINTK_S("Semaphore-ISR: ");
    RMP_PRINTK_I(Total/10000);
    RMP_PRINTK_S(" cycles.\r\n");
    
    while(1);
}

void RMP_Init_Hook(void)
{
    /* Init the timer */
    Timer_Init();
    /* Create counting semaphore */
    RMP_Sem_Crt(&Sem_1,0);
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
