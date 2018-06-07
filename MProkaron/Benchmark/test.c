/******************************************************************************
Filename    : test.c
Author      : pry 
Date        : 22/07/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The performance testbench for RMP. Do not modify this file; what
              you need to modify is the test chip header and the platform chip
              header.
******************************************************************************/

/* Includes ******************************************************************/
#include "test.h"
/* End Includes **************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
volatile tim_t Start=0;
volatile tim_t End=0;
volatile ptr_t Total=0;
volatile ptr_t Temp=0;
/* Test results also written here */
volatile ptr_t Yield_Time=0;
volatile ptr_t Mailbox_Time=0;
volatile ptr_t Semaphore_Time=0;
volatile ptr_t Mailbox_ISR_Time=0;
volatile ptr_t Semaphore_ISR_Time=0;
#ifdef TEST_MEM_POOL
volatile ptr_t Memory_Time=0;
#endif
/* Kernel objects */
volatile struct RMP_Thd Thd_1;
volatile struct RMP_Thd Thd_2;
volatile struct RMP_Thd Thd_Test;
volatile struct RMP_Sem Sem_1;
/* Memory pool */
#ifdef TEST_MEM_POOL
volatile ptr_t Pool[TEST_MEM_POOL]={0};
#endif
/* End Globals ***************************************************************/

/* Begin Function:Func_1 ******************************************************
Description : The test function group 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
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

void Test_Mail_1(void)
{
    static cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        /* Read counter here */
        Start=COUNTER_READ();
        RMP_Thd_Snd(&Thd_2, 1, RMP_MAX_SLICES);
    }
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

void Func_1(void)
{
    Test_Yield_1();
    /* Change priority of thread 2 */
    RMP_Thd_Set(&Thd_2,2,RMP_MAX_SLICES);
    Test_Mail_1();
    Test_Sem_1();
    while(1);
}
/* End Function:Test_Yield ***************************************************/

/* Begin Function:Func_2 ******************************************************
Description : The test function group 2.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Test_Yield_2(void)
{
    cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Yield();
        /* Read counter here */
        End=COUNTER_READ();
        Total+=(tim_t)(End-Start);
    };
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
        Total+=(tim_t)(End-Start);
    };
}

void Test_Sem_2(void)
{
    cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Sem_Pend(&Sem_1, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
        Total+=(tim_t)(End-Start);
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
        Total+=(tim_t)(End-Start);
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
        Total+=(tim_t)(End-Start);
    };
}

#ifdef TEST_MEM_POOL
ptr_t Rand(void)
{
    static ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
        LFSR^=0xB400;
    }
    else
        LFSR>>=1;
    
    return LFSR;
}

void Swap(u8* Arg1, u8* Arg2)
{
    u8 Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
    *Arg2=Temp;
}

