/******************************************************************************
Filename    : rmp_kernel.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The RMP RTOS single-file kernel.
              This is a single-process kernel that does nothing but simple 
              thread context switches. This operating system's kernel object
              allocation policy is totally exported, so the user assume full
              control over this.
              The white-box coverage of 100% of all kernel branches have been
              reached. Formal verification in progress.
              Use of 'volatile': we make every data structure volatile so all 
              memory reads and writes to potentially shared structures are 
              strongly ordered from a compiler perspective. This DOES decrease
              performance, but can survive smart LTOs without manually adding
              barriers. The kernel is written such that all frequently read data
              is a copy of the global data, so the performance loss is mild.
              Most accesses are cached and excessive reads are thus suppressed.
              Function naming conventions: user-program callable functions have
              no "_", while internal kernel-only functions begin with "_".
******************************************************************************/

/* Include *******************************************************************/
#define __HDR_DEF__
#include "rmp_platform.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "rmp_platform.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Kernel/rmp_kernel.h"

#define __HDR_PUBLIC__
#include "rmp_platform.h"
#undef __HDR_PUBLIC__
/* End Include ***************************************************************/

/* Function:RMP_Int_Print *****************************************************
Description : Print a signed integer on the debugging console. This integer is
              printed as decimal with sign.
Input       : rmp_cnt_t Int - The integer to print.
Output      : None.
Return      : rmp_cnt_t - The length of the string printed.
******************************************************************************/
#if(RMP_DBGLOG_ENABLE!=0U)
rmp_cnt_t RMP_Int_Print(rmp_cnt_t Int)
{
    rmp_cnt_t Num;
    rmp_cnt_t Abs;
    rmp_cnt_t Iter;
    rmp_cnt_t Count;
    rmp_cnt_t Div;
    
    /* Exit on zero */
    if(Int==0)
    {
        RMP_COV_MARKER();
        
        RMP_Putchar('0');
        return 1;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }

    /* Correct all negatives into positives */
    if(Int<0)
    {
        RMP_COV_MARKER();
        
        RMP_Putchar('-');
        Abs=-Int;
        Num=1;
    }
    else
    {
        RMP_COV_MARKER();
        
        Abs=Int;
        Num=0;
    }

    /* How many digits are there? */
    Count=0;
    Div=1;
    Iter=Abs;
    while(1)
    {
        Iter/=10;
        Count++;
        if(Iter!=0)
        {
            RMP_COV_MARKER();
            
            Div*=10;
        }
        else
        {
            RMP_COV_MARKER();
            
            break;
        }
    }
    Num+=Count;

    /* Print the integer */
    Iter=Abs;
    while(Count>0)
    {
        Count--;
        RMP_Putchar((rmp_s8_t)(Iter/Div)+'0');
        Iter=Iter%Div;
        Div/=10;
    }
    
    return Num;
}
#endif
/* End Function:RMP_Int_Print ************************************************/

/* Function:RMP_Hex_Print *****************************************************
Description : Print a unsigned integer on the debugging console. This integer is
              printed as hexadecimal.
Input       : rmp_ptr_t Uint - The unsigned integer to print.
Output      : None.
Return      : rmp_cnt_t - The length of the string printed.
******************************************************************************/
#if(RMP_DBGLOG_ENABLE!=0U)
rmp_cnt_t RMP_Hex_Print(rmp_ptr_t Uint)
{
    rmp_ptr_t Iter;
    rmp_ptr_t Count;
    rmp_ptr_t Num;

    /* Exit on zero */
    if(Uint==0U)
    {
        RMP_COV_MARKER();
        
        RMP_Putchar('0');
        return 1;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }

    /* Filter out all the leading zeroes */
    Count=0U;
    Iter=Uint;
    while((Iter>>(RMP_WORD_BIT-4U))==0U)
    {
        Iter<<=4U;
        Count++;
    }

    /* Count is the number of pts to print */
    Count=RMP_POW2(RMP_WORD_ORDER-2U)-Count;
    Num=Count;
    while(Count>0U)
    {
        Count--;
        Iter=(Uint>>(Count<<2U))&0x0FU;
        if(Iter<10U)
        {
            RMP_COV_MARKER();
            
            RMP_Putchar((rmp_s8_t)Iter+'0');
        }
        else
        {
            RMP_COV_MARKER();
            
            RMP_Putchar((rmp_s8_t)Iter+'A'-10);
        }
    }
    
    return (rmp_cnt_t)Num;
}
#endif
/* End Function:RMP_Hex_Print ************************************************/

/* Function:RMP_Str_Print *****************************************************
Description : Print a string on the debugging console.
Input       : const rmp_s8_t* String - The string to print.
Output      : None.
Return      : rmp_cnt_t - The length of the string printed, the '\0' is not included.
******************************************************************************/
#if(RMP_DBGLOG_ENABLE!=0U)
rmp_cnt_t RMP_Str_Print(const rmp_s8_t* String)
{
    rmp_ptr_t Count;
    
    for(Count=0U;Count<RMP_DBGLOG_MAX;Count++)
    {
        if(String[Count]==(rmp_s8_t)'\0')
        {
            RMP_COV_MARKER();
            
            break;
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
        
        RMP_Putchar(String[Count]);
    }
    
    return (rmp_cnt_t)Count;
}
#endif
/* End Function:RMP_Str_Print ************************************************/

/* Function:RMP_Log ***********************************************************
Description : Default logging function, will be used when the user does not 
              supply one. This will only be called when the kernel panics.
Input       : const char* File - The filename.
              long Line - The line number.
              const char* Date - The compilation date.
              const char* Time - The compilation time.
Output      : None.
Return      : None.
******************************************************************************/
#ifndef RMP_LOG
void RMP_Log(const char* File,
             long Line,
             const char* Date,
             const char* Time)
{
    RMP_DBG_S("\r\n***\r\nKernel panic - not syncing :\r\n"); \
    RMP_DBG_S(File); \
    RMP_DBG_S(" , Line "); \
    RMP_DBG_I(Line); \
    RMP_DBG_S("\r\n"); \
    RMP_DBG_S(Date); \
    RMP_DBG_S(" , "); \
    RMP_DBG_S(Time); \
    RMP_DBG_S("\r\n"); \
}
#endif
/* End Function:RMP_Log ******************************************************/

/* Function:RMP_Cov_Print *****************************************************
Description : The coverage data printer. Should always be disabled for all cases
              except where a kernel coverage test is needed. This should never
              be called any any user application; for coverage testing only.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
#ifdef RMP_COV_LINE_NUM
void RMP_Cov_Print(void)
{
    rmp_ptr_t Count;
    rmp_ptr_t Next;
    
    Next=0U;
    for(Count=0U;Count<RMP_COV_LINE_NUM;Count++)
    {
        if(RMP_BITMAP_IS_SET(RMP_Cov,Count))
        {
            RMP_COV_MARKER();
            
            RMP_DBG_I(Count);
            RMP_DBG_S(",");
            
            /* We put 12 markers on a single line */
            Next++;
            if(Next>11U)
            {
                RMP_COV_MARKER();
                
                Next=0U;
                RMP_DBG_S("\r\n");
            }
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
    }
}
#endif
/* End Function:RMP_Cov_Print ************************************************/

/* Function:RMP_Clear *********************************************************
Description : Memset a memory area to zero.
Input       : volatile void* Addr - The address to clear.
              rmp_ptr_t Size - The size to clear.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Clear(volatile void* Addr,
               rmp_ptr_t Size)
{
    rmp_ptr_t Count;
   
    for(Count=0U;Count<Size;Count++)
    {
        ((volatile rmp_u8_t*)Addr)[Count]=0U;
    }
}
/* End Function:RMP_Clear ****************************************************/

/* Function:RMP_MSB_Generic ***************************************************
Description : Find the MSB's position. This is a portable solution for all
              processors; if your processor does not have fast built-in bit
              manipulation support, you can resort to this when porting.
Input       : rmp_ptr_t Value - The value to compute for.
Output      : None.
Return      : rmp_ptr_t - The result. 0 will be returned for 0.
******************************************************************************/
rmp_ptr_t RMP_MSB_Generic(rmp_ptr_t Value)
{
    rmp_ptr_t Bit;
    static const rmp_u8_t Table[256U]=
    {
        0U,0U,1U,1U,2U,2U,2U,2U,3U,3U,3U,3U,3U,3U,3U,3U,
        4U,4U,4U,4U,4U,4U,4U,4U,4U,4U,4U,4U,4U,4U,4U,4U,
        5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,
        5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,
        6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,
        6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,
        6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,
        6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,
        7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,
        7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,
        7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,
        7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,
        7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,
        7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,
        7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,
        7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U
    };

#if(RMP_WORD_ORDER==4U)
    /* 15-8 */
    if(Value>=RMP_POW2(8U))
    {
        RMP_COV_MARKER();
    
        Bit=8U;
    }
    /* 7-0 */
    else
    {
        RMP_COV_MARKER();
    
        Bit=0U;
    }
#elif(RMP_WORD_ORDER==5U)
    /* 31-16 */
    if(Value>=RMP_POW2(16U))
    {
        RMP_COV_MARKER();
        
        /* 31-24 */
        if(Value>=RMP_POW2(24U))
        {
            RMP_COV_MARKER();
            
            Bit=24U;
        }
        /* 24-16 */
        else
        {
            RMP_COV_MARKER();
            
            Bit=16U;
        }
    }
    /* 15-0 */
    else
    {
        RMP_COV_MARKER();
        
        /* 15-8 */
        if(Value>=RMP_POW2(8U))
        {
            RMP_COV_MARKER();
            
            Bit=8U;
        }
        /* 7-0 */
        else
        {
            RMP_COV_MARKER();
            
            Bit=0U;
        }
    }
#elif(RMP_WORD_ORDER==6U)
    /* 63-32 */
    if(Value>=RMP_POW2(32U))
    {
        RMP_COV_MARKER();
    
        /* 63-48 */
        if(Value>=RMP_POW2(48U))
        {
            RMP_COV_MARKER();
    
            /* 63-56 */
            if(Value>=RMP_POW2(56U))
            {
                RMP_COV_MARKER();
    
                Bit=56U;
            }
            /* 56-48 */
            else
            {
                RMP_COV_MARKER();
                
                Bit=48U;
            }
        }
        /* 47-32 */
        else
        {
            RMP_COV_MARKER();
            
            /* 47-40 */
            if(Value>=RMP_POW2(40U))
            {
                RMP_COV_MARKER();
                
                Bit=40U;
            }
            /* 39-32 */
            else
            {
                RMP_COV_MARKER();
                
                Bit=32U;
            }
        }
    }
    /* 31-0 */
    else
    {
        RMP_COV_MARKER();
        
        /* 31-16 */
        if(Value>=RMP_POW2(16U))
        {
            RMP_COV_MARKER();
            
            /* 31-24 */
            if(Value>=RMP_POW2(24U))
            {
                RMP_COV_MARKER();
                
                Bit=24U;
            }
            /* 24-16 */
            else
            {
                RMP_COV_MARKER();
                
                Bit=16U;
            }
        }
        /* 15-0 */
        else
        {
            RMP_COV_MARKER();
            
            /* 15-8 */
            if(Value>=RMP_POW2(8U))
            {
                RMP_COV_MARKER();
                
                Bit=8U;
            }
            /* 7-0 */
            else
            {
                RMP_COV_MARKER();
                
                Bit=0U;
            }
        }
    }
#else
#error RMP : Generic MSB for 128-bits & above are not implemented.
#endif

    return Table[Value>>Bit]+Bit;
}
/* End Function:RMP_MSB_Generic **********************************************/

/* Function:RMP_LSB_Generic ***************************************************
Description : Find the LSB's position. This is a portable solution for all
              processors; if your processor does not have fast built-in bit
              manipulation support, you can resort to this when porting.
Input       : rmp_ptr_t Value - The value to count.
Output      : None.
Return      : rmp_ptr_t - The result. 0 will be returned for 0.
******************************************************************************/
rmp_ptr_t RMP_LSB_Generic(rmp_ptr_t Value)
{
    rmp_ptr_t Bit;
    static const rmp_u8_t Table[256U]=
    {
        0U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        4U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        5U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        4U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        6U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        4U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        5U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        4U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        7U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        4U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        5U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        4U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        6U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        4U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        5U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        4U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U
    };
    
#if(RMP_WORD_ORDER==4U)
    /* 16-8 */
    if((Value<<8U)==0U)
    {
        RMP_COV_MARKER();
    
        Bit=8U;
    }
    /* 7-0 */
    else
    {
        RMP_COV_MARKER();
    
        Bit=0U;
    }
#elif(RMP_WORD_ORDER==5U)
    /* 31-16 */
    if((Value<<16U)==0U)
    {
        RMP_COV_MARKER();
        
        /* 31-24 */
        if((Value<<8U)==0U)
        {
            RMP_COV_MARKER();
            
            Bit=24U;
        }
        /* 24-16 */
        else
        {
            RMP_COV_MARKER();
            
            Bit=16U;
        }
    }
    /* 15-0 */
    else
    {
        RMP_COV_MARKER();
        
        /* 15-8 */
        if((Value<<24U)==0U)
        {
            RMP_COV_MARKER();
            
            Bit=8U;
        }
        /* 7-0 */
        else
        {
            RMP_COV_MARKER();
            
            Bit=0U;
        }
    }
#elif(RMP_WORD_ORDER==6U)
    /* 63-32 */
    if((Value<<32U)==0U)
    {
        RMP_COV_MARKER();
    
        /* 63-48 */
        if((Value<<16U)==0U)
        {
            RMP_COV_MARKER();
    
            /* 63-56 */
            if((Value<<8U)==0U)
            {
                RMP_COV_MARKER();
    
                Bit=56U;
            }
            /* 56-48 */
            else
            {
                RMP_COV_MARKER();
                
                Bit=48U;
            }
        }
        /* 47-32 */
        else
        {
            RMP_COV_MARKER();
            
            /* 47-40 */
            if((Value<<24U)==0U)
            {
                RMP_COV_MARKER();
                
                Bit=40U;
            }
            /* 39-32 */
            else
            {
                RMP_COV_MARKER();
                
                Bit=32U;
            }
        }
    }
    /* 31-0 */
    else
    {
        RMP_COV_MARKER();
        
        /* 31-16 */
        if((Value<<48U)==0U)
        {
            RMP_COV_MARKER();
            
            /* 31-24 */
            if((Value<<40U)==0U)
            {
                RMP_COV_MARKER();
                
                Bit=24U;
            }
            /* 24-16 */
            else
            {
                RMP_COV_MARKER();
                
                Bit=16U;
            }
        }
        /* 15-0 */
        else
        {
            RMP_COV_MARKER();
            
            /* 15-8 */
            if((Value<<56U)==0U)
            {
                RMP_COV_MARKER();
                
                Bit=8U;
            }
            /* 7-0 */
            else
            {
                RMP_COV_MARKER();
                
                Bit=0U;
            }
        }
    }
#else
#error RMP : Generic LSB for 128-bits & above are not implemented.
#endif

    return Table[(rmp_u8_t)(Value>>Bit)]+Bit;
}
/* End Function:RMP_LSB_Generic **********************************************/

/* Function:RMP_List_Crt ******************************************************
Description : Create a doubly linkled list.
Input       : volatile struct RMP_List* Head - The pointer to the list head.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=Head;
    Head->Next=Head;
}
/* End Function:RMP_List_Crt *************************************************/

/* Function:RMP_List_Del ******************************************************
Description : Delete a node from the doubly-linked list.
Input       : volatile struct RMP_List* Prev - The prevoius node of the target node.
              volatile struct RMP_List* Next - The next node of the target node.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=Prev;
    Prev->Next=Next;
}
/* End Function:RMP_List_Del *************************************************/

/* Function:RMP_List_Ins ******************************************************
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
    Next->Prev=New;
    New->Next=Next;
    New->Prev=Prev;
    Prev->Next=New;
}
/* End Function:RMP_List_Ins *************************************************/

/* Function:RMP_Sched_Lock ****************************************************
Description : The function locks the scheduler. The locking can be stacked.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Sched_Lock(void)
{
    RMP_INT_MASK();
    RMP_Sched_Lock_Cnt++;
}
/* End Function:RMP_Sched_Lock ***********************************************/

/* Function:RMP_Sched_Unlock **************************************************
Description : The function unlocks the scheduler. The unlocking can be stacked.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Sched_Unlock(void)
{
    if(RMP_Sched_Lock_Cnt==1U)
    {
        RMP_COV_MARKER();
        
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0U;
        /* Deal with pending scheduler activations */
        if(RMP_Sched_Pend!=0U)
        {
            RMP_COV_MARKER();
            
             /* Context switch will clear the pend flag */
            RMP_YIELD();
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
        
        /* Calling RMP_YIELD before truly unmasking interrupts is allowed:
         * (1) We're using a dedicated interrupt handler to switch context.
         *     In this case, the software interrupt will be pending until we 
         *     unmask the kernel-aware interrupts here.
         * (2) We're using a routine to do context switch.
         *     In this case, the routine will unmask interrupts at its end 
         *     anyway, and will not leave the interrupt masked when it is 
         *     exiting, and unmasking it again here does no harm. 
         * We don't want to absorb RMP_INT_UNMASK into RMP_YIELD due to 
         * possible complications of the programming mental model, and
         * we want to keep all invariants as simple as possible. */
        RMP_INT_UNMASK();
    }
    else if(RMP_Sched_Lock_Cnt>1U)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Lock_Cnt--;
    }
    /* Trying to unlock a scheduler that is not locked - should never happen */
    else
    {
        RMP_ASSERT(0);
        /* No action required */
    }
}
/* End Function:RMP_Sched_Unlock *********************************************/

/* Function:_RMP_Tim_Proc *****************************************************
Description : Process RMP timer events.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
static void _RMP_Tim_Proc(void)
{
    rmp_ptr_t State;
    rmp_ptr_t Pure;
    rmp_ptr_t Diff;
    volatile struct RMP_Thd* Thread;
    
    /* Process the timer events, if there are any of them */
    while((&RMP_Delay)!=RMP_Delay.Next)
    {
        Thread=RMP_DLY2THD(RMP_Delay.Next);
        
        /* This thread is overflown */
        Diff=RMP_DLY_DIFF(Thread->Timeout);
        if(RMP_DIFF_OVF(Diff))
        {
            RMP_COV_MARKER();
            
            /* Remove it from delay queue */
            RMP_List_Del(Thread->Dly_Head.Prev, Thread->Dly_Head.Next);
            /* Cache volatile thread state */
            State=Thread->State;
            /* Extract pure state */
            Pure=RMP_THD_STATE(State);
            
            /* See what state the thread is in */
            if((Pure==RMP_THD_SNDDLY)||(Pure==RMP_THD_SEMDLY))
            {
                RMP_COV_MARKER();
                
                RMP_List_Del(Thread->Run_Head.Prev, Thread->Run_Head.Next);
                /* Supply timeout error code */
                Thread->Retval=RMP_ERR_OPER;
            }
            else if(Pure==RMP_THD_RCVDLY)
            {
                RMP_COV_MARKER();
                
                /* Supply timeout error code */
                Thread->Retval=RMP_ERR_OPER;
            }
            else if(Pure==RMP_THD_DELAY)
            {
                RMP_COV_MARKER();
                
                /* No action required */
            }
            /* Impossible: thread in the waiting list but not in delay */
            else
            {
                RMP_ASSERT(0);
            }

            /* Set to ready if not suspended */
            RMP_THD_STATE_SET(State,RMP_THD_READY);
            /* Put cached thread state back */
            Thread->State=State;
            /* Insert into runqueue if not suspended */
            _RMP_Run_Ins(Thread,State);
        }
        /* Stop when we find a timer that is not overflown */
        else
        {
            RMP_COV_MARKER();
            
            break;
        }
    }
}
/* Function:_RMP_Tim_Proc ****************************************************/

/* Function:_RMP_Run_High *****************************************************
Description : Get the highest priority thread that is ready. The return value
              will be written into the global variables. Note that providing a
              fast path where no context operations happen when the new thread
              equal the old thread has little value in RMP, because (1) context
              switches are rarely idempotent because threads prefer to block
              themselves rather than to exhaust slices, (2) such design trades
              worst-case latency for average performance, (3) such design 
              complicates the programmer mental model for using hooks. If such
              saving/restoring of coprocessor context is deemed to inefficient,
              the implementer is free to implement lazy switching.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Run_High(void)
{
    rmp_cnt_t Word;
    rmp_ptr_t Prio;
    volatile struct RMP_Thd* Thd_Cur;
    
    /* Save potential extra context */
#ifdef RMP_CTX_SAVE
    RMP_CTX_SAVE();
#endif
    
    /* Cache volatile current thread */
    Thd_Cur=RMP_Thd_Cur;
    
    /* Save SS/SP value to the old thread structure */
#if(RMP_STKSEG_ENABLE!=0U)
    Thd_Cur->Segment=RMP_SS_Cur;
