/******************************************************************************
Filename    : kernel.c
Author      : pry
Date        : 04/02/2018
Licence     : LGPL v3+; see COPYING for details.
Description : The RMP RTOS single-file kernel.
              This is a single-process kernel that does nothing but simple thread
              context switches. This operating system's kernel object allocation
              policy is totally exported, so the user assume full control over this.
******************************************************************************/

/* Includes ******************************************************************/
#define __HDR_DEFS__
#include "Platform/RMP_platform.h"
#include "Kernel/kernel.h"
#undef __HDR_DEFS__

#define __HDR_STRUCTS__
#include "Platform/RMP_platform.h"
#include "Kernel/kernel.h"
#undef __HDR_STRUCTS__

/* Private include */
#include "Kernel/kernel.h"

#define __HDR_PUBLIC_MEMBERS__
#include "Platform/RMP_platform.h"
#undef __HDR_PUBLIC_MEMBERS__
/* End Includes **************************************************************/

/* Begin Function:RMP_Clear ***************************************************
Description : Memset a memory area to zero.
Input       : volatile void* Addr - The address to clear.
              ptr_t Size - The size to clear.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Clear(volatile void* Addr, ptr_t Size)
{
    ptr_t* Word_Inc;
    u8* Byte_Inc;
    ptr_t Words;
    ptr_t Bytes;
    
    /* On processors not that fast, copy by word is really important */
    Word_Inc=(ptr_t*)Addr;
    for(Words=Size/sizeof(ptr_t);Words>0;Words--)
    {
        *Word_Inc=0;
        Word_Inc++;
    }
    
    /* Get the final bytes */
    Byte_Inc=(u8*)Word_Inc;
    for(Bytes=Size%sizeof(ptr_t);Bytes>0;Bytes--)
    {
        *Byte_Inc=0;
        Byte_Inc++;
    }
}
/* End Function:RMP_Clear ****************************************************/

/* Begin Function:RMP_Print_Int ***********************************************
Description : Print a signed integer on the debugging console. This integer is
              printed as decimal with sign.
Input       : cnt_t Int - The integer to print.
Output      : None.
Return      : cnt_t - The length of the string printed.
******************************************************************************/
cnt_t RMP_Print_Int(cnt_t Int)
{
    ptr_t Iter;
    cnt_t Count;
    cnt_t Num;
    ptr_t Div;
    
    /* how many digits are there? */
    if(Int==0)
    {
        RMP_Putchar('0');
        return 1;
    }
    else if(Int<0)
    {
        /* How many digits are there? */
        Count=0;
        Div=1;
        Iter=-Int;
        while(Iter!=0)
        {
            Iter/=10;
            Count++;
            Div*=10;
        }
        Div/=10;
        
        RMP_Putchar('-');
        Iter=-Int;
        Num=Count+1;
        
        while(Count>0)
        {
            Count--;
            RMP_Putchar(Iter/Div+'0');
            Iter=Iter%Div;
            Div/=10;
        }
    }
    else
    {
        /* How many digits are there? */
        Count=0;
        Div=1;
        Iter=Int;
        while(Iter!=0)
        {
            Iter/=10;
            Count++;
            Div*=10;
        }
        Div/=10;
        
        Iter=Int;
        Num=Count;
        
        while(Count>0)
        {
            Count--;
            RMP_Putchar(Iter/Div+'0');
            Iter=Iter%Div;
            Div/=10;
        }
    }
    
    return Num;
}
/* End Function:RMP_Print_Int ************************************************/

/* Begin Function:RMP_Print_Uint **********************************************
Description : Print a unsigned integer on the debugging console. This integer is
              printed as hexadecimal.
Input       : ptr_t Uint - The unsigned integer to print.
Output      : None.
Return      : cnt_t - The length of the string printed.
******************************************************************************/
cnt_t RMP_Print_Uint(ptr_t Uint)
{
    ptr_t Iter;
    cnt_t Count;
    cnt_t Num;
    
    /* how many digits are there? */
    if(Uint==0)
    {
        RMP_Putchar('0');
        return 1;
    }
    else
    {
        /* Filter out all the zeroes */
        Count=0;
        Iter=Uint;
        while((Iter>>((sizeof(ptr_t)*8)-4))==0)
        {
            Iter<<=4;
            Count++;
        }
        /* Count is the number of pts to print */
        Count=sizeof(ptr_t)*2-Count;
        Num=Count;
        while(Count>0)
        {
            Count--;
            Iter=(Uint>>(Count*4))&0x0F;
            if(Iter<10)
                RMP_Putchar('0'+Iter);
            else
                RMP_Putchar('A'+Iter-10);
        }
    }
    
    return Num;
}
/* End Function:RMP_Print_Uint ***********************************************/

/* Begin Function:RMP_Print_String ********************************************
Description : Print a string the kernel console.
              This is only used for kernel-level debugging.
Input       : s8* String - The string to print
Output      : None.
Return      : cnt_t - The length of the string printed, the '\0' is not included.
******************************************************************************/
cnt_t RMP_Print_String(s8* String)
{
    cnt_t Count;
    
    Count=0;
    while(Count<RMP_KERNEL_DEBUG_MAX_STR)
    {
        if(String[Count]=='\0')
            break;
        
        RMP_Putchar(String[Count++]);
    }
    
    return Count;
}
/* End Function:RMP_Print_String *********************************************/

/* Begin Function:RMP_List_Crt ************************************************
Description : Create a doubly linkled list.
Input       : volatile struct RMP_List* Head - The pointer to the list head.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
    Head->Next=(struct RMP_List*)Head;
}
/* End Function:RMP_List_Crt *************************************************/

/* Begin Function:RMP_List_Del ************************************************
Description : Delete a node from the doubly-linked list.
Input       : volatile struct RMP_List* Prev - The prevoius node of the target node.
              volatile struct RMP_List* Next - The next node of the target node.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
    Prev->Next=(struct RMP_List*)Next;
}
/* End Function:RMP_List_Del *************************************************/

/* Begin Function:RMP_List_Ins ************************************************
Description : Insert a node to the doubly-linked list.
Input       : volatile struct RMP_List* New - The new node to insert.
              volatile struct RMP_List* Prev - The previous node.
              volatile struct RMP_List* Next - The next node.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
    New->Next=(struct RMP_List*)Next;
    New->Prev=(struct RMP_List*)Prev;
    Prev->Next=(struct RMP_List*)New;
}
/* End Function:RMP_List_Ins *************************************************/

/* Begin Function:RMP_Lock_Sched **********************************************
Description : The function locks the scheduler. The locking can be stacked.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
    RMP_Sched_Locked=1;
    RMP_Sched_Lock_Cnt++;
}
/* End Function:RMP_Lock_Sched ***********************************************/

/* Begin Function:RMP_Unlock_Sched ********************************************
Description : The function unlocks the scheduler. The unlocking can be stacked.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
        {
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
            _RMP_Yield();
        }
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
        RMP_Sched_Lock_Cnt--;
    /* Trying to unlock a scheduler that is not locked */
    else
        while(1);
}
/* End Function:RMP_Unlock_Sched *********************************************/

/* Begin Function:RMP_Yield ***************************************************
Description : Yield to another thread.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Yield(void)
{
    if(RMP_Sched_Locked==0)
    {
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        _RMP_Yield();
    }
    else
        RMP_Sched_Pend=1;
}
/* End Function:RMP_Yield ****************************************************/

/* Begin Function:_RMP_Timer_Proc *********************************************
Description : Process RMP timer events.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Timer_Proc(void)
{
    struct RMP_Thd* Thread;
    
    /* Process the timer events, if there are any of them */
    while((&RMP_Delay)!=RMP_Delay.Next)
    {
        Thread=RMP_DLY2THD(RMP_Delay.Next);
        /* If the value is more than this, then it means that the time have
         * already passed and we have to process this */
        if((RMP_Tick-Thread->Timeout)>(RMP_ALLBITS>>1))
            break;
        
        /* This thread should be processed */
        RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
        switch(RMP_THD_STATE(Thread->State))
        {
            case RMP_THD_SNDDLY:
            case RMP_THD_SEMDLY:
            {
                RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
                Thread->Retval=RMP_ERR_OPER;
                break;
            }
            
            case RMP_THD_RCVDLY:
            {
                Thread->Retval=RMP_ERR_OPER;
                break;
            }
            
            case RMP_THD_DELAYED:break;
            /* Should not get here */
            default:break;
        }

        RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
        /* Is it suspended? If yes, we can't directly set it running */
        if((Thread->State&RMP_THD_SUSPENDED)==0)
        {
            /* Insert this into the corresponding runqueue */
            RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
            /* Set this runlevel as active */
            RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=1<<(Thread->Prio&RMP_WORD_MASK);
            /* See if we need to pend a scheduling event */
            if(Thread->Prio>RMP_Cur_Thd->Prio)
                RMP_Sched_Pend=1;
        }
    }
}
/* Begin Function:_RMP_Timer_Proc ********************************************/

