/******************************************************************************
Filename    : sys_arch.c
Author      : hrs 
Date        : 04/09/2024
Licence     : The Unlicense; see LICENSE for details.
Description : Define LwIP porting related functions.
******************************************************************************/

/* Include *******************************************************************/
#include "arch/sys_arch.h"
#include "lwip/sys.h"
/* End Include ***************************************************************/

/* Variable ******************************************************************/
rmp_u32_t Sys_Pool[SYS_POOL_SIZE];
struct RMP_List Msg_List;
sys_mbox_msg_t Msg_Array[MSG_ARRAY_SIZE];
/* End Variable **************************************************************/

/* Function:sys_init **********************************************************
Description : Initialize message list and memory pool.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void sys_init(void)
{
    rmp_cnt_t Count;
    
    /* Initialize memory pool */
    RMP_Mem_Init(Sys_Pool,sizeof(Sys_Pool));
    
    /* Initialize message array */
    RMP_List_Crt(&Msg_List);
    for(Count=0;Count<MSG_ARRAY_SIZE;Count++)
        RMP_List_Ins(&(Msg_Array[Count].head),&Msg_List,Msg_List.Next);
}
/* End Function:sys_init *****************************************************/

/* Function:sys_rand **********************************************************
Description : LCG random number generator. Neither MT-safe nor crypto-secure.
Input       : None.
Output      : None.
Return      : rmp_u32_t - The random number generated.
******************************************************************************/
rmp_u32_t sys_rand(void)
{
    static rmp_u32_t Seed;
    
    Seed+=RMP_Timestamp;
    Seed=(1664525U*Seed+1013904223U)%4294967296U;
    
    return Seed;
}
/* End Function:sys_rand *****************************************************/

/* Function:sys_now ***********************************************************
Description : Get current system time.
Input       : None.
Output      : None.
Return      : u32_t - Current system ticks.
******************************************************************************/
u32_t sys_now(void)
{
    return RMP_Timestamp;
}
/* End Function:sys_now ******************************************************/

/* Function:sys_arch_protect **************************************************
Description : This function does a "fast" critical region protection. This 
              function is only called during very short critical regions.
Input       : None.
Output      : None.
Return      : sys_prot_t - Returns the previous protection level.
******************************************************************************/
sys_prot_t sys_arch_protect(void)
{
    RMP_Sched_Lock();
    return 1;
}
/* End Function:sys_arch_protect *********************************************/

/* Function:sys_arch_unprotect ************************************************
Description : This function does a "fast" set of critical region protection to 
              the value specified by pval.
Input       : sys_prot_t pval - Not used.
Output      : None.
Return      : None.
******************************************************************************/
void sys_arch_unprotect(sys_prot_t pval)
{
    /* Unused variable - dewarn */
    (void)pval;
    RMP_Sched_Unlock();
}
/* End Function:sys_arch_unprotect *******************************************/

/* Function:sys_sem_new *******************************************************
Description : Create a new semaphore.
Input       : sys_sem_t* sem - Pointer to the semaphore to create.
              u8_t count - Initial count of the semaphore.
Output      : None.
Return      : err_t - Status of execution, ERR_OK means successful.
******************************************************************************/
err_t sys_sem_new(sys_sem_t* sem,
                  u8_t count)
{
    err_t Ret;
    LWIP_ASSERT("Semaphore is NULL.",sem!=SYS_SEM_NULL);
    LWIP_ASSERT("Initial count is invalid (not 0 or 1).",(count==0)||(count==1));
    Ret=RMP_Sem_Crt(sem,count);
    
    if(Ret==0) 
        return ERR_OK;
    
    return Ret;
}
/* End Function:sys_sem_new **************************************************/

/* Function:sys_sem_free ******************************************************
Description : Deallocates a semaphore.
Input       : sys_sem_t* sem - Semaphore to be deleted.
Output      : None.
Return      : None.
******************************************************************************/
void sys_sem_free(sys_sem_t* sem)
{
    RMP_Sem_Del(sem);
}
/* End Function:sys_sem_free *************************************************/

