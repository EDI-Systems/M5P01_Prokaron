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
tim_t Start=0;
tim_t End=0;
ptr_t Total=0;
ptr_t Temp=0;
/* Test results also written here */
volatile ptr_t Yield_Time=0;
volatile ptr_t Mailbox_Time=0;
volatile ptr_t Semaphore_Time=0;
volatile ptr_t Mailbox_ISR_Time=0;
volatile ptr_t Semaphore_ISR_Time=0;
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
    cnt_t Count;
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
            while(1);
    }
    else if(Count<20000)
    {
        Count++;
        Start=COUNTER_READ();
        if(RMP_Sem_Post_ISR(&Sem_1, 1)<0)
            while(1);
    }
    else
        Int_Disable();
}
#endif
/* End Function:Int_Handler **************************************************/

/* Begin Function:RMP_Init ****************************************************
Description : The init thread ook functions.
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
    RMP_Thd_Crt(&Thd_1, Func_1, &Stack_1[256], (void*)0x12345678, 1, 5);
    RMP_Thd_Crt(&Thd_2, Func_2, &Stack_2[256], (void*)0x87654321, 1, 5);
#endif
}

void RMP_Init_Idle(void)
{
    return;
}
/* End Function:RMP_Init *****************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
