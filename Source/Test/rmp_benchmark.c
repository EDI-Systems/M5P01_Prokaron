/******************************************************************************
Filename    : rmp_benchmark.c
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The performance benchmark for RMP. Do not modify this file; what
              you need to modify is the test chip header and the platform chip
              header.
******************************************************************************/

/* Includes ******************************************************************/
#include "rmp_test.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Number of rounds to test */
#define ROUND_NUM       10000
/* Interval of timer overflow reports: 
 * ~ 3MHz               100
 * ~ 30MHz              10000
 * ~ 300MHz             100000 */
#define OVERFLOW_NUM    100000
/* Whether to include FPU context */
#define FLOAT_CONTEXT

/* Data definitions */
#define TIME            ((rmp_tim_t)(End-Start))
#define TOTAL()         (Total+=(TIME))
#define MAX()           (Max=((TIME)>(Max)?(TIME):(Max)))
#define MIN()           (Min=((TIME)<(Min)?(TIME):(Min)))

/* Data initialization */
#define INIT() \
do \
{ \
    Total=0; \
    Max=0; \
    Min=65535; \
} \
while(0)

/* Data extraction */
#define DATA() \
do \
{ \
    TOTAL(); \
    MAX(); \
    MIN(); \
} \
while(0)
    
/* Data printing */
#define LIST(X) \
do \
{ \
    RMP_DBG_S(X); \
    RMP_DBG_S(" : "); \
    RMP_DBG_I(Total/ROUND_NUM); \
    RMP_DBG_S(" / "); \
    RMP_DBG_I(Max); \
    RMP_DBG_S(" / "); \
    RMP_DBG_I(Min); \
    RMP_DBG_S("\r\n"); \
} \
while(0)
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
#ifndef MINIMAL_SIZE
volatile rmp_tim_t Start=0;
volatile rmp_tim_t End=0;
volatile rmp_tim_t Min=0;
volatile rmp_tim_t Max=0;
volatile rmp_u32_t Overflow=0;
#ifdef PTR_16_BIT
volatile rmp_u32_t Total=0;
volatile rmp_u32_t Mail_ISR_Total=0;
volatile rmp_u32_t Sem_ISR_Total=0;
volatile rmp_u32_t Msgq_ISR_Total=0;
volatile rmp_u32_t Bmq_ISR_Total=0;
#else
volatile rmp_ptr_t Total=0;
volatile rmp_ptr_t Mail_ISR_Total=0;
volatile rmp_ptr_t Sem_ISR_Total=0;
volatile rmp_ptr_t Msgq_ISR_Total=0;
volatile rmp_ptr_t Bmq_ISR_Total=0;
#endif
volatile rmp_tim_t Mail_ISR_Max=0;
volatile rmp_tim_t Mail_ISR_Min=0;
volatile rmp_tim_t Sem_ISR_Max=0;
volatile rmp_tim_t Sem_ISR_Min=0;
volatile rmp_tim_t Msgq_ISR_Max=0;
volatile rmp_tim_t Msgq_ISR_Min=0;
volatile rmp_tim_t Bmq_ISR_Max=0;
volatile rmp_tim_t Bmq_ISR_Min=0;
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
/* Floating point context */
volatile float Float=0.0f;

/* Test functions */
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

void Func_2(void);
/* End Globals ***************************************************************/

/* Begin Function:Func_1 ******************************************************
Description : The test function group 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Test_Yield_1(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        /* Read counter here */
        Start=COUNTER_READ();
        RMP_Yield();
    }
}

void Test_Mail_1(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        /* Read counter here */
        Start=COUNTER_READ();
        RMP_Thd_Snd(&Thd_2, 1, RMP_SLICE_MAX);
    }
}

void Test_Sem_1(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        /* Read counter here */
        Start=COUNTER_READ();
        RMP_Sem_Post(&Sem_1, 1);
    }
}

void Test_Msgq_1(void)
{
    rmp_cnt_t Count;
    struct RMP_List Node;
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        /* Read counter here */
        Start=COUNTER_READ();
        RMP_Msgq_Snd(&Msgq_1, &Node);
    }
}

void Test_Bmq_1(void)
{
    rmp_cnt_t Count;
    struct RMP_List Node;
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        /* Read counter here */
        Start=COUNTER_READ();
        RMP_Bmq_Snd(&Bmq_1, &Node, RMP_SLICE_MAX);
    }
}