/* Function:sys_arch_sem_wait *************************************************
Description : Blocks the thread while waiting for the semaphore to be signaled.
Input       : sys_sem_t* sem - The semaphore to wait for.
              u32_t Timeout - Timeout in milliseconds to wait.if Timeout==0
              means wait forever.
Output      : None.
Return      : u32_t - SYS_ARCH_TIMEOUT on timeout, any other value on success.
******************************************************************************/
u32_t sys_arch_sem_wait(sys_sem_t* sem,
                        u32_t Timeout)
{
    u32_t Ret;
    if(sem==SYS_SEM_NULL) 
    {
        RMP_DBG_S("Semaphore passed to sync_sem_wait is NULL");
        return SYS_ARCH_TIMEOUT;
    }
    
    if(Timeout==0)
        Timeout=RMP_SLICE_MAX;
    Ret=RMP_Sem_Pend(sem,(rmp_ptr_t)Timeout);
    
    if(Ret==RMP_ERR_OPER)
        return SYS_ARCH_TIMEOUT;
    
    return Ret;
}
/* End Function:sys_arch_sem_wait ********************************************/

/* Function:sys_sem_signal ****************************************************
Description : Post or release a semaphore.
Input       : sys_sem_t* sem - The semaphore to signal.
Output      : None.
Return      : None.
******************************************************************************/
void sys_sem_signal(sys_sem_t* sem)
{
    rmp_ret_t Ret;
    LWIP_ASSERT("Semaphore is not NULL.",sem!=SYS_SEM_NULL);
    Ret=RMP_Sem_Post(sem,1);
    LWIP_ASSERT("semaphore is released successfully",Ret==0);
}
/* End Function:sys_sem_signal ***********************************************/

/* Function:sys_sem_valid *****************************************************
Description : Check if a semaphore is valid.
Input       : sys_sem_t* sem - The semaphore to check.
Output      : None.
Return      : int - Returns 1 if the semaphore is valid, 0 if it is not valid.
******************************************************************************/
int sys_sem_valid(sys_sem_t* sem) 
{
    return sem->Wait_List.Prev!=RMP_NULL;
}
/* End Function:sys_sem_valid ************************************************/

/* Function:sys_sem_set_invalid ***********************************************
Description : Set a semaphore invalid.
Input       : sys_sem_t* sem - The semaphore to be set.
Output      : None.
Return      : None.
******************************************************************************/
void sys_sem_set_invalid(sys_sem_t* sem) 
{
    sem->Wait_List.Prev=RMP_NULL;
    sem->Wait_List.Next=RMP_NULL;
}
/* End Function:sys_sem_set_invalid ******************************************/

/* Function:sys_mbox_new ******************************************************
Description : Creates an empty mailbox for maximum "size" elements.
Input       : sys_mbox_t* mbox - pointer to the mbox to create.
              int size - (minimum) number of messages in this mbox.
Output      : None.
Return      : err_t - ERR_OK if successful, another err_t otherwise.
******************************************************************************/
err_t sys_mbox_new(sys_mbox_t* mbox,
                   int size)
{
    err_t Ret;
    LWIP_ASSERT("mailbox is NULL",mbox!=SYS_MBOX_NULL);
    Ret=RMP_Msgq_Crt(mbox);
    LWIP_ASSERT("Return value is RMP_ERR_MSGQ",Ret!=RMP_ERR_MSGQ);
    return Ret;
}
/* End Function:sys_mbox_new *************************************************/