void Test_Mem_Pool(void)
{
    static void* Mem[8];
    static u8 Alloc[8];
    static u8 Free[8];
    static u8 Size[8];
    static ptr_t Amount[8];
    cnt_t Count;
    cnt_t Test_Count;
    
    Amount[0]=TEST_MEM_POOL/32;
    Amount[1]=TEST_MEM_POOL/64+16;
    Amount[2]=TEST_MEM_POOL/4;
    Amount[3]=TEST_MEM_POOL/128+32;
    Amount[4]=TEST_MEM_POOL/16;
    Amount[5]=TEST_MEM_POOL/8+16;
    Amount[6]=TEST_MEM_POOL/128+64;
    Amount[7]=TEST_MEM_POOL/2-64;
    
    /* Initialize the pool */
    Total=0;
    RMP_Mem_Init(Pool, TEST_MEM_POOL*sizeof(ptr_t));
    for(Test_Count=0;Test_Count<10000;Test_Count++)
    {
        /* Random sequence and number generation */
        for(Count=0;Count<8;Count++)
        {
            Alloc[Count]=Count;
            Free[Count]=Count;
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
        }
        
        Start=COUNTER_READ();
        /* Allocation tests */
        Mem[Alloc[0]]=RMP_Malloc(Pool, Amount[Size[0]]);
        Mem[Alloc[1]]=RMP_Malloc(Pool, Amount[Size[1]]);
        Mem[Alloc[2]]=RMP_Malloc(Pool, Amount[Size[2]]);
        Mem[Alloc[3]]=RMP_Malloc(Pool, Amount[Size[3]]);
        Mem[Alloc[4]]=RMP_Malloc(Pool, Amount[Size[4]]);
        Mem[Alloc[5]]=RMP_Malloc(Pool, Amount[Size[5]]);
        Mem[Alloc[6]]=RMP_Malloc(Pool, Amount[Size[6]]);
        Mem[Alloc[7]]=RMP_Malloc(Pool, Amount[Size[7]]);

        /* Deallocation tests */
        RMP_Free(Pool,Mem[Free[0]]);
        RMP_Free(Pool,Mem[Free[1]]);
        RMP_Free(Pool,Mem[Free[2]]);
        RMP_Free(Pool,Mem[Free[3]]);
        RMP_Free(Pool,Mem[Free[4]]);
        RMP_Free(Pool,Mem[Free[5]]);
        RMP_Free(Pool,Mem[Free[6]]);
        RMP_Free(Pool,Mem[Free[7]]);
        End=COUNTER_READ();
        Total+=(tim_t)(End-Start);
        
        /* This should always be successful because we deallocated everything else */
        Mem[0]=RMP_Malloc(Pool, (TEST_MEM_POOL>>7)*127);
        if(Mem[0]==0)
        {
            RMP_PRINTK_S("Memory test failure: ");
            RMP_PRINTK_I(Test_Count);
            RMP_PRINTK_S(" runs.\r\n");
            while(1);
        }
        RMP_Free(Pool, Mem[0]); 
    }
    
    RMP_PRINTK_S("Memory: ");
    Memory_Time=Total/160000;
    RMP_PRINTK_I(Memory_Time);
    RMP_PRINTK_S(" cycles.\r\n");
}
#endif

#ifdef RMP_COVERAGE
/* The first test set - deletion and miscellaneous cases */
void Test_Set_1(ptr_t Param)
{
    ptr_t Data;
    switch(Param)
    {
        case 0:
        {
            /* Scenario 1 start */
            RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
            
            /* Scenario 2 start */
            RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
            RMP_Thd_Delay(200);
            
            /* Scenario 3 start */
            RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
            RMP_Thd_Delay(200);
            
            /* Scenario 4 start */
            RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
            
            /* Scenario 5 start */
            RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
            
            /* Final fuzzing start */
            RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
            RMP_Thd_Delay(10000);
            while(1);
        }
        case 1:
        {
            /* Test deletion while running */
            while(1);
        }
        case 2:
        {
            /* Test deletion while in delay */
            RMP_Thd_Delay(10000);
            while(1);
        }
        case 3:
        {
            /* Test deletion while in receive */
            RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
            RMP_Thd_Suspend(&Thd_1);
            while(1);
        }
        case 4:
        {
            /* Test deletion while in timed receive&receive with timeout */
            RMP_Thd_Rcv(&Data, 10000);
            RMP_Thd_Suspend(&Thd_1);
            while(1);
        }
        case 5:
        {
            /* Test deletion while in send */
            RMP_Thd_Snd(&Thd_2, 1, RMP_MAX_SLICES); /* Must be successful */
            RMP_Thd_Snd(&Thd_2, 1, 0);              /* Will return immediately */
            RMP_Thd_Snd(&Thd_2, 1, RMP_MAX_SLICES); /* Will block */
            while(1);
        }
        case 6:
        {
            /* Test deletion while in timed send */
            RMP_Thd_Snd(&Thd_2, 1, 0);              /* Will return immediately */
            RMP_Thd_Snd(&Thd_2, 1, 10000);          /* Will block */
            RMP_Thd_Suspend(&Thd_1);
            while(1);
        }
        case 7:
        {
            /* Test deletion while in sem pend */
            RMP_Sem_Pend(&Sem_1, RMP_MAX_SLICES);
            while(1);
        }
        case 8:
        {
            /* Test deletion while in timed sem pend */
            RMP_Sem_Pend(&Sem_1, 10000);
            while(1);
        }
        case 9:
        {
            /* Test deletion while someone is blocked on it - This will be created with Test_Thd */
            RMP_Thd_Snd(&Thd_1, 1, RMP_MAX_SLICES); /* Must be successful */
            RMP_Thd_Snd(&Thd_1, 1, RMP_MAX_SLICES); /* Will block */
            /* Self delete, because if thread 1 gets deleted this will be released so thread 2 gets preempted */
            RMP_Thd_Del(&Thd_Test);
            while(1);
        }
        case 10:
        {
            /* Test deletion while someone is delaying on it - This will be created with Test_Thd */
            RMP_Thd_Snd(&Thd_1, 1, RMP_MAX_SLICES); /* Must be successful */
            RMP_Thd_Snd(&Thd_1, 1, 10000); /* Will block */
            /* Self delete, because if thread 1 gets deleted this will be released so thread 2 gets preempted */
            RMP_Thd_Del(&Thd_Test);
            while(1);
        }
    }
}

