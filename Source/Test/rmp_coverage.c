/******************************************************************************
Filename    : rmp_test.c
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The performance testbench for RMP. Do not modify this file; what
              you need to modify is the test chip header and the platform chip
              header.
******************************************************************************/

/* Include *******************************************************************/
#include "rmp_test.h"
/* End Include ***************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
volatile rmp_tim_t Start=0;
volatile rmp_tim_t End=0;
#ifdef PTR_16_BIT
volatile rmp_u32_t Total=0;
volatile rmp_u32_t Temp=0;
#else
volatile rmp_ptr_t Total=0;
volatile rmp_ptr_t Temp1=0;
volatile rmp_ptr_t Temp2=0;
volatile rmp_ptr_t Temp3=0;
#endif
/* Test results also written here */
volatile rmp_ptr_t Yield_Time=0;
volatile rmp_ptr_t Mailbox_Time=0;
volatile rmp_ptr_t Semaphore_Time=0;
volatile rmp_ptr_t Fifo_Time=0;
volatile rmp_ptr_t Msgq_Time=0;
volatile rmp_ptr_t Bmq_Time=0;
volatile rmp_ptr_t Mailbox_ISR_Time=0;
volatile rmp_ptr_t Semaphore_ISR_Time=0;
volatile rmp_ptr_t Msgq_ISR_Time=0;
volatile rmp_ptr_t Bmq_ISR_Time=0;
#ifdef TEST_MEM_POOL
volatile rmp_ptr_t Memory_Time=0;
#endif
/* Kernel objects */
volatile struct RMP_Thd Thd_1;
volatile struct RMP_Thd Thd_2;
volatile struct RMP_Thd Thd_Test;
volatile struct RMP_Sem Sem_1;
/* Extended kernel objects */
volatile struct RMP_Fifo Fifo_1;
volatile struct RMP_Msgq Msgq_1;
volatile struct RMP_Bmq Bmq_1;
/* Memory pool */
#ifdef TEST_MEM_POOL
volatile rmp_ptr_t Pool[TEST_MEM_POOL]={0};
#endif


void Test_Yield_1(void);
void Test_Mail_1(void);
void Test_Sem_1(void);
void Test_Msgq_1(void);
void Test_Bmq_1(void);
void Func_1(void);

void Test_Yield_2(void);
void Test_Mail_2(void);
void Test_Sem_2(void);
void Test_Fifo(void);
void Test_Msgq_2(void);
void Test_Bmq_2(void);
void Test_Mail_ISR(void);
void Test_Sem_ISR(void);
void Test_Msgq_ISR(void);
void Test_Bmq_ISR(void);

#ifdef TEST_MEM_POOL
rmp_ptr_t Rand(void);
void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2);
void Test_Mem_Pool(void);
#endif

#ifdef RMP_COVERAGE
void Test_Set_1(rmp_ptr_t Param);
void Test_Set_2(rmp_ptr_t Param);
#endif

void Func_2(void);
/* End Global ****************************************************************/

/* Function:Func_1 ************************************************************
Description : The test function group 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Test_Yield_1(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        /* Read counter here */
        Start=RMP_CNT_READ();
        RMP_Yield();
    }
}

void Test_Mail_1(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        /* Read counter here */
        Start=RMP_CNT_READ();
        RMP_Thd_Snd(&Thd_2, 1, RMP_SLICE_MAX);
    }
}

void Test_Sem_1(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        /* Read counter here */
        Start=RMP_CNT_READ();
        RMP_Sem_Post(&Sem_1, 1);
    }
}

void Test_Msgq_1(void)
{
    rmp_cnt_t Count;
    struct RMP_List Node;
    for(Count=0;Count<10000;Count++)
    {
        /* Read counter here */
        Start=RMP_CNT_READ();
        RMP_Msgq_Snd(&Msgq_1, &Node);
    }
}

void Test_Bmq_1(void)
{
    rmp_cnt_t Count;
    struct RMP_List Node;
    for(Count=0;Count<10000;Count++)
    {
        /* Read counter here */
        Start=RMP_CNT_READ();
        RMP_Bmq_Snd(&Bmq_1, &Node, RMP_SLICE_MAX);
    }
}

