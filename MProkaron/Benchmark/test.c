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
    
    /* Memory pool tests */
#ifdef TEST_MEM_POOL
    Test_Mem_Pool();
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
    RMP_Thd_Crt(&Thd_2, Func_2, THD2_STACK, (void*)0x4321, 1, 5);
#endif
}

volatile unsigned int count;
void RMP_Init_Idle(void)
{
    return;
}
/* End Function:RMP_Init *****************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