/* The second test set - semaphore pending and timing */
void Test_Set_2(ptr_t Param)
{
    switch(Param)
    {
        case 0:
        {
            RMP_Thd_Delay(10000);
            while(1);
        }
        case 1:
        {
            RMP_Thd_Suspend(&Thd_1);                /* Self suspend */
            while(1);
        }
        case 2:
        {
            RMP_Sem_Pend(&Sem_1, RMP_MAX_SLICES);   /* Delete sem in pend & pend abort */
            RMP_Thd_Del(&Thd_1);
            while(1);
        }
        case 3:
        {
            RMP_Sem_Pend(&Sem_1, 10000);            /* Delete sem in timed pend & pend abort */
            RMP_Thd_Del(&Thd_1);
            while(1);
        }
        case 4:
        {
            RMP_Thd_Delay(100);                     /* Test case 4 */
            while(1);
        }
    }
}

void Test_Coverage_2(void)
{
    ptr_t Data;
    ptr_t Temp;
    
    /* Scenario 0: Miscellaneous tests */
    RMP_Lock_Sched();
    RMP_Yield();
    RMP_Unlock_Sched();
    
    /* Restart thread 1 for testing */
    RMP_Thd_Del(&Thd_1);
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)0, 2, 5);
    
    /* Scenario 1: Delay ourself for a while. 
     *             The thread 1 will be active for some time, 
     *             before thread 2 wakes up */
    RMP_Thd_Snd(&Thd_1, 1, RMP_MAX_SLICES);
    RMP_Thd_Delay(100);
    
    /* Scenario 2: Simultaneous delays.
     *             The thread 1 and thread 2 will delay for 200/100 cycles.
     *             Thread1's delay will be cleared while thread2 is running */
    RMP_Thd_Snd(&Thd_1, 1, RMP_MAX_SLICES);
    RMP_Thd_Delay(100);
    /* Thread 2 will busy loop for 200 cycles */
    Data=RMP_Tick;
    while((RMP_Tick-Data)<200);
    
    /* Scenario 3: Simultaneous delays.
     *             The thread 1 and thread 2 will delay for 200/400 cycles,
     *             with thread 2's delay inserted before thread 1 in the chain,
     *             and thread 1 wakes up while it is suspended. */
    RMP_Thd_Snd(&Thd_1, 1, RMP_MAX_SLICES);
    RMP_Thd_Delay(50);
    RMP_Thd_Suspend(&Thd_1);
    RMP_Thd_Delay(50);
    RMP_Thd_Delay(200);
    RMP_Thd_Resume(&Thd_1);
    
    /* Scenario 4: Timeout on own mailbox receive.
     *             The thread 1 will be active for some time,
     *             before thread 2 timeouts */
    RMP_Thd_Snd(&Thd_1, 1, RMP_MAX_SLICES);
    RMP_Thd_Rcv(&Data, 100);
    
    /* Scenario 5: Timeout on thread 1 mailbox send. The thread 1 will
     *             be suspended by us first, then we send to it.
     *             The thread 1 will be active for some time,
     *             before thread 2 timeouts */
    RMP_Thd_Snd(&Thd_1, 1, RMP_MAX_SLICES);
    RMP_Thd_Suspend(&Thd_1);
    RMP_Thd_Snd(&Thd_1, 1, 100);
    RMP_Thd_Resume(&Thd_1);
    
    /* Scenario 6: stuff kernel API intentionally with trash to see if they can handle them correctly */
    RMP_Thd_Snd(&Thd_1, 1, RMP_MAX_SLICES);
    RMP_Thd_Delay(100);
    /* Thread creation */
    RMP_Thd_Crt(0, 0, 0, 0, 1, 10);                             /* NULL pointer */
    RMP_Thd_Crt(&Thd_Test, 0, 0, 0, RMP_MAX_PREEMPT_PRIO, 10);  /* Wrong priority */
    RMP_Thd_Crt(&Thd_Test, 0, 0, 0, 1, 0);                      /* Wrong slices - too small */
    RMP_Thd_Crt(&Thd_Test, 0, 0, 0, 1, RMP_MAX_SLICES);         /* Wrong slices - too large */
    RMP_Thd_Crt(&Thd_Test, 0, 0, 0, 1, 10);                     /* - */
    RMP_Thd_Crt(&Thd_Test, 0, 0, 0, 1, 10);                     /* Create again */
    RMP_Thd_Del(&Thd_Test);                                     /* - */
    /* Thread deletion */
    RMP_Thd_Del(0);                                             /* NULL pointer */
    RMP_Thd_Del(&Thd_Test);                                     /* Double delete */
    /* Set priority and slices */
    RMP_Thd_Set(0, 1, 10);                                      /* NULL pointer */
    RMP_Thd_Set(&Thd_Test, 1, 10);                              /* Thread already deleted */
    RMP_Thd_Set(&Thd_Test, 1, 0);                               /* Wrong slices - too small */
    /* Suspend */ 
    RMP_Thd_Suspend(0);                                         /* NULL pointer */
    RMP_Thd_Suspend(&Thd_Test);                                 /* Thread already deleted */
    RMP_Thd_Suspend(&Thd_1);                                    /* - */ 
    RMP_Thd_Suspend(&Thd_1);                                    /* Double suspension */ 
    /* Resume */
    RMP_Thd_Resume(&Thd_1);                                     /* - */ 
    RMP_Thd_Resume(0);                                          /* NULL pointer */
    RMP_Thd_Resume(&Thd_Test);                                  /* Thread already deleted */
    RMP_Thd_Resume(&Thd_1);                                     /* Double resume */
    /* Delay */
    RMP_Thd_Delay(0);                                           /* Wrong slices - too small */
    RMP_Thd_Delay(RMP_MAX_SLICES);                              /* Wrong slices - too large */
    /* Cancel delay */
    RMP_Thd_Cancel(0);                                          /* NULL pointer */
    RMP_Thd_Cancel(&Thd_1);                                     /* - */
    RMP_Thd_Cancel(&Thd_1);                                     /* Double cancel */
    /* Send */
    RMP_Thd_Snd(0, 1, 10);                                      /* NULL pointer */
    RMP_Thd_Snd(&Thd_2, 1, 10);                                 /* Send to self */
    RMP_Thd_Snd(&Thd_Test, 1, 10);                              /* Send to deleted thread */
    /* Receive */
    RMP_Thd_Rcv(0, 10);                                         /* NULL pointer */

    /* Semaphore creation */
    RMP_Sem_Crt(0, 0);                                          /* NULL pointer */
    RMP_Sem_Crt(&Sem_1, 0);                                     /* Double initialization */
    RMP_Sem_Del(&Sem_1);                                        /* - */
    RMP_Sem_Crt(&Sem_1, RMP_SEM_MAX_NUM);                       /* Number too large */
    /* Semaphore deletion */
    RMP_Sem_Del(0);                                             /* NULL pointer */
    RMP_Sem_Del(&Sem_1);                                        /* Double delete */
    /* Semaphore pend */
    RMP_Sem_Pend(0, 10);                                        /* NULL pointer */
    RMP_Sem_Pend(&Sem_1, 10);                                   /* Semaphore uninitialized */
    /* Semaphore pend abort */
    RMP_Sem_Abort(0);                                           /* NULL pointer */
    RMP_Sem_Abort(&Thd_Test);                                   /* Abort on deleted thread */
    RMP_Sem_Abort(&Thd_1);                                      /* Abort on thread that is not pending */
    /* Semaphore posting */
    RMP_Sem_Post(0, 10);                                        /* NULL pointer */
    RMP_Sem_Post(&Sem_1, 10);                                   /* Post to uninitialized semaphore */
    RMP_Sem_Crt(&Sem_1, 0);                                     /* - */
    RMP_Sem_Post(&Sem_1, 0);                                    /* Post nothing */
    RMP_Sem_Post(&Sem_1, RMP_SEM_MAX_NUM);                      /* Post number too large */
    RMP_Sem_Post(&Sem_1, RMP_SEM_MAX_NUM/3*2);                  /* - */
    RMP_Sem_Post(&Sem_1, RMP_SEM_MAX_NUM/3*2);                  /* Total number too large */
    RMP_Sem_Del(&Sem_1);                                        /* - */
    RMP_Sem_Crt(&Sem_1, 0);                                     /* - */

    /* The following test requires deleting thread 1 */
    RMP_Thd_Del(&Thd_1);
    /* Scenario 7: Test thread deletion. We will create a new thread, and delete it on different conditions */
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)1, 2, 5);
    RMP_Thd_Suspend(&Thd_1);                                    /* - */
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)1, 3, 5);
    RMP_Thd_Del(&Thd_1);                                        /* Deletion while running & Clear ready while suspended */
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)2, 3, 5); 
    RMP_Thd_Del(&Thd_1);                                        /* Deletion while in delay */
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)3, 3, 5); 
    RMP_Thd_Del(&Thd_1);                                        /* Deletion while in receive block */
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)4, 3, 5); 
    RMP_Thd_Del(&Thd_1);                                        /* Deletion while in receive delay */
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)4, 3, 5);
    RMP_Thd_Snd(&Thd_1, 1, RMP_MAX_SLICES);                     /* Receive delay cancelled by send here */
    RMP_Thd_Del(&Thd_1);                                        /* Deletion while in suspension */
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)5, 3, 5); 
    RMP_Thd_Del(&Thd_1);                                        /* Deletion while in send block */
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)6, 3, 5); 
    RMP_Thd_Del(&Thd_1);                                        /* Deletion while in send delay */
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)6, 3, 5);
    RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);                         /* - */
    RMP_Thd_Del(&Thd_1);                                        /* Sender wakeup in delay & deletion while suspended */
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)7, 3, 5); 
    RMP_Thd_Del(&Thd_1);                                        /* Deletion while in pend block */
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)8, 3, 5); 
    RMP_Thd_Del(&Thd_1);                                        /* Deletion while in pend delay */
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)5, 3, 5); 
    RMP_Thd_Crt(&Thd_Test, Test_Set_1, THD1_STACK, (void*)9, 4, 5); 
    RMP_Thd_Del(&Thd_1);                                        /* Deletion while someone sending to it */
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)5, 3, 5); 
    RMP_Thd_Crt(&Thd_Test, Test_Set_1, THD1_STACK, (void*)10, 4, 5); 
    RMP_Thd_Del(&Thd_1);                                        /* Deletion while someone sending to it with delay */
    
    /* The following test requires a new thread 1 */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, THD1_STACK, (void*)0, 2, 5);  
    RMP_Thd_Set(&Thd_1, 1, RMP_MAX_SLICES);                     /* Set priority but not slices */
    RMP_Thd_Set(&Thd_1, RMP_MAX_PREEMPT_PRIO, 10);              /* Set slices but not priority */
    RMP_Thd_Set(&Thd_1, 1, 7);                                  /* Set them both - to the same prio */
    RMP_Thd_Set(&Thd_1, 3, 7);                                  /* Set them both - different prio with preemption */
    RMP_Thd_Set(&Thd_1, RMP_MAX_PREEMPT_PRIO, RMP_MAX_SLICES);  /* Set none of them */
    /* Do the same while it is delaying */ 
    RMP_Thd_Set(&Thd_1, 1, RMP_MAX_SLICES);                     /* Set priority but not slices */
    RMP_Thd_Set(&Thd_1, RMP_MAX_PREEMPT_PRIO, 10);              /* Set slices but not priority */
    RMP_Thd_Set(&Thd_1, 1, 7);                                  /* Set them both - to the same prio */
    RMP_Thd_Set(&Thd_1, 3, 7);                                  /* Set them both - different prio with preemption */
    RMP_Thd_Del(&Thd_1);
    
    /* Test self suspend */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, THD1_STACK, (void*)1, 3, 5);   
    RMP_Thd_Del(&Thd_1);
    
    /* Test receive with immediate return if empty */
    RMP_Thd_Rcv(&Data, 0);                                      /* - */
    RMP_Thd_Rcv(&Data, 0);                                      /* - */
    
    /* Test semaphore delete with someone waiting */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, THD1_STACK, (void*)2, 3, 5);
    RMP_Sem_Del(&Sem_1);                                        /* - */
    RMP_Sem_Crt(&Sem_1, 0);                                     /* - */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, THD1_STACK, (void*)3, 3, 5);
    RMP_Sem_Del(&Sem_1);                                        /* - */
    RMP_Sem_Crt(&Sem_1, 0);                                     /* - */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, THD1_STACK, (void*)3, 3, 5);
    RMP_Sem_Post(&Sem_1, 1);                                    /* This semaphore receive success with timeout */
    
    /* Semaphore tests */
    RMP_Sem_Post(&Sem_1, 1);                                    /* - */
    RMP_Sem_Pend(&Sem_1, 100);                                  /* Timed pend that will return immediately */
    RMP_Sem_Pend(&Sem_1, 0);                                    /* Poll pend that will return immediately */
    
    /* Semaphore pend abort */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, THD1_STACK, (void*)2, 3, 5);
    RMP_Sem_Abort(&Thd_1);                                      /* Abort while blocked */
    RMP_Thd_Del(&Thd_1);
    RMP_Thd_Crt(&Thd_1, Test_Set_2, THD1_STACK, (void*)3, 3, 5);
    RMP_Sem_Abort(&Thd_1);                                      /* Abort while delay */
    RMP_Thd_Del(&Thd_1);
        
    /* Lock the scheduler and test ISR sends */
    RMP_Lock_Sched();
    RMP_Thd_Snd_ISR(0, 1);                                      /* NULL pointer */
    RMP_Thd_Snd_ISR(&Thd_Test, 1);                              /* Send to deleted thread */
    RMP_Unlock_Sched();
    
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)3, 3, 5);
    RMP_Lock_Sched();
    RMP_Thd_Snd_ISR(&Thd_1, 1);                                /* Send success with blocked thread */
    _RMP_Get_Near_Ticks();
    RMP_Unlock_Sched();
    RMP_Thd_Del(&Thd_1);
    
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)4, 3, 5);
    RMP_Lock_Sched();
    RMP_Thd_Snd_ISR(&Thd_1, 1);                                /* Send success with delaying thread */
    _RMP_Get_Near_Ticks();
    RMP_Thd_Snd_ISR(&Thd_1, 1);                                /* Send failure due to full mailbox */
    _RMP_Get_Near_Ticks();
    RMP_Unlock_Sched();
    RMP_Thd_Del(&Thd_1);
     
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)2, 3, 5);
    RMP_Lock_Sched();
    RMP_Thd_Snd_ISR(&Thd_1, 1);                                /* Send success with thread not blocked */
    _RMP_Get_Near_Ticks();
    RMP_Unlock_Sched();
    RMP_Thd_Del(&Thd_1);
    
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)3, 1, 5);
    RMP_Thd_Delay(100);
    RMP_Lock_Sched();
    RMP_Thd_Snd_ISR(&Thd_1, 1);                                /* Send success with thread not preempting current thread */
    _RMP_Get_Near_Ticks();
    RMP_Unlock_Sched();
    RMP_Thd_Del(&Thd_1);
    
    /* Semaphore pend tests */
    RMP_Sem_Post_ISR(&Sem_1, RMP_SEM_MAX_NUM);                  /* Posted number too large */
    RMP_Sem_Del(&Sem_1);
    RMP_Lock_Sched();
    RMP_Sem_Post_ISR(0, 1);                                     /* NULL pointer */
    RMP_Sem_Post_ISR(&Sem_1, 2);                                /* Post to deleted semaphore */
    RMP_Unlock_Sched();
    
    RMP_Sem_Crt(&Sem_1, 0);
    RMP_Thd_Crt(&Thd_1, Test_Set_2, THD1_STACK, (void*)2, 3, 5);
    RMP_Lock_Sched();
    RMP_Sem_Post_ISR(&Sem_1, 1);                                /* Post success with blocked thread */
    RMP_Unlock_Sched();
    RMP_Thd_Del(&Thd_1);
    
    RMP_Thd_Crt(&Thd_1, Test_Set_2, THD1_STACK, (void*)3, 3, 5);
    RMP_Lock_Sched();
    RMP_Sem_Post_ISR(&Sem_1, 1);                                /* Post success with delaying thread */
    RMP_Unlock_Sched();
    RMP_Thd_Del(&Thd_1);
    
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)2, 3, 5);
    RMP_Lock_Sched();
    RMP_Sem_Post_ISR(&Sem_1, 1);                                /* Send success with thread not pending */
    RMP_Unlock_Sched();
    RMP_Thd_Del(&Thd_1);
    RMP_Sem_Del(&Sem_1);
    
    RMP_Sem_Crt(&Sem_1, 0);
    RMP_Thd_Crt(&Thd_1, Test_Set_2, THD1_STACK, (void*)2, 1, 5);
    RMP_Thd_Delay(100);
    RMP_Lock_Sched();
    RMP_Sem_Post_ISR(&Sem_1, 1);                                /* Post success with thread not preempting current thread */
    RMP_Unlock_Sched();
    RMP_Thd_Del(&Thd_1);
    
    /* Memory pool fuzzing */
    RMP_Malloc(Pool, TEST_MEM_POOL*sizeof(ptr_t)*2);            /* Allocate more memory than we have */
    RMP_Free(Pool,0);                                           /* NULL pointer */
    RMP_Free(Pool,(void*)1);                                    /* Not in freeable range */
    RMP_Free(0,(void*)100);                                     /* NULL pointer */
    RMP_Malloc(0,100);                                          /* NULL pointer */
    RMP_Malloc(Pool,100);                                       /* Allocate zero size */
    Data=(ptr_t)RMP_Malloc(Pool, 256);                          /* - */
    RMP_Free(Pool,(void*)Data);                                 /* - */
    RMP_Free(Pool,(void*)Data);                                 /* Double free */
    RMP_Mem_Init(0, TEST_MEM_POOL*sizeof(ptr_t));               /* NULL pointer */
    RMP_Mem_Init(Pool, (1<<30));                                /* Memory too large */
    RMP_Mem_Init(Pool, 0);                                      /* Memory too small */
    RMP_Mem_Init((void*)(((ptr_t)Pool)+1), TEST_MEM_POOL*sizeof(ptr_t)); /* Memory not aligned */
    
    /* Memory allocations */
    Data=(ptr_t)RMP_Malloc(Pool, TEST_MEM_POOL*sizeof(ptr_t)/5*4); /* Should find it on the first try */
    
    /* Test residue smaller than 64 - keep allocating 64 byte until we run out of memory.
     * memory leaking is fine because this is just a test */
    while(1)
    {
        Data=(ptr_t)RMP_Malloc(Pool, 64);
        if(Data==0)
            break;
        /* Last good allocation stored here */
        Temp=Data;
        /* Iterate on this function */
        RMP_Lock_Sched();
        _RMP_Get_Near_Ticks();
        RMP_Unlock_Sched();
    }
    RMP_Free(Pool,(void*)Temp);                                 /* Test cannot merge with right side */
    
    RMP_Thd_Crt(&Thd_1, Test_Set_2, THD1_STACK, (void*)4, 2, 1);
    RMP_Yield();
    /* Thread 2 will busy loop for 1000 cycles */
    Data=RMP_Tick;
    while((RMP_Tick-Data)<1000)
    {
        RMP_Lock_Sched();
        _RMP_Get_Near_Ticks();
        RMP_Unlock_Sched();
    }
    RMP_Thd_Del(&Thd_1);
}
#endif