void Func_1(void)
{
    Test_Yield_1();
    /* Change priority of thread 2 */
    RMP_Thd_Set(&Thd_2, 2, RMP_SLICE_MAX);
    Test_Mail_1();
    Test_Sem_1();
    Test_Msgq_1();
    Test_Bmq_1();
    while(1);
}
/* End Function:Test_Yield ***************************************************/

/* Function:Func_2 ************************************************************
Description : The test function group 2.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Test_Yield_2(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Yield();
        /* Read counter here */
        End=RMP_CNT_READ();
        Total+=(rmp_tim_t)(End-Start);
    }
}

void Test_Mail_2(void)
{
    rmp_ptr_t Data;
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Thd_Rcv(&Data, RMP_SLICE_MAX);
        /* Read counter here */
        End=RMP_CNT_READ();
        Total+=(rmp_tim_t)(End-Start);
    }
}

void Test_Sem_2(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Sem_Pend(&Sem_1, RMP_SLICE_MAX);
        /* Read counter here */
        End=RMP_CNT_READ();
        Total+=(rmp_tim_t)(End-Start);
    }
}

void Test_Fifo(void)
{
    rmp_cnt_t Count;
    struct RMP_List Node;
    volatile struct RMP_List* Receive;
    
    for(Count=0;Count<10000;Count++)
    {
        /* FIFO is different in that it is non-blocking */
        Start=RMP_CNT_READ();
        RMP_Fifo_Write(&Fifo_1, &Node);
        RMP_Fifo_Read(&Fifo_1, &Receive);
        End=RMP_CNT_READ();
        /* This must be the same thing */
        if(Receive!=&Node)
            while(1);
        Total+=(rmp_tim_t)(End-Start);
    }
}

void Test_Msgq_2(void)
{
    rmp_cnt_t Count;
    volatile struct RMP_List* Receive;
    
    for(Count=0;Count<10000;Count++)
    {
        RMP_Msgq_Rcv(&Msgq_1, &Receive, RMP_SLICE_MAX);
        /* Read counter here */
        End=RMP_CNT_READ();
        Total+=(rmp_tim_t)(End-Start);
    }
}

void Test_Bmq_2(void)
{
    rmp_cnt_t Count;
    volatile struct RMP_List* Receive;
    
    for(Count=0;Count<10000;Count++)
    {
        RMP_Bmq_Rcv(&Bmq_1, &Receive, RMP_SLICE_MAX);
        /* Read counter here */
        End=RMP_CNT_READ();
        Total+=(rmp_tim_t)(End-Start);
    }
}

void Test_Mail_ISR(void)
{
    rmp_ptr_t Data;
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Thd_Rcv(&Data, RMP_SLICE_MAX);
        /* Read counter here */
        End=RMP_CNT_READ();
        Total+=(rmp_tim_t)(End-Start);
    }
}

void Test_Sem_ISR(void)
{
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Sem_Pend(&Sem_1, RMP_SLICE_MAX);
        /* Read counter here */
        End=RMP_CNT_READ();
        Total+=(rmp_tim_t)(End-Start);
    }
}

void Test_Msgq_ISR(void)
{
    static rmp_cnt_t Count;
    volatile struct RMP_List* Receive;
    
    for(Count=0;Count<10000;Count++)
    {
        RMP_Msgq_Rcv(&Msgq_1, &Receive, RMP_SLICE_MAX);
        /* Read counter here */
        End=RMP_CNT_READ();
        Total+=(rmp_tim_t)(End-Start);
    }
}

void Test_Bmq_ISR(void)
{
    static rmp_cnt_t Count;
    volatile struct RMP_List* Receive;
    
    for(Count=0;Count<10000;Count++)
    {
        RMP_Bmq_Rcv(&Bmq_1, &Receive, RMP_SLICE_MAX);
        /* Read counter here */
        End=RMP_CNT_READ();
        Total+=(rmp_tim_t)(End-Start);
    }
}

#ifdef TEST_MEM_POOL
rmp_ptr_t Rand(void)
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
        LFSR^=0xB400;
    }
    else
        LFSR>>=1;
    
    return LFSR;
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Char;
    
    Char=*Arg1;
    *Arg1=*Arg2;
    *Arg2=Char;
}