/* Begin Function:_RMP_Get_High_Rdy *******************************************
Description : Get the highest priority ready thread. The return value will be written
              into the global variables.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Get_High_Rdy(void)
{
    cnt_t Count;
    
    /* Write the SP value to thread structure */
    RMP_Cur_Thd->Stack=RMP_Cur_SP;
    
    /* Is the scheduler locked on other threads? If yes, we return without doing anything */
    if(RMP_Sched_Locked!=0)
        return;
    
    if(RMP_Timer_Pend==1)
    {
        RMP_Timer_Pend=0;
        _RMP_Timer_Proc();
    }
    
    RMP_Sched_Pend=0;
    /* See which one is ready, and pick it */
    for(Count=RMP_BITMAP_SIZE-1;Count>=0;Count--)
    {
        if(RMP_Bitmap[Count]==0)
            continue;
        
        Count=RMP_MSB_Get(RMP_Bitmap[Count])+(Count<<RMP_WORD_ORDER);
        
        /* See if the current thread and the next thread are the same. If yes, place the current at the end of the queue */
        if(RMP_Cur_Thd==(struct RMP_Thd*)(RMP_Run[Count].Next))
        {
            RMP_List_Del(RMP_Cur_Thd->Run_Head.Prev, RMP_Cur_Thd->Run_Head.Next);
            RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),
                         RMP_Run[RMP_Cur_Thd->Prio].Prev,
                         &(RMP_Run[RMP_Cur_Thd->Prio]));
        }
        
        /* Replenish timeslices */
        RMP_Cur_Thd->Slices_Left=RMP_Cur_Thd->Slices;
        RMP_Cur_Thd=(struct RMP_Thd*)(RMP_Run[Count].Next);
        break;
    }
    
    /* Load the SP value from thread structure */
    RMP_Cur_SP=RMP_Cur_Thd->Stack;
}
/* End Function:_RMP_Get_High_Rdy ********************************************/

/* Begin Function:_RMP_Tick_Handler *******************************************
Description : The system tick timer interrupt routine.
Input       : ptr_t Ticks - How many ticks have passed.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Tick_Handler(ptr_t Ticks)
{
    struct RMP_Thd* Thread;
    /* Increase the timestamp as always */
    RMP_Tick+=Ticks;
#if(RMP_USE_HOOKS==RMP_TRUE)
    RMP_Tick_Hook(Ticks);
#endif
    /* See if the current thread expired. If yes, trigger a scheduler event */
    if(Ticks>=RMP_Cur_Thd->Slices_Left)
        RMP_Sched_Pend=1;
    else
        RMP_Cur_Thd->Slices_Left-=Ticks;
    
    /* Is the scheduler locked? If yes, we cannot process timer events here and will have to pend them */
    if((&RMP_Delay)!=RMP_Delay.Next)
    {
        Thread=RMP_DLY2THD(RMP_Delay.Next);
        /* If the value is less than this, then it means that the time have
         * already passed and we have to process this */
        if((RMP_Tick-Thread->Timeout)<=(RMP_ALLBITS>>1))
        {
            /* If scheduler locked, we pend the processing until it is unlocked */
            if(RMP_Sched_Locked!=0)
                RMP_Timer_Pend=1;
            else
                _RMP_Timer_Proc();
        }
    }
    
    if((RMP_Sched_Locked==0)&&(RMP_Sched_Pend!=0))
    {
        RMP_Sched_Pend=0;
        _RMP_Yield();
    }
}
/* End Function:_RMP_Tick_Handler ********************************************/

/* Begin Function:_RMP_Get_Near_Ticks *****************************************
Description : Get the nearest timer interrupt arrival time.
Input       : None.
Output      : None.
Return      : ptr_t Ticks - How many ticks until the next timeout.
******************************************************************************/
ptr_t _RMP_Get_Near_Ticks(void)
{
    ptr_t Value;
    struct RMP_Thd* Thread;
    
    /* What is the current thread's timeout value? */
    Value=RMP_Cur_Thd->Slices_Left;
    
    /* What is the nearest timer timeout value? */
    if((&RMP_Delay)!=RMP_Delay.Next)
    {
        Thread=RMP_DLY2THD(RMP_Delay.Next);
        /* See if it is nearer - don't worry about the situation that the timer
         * have overflown, because if that is to happen, it would have been 
         * already processed by the timeout processing routine just called above. */
        if((Thread->Timeout-RMP_Tick)<Value)
            Value=Thread->Timeout-RMP_Tick;
    }
    
    /* The timer setting is at least 1 tick */
    if(Value==0)
        Value=1;
    
    return Value;
}
/* End Function:_RMP_Get_Near_Ticks ******************************************/

/* Begin Function:_RMP_Set_Rdy ************************************************
Description : Set the thread as ready to schedule. That means, put the thread into
              the runqueue. When this is called, please make sure that the scheduler
              is locked.
Input       : volatile struct RMP_Thd* Thread - The thread to put into the runqueue.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Set_Rdy(volatile struct RMP_Thd* Thread)
{
    /* Insert this into the corresponding runqueue */
    RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
    /* Set this runlevel as active */
    RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
    
    /* Compare this with the current one to see if we need a context switch */
    if(Thread->Prio>RMP_Cur_Thd->Prio)
        RMP_Sched_Pend=1;
}
/* End Function:_RMP_Set_Rdy *************************************************/

/* Begin Function:_RMP_Clr_Rdy ************************************************
Description : Clear the thread from the runqueue. When this is called, please 
              make sure that the scheduler is locked.
Input       : volatile struct RMP_Thd* Thread - The thread to clear from the runqueue.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Clr_Rdy(volatile struct RMP_Thd* Thread)
{
    /* See if it is the last thread on the priority level */
    if(Thread->Run_Head.Prev==Thread->Run_Head.Next)
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]&=~(((ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK));
    
    /* Insert this into the corresponding runqueue */
    RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
    
    /* If it is the current thread, request a context switch */
    if(Thread==RMP_Cur_Thd)
        RMP_Sched_Pend=1;
}
/* End Function:_RMP_Clr_Rdy *************************************************/

/* Begin Function:_RMP_Dly_Ins ************************************************
Description : Insert the thread into the delay queue, given some timeslices into the
              future. The thread must not be in the run queue any more.
Input       : volatile struct RMP_Thd* Thread - The thread to put into the delay queue.
              ptr_t - The timeslices to delay.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Dly_Ins(volatile struct RMP_Thd* Thread, ptr_t Slices)
{
    struct RMP_List* Trav_Ptr;
    struct RMP_Thd* Trav_Thd;
    
    Trav_Ptr=(struct RMP_List*)RMP_Delay.Next;
    while(Trav_Ptr!=&(RMP_Delay))
    {
        Trav_Thd=RMP_DLY2THD(Trav_Ptr);
        if((Trav_Thd->Timeout-RMP_Tick)>Slices)
            break;
    }

    /* Insert this into the list */
    Thread->Timeout=RMP_Tick+Slices;
    RMP_List_Ins(&Thread->Dly_Head,Trav_Ptr->Prev,Trav_Ptr);
}
/* End Function:_RMP_Dly_Ins *************************************************/