void Func_2(void)
{
    /* Yield tests */
    Total=0;
    Test_Yield_2();
    RMP_PRINTK_S("Yield: ");
    
    Yield_Time=Total/10000;
    RMP_PRINTK_I(Yield_Time);
    RMP_PRINTK_S(" cycles.\r\n");
    /* Change priority of thread 2, just in case */
    RMP_Thd_Set(&Thd_2,2,RMP_MAX_SLICES);
    
    /* Mailbox tests */
    Total=0;
    Test_Mail_2();
    RMP_PRINTK_S("Mailbox: ");
    Mailbox_Time=Total/10000;
    RMP_PRINTK_I(Mailbox_Time);
    RMP_PRINTK_S(" cycles.\r\n");
    
    /* Semaphore tests */
    Total=0;
    Test_Sem_2();
    RMP_PRINTK_S("Semaphore: ");
    Semaphore_Time=Total/10000;
    RMP_PRINTK_I(Semaphore_Time);
    RMP_PRINTK_S(" cycles.\r\n");
    
    /* Memory pool tests */
#ifdef TEST_MEM_POOL
    Test_Mem_Pool();
#endif

    /* Mailbox from interrupt tests */
    Total=0;
    Int_Init();
    Test_Mail_ISR();
    
    /* Semaphore from interrupt tests */
    Temp=Total;
    Total=0;
    Test_Sem_ISR();
    
    RMP_PRINTK_S("Mailbox-ISR: ");
    Mailbox_ISR_Time=Temp/10000;
    RMP_PRINTK_I(Mailbox_ISR_Time);
    RMP_PRINTK_S(" cycles.\r\n");
    RMP_PRINTK_S("Semaphore-ISR: ");
    Semaphore_ISR_Time=Total/10000;
    RMP_PRINTK_I(Semaphore_ISR_Time);
    RMP_PRINTK_S(" cycles.\r\n");

#ifdef RMP_COVERAGE
    Test_Coverage_2();
    RMP_Print_Coverage();
#endif
    
    while(1);
}
/* End Function:Func_2 *******************************************************/