void Test_Mem_Pool(void)
{
    static void* Mem[8];
    static rmp_u8_t Alloc[8];
    static rmp_u8_t Free[8];
    static rmp_u8_t Size[8];
    static rmp_ptr_t Amount[8];
    rmp_cnt_t Count;
    rmp_cnt_t Test_Count;
    
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
    RMP_Mem_Init(Pool, TEST_MEM_POOL*sizeof(rmp_ptr_t));
    for(Test_Count=0;Test_Count<10000;Test_Count++)
    {
        /* Random sequence and number generation */
        for(Count=0;Count<8;Count++)
        {
            Alloc[Count]=(rmp_u8_t)Count;
            Free[Count]=(rmp_u8_t)Count;
            Size[Count]=(rmp_u8_t)Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%((rmp_ptr_t)Count+1U)]);
            Swap(&Free[Count], &Free[Rand()%((rmp_ptr_t)Count+1U)]);
            Swap(&Size[Count], &Size[Rand()%((rmp_ptr_t)Count+1U)]);
        }
        
        Start=RMP_CNT_READ();
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
        End=RMP_CNT_READ();
        Total+=(rmp_tim_t)(End-Start);
        
        /* This should always be successful because we deallocated everything else */
        Mem[0]=RMP_Malloc(Pool, (TEST_MEM_POOL>>7)*127);
        if(Mem[0]==0)
        {
            RMP_DBG_S("Memory test failure: ");
            RMP_DBG_I(Test_Count);
            RMP_DBG_S(" runs.\r\n");
            while(1);
        }
        RMP_Free(Pool, Mem[0]); 
    }
    
    RMP_DBG_S("Memory: ");
    Memory_Time=Total/160000;
    RMP_DBG_I(Memory_Time);
    RMP_DBG_S(" cycles.\r\n");
}
#endif

#ifdef RMP_COVERAGE
/* The first test set - deletion and miscellaneous cases */
void Test_Set_1(rmp_ptr_t Param)
{
    rmp_ptr_t Data;
    switch(Param)
    {
        case 0:
        {
            /* Scenario 1 start */
            RMP_Thd_Rcv(&Data, RMP_SLICE_MAX);
            
            /* Scenario 2 start */
            RMP_Thd_Rcv(&Data, RMP_SLICE_MAX);
            RMP_Thd_Delay(200);
            
            /* Scenario 3 start */
            RMP_Thd_Rcv(&Data, RMP_SLICE_MAX);
            RMP_Thd_Delay(200);
            
            /* Scenario 4 start */
            RMP_Thd_Rcv(&Data, RMP_SLICE_MAX);
            
            /* Scenario 5 start */
            RMP_Thd_Rcv(&Data, RMP_SLICE_MAX);
            
            /* Final fuzzing start */
            RMP_Thd_Rcv(&Data, RMP_SLICE_MAX);
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
            RMP_Thd_Rcv(&Data, RMP_SLICE_MAX);
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
            RMP_Thd_Snd(&Thd_2, 1, RMP_SLICE_MAX); /* Must be successful */
            RMP_Thd_Snd(&Thd_2, 1, 0);              /* Will return immediately */
            RMP_Thd_Snd(&Thd_2, 1, RMP_SLICE_MAX); /* Will block */
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
            RMP_Sem_Pend(&Sem_1, RMP_SLICE_MAX);
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
            RMP_Thd_Snd(&Thd_1, 1, RMP_SLICE_MAX); /* Must be successful */
            RMP_Thd_Snd(&Thd_1, 1, RMP_SLICE_MAX); /* Will block */
            /* Self delete, because if thread 1 gets deleted this will be released so thread 2 gets preempted */
            RMP_Thd_Del(&Thd_Test);
            while(1);
        }
        case 10:
        {
            /* Test deletion while someone is delaying on it - This will be created with Test_Thd */
            RMP_Thd_Snd(&Thd_1, 1, RMP_SLICE_MAX); /* Must be successful */
            RMP_Thd_Snd(&Thd_1, 1, 10000); /* Will block */
            /* Self delete, because if thread 1 gets deleted this will be released so thread 2 gets preempted */
            RMP_Thd_Del(&Thd_Test);
            while(1);
        }
    }
}