/* Begin Function:RMP_Thd_Crt *************************************************
Description : Create a real-time thread.
Input       : volatile struct RMP_Thd* Thread - The thread structure provided. The user 
                                                should make this allocation according to
                                                his or her needs.
              void* Entry - The entry of the thread.
              void* Stack - The stack of this thread.
              void* Arg - The argument to pass to the thread.
              ptr_t Prio - The priority of the thread.
              ptr_t Slices - The number of timeslices to assign to the thread.
Output      : None.
Return      : ret_t - If successful, 0. on error, return an error code.
******************************************************************************/
ret_t RMP_Thd_Crt(volatile struct RMP_Thd* Thread, void* Entry, void* Stack, void* Arg, ptr_t Prio, ptr_t Slices)
{
    /* Check if the priority and timeslice range is correct */
    if(Prio>=RMP_MAX_PREEMPT_PRIO)
        return RMP_ERR_PRIO;
    if((Slices==0)||(Slices>=RMP_MAX_SLICES))
        return RMP_ERR_SLICE;
    
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
        return RMP_ERR_THD;
    
    RMP_Lock_Sched();
    
    if(RMP_THD_STATE(Thread->State)!=RMP_THD_FREE)
    {
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
    }
    
    /* Create the thread and insert it into the list */
    Thread->Prio=Prio;
    Thread->Slices=Slices;
    Thread->Slices_Left=Slices;
    Thread->Stack=(ptr_t)Stack;
    
    /* Initialize its stack and sending list */
    _RMP_Stack_Init((ptr_t)Entry, (ptr_t)Stack, (ptr_t)Arg);
    RMP_List_Crt(&(Thread->Snd_List));
    
    /* Notify the scheduler that we have created something new, also check locks */
    Thread->State=RMP_THD_RUNNING;
    _RMP_Set_Rdy(Thread);
    
    RMP_Unlock_Sched();

    return 0;
}
/* End Function:RMP_Thd_Crt **************************************************/

/* Begin Function:RMP_Thd_Del *************************************************
Description : Delete a real-time thread.
Input       : volatile struct RMP_Thd* Thread - The thread structure of the thread.
Output      : None.
Return      : ret_t - If successful, 0; or an error code.
******************************************************************************/
ret_t RMP_Thd_Del(volatile struct RMP_Thd* Thread)
{
    struct RMP_Thd* Release;
    ptr_t Self_Del;
    
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
        return RMP_ERR_THD;
    
    RMP_Lock_Sched();
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
    }
    
    /* See if anyone waiting to send to this thread. If there is, release all these threads */
    while(&(Thread->Snd_List)!=Thread->Snd_List.Next)
    {
        Release=(struct RMP_Thd*)(Thread->Snd_List.Next);
        RMP_List_Del(Release->Run_Head.Prev,Release->Run_Head.Next);
        if(RMP_THD_STATE(Release->State)==RMP_THD_SNDDLY)
            RMP_List_Del(Release->Dly_Head.Prev,Release->Dly_Head.Next);

        RMP_THD_STATE_SET(Release->State,RMP_THD_RUNNING);

        /* Is it suspended? If yes, we can't directly send it running */
        if((Release->State&RMP_THD_SUSPENDED)==0)
           _RMP_Set_Rdy(Release);
        
        Release->Retval=RMP_ERR_OPER;
    }
    
    /* See what is it state */
    Self_Del=0;
    if(RMP_Cur_Thd==Thread)
        Self_Del=1;
    
    switch(RMP_THD_STATE(Thread->State))
    {
        case RMP_THD_RUNNING:
        {
            if((Thread->State&RMP_THD_SUSPENDED)==0)
                _RMP_Clr_Rdy(Thread);
            break;
        }
        
        case RMP_THD_SNDDLY:
        case RMP_THD_SEMDLY:
        {
            if((Thread->State&RMP_THD_SUSPENDED)==0)
                RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
            RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
            Thread->Retval=RMP_ERR_OPER;
            break;
        }
            
        case RMP_THD_RCVDLY:
        {
            RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
            Thread->Retval=RMP_ERR_OPER;
            break;
        }
            
        case RMP_THD_DELAYED:
        {
            RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
            Thread->Retval=RMP_ERR_OPER;
            break;
        }
        /* Should not get here */
        default:break;
    }
    Thread->State=RMP_THD_FREE;
    
    RMP_Unlock_Sched();
    
    /* If we are deleting ourself, just stop the execution here */
    if(Self_Del!=0)
        while(1);

    return 0;
}
/* End Function:RMP_Thd_Del **************************************************/

/* Begin Function:RMP_Thd_Set *************************************************
Description : Change the priority or timeslice of a real-time thread. If one of
              the changes is not desired, just leave it to RMP_MAX_PREEMPT_PRIO
              or RMP_MAX_SLICES.
Input       : volatile struct RMP_Thd* Thread - The thread structure of the thread.
              ptr_t Prio - The priority of the thread.
              ptr_t Slices - The new timeslice value for this thread.
Output      : None.
Return      : ret_t - If successful, 0; else error code.
******************************************************************************/
ret_t RMP_Thd_Set(volatile struct RMP_Thd* Thread, ptr_t Prio, ptr_t Slices)
{
    /* Check if the priority and timeslice range is correct */
    if(Slices==0)
        return RMP_ERR_SLICE;
    
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
        return RMP_ERR_THD;
    
    RMP_Lock_Sched();
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
    }
    
    /* See if the thread is in running state */
    if(RMP_THD_STATE(Thread->State)==RMP_THD_RUNNING)
    {
        /* See if we are gonna change one of it or both */
        if(Prio<RMP_MAX_PREEMPT_PRIO)
        {
            if(Thread->Prio!=Prio)
            {
                _RMP_Clr_Rdy(Thread);
                Thread->Prio=Prio;
                _RMP_Set_Rdy(Thread);
            }
        }
        
        if(Slices<RMP_MAX_SLICES)
            Thread->Slices=Slices;
    }
    else
    {
        if(Prio<RMP_MAX_PREEMPT_PRIO)
            Thread->Prio=Prio;
        
        if(Slices<RMP_MAX_SLICES)
            Thread->Slices=Slices;
    }
    
    RMP_Unlock_Sched();
    
    return 0;
}
/* End Function:RMP_Thd_Set **************************************************/

/* Begin Function:RMP_Thd_Suspend *********************************************
Description : Suspend the execution of a real-time thread.
Input       : volatile struct RMP_Thd* Thread - The thread structure of the thread.
Output      : None.
Return      : ret_t - If successful, 0; else error code.
******************************************************************************/
ret_t RMP_Thd_Suspend(volatile struct RMP_Thd* Thread)
{
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
        return RMP_ERR_THD;
    
    RMP_Lock_Sched();
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
    }
    
    /* Suspend it */
    if((Thread->State&RMP_THD_SUSPENDED)!=0)
    {
        RMP_Unlock_Sched();
        return RMP_ERR_STATE;
    }
    /* Mark this as suspended */
    Thread->State|=RMP_THD_SUSPENDED;
    /* Only when it is running do we clear this */
    if(RMP_THD_STATE(Thread->State)==RMP_THD_RUNNING)
        _RMP_Clr_Rdy(Thread);
    
    RMP_Unlock_Sched();

    return 0;
}
/* End Function:RMP_Thd_Suspend **********************************************/

/* Begin Function:RMP_Thd_Resume **********************************************
Description : Resume the execution of a real-time thread.
Input       : volatile struct RMP_Thd* Thread - The thread structure of the thread.
Output      : None.
Return      : ret_t - If successful, 0; else error code.
******************************************************************************/
ret_t RMP_Thd_Resume(volatile struct RMP_Thd* Thread)
{
    ret_t Retval;
    
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
        return RMP_ERR_THD;

    RMP_Lock_Sched();
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
    }
    
    /* Check if the thread is suspended, if not, then throw an error */
    if((Thread->State&RMP_THD_SUSPENDED)!=0)
    {
        /* Suspended */
        Thread->State&=~RMP_THD_SUSPENDED;
        if(RMP_THD_STATE(Thread->State)==RMP_THD_RUNNING)
            _RMP_Set_Rdy(Thread);
        Retval=0;
    }
    else
        Retval=RMP_ERR_STATE;
    
    RMP_Unlock_Sched();

    return Retval;
}
/* End Function:RMP_Thd_Resume ***********************************************/

