/******************************************************************************
Filename    : rmp_benchmark.c
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The performance benchmark for RMP. Do not modify this file; what
              you need to modify is the test chip header and the platform chip
              header.
              When Yield Max < 0, there's a chance that the RMP_YIELD is not
              implemented correctly and the interrupt is not triggered
              immediately after writing to the software interrupt register.
              There's also a chance that the stack overflowed.
              When testing ISR latencies, the timer firing intervals shall be 
              set as such that no timer interrupt may be fired while the former
              one is still being processed, especially considering the fact that
              the OS tick timer may kick in at any time.
******************************************************************************/

/* Include *******************************************************************/
#include "rmp_test.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Number of rounds to test - default to 10000 */
#ifndef ROUND_NUM
#define ROUND_NUM       10000
#endif
/* Recommended interval of timer overflow reports: 
 * ~ 3MHz               1000
 * ~ 30MHz              10000
 * ~ 300MHz             100000 */
#ifndef OVERFLOW_NUM
#define OVERFLOW_NUM    10000U
#endif
/* Whether to include FPU context */
/* #define FLOAT_CONTEXT */
/* Whether to inject fault intentionally */
/* #define FAULT_INJECT */

/* Data definitions */
#define RMP_TIME        ((rmp_tim_t)(End-Start))
#define RMP_TOTAL()     (Total+=(Diff))
#define RMP_MAX()       (Max=((Diff)>(Max)?(Diff):(Max)))
#define RMP_MIN()       (Min=((Diff)<(Min)?(Diff):(Min)))

/* Data initialization */
#define RMP_INIT() \
do \
{ \
    Total=0U; \
    Max=0U; \
    Min=((rmp_tim_t)-1U); \
} \
while(0)

/* Data extraction */
#define RMP_DATA() \
do \
{ \
    Diff=RMP_TIME; \
    RMP_TOTAL(); \
    RMP_MAX(); \
    RMP_MIN(); \
} \
while(0)
    
/* Data printing */
#define RMP_LIST(X) \
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
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
volatile rmp_ptr_t Flip=0U;
volatile rmp_tim_t Start=0U;
volatile rmp_tim_t End=0U;
volatile rmp_tim_t Diff=0U;
volatile rmp_tim_t Min=0U;
volatile rmp_tim_t Max=0U;
volatile rmp_u32_t Overflow=0U;
#ifdef PTR_16_BIT
volatile rmp_u32_t Total=0U;
volatile rmp_u32_t Mail_ISR_Total=0U;
volatile rmp_u32_t Sem_ISR_Total=0U;
volatile rmp_u32_t Msgq_ISR_Total=0U;
volatile rmp_u32_t Bmq_ISR_Total=0U;
#else
volatile rmp_ptr_t Total=0U;
volatile rmp_ptr_t Mail_ISR_Total=0U;
volatile rmp_ptr_t Sem_ISR_Total=0U;
volatile rmp_ptr_t Msgq_ISR_Total=0U;
volatile rmp_ptr_t Bmq_ISR_Total=0U;
#endif
#ifdef RVM_MAGIC_VIRTUAL
volatile rmp_ptr_t Guest_ISR_Total=0U;
volatile rmp_tim_t Guest_ISR_Max=0U;
volatile rmp_tim_t Guest_ISR_Min=0U;
#endif
volatile rmp_tim_t Mail_ISR_Max=0U;
volatile rmp_tim_t Mail_ISR_Min=0U;
volatile rmp_tim_t Sem_ISR_Max=0U;
volatile rmp_tim_t Sem_ISR_Min=0U;
volatile rmp_tim_t Msgq_ISR_Max=0U;
volatile rmp_tim_t Msgq_ISR_Min=0U;
volatile rmp_tim_t Bmq_ISR_Max=0U;
volatile rmp_tim_t Bmq_ISR_Min=0U;
volatile rmp_ptr_t Alrm_1_Cnt=0U;
volatile rmp_ptr_t Alrm_2_Cnt=0U;
volatile rmp_ptr_t Alrm_3_Cnt=0U;
volatile rmp_ptr_t Alrm_5_Cnt=0U;
volatile rmp_ptr_t Alrm_7_Cnt=0U;
/* Kernel object */
volatile struct RMP_Thd Thd_1;
volatile struct RMP_Thd Thd_2;
volatile struct RMP_Sem Sem_1;
/* Extended kernel object */
volatile struct RMP_Fifo Fifo_1;
volatile struct RMP_Msgq Msgq_1;
volatile struct RMP_Bmq Bmq_1;
volatile struct RMP_Amgr Amgr;
volatile struct RMP_Alrm Alrm_1;
volatile struct RMP_Alrm Alrm_2;
volatile struct RMP_Alrm Alrm_3;
volatile struct RMP_Alrm Alrm_5;
volatile struct RMP_Alrm Alrm_7;
/* Memory pool */
#ifdef TEST_MEM_POOL
/* In case some stupid toolchain emits this into data section */
#ifdef TEST_MEM_POOL_NOINIT
volatile rmp_ptr_t Pool[TEST_MEM_POOL];
#else
volatile rmp_ptr_t Pool[TEST_MEM_POOL]={0U};
#endif
#endif
/* Floating point context */
#ifdef FLOAT_CONTEXT
volatile float Float=0.0f;
#endif

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