/* The second test set - semaphore pending and timing */
void Test_Set_2(rmp_ptr_t Param)
{
    volatile rmp_ptr_t Middle;
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
            RMP_Sem_Pend(&Sem_1, RMP_SLICE_MAX);   /* Delete sem in pend & pend abort */
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
    rmp_ptr_t Data;
    rmp_ptr_t Temp;
    rmp_ptr_t Stub;
    
    /* Scenario 0: Miscellaneous tests */
    RMP_Sched_Lock();
    RMP_Yield();
    RMP_Sched_Unlock();
    
    /* Restart thread 1 for testing */
    RMP_Thd_Del(&Thd_1);
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)0, 2, 5);
    
    /* Scenario 1: Delay ourself for a while. 
     *             The thread 1 will be active for some time, 
     *             before thread 2 wakes up */
    RMP_Thd_Snd(&Thd_1, 1, RMP_SLICE_MAX);
    RMP_Thd_Delay(100);
    
    /* Scenario 2: Simultaneous delays.
     *             The thread 1 and thread 2 will delay for 200/100 cycles.
     *             Thread1's delay will be cleared while thread2 is running */
    RMP_Thd_Snd(&Thd_1, 1, RMP_SLICE_MAX);
    RMP_Thd_Delay(100);
    /* Thread 2 will busy loop for 200 cycles */
    Data=RMP_Tick;
    while((RMP_Tick-Data)<200);
    
    /* Scenario 3: Simultaneous delays.
     *             The thread 1 and thread 2 will delay for 200/400 cycles,
     *             with thread 2's delay inserted before thread 1 in the chain,
     *             and thread 1 wakes up while it is suspended. */
    RMP_Thd_Snd(&Thd_1, 1, RMP_SLICE_MAX);
    RMP_Thd_Delay(50);
    RMP_Thd_Suspend(&Thd_1);
    RMP_Thd_Delay(50);
    RMP_Thd_Delay(200);
    RMP_Thd_Resume(&Thd_1);
    
    /* Scenario 4: Timeout on own mailbox receive.
     *             The thread 1 will be active for some time,
     *             before thread 2 timeouts */
    RMP_Thd_Snd(&Thd_1, 1, RMP_SLICE_MAX);
    RMP_Thd_Rcv(&Data, 100);
    
    /* Scenario 5: Timeout on thread 1 mailbox send. The thread 1 will
     *             be suspended by us first, then we send to it.
     *             The thread 1 will be active for some time,
     *             before thread 2 timeouts */
    RMP_Thd_Snd(&Thd_1, 1, RMP_SLICE_MAX);
    RMP_Thd_Suspend(&Thd_1);
    RMP_Thd_Snd(&Thd_1, 1, 100);
    RMP_Thd_Resume(&Thd_1);
    
    /* Scenario 6: stuff kernel API intentionally with trash to see if they can handle them correctly */
    RMP_Thd_Snd(&Thd_1, 1, RMP_SLICE_MAX);
    RMP_Thd_Delay(100);
    /* Thread creation */
    RMP_Thd_Crt(0, 0, 0, 0, 1, 10);                             /* NULL pointer */
    RMP_Thd_Crt(&Thd_Test, 0, 0, 0, RMP_PREEMPT_PRIO_NUM, 10);  /* Wrong priority */
    RMP_Thd_Crt(&Thd_Test, 0, 0, 0, 1, 0);                      /* Wrong slices - too small */
    RMP_Thd_Crt(&Thd_Test, 0, 0, 0, 1, RMP_SLICE_MAX);         /* Wrong slices - too large */
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
    RMP_Thd_Delay(RMP_SLICE_MAX);                              /* Wrong slices - too large */
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
    RMP_Sem_Crt(&Sem_1, RMP_SEM_CNT_MAX);                       /* Number too large */
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
    RMP_Sem_Post(&Sem_1, RMP_SEM_CNT_MAX);                      /* Post number too large */
    RMP_Sem_Post(&Sem_1, RMP_SEM_CNT_MAX/3*2);                  /* - */
    RMP_Sem_Post(&Sem_1, RMP_SEM_CNT_MAX/3*2);                  /* Total number too large */
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
    RMP_Thd_Snd(&Thd_1, 1, RMP_SLICE_MAX);                     /* Receive delay cancelled by send here */
    RMP_Thd_Del(&Thd_1);                                        /* Deletion while in suspension */
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)5, 3, 5); 
    RMP_Thd_Del(&Thd_1);                                        /* Deletion while in send block */
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)6, 3, 5); 
    RMP_Thd_Del(&Thd_1);                                        /* Deletion while in send delay */
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)6, 3, 5);
    RMP_Thd_Rcv(&Data, RMP_SLICE_MAX);                         /* - */
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
    RMP_Thd_Set(&Thd_1, 1, RMP_SLICE_MAX);                     /* Set priority but not slices */
    RMP_Thd_Set(&Thd_1, RMP_PREEMPT_PRIO_NUM, 10);              /* Set slices but not priority */
    RMP_Thd_Set(&Thd_1, 1, 7);                                  /* Set them both - to the same prio */
    RMP_Thd_Set(&Thd_1, 3, 7);                                  /* Set them both - different prio with preemption */
    RMP_Thd_Set(&Thd_1, RMP_PREEMPT_PRIO_NUM, RMP_SLICE_MAX);  /* Set none of them */
    /* Do the same while it is delaying */ 
    RMP_Thd_Set(&Thd_1, 1, RMP_SLICE_MAX);                     /* Set priority but not slices */
    RMP_Thd_Set(&Thd_1, RMP_PREEMPT_PRIO_NUM, 10);              /* Set slices but not priority */
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
    RMP_Sched_Lock();
    RMP_Thd_Snd_ISR(0, 1);                                      /* NULL pointer */
    RMP_Thd_Snd_ISR(&Thd_Test, 1);                              /* Send to deleted thread */
    RMP_Sched_Unlock();
    
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)3, 3, 5);
    RMP_Sched_Lock();
    RMP_Thd_Snd_ISR(&Thd_1, 1);                                /* Send success with blocked thread */
    _RMP_Tick_Near();
    RMP_Sched_Unlock();
    RMP_Thd_Del(&Thd_1);
    
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)4, 3, 5);
    RMP_Sched_Lock();
    RMP_Thd_Snd_ISR(&Thd_1, 1);                                /* Send success with delaying thread */
    _RMP_Tick_Near();
    RMP_Thd_Snd_ISR(&Thd_1, 1);                                /* Send failure due to full mailbox */
    _RMP_Tick_Near();
    RMP_Sched_Unlock();
    RMP_Thd_Del(&Thd_1);
     
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)2, 3, 5);
    RMP_Sched_Lock();
    RMP_Thd_Snd_ISR(&Thd_1, 1);                                /* Send success with thread not blocked */
    _RMP_Tick_Near();
    RMP_Sched_Unlock();
    RMP_Thd_Del(&Thd_1);
    
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)3, 1, 5);
    RMP_Thd_Delay(100);
    RMP_Sched_Lock();
    RMP_Thd_Snd_ISR(&Thd_1, 1);                                /* Send success with thread not preempting current thread */
    _RMP_Tick_Near();
    RMP_Sched_Unlock();
    RMP_Thd_Del(&Thd_1);
    
    /* Semaphore pend tests */
    RMP_Sem_Post_ISR(&Sem_1, RMP_SEM_CNT_MAX);                  /* Posted number too large */
    RMP_Sem_Del(&Sem_1);
    RMP_Sched_Lock();
    RMP_Sem_Post_ISR(0, 1);                                     /* NULL pointer */
    RMP_Sem_Post_ISR(&Sem_1, 2);                                /* Post to deleted semaphore */
    RMP_Sched_Unlock();
    
    RMP_Sem_Crt(&Sem_1, 0);
    RMP_Thd_Crt(&Thd_1, Test_Set_2, THD1_STACK, (void*)2, 3, 5);
    RMP_Sched_Lock();
    RMP_Sem_Post_ISR(&Sem_1, 1);                                /* Post success with blocked thread */
    RMP_Sched_Unlock();
    RMP_Thd_Del(&Thd_1);
    
    RMP_Thd_Crt(&Thd_1, Test_Set_2, THD1_STACK, (void*)3, 3, 5);
    RMP_Sched_Lock();
    RMP_Sem_Post_ISR(&Sem_1, 1);                                /* Post success with delaying thread */
    RMP_Sched_Unlock();
    RMP_Thd_Del(&Thd_1);
    
    RMP_Thd_Crt(&Thd_1, Test_Set_1, THD1_STACK, (void*)2, 3, 5);
    RMP_Sched_Lock();
    RMP_Sem_Post_ISR(&Sem_1, 1);                                /* Send success with thread not pending */
    RMP_Sched_Unlock();
    RMP_Thd_Del(&Thd_1);
    RMP_Sem_Del(&Sem_1);
    
    RMP_Sem_Crt(&Sem_1, 0);
    RMP_Thd_Crt(&Thd_1, Test_Set_2, THD1_STACK, (void*)2, 1, 5);
    RMP_Thd_Delay(100);
    RMP_Sched_Lock();
    RMP_Sem_Post_ISR(&Sem_1, 1);                                /* Post success with thread not preempting current thread */
    RMP_Sched_Unlock();
    RMP_Thd_Del(&Thd_1);
    
    /* Memory pool fuzzing */
    RMP_Malloc(Pool, TEST_MEM_POOL*sizeof(rmp_ptr_t)*2);        /* Allocate more memory than we have */
    RMP_Free(Pool,0);                                           /* NULL pointer */
    RMP_Free(Pool,(void*)1);                                    /* Not in freeable range */
    RMP_Free(0,(void*)100);                                     /* NULL pointer */
    RMP_Malloc(0,100);                                          /* NULL pointer */
    RMP_Malloc(Pool,0);                                         /* Allocate zero size */
    Data=(rmp_ptr_t)RMP_Malloc(Pool, 256);                      /* - */
    RMP_Free(Pool,(void*)Data);                                 /* - */
    RMP_Free(Pool,(void*)Data);                                 /* Double free */
    RMP_Mem_Init(0, TEST_MEM_POOL*sizeof(rmp_ptr_t));           /* NULL pointer */
    RMP_Mem_Init(Pool, 0);                                      /* Memory too small */
    RMP_Mem_Init((void*)(((rmp_ptr_t)Pool)+1), TEST_MEM_POOL*sizeof(rmp_ptr_t)); /* Memory not aligned */
    
    /* Testing reallocations */
    RMP_Realloc(0, (void*)0, 100);                              /* Realloc no pool */
    RMP_Realloc(Pool, (void*)1, 100);                           /* Realloc out of range */
    
    /* Realloc-based memory allocations */
    Data=(rmp_ptr_t)RMP_Realloc(Pool, (void*)0, TEST_MEM_POOL*sizeof(rmp_ptr_t)/2); /* Realloc some memory - malloc */
    Data=(rmp_ptr_t)RMP_Realloc(Pool, (void*)Data, TEST_MEM_POOL*sizeof(rmp_ptr_t)/2); /* Realloc the same size - no change at all */
    Data=(rmp_ptr_t)RMP_Realloc(Pool, (void*)Data, TEST_MEM_POOL*sizeof(rmp_ptr_t)/4); /* Decrease the amount of memory */
    Data=(rmp_ptr_t)RMP_Realloc(Pool, (void*)Data, TEST_MEM_POOL*sizeof(rmp_ptr_t)/4*3); /* Increase the amount of memory */
    
    /* Increase to merge the right side until failure */
    Temp=0;
    while(1)
    {
        Temp+=32;
        if(RMP_Realloc(Pool, (void*)Data, TEST_MEM_POOL*sizeof(rmp_ptr_t)/4*3+Temp)==0)
            break;
    }
    RMP_Realloc(Pool, (void*)Data, TEST_MEM_POOL*sizeof(rmp_ptr_t)/4*3+Temp-256); /* Back off some memory */
    RMP_Realloc(Pool, (void*)Data, TEST_MEM_POOL*sizeof(rmp_ptr_t)/4*3+Temp+512); /* Overrun the right side */
    RMP_Realloc(Pool, (void*)Data, TEST_MEM_POOL*sizeof(rmp_ptr_t)/4*3+Temp-256); /* Shrink back */
    Stub=(rmp_ptr_t)RMP_Malloc(Pool, 128);                      /* Malloc to make right side allocated */
    RMP_Realloc(Pool, (void*)Data, TEST_MEM_POOL*sizeof(rmp_ptr_t)/4*3+Temp-32); /* This expansion should fail */
    /* Realloc to the new boundary */
    Temp-=256;
    while(1)
    {
        Temp+=32;
        if(RMP_Realloc(Pool, (void*)Data, TEST_MEM_POOL*sizeof(rmp_ptr_t)/4*3+Temp)==0)
            break;
    }
    RMP_Realloc(Pool, (void*)Data, TEST_MEM_POOL*sizeof(rmp_ptr_t)/4*3+Temp-64); /* Shrink with right side allocated and less than 64B stub */
    RMP_Realloc(Pool, (void*)Data, 0);                          /* Realloc zero size - equivalent to free */
    RMP_Realloc(Pool, (void*)Data, 100);                        /* Realloc something already freed */
    Stub=(rmp_ptr_t)RMP_Realloc(Pool, (void*)Stub, TEST_MEM_POOL*sizeof(rmp_ptr_t)/2); /* Realloc to move memory content and expand size */
    RMP_Free(Pool, (void*)Stub);                                /* Kill the stub now */
    
    /* Memory allocations */
    Data=(rmp_ptr_t)RMP_Malloc(Pool, TEST_MEM_POOL*sizeof(rmp_ptr_t)/5*4); /* Should find it on the first try */
    
    /* Test residue smaller than 64 - keep allocating 64 byte until we run out of memory.
     * memory leaking is fine because this is just a test */
    while(1)
    {
        Data=(rmp_ptr_t)RMP_Malloc(Pool, 64);
        if(Data==0)
            break;
        /* Last good allocation stored here */
        Temp=Data;
        /* Iterate on this function */
        RMP_Sched_Lock();
        _RMP_Tick_Near();
        RMP_Sched_Unlock();
    }
    RMP_Realloc(Pool,(void*)Temp,256);                          /* Test realloc cannot expand */
    RMP_Free(Pool,(void*)Temp);                                 /* Test cannot merge with right side */
    
    RMP_Thd_Crt(&Thd_1, Test_Set_2, THD1_STACK, (void*)4, 2, 1);
    RMP_Yield();
    /* Thread 2 will busy loop for 1000 cycles */
    Data=RMP_Tick;
    while((RMP_Tick-Data)<1000)
    {
        RMP_Sched_Lock();
        _RMP_Tick_Near();
        RMP_Sched_Unlock();
    }
    RMP_Thd_Del(&Thd_1);
}
#endif