/* Begin Function:RMP_Thd_Snd *************************************************
Description : Send to a real-time thread's mailbox. If the mailbox is full, then
              this operation can potentially block.
Input       : volatile struct RMP_Thd* Thread - The thread structure of the thread to send to.
              ptr_t Data - The data to send to that thread.
              ptr_t Slices - The timeslice to wait, if the mailbox is already full.
Output      : None.
Return      : ret_t - If successful,0; or an error code.
******************************************************************************/
ret_t RMP_Thd_Snd(volatile struct RMP_Thd* Thread, ptr_t Data, ptr_t Slices)
{
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
        return RMP_ERR_THD;
    
    RMP_Lock_Sched();
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
    }
    
    /* Are we sending to ourself? This is not allowed */
    if(RMP_Cur_Thd==Thread)
    {
        RMP_Unlock_Sched();
        return RMP_ERR_OPER;
    }
    
    RMP_Cur_Thd->Retval=0;

    /* See if there are already a value in the mailbox, if there is, we block */
    if((Thread->State&RMP_THD_MBOXFUL)!=0)
    {
        /* Mailbox full, we block, and put ourself into the queue */
        if(Slices==0)
        {
            RMP_Unlock_Sched();
            return RMP_ERR_OPER;
        }

        /* We must be running */
        _RMP_Clr_Rdy(RMP_Cur_Thd);
        RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),Thread->Snd_List.Prev,&(Thread->Snd_List));

        if(Slices<RMP_MAX_SLICES)
        {
            _RMP_Dly_Ins(RMP_Cur_Thd, Slices);
            RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_SNDDLY);
        }
        else
            RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_SNDBLK);

        RMP_Cur_Thd->Data=Data;
    }
    else
    {
        /* Mailbox not full. We need to check if the receiver is waiting for us */
        if((RMP_THD_STATE(Thread->State)==RMP_THD_RCVBLK)||
           (RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY))
        {
            /* The receiver is blocked, wake it up and return the value */
            if(RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY)
                RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
            
            RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);

            /* Is it suspended? If yes, we can't directly send it running */
            if((Thread->State&RMP_THD_SUSPENDED)==0)
                _RMP_Set_Rdy(Thread);
        }
        
        /* Set the mailbox */
        Thread->Mailbox=Data;
        Thread->State|=RMP_THD_MBOXFUL;
    }
    
    RMP_Unlock_Sched();

    return RMP_Cur_Thd->Retval;
}
/* End Function:RMP_Thd_Snd **************************************************/

/* Begin Function:RMP_Thd_Snd_ISR *********************************************
Description : Send to a real-time thread's mailbox. If the mailbox is full, then
              this operation will just fail. This function can only be called from
              an ISR whose priority is below or equal to the context switch handler's.
Input       : volatile struct RMP_Thd* Thread - The thread structure of the thread to send to.
              ptr_t Data - The data to send to that thread.
Output      : None.
Return      : ret_t - If successful,0; or an error code.
******************************************************************************/
ret_t RMP_Thd_Snd_ISR(volatile struct RMP_Thd* Thread, ptr_t Data)
{
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
        return RMP_ERR_THD;
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
        return RMP_ERR_THD;
    
    if(RMP_Sched_Locked!=0)
        return RMP_ERR_OPER;

    /* See if there are already a value in the mailbox, if there is, we abort */
    if((Thread->State&RMP_THD_MBOXFUL)!=0)
        return RMP_ERR_OPER;
    else
    {
        /* Mailbox not full. We need to check if the receiver is waiting for us */
        if((RMP_THD_STATE(Thread->State)==RMP_THD_RCVBLK)||
           (RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY))
        {
            /* The receiver is blocked, wake it up and return the value */
            if(RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY)
                RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
            
            RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);

            /* Is it suspended? If yes, we can't directly send it running */
            if((Thread->State&RMP_THD_SUSPENDED)==0)
            {
                _RMP_Set_Rdy(Thread);
                if(Thread->Prio>RMP_Cur_Thd->Prio)
                    _RMP_Yield();
            }
        }
        
        /* Set the mailbox */
        Thread->Mailbox=Data;
        Thread->State|=RMP_THD_MBOXFUL;
    }

    return 0;
}
/* End Function:RMP_Thd_Snd_ISR **********************************************/

/* Begin Function:RMP_Thd_Rcv *************************************************
Description : Receive a message from our own mailbox, and this is blocking.
Input       : ptr_t Slices - The timeslices to wait, if the mailbox is empty.
Output      : ptr_t* Data - The pointer to put the data to.
Return      : ret_t - If successful,0; or an error code.
******************************************************************************/
ret_t RMP_Thd_Rcv(ptr_t* Data, ptr_t Slices)
{
    struct RMP_Thd* Sender;
    
    RMP_Lock_Sched();

    /* Check if there is a value in our mailbox. If yes, we return with that value */
    if((RMP_Cur_Thd->State&RMP_THD_MBOXFUL)!=0)
    {
        /* Get the value from mailbox */
        *Data=RMP_Cur_Thd->Mailbox;
        RMP_Cur_Thd->State&=~RMP_THD_MBOXFUL;
        RMP_Unlock_Sched();
        return 0;
    }
    else
    {
        /* Is there any other guy waiting on us? If there is, unblock it and set it running */
        if(&(RMP_Cur_Thd->Snd_List)!=RMP_Cur_Thd->Snd_List.Next)
        {
            /* Read the data */
            Sender=(struct RMP_Thd*)(RMP_Cur_Thd->Snd_List.Next);
            RMP_List_Del(Sender->Run_Head.Prev,Sender->Run_Head.Next);
            *Data=Sender->Data;
            /* Now we unblock it - what state is it in? */
            if((RMP_THD_STATE(Sender->State)==RMP_THD_SNDDLY))
                RMP_List_Del(Sender->Dly_Head.Prev,Sender->Dly_Head.Next);
            
            RMP_THD_STATE_SET(Sender->State,RMP_THD_RUNNING);

            /* Is it suspended? If yes, we can't directly send it running */
            if((Sender->State&RMP_THD_SUSPENDED)==0)
                _RMP_Set_Rdy(Sender);
            
            RMP_Unlock_Sched();
        }
        /* No sender waiting on us, we need to block */
        else
        {
            /* No such value in our mailbox, we need to block */
            if(Slices==0)
            {
                RMP_Unlock_Sched();
                return RMP_ERR_OPER;
            }

            /* We must be running */
            _RMP_Clr_Rdy(RMP_Cur_Thd);

            if(Slices<RMP_MAX_SLICES)
            {
                _RMP_Dly_Ins(RMP_Cur_Thd, Slices);
                RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_RCVDLY);
            }
            else
                RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_RCVBLK);
            
            RMP_Unlock_Sched();
            /* Dummy read - to separate the lock & unlock. If the compiler optimizes these two
             * functions(inline them) on some architectures sometimes we never block. */
            *Data=RMP_Cur_Thd->Mailbox;
            /* We've been unblocked. There must be something in our mbox, or we should have failed */
            RMP_Lock_Sched();
            *Data=RMP_Cur_Thd->Mailbox;
            RMP_Cur_Thd->State&=~RMP_THD_MBOXFUL;
            RMP_Unlock_Sched();
        }
    }
    
    return RMP_Cur_Thd->Retval;
}
/* End Function:RMP_Thd_Rcv **************************************************/

/* Begin Function:RMP_Thd_Delay ***********************************************
Description : Delay the execution of a real-time thread.
Input       : ptr_t Slices - The number of timeslices to delay.
Output      : None.
Return      : ret_t - If successful, 0; or an error code.
******************************************************************************/
ret_t RMP_Thd_Delay(ptr_t Slices)
{
    if((Slices==0)||(Slices>=RMP_MAX_SLICES))
        return RMP_ERR_SLICE;
    
    RMP_Lock_Sched();

    /* We must be running */
    _RMP_Clr_Rdy(RMP_Cur_Thd);
    RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_DELAYED);
    _RMP_Dly_Ins(RMP_Cur_Thd, Slices);

    RMP_Cur_Thd->Retval=0;
    
    RMP_Unlock_Sched();
    
    /* Need to return if successful or not */
    return RMP_Cur_Thd->Retval;
}
/* End Function:RMP_Thd_Delay ************************************************/