/* Function:sys_mbox_post *****************************************************
Description : Post a message to an mbox - may not fail -> blocks if full, only 
              to be used from tasks NOT from ISR!
Input       : sys_mbox_t* mbox - mbox to posts the message.
              void* msg - message to post (ATTENTION: can be NULL).
Output      : None.
Return      : None.
******************************************************************************/
void sys_mbox_post(sys_mbox_t* mbox,
                   void* msg)
{
    volatile struct RMP_List* Head;

    RMP_Sched_Lock();
    Head=Msg_List.Next;
    RMP_List_Del(Head->Prev,Head->Next);
    RMP_Sched_Unlock();
    
    ((sys_mbox_msg_t*)Head)->msg=msg;
    
    RMP_Msgq_Snd(mbox,Head);
}
/* End Function:sys_mbox_post ************************************************/

/* Function:sys_mbox_trypost **************************************************
Description : Try to post a message to an mbox - may fail if full. Can be used 
              from ISR (if the sys arch layer allows this). 
Input       : sys_mbox_t* mbox - mbox to posts the message.
              void* msg - message to post (ATTENTION: can be NULL).
Output      : None.
Return      : err_t - Returns ERR_MEM if it is full, else, ERR_OK if the msg 
              is posted.
******************************************************************************/
err_t sys_mbox_trypost(sys_mbox_t* mbox,
                       void* msg)
{
    err_t Ret;
    volatile struct RMP_List* Head;
    
    RMP_Sched_Lock();
    Head=Msg_List.Next;
    RMP_List_Del(Head->Prev,Head->Next);
    RMP_Sched_Unlock();

    ((sys_mbox_msg_t*)Head)->msg=msg;
    Ret=RMP_Msgq_Snd(mbox,Head);
    if(Ret!=0)
    {
        RMP_Sched_Lock();
        RMP_List_Ins(Head,&Msg_List,Msg_List.Next);
        RMP_Sched_Unlock();
        return ERR_MEM;
    }
    
    return ERR_OK;
}
/* End Function:sys_mbox_trypost *********************************************/

/* Function:sys_mbox_trypost_fromisr ******************************************
Description : Try to post a message to an mbox - may fail if full. To be be 
              used from ISR.
Input       : sys_mbox_t* mbox - mbox to posts the message.
              void* msg - message to post (ATTENTION: can be NULL).
Output      : None.
Return      : err_t - Returns ERR_MEM if it is full, else, ERR_OK if the msg 
              is posted.
******************************************************************************/
err_t sys_mbox_trypost_fromisr(sys_mbox_t* mbox,
                               void* msg)
{
    err_t Ret;
    volatile struct RMP_List* Head;
    
    Head=Msg_List.Next;
    RMP_List_Del(Head->Prev,Head->Next);
    
    ((sys_mbox_msg_t*)Head)->msg=msg;

    Ret=RMP_Msgq_Snd_ISR(mbox,Head);
    if(Ret!=0)
    {
        RMP_List_Ins(Head,&Msg_List,Msg_List.Next);
        return ERR_MEM;
    }

    return ERR_OK;
}
/* End Function:sys_mbox_trypost_fromisr *************************************/

/* Function:sys_arch_mbox_fetch ***********************************************
Description : Blocks the thread until a message arrives in the mailbox, but 
              does not block the thread longer than "Timeout" milliseconds.
Input       : sys_mbox_t* mbox - mbox to get a message from.
              void** msg - pointer where the message is stored.
              u32_t timeout_ms - maximum time (in milliseconds) to wait for
              a message (0 = wait forever).
Output      : None.
Return      : u32_t - SYS_ARCH_TIMEOUT if there was a timeout, any other 
              value if a messages is received.
******************************************************************************/
u32_t sys_arch_mbox_fetch(sys_mbox_t* mbox,
                          void** msg,
                          u32_t timeout_ms)
{
    u32_t Timeout;
    rmp_ret_t Ret;
    volatile struct RMP_List* Head;

    if(timeout_ms==0)
        timeout_ms=RMP_SLICE_MAX;
    
    Ret=RMP_Msgq_Rcv(mbox,&Head,timeout_ms);
    if(Ret==0)
    {
        *msg=((sys_mbox_msg_t*)Head)->msg;
        ((sys_mbox_msg_t*)Head)->msg=RMP_NULL;
        RMP_List_Ins(Head,&Msg_List,Msg_List.Next);
        return Timeout;
    }
    else if(timeout_ms!=RMP_SLICE_MAX)
        return SYS_ARCH_TIMEOUT;
    
    return Ret;
}
/* End Function:sys_arch_mbox_fetch ******************************************/