/* Begin Function:Int_Handler *************************************************
Description : The interrupt handler. Call this in your periodic ISR.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Handler(void)
{
    static cnt_t Count=0;
    
    if(Count<10000)
    {
        Count++;
        Start=COUNTER_READ();
        if(RMP_Thd_Snd_ISR(&Thd_2, 1)<0)
        {
            RMP_PRINTK_S("ISR Mailbox send failure: ");
            RMP_PRINTK_I(Count);
            RMP_PRINTK_S(" sends.\r\n");
            while(1);
        }
    }
    else if(Count<20000)
    {
        Count++;
        Start=COUNTER_READ();
        if(RMP_Sem_Post_ISR(&Sem_1, 1)<0)
        {
            RMP_PRINTK_S("ISR semaphore post failure: ");
            RMP_PRINTK_I(Count);
            RMP_PRINTK_S(" posts.\r\n");
            while(1);
        }
    }
    else
        Int_Disable();
}
#endif
/* End Function:Int_Handler **************************************************/

/* Begin Function:RMP_Init ****************************************************
Description : The init thread hook functions.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Init_Hook(void)
{
#ifndef MINIMAL_SIZE
    /* Init the timer */
    Timer_Init();
    /* Clean up the structures */
    RMP_Clear(&Thd_1,sizeof(struct RMP_Thd));
    RMP_Clear(&Thd_2,sizeof(struct RMP_Thd));
    RMP_Clear(&Sem_1,sizeof(struct RMP_Sem));
    /* Create counting semaphore */
    RMP_Sem_Crt(&Sem_1,0);
    /* Start threads */
    RMP_Thd_Crt(&Thd_1, Func_1, THD1_STACK, (void*)0x1234, 1, 5);
    RMP_Thd_Crt(&Thd_2, Func_2, THD2_STACK, (void*)0x4321, 1, 1000);
#endif
}

void RMP_Init_Idle(void)
{
    return;
}
/* End Function:RMP_Init *****************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