/* Begin Function:RMP_Thd_Cancel **********************************************
Description : Cancel the real-time thread from a previous delay.
Input       : volatile struct RMP_Thd* Thread - The thread structure of the thread.
Output      : None.
Return      : ret_t - If successful, 0; or an error code.
******************************************************************************/
ret_t RMP_Thd_Cancel(volatile struct RMP_Thd* Thread)
{
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
        return RMP_ERR_THD;
    
    RMP_Lock_Sched();
    
    /* Is it delayed? */
    if(RMP_THD_STATE(Thread->State)!=RMP_THD_DELAYED)
    {
        RMP_Unlock_Sched();
        return RMP_ERR_STATE;
    }

    /* Delete it from the delay list */
    RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
    RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
    
    /* Only when when this thread is not suspended do we change it back */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
        _RMP_Set_Rdy(Thread);
    
    Thread->Retval=RMP_ERR_OPER;
    
    RMP_Unlock_Sched();

    return 0;
}
/* End Function:RMP_Thd_Cancel ***********************************************/

/* Begin Function:RMP_Sem_Crt *************************************************
Description : Create a semaphore in the system.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore structure.
              ptr_t Number - The initial number of this semaphore.
Output      : None.
Return      : ret_t - If successful, 0; or an error code.
******************************************************************************/
ret_t RMP_Sem_Crt(volatile struct RMP_Sem* Semaphore, ptr_t Number)
{
    /* Check if this semaphore structure could possibly be in use */
    if(Semaphore==0)
        return RMP_ERR_SEM;
    
    RMP_Lock_Sched();
    
    if(Semaphore->State!=RMP_SEM_FREE)
    {
        RMP_Unlock_Sched();
        return RMP_ERR_SEM;
    }
    
    /* Is the number too great to initialize? */
    if(Number>=RMP_SEM_MAX_NUM)
    {
        RMP_Unlock_Sched();
        return RMP_ERR_OPER;
    }

    /* Initialize contents */
    Semaphore->Cur_Num=Number;
    Semaphore->State=RMP_SEM_USED;
    RMP_List_Crt(&(Semaphore->Wait_List));
    
    RMP_Unlock_Sched();
    
    return 0;
}
/* End Function:RMP_Sem_Crt **************************************************/

/* Begin Function:RMP_Sem_Del *************************************************
Description : Delete a semaphore in the system.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore structure.
Output      : None.
Return      : ret_t - If successful, 0; or an error code.
******************************************************************************/
ret_t RMP_Sem_Del(volatile struct RMP_Sem* Semaphore)
{
    struct RMP_Thd* Thread;
    
    /* Check if this semaphore structure could possibly be in use */
    if(Semaphore==0)
        return RMP_ERR_SEM;
    
    RMP_Lock_Sched();
    
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_Unlock_Sched();
        return RMP_ERR_SEM;
    }

    /* Get rid of all guys waiting on it */
    while(&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)
    {
        Thread=(struct RMP_Thd*)(Semaphore->Wait_List.Next);
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
        if(RMP_THD_STATE(Thread->State)==RMP_THD_SEMDLY)
            RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);

        RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);

        /* Is it suspended? If yes, we can't directly send it running */
        if((Thread->State&RMP_THD_SUSPENDED)==0)
           _RMP_Set_Rdy(Thread);
        
        Thread->Retval=RMP_ERR_OPER;
    }
    Semaphore->State=RMP_SEM_FREE;
    
    RMP_Unlock_Sched();

    return 0;
}
/* End Function:RMP_Sem_Del **************************************************/

/* Begin Function:RMP_Sem_Pend ************************************************
Description : Pend on the semaphore, trying to get one.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore structure.
              ptr_t Slices - The number of slices to wait.
Output      : None.
Return      : ret_t - If successful, the current semaphore number; or an error code.
******************************************************************************/
ret_t RMP_Sem_Pend(volatile struct RMP_Sem* Semaphore, ptr_t Slices)
{
    /* Check if this semaphore structure could possibly be in use */
    if(Semaphore==0)
        return RMP_ERR_SEM;
    
    RMP_Lock_Sched();
    
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_Unlock_Sched();
        return RMP_ERR_SEM;
    }
    
    /* Check if we can get one immediately */
    if(Semaphore->Cur_Num!=0)
    {
        Semaphore->Cur_Num--;
        RMP_Unlock_Sched();
        return Semaphore->Cur_Num;
    }
    else
    {
        /* Cannot get one, we need to block */
        if(Slices==0)
        {
            RMP_Unlock_Sched();
            return RMP_ERR_OPER;
        }

        /* We must be running - place into waitlist now */
        _RMP_Clr_Rdy(RMP_Cur_Thd);
        RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),Semaphore->Wait_List.Prev,&(Semaphore->Wait_List));
        
        if(Slices<RMP_MAX_SLICES)
        {
            _RMP_Dly_Ins(RMP_Cur_Thd, Slices);
            RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_SEMDLY);
        }
        else
            RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_SEMBLK);
        
        RMP_Cur_Thd->Retval=0;
    }
    
    RMP_Unlock_Sched();

    return RMP_Cur_Thd->Retval;
}
/* End Function:RMP_Sem_Pend *************************************************/

/* Begin Function:RMP_Sem_Abort ***********************************************
Description : Abort the waiting of one thread on a semaphore.
Input       : volatile struct RMP_Thd* Thread - The thread structure of the thread.
Output      : None.
Return      : ret_t - If successful, 0; or an error code.
******************************************************************************/
ret_t RMP_Sem_Abort(volatile struct RMP_Thd* Thread)
{
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
        return RMP_ERR_THD;

    RMP_Lock_Sched();
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
    }
    
    /* Is it waiting on a semaphore? If no, we abort and return an error code */
    if((RMP_THD_STATE(Thread->State)!=RMP_THD_SEMBLK)&&
       (RMP_THD_STATE(Thread->State)!=RMP_THD_SEMDLY))
    {
        RMP_Unlock_Sched();
        return RMP_ERR_STATE;
    }

    /* Waiting for a semaphore. We abort it and return */
    RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
    if(RMP_THD_STATE(Thread->State)==RMP_THD_SEMDLY)
        RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
    
    RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);

    /* Only when when this thread is not suspended do we change it back */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
        _RMP_Set_Rdy(Thread);
    
    Thread->Retval=RMP_ERR_OPER;
    
    RMP_Unlock_Sched();

    return 0;
}
/* End Function:RMP_Sem_Abort ************************************************/

/* Begin Function:RMP_Sem_Post ************************************************
Description : Post a number of semaphores to the list. This function can only be
              called from an ISR whose priority is below or equal to the context
              switch handler's.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore structure.
              ptr_t Number - The number to post.
Output      : None.
Return      : ret_t - If successful, 0; or an error code.
******************************************************************************/
ret_t RMP_Sem_Post(volatile struct RMP_Sem* Semaphore, ptr_t Number)
{
    struct RMP_Thd* Thread;
    
    /* Check if this semaphore structure could possibly be in use */
    if((Semaphore==0)||(Number==0))
        return RMP_ERR_SEM;
    
    RMP_Lock_Sched();
    
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_Unlock_Sched();
        return RMP_ERR_SEM;
    }
    
    /* Would the maximum value be exceeded if this is posted? */
    if((Semaphore->Cur_Num+Number)>=RMP_SEM_MAX_NUM)
    {
        RMP_Unlock_Sched();
        return RMP_ERR_OPER;
    }
    
    Semaphore->Cur_Num+=Number;
    /* Is there any thread waiting on it? If there are, clean them up*/
    while((&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)&&(Semaphore->Cur_Num!=0))
    {
        Thread=(struct RMP_Thd*)(Semaphore->Wait_List.Next);
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
        if(RMP_THD_STATE(Thread->State)==RMP_THD_SEMDLY)
            RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
        RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);

        /* Only when when this thread is not suspended do we change it back */
        if((Thread->State&RMP_THD_SUSPENDED)==0)
            _RMP_Set_Rdy(Thread);

        /* Finally, return success */
        Thread->Retval=0;
        Semaphore->Cur_Num--;
    }

    RMP_Unlock_Sched();

    return 0;
}
/* End Function:RMP_Sem_Post *************************************************/