void Func_2(void)
{
    /* Yield tests */
    Total=0;
    Test_Yield_2();
    RMP_DBG_S("Yield: ");

    Yield_Time=Total/10000;
    RMP_DBG_I(Yield_Time);
    RMP_DBG_S(" cycles.\r\n");
    /* Change priority of thread 2, just in case */
    RMP_Thd_Set(&Thd_2,2,RMP_SLICE_MAX);
    
    /* Mailbox tests */
    Total=0;
    Test_Mail_2();
    RMP_DBG_S("Mailbox: ");
    Mailbox_Time=Total/10000;
    RMP_DBG_I(Mailbox_Time);
    RMP_DBG_S(" cycles.\r\n");
    
    /* Semaphore tests */
    Total=0;
    Test_Sem_2();
    RMP_DBG_S("Semaphore: ");
    Semaphore_Time=Total/10000;
    RMP_DBG_I(Semaphore_Time);
    RMP_DBG_S(" cycles.\r\n");
    
    /* Fifo tests */
    Total=0;
    Test_Fifo();
    RMP_DBG_S("FIFO: ");
    Fifo_Time=Total/10000;
    RMP_DBG_I(Fifo_Time);
    RMP_DBG_S(" cycles.\r\n");
    
    /* Message queue tests */
    Total=0;
    Test_Msgq_2();
    RMP_DBG_S("Message queue: ");
    Msgq_Time=Total/10000;
    RMP_DBG_I(Msgq_Time);
    RMP_DBG_S(" cycles.\r\n");
    
    /* Blocking message queue tests */
    Total=0;
    Test_Bmq_2();
    RMP_DBG_S("Blocking message queue: ");
    Bmq_Time=Total/10000;
    RMP_DBG_I(Bmq_Time);
    RMP_DBG_S(" cycles.\r\n");
    
    /* Memory pool tests */
#ifdef TEST_MEM_POOL
    Test_Mem_Pool();
#endif

    /* Prepare interrupt tests */
    Int_Init();
    
    /* Mailbox from interrupt tests */
    Total=0;
    Test_Mail_ISR();
    Temp1=Total;
    
    /* Semaphore from interrupt tests */
    Total=0;
    Test_Sem_ISR();
    Temp2=Total;
    
    /* Message queue from interrupt tests */
    Total=0;
    Test_Msgq_ISR();
    Temp3=Total;
    
    /* Blocking message queue from interrupt tests */
    Total=0;
    Test_Bmq_ISR();
    
    /* Print results */
    RMP_DBG_S("Mailbox-ISR: ");
    Mailbox_ISR_Time=Temp1/10000;
    RMP_DBG_I(Mailbox_ISR_Time);
    RMP_DBG_S(" cycles.\r\n");
    
    RMP_DBG_S("Semaphore-ISR: ");
    Semaphore_ISR_Time=Temp2/10000;
    RMP_DBG_I(Semaphore_ISR_Time);
    RMP_DBG_S(" cycles.\r\n");
    
    RMP_DBG_S("Message queue-ISR: ");
    Msgq_ISR_Time=Temp3/10000;
    RMP_DBG_I(Msgq_ISR_Time);
    RMP_DBG_S(" cycles.\r\n");
    
    RMP_DBG_S("Blocking message queue-ISR: ");
    Bmq_ISR_Time=Total/10000;
    RMP_DBG_I(Bmq_ISR_Time);
    RMP_DBG_S(" cycles.\r\n");

#ifdef RMP_COVERAGE
    Test_Coverage_2();
    RMP_Print_Coverage();
#endif
    
    while(1);
}
/* End Function:Func_2 *******************************************************/