#ifdef TEST_MEM_POOL
rmp_ptr_t Rand(void);
void Swap(rmp_ptr_t* Arg1,
          rmp_ptr_t* Arg2);
void Test_Mem_Pool(void);
#endif
void Test_Alrm_Hook(volatile struct RMP_Amgr* Amgr,
                    volatile struct RMP_Alrm* Alrm,
                    rmp_cnt_t Overdue);
void Test_Alrm(void);

void Test_Mail_ISR(void);
void Test_Sem_ISR(void);
void Test_Msgq_ISR(void);
void Test_Bmq_ISR(void);

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
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        /* Test flip result */
        if(Flip!=0U)
        {
            RMP_DBG_S("Yield error in Thd1 @ round ");
            RMP_DBG_I(Count);
            RMP_DBG_S(".\r\n");
        }
        Flip=1U;
        /* Read counter here */
        Start=RMP_CNT_READ();
        RMP_Thd_Yield();
    }
}

void Test_Mail_1(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        /* Read counter here */
        Start=RMP_CNT_READ();
        RMP_Thd_Snd(&Thd_2,1U,RMP_SLICE_MAX);
    }
}

void Test_Sem_1(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        /* Read counter here */
        Start=RMP_CNT_READ();
        RMP_Sem_Post(&Sem_1,1U);
    }
}

void Test_Msgq_1(void)
{
    rmp_cnt_t Count;
    struct RMP_List Node;
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        /* Read counter here */
        Start=RMP_CNT_READ();
        RMP_Msgq_Snd(&Msgq_1,&Node);
    }
}

void Test_Bmq_1(void)
{
    rmp_cnt_t Count;
    struct RMP_List Node;
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        /* Read counter here */
        Start=RMP_CNT_READ();
        RMP_Bmq_Snd(&Bmq_1,&Node,RMP_SLICE_MAX);
    }
}

void Func_1(void)
{
#ifdef FLOAT_CONTEXT
    Float+=1.0f;
#endif

    Test_Yield_1();
    /* Change priority of thread 2 */
    RMP_Thd_Set(&Thd_2,2U,RMP_SLICE_MAX);
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
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        RMP_Thd_Yield();
        /* Read counter here */
        End=RMP_CNT_READ();
        RMP_DATA();
        /* Test flip result */
        if(Flip==0U)
        {
            RMP_DBG_S("Yield error in Thd2 @ round ");
            RMP_DBG_I(Count);
            RMP_DBG_S(", ");
            RMP_DBG_I(Start);
            RMP_DBG_S(", ");
            RMP_DBG_I(End);
            RMP_DBG_S(".\r\n");
        }
        Flip=0U;
    }
}

void Test_Mail_2(void)
{
    rmp_ptr_t Data;
    rmp_cnt_t Count;
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        RMP_Thd_Rcv(&Data,RMP_SLICE_MAX);
        /* Read counter here */
        End=RMP_CNT_READ();
        RMP_DATA();
    }
}

void Test_Sem_2(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        RMP_Sem_Pend(&Sem_1,RMP_SLICE_MAX);
        /* Read counter here */
        End=RMP_CNT_READ();
        RMP_DATA();
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
        Start=RMP_CNT_READ();
        RMP_Fifo_Write(&Fifo_1,&Node);
        RMP_Fifo_Read(&Fifo_1,&Receive);
        End=RMP_CNT_READ();
        RMP_DATA();
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
        RMP_Msgq_Rcv(&Msgq_1,&Receive,RMP_SLICE_MAX);
        /* Read counter here */
        End=RMP_CNT_READ();
        RMP_DATA();
    }
}