/* Begin Function:RMP_Sem_Post_ISR ********************************************
Description : Post a number of semaphores to the list.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore structure.
              ptr_t Number - The number to post.
Output      : None.
Return      : ret_t - If successful, 0; or an error code.
******************************************************************************/
ret_t RMP_Sem_Post_ISR(volatile struct RMP_Sem* Semaphore, ptr_t Number)
{
    struct RMP_Thd* Thread;
    /* Check if this semaphore structure could possibly be in use */
    if((Semaphore==0)||(Number==0))
        return RMP_ERR_SEM;
    
    if(Semaphore->State!=RMP_SEM_USED)
        return RMP_ERR_SEM;

    if(RMP_Sched_Locked!=0)
        return RMP_ERR_OPER;

    /* Would the maximum value be exceeded if this is posted? */
    if((Semaphore->Cur_Num+Number)>=RMP_SEM_MAX_NUM)
        return RMP_ERR_OPER;
    
    Semaphore->Cur_Num+=Number;
    /* Is there any thread waiting on it? If there are, clean them up*/
    while((&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)&&(Semaphore->Cur_Num!=0))
    {
        Thread=(struct RMP_Thd*)(Semaphore->Wait_List.Next);
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
        if(RMP_THD_STATE(Thread->State)==RMP_THD_SEMDLY)
            RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
        
        RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);

        /* Only when when this thread is not suspended do we change it back */
        if((Thread->State&RMP_THD_SUSPENDED)==0)
        {
            _RMP_Set_Rdy(Thread);
            if(Thread->Prio>RMP_Cur_Thd->Prio)
                _RMP_Yield();
        }

        /* Finally, return success */
        Thread->Retval=0;
        Semaphore->Cur_Num--;
    }

    return 0;
}
/* End Function:RMP_Sem_Post_ISR *********************************************/

/* Begin Function:RMP_Start_Hook **********************************************
Description : Initialization hook for low-level hardware, executed immediately
              after the kernel's low-level initialization.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
#if(RMP_USE_HOOKS==RMP_FALSE)
void RMP_Start_Hook(void)
{
    return;
}
#endif
/* End Function:RMP_Start_Hook ***********************************************/

/* Begin Function:RMP_Save_Ctx ************************************************
Description : Save hook for extra context, such as FPU, peripherals and MPU.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
#if(RMP_USE_HOOKS==RMP_FALSE)
void RMP_Save_Ctx(void)
{
    return;
}
#endif
/* End Function:RMP_Save_Ctx *************************************************/

/* Begin Function:RMP_Load_Ctx ************************************************
Description : Load hook for extra context, such as FPU, peripherals and MPU.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
#if(RMP_USE_HOOKS==RMP_FALSE)
void RMP_Load_Ctx(void)
{
    return;
}
#endif
/* End Function:RMP_Load_Ctx *************************************************/

/* Begin Function:RMP_Tick_Hook ***********************************************
Description : Save hook for extra context, such as FPU, peripherals and MPU.
Input       : ptr_t Ticks - The number of ticks passed. For constant rate ticking
                            applications this is always 1; For tickless systems this
                            could be anything.
Output      : None.
Return      : None.
******************************************************************************/
#if(RMP_USE_HOOKS==RMP_FALSE)
void RMP_Tick_Hook(ptr_t Ticks)
{
    Ticks=Ticks;
    return;
}
#endif
/* End Function:RMP_Tick_Hook ************************************************/

/* Begin Function:RMP_Init ****************************************************
Description : The entry of the user thread. This is the first user thread that
              will be created.
              The user threads should never return.
Input       : None.
Output      : None.
Return      : int - This function never returns.
******************************************************************************/
void RMP_Init(void)
{
    RMP_Lock_Sched();
    
    /* Platform will use this hook to do something */
    _RMP_Plat_Hook();
    /* Start the second thread here */
    RMP_Init_Hook();
    
    RMP_Unlock_Sched();
    
    while(1)
    {
        RMP_Init_Idle();
    };
}
/* End Function:RMP_Init *****************************************************/

/* Begin Function:main ********************************************************
Description : The entrance of the operating system. This function is for 
              compatibility with the ARM toolchain.
Input       : None.
Output      : None.
Return      : int - This function never returns.
******************************************************************************/
int main(void)
{
    ptr_t Count;
    /* Initialize the kernel data structures first */
    _RMP_Low_Level_Init();
    
#if(RMP_USE_HOOKS==RMP_TRUE)
    RMP_Start_Hook();
#endif
    
    RMP_Tick=0;
    /* Now initialize the kernel data structures */
    RMP_Sched_Lock_Cnt=0;
    RMP_Sched_Locked=0;
    RMP_Sched_Pend=0;
    RMP_Timer_Pend=0;
    
    /* Linked lists */
    RMP_List_Crt(&RMP_Delay);
    for(Count=0;Count<RMP_MAX_PREEMPT_PRIO;Count++)
        RMP_List_Crt(&RMP_Run[Count]);
    for(Count=0;Count<RMP_BITMAP_SIZE;Count++)
        RMP_Bitmap[Count]=0;
        
    /* Now boot into the first thread */
    RMP_Clear(&RMP_Init_Thd,sizeof(struct RMP_Thd));
    RMP_Init_Thd.Prio=0;
    RMP_Init_Thd.Slices=10;
    RMP_Init_Thd.Slices_Left=10;
    RMP_Init_Thd.State=RMP_THD_RUNNING;
    RMP_Init_Thd.Stack=RMP_INIT_STACK;
    
    /* Initialize sending list */
    RMP_List_Crt(&(RMP_Init_Thd.Snd_List));
    
    /* Insert this into the corresponding runqueue */
    RMP_List_Ins(&(RMP_Init_Thd.Run_Head),RMP_Run[0].Prev,&(RMP_Run[0]));
    /* Set this runlevel as active - in fact it is always active */
    RMP_Bitmap[0]|=1;
    
    /* Set current thread and stack */
    RMP_Cur_Thd=(struct RMP_Thd*)(&RMP_Init_Thd);
    RMP_Cur_SP=RMP_Init_Thd.Stack;
    
    /* Now jump to the user function and will never return. Initialization of stack is not needed */
    _RMP_Start((ptr_t)RMP_Init, (ptr_t)RMP_Init_Thd.Stack);
    
    return 0;
}
/* End Function:main *********************************************************/