/* Function:sys_arch_mbox_tryfetch ********************************************
Description : Blocks the thread until a message arrives in the mailbox, but 
              does not block the thread longer than "Timeout" milliseconds.
Input       : sys_mbox_t* mbox - mbox to get a message from.
              void** msg - pointer where the message is stored.
Output      : None.
Return      : u32_t - SYS_ARCH_TIMEOUT if there was a Timeout, any other 
              value if a messages is received.
******************************************************************************/
u32_t sys_arch_mbox_tryfetch(sys_mbox_t* mbox,
                             void** msg)
{
    return sys_arch_mbox_fetch(mbox,msg,0);
}
/* End Function:sys_arch_mbox_tryfetch ***************************************/

/* Function:sys_mbox_free *****************************************************
Description : Deallocates a mailbox.
Input       : sys_mbox_t* mbox - mbox to delete.
Output      : None.
Return      : None.
******************************************************************************/
void sys_mbox_free(sys_mbox_t* mbox)
{
    LWIP_ASSERT("Mailbox is not empty.",RMP_Msgq_Cnt(mbox)==0);
    RMP_Msgq_Del(mbox);
}
/* End Function:sys_mbox_free ************************************************/

/* Function:sys_mbox_valid ****************************************************
Description : Check if given mailbox valid.
Input       : sys_mbox_t* mbox - mbox to delete.
Output      : None.
Return      : int - Returns 1 if the mailbox is valid, 0 not valid.
******************************************************************************/
int sys_mbox_valid(sys_mbox_t* mbox)
{
    return mbox->State;
}
/* End Function:sys_mbox_valid ***********************************************/

/* Function:sys_mbox_set_invalid **********************************************
Description : Invalidate a mailbox so that sys_mbox_valid() returns 0.
Input       : sys_mbox_t* mbox - mbox to set.
Output      : None.
Return      : None.
******************************************************************************/
void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
    mbox->State=RMP_MSGQ_FREE;
}
/* End Function:sys_mbox_set_invalid *****************************************/

/* Function:sys_thread_new ****************************************************
Description : Starts a new thread.
Input       : const char* name - human-readable name for the thread.
              lwip_thread_fn thread - thread-function.
              void* arg - parameter passed to thread.
              int stacksize - stack size in bytes for the new thread.
              int prio - priority of the new thread.
Output      : None.
Return      : sys_thread_t - return the id of the new thread.
******************************************************************************/
sys_thread_t sys_thread_new(const char* name,
                            lwip_thread_fn thread,
                            void* arg,
                            int stacksize,
                            int prio)
{
    sys_thread_t Thd;
    void* Stack;
    
    /* Allocate stack */
    Stack=RMP_Malloc(Sys_Pool,stacksize);
    if(Stack==RMP_NULL)
        RMP_ASSERT(0);
    
    /* Allocate thread */
    Thd=RMP_Malloc(Sys_Pool,sizeof(struct RMP_Thd));
    if(Thd==RMP_NULL)
    {
        RMP_Free(Sys_Pool,Stack);
        RMP_ASSERT(0);
    }
    
    if(RMP_Thd_Crt(Thd,thread,arg,Stack,stacksize,prio,1)!=0)
    {
        RMP_Free(Sys_Pool,Stack);
        RMP_Free(Sys_Pool,Thd);
        RMP_ASSERT(0);
    }
    
    return Thd;
}
/* End Function:sys_thread_new ***********************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