void Func_1(void)
{
#ifdef FLOAT_CONTEXT
    Float+=1.0f;
#endif

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

/* Begin Function:Func_2 ******************************************************
Description : The test function group 2.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Test_Yield_2(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        RMP_Yield();
        /* Read counter here */
        End=COUNTER_READ();
        DATA();
    }
}

void Test_Mail_2(void)
{
    rmp_ptr_t Data;
    rmp_cnt_t Count;
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        RMP_Thd_Rcv(&Data, RMP_SLICE_MAX);
        /* Read counter here */
        End=COUNTER_READ();
        DATA();
    }
}

void Test_Sem_2(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        RMP_Sem_Pend(&Sem_1, RMP_SLICE_MAX);
        /* Read counter here */
        End=COUNTER_READ();
        DATA();
    }
}

void Test_Fifo(void)
{
    rmp_cnt_t Count;
    struct RMP_List Node;
    volatile struct RMP_List* Receive;
    
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        /* FIFO is different in that it is non-blocking */
        Start=COUNTER_READ();
        RMP_Fifo_Write(&Fifo_1, &Node);
        RMP_Fifo_Read(&Fifo_1, &Receive);
        End=COUNTER_READ();
        DATA();
        /* This must be the same thing */
        if(Receive!=&Node)
            while(1);
    }
}

void Test_Msgq_2(void)
{
    rmp_cnt_t Count;
    volatile struct RMP_List* Receive;
    
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        RMP_Msgq_Rcv(&Msgq_1, &Receive, RMP_SLICE_MAX);
        /* Read counter here */
        End=COUNTER_READ();
        DATA();
    }
}

void Test_Bmq_2(void)
{
    rmp_cnt_t Count;
    volatile struct RMP_List* Receive;
    
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        RMP_Bmq_Rcv(&Bmq_1, &Receive, RMP_SLICE_MAX);
        /* Read counter here */
        End=COUNTER_READ();
        DATA();
    }
}

void Test_Mail_ISR(void)
{
    rmp_ptr_t Data;
    static rmp_cnt_t Count;
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        RMP_Thd_Rcv(&Data, RMP_SLICE_MAX);
        /* Read counter here */
        End=COUNTER_READ();
        DATA();
    }
}

void Test_Sem_ISR(void)
{
    static rmp_cnt_t Count;
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        RMP_Sem_Pend(&Sem_1, RMP_SLICE_MAX);
        /* Read counter here */
        End=COUNTER_READ();
        DATA();
    }
}

void Test_Msgq_ISR(void)
{
    static rmp_cnt_t Count;
    volatile struct RMP_List* Receive;
    
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        RMP_Msgq_Rcv(&Msgq_1, &Receive, RMP_SLICE_MAX);
        /* Read counter here */
        End=COUNTER_READ();
        DATA();
    }
}

void Test_Bmq_ISR(void)
{
    static rmp_cnt_t Count;
    volatile struct RMP_List* Receive;
    
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        RMP_Bmq_Rcv(&Bmq_1, &Receive, RMP_SLICE_MAX);
        /* Read counter here */
        End=COUNTER_READ();
        DATA();
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
    RMP_Mem_Init(Pool, TEST_MEM_POOL*sizeof(rmp_ptr_t));
    for(Test_Count=0;Test_Count<ROUND_NUM;Test_Count++)
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
        DATA();
        
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
    
    Total/=8;
    Max/=8;
    Min/=8;
}
#endif