/* Begin Function:RMP_RBIT_Get ************************************************
Description : Reverse the bit order in a word.
Input       : ptr_t Val - The input value.
Output      : None.
Return      : ptr_t - The result.
******************************************************************************/
static const u8 RMP_RBIT_Table[256]=
{
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

ptr_t RMP_RBIT_Get(ptr_t Val)
{
    ptr_t Ret;
    u8* To;
    u8* From;
    
    To=(u8*)(&Ret);
    From=(u8*)(&Val);
    
#if(RMP_WORD_ORDER==4)
    To[0]=RMP_RBIT_Table[From[1]];
    To[1]=RMP_RBIT_Table[From[0]];
#elif(RMP_WORD_ORDER==5)
    To[0]=RMP_RBIT_Table[From[3]];
    To[1]=RMP_RBIT_Table[From[2]];
    To[2]=RMP_RBIT_Table[From[1]];
    To[3]=RMP_RBIT_Table[From[0]];
#else
    To[0]=RMP_RBIT_Table[From[7]];
    To[1]=RMP_RBIT_Table[From[6]];
    To[2]=RMP_RBIT_Table[From[5]];
    To[3]=RMP_RBIT_Table[From[4]];
    To[4]=RMP_RBIT_Table[From[3]];
    To[5]=RMP_RBIT_Table[From[2]];
    To[6]=RMP_RBIT_Table[From[1]];
    To[7]=RMP_RBIT_Table[From[0]];
#endif

    return Ret;
}
/* End Function:RMP_RBIT_Get *************************************************/

/* Begin Function:RMP_LSB_Get *************************************************
Description : Get the LSB in a word. If the word is all zero, a negative value
              will be returned.
Input       : ptr_t Val - The input value.
Output      : None.
Return      : ptr_t - The LSB found.
******************************************************************************/
ptr_t RMP_LSB_Get(ptr_t Val)
{
#if(RMP_WORD_ORDER==4)
    return 15-RMP_MSB_Get(RMP_RBIT_Get(Val));
#elif(RMP_WORD_ORDER==5)
    return 31-RMP_MSB_Get(RMP_RBIT_Get(Val));
#else
    return 63-RMP_MSB_Get(RMP_RBIT_Get(Val));
#endif 
}
/* End Function:RMP_LSB_Get **************************************************/

/* Begin Function:RMP_Mem_Init ************************************************
Description : Initialize a trunk of memory as the memory pool. The TLSF allocator's
              FLI will be decided upon the memory block size. All memory allocation
              functions does not lock the scheduler; decide whether you need to lock
              in your code.
              The TLSF memory allocator is consisted of FLI, SLI and allocatable
              memory. The FLI is classified by 2^n, and the SLI segregates the 
              FLI section by an power of 2, i.e. 8 or 16. Thus, when we need 
              an memory block, we try to find it in the corresponding FLI, and
              then the SLI.(This is a two-dimensional matrix.) Then 
              (1) If the SLI has no allocatable blocks, we will allocate some
                  from the nearest bigger block.
              (2) If there is some block from the SLI block, allocate the memory
                  size and put the residue memory into the corresponding FLI and
                  SLI area.
              When freeing memory, the adjacent free memory blocks will automatically
              merge.
              In the system, the FLI is variable and the SLI is fixed to 8.
              The FLI has a miniumum block size of 64 Byte(If the allocated size
              is always smaller than 64 bits, then there's no need to use DSA.)
              To make sure that it is like this, we set the smallest allocatable
              size to 64B. In addition, we set the alignment to 8.
              [FLI]:
              .....    6       5      4       3         2        1         0
                     8K-4K   4K-2K  2K-1K  1K-512B  511-256B  255-128B  127-64B
              For example, when a memory block is 720 byte, then it should be in
              FLI=3,SLI=3.
              When a lower FLI has no blocks for allocation, it will "borrow"
              a block from the nearest FLI block that is big enough.
Input       : volatile void* Pool - The start address of the memory pool, word-aligned.
              ptr_t Size - The size of the memory pool, word-aligned.
Output      : None.
Return      : ret_t - If successful, 0; else an error code.
******************************************************************************/
ret_t RMP_Mem_Init(volatile void* Pool, ptr_t Size)
{
    cnt_t FLI_Cnt;
    ptr_t Usable_Size;
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_Mem_Head* Mem_Head;
    
    /* See if the memory pool is large enough to enable dynamic allocation - at least 4096 words */
    if(Size<(4096*sizeof(ptr_t)))
        return RMP_ERR_MEM;
    
    /* See if the address and size is word-aligned */
    if((((ptr_t)Pool&(RMP_WORD_MASK>>2))!=0)||((Size&(RMP_WORD_MASK>>2))!=0))
        return RMP_ERR_MEM;
        
    Mem=(volatile struct RMP_Mem*)Pool;
    Mem->Size=Size;
    /* Initialize the allocated block list */
    RMP_List_Crt(&(Mem->Alloc));
    /* Calculate the FLI value needed for this - we always align to 64 byte */
    Mem->FLI_Num=RMP_MSB_Get(Size-sizeof(struct RMP_Mem))-6+1;
    /* Initialize the TLSF allocation table first */
    for(FLI_Cnt=0;FLI_Cnt<(cnt_t)(Mem->FLI_Num);FLI_Cnt++)
    {
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,0)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,1)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,2)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,3)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,4)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,5)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,6)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,7)]));
    }
    for(FLI_Cnt=0;FLI_Cnt<5;FLI_Cnt++)
        Mem->Bitmap[FLI_Cnt]=0;
    
    /* Get the big memory block's size and position */
    Usable_Size=sizeof(struct RMP_Mem)+(sizeof(struct RMP_List)<<3)*(Mem->FLI_Num-8);
    Mem_Head=(struct RMP_Mem_Head*)(((ptr_t)Pool)+Usable_Size);
    Mem->Start=(ptr_t)Mem_Head;
    Usable_Size=Size-Usable_Size;
    
    /* Initialize the big block */
    _RMP_Mem_Block(Mem_Head,Usable_Size);
    
    /* Insert the memory into the corresponding level */
    _RMP_Mem_Ins(Pool, Mem_Head);
    return 0;
}
/* End Function:RMP_Mem_Init *************************************************/

/* Begin Function:_RMP_Mem_Block **********************************************
Description : Make a memory block from the memory trunk. The memory block is always
              free when created. No parameter check performed here.
Input       : volatile struct RMP_Mem_Head* Addr - The start address of the
                                                   memory block, word-aligned.
              ptr_t Size - The size of the memory block, word-aligned.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Mem_Block(volatile struct RMP_Mem_Head* Addr, ptr_t Size)
{
    volatile struct RMP_Mem_Head* Mem_Head;
    
    /* Get the big memory block's size and position */
    Mem_Head=(struct RMP_Mem_Head*)Addr;
    
    /* Initialize the big memory block */
    Mem_Head->State=RMP_MEM_FREE;
    Mem_Head->Tail=(struct RMP_Mem_Tail*)(((ptr_t)Mem_Head)+Size-sizeof(struct RMP_Mem_Tail));
    Mem_Head->Tail->Head=Mem_Head;
}
/* End Function:_RMP_Mem_Block ***********************************************/

/* Begin Function:_RMP_Mem_Ins ************************************************
Description : The memory insertion function, to insert a certain memory block
              into the corresponding FLI and SLI slot.
Input       : volatile void* Pool - The memory pool.
              volatile struct RMP_Mem_Head* Mem_Head - The pointer to the memory block.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Mem_Ins(volatile void* Pool, volatile struct RMP_Mem_Head* Mem_Head)
{
    cnt_t FLI_Level;
    cnt_t SLI_Level;
    ptr_t Level;
    ptr_t Size;
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_List* Slot;
    
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(ptr_t)(Mem_Head->Tail)-((ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);

    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
    /* Get the slot */
    Slot=&(Mem->Table[RMP_MEM_POS(FLI_Level,SLI_Level)]);

    /* See if there are any blocks in the level, equal means no. So what we inserted is the first block */
    if(Slot==Slot->Next)
    {
        /* Set the corresponding bit in the TLSF bitmap */
        Mem->Bitmap[Level>>RMP_WORD_ORDER]|=1<<(Level&RMP_WORD_MASK);
    }

    /* Insert the node now */
    RMP_List_Ins(&(Mem_Head->Head), Slot, Slot->Next);
}
/* End Function:_RMP_Mem_Ins *************************************************/

/* Begin Function:_RMP_Mem_Del ************************************************
Description : The memory deletion function, to delete a certain memory block
              from the corresponding FLI and SLI class.
Input       : volatile void* Pool - The memory pool.
              volatile struct RMP_Mem_Head* Mem_Head - The pointer to the memory block.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Mem_Del(volatile void* Pool, volatile struct RMP_Mem_Head* Mem_Head)
{
    cnt_t FLI_Level;
    cnt_t SLI_Level;
    ptr_t Level;
    ptr_t Size;
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_List* Slot;    
    
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(ptr_t)(Mem_Head->Tail)-((ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);
    
    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
    /* Get the slot */
    Slot=&(Mem->Table[RMP_MEM_POS(FLI_Level,SLI_Level)]);
   
    RMP_List_Del(Mem_Head->Head.Prev,Mem_Head->Head.Next);

    /* See if there are any blocks in the level, equal means no. So
     * what we deleted is the last block */
    if(Slot==Slot->Next)
    {
        /* Clear the corresponding bit in the TLSF bitmap */
        Mem->Bitmap[Level>>RMP_WORD_ORDER]&=~(1<<(Level&RMP_WORD_MASK));
    }
}
/* End Function:_RMP_Mem_Del *************************************************/