void Test_Bmq_2(void)
{
    rmp_cnt_t Count;
    volatile struct RMP_List* Receive;
    
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        RMP_Bmq_Rcv(&Bmq_1,&Receive,RMP_SLICE_MAX);
        /* Read counter here */
        End=RMP_CNT_READ();
        RMP_DATA();
    }
}

void Test_Alrm_Hook(volatile struct RMP_Amgr* This_Amgr,
                    volatile struct RMP_Alrm* This_Alrm,
                    rmp_cnt_t Overdue)
{
    RMP_USE(This_Amgr);
    RMP_ASSERT(Overdue==0);

    switch(This_Alrm->Delay)
    {
        case 1U:Alrm_1_Cnt++;break;
        case 2U:Alrm_2_Cnt++;break;
        case 3U:Alrm_3_Cnt++;break;
        case 5U:Alrm_5_Cnt++;break;
        case 7U:Alrm_7_Cnt++;break;
        default:RMP_ASSERT(0);
    }
}

void Test_Alrm(void)
{
    rmp_cnt_t Count;
    
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        Start=RMP_CNT_READ();
        RMP_Amgr_Proc(&Amgr,1U);
        /* Read counter here */
        End=RMP_CNT_READ();
        RMP_DATA();
    }
    
    /* The callbacks must be triggered for a fixed number of times */
    RMP_ASSERT(Alrm_1_Cnt==ROUND_NUM);
    RMP_ASSERT(Alrm_2_Cnt==(ROUND_NUM/2U));
    RMP_ASSERT(Alrm_3_Cnt==(ROUND_NUM/3U));
    RMP_ASSERT(Alrm_5_Cnt==(ROUND_NUM/5U));
    RMP_ASSERT(Alrm_7_Cnt==(ROUND_NUM/7U));
}

void Test_Mail_ISR(void)
{
    rmp_ptr_t Data;
    static rmp_cnt_t Count;
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        RMP_Thd_Rcv(&Data,RMP_SLICE_MAX);
        /* Read counter here */
        End=RMP_CNT_READ();
        RMP_DATA();
        Flip=0U;
    }
}

void Test_Sem_ISR(void)
{
    static rmp_cnt_t Count;
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        RMP_Sem_Pend(&Sem_1,RMP_SLICE_MAX);
        /* Read counter here */
        End=RMP_CNT_READ();
        RMP_DATA();
        Flip=0U;
    }
}

void Test_Msgq_ISR(void)
{
    static rmp_cnt_t Count;
    volatile struct RMP_List* Receive;
    
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        RMP_Msgq_Rcv(&Msgq_1,&Receive,RMP_SLICE_MAX);
        /* Read counter here */
        End=RMP_CNT_READ();
        RMP_DATA();
        Flip=0U;
    }
}

void Test_Bmq_ISR(void)
{
    static rmp_cnt_t Count;
    volatile struct RMP_List* Receive;
    
    for(Count=0;Count<ROUND_NUM;Count++)
    {
        RMP_Bmq_Rcv(&Bmq_1,&Receive,RMP_SLICE_MAX);
        /* Read counter here */
        End=RMP_CNT_READ();
        RMP_DATA();
        Flip=0U;
    }
}

#ifdef TEST_MEM_POOL
rmp_ptr_t Rand(void)
{
    static rmp_ptr_t LFSR=0xACE1U;
    
    if((LFSR&0x01U)!=0U)
    {
        LFSR>>=1;
        LFSR^=0xB400U;
    }
    else
        LFSR>>=1;
    
    return LFSR;
}

void Swap(rmp_ptr_t* Arg1,
          rmp_ptr_t* Arg2)
{
    rmp_ptr_t Temp;
    
    Temp=*Arg1;
    *Arg1=*Arg2;
    *Arg2=Temp;
}