void Func_2(void)
{
    /* Print table header */
    RMP_DBG_S("\r\n    ___   __  ___ ___\r\n");
    RMP_DBG_S("   / _ \\ /  |/  // _ \\       Simple real-time kernel\r\n");
    RMP_DBG_S("  / , _// /|_/ // ___/       Standard benchmark test\r\n");
    RMP_DBG_S(" /_/|_|/_/  /_//_/\r\n");
    RMP_DBG_S("====================================================\r\n");
    RMP_DBG_S("Test (number in CPU cycles)        : AVG / MAX / MIN\r\n");

#ifdef FLOAT_CONTEXT
    Float+=1.0f;
#endif
    
    /* Yield tests */
    INIT();
    Test_Yield_2();
    LIST("Yield                             ");
    
    /* Change priority of thread 2, just in case */
    RMP_Thd_Set(&Thd_2,2,RMP_SLICE_MAX);
    
    /* Mailbox tests */
    INIT();
    Test_Mail_2();
    LIST("Mailbox                           ");
    
    /* Semaphore tests */
    INIT();
    Test_Sem_2();
    LIST("Semaphore                         ");
    
    /* Fifo tests */
    INIT();
    Test_Fifo();
    LIST("FIFO                              ");
    
    /* Message queue tests */
    INIT();
    Test_Msgq_2();
    LIST("Message queue                     ");
    
    /* Blocking message queue tests */
    INIT();
    Test_Bmq_2();
    LIST("Blocking message queue            ");
    
    /* Memory pool tests */
#ifdef TEST_MEM_POOL
    INIT();
    Test_Mem_Pool();
    LIST("Memory allocation/free pair       ");
#endif

    /* Prepare interrupt tests */
    Int_Init();
    
    /* Mailbox from interrupt tests */
    INIT();
    Test_Mail_ISR();
    Mail_ISR_Total=Total;
    Mail_ISR_Max=Max;
    Mail_ISR_Min=Min;
    
    /* Semaphore from interrupt tests */
    INIT();
    Test_Sem_ISR();
    Sem_ISR_Total=Total;
    Sem_ISR_Max=Max;
    Sem_ISR_Min=Min;
    
    /* Message queue from interrupt tests */
    INIT();
    Test_Msgq_ISR();
    Msgq_ISR_Total=Total;
    Msgq_ISR_Max=Max;
    Msgq_ISR_Min=Min;
    
    /* Blocking message queue from interrupt tests */
    INIT();
    Test_Bmq_ISR();
    Bmq_ISR_Total=Total;
    Bmq_ISR_Max=Max;
    Bmq_ISR_Min=Min;
    
    /* Print results */
    Total=Mail_ISR_Total;
    Max=Mail_ISR_Max;
    Min=Mail_ISR_Min;
    LIST("ISR Mailbox                       ");
    
    Total=Sem_ISR_Total;
    Max=Sem_ISR_Max;
    Min=Sem_ISR_Min;
    LIST("ISR Semaphore                     ");
    
    Total=Msgq_ISR_Total;
    Max=Msgq_ISR_Max;
    Min=Msgq_ISR_Min;
    LIST("ISR Message queue                 ");
    
    Total=Bmq_ISR_Total;
    Max=Bmq_ISR_Max;
    Min=Bmq_ISR_Min;
    LIST("ISR Blocking message queue        ");
    
    /* Test stop - keep dumping counter values to detect potential wrong
     * timer clock rate configurations */
    while(1)
    {
        Start=End;
        End=(rmp_u16_t)COUNTER_READ();
        if(Start>End)
        {
            Overflow++;
            if((Overflow%OVERFLOW_NUM)==0)
            {
                RMP_DBG_I(OVERFLOW_NUM);
                RMP_DBG_S(" overflows\r\n");
            }
        }
    }
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
    static rmp_ptr_t Count=0U;
    static struct RMP_List Node;
    
    if(Count<ROUND_NUM)
    {
        Count++;
        Start=COUNTER_READ();
        if(RMP_Thd_Snd_ISR(&Thd_2, 1)<0)
        {
            RMP_DBG_S("ISR Mailbox send failure: ");
            RMP_DBG_I(Count);
            RMP_DBG_S(" sends.\r\n");
            while(1);
        }
    }
    else if(Count<ROUND_NUM*2U)
    {
        Count++;
        Start=COUNTER_READ();
        if(RMP_Sem_Post_ISR(&Sem_1, 1)<0)
        {
            RMP_DBG_S("ISR semaphore post failure: ");
            RMP_DBG_I(Count);
            RMP_DBG_S(" posts.\r\n");
            while(1);
        }
    }
    else if(Count<ROUND_NUM*3U)
    {
        Count++;
        Start=COUNTER_READ();
        if(RMP_Msgq_Snd_ISR(&Msgq_1, &Node)<0)
        {
            RMP_DBG_S("ISR msgq message send failure: ");
            RMP_DBG_I(Count);
            RMP_DBG_S(" sends.\r\n");
            while(1);
        }
    }
    else if(Count<ROUND_NUM*4U)
    {
        Count++;
        Start=COUNTER_READ();
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
    
    /* Start threads - make sure thread 2 is scheduled first in the test */
    RMP_Thd_Crt(&Thd_2, (void*)Func_2, THD2_STACK, (void*)0x4321U, 1U, 1000U);
    RMP_Thd_Crt(&Thd_1, (void*)Func_1, THD1_STACK, (void*)0x1234U, 1U, 5U);
#endif
}

void RMP_Init_Idle(void)
{
    return;
}
/* End Function:RMP_Init *****************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