/* Begin Function:_RMP_Mem_Search *********************************************
Description : The TLSF memory searcher.
Input       : ptr_t Size - The memory size, must be bigger than 64. This must be
                           guaranteed before calling this function or an error
                           will unavoidably occur.
Output      : cnt_t* FLI_Level - The FLI level found.
              cnt_t* SLI_Level - The SLI level found.
Return      : ret_t - If successful, 0; else -1 for failure.
******************************************************************************/
ret_t _RMP_Mem_Search(volatile void* Pool, ptr_t Size, cnt_t* FLI_Level, cnt_t* SLI_Level)
{
    cnt_t FLI_Level_Temp;
    cnt_t SLI_Level_Temp;
    cnt_t Level;
    ptr_t LSB;
    volatile struct RMP_Mem* Mem;

    /* Make sure that it is bigger than 64. 64=2^6 */
    FLI_Level_Temp=RMP_MSB_Get(Size)-6;
    
    /* Decide the SLI level directly from the FLI level. We plus the number by one here
     * so that we can avoid the list search. However, when the allocated memory is just
     * an order of 2, then we do not need to jump to the next level and can fit directly */
    if(((cnt_t)Size)==((1<<6)<<FLI_Level_Temp))
        SLI_Level_Temp=(Size>>(FLI_Level_Temp+3))&0x07;
    else
    {
        SLI_Level_Temp=((Size>>(FLI_Level_Temp+3))&0x07)+1;
        
        /* If the SLI level is the largest of the SLI level, then jump to the next FLI level */
        if(SLI_Level_Temp==8)
        {
            FLI_Level_Temp+=1;
            SLI_Level_Temp=0;
        }
    }
    
    /* Check if the FLI level is over the boundary */
    Mem=(volatile struct RMP_Mem*)Pool;
    if((ptr_t)FLI_Level_Temp>=Mem->FLI_Num)
        return -1;
    
    /* Try to find one position on this processor word level */
    Level=(FLI_Level_Temp<<3)+SLI_Level_Temp;
    LSB=RMP_LSB_Get(Mem->Bitmap[Level>>RMP_WORD_ORDER]>>(Level&RMP_WORD_MASK));
    /* If there's at least one block that matches the query, return the level */
    if(LSB<32)
    {
        Level=(Level&(~RMP_WORD_MASK))+LSB+(Level&RMP_WORD_MASK);
        *FLI_Level=Level>>3;
        *SLI_Level=Level&0x07;
        return 0;
    }
    /* No one exactly fits */
    else
    {
        /* From the next word, query one by one */
        for(Level=(Level>>RMP_WORD_ORDER)+1;Level<5;Level++)
        {
            /* if the level has blocks of one FLI level */
            if(Mem->Bitmap[Level]!=0)
            {
                /* Find the actual level */ 
                LSB=RMP_LSB_Get(Mem->Bitmap[Level]);
                *FLI_Level=((Level<<RMP_WORD_ORDER)+LSB)>>3;
                *SLI_Level=LSB&0x07;
                return 0;
            }
        }
    }

    /* Search failed */
    return -1;
}
/* End Function:_RMP_Mem_Search **********************************************/

/* Begin Function:RMP_Malloc **************************************************
Description : Allocate some memory from a designated memory pool.
Input       : volatile void* Pool - The pool to allocate from.
              ptr_t Size - The size of the RAM needed to allocate.
Output      : None.
Return      : void* - The pointer to the memory. If no memory available, 0 is returned.
******************************************************************************/
void* RMP_Malloc(volatile void* Pool, ptr_t Size)									                   
{	
    cnt_t FLI_Level;
    cnt_t SLI_Level;
    volatile struct RMP_Mem* Mem;
    ptr_t Old_Size;
    volatile struct RMP_Mem_Head* Mem_Head;
    ptr_t Rounded_Size;
    volatile struct RMP_Mem_Head* New_Mem;
    ptr_t New_Size;
    
    if(Size==0)
        return (void*)(0);
    
    /* Round up the size:a multiple of 8 and bigger than 64B. In fact, we will add
     * extra 8 bytes at the end if the size is a multiple of 8 for safety. */
    Rounded_Size=(((Size-1)>>3)+1)<<3;
    /* See if it is smaller than the smallest block */
    Rounded_Size=(Rounded_Size>64)?Rounded_Size:64;

    /* See if such block exists, if not, abort */
    if(_RMP_Mem_Search(Pool,Rounded_Size,&FLI_Level,&SLI_Level)!=0)
        return (void*)(0);
    
    Mem=(volatile struct RMP_Mem*)Pool;
    
    /* There is such block. Get it and delete it from the TLSF list. */
    Mem_Head=(struct RMP_Mem_Head*)(Mem->Table[RMP_MEM_POS(FLI_Level,SLI_Level)].Next);
    _RMP_Mem_Del(Pool, Mem_Head);

    /* Allocate and calculate if the space left could be big enough to be a new 
     * block. If so, we will put the block back into the TLSF table */
    New_Size=(ptr_t)(Mem_Head->Tail)-((ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head)-Rounded_Size;
    if(New_Size>=sizeof(struct RMP_Mem_Head)+64+sizeof(struct RMP_Mem_Tail))
    {
        Old_Size=sizeof(struct RMP_Mem_Head)+Rounded_Size+sizeof(struct RMP_Mem_Tail);
        New_Mem=(volatile struct RMP_Mem_Head*)(((ptr_t)Mem_Head)+Old_Size);

        _RMP_Mem_Block(Mem_Head, Old_Size);
        _RMP_Mem_Block(New_Mem, New_Size);

        /* Put the extra block back */
        _RMP_Mem_Ins(Pool, New_Mem);
    }

    /* Insert the allocated block into the lists */
    RMP_List_Ins(&(Mem_Head->Head),&(Mem->Alloc),Mem->Alloc.Next);
    Mem_Head->State=RMP_MEM_USED;

    /* Finally, return the start address */
    return (void*)(((ptr_t)Mem_Head)+sizeof(struct RMP_Mem_Head));
}
/* End Function:RMP_Malloc ***************************************************/

/* Begin Function:RMP_Free ****************************************************
Description : Free allocated memory, for system use mainly. It will free memory 
              in the name of a certain process, specified by the PID.
Input       : volatile void* Pool - The pool to free to.
              void* Mem_Ptr - The pointer returned by "RMP_Malloc".
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Free(volatile void* Pool, void* Mem_Ptr)
{
    volatile struct RMP_Mem* Mem; 
    volatile struct RMP_Mem_Head* Mem_Head;
    volatile struct RMP_Mem_Head* Left_Head;
    volatile struct RMP_Mem_Head* Right_Head;
    cnt_t Merge_Left;

    /* See if the address is within the allocatable address range. If not, abort directly. */
    Mem=(volatile struct RMP_Mem*)Pool;
    if((((ptr_t)Mem_Ptr)<=((ptr_t)Mem))||(((ptr_t)Mem_Ptr)>=(((ptr_t)Mem)+Mem->Size)))
        return;

    Mem_Head=(struct RMP_Mem_Head*)(((ptr_t)Mem_Ptr)-sizeof(struct RMP_Mem_Head));
    /* See if the block can really be freed by this PID. If cannot, return directly */
    if(Mem_Head->State==RMP_MEM_FREE)
        return;

    /* Now we are sure that it can be freed. Delete it from the allocated list now */
    RMP_List_Del(Mem_Head->Head.Prev,Mem_Head->Head.Next);
    Mem_Head->State=RMP_MEM_FREE;
    
    /* Now check if we can merge it with the higher blocks */
    Right_Head=(struct RMP_Mem_Head*)(((ptr_t)(Mem_Head->Tail))+sizeof(struct RMP_Mem_Tail));
    if(((ptr_t)Right_Head)!=(((ptr_t)Mem)+Mem->Size))
    {
        /* If this one is unoccupied */
        if((Right_Head->State)==RMP_MEM_FREE)
        {
            /* Delete, merge */
            _RMP_Mem_Del(Pool,Right_Head);
            _RMP_Mem_Block(Mem_Head,
                           ((ptr_t)(Right_Head->Tail))+sizeof(struct RMP_Mem_Tail)-(ptr_t)Mem_Head);
        }
    }

    /* Now check if we can merge it with the lower blocks */
    Merge_Left=0;
    if((ptr_t)Mem_Head!=Mem->Start)
    {
        Left_Head=((struct RMP_Mem_Tail*)(((ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Tail)))->Head;

        /* If this one is unoccupied */
        if(Left_Head->State==RMP_MEM_FREE)
        {
            /* Delete, merge */
            _RMP_Mem_Del(Pool, Left_Head);
            _RMP_Mem_Block(Left_Head,
                           (ptr_t)((ptr_t)(Mem_Head->Tail)+sizeof(struct RMP_Mem_Tail)-(ptr_t)Left_Head));

            /* We have completed the merge here and the original block has destroyed.
             * Thus there's no need to insert it into the list again */
            Merge_Left=1;
        }
    }

    /* If we did not merge it with the left-side blocks, insert the original pointer's block 
     * into the TLSF table(Merging with the right-side one won't disturb this) */
    if(Merge_Left==0)
        _RMP_Mem_Ins(Pool, Mem_Head);
    else
        _RMP_Mem_Ins(Pool, Left_Head);
}
/* End Function:RMP_Free *****************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