void Test_Mem_Pool(void)
{
    static void* Mem[8];
    static rmp_ptr_t Alloc[8];
    static rmp_ptr_t Free[8];
    static rmp_ptr_t Size[8];
    static rmp_ptr_t Amount[8];
    rmp_ptr_t Pool_Addr;
    rmp_ptr_t* Pool_Align;
    rmp_cnt_t Case_Cnt;
    rmp_cnt_t Test_Cnt;
    
    Amount[0]=(TEST_MEM_POOL/32U)*sizeof(rmp_ptr_t);
    Amount[1]=(TEST_MEM_POOL/64U+16U)*sizeof(rmp_ptr_t);
    Amount[2]=(TEST_MEM_POOL/4U)*sizeof(rmp_ptr_t);
    Amount[3]=(TEST_MEM_POOL/128U+32U)*sizeof(rmp_ptr_t);
    Amount[4]=(TEST_MEM_POOL/16U)*sizeof(rmp_ptr_t);
    Amount[5]=(TEST_MEM_POOL/8U+16U)*sizeof(rmp_ptr_t);
    Amount[6]=(TEST_MEM_POOL/128U+64U)*sizeof(rmp_ptr_t);
    Amount[7]=(TEST_MEM_POOL/2U-256U)*sizeof(rmp_ptr_t);
    
    /* Round up the pool to machine word boundary just in case toolchains don't */
    Pool_Addr=(rmp_ptr_t)Pool;
    Pool_Addr=(Pool_Addr+sizeof(rmp_ptr_t)-1U)/sizeof(rmp_ptr_t)*sizeof(rmp_ptr_t);
    Pool_Align=(rmp_ptr_t*)Pool_Addr;

    /* Initialize the pool */
    RMP_ASSERT(RMP_Mem_Init(Pool_Align,(TEST_MEM_POOL-1U)*sizeof(rmp_ptr_t))==0);
    for(Test_Cnt=0;Test_Cnt<ROUND_NUM;Test_Cnt++)
    {
        /* Random sequence and number generation */
        for(Case_Cnt=0;Case_Cnt<8;Case_Cnt++)
        {
            Alloc[Case_Cnt]=(rmp_u8_t)Case_Cnt;
            Free[Case_Cnt]=(rmp_u8_t)Case_Cnt;
            Size[Case_Cnt]=(rmp_u8_t)Case_Cnt;
        }
        
        for(Case_Cnt=7;Case_Cnt>0;Case_Cnt--)
        {
            Swap(&Alloc[Case_Cnt],&Alloc[Rand()%((rmp_ptr_t)Case_Cnt+1U)]);
            Swap(&Free[Case_Cnt],&Free[Rand()%((rmp_ptr_t)Case_Cnt+1U)]);
            Swap(&Size[Case_Cnt],&Size[Rand()%((rmp_ptr_t)Case_Cnt+1U)]);
        }
        
        Start=RMP_CNT_READ();
        /* Allocation tests - one of the mallocs may fail if because the management data
         * structure takes up some space. However, the first four must be successful. */
        Mem[Alloc[0]]=RMP_Malloc(Pool_Align,Amount[Size[0]]);
        RMP_ASSERT(Mem[Alloc[0]]!=RMP_NULL);
        Mem[Alloc[1]]=RMP_Malloc(Pool_Align,Amount[Size[1]]);
        RMP_ASSERT(Mem[Alloc[1]]!=RMP_NULL);
        Mem[Alloc[2]]=RMP_Malloc(Pool_Align,Amount[Size[2]]);
        RMP_ASSERT(Mem[Alloc[2]]!=RMP_NULL);
        Mem[Alloc[3]]=RMP_Malloc(Pool_Align,Amount[Size[3]]);
        RMP_ASSERT(Mem[Alloc[3]]!=RMP_NULL);
        Mem[Alloc[4]]=RMP_Malloc(Pool_Align,Amount[Size[4]]);
        Mem[Alloc[5]]=RMP_Malloc(Pool_Align,Amount[Size[5]]);
        Mem[Alloc[6]]=RMP_Malloc(Pool_Align,Amount[Size[6]]);
        Mem[Alloc[7]]=RMP_Malloc(Pool_Align,Amount[Size[7]]);

        /* Deallocation tests */
        RMP_Free(Pool_Align,Mem[Free[0]]);
        RMP_Free(Pool_Align,Mem[Free[1]]);
        RMP_Free(Pool_Align,Mem[Free[2]]);
        RMP_Free(Pool_Align,Mem[Free[3]]);
        RMP_Free(Pool_Align,Mem[Free[4]]);
        RMP_Free(Pool_Align,Mem[Free[5]]);
        RMP_Free(Pool_Align,Mem[Free[6]]);
        RMP_Free(Pool_Align,Mem[Free[7]]);
        End=RMP_CNT_READ();
        RMP_DATA();

        /* This should always be successful because we deallocated everything else, and
         * management data structure should never take up more than 1/8 of the pool. */
        Mem[0]=RMP_Malloc(Pool_Align,TEST_MEM_POOL*sizeof(rmp_ptr_t)*7U/8U);
        if(Mem[0]==RMP_NULL)
        {
            RMP_DBG_S("Memory test failure: ");
            RMP_DBG_I(Test_Cnt);
            RMP_DBG_S(" runs.\r\n");
            while(1);
        }
        RMP_Free(Pool_Align,Mem[0]); 
    }
    
    Total/=8U;
    Max/=8U;
    Min/=8U;
}
#endif