#endif
    Thd_Cur->Stack=RMP_SP_Cur;
    
    /* No need to detect scheduler locks - if this function can be called,
     * the scheduler can't be locked, and after we choose the highest
     * priority thread the scheduler pending flag should be cleared */
    RMP_Sched_Pend=0U;
    /* See which one is ready, and pick it */
    Prio=RMP_PRIO_WORD_NUM-1U;
    for(Word=(rmp_cnt_t)Prio;Word>=0;Word--)
    {
        if(RMP_Bitmap[Word]!=0U)
        {
            RMP_COV_MARKER();
            
            break;
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
    }

    /* There must be one, at least the initial thread must be ready at all moments! */
    RMP_ASSERT(Word>=0);
    Prio=(rmp_ptr_t)Word;
    Prio=RMP_MSB_GET(RMP_Bitmap[Prio])+(Prio<<RMP_WORD_ORDER);

    /* See if the current thread is the highest priority one.
     * If yes, place the current at the end of the queue. */
    if(Thd_Cur==(volatile struct RMP_Thd*)(RMP_Run[Prio].Next))
    {
        RMP_COV_MARKER();
        
        RMP_ASSERT(Thd_Cur->Prio==Prio);
        RMP_List_Del(Thd_Cur->Run_Head.Prev,Thd_Cur->Run_Head.Next);
        RMP_List_Ins(&(Thd_Cur->Run_Head),
                     RMP_Run[Prio].Prev,
                     &(RMP_Run[Prio]));
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Replenish timeslices for the old thread and switch to the new one */
    Thd_Cur->Slice_Left=Thd_Cur->Slice;
    Thd_Cur=(volatile struct RMP_Thd*)(RMP_Run[Prio].Next);

    /* Restore SS/SP value from the new thread structure */
#if(RMP_STKSEG_ENABLE!=0U)
    RMP_SS_Cur=Thd_Cur->Segment;
#endif
    RMP_SP_Cur=Thd_Cur->Stack;
    
    /* Write cached current thread back */
    RMP_Thd_Cur=Thd_Cur;
    
    /* Load potential extra context and do scheduler hook */
#ifdef RMP_CTX_LOAD
    RMP_CTX_LOAD();
#endif
#ifdef RMP_SCHED_HOOK
    RMP_SCHED_HOOK();
#endif
}
/* End Function:_RMP_Run_High ************************************************/

/* Function:_RMP_Tim_Handler **************************************************
Description : The system tick timer interrupt routine.
Input       : rmp_ptr_t Slice - How many timeslices have passed.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Tim_Handler(rmp_ptr_t Slice)
{
    rmp_ptr_t Diff;
    volatile struct RMP_Thd* Thread;
    
    /* Increase the timestamp as always */
    RMP_Timestamp+=Slice;
    
    /* Cache volatile current thread */
    Thread=RMP_Thd_Cur;
    
    /* See if the current thread expired; if yes, trigger a scheduler event */
    if(Slice>=Thread->Slice_Left)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Pend=1U;
    }
    else
    {
        RMP_COV_MARKER();
        
        Thread->Slice_Left-=Slice;
    }
    
    /* Check if there are any timer events */
    if((&RMP_Delay)!=RMP_Delay.Next)
    {
        RMP_COV_MARKER();
        
        Thread=RMP_DLY2THD(RMP_Delay.Next);
        /* If there are overflows, process all pending timers */
        Diff=RMP_DLY_DIFF(Thread->Timeout);
        if(RMP_DIFF_OVF(Diff))
        {
            RMP_COV_MARKER();
            
            /* No need to care about scheduler locks if this interrupt can be entered
             * - we have disabled timer and scheduler interrupts in scheduler lock */
            _RMP_Tim_Proc();
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Trigger a context switch if required */
#ifdef RMP_YIELD_ISR
    if(RMP_Sched_Pend!=0U)
    {
        RMP_COV_MARKER();
        
        /* Context switch will clear the pend flag */
        RMP_YIELD_ISR();
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
#ifdef RMP_TIM_HOOK
    RMP_TIM_HOOK(Slice);
#endif
}
/* End Function:_RMP_Tim_Handler *********************************************/

/* Function:_RMP_Tim_Elapse ***************************************************
Description : Honor the elapse of time from the last timer firing. This is to be
              called before all potential context switch points to correctly 
              account for the time elapsed before a context switch.
              If a tickless kernel is not desired, this function can be ignored.
Input       : rmp_ptr_t Slice - Number of slices passed since last call of
                                _RMP_Tim_Elapse or _RMP_Tim_Handler.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Tim_Elapse(rmp_ptr_t Slice)
{
    /* Increase the timestamp as always */
    RMP_Timestamp+=Slice;

    /* When calculating the next timer interrupt with _RMP_Tim_Future
     * in context switches triggered by means other than timer interrupts, and 
     * when inserting threads in to the delay queue in _RMP_Dly_Ins, we need to
     * update RMP_Timestamp to the latest value manually. This is where
     * _RMP_Tim_Elapse kicks in, and why it must be called before these uses. 
     * It is not required to call this immediately on context switch entry 
     * (which is the required method for RME), but doing so is acceptable. */

    /* When a context switch is pending, there's no need to account for the 
     * timeslice of the current thread anymore, because its timeslice is always
     * replenished immediately. Note that this is VERY different from RME which 
     * needs user-level budget replenishments.
     * That said, between context switches, if we can guarantee that the elapsed
     * time is always tracked exclusively by _RMP_Tim_Handler which accounts for
     * all timeslice losses, and we can load whatever is left into the hardware
     * timer without losing track of timeslices. 
     * We note that there are two usecases for _RMP_Tim_Elapse: (1) in context
     * switch hooks and (2) in delay hooks, and both will always follow a 
     * context switch. Thus the optimization is valid. */
}
/* End Function:_RMP_Tim_Elapse **********************************************/

/* Function:_RMP_Tim_Future ***************************************************
Description : Get the nearest timer interrupt arrival time. This is used to set
              the timer after a context switch or a timer interrupt. If a 
              tickless kernel is not desired, this function can be ignored.
Input       : None.
Output      : None.
Return      : rmp_ptr_t - How many slices to program until the next timeout.
******************************************************************************/
rmp_ptr_t _RMP_Tim_Future(void)
{
    rmp_ptr_t Diff;
    rmp_ptr_t Value;
    volatile struct RMP_Thd* Thread;
    
    /* Cache current thread's timeout value */
    Value=RMP_Thd_Cur->Slice_Left;
    
    /* What is the nearest timer timeout value? */
    if(RMP_Delay.Next!=&RMP_Delay)
    {
        RMP_COV_MARKER();
        
        Thread=RMP_DLY2THD(RMP_Delay.Next);
        
        /* Detect possible overflows - trigger timer interrupt ASAP */
        Diff=RMP_DLY_DIFF(Thread->Timeout);
        if(RMP_DIFF_OVF(Diff))
        {
            RMP_COV_MARKER();
            
            Value=1U;
        }
        else if(Diff<Value)
        {
            RMP_COV_MARKER();
            
            Value=Diff;
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    return Value;
}
/* End Function:_RMP_Tim_Future **********************************************/

/* Function:_RMP_Tim_Idle *****************************************************
Description : See if the timer could be idle. When both conditions below are
              met, we could turn off all kernel clock sources altogether to
              achieve the lowest possible power:
              1. The only ready thread is the initial idle thread.
              2. There are no outstanding delay timers.
Input       : None.
Output      : None.
Return      : rmp_ret_t - If yes, 0; else -1.
******************************************************************************/
rmp_ret_t _RMP_Tim_Idle(void)
{
    /* Are there any threads other than the initial idle thread? */
    if(RMP_Run[0].Next!=((volatile struct RMP_List*)&RMP_Init_Thd))
    {
        RMP_COV_MARKER();
        
        return -1;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }

    if(RMP_Run[0].Next!=RMP_Run[0].Prev)
    {
        RMP_COV_MARKER();
        
        return -1;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }

    /* Are there any pending timers? */
    if(RMP_Delay.Next!=&RMP_Delay)
    {
        RMP_COV_MARKER();
        
        return -1;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }

    return 0;
}
/* End Function:_RMP_Tim_Idle ************************************************/

/* Function:_RMP_Run_Ins ******************************************************
Description : Set the thread as ready to schedule. That means, put the thread
              into the runqueue. When this is called, please make sure that
              the scheduler is locked.
              This function will also try to identify if this thread is currently 
              suspended. If yes, it will not be placed into the queue.
Input       : volatile struct RMP_Thd* Thread - The thread to put into the runqueue.
              rmp_ptr_t State - The cached thread state.
Output      : None.
Return      : None.
******************************************************************************/
static void _RMP_Run_Ins(volatile struct RMP_Thd* Thread,
                         rmp_ptr_t State)
{
    rmp_ptr_t Prio;
    
    /* No need to operate on suspended threads */
    if((State&RMP_THD_SUSPEND)==0U)
    {
        RMP_COV_MARKER();
        
        /* Cache volatile thread priority */
        Prio=Thread->Prio;
        
        /* Insert this into the corresponding runqueue's back */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Prio].Prev,&(RMP_Run[Prio]));
        /* Set this priority level as active */
        RMP_BITMAP_SET(RMP_Bitmap,Prio);
        
        /* Compare this with the current one to see if we need a context switch */
        if(Prio>RMP_Thd_Cur->Prio)
        {
            RMP_COV_MARKER();
            
            RMP_Sched_Pend=1U;
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
}
/* End Function:_RMP_Run_Ins *************************************************/

/* Function:_RMP_Run_Del ******************************************************
Description : Clear the thread from the runqueue. When this is called, please 
              make sure that the scheduler is locked. This function also checks whether
              the thread is suspended. If yes, it will not remove it from the queue.
Input       : volatile struct RMP_Thd* Thread - The thread to clear from the runqueue.
              rmp_ptr_t State - The cached thread state.
Output      : None.
Return      : None.
******************************************************************************/
static void _RMP_Run_Del(volatile struct RMP_Thd* Thread,
                         rmp_ptr_t State)
{
    rmp_ptr_t Prio;
    
    /* No need to operate on suspended threads */
    if((State&RMP_THD_SUSPEND)==0U)
    {
        RMP_COV_MARKER();
        
        /* See if it is the last thread on the priority level */
        if(Thread->Run_Head.Prev==Thread->Run_Head.Next)
        {
            RMP_COV_MARKER();
            
            /* Cache volatile thread priority */
            Prio=Thread->Prio;
            /* If yes, set the priority level as inactive */
            RMP_BITMAP_CLR(RMP_Bitmap,Prio);
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
        
        /* Delete this from the corresponding runqueue */
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
        
        /* If it is the current thread, request a context switch */
        if(Thread==RMP_Thd_Cur)
        {
            RMP_COV_MARKER();
            
            RMP_Sched_Pend=1U;
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
}
/* End Function:_RMP_Run_Del *************************************************/

/* Function:_RMP_Dly_Ins ******************************************************
Description : Insert the thread into the delay queue, given some timeslices into
              the future. The thread must not be in the run queue any more.
Input       : volatile struct RMP_Thd* Thread - The thread to put into the delay
                                                queue.
              rmp_ptr_t - The timeslices to delay.
Output      : None.
Return      : None.
******************************************************************************/
static void _RMP_Dly_Ins(volatile struct RMP_Thd* Thread,
                         rmp_ptr_t Slice)
{
    rmp_ptr_t Diff;
    volatile struct RMP_List* Trav_Ptr;
    volatile struct RMP_Thd* Trav_Thd;

    /* Potentially update the timestamp if we're tickless */
#ifdef RMP_DLY_HOOK
    RMP_DLY_HOOK(Slice);
#endif
    
    /* Find a place to insert this timer */
    Trav_Ptr=RMP_Delay.Next;
    while(Trav_Ptr!=&RMP_Delay)
    {
        Trav_Thd=RMP_DLY2THD(Trav_Ptr);
        
        /* Overflow possible due to bumpy timestamp updates in tickless kernel - 
         * we need to find one that is greater than us yet is not overflown */
        Diff=RMP_DLY_DIFF(Trav_Thd->Timeout);
        if(RMP_DIFF_OVF(Diff)||(Diff<Slice))
        {
            RMP_COV_MARKER();
            
            Trav_Ptr=Trav_Ptr->Next;
        }
        else
        {
            RMP_COV_MARKER();
            
            break;
        }
    }

    /* Insert this into the list */
    Thread->Timeout=RMP_Timestamp+Slice;
    RMP_List_Ins(&(Thread->Dly_Head),Trav_Ptr->Prev,Trav_Ptr);
}
/* End Function:_RMP_Dly_Ins *************************************************/

/* Function:RMP_Thd_Yield *****************************************************
Description : Yield to another thread.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Thd_Yield(void)
{
    /* Scheduler not locked, perform a schedule immediately */
    if(RMP_Sched_Lock_Cnt==0U)
    {
        RMP_COV_MARKER();
        
        RMP_YIELD();
    }
    /* Scheduler locked, have to pend the flag to schedule later */
    else
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Pend=1U;
    }
}
/* End Function:RMP_Thd_Yield ************************************************/

/* Function:RMP_Thd_Crt *******************************************************
Description : Create a real-time thread and put it into the runqueue.
Input       : volatile struct RMP_Thd* Thread - The pointer to the thread.
              void* Entry - The entry of the thread.
              void* Param - The argument to pass to the thread.
              rmp_ptr_t Segment - The stack segment of this thread.
              void* Stack - The stack base of this thread.
              rmp_ptr_t Size - The stack size of this thread.
              rmp_ptr_t Prio - The priority of the thread.
              rmp_ptr_t Slice - The number of timeslices to assign to the thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0. on error, return an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Crt(volatile struct RMP_Thd* Thread,
                      void* Entry,
                      void* Param,
#if(RMP_STKSEG_ENABLE!=0U)
                      rmp_ptr_t Segment,
#endif
                      void* Stack,
                      rmp_ptr_t Size,
                      rmp_ptr_t Prio,
                      rmp_ptr_t Slice)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the priority is valid */
    if(Prio>=RMP_PREEMPT_PRIO_NUM)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_PRIO;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the slice is valid */
    if((Slice==0U)||(Slice>=RMP_SLICE_MAX))
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_SLICE;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the stack is valid - must not be NULL and have at least the context size */
    if((Stack==RMP_NULL)||(Size<=sizeof(RMP_STACK_STRUCT)))
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_STACK;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the thread pointer is valid */
    if(Thread==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif

    RMP_Sched_Lock();
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the thread structure is in use */
    if(RMP_THD_STATE(Thread->State)!=RMP_THD_FREE)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Initialize thread basic info */
    Thread->Prio=Prio;
    Thread->Slice=Slice;
    Thread->Slice_Left=Slice;
    RMP_List_Crt(&(Thread->Snd_List));
    
    /* Initialize thread stack */
#if(RMP_STKSEG_ENABLE!=0U)
    Thread->Segment=Segment;
    Thread->Stack=_RMP_Stack_Init(Segment,(rmp_ptr_t)Stack,Size, 
                                  (rmp_ptr_t)Entry,(rmp_ptr_t)Param);
#else
    Thread->Stack=_RMP_Stack_Init((rmp_ptr_t)Stack,Size, 
                                  (rmp_ptr_t)Entry,(rmp_ptr_t)Param);
#endif
    
    /* Thread is always set to ready on creation */
    Thread->State=RMP_THD_READY;
    /* Insert into runqueue - must be not suspended */
    _RMP_Run_Ins(Thread,RMP_THD_READY);
    
    RMP_Sched_Unlock();

    return 0;
}
/* End Function:RMP_Thd_Crt **************************************************/

/* Function:_RMP_Thd_Remove ***************************************************
Description : Remove a thread from the waitlist, and put it back to the ready
              list if it was not suspended.
Input       : volatile struct RMP_Thd* Thread - The pointer to the thread.
              rmp_ptr_t Delay_Queue - The delay queue type, can be one of
                                      RMP_THD_SNDDLY or RMP_THD_SEMDLY.
Output      : None.
Return      : None.
******************************************************************************/
static void _RMP_Thd_Remove(volatile struct RMP_Thd* Thread,
                            rmp_ptr_t Delay_Queue)
{
    rmp_ptr_t State;
    
    /* Remove from the mailbox queue */
    RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
    /* Cache volatile thread state */
    State=Thread->State;
    
    /* Remove from delay queue if it was a timed delay */
    if(RMP_THD_STATE(State)==Delay_Queue)
    {
        RMP_COV_MARKER();
        
        RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Set to ready */
    RMP_THD_STATE_SET(State,RMP_THD_READY);
    /* Put cached thread state back */
    Thread->State=State;
    /* Insert into runqueue if not suspended */
    _RMP_Run_Ins(Thread,State);
}
/* End Function:_RMP_Thd_Remove **********************************************/

/* Function:RMP_Thd_Del *******************************************************
Description : Delete a real-time thread.
Input       : volatile struct RMP_Thd* Thread - The pointer to the thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Del(volatile struct RMP_Thd* Thread)
{
    rmp_ptr_t State;
    rmp_ptr_t Pure;
    volatile struct RMP_Thd* Wait;

#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the thread pointer is valid */
    if(Thread==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
    /* Cache volatile thread state */
    State=Thread->State;
    /* Extract pure state */
    Pure=RMP_THD_STATE(State);
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the thread structure is in use */
    if(Pure==RMP_THD_FREE)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* See if anyone waiting to send to this thread */
    while(&(Thread->Snd_List)!=Thread->Snd_List.Next)
    {
        Wait=(volatile struct RMP_Thd*)(Thread->Snd_List.Next);
        /* Remove from mailbox queue */
        _RMP_Thd_Remove(Wait,RMP_THD_SNDDLY);
        /* Supply abort error code */
        Wait->Retval=RMP_ERR_OPER;
    }
    
    /* Clear ready if not suspended */
    if(Pure==RMP_THD_READY)
    {
        RMP_COV_MARKER();
        
        _RMP_Run_Del(Thread,State);
    }
    /* Do nothing if it is just blocked on receive */
    else if(Pure==RMP_THD_RCVBLK)
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    /* Unblock it if it was blocked on other stuff */
    else if((Pure==RMP_THD_SNDBLK)||(Pure==RMP_THD_SEMBLK))
    {
        RMP_COV_MARKER();
        
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
    }
    else if((Pure==RMP_THD_RCVDLY)||(Pure==RMP_THD_DELAY))
    {
        RMP_COV_MARKER();
        
        RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
    }
    else if((Pure==RMP_THD_SNDDLY)||(Pure==RMP_THD_SEMDLY))
    {
        RMP_COV_MARKER();
        
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
        RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
    }
    /* Invalid state */
    else
    {
        RMP_ASSERT(0);
    }

    /* Set return value to abort anyway, and free the structure */
    Thread->Retval=RMP_ERR_OPER;
    Thread->State=RMP_THD_FREE;
    
    /* If we are deleting ourself, a schedule must be pending at this point */
    RMP_ASSERT((Thread!=RMP_Thd_Cur)||(RMP_Sched_Pend!=0U));
    
    RMP_Sched_Unlock();
    
    /* Can't reach here if we're deleting ourself */
    RMP_ASSERT(Thread!=RMP_Thd_Cur);

    return 0;
}
/* End Function:RMP_Thd_Del **************************************************/

/* Function:RMP_Thd_Set *******************************************************
Description : Change the priority or timeslice of a real-time thread. If one of
              the changes is not desired, just leave it to RMP_PREEMPT_PRIO_NUM
              or RMP_SLICE_MAX.
Input       : volatile struct RMP_Thd* Thread - The pointer to the thread.
              rmp_ptr_t Prio - The priority of the thread.
              rmp_ptr_t Slice - The new timeslice value for this thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0; else error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Set(volatile struct RMP_Thd* Thread,
                      rmp_ptr_t Prio,
                      rmp_ptr_t Slice)
{
    rmp_ptr_t State;
    rmp_ptr_t Pure;
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the thread pointer is valid */
    if(Thread==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the slice is valid */
    if(Slice==0U)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_SLICE;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
    /* Cache volatile thread state */
    State=Thread->State;
    /* Extract pure state */
    Pure=RMP_THD_STATE(State);
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the thread structure is in use */
    if(Pure==RMP_THD_FREE)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* See if the thread is in ready state */
    if(Pure==RMP_THD_READY)
    {
        RMP_COV_MARKER();
        
        /* See if we are gonna change one of it or both */
        if(Prio<RMP_PREEMPT_PRIO_NUM)
        {
            RMP_COV_MARKER();
            
            if(Thread->Prio!=Prio)
            {
                RMP_COV_MARKER();
                
                /* Delete from runqueue if not suspended */
                _RMP_Run_Del(Thread,State);
                /* Change priority */
                Thread->Prio=Prio;
                /* Insert into runqueue if not suspended */
                _RMP_Run_Ins(Thread,State);
            }
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
        
        if(Slice<RMP_SLICE_MAX)
        {
            RMP_COV_MARKER();
            
            Thread->Slice=Slice;
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
    }
    else
    {
        RMP_COV_MARKER();
        
        if(Prio<RMP_PREEMPT_PRIO_NUM)
        {
            RMP_COV_MARKER();
            
            Thread->Prio=Prio;
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
        
        if(Slice<RMP_SLICE_MAX)
        {
            RMP_COV_MARKER();
            
            Thread->Slice=Slice;
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
    }
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Thd_Set **************************************************/

/* Function:RMP_Thd_Suspend ***************************************************
Description : Suspend the execution of a real-time thread.
Input       : volatile struct RMP_Thd* Thread - The pointer to the thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0; else error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Suspend(volatile struct RMP_Thd* Thread)
{
    rmp_ptr_t State;
    rmp_ptr_t Pure;
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the thread pointer is valid */
    if(Thread==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
    /* Cache volatile thread state */
    State=Thread->State;
    /* Extract pure state */
    Pure=RMP_THD_STATE(State);
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the thread structure is in use */
    if(Pure==RMP_THD_FREE)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Check if the thread is already suspended */
    if((State&RMP_THD_SUSPEND)!=0U)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_STATE;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Remove the thread from ready queue if it was ready */
    if(Pure==RMP_THD_READY)
    {
        RMP_COV_MARKER();
        
        /* Delete from runqueue - must be not suspended */
        _RMP_Run_Del(Thread,State);
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Mark it as suspended and put cached thread state back */
    Thread->State=State|RMP_THD_SUSPEND;
    
    /* If we are suspending ourself, a schedule must be pending at this point */
    RMP_ASSERT((Thread!=RMP_Thd_Cur)||(RMP_Sched_Pend!=0U));
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Thd_Suspend **********************************************/

/* Function:RMP_Thd_Resume ****************************************************
Description : Resume the execution of a real-time thread.
Input       : volatile struct RMP_Thd* Thread - The pointer to the thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0; else error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Resume(volatile struct RMP_Thd* Thread)
{
    rmp_ptr_t State;
    rmp_ptr_t Pure;
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the thread pointer is valid */
    if(Thread==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
    /* Cache volatile thread state */
    State=Thread->State;
    /* Extract pure state */
    Pure=RMP_THD_STATE(State);
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the thread structure is in use */
    if(Pure==RMP_THD_FREE)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Check if the thread is already suspended */
    if((State&RMP_THD_SUSPEND)==0U)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_STATE;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Resume the thread and put cached thread state back */
    State&=~RMP_THD_SUSPEND;
    Thread->State=State;
    
    /* Put the thread back if it was ready */
    if(Pure==RMP_THD_READY)
    {
        RMP_COV_MARKER();
        
        /* Insert into runqueue - must be not suspended */
        _RMP_Run_Ins(Thread,State);
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Thd_Resume ***********************************************/

/* Function:RMP_Thd_Delay *****************************************************
Description : Delay the execution of a real-time thread.
Input       : rmp_ptr_t Slice - The number of timeslices to delay.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Delay(rmp_ptr_t Slice)
{
    rmp_ptr_t State;
    volatile struct RMP_Thd* Thd_Cur;

#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the slice is valid */
    if((Slice==0U)||(Slice>=RMP_SLICE_MAX))
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_SLICE;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
    /* Cache volatile current thread - must be own thread */
    Thd_Cur=RMP_Thd_Cur;
    /* Cache volatile thread state */
    State=Thd_Cur->State;

    /* We must be ready and not suspended so we will be in ready queue */
    _RMP_Run_Del(Thd_Cur,State);
    /* Set to delayed */
    RMP_THD_STATE_SET(State,RMP_THD_DELAY);
    /* Put cached thread state back */
    Thd_Cur->State=State;
    /* Insert into delay queue */
    _RMP_Dly_Ins(Thd_Cur,Slice);

    /* When abort, an error code will be supplied instead */
    Thd_Cur->Retval=0;
    RMP_Sched_Unlock();
    
    /* Retval might be updated */
    return Thd_Cur->Retval;
}
/* End Function:RMP_Thd_Delay ************************************************/

/* Function:RMP_Thd_Cancel ****************************************************
Description : Cancel the real-time thread from a previous delay.
Input       : volatile struct RMP_Thd* Thread - The pointer to the thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Cancel(volatile struct RMP_Thd* Thread)
{
    rmp_ptr_t State;
    rmp_ptr_t Pure;
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the thread pointer is valid */
    if(Thread==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
    /* Cache volatile thread state */
    State=Thread->State;
    /* Extract pure state */
    Pure=RMP_THD_STATE(State);
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the thread structure is in use */
    if(Pure==RMP_THD_FREE)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Check if the thread is in delay */
    if(Pure!=RMP_THD_DELAY)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_STATE;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Delete it from the delay list */
    RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
    /* Set to ready */
    RMP_THD_STATE_SET(State,RMP_THD_READY);
    /* Put cached thread state back */
    Thread->State=State;
    /* Insert into runqueue if not suspended */
    _RMP_Run_Ins(Thread,State);
    
    /* Supply cancel error code */
    Thread->Retval=RMP_ERR_OPER;
    RMP_Sched_Unlock();
        
    return 0;
}
/* End Function:RMP_Thd_Cancel ***********************************************/

/* Function:RMP_Thd_Loop ******************************************************
Description : Enter a useless loop to waste some time. Can be used when the
              scheduler is locked. The delay caused by each loop is chip and
              toolchain specific.
Input       : rmp_ptr_t Loop - The number of useless loops to run.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Thd_Loop(rmp_ptr_t Loop)
{
    volatile rmp_ptr_t Count;
    
    /* Just waste some time - volatile thus the compiler will not optimize */
    for(Count=0U;Count<Loop;Count++)
    {
        /* No action required */
    }
}
/* End Function:RMP_Thd_Loop *************************************************/

/* Function:RMP_Thd_Snd *******************************************************
Description : Send to a real-time thread's mailbox. If the mailbox is full, then
              this operation can potentially block.
Input       : volatile struct RMP_Thd* Thread - The thread structure of the
                                                thread to send to.
              rmp_ptr_t Data - The data to send to that thread.
              rmp_ptr_t Slice - The timeslices to wait if the mailbox is full.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Snd(volatile struct RMP_Thd* Thread,
                      rmp_ptr_t Data,
                      rmp_ptr_t Slice)
{
    rmp_ptr_t State;
    rmp_ptr_t Pure;
    rmp_ptr_t State_Cur;
    volatile struct RMP_Thd* Thd_Cur;
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the thread pointer is valid */
    if(Thread==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
    /* Cache volatile thread state */
    State=Thread->State;
    /* Extract pure state */
    Pure=RMP_THD_STATE(State);
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the thread structure is in use */
    if(Pure==RMP_THD_FREE)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Cache volatile current thread - must be own thread */
    Thd_Cur=RMP_Thd_Cur;

#if(RMP_CHECK_ENABLE!=0U)
    /* Check if this is a disallowed self-sending */
    if(Thd_Cur==Thread)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Check if the mailbox is empty */
    if((State&RMP_THD_MBOXFUL)==0U)
    {
        RMP_COV_MARKER();
        
        /* Mailbox empty, check if the receiver is waiting for us */
        if((Pure==RMP_THD_RCVBLK)||(Pure==RMP_THD_RCVDLY))
        {
            RMP_COV_MARKER();
            
            /* The receiver is blocked, wake it up */
            if(Pure==RMP_THD_RCVDLY)
            {
                RMP_COV_MARKER();
                
                RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
            }
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }
            
            /* Set to ready */
            RMP_THD_STATE_SET(State,RMP_THD_READY);
            /* Insert into runqueue if not suspended */
            _RMP_Run_Ins(Thread,State);
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
        
        /* Fill the receive buffer and put cached thread state back */
        Thread->Mail_Rcv=Data;
        Thread->State=State|RMP_THD_MBOXFUL;
        
        RMP_Sched_Unlock();
        return 0;
    }
    else
    {
        RMP_COV_MARKER();
        
        /* Mailbox full; see if we're blocking */
        if(Slice==0U)
        {
            RMP_COV_MARKER();
            
            RMP_Sched_Unlock();
            return RMP_ERR_OPER;
        }
        else
        {
            RMP_COV_MARKER();
            
            /* When abort, an error code will be supplied instead */
            Thd_Cur->Retval=0;
        }

        /* Cache volatile thread state */
        State_Cur=Thd_Cur->State;
        /* Delete from runqueue - must not be suspended */
        _RMP_Run_Del(Thd_Cur,State_Cur);
        /* Insert into mailbox wait queue */
        RMP_List_Ins(&(Thd_Cur->Run_Head),Thread->Snd_List.Prev,&(Thread->Snd_List));
        
        /* See if the wait is timed or infinite */
        if(Slice<RMP_SLICE_MAX)
        {
            RMP_COV_MARKER();
            
            _RMP_Dly_Ins(Thd_Cur,Slice);
            RMP_THD_STATE_SET(State_Cur,RMP_THD_SNDDLY);
        }
        else
        {
            RMP_COV_MARKER();
            
            RMP_THD_STATE_SET(State_Cur,RMP_THD_SNDBLK);
        }

        /* Fill the send buffer and put cached thread state back */
        Thd_Cur->Mail_Snd=Data;
        Thd_Cur->State=State_Cur;
    }
    
    RMP_Sched_Unlock();

    /* Retval might be updated */
    return Thd_Cur->Retval;
}
/* End Function:RMP_Thd_Snd **************************************************/

/* Function:RMP_Thd_Snd_ISR ***************************************************
Description : Send to a real-time thread's mailbox. If the mailbox is full, then
              this operation will just fail. This function can only be called 
              from an ISR whose priority is below or equal to the context switch
              handler's. We do not check whether the scheduler is locked; if we
              are calling this function, we're pretty sure that it's not.
Input       : volatile struct RMP_Thd* Thread - The thread structure of the 
                                                thread to send to.
              rmp_ptr_t Data - The data to send to that thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Snd_ISR(volatile struct RMP_Thd* Thread,
                          rmp_ptr_t Data)
{
    rmp_ptr_t State;
    rmp_ptr_t Pure;
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the thread pointer is valid */
    if(Thread==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Cache volatile thread state */
    State=Thread->State;
    /* Extract pure state */
    Pure=RMP_THD_STATE(State);
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the thread structure is in use */
    if(Pure==RMP_THD_FREE)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Check if the mailbox is empty */
    if((State&RMP_THD_MBOXFUL)==0U)
    {
        RMP_COV_MARKER();
        
        /* Mailbox empty, check if the receiver is waiting for us */
        if((Pure==RMP_THD_RCVBLK)||(Pure==RMP_THD_RCVDLY))
        {
            RMP_COV_MARKER();

            /* The receiver is blocked, wake it up */
            if(Pure==RMP_THD_RCVDLY)
            {
                RMP_COV_MARKER();
                
                RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
            }
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }
            
            /* Set to ready */
            RMP_THD_STATE_SET(State,RMP_THD_READY);
            /* Insert into runqueue if not suspended */
            _RMP_Run_Ins(Thread,State);

            /* Trigger a context switch if required */
#ifdef RMP_YIELD_ISR
            if(RMP_Sched_Pend!=0U)
            {
                RMP_COV_MARKER();
                
                /* Context switch will clear the pend flag */
                RMP_YIELD_ISR();
            }
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }
#endif
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
        
        /* Fill the receive buffer and put cached thread state back */
        Thread->Mail_Rcv=Data;
        Thread->State=State|RMP_THD_MBOXFUL;
    }
    else
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }

    return 0;
}
/* End Function:RMP_Thd_Snd_ISR **********************************************/

/* Function:_RMP_Thd_Unblock **************************************************
Description : Unblock one thread if there are one waiting, and set the mailbox 
              full states accordingly.
Input       : volatile struct RMP_Thd* Thd_Cur - The cached current thread.
Output      : rmp_ptr_t* Data - The pointer to put the data to.
              rmp_ptr_t State - The cached current thread state.
Return      : rmp_ptr_t - The new thread state to put back.
******************************************************************************/
static rmp_ptr_t _RMP_Thd_Unblock(volatile struct RMP_Thd* Thd_Cur,
                                  rmp_ptr_t* Data,
                                  rmp_ptr_t State)
{
    volatile struct RMP_Thd* Sender;

    /* Mailbox full; get the value from mailbox if needed */
    if(Data!=RMP_NULL)
    {
        RMP_COV_MARKER();
        
        *Data=Thd_Cur->Mail_Rcv;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if there are senders waiting */
    if(&(Thd_Cur->Snd_List)!=Thd_Cur->Snd_List.Next)
    {
        RMP_COV_MARKER();
        
        /* Delete the sender from waitlist and read the data; mailbox still full */
        Sender=(volatile struct RMP_Thd*)(Thd_Cur->Snd_List.Next);
        /* Extract mail from buffer */
        Thd_Cur->Mail_Rcv=Sender->Mail_Snd;
        /* Remove from mailbox queue */
        _RMP_Thd_Remove(Sender,RMP_THD_SNDDLY);
        /* Mailbox full state is unchanged */
        return State;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Nobody to unblock, mailbox empty now */
    return State&(~RMP_THD_MBOXFUL);
}
/* End Function:_RMP_Thd_Unblock *********************************************/

/* Function:RMP_Thd_Rcv *******************************************************
Description : Receive a message from our own mailbox, and this is blocking.
Input       : rmp_ptr_t Slice - The timeslices to wait if the mailbox is empty.
Output      : rmp_ptr_t* Data - The pointer to put the data to; if NULL, data
                                will be discarded.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Rcv(rmp_ptr_t* Data,
                      rmp_ptr_t Slice)
{
    rmp_ptr_t State;
    volatile struct RMP_Thd* Thd_Cur;
    
    RMP_Sched_Lock();
    
    /* Cache volatile current thread - must be own thread */
    Thd_Cur=RMP_Thd_Cur;
    /* Cache volatile thread state */
    State=Thd_Cur->State;
    
    /* Check if the mailbox is empty */
    if((State&RMP_THD_MBOXFUL)!=0U)
    {
        RMP_COV_MARKER();
        
        /* Extract value and unblock next sender, then put cached thread state back */
        Thd_Cur->State=_RMP_Thd_Unblock(Thd_Cur,Data,State);
        RMP_Sched_Unlock();
        return 0;
    }
    else
    {
        RMP_COV_MARKER();
        
        /* Mailbox is empty, must have nobody on the list now */
        RMP_ASSERT(&(Thd_Cur->Snd_List)==Thd_Cur->Snd_List.Next);
        /* See if we're blocking */
        if(Slice==0U)
        {
            RMP_COV_MARKER();
            
            RMP_Sched_Unlock();
            return RMP_ERR_OPER;
        }
        else
        {
            RMP_COV_MARKER();
            
            /* When abort, an error code will be supplied instead */
            Thd_Cur->Retval=0;
        }

        /* We must be ready and not suspended and will be deleted from ready queue */
        _RMP_Run_Del(Thd_Cur,State);
        /* See if this is a finite wait */
        if(Slice<RMP_SLICE_MAX)
        {
            RMP_COV_MARKER();
            
            _RMP_Dly_Ins(Thd_Cur,Slice);
            RMP_THD_STATE_SET(State,RMP_THD_RCVDLY);
        }
        else
        {
            RMP_COV_MARKER();
            
            RMP_THD_STATE_SET(State,RMP_THD_RCVBLK);
        }
        
        /* Put cached thread state back */
        Thd_Cur->State=State;
        /* Unlock the scheduler, wait for mail or timeout */
        RMP_Sched_Unlock();
        
        /* Retval must be updated or mailbox must be filled in this interval */
        
        /* We've been unblocked; could be a timeout or a full mailbox */
        RMP_Sched_Lock();
        /* Cache volatile thread state */
        State=Thd_Cur->State;
        
        /* Full mailbox, or timeout? */
        if((State&RMP_THD_MBOXFUL)!=0U)
        {
            RMP_COV_MARKER();
            
            /* The return value must be good */
            RMP_ASSERT(Thd_Cur->Retval==0);
            /* Extract value and unblock next sender, then put cached thread state back */
            Thd_Cur->State=_RMP_Thd_Unblock(Thd_Cur,Data,State);
        }
        /* Timeout */
        else
        {
            RMP_COV_MARKER();
            
            /* The return value must be bad */
            RMP_ASSERT(Thd_Cur->Retval!=0);
        }
    }
    
    RMP_Sched_Unlock();
    
    /* Retval might be updated earlier */
    return Thd_Cur->Retval;
}
/* End Function:RMP_Thd_Rcv **************************************************/

/* Function:RMP_Sem_Crt *******************************************************
Description : Create a semaphore in the system.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
              rmp_ptr_t Number - The initial number of this semaphore.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Crt(volatile struct RMP_Sem* Semaphore,
                      rmp_ptr_t Number)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the semaphore pointer is valid */
    if(Semaphore==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();

#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the semaphore structure is in use */
    if(Semaphore->State!=RMP_SEM_FREE)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the number is too great */
    if(Number>=RMP_SEM_CNT_MAX)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Initialize semaphore */
    Semaphore->State=RMP_SEM_USED;
    Semaphore->Num_Cur=Number;
    RMP_List_Crt(&(Semaphore->Wait_List));
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Sem_Crt **************************************************/

/* Function:RMP_Sem_Del *******************************************************
Description : Delete a semaphore in the system.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Del(volatile struct RMP_Sem* Semaphore)
{
    volatile struct RMP_Thd* Wait;

#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the semaphore pointer is valid */
    if(Semaphore==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the semaphore structure is in use */
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Get rid of all threads waiting on it */
    while(&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)
    {
        Wait=(volatile struct RMP_Thd*)(Semaphore->Wait_List.Next);
        /* Remove from semaphore waitlist */
        _RMP_Thd_Remove(Wait,RMP_THD_SEMDLY);
        /* Supply delete error code */
        Wait->Retval=RMP_ERR_OPER;
    }
    
    Semaphore->State=RMP_SEM_FREE;
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Sem_Del **************************************************/

/* Function:RMP_Sem_Post ******************************************************
Description : Post a number of semaphores to the list.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
              rmp_ptr_t Number - The number to post.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Post(volatile struct RMP_Sem* Semaphore,
                       rmp_ptr_t Number)
{
    rmp_ptr_t Num_Cur;
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the semaphore pointer is valid */
    if(Semaphore==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the number is valid */
    if(Number==0U)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the semaphore structure is in use */
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Cache volatile current number */
    Num_Cur=Semaphore->Num_Cur;

#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the maximum value will be exceeded if this is posted */
    if((Num_Cur+Number)>=RMP_SEM_CNT_MAX)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    Num_Cur+=Number;
    
    /* Wake up some waiting threads */
    while((&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)&&(Num_Cur!=0U))
    {
        _RMP_Thd_Remove((volatile struct RMP_Thd*)(Semaphore->Wait_List.Next),
                        RMP_THD_SEMDLY);
        Num_Cur--;
    }
    
    /* Put cached number back */
    Semaphore->Num_Cur=Num_Cur;

    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Sem_Post *************************************************/

/* Function:RMP_Sem_Post_ISR **************************************************
Description : Post a number of semaphores to the list. This function can only be
              called from an ISR whose priority is below or equal to the context
              switch handler's. We do not check whether the scheduler is locked;
              if we are calling this function, we're pretty sure that it's not.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
              rmp_ptr_t Number - The number to post.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Post_ISR(volatile struct RMP_Sem* Semaphore,
                           rmp_ptr_t Number)
{
    rmp_ptr_t Num_Cur;
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the semaphore pointer is valid */
    if(Semaphore==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the number is valid */
    if(Number==0U)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the semaphore structure is in use */
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Cache volatile current number */
    Num_Cur=Semaphore->Num_Cur;

#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the maximum value will be exceeded if this is posted */
    if((Num_Cur+Number)>=RMP_SEM_CNT_MAX)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    Num_Cur+=Number;
    
    /* Wake up some waiting threads */
    while((&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)&&(Num_Cur!=0U))
    {
        _RMP_Thd_Remove((volatile struct RMP_Thd*)(Semaphore->Wait_List.Next),
                        RMP_THD_SEMDLY);
        Num_Cur--;
    }
    
    /* Put cached number back */
    Semaphore->Num_Cur=Num_Cur;
    
    /* Trigger a context switch if required */
#ifdef RMP_YIELD_ISR
    if(RMP_Sched_Pend!=0U)
    {
        RMP_COV_MARKER();
        
        /* Context switch will clear the pend flag */
        RMP_YIELD_ISR();   
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif

    return 0;
}
/* End Function:RMP_Sem_Post_ISR *********************************************/

/* Function:RMP_Sem_Bcst ******************************************************
Description : Unblock all threads waiting on the semaphore.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
Output      : None.
Return      : rmp_ret_t - If successful, the number of threads unblocked; 
                          or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Bcst(volatile struct RMP_Sem* Semaphore)
{
    rmp_ret_t Number;
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the semaphore pointer is valid */
    if(Semaphore==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the semaphore structure is in use */
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Wake up all waiting threads */
    Number=0;
    while(&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)
    {
        _RMP_Thd_Remove((volatile struct RMP_Thd*)(Semaphore->Wait_List.Next),
                        RMP_THD_SEMDLY);
        Number++;
    }

    RMP_Sched_Unlock();
    return Number;
}
/* End Function:RMP_Sem_Bcst *************************************************/

/* Function:RMP_Sem_Bcst_ISR **************************************************
Description : Unblock all threads on the semaphore. This function can only be
              called from an ISR whose priority is below or equal to the context
              switch handler's. We do not check whether the scheduler is locked;
              if we are calling this function, we're pretty sure that it's not.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
Output      : None.
Return      : rmp_ret_t - If successful, the number of threads unblocked; 
                          or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Bcst_ISR(volatile struct RMP_Sem* Semaphore)
{   
    rmp_ret_t Number;
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the semaphore pointer is valid */
    if(Semaphore==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the semaphore structure is in use */
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Wake up all waiting threads */
    Number=0;
    while(&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)
    {
        _RMP_Thd_Remove((volatile struct RMP_Thd*)(Semaphore->Wait_List.Next),
                        RMP_THD_SEMDLY);
        Number++;
    }
    
    /* Trigger a context switch if required */
#ifdef RMP_YIELD_ISR
    if(RMP_Sched_Pend!=0U)
    {
        RMP_COV_MARKER();
        
        /* Context switch will clear the pend flag */
        RMP_YIELD_ISR();
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif

    return Number;
}
/* End Function:RMP_Sem_Bcst_ISR *********************************************/

/* Function:_RMP_Sem_Pend_Core ************************************************
Description : Pend on the semaphore, trying to get one. This is the core logic.
              When this is entered, the scheduler shall be locked.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
              rmp_ptr_t Slice - The number of slices to wait.
Output      : None.
Return      : rmp_ret_t - If successful, the current semaphore number; or an error code.
******************************************************************************/
static rmp_ret_t _RMP_Sem_Pend_Core(volatile struct RMP_Sem* Semaphore,
                                    rmp_ptr_t Slice)
{
    rmp_ptr_t State;
    rmp_ptr_t Num_Cur;
    volatile struct RMP_Thd* Thd_Cur;
    
    /* Cache volatile current thread */
    Thd_Cur=RMP_Thd_Cur;
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the semaphore structure is in use */
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Cache volatile current number */
    Num_Cur=Semaphore->Num_Cur;
    
    /* See if we can get one immediately */
    if(Num_Cur!=0U)
    {
        RMP_COV_MARKER();
        
        Num_Cur--;
        /* Put cached number back */
        Semaphore->Num_Cur=Num_Cur;
        RMP_Sched_Unlock();
        return (rmp_ret_t)Num_Cur;
    }
    else
    {
        RMP_COV_MARKER();
        
        /* Cannot get one, we need to block */
        if(Slice==0U)
        {
            RMP_COV_MARKER();
            
            RMP_Sched_Unlock();
            return RMP_ERR_OPER;
        }
        else
        {
            RMP_COV_MARKER();
            
            /* When abort, an error code will be supplied instead */
            Thd_Cur->Retval=0;
        }
        
        /* Cache volatile thread state */
        State=Thd_Cur->State;
        /* We must be ready - place into waitlist now */
        _RMP_Run_Del(Thd_Cur,State);
        RMP_List_Ins(&(Thd_Cur->Run_Head),
                     Semaphore->Wait_List.Prev,
                     &(Semaphore->Wait_List));
        
        if(Slice<RMP_SLICE_MAX)
        {
            RMP_COV_MARKER();
            
            _RMP_Dly_Ins(Thd_Cur,Slice);
            RMP_THD_STATE_SET(State,RMP_THD_SEMDLY);
        }
        else
        {
            RMP_COV_MARKER();
            
            RMP_THD_STATE_SET(State,RMP_THD_SEMBLK);
        }
        
        /* Put cached thread state back */
        Thd_Cur->State=State;
    }
    
    RMP_Sched_Unlock();

    /* Retval might be updated */
    return Thd_Cur->Retval;
}
/* End Function:_RMP_Sem_Pend_Core *******************************************/

/* Function:RMP_Sem_Pend ******************************************************
Description : Pend on the semaphore, trying to get one.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
              rmp_ptr_t Slice - The number of slices to wait.
Output      : None.
Return      : rmp_ret_t - If successful, the current semaphore number; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Pend(volatile struct RMP_Sem* Semaphore,
                       rmp_ptr_t Slice)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the semaphore pointer is valid */
    if(Semaphore==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
    return _RMP_Sem_Pend_Core(Semaphore,Slice);
}
/* End Function:RMP_Sem_Pend *************************************************/

/* Function:RMP_Sem_Pend_Unlock ***********************************************
Description : Pend on the semaphore, trying to get one. When we enter this
              function, the scheduler shall be locked, and it would be auto-
              unlocked when we exit.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
              rmp_ptr_t Slice - The number of slices to wait.
Output      : None.
Return      : rmp_ret_t - If successful, the current semaphore number; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Pend_Unlock(volatile struct RMP_Sem* Semaphore,
                              rmp_ptr_t Slice)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the semaphore pointer is valid */
    if(Semaphore==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the scheduler is locked just once - no more, no less */
    if(RMP_Sched_Lock_Cnt!=1U)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        /* No action required */
        RMP_COV_MARKER();
    }
#endif
    
    return _RMP_Sem_Pend_Core(Semaphore,Slice);
}
/* End Function:RMP_Sem_Pend_Unlock ******************************************/

/* Function:RMP_Sem_Abort *****************************************************
Description : Abort the waiting of one thread on a semaphore.
Input       : volatile struct RMP_Thd* Thread - The pointer to the thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Abort(volatile struct RMP_Thd* Thread)
{
    rmp_ptr_t State;
    rmp_ptr_t Pure;
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the thread pointer is valid */
    if(Thread==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
    /* Cache volatile thread state */
    State=Thread->State;
    /* Extract pure state */
    Pure=RMP_THD_STATE(State);
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the thread structure is in use */
    if(Pure==RMP_THD_FREE)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Check if the thread is really waiting on a semaphore */
    if((Pure!=RMP_THD_SEMBLK)&&(Pure!=RMP_THD_SEMDLY))
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_STATE;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Waiting for a semaphore. We abort it and return */
    RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
    if(Pure==RMP_THD_SEMDLY)
    {
        RMP_COV_MARKER();
        
        RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Set to ready */
    RMP_THD_STATE_SET(State,RMP_THD_READY);
    /* Put cached thread state back */
    Thread->State=State;
    /* Insert into runqueue if not suspended */
    _RMP_Run_Ins(Thread,State);
    
    /* Supply abort error code */
    Thread->Retval=RMP_ERR_OPER;
    RMP_Sched_Unlock();
    
    return 0;
}
/* End Function:RMP_Sem_Abort ************************************************/

/* Function:RMP_Sem_Cnt *******************************************************
Description : Get the number of semaphores.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
Output      : None.
Return      : rmp_ret_t - If successful, the number of semaphores; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Cnt(volatile struct RMP_Sem* Semaphore)
{
    rmp_ret_t Count;

#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the semaphore pointer is valid */
    if(Semaphore==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();

#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the semaphore structure is in use */
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    Count=(rmp_ret_t)(Semaphore->Num_Cur);
    
    RMP_Sched_Unlock();
    return Count;
}
/* End Function:RMP_Sem_Cnt **************************************************/

/* Function:RMP_Init **********************************************************
Description : The entry of the user thread. This is the first user thread that
              will be created. The user threads shall never return.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Init(void)
{
    RMP_Sched_Lock();
    /* Perform per-platform initialization */
    _RMP_Plat_Hook();
    /* Create all other kernel objects here */
    RMP_Init_Hook();
    RMP_Sched_Unlock();
    
    while(1)
    {
        RMP_Init_Idle();
    }
}
/* End Function:RMP_Init *****************************************************/

/* Function:main **************************************************************
Description : The entry of the operating system. This function is for 
              compatibility with the toolchains.
Input       : None.
Output      : None.
Return      : int - Dummy; this function never returns.
******************************************************************************/
int main(void)
{
    rmp_ptr_t Count;
    
    /* Initialize the low-level hardware first */
    _RMP_Lowlvl_Init();
    
#ifdef RMP_START_HOOK
    RMP_START_HOOK();
#endif

#ifdef RMP_COV_LINE_NUM
    /* Initialize coverage markers if coverage enabled */
    for(Count=0U;Count<RMP_COV_WORD_NUM;Count++)
    {
        RMP_Coverage[Count]=0U;
    }
#endif

    /* Now initialize the kernel data structures */
    RMP_Timestamp=0U;
    RMP_Sched_Lock_Cnt=0U;
    RMP_Sched_Pend=0U;

    /* Linked lists */
    RMP_List_Crt(&RMP_Delay);
    for(Count=0U;Count<RMP_PREEMPT_PRIO_NUM;Count++)
    {
        RMP_List_Crt(&RMP_Run[Count]);
    }
    for(Count=0U;Count<RMP_PRIO_WORD_NUM;Count++)
    {
        RMP_Bitmap[Count]=0U;
    }

    /* Now boot into the first thread */
    RMP_Clear(&RMP_Init_Thd,sizeof(struct RMP_Thd));
    RMP_Init_Thd.Prio=0U;
    RMP_Init_Thd.Slice=65500U;
    RMP_Init_Thd.Slice_Left=65500U;
    RMP_Init_Thd.State=RMP_THD_READY;
    RMP_Init_Thd.Stack=RMP_INIT_STACK;

    /* Initialize sending list */
    RMP_List_Crt(&(RMP_Init_Thd.Snd_List));

    /* Insert this into the corresponding runqueue */
    RMP_List_Ins(&(RMP_Init_Thd.Run_Head),RMP_Run[0].Prev,&(RMP_Run[0]));
    /* Set this runlevel as active - in fact it is always active */
    RMP_Bitmap[0]|=1U;
    
    /* Set current thread and stack */
    RMP_Thd_Cur=(volatile struct RMP_Thd*)&RMP_Init_Thd;
    RMP_SP_Cur=RMP_Init_Thd.Stack;
    
    /* Now jump to the init thread and will never return; stack
     * initialization is unnecessary for the first thread */
    _RMP_Start((rmp_ptr_t)RMP_Init, (rmp_ptr_t)RMP_Init_Thd.Stack);
    
    return 0;
}
/* End Function:main *********************************************************/

/* Function:RMP_Mem_Init ******************************************************
Description : Initialize a trunk of memory as the memory pool. The TLSF allocator's
              FLI will be decided upon the memory block size. Memory allocation does
              not lock the scheduler by itself; it is up to you to decide whether a
              scheduler lock is needed.
              The TLSF memory allocator consists of FLI, SLI and allocatable memory. 
              The FLI is grouped by 2^n, and the SLI segregates the FLI section by a
              power of 2, i.e. 8 or 16. Thus, when we need a memory block, we will 
              try to find it in the corresponding FLI, and then the SLI. (You can 
              consider the FLI-SLI segregation as a two-dimensional matrix.) Then 
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
              rmp_ptr_t Size - The size of the memory pool, word-aligned. Must be 
                               more than 1024 machine words, or pool creation will not
                               be successful.
Output      : None.
Return      : rmp_ret_t - If successful, 0; else an error code.
******************************************************************************/
rmp_ret_t RMP_Mem_Init(volatile void* Pool,
                       rmp_ptr_t Size)
{
    rmp_ptr_t FLI_Cnt;
    rmp_ptr_t Offset;
    rmp_ptr_t Bitmap_Size;
    volatile struct RMP_Mem* Mem;

#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the memory pool is large enough to enable dynamic allocation -
     * at least 1024 machine words or pool initialization will be refused */
    if((Pool==RMP_NULL)||(Size<(1024U*sizeof(rmp_ptr_t)))||((((rmp_ptr_t)Pool)+Size)<Size))
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_MEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the address and size is word-aligned - divisions will be optimized out */
    if(((((rmp_ptr_t)Pool)%sizeof(rmp_ptr_t))!=0U)||((Size%sizeof(rmp_ptr_t))!=0U))
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_MEM;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    Mem=(volatile struct RMP_Mem*)Pool;
    Mem->Size=Size;
    /* Calculate the FLI value needed for this - we always align to 64 byte */
    Mem->FLI_Num=RMP_MSB_GET(Size-sizeof(struct RMP_Mem))-6U+1U;
    
    /* Initialize the bitmap - how many words are needed for this bitmap? */
    Bitmap_Size=RMP_MEM_WORD_NUM(Mem->FLI_Num);
    for(FLI_Cnt=0U;FLI_Cnt<Bitmap_Size;FLI_Cnt++)
    {
        Mem->Bitmap[FLI_Cnt]=0U;
    }
    
    /* Decide the location of the allocation list table - "-1" is
     * because we defined the length=1 in our struct already */
    Offset=sizeof(struct RMP_Mem)+(Bitmap_Size-1U)*sizeof(rmp_ptr_t);
    Mem->Table=(struct RMP_List*)(((rmp_ptr_t)Mem)+Offset);
    /* Initialize the allocation table */
    for(FLI_Cnt=0U;FLI_Cnt<Mem->FLI_Num;FLI_Cnt++)
    {
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt, 0U)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt, 1U)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt, 2U)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt, 3U)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt, 4U)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt, 5U)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt, 6U)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt, 7U)]));
    }
    
    /* Decide the offset of the actual allocatable memory - each FLI
     * has 8 SLIs, and each SLI has a corresponding table header */
    Offset+=sizeof(struct RMP_List)*8U*Mem->FLI_Num;
    Mem->Base=((rmp_ptr_t)Mem)+Offset;
    
    /* Initialize the first big block */
    _RMP_Mem_Block((struct RMP_Mem_Head*)(Mem->Base),Size-Offset,RMP_MEM_FREE);
    /* Insert the memory into the corresponding level */
    _RMP_Mem_Ins(Pool,(struct RMP_Mem_Head*)(Mem->Base));
    
    return 0;
}
/* End Function:RMP_Mem_Init *************************************************/

/* Function:_RMP_Mem_Block ****************************************************
Description : Make a memory block from the memory trunk. The memory block is
              always free when created. No parameter check is performed here.
Input       : volatile struct RMP_Mem_Head* Head - The start address of the
                                                   memory block, word-aligned.
              rmp_ptr_t Size - The total size of the memory block, word-aligned.
              rmp_ptr_t State - The allocation state of the new memory block.
Output      : None.
Return      : None.
******************************************************************************/
static void _RMP_Mem_Block(volatile struct RMP_Mem_Head* Head,
                           rmp_ptr_t Size,
                           rmp_ptr_t State)
{
    Head->State=State;
    Head->Tail=RMP_MEM_TAIL_INIT(Head,Size);
    Head->Tail->Head=Head;
}
/* End Function:_RMP_Mem_Block ***********************************************/

/* Function:_RMP_Mem_Ins ******************************************************
Description : The memory insertion function, to insert a certain memory block
              into the corresponding FLI and SLI slot.
Input       : volatile void* Pool - The memory pool.
              volatile struct RMP_Mem_Head* Head - The pointer to the memory block.
Output      : None.
Return      : None.
******************************************************************************/
static void _RMP_Mem_Ins(volatile void* Pool,
                         volatile struct RMP_Mem_Head* Head)
{
    rmp_ptr_t FLI_Level;
    rmp_ptr_t SLI_Level;
    rmp_ptr_t Level;
    rmp_ptr_t Size;
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_List* Slot;
    
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=RMP_MEM_HEAD2SIZE(Head);

    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_GET(Size)-6U;
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3U))&0x07U;
    /* Calculate the bit position */
    Level=RMP_MEM_POS(FLI_Level,SLI_Level);
    /* Get the slot */
    Slot=&(Mem->Table[Level]);

    /* See if we are inserting the first memory block */
    if(Slot==Slot->Next)
    {
        RMP_COV_MARKER();
        
        /* Set the corresponding bit in the TLSF bitmap */
        RMP_BITMAP_SET(Mem->Bitmap,Level);
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }

    /* Insert the node now */
    RMP_List_Ins(&(Head->Head),Slot,Slot->Next);
}
/* End Function:_RMP_Mem_Ins *************************************************/

/* Function:_RMP_Mem_Del ******************************************************
Description : The memory deletion function, to delete a certain memory block
              from the corresponding FLI and SLI class.
Input       : volatile void* Pool - The memory pool.
              volatile struct RMP_Mem_Head* Head - The pointer to the memory block.
Output      : None.
Return      : None.
******************************************************************************/
static void _RMP_Mem_Del(volatile void* Pool,
                         volatile struct RMP_Mem_Head* Head)
{
    rmp_ptr_t FLI_Level;
    rmp_ptr_t SLI_Level;
    rmp_ptr_t Level;
    rmp_ptr_t Size;
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_List* Slot;    
    
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=RMP_MEM_HEAD2SIZE(Head);
    
    /* Guarantee the Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_GET(Size)-6U;
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3U))&0x07U;
    /* Calculate the bit position */
    Level=RMP_MEM_POS(FLI_Level,SLI_Level);
    /* Get the slot */
    Slot=&(Mem->Table[Level]);

    /* Delete the node now */
    RMP_List_Del(Head->Head.Prev,Head->Head.Next);

    /* See if there are any blocks in the level, equal means no. So
     * what we deleted is the last blockm need to clear the flag */
    if(Slot==Slot->Next)
    {
        RMP_COV_MARKER();
        
        /* Clear the corresponding bit in the TLSF bitmap */
        RMP_BITMAP_CLR(Mem->Bitmap,Level);
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
}
/* End Function:_RMP_Mem_Del *************************************************/

/* Function:_RMP_Mem_Search ***************************************************
Description : The TLSF memory searcher.
Input       : volatile void* Pool - The memory pool.
              rmp_ptr_t Size - The memory size, must be bigger than 64. This must be
                               guaranteed before calling this function or an error
                               will unavoidably occur.
Output      : rmp_ptr_t* FLI_Level - The FLI level found.
              rmp_ptr_t* SLI_Level - The SLI level found.
Return      : rmp_ret_t - If successful, 0; else -1 for failure.
******************************************************************************/
static rmp_ret_t _RMP_Mem_Search(volatile void* Pool,
                                 rmp_ptr_t Size,
                                 rmp_ptr_t* FLI_Level,
                                 rmp_ptr_t* SLI_Level)
{
    rmp_ptr_t Level;
    rmp_ptr_t Word;
    rmp_ptr_t Limit;
    rmp_ptr_t FLI_Search;
    rmp_ptr_t SLI_Search;
    volatile struct RMP_Mem* Mem;

    /* Make sure that it is bigger than 64=2^6 */
    FLI_Search=RMP_MSB_GET(Size)-6U;
    
    /* Decide the SLI level directly from the FLI level. We plus the number by one here
     * so that we can avoid the list search. However, when the allocated memory is just
     * one of the levels, then we don't need to jump to the next level and can fit directly */
    SLI_Search=(Size>>(FLI_Search+3U))&0x07U;
    if(Size!=(RMP_POW2(FLI_Search+3U)*(SLI_Search+8U)))
    {
        RMP_COV_MARKER();
        
        SLI_Search++;
        
        /* If the SLI level is the largest of the SLI levels, then jump to the next FLI level */
        if(SLI_Search==8U)
        {
            RMP_COV_MARKER();
            
            FLI_Search+=1U;
            SLI_Search=0U;
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the FLI level is over the boundary */
    Mem=(volatile struct RMP_Mem*)Pool;
    if(FLI_Search>=Mem->FLI_Num)
    {
        RMP_COV_MARKER();
        
        return -1;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Try to find the word that contains this level, then right shift away the
     * lower levels to extract the ones that can satisfy this allocation request */
    Level=RMP_MEM_POS(FLI_Search,SLI_Search);
    Word=Mem->Bitmap[Level>>RMP_WORD_ORDER]>>(Level&RMP_MASK_WORD);
    
    /* If there's at least one block that matches the query, return the level */
    if(Word!=0U)
    {
        RMP_COV_MARKER();
        
        /* Also need to compensate for the lower levels that were shifted away; the following line is
         * simplified from "Level=(Level&(~RMP_MASK_WORD))+(RMP_LSB_GET(Word)+(Level&RMP_MASK_WORD))" */
        Level+=RMP_LSB_GET(Word);
        *FLI_Level=Level>>3U;
        *SLI_Level=Level&0x07U;
        return 0;
    }
    /* No fits in that exact level, compute the size of bitmap and look through higher levels */
    else
    {
        RMP_COV_MARKER();
        
        Limit=RMP_MEM_WORD_NUM(Mem->FLI_Num);
        /* From the next word, query one by one */
        for(Word=(Level>>RMP_WORD_ORDER)+1U;Word<Limit;Word++)
        {
            /* If the level has blocks of one FLI level */
            if(Mem->Bitmap[Word]!=0U)
            {
                RMP_COV_MARKER();
                
                /* Find the actual level */ 
                Level=RMP_LSB_GET(Mem->Bitmap[Word]);
                *FLI_Level=((Word<<RMP_WORD_ORDER)+Level)>>3U;
                *SLI_Level=Level&0x07U;
                return 0;
            }
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }
        }
    }

    /* Search failed */
    return -1;
}
/* End Function:_RMP_Mem_Search **********************************************/

/* Function:RMP_Malloc ********************************************************
Description : Allocate some memory from a designated memory pool.
Input       : volatile void* Pool - The pool to allocate from.
              rmp_ptr_t Size - The size of the RAM needed to allocate.
Output      : None.
Return      : void* - The pointer to the memory. If no memory is available,
                      NULL is returned.
******************************************************************************/
void* RMP_Malloc(volatile void* Pool,
                 rmp_ptr_t Size)
{    
    rmp_ptr_t FLI_Level;
    rmp_ptr_t SLI_Level;
    volatile struct RMP_Mem* Mem;
    rmp_ptr_t Old_Size;
    volatile struct RMP_Mem_Head* Head;
    rmp_ptr_t Round_Size;
    volatile struct RMP_Mem_Head* New;
    rmp_ptr_t New_Size;
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the pool pointer and size is valid */
    if((Pool==RMP_NULL)||(Size==0U))
    {
        RMP_COV_MARKER();
        
        return RMP_NULL;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Round up the size:a multiple of 8 and bigger than 64B */
    Round_Size=RMP_ROUND_UP(Size, 3U);
    /* See if it is smaller than the smallest block */
    Round_Size=(Round_Size>64U)?Round_Size:64U;

    /* See if such block exists, if not, abort */
    if(_RMP_Mem_Search(Pool,Round_Size,&FLI_Level,&SLI_Level)!=0)
    {
        RMP_COV_MARKER();
        
        return RMP_NULL;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    Mem=(volatile struct RMP_Mem*)Pool;
    
    /* There is such a block; get it and delete it from the TLSF list */
    Head=(volatile struct RMP_Mem_Head*)(Mem->Table[RMP_MEM_POS(FLI_Level,SLI_Level)].Next);
    _RMP_Mem_Del(Pool,Head);

    /* Allocate and calculate if the space left could be big enough to be a new 
     * block. If so, we will put the block back into the TLSF table. */
    New_Size=RMP_MEM_HEAD2SIZE(Head)-Round_Size;
    if(New_Size>=RMP_MEM_SIZE2WHOLE(64U))
    {
        RMP_COV_MARKER();
        
        Old_Size=RMP_MEM_SIZE2WHOLE(Round_Size);
        New=(volatile struct RMP_Mem_Head*)(((rmp_ptr_t)Head)+Old_Size);

        _RMP_Mem_Block(Head,Old_Size,RMP_MEM_USED);
        _RMP_Mem_Block(New,New_Size,RMP_MEM_FREE);

        /* Put the extra block back */
        _RMP_Mem_Ins(Pool, New);
    }
    else
    {
        RMP_COV_MARKER();
        
        /* Residue too small, mark the whole block as in use */
        Head->State=RMP_MEM_USED;
    }

    /* Finally, return the start address */
    return (void*)(((rmp_ptr_t)Head)+sizeof(struct RMP_Mem_Head));
}
/* End Function:RMP_Malloc ***************************************************/

/* Function:RMP_Free **********************************************************
Description : Free allocated memory, for system use mainly. It will free memory 
              in the name of a certain process, specified by the PID.
Input       : volatile void* Pool - The pool to free to.
              void* Mem_Ptr - The pointer returned by "RMP_Malloc".
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Free(volatile void* Pool,
              void* Mem_Ptr)
{
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_Mem_Head* Head;
    volatile struct RMP_Mem_Head* Left;
    volatile struct RMP_Mem_Head* Right;
    rmp_ptr_t Merge_Left;

    /* Check if the memory pointer is valid */
    if(Mem_Ptr==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the pool is valid */
    if(Pool==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    Mem=(volatile struct RMP_Mem*)Pool;
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the address is within the allocatable address range */
    if((((rmp_ptr_t)Mem_Ptr)<=((rmp_ptr_t)Mem))||(((rmp_ptr_t)Mem_Ptr)>=(((rmp_ptr_t)Mem)+Mem->Size)))
    {
        RMP_COV_MARKER();
        
        return;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif

    Head=RMP_MEM_PTR2HEAD(Mem_Ptr);
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the block is already freed */
    if(Head->State==RMP_MEM_FREE)
    {
        RMP_COV_MARKER();
        
        return;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Mark it as free in case no merge happens */
    Head->State=RMP_MEM_FREE;
    
    /* Check if we can merge it with the right side block */
    Right=RMP_MEM_HEAD2RIGHT(Head);
    if(((rmp_ptr_t)Right)!=(((rmp_ptr_t)Mem)+Mem->Size))
    {
        RMP_COV_MARKER();
        
        /* If this one is unoccupied */
        if((Right->State)==RMP_MEM_FREE)
        {
            RMP_COV_MARKER();
            
            /* Delete, merge */
            _RMP_Mem_Del(Pool,Right);
            _RMP_Mem_Block(Head,RMP_MEM_HEAD2END(Right)-(rmp_ptr_t)Head,RMP_MEM_FREE);
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }

    /* Check if we can merge it with the left side block */
    Merge_Left=0U;
    Left=RMP_MEM_HEAD2LEFT(Head);
    if((rmp_ptr_t)Head!=Mem->Base)
    {
        RMP_COV_MARKER();

        /* If this one is unoccupied */
        if(Left->State==RMP_MEM_FREE)
        {
            RMP_COV_MARKER();
            
            /* Delete, merge */
            _RMP_Mem_Del(Pool,Left);
            _RMP_Mem_Block(Left,RMP_MEM_HEAD2END(Head)-(rmp_ptr_t)Left,RMP_MEM_FREE);
            /* We have completed the merge here and the original block has destroyed */
            Merge_Left=1U;
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }

    /* If we did not merge it with the left-side blocks, insert the original pointer's block 
     * into the TLSF table(Merging with the right-side one won't disturb this) */
    if(Merge_Left==0U)
    {
        RMP_COV_MARKER();
        
        _RMP_Mem_Ins(Pool,Head);
    }
    else
    {
        RMP_COV_MARKER();
        
        _RMP_Mem_Ins(Pool,Left);
    }
}
/* End Function:RMP_Free *****************************************************/

/* Function:RMP_Realloc *******************************************************
Description : Expand or shrink an allocation to the desired size. The behavior
              of this function equals RMP_Malloc if the Mem_Ptr passed in is 0,
              or RMP_Free if the Size passed in is 0.
Input       : volatile void* Pool - The pool to reallocate from.
              void* Mem_Ptr - The old memory block to expand.
              rmp_ptr_t Size - The size of the RAM needed to resize to.
Output      : None.
Return      : void* - The pointer to the memory. If no memory available or an
                      error occurred, 0 is returned.
******************************************************************************/
void* RMP_Realloc(volatile void* Pool,
                  void* Mem_Ptr,
                  rmp_ptr_t Size)
{
    rmp_ptr_t Count;
    /* The size of the original memory block */
    rmp_ptr_t Mem_Size;
    /* The rounded size of the new memory request */
    rmp_ptr_t Round_Size;
    /* The pointer to the pool */
    volatile struct RMP_Mem* Mem;
    /* The head of the old memory */
    volatile struct RMP_Mem_Head* Head;
    /* The right-side block head */
    volatile struct RMP_Mem_Head* Right;
    /* The pointer to the residue memory head */
    volatile struct RMP_Mem_Head* Res;
    /* The new memory block */
    void* New;
    /* The size of the memory block including the header sizes */
    rmp_ptr_t Old_Size;
    /* The size of the residue memory block including the header sizes */
    rmp_ptr_t Res_Size;
    
    /* Are we passing in a NULL pointer? If yes, allocate. It is fine to allocate
     * without further checking because the allocation will check them anyway. The
     * same goes for the "free" below. */
    if(Mem_Ptr==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_Malloc(Pool, Size);
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Is the size passed in zero? If yes, we free directly - this is somewhat different
     * than standard realloc where you get a "0"-sized non-NULL realloc-able trunk. */
    if(Size==0U)
    {
        RMP_COV_MARKER();
        
        RMP_Free(Pool, Mem_Ptr);
        return RMP_NULL;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* The real reallocation starts here */
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the pool pointer is valid */
    if(Pool==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_NULL;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif

    Mem=(volatile struct RMP_Mem*)Pool;
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the address is within the allocatable address range */
    if((((rmp_ptr_t)Mem_Ptr)<=((rmp_ptr_t)Mem))||(((rmp_ptr_t)Mem_Ptr)>=(((rmp_ptr_t)Mem)+Mem->Size)))
    {
        RMP_COV_MARKER();
        
        return RMP_NULL;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif

    /* Get the location of the header of the memory */
    Head=RMP_MEM_PTR2HEAD(Mem_Ptr);
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the block is already freed */
    if(Head->State==RMP_MEM_FREE)
    {
        RMP_COV_MARKER();
        
        return RMP_NULL;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Round up the size:a multiple of 8 and bigger than 64B */
    Round_Size=RMP_ROUND_UP(Size, 3U);
    /* See if it is smaller than the smallest block */
    Round_Size=(Round_Size>64U)?Round_Size:64U;
    
    Mem_Size=RMP_MEM_PTR_DIFF(Head->Tail, Mem_Ptr);
    /* Does the right-side head exist at all? */
    Right=RMP_MEM_HEAD2RIGHT(Head);
    if(((rmp_ptr_t)Right)==(((rmp_ptr_t)Mem)+Mem->Size))
    {
        RMP_COV_MARKER();
        
        Right=RMP_NULL;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Are we gonna expand it? */
    if(Mem_Size<Round_Size)
    {
        RMP_COV_MARKER();
        
        /* Expanding - does the right side exist at all? */
        if(Right!=RMP_NULL)
        {
            RMP_COV_MARKER();
            
            /* Is it allocated? */
            if(Right->State==RMP_MEM_FREE)
            {
                RMP_COV_MARKER();
                
                /* Right-side exists and is free, need to see if it is big enough */
                Res_Size=RMP_MEM_PTR_DIFF(Right->Tail,Mem_Ptr);
                if(Res_Size>=Round_Size)
                {
                    RMP_COV_MARKER();
                    
                    /* Remove the right-side from the free list so we can operate on it */
                    _RMP_Mem_Del(Pool, Right);   
                    /* Allocate and calculate if the space left could be big enough to be a new 
                     * block. If so, we will put the block back into the TLSF table. */
                    Res_Size-=Round_Size;
                    /* Is the residue big enough to be a block? */
                    if(Res_Size>=RMP_MEM_SIZE2WHOLE(64U))
                    {
                        RMP_COV_MARKER();
                        
                        Old_Size=RMP_MEM_SIZE2WHOLE(Round_Size);
                        Res=(volatile struct RMP_Mem_Head*)(((rmp_ptr_t)Head)+Old_Size);

                        _RMP_Mem_Block(Head,Old_Size,RMP_MEM_USED);
                        _RMP_Mem_Block(Res,Res_Size,RMP_MEM_FREE);

                        /* Put the residue block back */
                        _RMP_Mem_Ins(Pool, Res);
                    }
                    /* Residue too small, merging the whole thing in is the only option */
                    else
                    {
                        RMP_COV_MARKER();
                        
                        Old_Size=RMP_MEM_PTR_DIFF(Right->Tail,Head)+sizeof(struct RMP_Mem_Tail);
                        _RMP_Mem_Block(Head,Old_Size,RMP_MEM_USED);
                    }
                    
                    /* Return the old pointer because we expanded it */
                    return Mem_Ptr;
                }
                /* Right-side not large enough, have to go malloc then memcpy */
                else
                {
                    RMP_COV_MARKER();
                    /* No action required */
                }
            }
            /* It is allocated, have to go malloc then memcpy */
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }
        }
        /* Right-side doesn't exist, have to go malloc then memcpy */
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
        
        New=RMP_Malloc(Pool,Round_Size);
        /* See if we can allocate this much, if we can't at all, exit */
        if(New==RMP_NULL)
        {
            RMP_COV_MARKER();
            
            return RMP_NULL;
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
        
        /* Copy old memory to new memory - we know that it must be aligned to word boundary;
         * cannot use bitshift with RMP_WORD_ORDER here in case sizeof(rmp_ptr_t) is not 1 */
        Mem_Size/=sizeof(rmp_ptr_t);
        for(Count=0U;Count<Mem_Size;Count++)
        {
            ((rmp_ptr_t*)New)[Count]=((rmp_ptr_t*)Mem_Ptr)[Count];
        }
        
        /* Free old memory then return */
        RMP_Free(Pool,Mem_Ptr);
        return New;
    }
    /* Keeping the same memory, useless call */
    else if(Mem_Size==Round_Size)
    {
        RMP_COV_MARKER();
        
        return Mem_Ptr;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Must be shrinking memory */
    if(Right!=RMP_NULL)
    {
        RMP_COV_MARKER();
        
        /* Right side does exist and not allocated; need to merge the block */
        if(Right->State==RMP_MEM_FREE)
        {
            RMP_COV_MARKER();
            
            /* Remove the right-side from the allocation list so we can operate on it */
            _RMP_Mem_Del(Pool, Right);
            Res_Size=RMP_MEM_PTR_DIFF(Right->Tail,Mem_Ptr)-Round_Size;
            Old_Size=RMP_MEM_SIZE2WHOLE(Round_Size);
            Res=(volatile struct RMP_Mem_Head*)(((rmp_ptr_t)Head)+Old_Size);

            _RMP_Mem_Block(Head,Old_Size,RMP_MEM_USED);
            _RMP_Mem_Block(Res,Res_Size,RMP_MEM_FREE);

            /* Put the extra block back */
            _RMP_Mem_Ins(Pool,Res);
            
            /* Return the old pointer because we shrinked it */
            return Mem_Ptr;
        }
        /* Allocated. Need to see if the residue block itself is large enough to be inserted back */
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* The right-side head either does not exist or is allocated, calculate the resulting residue size */
    Res_Size=Mem_Size-Round_Size;
    if(Res_Size<RMP_MEM_SIZE2WHOLE(64U))
    {
        RMP_COV_MARKER();
        
        /* The residue block wouldn't even count as a small one, do nothing and quit */
        return Mem_Ptr;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* The residue will be big enough to become a standalone block, and we need to place it back */ 
    Old_Size=RMP_MEM_SIZE2WHOLE(Round_Size);
    Res=(volatile struct RMP_Mem_Head*)(((rmp_ptr_t)Head)+Old_Size);
    
    _RMP_Mem_Block(Head,Old_Size,RMP_MEM_USED);
    _RMP_Mem_Block(Res,Res_Size,RMP_MEM_FREE);

    /* Put the extra block back */
    _RMP_Mem_Ins(Pool,Res);
    
    /* Return the old pointer because we shrinked it */
    return Mem_Ptr;
}
/* End Function:RMP_Realloc **************************************************/

/* Function:RMP_Fifo_Crt ******************************************************
Description : Create a FIFO.
Input       : volatile struct RMP_Fifo* Fifo - The pointer to the FIFO.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Fifo_Crt(volatile struct RMP_Fifo* Fifo)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the FIFO pointer is valid */
    if(Fifo==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();

#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the FIFO structure is in use */
    if(Fifo->State!=RMP_FIFO_FREE)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Create linked list */
    RMP_List_Crt(&(Fifo->Head));
    Fifo->Num_Cur=0U;
    Fifo->State=RMP_FIFO_USED;
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Fifo_Crt *************************************************/

/* Function:RMP_Fifo_Del ******************************************************
Description : Delete a FIFO.
Input       : volatile struct RMP_Fifo* Fifo - The pointer to the FIFO.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Fifo_Del(volatile struct RMP_Fifo* Fifo)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the FIFO pointer is valid */
    if(Fifo==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the FIFO structure is in use */
    if(Fifo->State!=RMP_FIFO_USED)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Check if the FIFO have any elements */
    if(Fifo->Num_Cur!=0U)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Mark as free */
    Fifo->State=RMP_FIFO_FREE;
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Fifo_Del *************************************************/

/* Function:RMP_Fifo_Read *****************************************************
Description : Read an element from a FIFO.
Input       : volatile struct RMP_Fifo* Fifo - The pointer to the FIFO.
Output      : struct RMP_List** Node - The node read.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Fifo_Read(volatile struct RMP_Fifo* Fifo,
                        volatile struct RMP_List** Node)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the FIFO pointer is valid */
    if(Fifo==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the node pointer is valid */
    if(Node==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the FIFO structure is in use */
    if(Fifo->State!=RMP_FIFO_USED)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* See if the FIFO is empty */
    if(Fifo->Head.Next==&(Fifo->Head))
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* If not, grab one */
    *Node=Fifo->Head.Next;
    RMP_List_Del((*Node)->Prev,(*Node)->Next);
    
    /* The count should not be zero, decrease it */
    RMP_ASSERT(Fifo->Num_Cur!=0U);
    Fifo->Num_Cur--;
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Fifo_Read ************************************************/

/* Function:RMP_Fifo_Write ****************************************************
Description : Write an element to a FIFO.
Input       : volatile struct RMP_Fifo* Fifo - The pointer to the FIFO.
              volatile struct RMP_List* Node - The node to put into the FIFO.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Fifo_Write(volatile struct RMP_Fifo* Fifo,
                         volatile struct RMP_List* Node)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the FIFO pointer is valid */
    if(Fifo==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }

    /* Check if the data pointer is valid */
    if(Node==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the FIFO structure is in use */
    if(Fifo->State!=RMP_FIFO_USED)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Write to list and increase count */
    RMP_List_Ins(Node,Fifo->Head.Prev,&(Fifo->Head));
    Fifo->Num_Cur++;

    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Fifo_Write ***********************************************/

/* Function:RMP_Fifo_Write_ISR ************************************************
Description : Write an element to a FIFO, from the ISR. This function can only be
              called from an ISR whose priority is below or equal to the context
              switch handler's. We do not check whether the scheduler is locked;
              if we are calling this function, we're pretty sure that it's not.
Input       : volatile struct RMP_Fifo* Fifo - The pointer to the FIFO.
              volatile struct RMP_List* Node - The node to put into the FIFO.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Fifo_Write_ISR(volatile struct RMP_Fifo* Fifo,
                             volatile struct RMP_List* Node)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the FIFO pointer is valid */
    if(Fifo==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }

    /* Check if the data pointer is valid */
    if(Node==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the FIFO structure is in use */
    if(Fifo->State!=RMP_FIFO_USED)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Write to list and increase count */
    RMP_List_Ins(Node, Fifo->Head.Prev, &(Fifo->Head));
    Fifo->Num_Cur++;

    return 0;
}
/* End Function:RMP_Fifo_Write_ISR *******************************************/

/* Function:RMP_Fifo_Cnt ******************************************************
Description : Get the number of elements in the FIFO.
Input       : volatile struct RMP_Fifo* Fifo - The pointer to the FIFO.
Output      : None.
Return      : rmp_ret_t - If successful, the number of nodes; or an error code.
******************************************************************************/
rmp_ret_t RMP_Fifo_Cnt(volatile struct RMP_Fifo* Fifo)
{
    rmp_ret_t Count;

#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the FIFO pointer is valid */
    if(Fifo==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the FIFO structure is in use */
    if(Fifo->State!=RMP_FIFO_USED)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    Count=(rmp_ret_t)(Fifo->Num_Cur);
    
    RMP_Sched_Unlock();
    return Count;
}
/* End Function:RMP_Fifo_Cnt *************************************************/

/* Function:RMP_Msgq_Crt ******************************************************
Description : Create a message queue.
Input       : volatile struct RMP_Msgq* Queue - The pointer to the queue.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Msgq_Crt(volatile struct RMP_Msgq* Queue)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue pointer is valid */
    if(Queue==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue structure is in use */
    if(Queue->State!=RMP_MSGQ_FREE)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* A queue is just a FIFO paired with a counting semaphore */
    RMP_ASSERT(RMP_Sem_Crt(&(Queue->Sem), 0U)==0);
    RMP_ASSERT(RMP_Fifo_Crt(&(Queue->Fifo))==0);
    Queue->State=RMP_MSGQ_USED;
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Msgq_Crt *************************************************/

/* Function:RMP_Msgq_Del ******************************************************
Description : Delete a message queue. Only message queues that are empty may be
              deleted.
Input       : volatile struct RMP_Msgq* Queue - The pointer to the queue.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Msgq_Del(volatile struct RMP_Msgq* Queue)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue pointer is valid */
    if(Queue==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();

#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue structure is in use */
    if(Queue->State!=RMP_MSGQ_USED)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* See if the FIFO could be deleted */
    if(RMP_Fifo_Del(&(Queue->Fifo))<0)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Proceed to delete the companion semaphore */
    RMP_ASSERT(RMP_Sem_Del(&(Queue->Sem))==0);
    Queue->State=RMP_MSGQ_FREE;
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Msgq_Del *************************************************/

/* Function:RMP_Msgq_Snd ******************************************************
Description : Send a message to the message queue.
Input       : volatile struct RMP_Msgq* Queue - The pointer to the queue.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Msgq_Snd(volatile struct RMP_Msgq* Queue,
                       volatile struct RMP_List* Node)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue pointer is valid */
    if(Queue==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the data pointer is valid */
    if(Node==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue structure is in use */
    if(Queue->State!=RMP_MSGQ_USED)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Note the trick here: we have locked the scheduler, so we're safe to 
     * post the semaphore first. We do this lest the semaphore post may fail
     * due to maximum number limit. Semaphore post is in fact safe to use
     * when inside a critical section. */
    if(RMP_Sem_Post(&(Queue->Sem), 1U)<0)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Insert the node */
    RMP_ASSERT(RMP_Fifo_Write(&(Queue->Fifo), Node)==0);
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Msgq_Snd *************************************************/

/* Function:RMP_Msgq_Snd_ISR **************************************************
Description : Send a message to the message queue. This function can only be
              called from an ISR whose priority is below or equal to the context
              switch handler's. We do not check whether the scheduler is locked;
              if we are calling this function, we're pretty sure that it's not.
              We do not check whether the scheduler is locked; if we are calling
              this function, we're pretty sure that it's not.
Input       : volatile struct RMP_Msgq* Queue - The pointer to the queue.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Msgq_Snd_ISR(volatile struct RMP_Msgq* Queue,
                           volatile struct RMP_List* Node)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the the queue pointer is valid */
    if(Queue==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the data pointer is valid */
    if(Node==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the queue structure is in use */
    if(Queue->State!=RMP_MSGQ_USED)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Notify the receiver(s) first */
    if(RMP_Sem_Post_ISR(&(Queue->Sem), 1U)<0)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Then insert node - must be successful */
    RMP_ASSERT(RMP_Fifo_Write_ISR(&(Queue->Fifo), Node)==0);

    return 0;
}
/* End Function:RMP_Msgq_Snd_ISR *********************************************/

/* Function:RMP_Msgq_Rcv ******************************************************
Description : Receive a message from a message queue.
Input       : volatile struct RMP_Msgq* Queue - The pointer to the queue.
              rmp_ptr_t Slice - The number of slices to wait.
Output      : volatile struct RMP_List** Node - The node received.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Msgq_Rcv(volatile struct RMP_Msgq* Queue,
                       volatile struct RMP_List** Node,
                       rmp_ptr_t Slice)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue pointer is valid */
    if(Queue==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the data pointer is valid */
    if(Node==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();

#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue structure is in use */
    if(Queue->State!=RMP_MSGQ_USED)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Try to grab a semaphore, and only when we succeed do we proceed - 
     * there is the possibility that the whole queue gets deleted, so
     * we need to take that into account. Note that the potential 
     * semaphore-FIFO race here is impossible: unlike condition variables,
     * semaphore is counting, so if a thread grabs a semaphore it is 
     * guaranteed that something is in the FIFO. */
    if(RMP_Sem_Pend_Unlock(&(Queue->Sem), Slice)<0)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Grab the data then - we could have a delete race here */
    if(RMP_Fifo_Read(&(Queue->Fifo), Node)<0)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    return 0;
}
/* End Function:RMP_Msgq_Rcv *************************************************/

/* Function:RMP_Msgq_Cnt ******************************************************
Description : Get the number of nodes in the message queue.
Input       : volatile struct RMP_Msgq* Queue - The pointer to the message queue.
Output      : None.
Return      : rmp_ret_t - If successful, the number of nodes; or an error code.
******************************************************************************/
rmp_ret_t RMP_Msgq_Cnt(volatile struct RMP_Msgq* Queue)
{
    rmp_ret_t Count;
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue pointer is valid */
    if(Queue==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue structure is in use */
    if(Queue->State!=RMP_MSGQ_USED)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    Count=RMP_Fifo_Cnt(&(Queue->Fifo));
    
    RMP_Sched_Unlock();
    return Count;
}
/* End Function:RMP_Msgq_Cnt *************************************************/

/* Function:RMP_Bmq_Crt *******************************************************
Description : Create a blocking message queue. A blocking message queue is a
              queue that may block senders in addition to receivers.
              It must be pointed out that providing a core-level semaphore
              variant that will block senders if it reaches limit does NOT help
              implementation of this type of queue. A race between (1) mounting
              the actual info block and (2) notifying the receiver will race no
              matter how they are arranged relative to each other. If (1) is 
              before (2), the actual number of messages on the queue may exceed
              limit when multiple senders are present. If (2) is before (1),
              the receiver may be notified before the actual block is mounted.
              Using normal semaphores do not have this issue because a mounting
              permission is always granted before the actual mounting, and the
              notification of the receiver will follow the actual mounting.
Input       : volatile struct RMP_Bmq* Queue - The pointer to the queue.
              rmp_ptr_t Limit - The message number limit.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Bmq_Crt(volatile struct RMP_Bmq* Queue,
                      rmp_ptr_t Limit)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue pointer is valid */
    if(Queue==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_BMQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();

#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue pointer is in use */
    if(Queue->State!=RMP_BMQ_FREE)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_BMQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the limit is valid */
    if((Limit==0U)||(Limit>=RMP_SEM_CNT_MAX))
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* A blocking queue is just a message queue paired
     * with a number limiting semaphore */
    RMP_ASSERT(RMP_Sem_Crt(&(Queue->Sem),Limit)==0);
    RMP_ASSERT(RMP_Msgq_Crt(&(Queue->Msgq))==0);
    Queue->State=RMP_BMQ_USED;
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Bmq_Crt **************************************************/

/* Function:RMP_Bmq_Del *******************************************************
Description : Delete a blocking message queue. Only blocking message queues that
              are empty may be deleted.
Input       : volatile struct RMP_Bmq* Queue - The pointer to the queue.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Bmq_Del(volatile struct RMP_Bmq* Queue)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue pointer is valid */
    if(Queue==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue structure is in use */
    if(Queue->State!=RMP_BMQ_USED)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_BMQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Attempt to delete the message queue first */
    if(RMP_Msgq_Del(&(Queue->Msgq))<0)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Proceed to delete the companion semaphore */
    RMP_ASSERT(RMP_Sem_Del(&(Queue->Sem))==0);
    Queue->State=RMP_BMQ_FREE;
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Bmq_Del **************************************************/

/* Function:RMP_Bmq_Snd *******************************************************
Description : Send to a blocking message queue.
Input       : volatile struct RMP_Bmq* Queue - The pointer to the queue.
              rmp_ptr_t Slice - The number of slices to wait.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Bmq_Snd(volatile struct RMP_Bmq* Queue,
                      volatile struct RMP_List* Node,
                      rmp_ptr_t Slice)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue pointer is valid */
    if(Queue==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the data pointer is valid */
    if(Node==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the queue structure is in use - no lock needed cause we have 
     * deletion race protection below: assuming the operations can fail. */
    if(Queue->State!=RMP_BMQ_USED)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_BMQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Grab a slot first. If we're unable to do this, we need to exit */
    if(RMP_Sem_Pend(&(Queue->Sem),Slice)<0)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }

    /* Do the message queue send. This can't fail due to semaphore limit
     * because if it was the case the creation of the blocking message queue
     * won't succeed at first. However, if the message queue is deleted, or
     * the wait is aborted, then this could fail somewhere in the middle. */
    if(RMP_Msgq_Snd(&(Queue->Msgq),Node)<0)
    {
        RMP_COV_MARKER();
        
        /* Put the semaphore back - this is for handling aborts, and the user
         * is fully responsible for ABA issues that arise from deletion. */
        if(Queue->State==RMP_BMQ_USED)
        {
            RMP_COV_MARKER();
            
            RMP_Sem_Post(&(Queue->Sem),1U);
        }
        else
        {
            RMP_COV_MARKER();
            /* No action required */
        }
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    return 0;
}
/* End Function:RMP_Bmq_Snd **************************************************/

/* Function:RMP_Bmq_Snd_ISR ***************************************************
Description : Send to a blocking message queue. Different from the normal 
              version, This function does not block (and instead return failure
              when the queue is full) and can only be called from an ISR whose
              priority is below or equal to the context switch handler's. We do
              not check whether the scheduler is locked; if we are calling this
              function, we're pretty sure that it's not.
Input       : volatile struct RMP_Bmq* Queue - The pointer to the queue.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Bmq_Snd_ISR(volatile struct RMP_Bmq* Queue,
                          volatile struct RMP_List* Node)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue pointer is valid */
    if(Queue==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the data pointer is valid */
    if(Node==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the queue structure is in use */
    if(Queue->State!=RMP_BMQ_USED)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_BMQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if we have used the queue up */
    if(Queue->Sem.Num_Cur==0U)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Manually operate the semaphore, then send to message queue */
    Queue->Sem.Num_Cur--;
    RMP_Msgq_Snd_ISR(&(Queue->Msgq), Node);

    return 0;
}
/* End Function:RMP_Bmq_Snd_ISR **********************************************/

/* Function:RMP_Bmq_Rcv *******************************************************
Description : Receive from a blocking message queue.
Input       : volatile struct RMP_Bmq* Queue - The pointer to the queue.
              rmp_ptr_t Slice - The number of slices to wait.
Output      : volatile struct RMP_List** Node - The node received.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Bmq_Rcv(volatile struct RMP_Bmq* Queue,
                      volatile struct RMP_List** Node,
                      rmp_ptr_t Slice)
{
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue pointer is valid */
    if(Queue==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_BMQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the data pointer is valid */
    if(Node==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    /* Check if the queue structure is in use - no lock needed cause we have 
     * deletion race protection below: assuming the operations can fail */
    if(Queue->State!=RMP_BMQ_USED)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_BMQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    /* Attempt a message queue receive */
    if(RMP_Msgq_Rcv(&(Queue->Msgq),Node,Slice)<0)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
                          
    /* If we're successful, wake up one guy on the send wait list. We don't care 
     * if this fails: if this fails due to queue deletion, that's totally fine.
     * Also there is no possibility that the FIFO will (even transiently) contain
     * more elements than the limit: this only gets posted when the FIFO dequeue
     * is actually performed. */
    RMP_Sem_Post(&(Queue->Sem),1U);
    
    return 0;
}
/* End Function:RMP_Bmq_Rcv **************************************************/

/* Function:RMP_Bmq_Cnt *******************************************************
Description : Get the number of nodes in the blocking message queue.
Input       : volatile struct RMP_Bmq* Queue - The pointer to the message queue.
Output      : None.
Return      : rmp_ret_t - If successful, the number of nodes; or an error code.
******************************************************************************/
rmp_ret_t RMP_Bmq_Cnt(volatile struct RMP_Bmq* Queue)
{
    rmp_ret_t Count;

#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue pointer is valid */
    if(Queue==RMP_NULL)
    {
        RMP_COV_MARKER();
        
        return RMP_ERR_BMQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    RMP_Sched_Lock();
    
#if(RMP_CHECK_ENABLE!=0U)
    /* Check if the queue structure is in use */
    if(Queue->State!=RMP_MSGQ_USED)
    {
        RMP_COV_MARKER();
        
        RMP_Sched_Unlock();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
#endif
    
    Count=RMP_Msgq_Cnt(&(Queue->Msgq));
    
    RMP_Sched_Unlock();
    return Count;
}
/* End Function:RMP_Bmq_Cnt **************************************************/

/* Function:RMP_Line **********************************************************
Description : Draw a line given the start and end coordinates.
Input       : rmp_cnt_t Begin_X - The begin point X coordinate.
              rmp_cnt_t Begin_Y - The begin point Y coordinate.
              rmp_cnt_t End_X - The end point X coordinate.
              rmp_cnt_t End_Y - The end point Y coordinate.
              rmp_ptr_t Color - The color of the line.
Output      : None.
Return      : None.
******************************************************************************/
#if(RMP_GUI_ENABLE!=0U)
void RMP_Line(rmp_cnt_t Begin_X,
              rmp_cnt_t Begin_Y,
              rmp_cnt_t End_X,
              rmp_cnt_t End_Y,
              rmp_ptr_t Color)
{
    rmp_cnt_t Trav_X;
    rmp_cnt_t Trav_Y;
    rmp_cnt_t Dir_X;
    rmp_cnt_t Dir_Y;
    rmp_cnt_t Error;
    rmp_cnt_t Cur_X;
    rmp_cnt_t Cur_Y;
    
    /* See if this line is horizontal or vertical. If so we speed it up */
    if(Begin_X==End_X)
    {
        RMP_COV_MARKER();

        /* Vertical */
        if(Begin_Y>End_Y)
        {
            RMP_COV_MARKER();
            
            Dir_Y=End_Y;
            Trav_Y=Begin_Y;
        }
        else
        {
            RMP_COV_MARKER();
            
            Dir_Y=Begin_Y;
            Trav_Y=End_Y;
        }
        
        for(Cur_Y=Dir_Y;Cur_Y<=Trav_Y;Cur_Y++)
        {
            RMP_POINT(Begin_X,Cur_Y,Color);
        }
        return;
    }
    else if(Begin_Y==End_Y)
    {
        RMP_COV_MARKER();

        /* Horizontal */
        if(Begin_X>End_X)
        {
            RMP_COV_MARKER();
            
            Dir_X=End_X;
            Trav_X=Begin_X;
        }
        else
        {
            RMP_COV_MARKER();
            
            Dir_X=Begin_X;
            Trav_X=End_X;
        }
        
        for(Cur_X=Dir_X;Cur_X<=Trav_X;Cur_X++)
        {
            RMP_POINT(Cur_X,Begin_Y,Color);
        }
        return;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }

    Error=0;
    /* Get their absolute value, and then draw the line */
    Trav_X=RMP_ABS(Begin_X,End_X);
    Trav_Y=RMP_ABS(Begin_Y,End_Y);

    /* Decide the increment direction */
    if((End_X-Begin_X)>0)
    {
        RMP_COV_MARKER();
        
        Dir_X=1;
    }
    else
    {
        RMP_COV_MARKER();
        
        Dir_X=-1;
    }

    if((End_Y-Begin_Y)>0)
    {
        RMP_COV_MARKER();
        
        Dir_Y=1;
    }
    else
    {
        RMP_COV_MARKER();
        
        Dir_Y=-1;
    }

    if(Trav_X>Trav_Y)
    {
        RMP_COV_MARKER();

        Cur_Y=Begin_Y;
        for(Cur_X=Begin_X;Cur_X!=(End_X+Dir_X);Cur_X+=Dir_X)
        {
            RMP_POINT(Cur_X,Cur_Y,Color);
            Error+=Trav_Y;
            if(RMP_SAL(Error,1)>=Trav_X)
            {
                RMP_COV_MARKER();
                
                Cur_Y+=Dir_Y;
                Error-=Trav_X;
            }
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }
        }
    }
    else
    {
        RMP_COV_MARKER();

        Cur_X=Begin_X;
        for(Cur_Y=Begin_Y;Cur_Y!=(End_Y+Dir_Y);Cur_Y+=Dir_Y)
        {
            RMP_POINT(Cur_X,Cur_Y,Color);
            Error+=Trav_X;
            if(RMP_SAL(Error,1)>=Trav_Y)
            {
                RMP_COV_MARKER();
                
                Cur_X+=Dir_X;
                Error-=Trav_Y;
            }
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }
        }
    }
}
/* End Function:RMP_Line *****************************************************/

/* Function:RMP_Dot_Line ******************************************************
Description : Draw a dotted line given the start and end coordinates.
Input       : rmp_cnt_t Begin_X - The begin point X coordinate.
              rmp_cnt_t Begin_Y - The begin point Y coordinate.
              rmp_cnt_t End_X - The end point X coordinate.
              rmp_cnt_t End_Y - The end point Y coordinate.
              rmp_ptr_t Dot - The color of the dotted line.
              rmp_ptr_t Space - The color of the white space. "RMP_TRANS" for nothing.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Dot_Line(rmp_cnt_t Begin_X,
                  rmp_cnt_t Begin_Y,
                  rmp_cnt_t End_X,
                  rmp_cnt_t End_Y,
                  rmp_ptr_t Dot,
                  rmp_ptr_t Space)
{
    rmp_cnt_t Trav_X;
    rmp_cnt_t Trav_Y;
    rmp_cnt_t Dir_X;
    rmp_cnt_t Dir_Y;
    rmp_cnt_t Error;
    rmp_cnt_t Cur_X;
    rmp_cnt_t Cur_Y;

    Error=0;
    /* Get their absolute value, and then draw the line */
    Trav_X=RMP_ABS(Begin_X,End_X);
    Trav_Y=RMP_ABS(Begin_Y,End_Y);

    /* Decide the increment direction */
    if((End_X-Begin_X)>0)
    {
        RMP_COV_MARKER();
        
        Dir_X=1;
    }
    else
    {
        RMP_COV_MARKER();
        
        Dir_X=-1;
    }

    if((End_Y-Begin_Y)>0)
    {
        RMP_COV_MARKER();
        
        Dir_Y=1;
    }
    else
    {
        RMP_COV_MARKER();
        
        Dir_Y=-1;
    }

    if(Trav_X>Trav_Y)
    {
        RMP_COV_MARKER();

        Cur_Y=Begin_Y;
        for(Cur_X=Begin_X;Cur_X!=(End_X+Dir_X);Cur_X+=Dir_X)
        {
            /* Draw the dot and the white space alternatively */
            if((Cur_X&0x01)!=0)
            {
                RMP_COV_MARKER();
                
                RMP_POINT(Cur_X,Cur_Y,Dot);
            }
            else
            {
                RMP_COV_MARKER();

                if(Space!=RMP_TRANS)
                {
                    RMP_COV_MARKER();
                    
                    RMP_POINT(Cur_X,Cur_Y,Space);
                }
                else
                {
                    RMP_COV_MARKER();
                    /* No action required */
                }
            }

            Error+=Trav_Y;
            if(RMP_SAL(Error,1)>=Trav_X)
            {
                RMP_COV_MARKER();
                
                Cur_Y+=Dir_Y;
                Error-=Trav_X;
            }
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }
        }
    }
    else
    {
        RMP_COV_MARKER();

        Cur_X=Begin_X;
        for(Cur_Y=Begin_Y;Cur_Y!=(End_Y+Dir_Y);Cur_Y+=Dir_Y)
        {
            /* Draw the dot and the white space alternatively */
            if((((rmp_ptr_t)Cur_Y)&0x01U)!=0U)
            {
                RMP_COV_MARKER();
                
                RMP_POINT(Cur_X,Cur_Y,Dot);
            }
            else
            {
                RMP_COV_MARKER();

                if(Space!=RMP_TRANS)
                {
                    RMP_COV_MARKER();
                    
                    RMP_POINT(Cur_X,Cur_Y,Space);
                }
                else
                {
                    RMP_COV_MARKER();
                    /* No action required */
                }
            }

            Error+=Trav_X;
            if(RMP_SAL(Error,1)>=Trav_Y)
            {
                RMP_COV_MARKER();
                
                Cur_X+=Dir_X;
                Error-=Trav_Y;
            }
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }
        }
    }
}
/* End Function:RMP_Dot_Line *************************************************/

/*Begin Function:RMP_Rectangle ************************************************
Description : Draw a rectangle on the screen. You can choose whether the rectangle 
              is filled or not. By setting the fill color to "RMP_TRANS", the 
              rectangle is not filled.
              The border can also be transparent.
Input       : rmp_cnt_t Coord_X - The X position of The top-left corner.
              rmp_cnt_t Coord_Y - The Y position of The top-left corner.
               rmp_cnt_t Length - The length of the rectangle.
              rmp_cnt_t Width - The width of the rectangle. 
              rmp_ptr_t Border - The color of its boundary.
              rmp_ptr_t Fill - The color filled within its boundary.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Rectangle(rmp_cnt_t Coord_X,
                   rmp_cnt_t Coord_Y,
                   rmp_cnt_t Length,
                   rmp_cnt_t Width,
                   rmp_ptr_t Border,
                   rmp_ptr_t Fill)
{
    rmp_cnt_t Line_Cnt;
    
    if(Fill!=RMP_TRANS)
    {
        RMP_COV_MARKER();
        
        for(Line_Cnt=0;Line_Cnt<Width;Line_Cnt++)
        {
            RMP_Line(Coord_X, Coord_Y+Line_Cnt, Coord_X+Length-1, Coord_Y+Line_Cnt, Fill);
        }
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }

    if(Border!=RMP_TRANS)
    {
        RMP_COV_MARKER();
        
        RMP_Line(Coord_X, Coord_Y, Coord_X+Length-1, Coord_Y, Border);
        RMP_Line(Coord_X+Length-1, Coord_Y, Coord_X+Length-1, Coord_Y+Width-1, Border);
        RMP_Line(Coord_X+Length-1, Coord_Y+Width-1, Coord_X, Coord_Y+Width-1, Border);
        RMP_Line(Coord_X, Coord_Y+Width-1, Coord_X, Coord_Y, Border);
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
}
/* End Function:RMP_Rectangle ************************************************/

/* Function:RMP_Round_Rect ****************************************************
Description : Draw a rectangle with rounded corners.
Input       : rmp_cnt_t Coord_X - The X position of The top-left corner.
              rmp_cnt_t Coord_Y - The Y position of The top-left corner.
              rmp_cnt_t Length - The length of the rectangle.
              rmp_cnt_t Width - The width of the rectangle. 
              rmp_cnt_t Round - The radius of the round corner. 
              rmp_ptr_t Color - The color of the rectangle.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Round_Rect(rmp_cnt_t Coord_X,
                    rmp_cnt_t Coord_Y,
                    rmp_cnt_t Length,
                    rmp_cnt_t Width,
                    rmp_cnt_t Round,
                    rmp_ptr_t Color)
{
    rmp_cnt_t Cir_X_0;
    rmp_cnt_t Cir_X_1;
    rmp_cnt_t Cir_Y_0;
    rmp_cnt_t Cir_Y_1;
    
    Cir_X_0=Coord_X+Round+1;
    Cir_X_1=Coord_X+Length-Round-1;
    Cir_Y_0=Coord_Y+Round+1;
    Cir_Y_1=Coord_Y+Width-Round-1;
    
    /* Draw the innermost one rectangle */
    RMP_Rectangle(Coord_X+Round+1, Coord_Y+Round+1, Length-Round-Round-2, Width-Round-Round-2, Color, Color);

    /* Draw 4 small side rectangles */
    RMP_Rectangle(Coord_X, Cir_Y_0, Round+1, Width-Round-Round-2, Color, Color);
    RMP_Rectangle(Cir_X_1, Cir_Y_0, Round+1, Width-Round-Round-2, Color, Color);
    RMP_Rectangle(Cir_X_0, Cir_Y_1, Length-Round-Round-2, Round+1, Color, Color);
    RMP_Rectangle(Cir_X_0, Coord_Y, Length-Round-Round-2, Round+1, Color, Color);
    
    /* Draw 4 circles */
    RMP_Circle(Cir_X_0, Cir_Y_0, Round, Color, Color);
    RMP_Circle(Cir_X_1, Cir_Y_0, Round, Color, Color);
    RMP_Circle(Cir_X_0, Cir_Y_1, Round, Color, Color);
    RMP_Circle(Cir_X_1, Cir_Y_1, Round, Color, Color);
}
/* End Function:RMP_Round_Rect ***********************************************/

/* Function:RMP_Circle ********************************************************
Description : Draw a circle on the screen. You can choose whether the circle 
              is filled or not. By setting the fill color to "RMP_TRANS", the 
              circle is not filled. Here, the arc function is not supported.
              Here we utilize the Bresenham algorithm to draw the circle.
              In this algorithm, we draw a 1/8 circle first, then we make use of
              the symmetry of the circle to get the rest of the circle.
Input       : rmp_cnt_t Center_X - The circle center's X coordinate.
              rmp_cnt_t Center_Y - The circle center's Y coordinate.
              rmp_cnt_t Radius - The radius of the circle.
              rmp_ptr_t Border -The color of its boundary.
              rmp_ptr_t Fill - The color filled within its boundary.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Circle(rmp_cnt_t Center_X,
                rmp_cnt_t Center_Y,
                rmp_cnt_t Radius,
                rmp_ptr_t Border,
                rmp_ptr_t Fill)
{
    rmp_cnt_t Cur_X;
    rmp_cnt_t Cur_Y;
    rmp_cnt_t Fill_Y;
    rmp_cnt_t Error;  
    rmp_cnt_t Quick;

    Cur_X=0;
    Cur_Y=Radius;
    Error=3-RMP_SAL(Radius,1);

    if(Fill!=RMP_TRANS)
    {
        RMP_COV_MARKER();

        /* When we are filling the circle, we can try not to fill the areas that have been filled
         * using the rectangle fill method to increase fill speed.
         * Square 2 is 1.414. Here we let the condition to be 1.5. */
        Quick=RMP_SAL(Radius,1)/3;
        /* First, fill the square hole using the fast direct-fill method */
        RMP_Rectangle(Center_X-Quick, Center_Y-Quick, RMP_SAL(Quick,1)+1, RMP_SAL(Quick,1)+1, Fill, Fill);

        while(Cur_X<=Cur_Y) 
        {  
            if(Cur_X<Quick)
            {
                RMP_COV_MARKER();

                for(Fill_Y=Cur_X;Fill_Y<=Cur_Y;Fill_Y++)  
                {
                    if(Fill_Y<Quick)
                    {
                        RMP_COV_MARKER();
                        
                        continue;
                    }
                    else
                    {
                        RMP_COV_MARKER();
                        /* No action required */
                    }
                    
                    RMP_POINT(Center_X+Cur_X, Center_Y+Fill_Y, Fill);  
                    RMP_POINT(Center_X-Cur_X, Center_Y+Fill_Y, Fill);  
                    RMP_POINT(Center_X+Cur_X, Center_Y-Fill_Y, Fill); 
                    RMP_POINT(Center_X-Cur_X, Center_Y-Fill_Y, Fill);  
                    RMP_POINT(Center_X+Fill_Y, Center_Y+Cur_X, Fill); 
                    RMP_POINT(Center_X-Fill_Y, Center_Y+Cur_X, Fill);  
                    RMP_POINT(Center_X+Fill_Y, Center_Y-Cur_X, Fill);  
                    RMP_POINT(Center_X-Fill_Y, Center_Y-Cur_X, Fill);
                }
            }
            /* Here the "Cur_X" is already out of range. We do not check the conditions anymore */
            else
            {
                RMP_COV_MARKER();

                for(Fill_Y=Cur_X;Fill_Y<=Cur_Y;Fill_Y++)  
                {                   
                    RMP_POINT(Center_X+Cur_X, Center_Y+Fill_Y, Fill);  
                    RMP_POINT(Center_X-Cur_X, Center_Y+Fill_Y, Fill);  
                    RMP_POINT(Center_X+Cur_X, Center_Y-Fill_Y, Fill); 
                    RMP_POINT(Center_X-Cur_X, Center_Y-Fill_Y, Fill);  
                    RMP_POINT(Center_X+Fill_Y, Center_Y+Cur_X, Fill); 
                    RMP_POINT(Center_X-Fill_Y, Center_Y+Cur_X, Fill);  
                    RMP_POINT(Center_X+Fill_Y, Center_Y-Cur_X, Fill);  
                    RMP_POINT(Center_X-Fill_Y, Center_Y-Cur_X, Fill);
                }
            }
            RMP_POINT(Center_X+Cur_X, Center_Y+Cur_Y, Border);  
            RMP_POINT(Center_X-Cur_X, Center_Y+Cur_Y, Border);  
            RMP_POINT(Center_X+Cur_X, Center_Y-Cur_Y, Border); 
            RMP_POINT(Center_X-Cur_X, Center_Y-Cur_Y, Border);  
            RMP_POINT(Center_X+Cur_Y, Center_Y+Cur_X, Border); 
            RMP_POINT(Center_X-Cur_Y, Center_Y+Cur_X, Border);  
            RMP_POINT(Center_X+Cur_Y, Center_Y-Cur_X, Border);  
            RMP_POINT(Center_X-Cur_Y, Center_Y-Cur_X, Border);

            if(Error<0) 
            {
                RMP_COV_MARKER();
                
                Error+=RMP_SAL(Cur_X,2)+6;
            }
            else 
            {  
                RMP_COV_MARKER();
                
                Error+=RMP_SAL((rmp_ptr_t)Cur_X-(rmp_ptr_t)Cur_Y,2)+10;
                Cur_Y--;  
            }

            Cur_X++;  
        }
    }
    else 
    {
        RMP_COV_MARKER();

        /* Border only */ 
        while(Cur_X<=Cur_Y) 
        {
            RMP_POINT(Center_X+Cur_X, Center_Y+Cur_Y, Border);  
            RMP_POINT(Center_X-Cur_X, Center_Y+Cur_Y, Border);  
            RMP_POINT(Center_X+Cur_X, Center_Y-Cur_Y, Border); 
            RMP_POINT(Center_X-Cur_X, Center_Y-Cur_Y, Border);  
            RMP_POINT(Center_X+Cur_Y, Center_Y+Cur_X, Border); 
            RMP_POINT(Center_X-Cur_Y, Center_Y+Cur_X, Border);  
            RMP_POINT(Center_X+Cur_Y, Center_Y-Cur_X, Border);  
            RMP_POINT(Center_X-Cur_Y, Center_Y-Cur_X, Border);

            if(Error<0)
            {
                RMP_COV_MARKER();
                
                Error+=RMP_SAL(Cur_X,2)+6;
            }
            else 
            {
                RMP_COV_MARKER();
                
                Error+=RMP_SAL((rmp_ptr_t)Cur_X-(rmp_ptr_t)Cur_Y,2);
                Cur_Y--;
            }

            Cur_X++;  
        }  
    }
}
/* End Function:RMP_Circle ***************************************************/

/* Function:RMP_Matrix ********************************************************
Description : Display a monochrome bit map in the given color.
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              const rmp_u8_t* Matrix - The data matrix.
              rmp_ptr_t Bit_Order - The bit ordering.
              rmp_cnt_t Length - The length of the picture.
              rmp_cnt_t Width - The width of the picture.
              rmp_ptr_t Color - The color to display this with.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Matrix(rmp_cnt_t Coord_X,
                rmp_cnt_t Coord_Y,
                const rmp_u8_t* Matrix,
                rmp_ptr_t Bit_Order,
                rmp_cnt_t Length,
                rmp_cnt_t Width,
                rmp_ptr_t Color)
{
    rmp_cnt_t Len_Cnt;
    rmp_cnt_t Wid_Cnt;
    rmp_cnt_t Mat_Pos;
    
    if((Matrix==RMP_NULL)||(Length==0)||(Width==0)||((((rmp_u8_t)Length)&0x07U)!=0U))
    {
        RMP_COV_MARKER();
        
        return;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    Mat_Pos=0;
    /* Natural order */
    if(Bit_Order==RMP_MAT_BIG)
    {
        RMP_COV_MARKER();

        for(Wid_Cnt=Coord_Y;Wid_Cnt<(Width+Coord_Y);Wid_Cnt++)
        {
            for(Len_Cnt=Coord_X;Len_Cnt<(Length+Coord_X);Len_Cnt++)
            {
                if(RMP_MAT_BPOS(Matrix,Mat_Pos)!=0U)
                {
                    RMP_COV_MARKER();
                    
                    RMP_POINT(Len_Cnt,Wid_Cnt,Color);
                }
                else
                {
                    RMP_COV_MARKER();
                    /* No action required */
                }

                Mat_Pos++;
            }
        }
    }
    /* Small order */
    else
    {
        RMP_COV_MARKER();

        for(Wid_Cnt=Coord_Y;Wid_Cnt<(Width+Coord_Y);Wid_Cnt++)
        {
            for(Len_Cnt=Coord_X;Len_Cnt<(Length+Coord_X);Len_Cnt++)
            {
                if(RMP_MAT_SPOS(Matrix,Mat_Pos)!=0U)
                {
                    RMP_COV_MARKER();
                    
                    RMP_POINT(Len_Cnt,Wid_Cnt,Color);
                }
                else
                {
                    RMP_COV_MARKER();
                    /* No action required */
                }

                Mat_Pos++;
            }
        }
    }
}
/* End Function:RMP_Matrix ***************************************************/

/* Function:RMP_Matrix_AA *****************************************************
Description : Display a monochrome bit map in the given color, with simple 
              anti-aliasing (FXAA 1X algorithm).
              When using this, three color mixing macros must be provided.
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              const rmp_u8_t* Matrix - The data matrix.
              rmp_ptr_t Bit_Order - The bit ordering.
              rmp_cnt_t Length - The length of the picture.
              rmp_cnt_t Width - The width of the picture.
              rmp_ptr_t Color - The color to display this with.
              rmp_ptr_t Back - The background color, for anti-aliasing.
Output      : None.
Return      : None.
******************************************************************************/
#if(RMP_GUI_ANTIALIAS_ENABLE!=0U)
void RMP_Matrix_AA(rmp_cnt_t Coord_X,
                   rmp_cnt_t Coord_Y,
                   const rmp_u8_t* Matrix,
                   rmp_ptr_t Bit_Order,
                   rmp_cnt_t Length,
                   rmp_cnt_t Width,
                   rmp_ptr_t Color,
                   rmp_ptr_t Back)
{
    rmp_cnt_t Len_Cnt;
    rmp_cnt_t Wid_Cnt;
    rmp_cnt_t Mat_Pos;
    rmp_cnt_t AA_Pos;
    rmp_cnt_t AA_Val;
    rmp_ptr_t Color_25;
    rmp_ptr_t Color_50;
    rmp_ptr_t Color_75;
    rmp_cnt_t Total;
    
    if((Matrix==RMP_NULL)||(Length==0)||(Width==0)||((((rmp_u8_t)Length)&0x07U)!=0U))
    {
        RMP_COV_MARKER();
        
        return;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    Mat_Pos=0;
    Total=Length*Width;
    Color_25=RMP_COLOR_25P(Color,Back);
    Color_50=RMP_COLOR_50P(Color,Back);
    Color_75=RMP_COLOR_75P(Color,Back);
    
    /* Natural order */
    if(Bit_Order==RMP_MAT_BIG)
    {
        RMP_COV_MARKER();

        for(Wid_Cnt=Coord_Y;Wid_Cnt<(Width+Coord_Y);Wid_Cnt++)
        {
            for(Len_Cnt=Coord_X;Len_Cnt<(Length+Coord_X);Len_Cnt++)
            {
                if(RMP_MAT_BPOS(Matrix, Mat_Pos)!=0U)
                {
                    RMP_COV_MARKER();
                    
                    RMP_POINT(Len_Cnt, Wid_Cnt, Color);
                }
                else
                {
                    RMP_COV_MARKER();

                    /* Anti-aliasing */
                    AA_Val=0;
                    
                    AA_Pos=Mat_Pos-1;
                    if(AA_Pos>=0)
                    {
                        RMP_COV_MARKER();

                        if((Len_Cnt!=Coord_X)&&(RMP_MAT_BPOS(Matrix, AA_Pos)!=0U))
                        {
                            RMP_COV_MARKER();
                            
                            AA_Val++;
                        }
                        else
                        {
                            RMP_COV_MARKER();
                            /* No action required */
                        }

                        AA_Pos=Mat_Pos-Length;
                        if((AA_Pos>=0)&&(RMP_MAT_BPOS(Matrix, AA_Pos)!=0U))
                        {
                            RMP_COV_MARKER();
                            
                            AA_Val++;
                        }
                        else
                        {
                            RMP_COV_MARKER();
                            /* No action required */
                        }
                    }
                    else
                    {
                        RMP_COV_MARKER();
                        /* No action required */
                    }
                    
                    AA_Pos=Mat_Pos+1;
                    if(AA_Pos<Total)
                    {
                        RMP_COV_MARKER();

                        if((Len_Cnt!=(Coord_X+Length-1))&&(RMP_MAT_BPOS(Matrix, AA_Pos)!=0U))
                        {
                            RMP_COV_MARKER();
                            
                            AA_Val++;
                        }
                        else
                        {
                            RMP_COV_MARKER();
                            /* No action required */
                        }

                        AA_Pos=Mat_Pos+Length;
                        if((AA_Pos<Total)&&(RMP_MAT_BPOS(Matrix, AA_Pos)!=0U))
                        {
                            RMP_COV_MARKER();
                            
                            AA_Val++;
                        }
                        else
                        {
                            RMP_COV_MARKER();
                            /* No action required */
                        }
                    }
                    else
                    {
                        RMP_COV_MARKER();
                        /* No action required */
                    }
                    
                    switch(AA_Val)
                    {
                        case 1:
                        {
                            RMP_COV_MARKER();
                            
                            RMP_POINT(Len_Cnt, Wid_Cnt, Color_25);
                            break;
                        }
                        case 2:
                        {
                            RMP_COV_MARKER();
                            
                            RMP_POINT(Len_Cnt, Wid_Cnt, Color_50);
                            break;
                        }
                        case 3:
                        {
                            RMP_COV_MARKER();
                            
                            RMP_POINT(Len_Cnt, Wid_Cnt, Color_75);
                            break;
                        }
                        default:
                        {
                            RMP_COV_MARKER();
                            
                            break;
                        }
                    }
                }
                
                Mat_Pos++;
            }
        }
    }
    /* Small order */
    else
    {
        RMP_COV_MARKER();

        for(Wid_Cnt=Coord_Y;Wid_Cnt<(Width+Coord_Y);Wid_Cnt++)
        {
            for(Len_Cnt=Coord_X;Len_Cnt<(Length+Coord_X);Len_Cnt++)
            {
                if(RMP_MAT_SPOS(Matrix, Mat_Pos)!=0U)
                {
                    RMP_COV_MARKER();
                    
                    RMP_POINT(Len_Cnt, Wid_Cnt, Color);
                }
                else
                {
                    RMP_COV_MARKER();

                    /* Anti-aliasing */
                    AA_Val=0;

                    AA_Pos=Mat_Pos-1;
                    if(AA_Pos>=0)
                    {
                        RMP_COV_MARKER();

                        if((Len_Cnt!=Coord_X)&&(RMP_MAT_SPOS(Matrix, AA_Pos)!=0U))
                        {
                            RMP_COV_MARKER();
                            
                            AA_Val++;
                        }
                        else
                        {
                            RMP_COV_MARKER();
                            /* No action required */
                        }

                        AA_Pos=Mat_Pos-Length;
                        if((AA_Pos>=0)&&(RMP_MAT_SPOS(Matrix, AA_Pos)!=0U))
                        {
                            RMP_COV_MARKER();
                            
                            AA_Val++;
                        }
                        else
                        {
                            RMP_COV_MARKER();
                            /* No action required */
                        }
                    }
                    else
                    {
                        RMP_COV_MARKER();
                        /* No action required */
                    }

                    AA_Pos=Mat_Pos+1;
                    if(AA_Pos<Total)
                    {
                        RMP_COV_MARKER();

                        if((Len_Cnt!=(Coord_X+Length-1))&&(RMP_MAT_SPOS(Matrix, AA_Pos)!=0U))
                        {
                            RMP_COV_MARKER();
                            
                            AA_Val++;
                        }
                        else
                        {
                            RMP_COV_MARKER();
                            /* No action required */
                        }

                        AA_Pos=Mat_Pos+Length;
                        if((AA_Pos<Total)&&(RMP_MAT_SPOS(Matrix, AA_Pos)!=0U))
                        {
                            RMP_COV_MARKER();
                            
                            AA_Val++;
                        }
                        else
                        {
                            RMP_COV_MARKER();
                            /* No action required */
                        }
                    }
                    else
                    {
                        RMP_COV_MARKER();
                        /* No action required */
                    }
                    
                    switch(AA_Val)
                    {
                        case 1:
                        {
                            RMP_COV_MARKER();
                            
                            RMP_POINT(Len_Cnt, Wid_Cnt, Color_25);
                            break;
                        }
                        case 2:
                        {
                            RMP_COV_MARKER();
                            
                            RMP_POINT(Len_Cnt, Wid_Cnt, Color_50);
                            break;
                        }
                        case 3:
                        {
                            RMP_COV_MARKER();
                            
                            RMP_POINT(Len_Cnt, Wid_Cnt, Color_75);
                            break;
                        }
                        default:
                        {
                            RMP_COV_MARKER();
                            
                            break;
                        }
                    }
                }
                
                Mat_Pos++;
            }
        }
    }
}
#endif
/* End Function:RMP_Matrix_AA ************************************************/

/* Function:RMP_Cursor ********************************************************
Description : Draw a cursor (size 16x16) on the screen.
Input       : rmp_cnt_t Coord_X - The X coordinate of the cursor.
              rmp_cnt_t Coord_Y - The Y coordinate of the cursor.
              rmp_ptr_t Style - The style of the mouse. Can be one of the following:
              RMP_CUR_NORM         [0x00]    The normal arrow cursor.
              RMP_CUR_BUSY         [0x01]    The cursor with a busy mark.
              RMP_CUR_QUESTION     [0x02]    The cursor with a question mark.
              RMP_CUR_HAND         [0x03]    The cursor for hyperlinks.
              RMP_CUR_TEXT         [0x04]    The text editing cursor.
              RMP_CUR_STOP         [0x05]    The "not available" cursor.
              RMP_CUR_MOVE         [0x06]    The cursor for moving the window.
              RMP_CUR_LR           [0x07]    Left-to-right double arrow.
              RMP_CUR_UD           [0x08]    Up-to-down double arrow.
              RMP_CUR_ULBR         [0x09]    Upper-left-to-bottom-right double arrow.
              RMP_CUR_URBL         [0x0A]    Upper-right-to-bottom-left double arrow.
              RMP_CUR_CROSS        [0x0B]    Crosshair cursor.
Output      : None.
Return      : None.
******************************************************************************/
#if(RMP_GUI_WIDGET_ENABLE!=0U)
void RMP_Cursor(rmp_cnt_t Coord_X,
                rmp_cnt_t Coord_Y,
                rmp_ptr_t Style)
{
    rmp_cnt_t Count;
    rmp_cnt_t Size_B;
    rmp_cnt_t Size_W;
    const rmp_u8_t* Black;
    const rmp_u8_t* White;

    /* Normal Mouse - all matrix encoded in (Y,X) */
    static const rmp_u8_t Arrow_B[]=
    {
        0x00U, 0x10U, 0x11U, 0x20U, 0x22U, 0x30U, 0x33U, 0x40U,
        0x44U, 0x50U, 0x55U, 0x60U, 0x66U, 0x70U, 0x77U, 0x80U,
        0x88U, 0x90U, 0x95U, 0x96U, 0x97U, 0x98U, 0x99U, 0xA0U,
        0xA2U, 0xA3U, 0xA5U, 0xB0U, 0xB1U, 0xB3U, 0xB6U, 0xC0U,
        0xC1U, 0xC4U, 0xC6U, 0xD0U, 0xD4U, 0xD7U, 0xE5U, 0xE7U,
        0xF5U, 0xF6U
    };
    static const rmp_u8_t Arrow_W[]=
    {
        0x21U, 0x31U, 0x32U, 0x41U, 0x42U, 0x43U, 0x51U, 0x52U,
        0x53U, 0x54U, 0x61U, 0x62U, 0x63U, 0x64U, 0x65U, 0x71U,
        0x72U, 0x73U, 0x74U, 0x75U, 0x76U, 0x81U, 0x82U, 0x83U,
        0x84U, 0x85U, 0x86U, 0x87U, 0x91U, 0x92U, 0x93U, 0x94U,
        0xA1U, 0xA4U, 0xB4U, 0xB5U, 0xC5U, 0xD5U, 0xD6U, 0xE6U
    };
    /* Busy mark - no white part */
    static const rmp_u8_t Busy_B[]=
    {
        0x0AU, 0x0BU, 0x0CU, 0x0DU, 0x0EU, 0x1AU, 0x1BU, 0x1DU,
        0x1EU, 0x2AU, 0x2CU, 0x2EU, 0x3BU, 0x3DU, 0x4CU, 0x5BU,
        0x5DU, 0x6AU, 0x6EU, 0x7AU, 0x7CU, 0x7EU, 0x8AU, 0x8BU,
        0x8CU, 0x8DU, 0x8EU
    };
    static const rmp_u8_t Busy_W[]=
    {
        0x1CU, 0x2BU, 0x2DU, 0x3CU, 0x5CU, 0x6BU, 0x6CU, 0x6DU,
        0x7BU, 0x7DU
    };
    /* Question mark - no white part */
    static const rmp_u8_t Question[]=
    {
        0x0BU, 0x0CU, 0x0DU, 0x1AU, 0x1BU, 0x1CU, 0x1DU, 0x1EU,
        0x29U, 0x2AU, 0x2BU, 0x2DU, 0x2EU, 0x2FU, 0x39U, 0x3AU,
        0x3EU, 0x3FU, 0x4AU, 0x4DU, 0x4EU, 0x4FU, 0x5CU, 0x5DU,
        0x5EU, 0x6CU, 0x6DU, 0x8DU, 0x8EU, 0x9DU, 0x9EU
    };
    /* Hand-shaped cursor */
    static const rmp_u8_t Hand_B[]=
    {
        0x05U, 0x06U, 0x14U, 0x17U, 0x24U, 0x27U, 0x34U, 0x37U,
        0x39U, 0x44U, 0x47U, 0x48U, 0x4AU, 0x4CU, 0x51U, 0x52U,
        0x54U, 0x57U, 0x5AU, 0x5BU, 0x5DU, 0x5EU, 0x60U, 0x63U,
        0x64U, 0x67U, 0x6AU, 0x6DU, 0x6FU, 0x70U, 0x74U, 0x77U,
        0x7AU, 0x7DU, 0x7FU, 0x80U, 0x8FU, 0x90U, 0x9FU, 0xA0U,
        0xAFU, 0xB0U, 0xBFU, 0xC1U, 0xCEU, 0xD1U, 0xD2U, 0xD3U,
        0xD4U, 0xD5U, 0xD6U, 0xD7U, 0xD8U, 0xD9U, 0xDAU, 0xDBU,
        0xDCU, 0xDDU, 0xDEU, 0xE1U, 0xEEU, 0xF2U, 0xF3U, 0xF4U,
        0xF5U, 0xF6U, 0xF7U, 0xF8U, 0xF9U, 0xFAU, 0xFBU, 0xFCU,
        0xFDU
    };
    static const rmp_u8_t Hand_W[]=
    {
        0x15U, 0x16U, 0x25U, 0x26U, 0x35U, 0x36U, 0x45U, 0x46U,
        0x49U, 0x55U, 0x56U, 0x58U, 0x59U, 0x5CU, 0x61U, 0x62U,
        0x65U, 0x66U, 0x68U, 0x69U, 0x6BU, 0x6CU, 0x6EU, 0x71U,
        0x72U, 0x73U, 0x75U, 0x76U, 0x78U, 0x79U, 0x7BU, 0x7CU,
        0x7EU, 0x81U, 0x82U, 0x83U, 0x84U, 0x85U, 0x86U, 0x87U,
        0x88U, 0x89U, 0x8AU, 0x8BU, 0x8CU, 0x8DU, 0x8EU, 0x91U,
        0x92U, 0x93U, 0x94U, 0x95U, 0x96U, 0x97U, 0x98U, 0x99U,
        0x9AU, 0x9BU, 0x9CU, 0x9DU, 0x9EU, 0xA1U, 0xA2U, 0xA3U,
        0xA4U, 0xA5U, 0xA6U, 0xA7U, 0xA8U, 0xA9U, 0xAAU, 0xABU,
        0xACU, 0xADU, 0xAEU, 0xB1U, 0xB2U, 0xB3U, 0xB4U, 0xB5U,
        0xB6U, 0xB7U, 0xB8U, 0xB9U, 0xBAU, 0xBBU, 0xBCU, 0xBDU,
        0xBEU, 0xC2U, 0xC3U, 0xC4U, 0xC5U, 0xC6U, 0xC7U, 0xC8U,
        0xC9U, 0xCAU, 0xCBU, 0xCCU, 0xCDU, 0xE2U, 0xE3U, 0xE4U,
        0xE5U, 0xE6U, 0xE7U, 0xE8U, 0xE9U, 0xEAU, 0xEBU, 0xECU,
        0xEDU
    };
    /* The cursor for text editing - no white part */
    static const rmp_u8_t Text[]=
    {
        0x00U, 0x01U, 0x02U, 0x04U, 0x05U, 0x06U, 0x13U, 0x23U,
        0x33U, 0x43U, 0x53U, 0x63U, 0x73U, 0x83U, 0x93U, 0xA3U,
        0xB3U, 0xC3U, 0xD3U, 0xE3U, 0xF0U, 0xF1U, 0xF2U, 0xF4U,
        0xF5U, 0xF6U
    };
    /* The stop cursor */
    static const rmp_u8_t Stop_B[]=
    {
        0x05U, 0x06U, 0x07U, 0x08U, 0x13U, 0x14U, 0x15U, 0x16U,
        0x17U, 0x18U, 0x19U, 0x1AU, 0x22U, 0x23U, 0x24U, 0x29U,
        0x2AU, 0x2BU, 0x31U, 0x32U, 0x33U, 0x34U, 0x3AU, 0x3BU,
        0x3CU, 0x41U, 0x42U, 0x43U, 0x44U, 0x45U, 0x4BU, 0x4CU,
        0x50U, 0x51U, 0x54U, 0x55U, 0x56U, 0x5CU, 0x5DU, 0x60U,
        0x61U, 0x65U, 0x66U, 0x67U, 0x6CU, 0x6DU, 0x70U, 0x71U,
        0x76U, 0x77U, 0x78U, 0x7CU, 0x7DU, 0x80U, 0x81U, 0x87U,
        0x88U, 0x89U, 0x8CU, 0x8DU, 0x91U, 0x92U, 0x98U, 0x99U,
        0x9AU, 0x9BU, 0x9CU, 0xA1U, 0xA2U, 0xA3U, 0xA9U, 0xAAU,
        0xABU, 0xACU, 0xB2U, 0xB3U, 0xB4U, 0xB9U, 0xBAU, 0xBBU,
        0xC3U, 0xC4U, 0xC5U, 0xC6U, 0xC7U, 0xC8U, 0xC9U, 0xCAU,
        0xD5U, 0xD6U, 0xD7U, 0xD8U
    };
    static const rmp_u8_t Stop_W[]=
    {
        0x25U, 0x26U, 0x27U, 0x28U, 0x35U, 0x36U, 0x37U, 0x38U,
        0x39U, 0x46U, 0x47U, 0x48U, 0x49U, 0x4AU, 0x52U, 0x53U,
        0x57U, 0x58U, 0x59U, 0x5AU, 0x5BU, 0x62U, 0x63U, 0x64U,
        0x68U, 0x69U, 0x6AU, 0x6BU, 0x72U, 0x73U, 0x74U, 0x75U,
        0x79U, 0x7AU, 0x7BU, 0x82U, 0x83U, 0x84U, 0x85U, 0x86U,
        0x8AU, 0x8BU, 0x93U, 0x94U, 0x95U, 0x96U, 0x97U, 0xA4U,
        0xA5U, 0xA6U, 0xA7U, 0xA8U, 0xB5U, 0xB6U, 0xB7U, 0xB8U
    };
       /* Cross arrow - no white part  */
    static const rmp_u8_t Adj_ALL[]=
    {
        0x07U, 0x16U, 0x17U, 0x18U, 0x25U, 0x26U, 0x27U, 0x28U,
        0x29U, 0x37U, 0x47U, 0x52U, 0x57U, 0x5CU, 0x61U, 0x62U,
        0x67U, 0x6CU, 0x6DU, 0x70U, 0x71U, 0x72U, 0x73U, 0x74U,
        0x75U, 0x76U, 0x77U, 0x78U, 0x79U, 0x7AU, 0x7BU, 0x7CU,
        0x7DU, 0x7EU, 0x81U, 0x82U, 0x87U, 0x8CU, 0x8DU, 0x92U,
        0x97U, 0x9CU, 0xA7U, 0xB7U, 0xC5U, 0xC6U, 0xC7U, 0xC8U,
        0xC9U, 0xD6U, 0xD7U, 0xD8U, 0xE7U
    };    
    /* Left to right arrow - no white part */
    static const rmp_u8_t Adj_LR[]=
    {
        0x52U, 0x5DU, 0x61U, 0x62U, 0x6DU, 0x6EU, 0x70U, 0x71U,
        0x72U, 0x73U, 0x74U, 0x75U, 0x76U, 0x77U, 0x78U, 0x79U,
        0x7AU, 0x7BU, 0x7CU, 0x7DU, 0x7EU, 0x7FU, 0x81U, 0x82U,
        0x8DU, 0x8EU, 0x92U, 0x9DU
    };
    /* Up to down arrow - no white part */
    static const rmp_u8_t Adj_UD[]=
    {
        0x07U, 0x16U, 0x17U, 0x18U, 0x25U, 0x26U, 0x27U, 0x28U,
        0x29U, 0x37U, 0x47U, 0x57U, 0x67U, 0x77U, 0x87U, 0x97U,
        0xA7U, 0xB7U, 0xC7U, 0xD5U, 0xD6U, 0xD7U, 0xD8U, 0xD9U,
        0xE6U, 0xE7U, 0xE8U, 0xF7U

    };
    /* Bottom-right to top-left arrow - no white part */
    static const rmp_u8_t Adj_ULBR[]=
    {
        0x00U, 0x01U, 0x02U, 0x03U, 0x04U, 0x10U, 0x11U, 0x12U,
        0x13U, 0x20U, 0x21U, 0x22U, 0x30U, 0x31U, 0x33U, 0x40U,
        0x44U, 0x55U, 0x66U, 0x77U, 0x88U, 0x99U, 0xAAU, 0xBBU,
        0xBFU, 0xCCU, 0xCEU, 0xCFU, 0xDDU, 0xDEU, 0xDFU, 0xECU,
        0xEDU, 0xEEU, 0xEFU, 0xFBU, 0xFCU, 0xFDU, 0xFEU, 0xFFU
    };
    /* Bottom-left to top-right arrow - no white part */
    static const rmp_u8_t Adj_URBL[]=
    {
        0x0BU, 0x0CU, 0x0DU, 0x0EU, 0x0FU, 0x1CU, 0x1DU, 0x1EU,
        0x1FU, 0x2DU, 0x2EU, 0x2FU, 0x3CU, 0x3EU, 0x3FU, 0x4BU,
        0x4FU, 0x5AU, 0x69U, 0x78U, 0x87U, 0x96U, 0xA5U, 0xB0U,
        0xB4U, 0xC0U, 0xC1U, 0xC3U, 0xD0U, 0xD1U, 0xD2U, 0xE0U,
        0xE1U, 0xE2U, 0xE3U, 0xF0U, 0xF1U, 0xF2U, 0xF3U, 0xF4U
    };
    /* The crosshair cursor - no white part */
    static const rmp_u8_t Cross[]=
    {
        0x07U, 0x17U, 0x27U, 0x37U, 0x46U, 0x47U, 0x48U, 0x55U,
        0x57U, 0x59U, 0x64U, 0x6AU, 0x70U, 0x71U, 0x72U, 0x73U,
        0x74U, 0x75U, 0x77U, 0x79U, 0x7AU, 0x7BU, 0x7CU, 0x7DU,
        0x7EU, 0x84U, 0x8AU, 0x95U, 0x97U, 0x99U, 0xA6U, 0xA7U,
        0xA8U, 0xB7U, 0xC7U, 0xD7U, 0xE7U
    };
    
    /* Draw them */
    switch(Style)
    {
        case RMP_CUR_NORM:
        {
            RMP_COV_MARKER();
            
            Size_B=sizeof(Arrow_B);
            Size_W=sizeof(Arrow_W);
            Black=Arrow_B;
            White=Arrow_W;
            break;
        }
        case RMP_CUR_BUSY:
        case RMP_CUR_QUESTION:
        {
            RMP_COV_MARKER();

            for(Count=0;Count<(rmp_cnt_t)sizeof(Arrow_B);Count++)
            {
                RMP_POINT(Coord_X+RMP_CUR_XPOS(Arrow_B,Count),
                          Coord_Y+RMP_CUR_YPOS(Arrow_B,Count),RMP_COLOR_BLACK);
            }
            for(Count=0;Count<(rmp_cnt_t)sizeof(Arrow_W);Count++)
            {
                RMP_POINT(Coord_X+RMP_CUR_XPOS(Arrow_W,Count),
                          Coord_Y+RMP_CUR_YPOS(Arrow_W,Count),RMP_COLOR_WHITE);
            }
            
            if(Style==RMP_CUR_BUSY)
            {
                RMP_COV_MARKER();
                
                Size_B=sizeof(Busy_B);
                Size_W=sizeof(Busy_W);
                Black=Busy_B;
                White=Busy_W;
            }
            else
            {
                RMP_COV_MARKER();
                
                Size_B=sizeof(Question);
                Size_W=0;
                Black=Question;
                White=RMP_NULL;
            }
            
            break;
        }       
        case RMP_CUR_HAND:
        {
            RMP_COV_MARKER();
            
            Size_B=sizeof(Hand_B);
            Size_W=sizeof(Hand_W);
            Black=Hand_B;
            White=Hand_W;
            break;
        }
        case RMP_CUR_TEXT:
        {
            RMP_COV_MARKER();
            
            Size_B=sizeof(Text);
            Size_W=0;
            Black=Text;
            White=RMP_NULL;
            break;
        }
        case RMP_CUR_STOP:
        {
            RMP_COV_MARKER();
            
            Size_B=sizeof(Stop_B);
            Size_W=sizeof(Stop_W);
            Black=Stop_B;
            White=Stop_W;
            break;
        }
        case RMP_CUR_MOVE:
        {
            RMP_COV_MARKER();
            
            Size_B=sizeof(Adj_ALL);
            Size_W=0;
            Black=Adj_ALL;
            White=RMP_NULL;
            break;
        }
        case RMP_CUR_LR:
        {
            RMP_COV_MARKER();
            
            Size_B=sizeof(Adj_LR);
            Size_W=0;
            Black=Adj_LR;
            White=RMP_NULL;
            break;
        }
        case RMP_CUR_UD:
        {
            RMP_COV_MARKER();
            
            Size_B=sizeof(Adj_UD);
            Size_W=0;
            Black=Adj_UD;
            White=RMP_NULL;
            break;
        }
        case RMP_CUR_ULBR:
        {
            RMP_COV_MARKER();
            
            Size_B=sizeof(Adj_ULBR);
            Size_W=0;
            Black=Adj_ULBR;
            White=RMP_NULL;
            break;
        }
        case RMP_CUR_URBL:
        {
            RMP_COV_MARKER();
            
            Size_B=sizeof(Adj_URBL);
            Size_W=0;
            Black=Adj_URBL;
            White=RMP_NULL;
            break;
        }
        case RMP_CUR_CROSS:
        {
            RMP_COV_MARKER();
            
            Size_B=sizeof(Cross);
            Size_W=0;
            Black=Cross;
            White=RMP_NULL;
            break;
        }
        default:
        {
            RMP_COV_MARKER();
            
            return;
        }
    }
    
    for(Count=0;Count<Size_B;Count++)
    {
        RMP_POINT(Coord_X+RMP_CUR_XPOS(Black,Count),
                  Coord_Y+RMP_CUR_YPOS(Black,Count),RMP_COLOR_BLACK);
    }
    
    for(Count=0;Count<Size_W;Count++)
    {
        RMP_POINT(Coord_X+RMP_CUR_XPOS(White,Count),
                  Coord_Y+RMP_CUR_YPOS(White,Count),RMP_COLOR_WHITE);
    }
}
/* End Function: RMP_Cursor **************************************************/

/* Function:RMP_Checkbox_Set **************************************************
Description : Set the checbox (tick it).
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the checkbox(also the width).
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Checkbox_Set(rmp_cnt_t Coord_X,
                      rmp_cnt_t Coord_Y,
                      rmp_cnt_t Length)
{
    rmp_cnt_t Count;
    
    /* Draw a tick inside - not using shifts for accuracy */
    for(Count=0;Count<=(2*Length/13);Count++)
    {
        RMP_Line(Coord_X+(4*Length/13)+Count, Coord_Y+(6*Length/13)+Count,
                 Coord_X+(4*Length/13)+Count, Coord_Y+(8*Length/13)+Count, RMP_COLOR_BLACK);
    }
    
    for(Count=0;Count<=(4*Length/13);Count++)
    {
        RMP_Line(Coord_X+(6*Length/13)+Count, Coord_Y+(8*Length/13)-Count,
                 Coord_X+(6*Length/13)+Count, Coord_Y+(10*Length/13)-Count, RMP_COLOR_BLACK);
    }
}
/* End Function:RMP_Checkbox_Set *********************************************/

/* Function:RMP_Checkbox_Clr **************************************************
Description : Clear the checbox (reset it).
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the checkbox(also the width).
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Checkbox_Clr(rmp_cnt_t Coord_X,
                      rmp_cnt_t Coord_Y,
                      rmp_cnt_t Length)
{
    rmp_cnt_t Count;
    
    /* Erase the tick inside - not using shifts for accuracy */
    for(Count=0;Count<=(2*Length/13);Count++)
    {
        RMP_Line(Coord_X+(4*Length/13)+Count, Coord_Y+(6*Length/13)+Count,
                 Coord_X+(4*Length/13)+Count, Coord_Y+(8*Length/13)+Count, RMP_COLOR_WHITE);
    }
    
    for(Count=0;Count<=(4*Length/13);Count++)
    {
        RMP_Line(Coord_X+(6*Length/13)+Count, Coord_Y+(8*Length/13)-Count,
                 Coord_X+(6*Length/13)+Count, Coord_Y+(10*Length/13)-Count, RMP_COLOR_WHITE);
    }
}
/* End Function:RMP_Checkbox_Clr *********************************************/

/* Function:RMP_Checkbox ******************************************************
Description : Draw the checkbox according to the parameters.
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the checkbox(also the width).
              rmp_ptr_t Status - The checkbox status.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Checkbox(rmp_cnt_t Coord_X,
                  rmp_cnt_t Coord_Y,
                  rmp_cnt_t Length,
                  rmp_ptr_t Status)    
{   
    /* Clear the area */
    RMP_Rectangle(Coord_X, Coord_Y, Length, Length, RMP_COLOR_WHITE, RMP_COLOR_WHITE);
                                                                     
    /* Draw checkbox outline */
    RMP_Line(Coord_X, Coord_Y, Coord_X+Length-1, Coord_Y, RMP_COLOR_DARK);                 
    RMP_Line(Coord_X+Length-1, Coord_Y, Coord_X+Length-1, Coord_Y+Length-1, RMP_COLOR_WHITE);
    RMP_Line(Coord_X+Length-1, Coord_Y+Length-1, Coord_X, Coord_Y+Length-1, RMP_COLOR_WHITE);
    RMP_Line(Coord_X, Coord_Y+Length-1, Coord_X, Coord_Y, RMP_COLOR_DARK);

    RMP_Line(Coord_X+1, Coord_Y+1, Coord_X+Length-2, Coord_Y+1, RMP_COLOR_BLACK);
    RMP_Line(Coord_X+Length-2, Coord_Y+1, Coord_X+Length-2, Coord_Y+Length-2, RMP_COLOR_DARK);
    RMP_Line(Coord_X+Length-2, Coord_Y+Length-2, Coord_X, Coord_Y+Length-2, RMP_COLOR_DARK);
    RMP_Line(Coord_X+1, Coord_Y+Length-2, Coord_X+1, Coord_Y+1, RMP_COLOR_BLACK);
    
    if(Status!=0U)
    {
        RMP_COV_MARKER();
        
        RMP_Checkbox_Set(Coord_X, Coord_Y, Length);
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
}
/* End Function:RMP_Checkbox *************************************************/

/* Function:RMP_Cmdbtn_Down ***************************************************
Description : Turn the command button to a pushed-down version.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the button.
              rmp_cnt_t Width - The width of the button.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Cmdbtn_Down(rmp_cnt_t Coord_X,
                     rmp_cnt_t Coord_Y,
                     rmp_cnt_t Length,
                     rmp_cnt_t Width)
{
    RMP_Line(Coord_X, Coord_Y, Coord_X+Length-1, Coord_Y, RMP_COLOR_BLACK);
    RMP_Line(Coord_X+Length-1, Coord_Y, Coord_X+Length-1, Coord_Y+Width-1, RMP_COLOR_WHITE);
    RMP_Line(Coord_X+Length-1, Coord_Y+Width-1, Coord_X, Coord_Y+Width-1, RMP_COLOR_WHITE);
    RMP_Line(Coord_X, Coord_Y+Width-1, Coord_X, Coord_Y, RMP_COLOR_BLACK);
    /* Clear the old shadow */
    RMP_Line(Coord_X+Length-2, Coord_Y+Width-2, Coord_X+1, Coord_Y+Width-2, RMP_COLOR_GREY);    
    RMP_Line(Coord_X+Length-2, Coord_Y+1, Coord_X+Length-2, Coord_Y+Width-2, RMP_COLOR_GREY);    
    /* The shadow */                            
    RMP_Line(Coord_X+1, Coord_Y+Width-2, Coord_X+1, Coord_Y+1, RMP_COLOR_DARK);    
    RMP_Line(Coord_X+1, Coord_Y+1, Coord_X+Length-2, Coord_Y+1, RMP_COLOR_DARK);    
}
/* End Function:RMP_Cmdbtn_Down **********************************************/

/* Function:RMP_Cmdbtn_Up *****************************************************
Description : Turn the command button to a popped-up version.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the button.
              rmp_cnt_t Width - The width of the button.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Cmdbtn_Up(rmp_cnt_t Coord_X,
                   rmp_cnt_t Coord_Y,
                   rmp_cnt_t Length,
                   rmp_cnt_t Width)
{
    RMP_Line(Coord_X, Coord_Y, Coord_X+Length-1, Coord_Y, RMP_COLOR_WHITE);
    RMP_Line(Coord_X+Length-1, Coord_Y, Coord_X+Length-1, Coord_Y+Width-1, RMP_COLOR_BLACK);
    RMP_Line(Coord_X+Length-1, Coord_Y+Width-1, Coord_X, Coord_Y+Width-1, RMP_COLOR_BLACK);
    RMP_Line(Coord_X, Coord_Y+Width-1, Coord_X, Coord_Y, RMP_COLOR_WHITE);
    /* Clear the old shadow */
    RMP_Line(Coord_X+1, Coord_Y+Width-2, Coord_X+1, Coord_Y+1, RMP_COLOR_GREY);    
    RMP_Line(Coord_X+1, Coord_Y+1, Coord_X+Length-2, Coord_Y+1, RMP_COLOR_GREY);    
    /* The shadow */
    RMP_Line(Coord_X+Length-2, Coord_Y+Width-2, Coord_X+1, Coord_Y+Width-2, RMP_COLOR_DARK);    
    RMP_Line(Coord_X+Length-2, Coord_Y+1, Coord_X+Length-2, Coord_Y+Width-2, RMP_COLOR_DARK);    
}
/* End Function:RMP_Cmdbtn_Up ************************************************/

/* Function:RMP_Cmdbtn ********************************************************
Description : Draw a command button.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the button.
              rmp_cnt_t Width - The width of the button.
              rmp_ptr_t Status - The status of the command button.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Cmdbtn(rmp_cnt_t Coord_X,
                rmp_cnt_t Coord_Y,
                rmp_cnt_t Length,
                rmp_cnt_t Width,
                rmp_ptr_t Status)
{
    RMP_Rectangle(Coord_X, Coord_Y, Length, Width, RMP_COLOR_GREY, RMP_COLOR_GREY);

    if(Status!=0U)
    {
        RMP_COV_MARKER();
        
        RMP_Cmdbtn_Down(Coord_X, Coord_Y, Length, Width);
    }
    else
    {
        RMP_COV_MARKER();
        
        RMP_Cmdbtn_Up(Coord_X, Coord_Y, Length, Width);
    }
}
/* End Function:RMP_Cmdbtn ***************************************************/

/* Function:RMP_Lineedit_Clr **************************************************
Description : Clear a portion of the line edit box.
Input       : rmp_cnt_t Clr_X - The X coordinate to start clearing.
              rmp_cnt_t Clr_Len - The length to clear.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Width - The width of the line edit box.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Lineedit_Clr(rmp_cnt_t Clr_X,
                      rmp_cnt_t Clr_Len,
                      rmp_cnt_t Coord_Y,
                      rmp_cnt_t Width)
{
    RMP_Rectangle(Clr_X, Coord_Y+1, Clr_Len, Width-2, RMP_COLOR_WHITE, RMP_COLOR_WHITE);
}
/* End Function:RMP_Lineedit_Clr *********************************************/

/* Function:RMP_Lineedit ******************************************************
Description : Draw a line edit box with string on it.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the line edit box.
              rmp_cnt_t Width - The width of the line edit box.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Lineedit(rmp_cnt_t Coord_X,
                  rmp_cnt_t Coord_Y,
                  rmp_cnt_t Length,
                  rmp_cnt_t Width)
{
    RMP_Rectangle(Coord_X, Coord_Y, Length, Width, RMP_COLOR_WHITE, RMP_COLOR_WHITE);
    
    /* Now draw the border */
    RMP_Line(Coord_X, Coord_Y, Coord_X+Length-1, Coord_Y, RMP_COLOR_DARK);
    RMP_Line(Coord_X+Length-1, Coord_Y, Coord_X+Length-1, Coord_Y+Width-1, RMP_COLOR_WHITE);
    RMP_Line(Coord_X+Length-1, Coord_Y+Width-1, Coord_X, Coord_Y+Width-1, RMP_COLOR_WHITE);
    RMP_Line(Coord_X, Coord_Y+Width-1, Coord_X, Coord_Y, RMP_COLOR_DARK);
    /* The shadow */
    RMP_Line(Coord_X+1, Coord_Y+Width-2, Coord_X+1, Coord_Y+1, RMP_COLOR_BLACK);    
    RMP_Line(Coord_X+1, Coord_Y+1, Coord_X+Length-2, Coord_Y+1, RMP_COLOR_BLACK);
}
/* End Function:RMP_Lineedit *************************************************/

/* Function:RMP_Radiobtn_Circle ***********************************************
Description : Draw the radio button outside circle according to the parameters.
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the radio button(also the width).         
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Radiobtn_Circle(rmp_cnt_t Coord_X,
                         rmp_cnt_t Coord_Y,
                         rmp_cnt_t Length)
{   
    rmp_cnt_t Radius;
    rmp_cnt_t Center_X;
    rmp_cnt_t Center_Y;
    rmp_cnt_t Cur_X;
    rmp_cnt_t Cur_Y;
    rmp_cnt_t Error;
    
    /* The radius is the length/2 */
    Radius=RMP_SAR(Length,1);
    Center_X=Coord_X+Radius;
    Center_Y=Coord_Y+Radius;
    
    /* Now we begin to draw the inner halfcircle */
    Cur_X=0;
    Cur_Y=Radius-1;
    Error=3-RMP_SAL(Cur_Y,1);
    
    while(Cur_X<=Cur_Y) 
    {  
        /* This is the upper-left part */
        RMP_POINT(Center_X+Cur_X, Center_Y-Cur_Y, RMP_COLOR_DDARK);
        RMP_POINT(Center_X-Cur_X, Center_Y-Cur_Y, RMP_COLOR_DDARK);
        RMP_POINT(Center_X-Cur_Y, Center_Y-Cur_X, RMP_COLOR_DDARK);
        RMP_POINT(Center_X-Cur_Y, Center_Y+Cur_X, RMP_COLOR_DDARK);
        
        /* This is the lower-right part */
        RMP_POINT(Center_X+Cur_X, Center_Y+Cur_Y, RMP_COLOR_LGREY);
        RMP_POINT(Center_X-Cur_X, Center_Y+Cur_Y, RMP_COLOR_LGREY);  
        RMP_POINT(Center_X+Cur_Y, Center_Y+Cur_X, RMP_COLOR_LGREY); 
        RMP_POINT(Center_X+Cur_Y, Center_Y-Cur_X, RMP_COLOR_LGREY);  
        
        if(Error<0) 
        {
            RMP_COV_MARKER();
            
            Error+=RMP_SAL(Cur_X,2)+6;
        }
        else 
        {
            RMP_COV_MARKER();
            
            Error+=RMP_SAL((rmp_ptr_t)Cur_X-(rmp_ptr_t)Cur_Y,2);
            Cur_Y--;
        }
        
        Cur_X++;
    } 
    
    /* Now we begin to draw the outer halfcircle */
    Cur_X=0;
    Cur_Y=Radius;
    Error=3-RMP_SAL(Radius,1);
    
    while(Cur_X<=Cur_Y) 
    {  
        /* This is the upper-left part */
        RMP_POINT(Center_X+Cur_X, Center_Y-Cur_Y, RMP_COLOR_DGREY);
        RMP_POINT(Center_X-Cur_X, Center_Y-Cur_Y, RMP_COLOR_DGREY);
        RMP_POINT(Center_X-Cur_Y, Center_Y-Cur_X, RMP_COLOR_DGREY);
        RMP_POINT(Center_X-Cur_Y, Center_Y+Cur_X, RMP_COLOR_DGREY);
        
        /* This is the lower-right part */
        RMP_POINT(Center_X+Cur_X, Center_Y+Cur_Y, RMP_COLOR_WHITE);
        RMP_POINT(Center_X-Cur_X, Center_Y+Cur_Y, RMP_COLOR_WHITE);  
        RMP_POINT(Center_X+Cur_Y, Center_Y+Cur_X, RMP_COLOR_WHITE); 
        RMP_POINT(Center_X+Cur_Y, Center_Y-Cur_X, RMP_COLOR_WHITE);  
        
        if(Error<0) 
        {
            RMP_COV_MARKER();
            
            Error+=RMP_SAL(Cur_X,2)+6;
        } 
        else 
        {
            RMP_COV_MARKER();
            
            Error+=RMP_SAL((rmp_ptr_t)Cur_X-(rmp_ptr_t)Cur_Y,2);
            Cur_Y--;  
        }
        
        Cur_X++;  
    } 
}
/* End Function:RMP_Radiobtn_Circle ******************************************/

/* Function:RMP_Radiobtn_Set **************************************************
Description : Set the radio button (select it).
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the radio button(also the width).
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Radiobtn_Set(rmp_cnt_t Coord_X,
                      rmp_cnt_t Coord_Y,
                      rmp_cnt_t Length)
{
    RMP_Circle(Coord_X+RMP_SAR(Length,1), Coord_Y+RMP_SAR(Length,1),
               Length/6, RMP_COLOR_BLACK, RMP_COLOR_BLACK);
}
/* End Function:RMP_Radiobtn_Set *********************************************/

/* Function:RMP_Radiobtn_Clr **************************************************
Description : Set the radio button (reset it).
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the radio button(also the width).
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Radiobtn_Clr(rmp_cnt_t Coord_X,
                      rmp_cnt_t Coord_Y,
                      rmp_cnt_t Length)
{
    RMP_Circle(Coord_X+RMP_SAR(Length,1), Coord_Y+RMP_SAR(Length,1),
               Length/6, RMP_COLOR_GREY, RMP_COLOR_GREY);
}
/* End Function:RMP_Radiobtn_Clr *********************************************/

/* Function:RMP_Radiobtn ******************************************************
Description : Draw the radio button according to the parameters.
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the radio button(also the width).
              rmp_ptr_t Status - The radio button status.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Radiobtn(rmp_cnt_t Coord_X,
                  rmp_cnt_t Coord_Y,
                  rmp_cnt_t Length,
                  rmp_ptr_t Status)
{   
    /* Fill the area with rect */
    RMP_Rectangle(Coord_X, Coord_Y, Length, Length, RMP_COLOR_GREY, RMP_COLOR_GREY);
    
    /* Draw the circle first */
    RMP_Radiobtn_Circle(Coord_X, Coord_Y, Length);
    
    /* See if the radio button is selected */
    if(Status==RMP_RBTN_SEL)
    {
        RMP_COV_MARKER();
        
        RMP_Radiobtn_Set(Coord_X, Coord_Y, Length);
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
}
/* End Function:RMP_Radiobtn *************************************************/

/* Function:RMP_Progbar_Set **************************************************
Description : Set the progress of a progress bar.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the button.
              rmp_cnt_t Width - The width of the button.
              rmp_cnt_t Style - The style of the progress.
              rmp_cnt_t Old_Prog - The old progress of this bar. 
              rmp_cnt_t New_Prog - The new progress of this bar. 
              rmp_ptr_t Fore - The foreground color.
              rmp_ptr_t Back - The background color.      
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Progbar_Set(rmp_cnt_t Coord_X,
                     rmp_cnt_t Coord_Y,
                     rmp_cnt_t Length,
                     rmp_cnt_t Width,
                     rmp_cnt_t Style,
                     rmp_cnt_t Old_Prog,
                     rmp_cnt_t New_Prog,
                     rmp_ptr_t Fore,
                     rmp_ptr_t Back)
{
    /* The critical values for drawing the bar */
    rmp_cnt_t Old_Pivot;
    rmp_cnt_t New_Pivot;
    
    /* If things does not change, return to save time */
    if(Old_Prog==New_Prog)
    {
        RMP_COV_MARKER();
        
        return;
    }
    else
    {
        RMP_COV_MARKER();
        /* No action required */
    }
    
    switch(Style)
    {
        case RMP_PBAR_L2R:
        {
            RMP_COV_MARKER();

            Old_Pivot=(Length-2)*Old_Prog/100;
            New_Pivot=(Length-2)*New_Prog/100;
            
            /* Progress decreased */
            if(Old_Pivot>New_Pivot)
            {
                RMP_COV_MARKER();
                
                RMP_Rectangle(Coord_X+1+New_Pivot, Coord_Y+1, Old_Pivot-New_Pivot, Width-2, Back, Back);
            }
            /* Progress increased */
            else
            {
                RMP_COV_MARKER();
                
                RMP_Rectangle(Coord_X+1+Old_Pivot, Coord_Y+1, New_Pivot-Old_Pivot, Width-2, Fore, Fore);
            }
            
            break;
        }
        case RMP_PBAR_D2U:
        {
            RMP_COV_MARKER();

            Old_Pivot=(Width-2)*Old_Prog/100;
            New_Pivot=(Width-2)*New_Prog/100;
            
            /* Progress decreased */
            if(Old_Pivot>New_Pivot)
            {
                RMP_COV_MARKER();
                
                RMP_Rectangle(Coord_X+1, Coord_Y-1+Width-Old_Pivot, Length-2, Old_Pivot-New_Pivot, Back, Back);
            }
            /* Progress increased */
            else
            {
                RMP_COV_MARKER();
                
                RMP_Rectangle(Coord_X+1, Coord_Y-1+Width-New_Pivot, Length-2, New_Pivot-Old_Pivot, Fore, Fore);
            }
            
            break;
        }
        case RMP_PBAR_R2L:
        {
            RMP_COV_MARKER();

            Old_Pivot=(Length-2)*Old_Prog/100;
            New_Pivot=(Length-2)*New_Prog/100;
            
            /* Progress decreased */
            if(Old_Pivot>New_Pivot)
            {
                RMP_COV_MARKER();
                
                RMP_Rectangle(Coord_X-1+Length-Old_Pivot, Coord_Y+1, Old_Pivot-New_Pivot, Width-2, Back, Back);
            }
            /* Progress increased */
            else
            {
                RMP_COV_MARKER();
                
                RMP_Rectangle(Coord_X-1+Length-New_Pivot, Coord_Y+1, New_Pivot-Old_Pivot, Width-2, Fore, Fore);
            }
            
            break;
        }
        case RMP_PBAR_U2D:
        {
            RMP_COV_MARKER();

            Old_Pivot=(Width-2)*Old_Prog/100;
            New_Pivot=(Width-2)*New_Prog/100;
            
            /* Progress decreased */
            if(Old_Pivot>New_Pivot)
            {
                RMP_COV_MARKER();
                
                RMP_Rectangle(Coord_X+1, Coord_Y+1+New_Pivot, Length-2, Old_Pivot-New_Pivot, Back, Back);
            }
            /* Progress increased */
            else
            {
                RMP_COV_MARKER();
                
                RMP_Rectangle(Coord_X+1, Coord_Y+1+Old_Pivot, Length-2, New_Pivot-Old_Pivot, Fore, Fore);
            }
            
            break;
        }
        default:
        {
            RMP_COV_MARKER();
            
            break;
        }
    }
}
/* End Function:RMP_Progbar_Set **********************************************/

/* Function:RMP_Progbar_Prog **************************************************
Description : Draw a progress bar's progress part.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the button.
              rmp_cnt_t Width - The width of the button.
              rmp_cnt_t Style - The style of the progress.
              rmp_cnt_t Prog - The progress of the progress bar.    
              rmp_ptr_t Fore - The foreground color.
              rmp_ptr_t Back - The background color.   
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Progbar_Prog(rmp_cnt_t Coord_X,
                      rmp_cnt_t Coord_Y,
                      rmp_cnt_t Length,
                      rmp_cnt_t Width,
                      rmp_cnt_t Style,
                      rmp_cnt_t Prog,
                      rmp_ptr_t Fore,
                      rmp_ptr_t Back)
{
    /* The critical value for drawing the bar */
    rmp_cnt_t Pivot;
    
    switch(Style)
    {
        case RMP_PBAR_L2R:
        {
            RMP_COV_MARKER();

            Pivot=Length*Prog/100;
            
            if(Prog!=0)
            {
                RMP_COV_MARKER();
                
                RMP_Rectangle(Coord_X, Coord_Y, Pivot, Width, Fore, Fore);
            }
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }

            if(Prog!=100)
            {
                RMP_COV_MARKER();
                
                RMP_Rectangle(Coord_X+Pivot, Coord_Y, Length-Pivot, Width, Back, Back);
            }
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }
            
            break;
        }
        case RMP_PBAR_D2U:
        {
            RMP_COV_MARKER();

            Pivot=Width*Prog/100;
            
            if(Prog!=0)
            {
                RMP_COV_MARKER();
                
                RMP_Rectangle(Coord_X, Coord_Y+Width-Pivot, Length, Pivot, Fore, Fore);
            }
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }

            if(Prog!=100)
            {
                RMP_COV_MARKER();
                
                RMP_Rectangle(Coord_X, Coord_Y, Length, Width-Pivot, Back, Back);
            }
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }
            
            break;
        }
        case RMP_PBAR_R2L:
        {
            RMP_COV_MARKER();

            Pivot=Length*Prog/100;
            
            if(Prog!=0)
            {
                RMP_COV_MARKER();
                
                RMP_Rectangle(Coord_X+Length-Pivot, Coord_Y, Pivot, Width, Fore, Fore);
            }
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }

            if(Prog!=100)
            {
                RMP_COV_MARKER();
                
                RMP_Rectangle(Coord_X, Coord_Y, Length-Pivot, Width, Back, Back);
            }
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }
            
            break;
        }
        case RMP_PBAR_U2D:
        {
            RMP_COV_MARKER();

            Pivot=Width*Prog/100;
            
            if(Prog!=0)
            {
                RMP_COV_MARKER();
                
                RMP_Rectangle(Coord_X, Coord_Y, Length, Pivot, Fore, Fore);
            }
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }

            if(Prog!=100)
            {
                RMP_COV_MARKER();
                
                RMP_Rectangle(Coord_X, Coord_Y+Pivot, Length, Width-Pivot, Back, Back);
            }
            else
            {
                RMP_COV_MARKER();
                /* No action required */
            }
            break;
        }
        default:
        {
            RMP_COV_MARKER();
            
            break;
        }
    }
}
/* End Function:RMP_Progbar_Prog *********************************************/

/* Function:RMP_Progbar *******************************************************
Description : Draw a progress bar with string on it.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the button.
              rmp_cnt_t Width - The width of the button.
              rmp_cnt_t Style - The style of the progress.
              rmp_cnt_t Prog - The progress of the progress bar. 
              rmp_ptr_t Fore - The foreground color.
              rmp_ptr_t Back - The background color.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Progbar(rmp_cnt_t Coord_X,
                 rmp_cnt_t Coord_Y,
                 rmp_cnt_t Length,
                 rmp_cnt_t Width,
                 rmp_cnt_t Style,
                 rmp_cnt_t Prog,
                 rmp_ptr_t Fore,
                 rmp_ptr_t Back)
{
    /* Draw the progress bar according to the style of the progress bar */
    RMP_Progbar_Prog(Coord_X, Coord_Y, Length, Width, Style, Prog, Fore, Back);
    /* Now draw the border */
    RMP_Line(Coord_X, Coord_Y, Coord_X+Length-1, Coord_Y, RMP_COLOR_BLACK);
    RMP_Line(Coord_X+Length-1, Coord_Y, Coord_X+Length-1, Coord_Y+Width-1, RMP_COLOR_BLACK);
    RMP_Line(Coord_X+Length-1, Coord_Y+Width-1, Coord_X, Coord_Y+Width-1, RMP_COLOR_BLACK);
    RMP_Line(Coord_X, Coord_Y+Width-1, Coord_X, Coord_Y, RMP_COLOR_BLACK);
}
#endif
#endif
/* End Function:RMP_Progbar **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