/* Function:Int_Handler *******************************************************
Description : The interrupt handler. Call this in your periodic ISR.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Handler(void)
{
    static rmp_cnt_t Count=0;
    static struct RMP_List Node;
    
    if(Count<10000)
    {
        Count++;
        Start=RMP_CNT_READ();
        if(RMP_Thd_Snd_ISR(&Thd_2, 1)<0)
        {
            RMP_DBG_S("ISR Mailbox send failure: ");
            RMP_DBG_I(Count);
            RMP_DBG_S(" sends.\r\n");
            while(1);
        }
    }
    else if(Count<20000)
    {
        Count++;
        Start=RMP_CNT_READ();
        if(RMP_Sem_Post_ISR(&Sem_1, 1)<0)
        {
            RMP_DBG_S("ISR semaphore post failure: ");
            RMP_DBG_I(Count);
            RMP_DBG_S(" posts.\r\n");
            while(1);
        }
    }
    else if(Count<30000)
    {
        Count++;
        Start=RMP_CNT_READ();
        if(RMP_Msgq_Snd_ISR(&Msgq_1, &Node)<0)
        {
            RMP_DBG_S("ISR msgq message send failure: ");
            RMP_DBG_I(Count);
            RMP_DBG_S(" sends.\r\n");
            while(1);
        }
    }
    else if(Count<40000)
    {
        Count++;
        Start=RMP_CNT_READ();
        if(RMP_Bmq_Snd_ISR(&Bmq_1, &Node)<0)
        {
            RMP_DBG_S("ISR bmq message send failure: ");
            RMP_DBG_I(Count);
            RMP_DBG_S(" sends.\r\n");
            while(1);
        }
    }
    else
        Int_Disable();
}
#endif
/* End Function:Int_Handler **************************************************/

/* Function:RMP_Init **********************************************************
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
    RMP_Clear(&Thd_1, sizeof(struct RMP_Thd));
    RMP_Clear(&Thd_2, sizeof(struct RMP_Thd));
    RMP_Clear(&Sem_1, sizeof(struct RMP_Sem));
    RMP_Clear(&Fifo_1, sizeof(struct RMP_Fifo));
    RMP_Clear(&Msgq_1, sizeof(struct RMP_Msgq));
    RMP_Clear(&Bmq_1, sizeof(struct RMP_Bmq));
    /* Create kernel objects */
    RMP_Sem_Crt(&Sem_1, 0U);
    RMP_Fifo_Crt(&Fifo_1);
    RMP_Msgq_Crt(&Msgq_1);
    RMP_Bmq_Crt(&Bmq_1, 1U);
    
    /* Start threads */
    RMP_Thd_Crt(&Thd_1, (void*)Func_1, THD1_STACK, (void*)0x1234U, 1U, 5U);
    RMP_Thd_Crt(&Thd_2, (void*)Func_2, THD2_STACK, (void*)0x4321U, 1U, 1000U);
#endif
}

void RMP_Init_Idle(void)
{
    return;
}
/* End Function:RMP_Init *****************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