void Func_2(void)
{
    /* Print table header */
    RMP_DBG_S("\r\n");
#ifndef SMALL_TERMINAL
    /* Standard display assumes a standard 52/80-character terminal */
    RMP_DBG_S("    ___   __  ___ ___\r\n");
    RMP_DBG_S("   / _ \\ /  |/  // _ \\       Simple real-time kernel\r\n");
    RMP_DBG_S("  / , _// /|_/ // ___/       Standard benchmark test\r\n");
    RMP_DBG_S(" /_/|_|/_/  /_//_/\r\n");
    RMP_DBG_S("====================================================\r\n");
    RMP_DBG_S("Test (number in CPU cycles)        : AVG / MAX / MIN\r\n");
#else
    /* Miniature display assumes 16 characters per line, and guarantees AVG value display */
    RMP_DBG_S("=RMP= RT kernel\r\n");
    RMP_DBG_S(" Std benchmark\r\n");
    RMP_DBG_S("================\r\n");
    RMP_DBG_S("CY : A / TP / BM\r\n");
#endif

#ifdef FAULT_INJECT
    RMP_DBG_S("Injecting fault by accessing NULL address.\r\n");
    *((volatile rmp_ptr_t*)0U)=0U;
#endif
    
#ifdef FLOAT_CONTEXT
    Float+=1.0f;
#endif
    
    /* Yield tests */
    RMP_INIT();
    Test_Yield_2();
#ifndef SMALL_TERMINAL
    RMP_LIST("Yield                             ");
#else
    RMP_LIST("Y ");
#endif

    /* Elevate priority of thread 2 for IPC tests */
    RMP_Thd_Set(&Thd_2,2U,RMP_SLICE_MAX);
    
    /* Mailbox tests */
    RMP_INIT();
    Test_Mail_2();
#ifndef SMALL_TERMINAL
    RMP_LIST("Mailbox                           ");
#else
    RMP_LIST("M ");
#endif
    
    /* Semaphore tests */
    RMP_INIT();
    Test_Sem_2();
#ifndef SMALL_TERMINAL
    RMP_LIST("Semaphore                         ");
#else
    RMP_LIST("S ");
#endif
    
    /* Fifo tests */
    RMP_INIT();
    Test_Fifo();
#ifndef SMALL_TERMINAL
    RMP_LIST("FIFO                              ");
#else
    RMP_LIST("F ");
#endif
    
    /* Message queue tests */
    RMP_INIT();
    Test_Msgq_2();
#ifndef SMALL_TERMINAL
    RMP_LIST("Message queue                     ");
#else
    RMP_LIST("Q ");
#endif
    
    /* Blocking message queue tests */
    RMP_INIT();
    Test_Bmq_2();
#ifndef SMALL_TERMINAL
    RMP_LIST("Blocking message queue            ");
#else
    RMP_LIST("B ");
#endif

    /* Alarm tests */
    RMP_INIT();
    Test_Alrm();
#ifndef SMALL_TERMINAL
    RMP_LIST("Alarm combination (1/2/3/5/7)     ");
#else
    RMP_LIST("A ");
#endif
    
    /* Memory pool tests */
#ifdef TEST_MEM_POOL
    RMP_INIT();
    Test_Mem_Pool();
#ifndef SMALL_TERMINAL
    RMP_LIST("Memory allocation/free pair       ");
#else
    RMP_LIST("MM");
#endif
#endif

    /* Clear RVM test variable if there is */
#ifdef RVM_MAGIC_VIRTUAL
    Guest_ISR_Min=((rmp_tim_t)-1U);
#endif

    /* Prepare interrupt tests */
    Int_Init();
    
    /* Mailbox from interrupt tests */
    RMP_INIT();
    Test_Mail_ISR();
    Mail_ISR_Total=Total;
    Mail_ISR_Max=Max;
    Mail_ISR_Min=Min;
    
    /* Semaphore from interrupt tests */
    RMP_INIT();
    Test_Sem_ISR();
    Sem_ISR_Total=Total;
    Sem_ISR_Max=Max;
    Sem_ISR_Min=Min;
    
    /* Message queue from interrupt tests */
    RMP_INIT();
    Test_Msgq_ISR();
    Msgq_ISR_Total=Total;
    Msgq_ISR_Max=Max;
    Msgq_ISR_Min=Min;
    
    /* Blocking message queue from interrupt tests */
    RMP_INIT();
    Test_Bmq_ISR();
    Bmq_ISR_Total=Total;
    Bmq_ISR_Max=Max;
    Bmq_ISR_Min=Min;
    
    /* Print results */
    Total=Mail_ISR_Total;
    Max=Mail_ISR_Max;
    Min=Mail_ISR_Min;
#ifndef SMALL_TERMINAL
    RMP_LIST("ISR Mailbox                       ");
#else
    RMP_LIST("MI");
#endif
    
    Total=Sem_ISR_Total;
    Max=Sem_ISR_Max;
    Min=Sem_ISR_Min;
#ifndef SMALL_TERMINAL
    RMP_LIST("ISR Semaphore                     ");
#else
    RMP_LIST("SI");
#endif
    
    Total=Msgq_ISR_Total;
    Max=Msgq_ISR_Max;
    Min=Msgq_ISR_Min;
#ifndef SMALL_TERMINAL
    RMP_LIST("ISR Message queue                 ");
#else
    RMP_LIST("QI");
#endif
    
    Total=Bmq_ISR_Total;
    Max=Bmq_ISR_Max;
    Min=Bmq_ISR_Min;
#ifndef SMALL_TERMINAL
    RMP_LIST("ISR Blocking message queue        ");
#else
    RMP_LIST("BI");
#endif

    /* RVM needs to divide by 4 due to 4x accumulation */
#ifdef RVM_MAGIC_VIRTUAL
    Total=Guest_ISR_Total/4;
    Max=Guest_ISR_Max;
    Min=Guest_ISR_Min;
#ifndef SMALL_TERMINAL
    RMP_LIST("ISR RVM activation relay          ");
#else
    RMP_LIST("RI");
#endif
#endif
    
    /* Test stop - Decide whether to exit, or keep dumping counter values
     * to detect potentially wrong timer clock rate configurations */
#ifdef TEST_EXIT
    Test_Exit();
#else
    while(1)
    {
        Start=End;
        End=(rmp_u16_t)RMP_CNT_READ();
        if(Start>End)
        {
            Overflow++;
            if((Overflow%OVERFLOW_NUM)==0U)
            {
                RMP_DBG_I(OVERFLOW_NUM);
                RMP_DBG_S(" overflows\r\n");
            }
        }
    }
#endif
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
    rmp_ret_t Retval;
    static rmp_ptr_t Count=0U;
    static struct RMP_List Node;
    
    /* Deal with RVM if there is; this won't interfere with RMP data
     * because the RMP timers are read just before its ISR sends */
#ifdef RVM_MAGIC_VIRTUAL
    rvm_tim_t Guest_Diff;
    
    RVM_DATA();
#endif
    
    /* Interrupt reentry - if this happens, increase interrupt interval */
    if(Flip!=0U)
    {
        RMP_DBG_S("Interrupt reentered.\r\n");
    }
    Flip=1U;
    
    /* Deal with the real benchmark */
    if(Count<ROUND_NUM)
    {
        Count++;
        Start=RMP_CNT_READ();
        Retval=RMP_Thd_Snd_ISR(&Thd_2,1U);
        if(Retval<0)
        {
            RMP_DBG_S("ISR Mailbox send failed with code ");
            RMP_DBG_I(Retval);
            RMP_DBG_S(": ");
            RMP_DBG_I(Count);
            RMP_DBG_S(" sends.\r\n");
        }
    }
    else if(Count<ROUND_NUM*2U)
    {
        Count++;
        Start=RMP_CNT_READ();
        Retval=RMP_Sem_Post_ISR(&Sem_1,1U);
        if(Retval<0)
        {
            RMP_DBG_S("ISR semaphore post failed with code ");
            RMP_DBG_I(Retval);
            RMP_DBG_S(": ");
            RMP_DBG_I(Count);
            RMP_DBG_S(" posts.\r\n");
        }
    }
    else if(Count<ROUND_NUM*3U)
    {
        Count++;
        Start=RMP_CNT_READ();
        Retval=RMP_Msgq_Snd_ISR(&Msgq_1,&Node);
        if(Retval<0)
        {
            RMP_DBG_S("ISR msgq message send failed with code ");
            RMP_DBG_I(Retval);
            RMP_DBG_S(": ");
            RMP_DBG_I(Count);
            RMP_DBG_S(" sends.\r\n");
        }
    }
    else if(Count<ROUND_NUM*4U)
    {
        Count++;
        Start=RMP_CNT_READ();
        Retval=RMP_Bmq_Snd_ISR(&Bmq_1,&Node);
        if(Retval<0)
        {
            RMP_DBG_S("ISR bmq message send failed with code ");
            RMP_DBG_I(Retval);
            RMP_DBG_S(": ");
            RMP_DBG_I(Count);
            RMP_DBG_S(" sends.\r\n");
        }
    }
    else
    {
        Retval=0;
        Int_Disable();
    }
    
    if(Retval<0)
    {
        RMP_DBG_S("Thd 2 state: 0x");
        RMP_DBG_H(Thd_2.State);
        RMP_DBG_S(".\r\n");
        while(1);
    }
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
    /* Initialize the performance monitoring timer */
    Timer_Init();
    
    /* Clean up the structures */
    RMP_Clear(&Thd_1,sizeof(struct RMP_Thd));
    RMP_Clear(&Thd_2,sizeof(struct RMP_Thd));
    RMP_Clear(&Sem_1,sizeof(struct RMP_Sem));
    RMP_Clear(&Fifo_1,sizeof(struct RMP_Fifo));
    RMP_Clear(&Msgq_1,sizeof(struct RMP_Msgq));
    RMP_Clear(&Bmq_1,sizeof(struct RMP_Bmq));
    RMP_Clear(&Amgr,sizeof(struct RMP_Amgr));
    RMP_Clear(&Alrm_1,sizeof(struct RMP_Alrm));
    RMP_Clear(&Alrm_2,sizeof(struct RMP_Alrm));
    RMP_Clear(&Alrm_3,sizeof(struct RMP_Alrm));
    RMP_Clear(&Alrm_5,sizeof(struct RMP_Alrm));
    RMP_Clear(&Alrm_7,sizeof(struct RMP_Alrm));
    
    /* Create kernel objects */
    RMP_Sem_Crt(&Sem_1,0U);
    RMP_Fifo_Crt(&Fifo_1);
    RMP_Msgq_Crt(&Msgq_1);
    RMP_Bmq_Crt(&Bmq_1,1U);
    RMP_Amgr_Crt(&Amgr);
    RMP_Alrm_Init(&Alrm_1,1U,RMP_ALRM_AUTORLD,Test_Alrm_Hook);
    RMP_Alrm_Init(&Alrm_2,2U,RMP_ALRM_AUTORLD,Test_Alrm_Hook);
    RMP_Alrm_Init(&Alrm_3,3U,RMP_ALRM_AUTORLD,Test_Alrm_Hook);
    RMP_Alrm_Init(&Alrm_5,5U,RMP_ALRM_AUTORLD,Test_Alrm_Hook);
    RMP_Alrm_Init(&Alrm_7,7U,RMP_ALRM_AUTORLD,Test_Alrm_Hook);
    RMP_Alrm_Set(&Amgr,&Alrm_1);
    RMP_Alrm_Set(&Amgr,&Alrm_2);
    RMP_Alrm_Set(&Amgr,&Alrm_3);
    RMP_Alrm_Set(&Amgr,&Alrm_5);
    RMP_Alrm_Set(&Amgr,&Alrm_7);
    
    /* Start threads - thread 2 is added at first so it will be scheduled first */
    RMP_Thd_Crt(&Thd_2,
                (void*)Func_2, (void*)0x4321U,
                Stack_2, sizeof(Stack_2),
                1U, 1000U);
    RMP_Thd_Crt(&Thd_1,
                (void*)Func_1, (void*)0x1234U,
                Stack_1, sizeof(Stack_1),
                1U, 1000U);
#endif
}

void RMP_Init_Idle(void)
{
    return;
}
/* End Function:RMP_Init *****************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
