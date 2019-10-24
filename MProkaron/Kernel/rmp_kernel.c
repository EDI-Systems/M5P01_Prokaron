/******************************************************************************
Filename    : rmp_kernel.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The RMP RTOS single-file kernel.
              This is a single-process kernel that does nothing but simple thread
              context switches. This operating system's kernel object allocation
              policy is totally exported, so the user assume full control over this.
              The white-box coverage of 100% of all kernel branches have been reached.
              Formal verification in progress.
******************************************************************************/

/* Includes ******************************************************************/
#define __HDR_DEFS__
#include "Platform/rmp_platform.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEFS__

#define __HDR_STRUCTS__
#include "Platform/rmp_platform.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCTS__

/* Private include */
#include "Kernel/rmp_kernel.h"

#define __HDR_PUBLIC_MEMBERS__
#include "Platform/rmp_platform.h"
#undef __HDR_PUBLIC_MEMBERS__
/* End Includes **************************************************************/

/* Begin Function:RMP_Print_Coverage ******************************************
Description : The coverage data printer. Should always be disabled for all cases
              except where a kernel coverage test is needed. This should never
              be called ny any user application; for EDI coverage testing only.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
#ifdef RMP_COVERAGE
void RMP_Print_Coverage(void)
{
    rmp_cnt_t Count;
    
    for(Count=1;Count<RMP_COVERAGE_LINES;Count++)
    {
        if(RMP_Coverage[Count]!=0)
        {
            RMP_PRINTK_I(Count);
            RMP_PRINTK_S(":");
            RMP_PRINTK_I(RMP_Coverage[Count]);
            RMP_PRINTK_S("\r\n");
        }
    }
}
#endif
/* End Function:RMP_Print_Coverage *******************************************/

/* Begin Function:RMP_Clear ***************************************************
Description : Memset a memory area to zero.
Input       : volatile void* Addr - The address to clear.
              rmp_ptr_t Size - The size to clear.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Clear(volatile void* Addr, rmp_ptr_t Size)
{
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
        Ptr[Count]=0;
}
/* End Function:RMP_Clear ****************************************************/

/* Begin Function:RMP_Print_Int ***********************************************
Description : Print a signed integer on the debugging console. This integer is
              printed as decimal with sign.
Input       : rmp_cnt_t Int - The integer to print.
Output      : None.
Return      : rmp_cnt_t - The length of the string printed.
******************************************************************************/
rmp_cnt_t RMP_Print_Int(rmp_cnt_t Int)
{
    rmp_ptr_t Iter;
    rmp_cnt_t Count;
    rmp_cnt_t Num;
    rmp_ptr_t Div;
    
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
        Iter=(rmp_ptr_t)(-Int);
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
Input       : rmp_ptr_t Uint - The unsigned integer to print.
Output      : None.
Return      : rmp_cnt_t - The length of the string printed.
******************************************************************************/
rmp_cnt_t RMP_Print_Uint(rmp_ptr_t Uint)
{
    rmp_ptr_t Iter;
    rmp_cnt_t Count;
    rmp_cnt_t Num;
    
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
        while((Iter>>((sizeof(rmp_ptr_t)*8)-4))==0)
        {
            Iter<<=4;
            Count++;
        }
        /* Count is the number of pts to print */
        Count=sizeof(rmp_ptr_t)*2-Count;
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
Input       : rmp_s8_t* String - The string to print
Output      : None.
Return      : rmp_cnt_t - The length of the string printed, the '\0' is not included.
******************************************************************************/
rmp_cnt_t RMP_Print_String(rmp_s8_t* String)
{
    rmp_cnt_t Count;
    
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

/* Begin Function:RMP_CRC16 ***************************************************
Description    : CRC16 checksum calculation. Polynomial=0xA001.
Input          : const rmp_u8_t* Data - The pointer to the dataset.
                 rmp_ptr_t Length - The length of the data in bytes.
Output         : None.
Return         : rmp_ptr_t - The CRC16 value calculated.
******************************************************************************/
#ifdef __RMP_U8_T__
#ifdef __RMP_U16_T__
rmp_ptr_t RMP_CRC16(const rmp_u8_t* Data, rmp_ptr_t Length)
{
    rmp_ptr_t Data_Cnt;
    rmp_u8_t Index;
    rmp_u8_t Temp_High;
    rmp_u8_t Temp_Low;
    
    /* CRC16 constants with X^16+X^15+X^2+1 */
    static const rmp_u8_t CRC16_High[256]=
    {
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40
    };

    const rmp_u8_t CRC16_Low[256]=
    {
        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
        0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
        0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
        0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
        0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
        0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
        0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
        0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
        0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
        0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
        0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
        0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
        0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
        0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
        0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
        0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
        0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
        0x41, 0x81, 0x80, 0x40
    };

    Temp_High=0xFF;
    Temp_Low=0xFF;
    for(Data_Cnt=0;Data_Cnt<Length;Data_Cnt++)
    {
        Index=Temp_Low^Data[Data_Cnt];
        Temp_Low=(rmp_u8_t)(Temp_High^CRC16_High[Index]);
        Temp_High=CRC16_Low[Index];
    }

    return (rmp_u16_t)(((rmp_u16_t)Temp_High)<<8|Temp_Low);
}
#endif
#endif
/* End Function:RMP_CRC16 ****************************************************/

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
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
            _RMP_Yield();
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
    }
    /* Trying to unlock a scheduler that is not locked - should never happen */
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
        RMP_COVERAGE_MARKER();
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        _RMP_Yield();
    }
    else
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Pend=1;
    }
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
        {
            RMP_COVERAGE_MARKER();
            break;
        }
        else
            RMP_COVERAGE_MARKER();
        
        /* This thread should be processed */
        RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
        switch(RMP_THD_STATE(Thread->State))
        {
            case RMP_THD_SNDDLY:RMP_COVERAGE_MARKER();
            case RMP_THD_SEMDLY:
            {
                RMP_COVERAGE_MARKER();
                RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
                Thread->Retval=RMP_ERR_OPER;
                break;
            }
            
            case RMP_THD_RCVDLY:
            {
                RMP_COVERAGE_MARKER();
                Thread->Retval=RMP_ERR_OPER;
                break;
            }
            
            case RMP_THD_DELAYED:RMP_COVERAGE_MARKER();break;
            /* Should not get here */
            default:while(1);
        }

        RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
        /* Set to ready if not suspended */
        _RMP_Set_Rdy(Thread);
    }
}
/* Begin Function:_RMP_Timer_Proc ********************************************/

/* Begin Function:_RMP_Get_High_Rdy *******************************************
Description : Get the highest priority ready thread. The return value will be
              written into the global variables.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Get_High_Rdy(void)
{
    rmp_cnt_t Count;
    
    /* Write the SP value to thread structure */
    RMP_Cur_Thd->Stack=RMP_Cur_SP;
    
    /* No need to detect scheduler locks - if this interrupt can be entered, the scheduler is not locked */
    RMP_Sched_Pend=0;
    /* See which one is ready, and pick it */
    for(Count=RMP_BITMAP_SIZE-1;Count>=0;Count--)
    {
        if(RMP_Bitmap[Count]==0)
        {
            RMP_COVERAGE_MARKER();
            continue;
        }
        else
            RMP_COVERAGE_MARKER();
        
        Count=RMP_MSB_Get(RMP_Bitmap[Count])+(Count<<RMP_WORD_ORDER);
        
        /* See if the current thread and the next thread are the same. If yes, place the current at the end of the queue */
        if(RMP_Cur_Thd==(struct RMP_Thd*)(RMP_Run[Count].Next))
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(RMP_Cur_Thd->Run_Head.Prev, RMP_Cur_Thd->Run_Head.Next);
            RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),
                         RMP_Run[RMP_Cur_Thd->Prio].Prev,
                         &(RMP_Run[RMP_Cur_Thd->Prio]));
        }
        else
            RMP_COVERAGE_MARKER();
        
        /* Replenish timeslices */
        RMP_Cur_Thd->Slices_Left=RMP_Cur_Thd->Slices;
        RMP_Cur_Thd=(struct RMP_Thd*)(RMP_Run[Count].Next);
        break;
    }
    
    /* Load the SP value from thread structure */
    RMP_Cur_SP=RMP_Cur_Thd->Stack;
    
#if(RMP_USE_HOOKS==RMP_TRUE)
    RMP_Sched_Hook();
#endif
}
/* End Function:_RMP_Get_High_Rdy ********************************************/

/* Begin Function:_RMP_Tick_Handler *******************************************
Description : The system tick timer interrupt routine.
Input       : rmp_ptr_t Ticks - How many ticks have passed.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Tick_Handler(rmp_ptr_t Ticks)
{
    struct RMP_Thd* Thread;
    /* Increase the timestamp as always */
    RMP_Tick+=Ticks;
    
    /* See if the current thread expired. If yes, trigger a scheduler event */
    if(Ticks>RMP_Cur_Thd->Slices_Left)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Pend=1;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        RMP_Cur_Thd->Slices_Left-=Ticks;
    }
    
    /* Check if there are any timer events */
    if((&RMP_Delay)!=RMP_Delay.Next)
    {
        RMP_COVERAGE_MARKER();
        Thread=RMP_DLY2THD(RMP_Delay.Next);
        /* If the value is less than this, then it means that the time have
         * already passed and we have to process this */
        if((RMP_Tick-Thread->Timeout)<=(RMP_ALLBITS>>1))
        {
            RMP_COVERAGE_MARKER();
            /* No need to care about scheduler locks if this interrupt can be entered
             * - we have disabled timer and scheduler interrupts in scheduler lock */
            _RMP_Timer_Proc();
        }
        else
            RMP_COVERAGE_MARKER();
    }
    else
        RMP_COVERAGE_MARKER();
    
    if(RMP_Sched_Pend!=0)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Pend=0;
        _RMP_Yield();
    }
    else
        RMP_COVERAGE_MARKER();
    
#if(RMP_USE_HOOKS==RMP_TRUE)
    RMP_Tick_Hook(Ticks);
#endif
}
/* End Function:_RMP_Tick_Handler ********************************************/

/* Begin Function:_RMP_Get_Near_Ticks *****************************************
Description : Get the nearest timer interrupt arrival time. This is used to set
              the timer after the body of tick handler have been executed. This
              can be called in the ticker hook and scheduler hook to set the next
              timeout value to implement a tickless kernel. If a tickless kernel
              is not desired, this function can be ignored.
Input       : None.
Output      : None.
Return      : rmp_ptr_t Ticks - How many ticks until the next timeout.
******************************************************************************/
rmp_ptr_t _RMP_Get_Near_Ticks(void)
{
    rmp_ptr_t Value;
    struct RMP_Thd* Thread;
    
    /* What is the current thread's timeout value? */
    Value=RMP_Cur_Thd->Slices_Left;
    
    /* What is the nearest timer timeout value? */
    if((&RMP_Delay)!=RMP_Delay.Next)
    {
        RMP_COVERAGE_MARKER();
        Thread=RMP_DLY2THD(RMP_Delay.Next);
        /* See if it is nearer - don't worry about the situation that the timer
         * have overflown, because if that is to happen, it would have been 
         * already processed by the timeout processing routine just before. */
        if((Thread->Timeout-RMP_Tick)<Value)
        {
            RMP_COVERAGE_MARKER();
            Value=Thread->Timeout-RMP_Tick;
        }
        else
            RMP_COVERAGE_MARKER();
    }
    else
        RMP_COVERAGE_MARKER();
    
    return Value;
}
/* End Function:_RMP_Get_Near_Ticks ******************************************/

/* Begin Function:_RMP_Set_Rdy ************************************************
Description : Set the thread as ready to schedule. That means, put the thread into
              the runqueue. When this is called, please make sure that the scheduler
              is locked.
              This function will also try to identify if this thread is currently 
              suspended. If yes, it will not be placed into the queue.
Input       : volatile struct RMP_Thd* Thread - The thread to put into the runqueue.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Set_Rdy(volatile struct RMP_Thd* Thread)
{        
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
        
        /* Compare this with the current one to see if we need a context switch */
        if(Thread->Prio>RMP_Cur_Thd->Prio)
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1;
        }
        else
            RMP_COVERAGE_MARKER();
    }
    else
        RMP_COVERAGE_MARKER();
}
/* End Function:_RMP_Set_Rdy *************************************************/

/* Begin Function:_RMP_Clr_Rdy ************************************************
Description : Clear the thread from the runqueue. When this is called, please 
              make sure that the scheduler is locked. This function also checks whether
              the thread is suspended. If yes, it will not remove it from the queue.
Input       : volatile struct RMP_Thd* Thread - The thread to clear from the runqueue.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Clr_Rdy(volatile struct RMP_Thd* Thread)
{
    /* Is it suspended? If yes, no need to delete again */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* See if it is the last thread on the priority level */
        if(Thread->Run_Head.Prev==Thread->Run_Head.Next)
        {
            RMP_COVERAGE_MARKER();
            RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]&=~(((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK));
        }
        else
            RMP_COVERAGE_MARKER();
        
        /* Delete this from the corresponding runqueue */
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
        
        /* If it is the current thread, request a context switch */
        if(Thread==RMP_Cur_Thd)
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1;
        }
        else
            RMP_COVERAGE_MARKER();
    }
    else
        RMP_COVERAGE_MARKER();
}
/* End Function:_RMP_Clr_Rdy *************************************************/

/* Begin Function:_RMP_Dly_Ins ************************************************
Description : Insert the thread into the delay queue, given some timeslices into the
              future. The thread must not be in the run queue any more.
Input       : volatile struct RMP_Thd* Thread - The thread to put into the delay queue.
              rmp_ptr_t - The timeslices to delay.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Dly_Ins(volatile struct RMP_Thd* Thread, rmp_ptr_t Slices)
{
    struct RMP_List* Trav_Ptr;
    struct RMP_Thd* Trav_Thd;
    
    /* Find a place to insert this timer */
    Trav_Ptr=(struct RMP_List*)RMP_Delay.Next;
    while(Trav_Ptr!=&(RMP_Delay))
    {
        Trav_Thd=RMP_DLY2THD(Trav_Ptr);
        if((Trav_Thd->Timeout-RMP_Tick)>Slices)
        {
            RMP_COVERAGE_MARKER();
            break;
        }
        else
            RMP_COVERAGE_MARKER();
        
        Trav_Ptr=(struct RMP_List*)(Trav_Ptr->Next);
    }

    /* Insert this into the list */
    Thread->Timeout=RMP_Tick+Slices;
    RMP_List_Ins(&(Thread->Dly_Head),Trav_Ptr->Prev,Trav_Ptr);
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
              rmp_ptr_t Prio - The priority of the thread.
              rmp_ptr_t Slices - The number of timeslices to assign to the thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0. on error, return an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Crt(volatile struct RMP_Thd* Thread, void* Entry, void* Stack, void* Arg, rmp_ptr_t Prio, rmp_ptr_t Slices)
{
    /* Check if the priority and timeslice range is correct */
    if(Prio>=RMP_MAX_PREEMPT_PRIO)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_PRIO;
    }
    else
        RMP_COVERAGE_MARKER();
    
    if((Slices==0)||(Slices>=RMP_MAX_SLICES))
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SLICE;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Lock_Sched();
    
    if(RMP_THD_STATE(Thread->State)!=RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Create the thread and insert it into the list */
    Thread->Prio=Prio;
    Thread->Slices=Slices;
    Thread->Slices_Left=Slices;
    Thread->Stack=(rmp_ptr_t)Stack;
    
    /* Initialize its stack and sending list */
    _RMP_Stack_Init((rmp_ptr_t)Entry, (rmp_ptr_t)Stack, (rmp_ptr_t)Arg);
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
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Del(volatile struct RMP_Thd* Thread)
{
    struct RMP_Thd* Release;
    
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Lock_Sched();
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* See if anyone waiting to send to this thread. If there is, release all these threads */
    while(&(Thread->Snd_List)!=Thread->Snd_List.Next)
    {
        Release=(struct RMP_Thd*)(Thread->Snd_List.Next);
        RMP_List_Del(Release->Run_Head.Prev,Release->Run_Head.Next);
        if(RMP_THD_STATE(Release->State)==RMP_THD_SNDDLY)
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(Release->Dly_Head.Prev,Release->Dly_Head.Next);
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_THD_STATE_SET(Release->State,RMP_THD_RUNNING);
        /* Set ready if not suspended */
        _RMP_Set_Rdy(Release);
        Release->Retval=RMP_ERR_OPER;
    }
    
    /* See what is its state */
    switch(RMP_THD_STATE(Thread->State))
    {
        case RMP_THD_RUNNING:
        {
            RMP_COVERAGE_MARKER();
            /* Clear ready if not suspended */
            _RMP_Clr_Rdy(Thread);
            break;
        }
        
        /* Do nothing if it is blocked on receive */
        case RMP_THD_RCVBLK:
        {
            RMP_COVERAGE_MARKER();
            break;
        }
        
        case RMP_THD_SNDBLK:RMP_COVERAGE_MARKER();
        case RMP_THD_SEMBLK:
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
            break;
        }
        
        case RMP_THD_SNDDLY:RMP_COVERAGE_MARKER();
        case RMP_THD_SEMDLY:
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
            /* Fall-through case */
        }
        case RMP_THD_RCVDLY:RMP_COVERAGE_MARKER();
        case RMP_THD_DELAYED:
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
            break;
        }
        /* Should not get here */
        default:while(1);
    }
    /* Set return value to failure anyway */
    Thread->Retval=RMP_ERR_OPER;
    Thread->State=RMP_THD_FREE;
    /* If we are deleting ourself, pend a yield */
    if(Thread==RMP_Cur_Thd)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Pend=1;
    }
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Unlock_Sched();
    
    /* If we are deleting ourself, just stop the execution here */
    if(Thread==RMP_Cur_Thd)
        while(1);
    else
        RMP_COVERAGE_MARKER();

    return 0;
}
/* End Function:RMP_Thd_Del **************************************************/

/* Begin Function:RMP_Thd_Set *************************************************
Description : Change the priority or timeslice of a real-time thread. If one of
              the changes is not desired, just leave it to RMP_MAX_PREEMPT_PRIO
              or RMP_MAX_SLICES.
Input       : volatile struct RMP_Thd* Thread - The thread structure of the thread.
              rmp_ptr_t Prio - The priority of the thread.
              rmp_ptr_t Slices - The new timeslice value for this thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0; else error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Set(volatile struct RMP_Thd* Thread, rmp_ptr_t Prio, rmp_ptr_t Slices)
{
    /* Check if the priority and timeslice range is correct */
    if(Slices==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SLICE;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Lock_Sched();
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* See if the thread is in running state */
    if(RMP_THD_STATE(Thread->State)==RMP_THD_RUNNING)
    {
        RMP_COVERAGE_MARKER();
        
        /* See if we are gonna change one of it or both */
        if(Prio<RMP_MAX_PREEMPT_PRIO)
        {
            RMP_COVERAGE_MARKER();
            if(Thread->Prio!=Prio)
            {
                RMP_COVERAGE_MARKER();
                /* It doesn't matter whether this is suspended or not. 
                 * If suspended, the operations will not be conducted. */
                _RMP_Clr_Rdy(Thread);
                Thread->Prio=Prio;
                _RMP_Set_Rdy(Thread);
            }
            else
                RMP_COVERAGE_MARKER();
        }
        else
            RMP_COVERAGE_MARKER();
        
        if(Slices<RMP_MAX_SLICES)
        {
            RMP_COVERAGE_MARKER();
            Thread->Slices=Slices;
        }
        else
            RMP_COVERAGE_MARKER();
    }
    else
    {
        RMP_COVERAGE_MARKER();
        
        if(Prio<RMP_MAX_PREEMPT_PRIO)
        {
            RMP_COVERAGE_MARKER();
            Thread->Prio=Prio;
        }
        else
            RMP_COVERAGE_MARKER();
        
        if(Slices<RMP_MAX_SLICES)
        {
            RMP_COVERAGE_MARKER();
            Thread->Slices=Slices;
        }
        else
            RMP_COVERAGE_MARKER();
    }
    
    RMP_Unlock_Sched();
    return 0;
}
/* End Function:RMP_Thd_Set **************************************************/

/* Begin Function:RMP_Thd_Suspend *********************************************
Description : Suspend the execution of a real-time thread.
Input       : volatile struct RMP_Thd* Thread - The thread structure of the thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0; else error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Suspend(volatile struct RMP_Thd* Thread)
{
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Lock_Sched();
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Suspend it */
    if((Thread->State&RMP_THD_SUSPENDED)!=0)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_STATE;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Only when it is running do we clear this. If we are clearing this, it is not
     * suspended, so the running queue removal is guaranteed to succceed */
    if(RMP_THD_STATE(Thread->State)==RMP_THD_RUNNING)
    {
        RMP_COVERAGE_MARKER();
        _RMP_Clr_Rdy(Thread);
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Mark this as suspended */
    Thread->State|=RMP_THD_SUSPENDED;
    
    /* If we are suspending ourself, pend a yield */
    if(Thread==RMP_Cur_Thd)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Pend=1;
    }
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Unlock_Sched();
    return 0;
}
/* End Function:RMP_Thd_Suspend **********************************************/

/* Begin Function:RMP_Thd_Resume **********************************************
Description : Resume the execution of a real-time thread.
Input       : volatile struct RMP_Thd* Thread - The thread structure of the thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0; else error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Resume(volatile struct RMP_Thd* Thread)
{
    rmp_ret_t Retval;
    
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
        RMP_COVERAGE_MARKER();

    RMP_Lock_Sched();
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Check if the thread is suspended, if not, then throw an error */
    if((Thread->State&RMP_THD_SUSPENDED)!=0)
    {
        RMP_COVERAGE_MARKER();
        
        /* Suspended */
        Thread->State&=~RMP_THD_SUSPENDED;
        /* Only when it is running will we put it back. It can't be suspended here, 
         * so the set ready operation will surely put it back */
        if(RMP_THD_STATE(Thread->State)==RMP_THD_RUNNING)
        {
            RMP_COVERAGE_MARKER();
            _RMP_Set_Rdy(Thread);
        }
        else
            RMP_COVERAGE_MARKER();
        
        Retval=0;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        Retval=RMP_ERR_STATE;
    }
    
    RMP_Unlock_Sched();

    return Retval;
}
/* End Function:RMP_Thd_Resume ***********************************************/

/* Begin Function:RMP_Thd_Snd *************************************************
Description : Send to a real-time thread's mailbox. If the mailbox is full, then
              this operation can potentially block.
Input       : volatile struct RMP_Thd* Thread - The thread structure of the thread to send to.
              rmp_ptr_t Data - The data to send to that thread.
              rmp_ptr_t Slices - The timeslice to wait, if the mailbox is already full.
Output      : None.
Return      : rmp_ret_t - If successful,0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Snd(volatile struct RMP_Thd* Thread, rmp_ptr_t Data, rmp_ptr_t Slices)
{
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Lock_Sched();
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Are we sending to ourself? This is not allowed */
    if(RMP_Cur_Thd==Thread)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_OPER;
    }
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Cur_Thd->Retval=0;

    /* See if there is already a value in the mailbox, if yes, we block */
    if((Thread->State&RMP_THD_MBOXFUL)!=0)
    {
        RMP_COVERAGE_MARKER();
        
        /* Mailbox full, we block, and put ourself into the queue */
        if(Slices==0)
        {
            RMP_COVERAGE_MARKER();
            RMP_Unlock_Sched();
            return RMP_ERR_OPER;
        }
        else
            RMP_COVERAGE_MARKER();

        /* We must be running */
        _RMP_Clr_Rdy(RMP_Cur_Thd);
        RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),Thread->Snd_List.Prev,&(Thread->Snd_List));

        if(Slices<RMP_MAX_SLICES)
        {
            RMP_COVERAGE_MARKER();
            _RMP_Dly_Ins(RMP_Cur_Thd, Slices);
            RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_SNDDLY);
        }
        else
        {
            RMP_COVERAGE_MARKER();
            RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_SNDBLK);
        }

        RMP_Cur_Thd->Data=Data;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        
        /* Mailbox not full. We need to check if the receiver is waiting for us */
        if((RMP_THD_STATE(Thread->State)==RMP_THD_RCVBLK)||
           (RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY))
        {
            RMP_COVERAGE_MARKER();
            
            /* The receiver is blocked, wake it up and return the value */
            if(RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY)
            {
                RMP_COVERAGE_MARKER();
                RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
            }
            else
                RMP_COVERAGE_MARKER();
            
            RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
            /* Set to running if not suspended */
            _RMP_Set_Rdy(Thread);
        }
        else
            RMP_COVERAGE_MARKER();
        
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
              We do not check whether the scheduler is locked; if we are calling this
              function, we're pretty sure that it's not.
Input       : volatile struct RMP_Thd* Thread - The thread structure of the thread to send to.
              rmp_ptr_t Data - The data to send to that thread.
Output      : None.
Return      : rmp_ret_t - If successful,0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Snd_ISR(volatile struct RMP_Thd* Thread, rmp_ptr_t Data)
{
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
        RMP_COVERAGE_MARKER();
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
        RMP_COVERAGE_MARKER();

    /* See if there is already a value in the mailbox, if yes, we abort */
    if((Thread->State&RMP_THD_MBOXFUL)!=0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        
        /* Mailbox not full. We need to check if the receiver is waiting for us */
        if((RMP_THD_STATE(Thread->State)==RMP_THD_RCVBLK)||
           (RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY))
        {
            RMP_COVERAGE_MARKER();

            /* The receiver is blocked, wake it up and return the value */
            if(RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY)
            {
                RMP_COVERAGE_MARKER();
                RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
            }
            else
                RMP_COVERAGE_MARKER();
            
            RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);

            /* Set to running if not suspended */
            _RMP_Set_Rdy(Thread);

            /* If schedule pending, trigger it now because we are in ISR */
            if(RMP_Sched_Pend!=0)
            {
                RMP_COVERAGE_MARKER();
                RMP_Sched_Pend=0;
                _RMP_Yield();
            }
            else
                RMP_COVERAGE_MARKER();
        }
        else
            RMP_COVERAGE_MARKER();
        
        /* Set the mailbox */
        Thread->Mailbox=Data;
        Thread->State|=RMP_THD_MBOXFUL;
    }

    return 0;
}
/* End Function:RMP_Thd_Snd_ISR **********************************************/

/* Begin Function:RMP_Thd_Rcv *************************************************
Description : Receive a message from our own mailbox, and this is blocking.
Input       : rmp_ptr_t Slices - The timeslices to wait, if the mailbox is empty.
Output      : rmp_ptr_t* Data - The pointer to put the data to.
Return      : rmp_ret_t - If successful,0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Rcv(rmp_ptr_t* Data, rmp_ptr_t Slices)
{
    struct RMP_Thd* Sender;
    
    if(Data==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Lock_Sched();
    
    /* Is there any other guy waiting on us? If there is, unblock it and set it running */
    Sender=0;
    if(&(RMP_Cur_Thd->Snd_List)!=RMP_Cur_Thd->Snd_List.Next)
    {
        RMP_COVERAGE_MARKER();
        /* Read the data */
        Sender=(struct RMP_Thd*)(RMP_Cur_Thd->Snd_List.Next);
        RMP_List_Del(Sender->Run_Head.Prev,Sender->Run_Head.Next);
        *Data=Sender->Data;
        /* Now we unblock it - what state is it in? */
        if((RMP_THD_STATE(Sender->State)==RMP_THD_SNDDLY))
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(Sender->Dly_Head.Prev,Sender->Dly_Head.Next);
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_THD_STATE_SET(Sender->State,RMP_THD_RUNNING);
        /* Set to running if not suspended */
        _RMP_Set_Rdy(Sender);
    }

    /* Check if there is a value in our mailbox. If yes, we return with that value */
    if((RMP_Cur_Thd->State&RMP_THD_MBOXFUL)!=0)
    {
        RMP_COVERAGE_MARKER();
        /* Get the value from mailbox */
        *Data=RMP_Cur_Thd->Mailbox;
        /* See if we unblocked a sender. If yes, we place the new value into 
         * our mailbox and it is still full */
        if(Sender!=0)
            RMP_Cur_Thd->Mailbox=Sender->Data;
        else
            RMP_Cur_Thd->State&=~RMP_THD_MBOXFUL;
        
        RMP_Unlock_Sched();
        return 0;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        
        /* Box empty. Do we have somebody waiting? */
        if(Sender!=0)
        {
            RMP_COVERAGE_MARKER();
            RMP_Cur_Thd->Mailbox=Sender->Data;
            RMP_Unlock_Sched();
            return 0;
        }
        /* No sender waiting on us and box empty, we need to block */
        else
        {
            RMP_COVERAGE_MARKER();
            
            if(Slices==0)
            {
                RMP_COVERAGE_MARKER();
                RMP_Unlock_Sched();
                return RMP_ERR_OPER;
            }
            else
                RMP_COVERAGE_MARKER();

            /* We must be running and not suspended so we will surely be deleted from queue */
            _RMP_Clr_Rdy(RMP_Cur_Thd);

            if(Slices<RMP_MAX_SLICES)
            {
                RMP_COVERAGE_MARKER();
                _RMP_Dly_Ins(RMP_Cur_Thd, Slices);
                RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_RCVDLY);
            }
            else
            {
                RMP_COVERAGE_MARKER();
                RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_RCVBLK);
            }
            RMP_Unlock_Sched();
            
            /* Dummy read - to separate the lock & unlock. If the compiler optimizes these two
             * functions(inline them) on some architectures sometimes we never block. */
            *Data=RMP_Cur_Thd->Mailbox;
            
            /* We've been unblocked. There must be something in our mbox, or we should have failed */
            RMP_Lock_Sched();
            *Data=RMP_Cur_Thd->Mailbox;
            RMP_Cur_Thd->State&=~RMP_THD_MBOXFUL;
        }
    }
    
    RMP_Unlock_Sched();
    return RMP_Cur_Thd->Retval;
}
/* End Function:RMP_Thd_Rcv **************************************************/

/* Begin Function:RMP_Thd_Delay ***********************************************
Description : Delay the execution of a real-time thread.
Input       : rmp_ptr_t Slices - The number of timeslices to delay.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Delay(rmp_ptr_t Slices)
{
    if((Slices==0)||(Slices>=RMP_MAX_SLICES))
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SLICE;
    }
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Lock_Sched();

    /* We must be running and not suspended so we will be out of running queue */
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
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Cancel(volatile struct RMP_Thd* Thread)
{
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Lock_Sched();
    
    /* Is it delayed? */
    if(RMP_THD_STATE(Thread->State)!=RMP_THD_DELAYED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_STATE;
    }
    else
        RMP_COVERAGE_MARKER();

    /* Delete it from the delay list */
    RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
    RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
    /* Set to running if not suspended */
    _RMP_Set_Rdy(Thread);
    
    Thread->Retval=RMP_ERR_OPER;
    RMP_Unlock_Sched();
    return 0;
}
/* End Function:RMP_Thd_Cancel ***********************************************/

/* Begin Function:RMP_Sem_Crt *************************************************
Description : Create a semaphore in the system.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore structure.
              rmp_ptr_t Number - The initial number of this semaphore.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Crt(volatile struct RMP_Sem* Semaphore, rmp_ptr_t Number)
{
    /* Check if this semaphore structure could possibly be in use */
    if(Semaphore==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
    }
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Lock_Sched();
    
    if(Semaphore->State!=RMP_SEM_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_SEM;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Is the number too great to initialize? */
    if(Number>=RMP_SEM_MAX_NUM)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_OPER;
    }
    else
        RMP_COVERAGE_MARKER();

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
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Del(volatile struct RMP_Sem* Semaphore)
{
    struct RMP_Thd* Thread;
    
    /* Check if this semaphore structure could possibly be in use */
    if(Semaphore==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
    }
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Lock_Sched();
    
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_SEM;
    }
    else
        RMP_COVERAGE_MARKER();

    /* Get rid of all guys waiting on it */
    while(&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)
    {
        Thread=(struct RMP_Thd*)(Semaphore->Wait_List.Next);
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
        
        if(RMP_THD_STATE(Thread->State)==RMP_THD_SEMDLY)
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
        }
        else
            RMP_COVERAGE_MARKER();

        RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
        /* Set to running if not suspended */
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
              rmp_ptr_t Slices - The number of slices to wait.
Output      : None.
Return      : rmp_ret_t - If successful, the current semaphore number; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Pend(volatile struct RMP_Sem* Semaphore, rmp_ptr_t Slices)
{
    /* Check if this semaphore structure could possibly be in use */
    if(Semaphore==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
    }
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Lock_Sched();
    
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_SEM;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Check if we can get one immediately */
    if(Semaphore->Cur_Num!=0)
    {
        RMP_COVERAGE_MARKER();
        Semaphore->Cur_Num--;
        RMP_Unlock_Sched();
        return Semaphore->Cur_Num;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* Cannot get one, we need to block */
        if(Slices==0)
        {
            RMP_COVERAGE_MARKER();
            RMP_Unlock_Sched();
            return RMP_ERR_OPER;
        }
        else
            RMP_COVERAGE_MARKER();

        /* We must be running - place into waitlist now */
        _RMP_Clr_Rdy(RMP_Cur_Thd);
        RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),Semaphore->Wait_List.Prev,&(Semaphore->Wait_List));
        
        if(Slices<RMP_MAX_SLICES)
        {
            RMP_COVERAGE_MARKER();
            _RMP_Dly_Ins(RMP_Cur_Thd, Slices);
            RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_SEMDLY);
        }
        else
        {
            RMP_COVERAGE_MARKER();
            RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_SEMBLK);
        }
        
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
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Abort(volatile struct RMP_Thd* Thread)
{
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
        RMP_COVERAGE_MARKER();

    RMP_Lock_Sched();
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Is it waiting on a semaphore? If no, we abort and return an error code */
    if((RMP_THD_STATE(Thread->State)!=RMP_THD_SEMBLK)&&
       (RMP_THD_STATE(Thread->State)!=RMP_THD_SEMDLY))
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_STATE;
    }
    else
        RMP_COVERAGE_MARKER();

    /* Waiting for a semaphore. We abort it and return */
    RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
    if(RMP_THD_STATE(Thread->State)==RMP_THD_SEMDLY)
    {
        RMP_COVERAGE_MARKER();
        RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
    }
    else
        RMP_COVERAGE_MARKER();
    
    RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
    /* Set to running if not suspended */
    _RMP_Set_Rdy(Thread);
    
    Thread->Retval=RMP_ERR_OPER;
    RMP_Unlock_Sched();
    return 0;
}
/* End Function:RMP_Sem_Abort ************************************************/

/* Begin Function:RMP_Sem_Post ************************************************
Description : Post a number of semaphores to the list.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore structure.
              rmp_ptr_t Number - The number to post.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Post(volatile struct RMP_Sem* Semaphore, rmp_ptr_t Number)
{
    struct RMP_Thd* Thread;
    
    /* Check if this semaphore structure could possibly be in use */
    if((Semaphore==0)||(Number==0))
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
    }
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Lock_Sched();
    
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_SEM;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Would the maximum value be exceeded if this is posted? */
    if((Semaphore->Cur_Num+Number)>=RMP_SEM_MAX_NUM)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_OPER;
    }
    else
        RMP_COVERAGE_MARKER();
    
    Semaphore->Cur_Num+=Number;
    /* Is there any thread waiting on it? If there are, clean them up*/
    while((&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)&&(Semaphore->Cur_Num!=0))
    {
        Thread=(struct RMP_Thd*)(Semaphore->Wait_List.Next);
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
        
        if(RMP_THD_STATE(Thread->State)==RMP_THD_SEMDLY)
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
        /* Set to running if not suspended */
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
              We do not check whether the scheduler is locked; if we are calling this
              function, we're pretty sure that it's not.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore structure.
              rmp_ptr_t Number - The number to post.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Post_ISR(volatile struct RMP_Sem* Semaphore, rmp_ptr_t Number)
{
    struct RMP_Thd* Thread;
    /* Check if this semaphore structure could possibly be in use */
    if((Semaphore==0)||(Number==0))
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
    }
    else
        RMP_COVERAGE_MARKER();
    
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
    }
    else
        RMP_COVERAGE_MARKER();

    /* Would the maximum value be exceeded if this is posted? */
    if((Semaphore->Cur_Num+Number)>=RMP_SEM_MAX_NUM)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
        RMP_COVERAGE_MARKER();
    
    Semaphore->Cur_Num+=Number;
    /* Is there any thread waiting on it? If there are, clean them up*/
    while((&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)&&(Semaphore->Cur_Num!=0))
    {
        Thread=(struct RMP_Thd*)(Semaphore->Wait_List.Next);
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
        if(RMP_THD_STATE(Thread->State)==RMP_THD_SEMDLY)
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
        /* Set to running if not suspended */
        _RMP_Set_Rdy(Thread);
        
        /* If schedule pending, trigger it now because we are in ISR */
        if(RMP_Sched_Pend!=0)
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=0;
            _RMP_Yield();   
        }
        else
            RMP_COVERAGE_MARKER();

        /* Finally, return success */
        Thread->Retval=0;
        Semaphore->Cur_Num--;
    }

    return 0;
}
/* End Function:RMP_Sem_Post_ISR *********************************************/

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
        RMP_Init_Idle();
}
/* End Function:RMP_Init *****************************************************/

/* Begin Function:main ********************************************************
Description : The entry of the operating system. This function is for 
              compatibility with the toolchains.
Input       : None.
Output      : None.
Return      : int - This function never returns.
******************************************************************************/
int main(void)
{
    rmp_ptr_t Count;
    
#ifdef RMP_COVERAGE
    /* Initialize coverage markers if coverage enabled */
    for(Count=0;Count<RMP_COVERAGE_LINES;Count++)
        RMP_Coverage[Count]=0;
#endif
    
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
    _RMP_Start((rmp_ptr_t)RMP_Init, (rmp_ptr_t)RMP_Init_Thd.Stack);
    
    return 0;
}
/* End Function:main *********************************************************/

/* Begin Function:RMP_RBIT_Get ************************************************
Description : Reverse the bit order in a word.
Input       : rmp_ptr_t Val - The input value.
Output      : None.
Return      : rmp_ptr_t - The result.
******************************************************************************/
static const rmp_u8_t RMP_RBIT_Table[256]=
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

rmp_ptr_t RMP_RBIT_Get(rmp_ptr_t Val)
{
    rmp_ptr_t Ret;
    rmp_ptr_t Src;
    rmp_u8_t* To;
    rmp_u8_t* From;
    
    Src=Val;
    To=(rmp_u8_t*)(&Ret);
    From=(rmp_u8_t*)(&Src);
    
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
Description : Get the LSB in a word. If the word is all zero, a value that is 
              equal to the processor word size will be returned.
Input       : rmp_ptr_t Val - The input value.
Output      : None.
Return      : rmp_ptr_t - The LSB found.
******************************************************************************/
rmp_ptr_t RMP_LSB_Get(rmp_ptr_t Val)
{
    return RMP_WORD_SIZE-1-RMP_MSB_Get(RMP_RBIT_Get(Val));
}
/* End Function:RMP_LSB_Get **************************************************/

/* Begin Function:RMP_Mem_Init ************************************************
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
rmp_ret_t RMP_Mem_Init(volatile void* Pool, rmp_ptr_t Size)
{
    rmp_cnt_t FLI_Cnt;
    rmp_ptr_t Offset;
    rmp_ptr_t Bitmap_Size;
    volatile struct RMP_Mem* Mem;
    
    /* See if the memory pool is large enough to enable dynamic allocation - at
     * least 1024 machine words or pool initialization will be refused */
    if((Pool==0)||(Size<(1024*sizeof(rmp_ptr_t)))||((((rmp_ptr_t)Pool)+Size)<Size))
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_MEM;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* See if the address and size is word-aligned */
    if(((((rmp_ptr_t)Pool)&RMP_ALIGN_MASK)!=0)||((Size&RMP_ALIGN_MASK)!=0))
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_MEM;
    }
    else
        RMP_COVERAGE_MARKER();

    Mem=(volatile struct RMP_Mem*)Pool;
    Mem->Size=Size;
    /* Calculate the FLI value needed for this - we always align to 64 byte */
    Mem->FLI_Num=RMP_MSB_Get(Size-sizeof(struct RMP_Mem))-6+1;
    
    /* Decide the location of the bitmap */
    Offset=sizeof(struct RMP_Mem);
    Bitmap_Size=RMP_ROUND_UP(Mem->FLI_Num,RMP_ALIGN_ORDER);
    /* Initialize the bitmap */
    for(FLI_Cnt=0;FLI_Cnt<(rmp_cnt_t)(Bitmap_Size>>RMP_ALIGN_ORDER);FLI_Cnt++)
        Mem->Bitmap[FLI_Cnt]=0;
    
    /* Decide the location of the allocation table - "-sizeof(rmp_ptr_t)" is
     * because we defined the length=1 in our struct already */
    Offset+=Bitmap_Size-sizeof(rmp_ptr_t);
    Mem->Table=(struct RMP_List*)(((rmp_ptr_t)Mem)+Offset);
    /* Initialize the allocation table */
    for(FLI_Cnt=0;FLI_Cnt<(rmp_cnt_t)(Mem->FLI_Num);FLI_Cnt++)
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
    
    /* Calculate the offset of the actual allocatable memory - each FLI have
     * 8 SLIs, and each SLI has a corresponding table header */
    Offset+=sizeof(struct RMP_List)*8*Mem->FLI_Num;
    Mem->Start=((rmp_ptr_t)Mem)+Offset;
    
    /* Initialize the first big block */
    _RMP_Mem_Block((struct RMP_Mem_Head*)(Mem->Start),Size-Offset);
    /* Insert the memory into the corresponding level */
    _RMP_Mem_Ins(Pool,(struct RMP_Mem_Head*)(Mem->Start));
    
    return 0;
}
/* End Function:RMP_Mem_Init *************************************************/

/* Begin Function:_RMP_Mem_Block **********************************************
Description : Make a memory block from the memory trunk. The memory block is always
              free when created. No parameter check performed here.
Input       : volatile struct RMP_Mem_Head* Addr - The start address of the
                                                   memory block, word-aligned.
              rmp_ptr_t Size - The size of the memory block, word-aligned.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Mem_Block(volatile struct RMP_Mem_Head* Addr, rmp_ptr_t Size)
{
    volatile struct RMP_Mem_Head* Mem_Head;
    
    /* Get the big memory block's size and position */
    Mem_Head=(struct RMP_Mem_Head*)Addr;
    
    /* Initialize the big memory block */
    Mem_Head->State=RMP_MEM_FREE;
    Mem_Head->Tail=(struct RMP_Mem_Tail*)(((rmp_ptr_t)Mem_Head)+Size-sizeof(struct RMP_Mem_Tail));
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
    rmp_ptr_t FLI_Level;
    rmp_ptr_t SLI_Level;
    rmp_ptr_t Level;
    rmp_ptr_t Size;
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_List* Slot;
    
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(rmp_ptr_t)(Mem_Head->Tail)-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);

    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    /* Calculate the bit position */
    Level=RMP_MEM_POS(FLI_Level,SLI_Level);
    /* Get the slot */
    Slot=&(Mem->Table[Level]);

    /* See if there are any blocks in the level, equal means no. So what we inserted is the first block */
    if(Slot==Slot->Next)
    {
        RMP_COVERAGE_MARKER();
        /* Set the corresponding bit in the TLSF bitmap */
        Mem->Bitmap[Level>>RMP_WORD_ORDER]|=RMP_POW2(Level&RMP_WORD_MASK);
    }
    else
        RMP_COVERAGE_MARKER();

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
    rmp_ptr_t FLI_Level;
    rmp_ptr_t SLI_Level;
    rmp_ptr_t Level;
    rmp_ptr_t Size;
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_List* Slot;    
    
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(rmp_ptr_t)(Mem_Head->Tail)-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);
    
    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    /* Calculate the bit position */
    Level=RMP_MEM_POS(FLI_Level,SLI_Level);
    /* Get the slot */
    Slot=&(Mem->Table[Level]);

    /* Delete the node now */
    RMP_List_Del(Mem_Head->Head.Prev,Mem_Head->Head.Next);

    /* See if there are any blocks in the level, equal means no. So
     * what we deleted is the last blockm need to clear the flag */
    if(Slot==Slot->Next)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the corresponding bit in the TLSF bitmap */
        Mem->Bitmap[Level>>RMP_WORD_ORDER]&=~RMP_POW2(Level&RMP_WORD_MASK);
    }
    else
        RMP_COVERAGE_MARKER();
}
/* End Function:_RMP_Mem_Del *************************************************/

/* Begin Function:_RMP_Mem_Search *********************************************
Description : The TLSF memory searcher.
Input       : rmp_ptr_t Size - The memory size, must be bigger than 64. This must be
                               guaranteed before calling this function or an error
                               will unavoidably occur.
Output      : rmp_cnt_t* FLI_Level - The FLI level found.
              rmp_cnt_t* SLI_Level - The SLI level found.
Return      : rmp_ret_t - If successful, 0; else -1 for failure.
******************************************************************************/
rmp_ret_t _RMP_Mem_Search(volatile void* Pool, rmp_ptr_t Size, rmp_cnt_t* FLI_Level, rmp_cnt_t* SLI_Level)
{
    rmp_ptr_t FLI_Level_Temp;
    rmp_ptr_t SLI_Level_Temp;
    rmp_cnt_t Level;
    rmp_cnt_t Word;
    rmp_cnt_t Limit;
    rmp_ptr_t LSB;
    volatile struct RMP_Mem* Mem;

    /* Make sure that it is bigger than 64. 64=2^6 */
    FLI_Level_Temp=RMP_MSB_Get(Size)-6;
    
    /* Decide the SLI level directly from the FLI level. We plus the number by one here
     * so that we can avoid the list search. However, when the allocated memory is just
     * one of the levels, then we don't need to jump to the next level and can fit directly */
    SLI_Level_Temp=(Size>>(FLI_Level_Temp+3))&0x07;
    if(Size!=(RMP_POW2(FLI_Level_Temp+3)*(SLI_Level_Temp+8)))
    {
        RMP_COVERAGE_MARKER();
        SLI_Level_Temp++;
        
        /* If the SLI level is the largest of the SLI level, then jump to the next FLI level */
        if(SLI_Level_Temp==8)
        {
            RMP_COVERAGE_MARKER();
            FLI_Level_Temp+=1;
            SLI_Level_Temp=0;
        }
        else
            RMP_COVERAGE_MARKER();
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Check if the FLI level is over the boundary */
    Mem=(volatile struct RMP_Mem*)Pool;
    if(FLI_Level_Temp>=Mem->FLI_Num)
    {
        RMP_COVERAGE_MARKER();
        return -1;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Try to find one position on this processor word level */
    Level=RMP_MEM_POS(FLI_Level_Temp,SLI_Level_Temp);
    LSB=RMP_LSB_Get(Mem->Bitmap[Level>>RMP_WORD_ORDER]>>(Level&RMP_WORD_MASK));
    /* If there's at least one block that matches the query, return the level */
    if(LSB<RMP_WORD_SIZE)
    {
        RMP_COVERAGE_MARKER();
        Level=(Level&(~RMP_WORD_MASK))+LSB+(Level&RMP_WORD_MASK);
        *FLI_Level=Level>>3;
        *SLI_Level=Level&0x07;
        return 0;
    }
    /* No one exactly fits */
    else
    {
        RMP_COVERAGE_MARKER();
        Limit=RMP_ROUND_UP(Mem->FLI_Num,RMP_ALIGN_ORDER)>>RMP_ALIGN_ORDER;
        /* From the next word, query one by one */
        for(Word=(Level>>RMP_WORD_ORDER)+1;Word<Limit;Word++)
        {
            /* If the level has blocks of one FLI level */
            if(Mem->Bitmap[Word]!=0)
            {
                RMP_COVERAGE_MARKER();
                /* Find the actual level */ 
                LSB=RMP_LSB_Get(Mem->Bitmap[Word]);
                *FLI_Level=((Word<<RMP_WORD_ORDER)+LSB)>>3;
                *SLI_Level=LSB&0x07;
                return 0;
            }
            else
                RMP_COVERAGE_MARKER();
        }
    }

    /* Search failed */
    return -1;
}
/* End Function:_RMP_Mem_Search **********************************************/

/* Begin Function:RMP_Malloc **************************************************
Description : Allocate some memory from a designated memory pool.
Input       : volatile void* Pool - The pool to allocate from.
              rmp_ptr_t Size - The size of the RAM needed to allocate.
Output      : None.
Return      : void* - The pointer to the memory. If no memory available, 0 is returned.
******************************************************************************/
void* RMP_Malloc(volatile void* Pool, rmp_ptr_t Size)
{    
    rmp_cnt_t FLI_Level;
    rmp_cnt_t SLI_Level;
    volatile struct RMP_Mem* Mem;
    rmp_ptr_t Old_Size;
    volatile struct RMP_Mem_Head* Mem_Head;
    rmp_ptr_t Rounded_Size;
    volatile struct RMP_Mem_Head* New_Mem;
    rmp_ptr_t New_Size;
    
    if((Pool==0)||(Size==0))
    {
        RMP_COVERAGE_MARKER();
        return (void*)(0);
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Round up the size:a multiple of 8 and bigger than 64B */
    Rounded_Size=RMP_ROUND_UP(Size,3);
    /* See if it is smaller than the smallest block */
    Rounded_Size=(Rounded_Size>64)?Rounded_Size:64;

    /* See if such block exists, if not, abort */
    if(_RMP_Mem_Search(Pool,Rounded_Size,&FLI_Level,&SLI_Level)!=0)
    {
        RMP_COVERAGE_MARKER();
        return (void*)(0);
    }
    else
        RMP_COVERAGE_MARKER();
    
    Mem=(volatile struct RMP_Mem*)Pool;
    
    /* There is such block. Get it and delete it from the TLSF list. */
    Mem_Head=(struct RMP_Mem_Head*)(Mem->Table[RMP_MEM_POS(FLI_Level,SLI_Level)].Next);
    _RMP_Mem_Del(Pool, Mem_Head);

    /* Allocate and calculate if the space left could be big enough to be a new 
     * block. If so, we will put the block back into the TLSF table */
    New_Size=((rmp_ptr_t)(Mem_Head->Tail))-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head)-Rounded_Size;
    if(New_Size>=(sizeof(struct RMP_Mem_Head)+64+sizeof(struct RMP_Mem_Tail)))
    {
        RMP_COVERAGE_MARKER();
        Old_Size=sizeof(struct RMP_Mem_Head)+Rounded_Size+sizeof(struct RMP_Mem_Tail);
        New_Mem=(volatile struct RMP_Mem_Head*)(((rmp_ptr_t)Mem_Head)+Old_Size);

        _RMP_Mem_Block(Mem_Head, Old_Size);
        _RMP_Mem_Block(New_Mem, New_Size);

        /* Put the extra block back */
        _RMP_Mem_Ins(Pool, New_Mem);
    }
    else
        RMP_COVERAGE_MARKER();

    /* Mark the block as in use */
    Mem_Head->State=RMP_MEM_USED;

    /* Finally, return the start address */
    return (void*)(((rmp_ptr_t)Mem_Head)+sizeof(struct RMP_Mem_Head));
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
    rmp_cnt_t Merge_Left;

    /* Check if pointer is null */
    if((Pool==0)||(Mem_Ptr==0))
    {
        RMP_COVERAGE_MARKER();
        return;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* See if the address is within the allocatable address range. If not, abort directly. */
    Mem=(volatile struct RMP_Mem*)Pool;
    if((((rmp_ptr_t)Mem_Ptr)<=((rmp_ptr_t)Mem))||(((rmp_ptr_t)Mem_Ptr)>=(((rmp_ptr_t)Mem)+Mem->Size)))
    {
        RMP_COVERAGE_MARKER();
        return;
    }
    else
        RMP_COVERAGE_MARKER();

    Mem_Head=(struct RMP_Mem_Head*)(((rmp_ptr_t)Mem_Ptr)-sizeof(struct RMP_Mem_Head));
    /* See if the block can really be freed */
    if(Mem_Head->State==RMP_MEM_FREE)
    {
        RMP_COVERAGE_MARKER();
        return;
    }
    else
        RMP_COVERAGE_MARKER();

    /* Mark it as free */
    Mem_Head->State=RMP_MEM_FREE;
    
    /* Now check if we can merge it with the higher blocks */
    Right_Head=(struct RMP_Mem_Head*)(((rmp_ptr_t)(Mem_Head->Tail))+sizeof(struct RMP_Mem_Tail));
    if(((rmp_ptr_t)Right_Head)!=(((rmp_ptr_t)Mem)+Mem->Size))
    {
        RMP_COVERAGE_MARKER();
        /* If this one is unoccupied */
        if((Right_Head->State)==RMP_MEM_FREE)
        {
            RMP_COVERAGE_MARKER();
            /* Delete, merge */
            _RMP_Mem_Del(Pool,Right_Head);
            _RMP_Mem_Block(Mem_Head,
                           ((rmp_ptr_t)(Right_Head->Tail))+sizeof(struct RMP_Mem_Tail)-(rmp_ptr_t)Mem_Head);
        }
        else
            RMP_COVERAGE_MARKER();
    }
    else
        RMP_COVERAGE_MARKER();

    /* Now check if we can merge it with the lower blocks */
    Merge_Left=0;
    if((rmp_ptr_t)Mem_Head!=Mem->Start)
    {
        RMP_COVERAGE_MARKER();
        Left_Head=((struct RMP_Mem_Tail*)(((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Tail)))->Head;

        /* If this one is unoccupied */
        if(Left_Head->State==RMP_MEM_FREE)
        {
            RMP_COVERAGE_MARKER();
            /* Delete, merge */
            _RMP_Mem_Del(Pool, Left_Head);
            _RMP_Mem_Block(Left_Head,
                           (rmp_ptr_t)((rmp_ptr_t)(Mem_Head->Tail)+sizeof(struct RMP_Mem_Tail)-(rmp_ptr_t)Left_Head));

            /* We have completed the merge here and the original block has destroyed.
             * Thus there's no need to insert it into the list again */
            Merge_Left=1;
        }
        else
            RMP_COVERAGE_MARKER();
    }
    else
        RMP_COVERAGE_MARKER();

    /* If we did not merge it with the left-side blocks, insert the original pointer's block 
     * into the TLSF table(Merging with the right-side one won't disturb this) */
    if(Merge_Left==0)
    {
        RMP_COVERAGE_MARKER();
        _RMP_Mem_Ins(Pool, Mem_Head);
    }
    else
    {
        RMP_COVERAGE_MARKER();
        _RMP_Mem_Ins(Pool, Left_Head);
    }
}
/* End Function:RMP_Free *****************************************************/

/* Begin Function:RMP_Realloc *************************************************
Description : Expand or shrink an allocation to the desired size. The behavior
              of this function equals RMP_Malloc if the Mem_Ptr passed in is 0,
              or RMP_Free if the Size passed in is 0.
Input       : volatile void* Pool - The pool to allocate from.
              void* Mem_Ptr - The old memory block to expand.
              rmp_ptr_t Size - The size of the RAM needed to resize to.
Output      : None.
Return      : void* - The pointer to the memory. If no memory available or an
                      error occurred, 0 is returned.
******************************************************************************/
void* RMP_Realloc(volatile void* Pool, void* Mem_Ptr, rmp_ptr_t Size)
{
    /* The size of the original memory block */
    rmp_ptr_t Mem_Size;
    /* The rounded size of the new memory request */
    rmp_ptr_t Rounded_Size;
    rmp_ptr_t Count;
    /* The pointer to the pool */
    volatile struct RMP_Mem* Mem;
    /* The head of the old memory */
    volatile struct RMP_Mem_Head* Mem_Head;
    /* The right-side block head */
    volatile struct RMP_Mem_Head* Right_Head;
    /* The pointer to the residue memory head */
    volatile struct RMP_Mem_Head* Res_Mem;
    /* The new memory block */
    void* New_Mem;
    /* The size of the memory block including the header sizes */
    rmp_ptr_t Old_Size;
    /* The size of the residue memory block including the header sizes */
    rmp_ptr_t Res_Size;
    
    /* Check if no pool present */
    if(Pool==0)
    {
        RMP_COVERAGE_MARKER();
        return 0;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Are we passing in a NULL pointer? */
    if(Mem_Ptr==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_Malloc(Pool,Size);
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Is the size passed in zero? If yes, we free directly */
    if(Size==0)
    {
        RMP_COVERAGE_MARKER();
        RMP_Free(Pool,Mem_Ptr);
        return 0;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* See if the address is within the allocatable address range. If not, abort directly. */
    Mem=(volatile struct RMP_Mem*)Pool;
    if((((rmp_ptr_t)Mem_Ptr)<=((rmp_ptr_t)Mem))||(((rmp_ptr_t)Mem_Ptr)>=(((rmp_ptr_t)Mem)+Mem->Size)))
    {
        RMP_COVERAGE_MARKER();
        return 0;
    }
    else
        RMP_COVERAGE_MARKER();

    /* Yes, get the location of the header of the memory */
    Mem_Head=(struct RMP_Mem_Head*)(((rmp_ptr_t)Mem_Ptr)-sizeof(struct RMP_Mem_Head));
    /* See if the block can really be realloced */
    if(Mem_Head->State==RMP_MEM_FREE)
    {
        RMP_COVERAGE_MARKER();
        return 0;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Round up the size:a multiple of 8 and bigger than 64B */
    Rounded_Size=RMP_ROUND_UP(Size,3);
    /* See if it is smaller than the smallest block */
    Rounded_Size=(Rounded_Size>64)?Rounded_Size:64;
    
    Mem_Size=((rmp_ptr_t)Mem_Head->Tail)-((rmp_ptr_t)Mem_Ptr);
    /* Does the right-side head exist at all? */
    Right_Head=(struct RMP_Mem_Head*)(((rmp_ptr_t)(Mem_Head->Tail))+sizeof(struct RMP_Mem_Tail));
    if(((rmp_ptr_t)Right_Head)==(((rmp_ptr_t)Mem)+Mem->Size))
    {
        RMP_COVERAGE_MARKER();
        Right_Head=0;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Are we gonna expand it? */
    if(Mem_Size<Rounded_Size)
    {
        /* Expanding */
        RMP_COVERAGE_MARKER();
        /* Does the right side exist at all? */
        if(Right_Head!=0)
        {
            RMP_COVERAGE_MARKER();
            /* Is it allocated? */
            if(Right_Head->State==RMP_MEM_FREE)
            {
                RMP_COVERAGE_MARKER();
                /* Right-side exists and is free. How big is its usable size? Is it sufficient for our realloc? */
                if((((rmp_ptr_t)Right_Head->Tail)-((rmp_ptr_t)Mem_Ptr))>=Rounded_Size)
                {
                    RMP_COVERAGE_MARKER();
                    /* Remove the right-side from the free list so we can operate on it */
                    _RMP_Mem_Del(Pool, Right_Head);   
                    /* Allocate and calculate if the space left could be big enough to be a new 
                     * block. If so, we will put the block back into the TLSF table */
                    Res_Size=((rmp_ptr_t)(Right_Head->Tail))-((rmp_ptr_t)Mem_Ptr)-Rounded_Size;
                    /* Is the residue big enough to be a block? */
                    if(Res_Size>=(sizeof(struct RMP_Mem_Head)+64+sizeof(struct RMP_Mem_Tail)))
                    {
                        RMP_COVERAGE_MARKER();
                        Old_Size=sizeof(struct RMP_Mem_Head)+Rounded_Size+sizeof(struct RMP_Mem_Tail);
                        Res_Mem=(volatile struct RMP_Mem_Head*)(((rmp_ptr_t)Mem_Head)+Old_Size);

                        _RMP_Mem_Block(Mem_Head, Old_Size);
                        _RMP_Mem_Block(Res_Mem, Res_Size);

                        /* Put the extra block back */
                        _RMP_Mem_Ins(Pool, Res_Mem);
                    }
                    else
                    {
                        /* Residue too small. Merging the whole thing in is the only option */
                        RMP_COVERAGE_MARKER();
                        Old_Size=((rmp_ptr_t)(Right_Head->Tail))-((rmp_ptr_t)Mem_Head)+sizeof(struct RMP_Mem_Tail);
                        _RMP_Mem_Block(Mem_Head, Old_Size);
                    }
                    
                    /* Mark the block as in use (making new block clears this flag) */
                    Mem_Head->State=RMP_MEM_USED;
                    /* Return the old pointer because we expanded it */
                    return Mem_Ptr;
                }
                /* Right-side not large enough, have to go malloc then memcpy */
                else
                    RMP_COVERAGE_MARKER();
            }
            /* It is allocated, have to go malloc then memcpy */
            else
                RMP_COVERAGE_MARKER();
        }
        /* Right-side doesn't exist, have to go malloc then memcpy */
        else
            RMP_COVERAGE_MARKER();
        
        New_Mem=RMP_Malloc(Pool,Rounded_Size);
        /* See if we can allocate this much, if we can't at all, exit */
        if(New_Mem==0)
        {
            RMP_COVERAGE_MARKER();
            return 0;
        }
        else
            RMP_COVERAGE_MARKER();
        
        /* Copy old memory to new memory - we know that this is always aligned, so this is fine */
        for(Count=0;Count<(Mem_Size>>RMP_ALIGN_ORDER);Count++)
            ((rmp_ptr_t*)New_Mem)[Count]=((rmp_ptr_t*)Mem_Ptr)[Count];
        
        /* Free old memory then return */
        RMP_Free(Pool,Mem_Ptr);
        return New_Mem;
    }
    /* Shrinking or keeping */
    else
        RMP_COVERAGE_MARKER();
    
    /* Are we keeping the size? */
    if(Mem_Size==Rounded_Size)
    {
        RMP_COVERAGE_MARKER();
        return Mem_Ptr;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Does the right side exist at all? */
    if(Right_Head!=0)
    {
        RMP_COVERAGE_MARKER();
        /* Is it allocated? */
        if(Right_Head->State==RMP_MEM_FREE)
        {
            /* Right-side not allocated. Need to merge the block */
            RMP_COVERAGE_MARKER();
            /* Remove the right-side from the allocation list so we can operate on it */
            _RMP_Mem_Del(Pool, Right_Head);
            Res_Size=((rmp_ptr_t)(Right_Head->Tail))-((rmp_ptr_t)Mem_Ptr)-Rounded_Size;
            Old_Size=sizeof(struct RMP_Mem_Head)+Rounded_Size+sizeof(struct RMP_Mem_Tail);
            Res_Mem=(volatile struct RMP_Mem_Head*)(((rmp_ptr_t)Mem_Head)+Old_Size);

            _RMP_Mem_Block(Mem_Head, Old_Size);
            _RMP_Mem_Block(Res_Mem, Res_Size);

            /* Put the extra block back */
            _RMP_Mem_Ins(Pool, Res_Mem);
            
            /* Mark the block as in use (making new block clears this flag) */
            Mem_Head->State=RMP_MEM_USED;
            /* Return the old pointer because we shrinked it */
            return Mem_Ptr;
        }
        /* Allocated. Need to see if the residue block itself is large enough to be inserted back */
        else
            RMP_COVERAGE_MARKER();
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* The right-side head either does not exist or is allocated. Calculate the resulting residue size */
    Res_Size=Mem_Size-Rounded_Size;
    if(Res_Size<(sizeof(struct RMP_Mem_Head)+64+sizeof(struct RMP_Mem_Tail)))
    {
        RMP_COVERAGE_MARKER();
        /* The residue block wouldn't even count as a small one. Do nothing and quit */
        return Mem_Ptr;
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* The residue will be big enough to become a standalone block. We need to place it back */ 
    Old_Size=sizeof(struct RMP_Mem_Head)+Rounded_Size+sizeof(struct RMP_Mem_Tail);
    Res_Mem=(volatile struct RMP_Mem_Head*)(((rmp_ptr_t)Mem_Head)+Old_Size);

    _RMP_Mem_Block(Mem_Head, Old_Size);
    _RMP_Mem_Block(Res_Mem, Res_Size);

    /* Put the extra block back */
    _RMP_Mem_Ins(Pool, Res_Mem);
            
    /* Mark the block as in use (making new block clears this flag) */
    Mem_Head->State=RMP_MEM_USED;
    /* Return the old pointer because we shrinked it */
    return Mem_Ptr;
}
/* End Function:RMP_Realloc **************************************************/

/* Begin Function:RMP_Line ****************************************************
Description : Draw a line given the start and end coordinates.
Input       : rmp_cnt_t Start_X - The start point X coordinate.
              rmp_cnt_t Start_Y - The start point Y coordinate.
              rmp_cnt_t End_X - The end point X coordinate.
              rmp_cnt_t End_Y - The end point Y coordinate.
              rmp_ptr_t Color - The color of the line.
Output      : None.
Return      : None.
******************************************************************************/
#ifdef RMP_POINT
void RMP_Line(rmp_cnt_t Start_X, rmp_cnt_t Start_Y, rmp_cnt_t End_X, rmp_cnt_t End_Y, rmp_ptr_t Color)
{
    rmp_cnt_t Trav_X;
    rmp_cnt_t Trav_Y;
    rmp_cnt_t Dir_X;
    rmp_cnt_t Dir_Y;
    rmp_cnt_t Error;
    rmp_cnt_t Cur_X;
    rmp_cnt_t Cur_Y;
    
    /* See if this line is horizontal or vertical. If so we speed it up */
    if(Start_X==End_X)
    {
        /* Vertical */
        if(Start_Y>End_Y)
        {
            Dir_Y=End_Y;
            Trav_Y=Start_Y;
        }
        else
        {
            Dir_Y=Start_Y;
            Trav_Y=End_Y;
        }
        
        for(Cur_Y=Dir_Y;Cur_Y<=Trav_Y;Cur_Y++)
            RMP_POINT(Start_X,Cur_Y,Color);
        return;
    }
    else if(Start_Y==End_Y)
    {
        /* Horizontal */
        if(Start_X>End_X)
        {
            Dir_X=End_X;
            Trav_X=Start_X;
        }
        else
        {
            Dir_X=Start_X;
            Trav_X=End_X;
        }
        
        for(Cur_X=Dir_X;Cur_X<=Trav_X;Cur_X++)
            RMP_POINT(Cur_X,Start_Y,Color);
        return;
    }

    Error=0;
    /* Get their absolute value, and then draw the line */
    Trav_X=(Start_X>End_X)?(Start_X-End_X):(End_X-Start_X);
    Trav_Y=(Start_Y>End_Y)?(Start_Y-End_Y):(End_Y-Start_Y);

    /* Decide the increment direction */
    if(End_X-Start_X>0)
        Dir_X=1;
    else
        Dir_X=-1;

    if(End_Y-Start_Y>0)
        Dir_Y=1;
    else
        Dir_Y=-1;

    if(Trav_X>Trav_Y)
    {
        Cur_Y=Start_Y;
        for(Cur_X=Start_X;Cur_X!=End_X+Dir_X;Cur_X+=Dir_X)
        {
            RMP_POINT(Cur_X,Cur_Y,Color);
            Error+=Trav_Y;
            if((Error<<1)>=Trav_X)
            {
                Cur_Y+=Dir_Y;
                Error-=Trav_X;
            }
        }
    }
    else
    {
        Cur_X=Start_X;
        for(Cur_Y=Start_Y;Cur_Y!=End_Y+Dir_Y;Cur_Y+=Dir_Y)
        {
            RMP_POINT(Cur_X,Cur_Y,Color);
            Error+=Trav_X;
            if((Error<<1)>=Trav_Y)
            {
                Cur_X+=Dir_X;
                Error-=Trav_Y;
            }
        }
    }
}
/* End Function:RMP_Line *****************************************************/

/* Begin Function:RMP_Dot_Line ************************************************
Description : Draw a dotted line given the start and end coordinates.
Input       : rmp_cnt_t Start_X - The start point X coordinate.
              rmp_cnt_t Start_Y - The start point Y coordinate.
              rmp_cnt_t End_X - The end point X coordinate.
              rmp_cnt_t End_Y - The end point Y coordinate.
              rmp_ptr_t Dot - The color of the dotted line.
              rmp_ptr_t Space - The color of the white space. "RMP_TRANS" for nothing.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Dot_Line(rmp_cnt_t Start_X, rmp_cnt_t Start_Y, rmp_cnt_t End_X,rmp_cnt_t End_Y, rmp_ptr_t Dot, rmp_ptr_t Space)
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
    Trav_X=(Start_X>End_X)?(Start_X-End_X):(End_X-Start_X);
    Trav_Y=(Start_Y>End_Y)?(Start_Y-End_Y):(End_Y-Start_Y);

    /* Decide the increment direction */
    if(End_X-Start_X>0)
        Dir_X=1;
    else
        Dir_X=-1;

    if(End_Y-Start_Y>0)
        Dir_Y=1;
    else
        Dir_Y=-1;

    if(Trav_X>Trav_Y)
    {
        Cur_Y=Start_Y;
        for(Cur_X=Start_X;Cur_X!=End_X+Dir_X;Cur_X+=Dir_X)
        {
            /* Draw the dot and the white space alternatively */
            if((Cur_X&0x01)!=0)
                RMP_POINT(Cur_X,Cur_Y,Dot);
            else
            {
                if(Space!=RMP_TRANS)
                    RMP_POINT(Cur_X,Cur_Y,Space);
            }
            Error+=Trav_Y;
            if((Error<<1)>=Trav_X)
            {
                Cur_Y+=Dir_Y;
                Error-=Trav_X;
            }
        }
    }
    else
    {
        Cur_X=Start_X;
        for(Cur_Y=Start_Y;Cur_Y!=End_Y+Dir_Y;Cur_Y+=Dir_Y)
        {
            /* Draw the dot and the white space alternatively */
            if((Cur_Y&0x01)!=0)
                RMP_POINT(Cur_X,Cur_Y,Dot);
            else
            {
                if(Space!=RMP_TRANS)
                    RMP_POINT(Cur_X,Cur_Y,Space);
            }

            Error+=Trav_X;
            if((Error<<1)>=Trav_Y)
            {
                Cur_X+=Dir_X;
                Error-=Trav_Y;
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
void RMP_Rectangle(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_cnt_t Width, rmp_ptr_t Border, rmp_ptr_t Fill)
{
    rmp_cnt_t Line_Cnt;
    
    if(Fill!=RMP_TRANS)
    {
        for(Line_Cnt=0;Line_Cnt<Width;Line_Cnt++)
            RMP_Line(Coord_X,Coord_Y+Line_Cnt,Coord_X+Length-1,Coord_Y+Line_Cnt,Fill);
    }

    if(Border!=RMP_TRANS)
    {
        RMP_Line(Coord_X,Coord_Y,Coord_X+Length-1,Coord_Y,Border);
        RMP_Line(Coord_X+Length-1,Coord_Y,Coord_X+Length-1,Coord_Y+Width-1,Border);
        RMP_Line(Coord_X+Length-1,Coord_Y+Width-1,Coord_X,Coord_Y+Width-1,Border);
        RMP_Line(Coord_X,Coord_Y+Width-1,Coord_X,Coord_Y,Border);
    }
}
/* End Function:RMP_Rectangle ************************************************/

/* Begin Function:RMP_Round_Rect **********************************************
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
void RMP_Round_Rect(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y,
                    rmp_cnt_t Length, rmp_cnt_t Width, rmp_cnt_t Round, rmp_ptr_t Color)
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
    RMP_Rectangle(Coord_X+Round+1,Coord_Y+Round+1,Length-Round-Round-2,Width-Round-Round-2,Color,Color);

    /* Draw 4 small side rectangles */
    RMP_Rectangle(Coord_X,Cir_Y_0,Round+1,Width-Round-Round-2,Color,Color);
    RMP_Rectangle(Cir_X_1,Cir_Y_0,Round+1,Width-Round-Round-2,Color,Color);
    RMP_Rectangle(Cir_X_0,Cir_Y_1,Length-Round-Round-2,Round+1,Color,Color);
    RMP_Rectangle(Cir_X_0,Coord_Y,Length-Round-Round-2,Round+1,Color,Color);
    
    /* Draw 4 circles */
    RMP_Circle(Cir_X_0,Cir_Y_0,Round,Color,Color);
    RMP_Circle(Cir_X_1,Cir_Y_0,Round,Color,Color);
    RMP_Circle(Cir_X_0,Cir_Y_1,Round,Color,Color);
    RMP_Circle(Cir_X_1,Cir_Y_1,Round,Color,Color);
}
/* End Function:RMP_Round_Rect ***********************************************/

/* Begin Function:RMP_Circle **************************************************
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
void RMP_Circle(rmp_cnt_t Center_X, rmp_cnt_t Center_Y, rmp_cnt_t Radius, rmp_ptr_t Border, rmp_ptr_t Fill)
{
    rmp_cnt_t Cur_X;
    rmp_cnt_t Cur_Y;
    rmp_cnt_t Fill_Y;
    rmp_cnt_t Error;  
    rmp_cnt_t Quick;

    Cur_X=0;
    Cur_Y=Radius;
    Error=3-(Radius<<1);

    if(Fill!=RMP_TRANS)
    {  
        /* When we are filling the circle, we can try not to fill the areas that have been filled
         * using the rectangle fill method to increase fill speed.
         * Square 2 is 1.414. Here we let the condition to be 1.5. */
        Quick=(Radius<<1)/3;
        /* First, fill the square hole using the fast direct-fill method */
        RMP_Rectangle(Center_X-Quick,Center_Y-Quick, (Quick<<1)+1,(Quick<<1)+1, Fill, Fill);  

        while(Cur_X<=Cur_Y) 
        {  
            if(Cur_X<Quick)
            {
                for(Fill_Y=Cur_X;Fill_Y<=Cur_Y;Fill_Y++)  
                {
                    if(Fill_Y<Quick)
                        continue;
                    
                    RMP_POINT(Center_X+Cur_X,Center_Y+Fill_Y,Fill);  
                    RMP_POINT(Center_X-Cur_X,Center_Y+Fill_Y,Fill);  
                    RMP_POINT(Center_X+Cur_X,Center_Y-Fill_Y,Fill); 
                    RMP_POINT(Center_X-Cur_X,Center_Y-Fill_Y,Fill);  
                    RMP_POINT(Center_X+Fill_Y,Center_Y+Cur_X,Fill); 
                    RMP_POINT(Center_X-Fill_Y,Center_Y+Cur_X,Fill);  
                    RMP_POINT(Center_X+Fill_Y,Center_Y-Cur_X,Fill);  
                    RMP_POINT(Center_X-Fill_Y,Center_Y-Cur_X,Fill);
                }
            }
            /* Here the "Cur_X" is already out of range. We do not check the conditions anymore */
            else
            {
                for(Fill_Y=Cur_X;Fill_Y<=Cur_Y;Fill_Y++)  
                {                   
                    RMP_POINT(Center_X+Cur_X,Center_Y+Fill_Y,Fill);  
                    RMP_POINT(Center_X-Cur_X,Center_Y+Fill_Y,Fill);  
                    RMP_POINT(Center_X+Cur_X,Center_Y-Fill_Y,Fill); 
                    RMP_POINT(Center_X-Cur_X,Center_Y-Fill_Y,Fill);  
                    RMP_POINT(Center_X+Fill_Y,Center_Y+Cur_X,Fill); 
                    RMP_POINT(Center_X-Fill_Y,Center_Y+Cur_X,Fill);  
                    RMP_POINT(Center_X+Fill_Y,Center_Y-Cur_X,Fill);  
                    RMP_POINT(Center_X-Fill_Y,Center_Y-Cur_X,Fill);
                }
            }
            RMP_POINT(Center_X+Cur_X,Center_Y+Cur_Y,Border);  
            RMP_POINT(Center_X-Cur_X,Center_Y+Cur_Y,Border);  
            RMP_POINT(Center_X+Cur_X,Center_Y-Cur_Y,Border); 
            RMP_POINT(Center_X-Cur_X,Center_Y-Cur_Y,Border);  
            RMP_POINT(Center_X+Cur_Y,Center_Y+Cur_X,Border); 
            RMP_POINT(Center_X-Cur_Y,Center_Y+Cur_X,Border);  
            RMP_POINT(Center_X+Cur_Y,Center_Y-Cur_X,Border);  
            RMP_POINT(Center_X-Cur_Y,Center_Y-Cur_X,Border);

            if(Error<0) 
                Error=Error+(Cur_X<<2)+6;
            else 
            {  
                Error=Error+((Cur_X-Cur_Y)<<2)+10;  
                Cur_Y--;  
            }  
            Cur_X++;  
        }
    }
    else 
    {  
        /* Border only */ 
        while(Cur_X<=Cur_Y) 
        {
            RMP_POINT(Center_X+Cur_X,Center_Y+Cur_Y,Border);  
            RMP_POINT(Center_X-Cur_X,Center_Y+Cur_Y,Border);  
            RMP_POINT(Center_X+Cur_X,Center_Y-Cur_Y,Border); 
            RMP_POINT(Center_X-Cur_X,Center_Y-Cur_Y,Border);  
            RMP_POINT(Center_X+Cur_Y,Center_Y+Cur_X,Border); 
            RMP_POINT(Center_X-Cur_Y,Center_Y+Cur_X,Border);  
            RMP_POINT(Center_X+Cur_Y,Center_Y-Cur_X,Border);  
            RMP_POINT(Center_X-Cur_Y,Center_Y-Cur_X,Border);

            if(Error<0)
                Error=Error+(Cur_X<<2)+6;
            else 
            {
                Error=Error+((Cur_X-Cur_Y)<<2);
                Cur_Y--;
            }
            Cur_X++;  
        }  
    }
}
/* End Function:RMP_Circle ***************************************************/

/* Begin Function:RMP_Matrix **************************************************
Description : Display a monochrome bit map in the given color.
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              const rmp_u8_t* Matrix - The data matrix.
              rmp_cnt_t Length - The length of the picture.
              rmp_cnt_t Width - The width of the picture.
              rmp_ptr_t Color - The color to display this with.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Matrix(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, const rmp_u8_t* Matrix,
                rmp_cnt_t Bit_Order, rmp_cnt_t Length, rmp_cnt_t Width, rmp_ptr_t Color)
{
    rmp_cnt_t Len_Cnt;
    rmp_cnt_t Wid_Cnt;
    rmp_cnt_t Mat_Pos;
    
    if((Matrix==0)||((Length&0x07)!=0)||(Length==0)||(Width==0))
        return;
    
    Mat_Pos=0;
    /* Natural order */
    if(Bit_Order==RMP_MAT_BIG)
    {
        for(Wid_Cnt=Coord_Y;Wid_Cnt<Width+Coord_Y;Wid_Cnt++)
        {
            for(Len_Cnt=Coord_X;Len_Cnt<Length+Coord_X;Len_Cnt++)
            {
                if(RMP_MAT_BPOS(Matrix,Mat_Pos)!=0)
                    RMP_Point(Len_Cnt,Wid_Cnt,Color);
                Mat_Pos++;
            }
        }
    }
    /* Small order */
    else
    {
        for(Wid_Cnt=Coord_Y;Wid_Cnt<Width+Coord_Y;Wid_Cnt++)
        {
            for(Len_Cnt=Coord_X;Len_Cnt<Length+Coord_X;Len_Cnt++)
            {
                if(RMP_MAT_SPOS(Matrix,Mat_Pos)!=0)
                    RMP_Point(Len_Cnt,Wid_Cnt,Color);
                Mat_Pos++;
            }
        }
    }
}
/* End Function:RMP_Matrix ***************************************************/

/* Begin Function:RMP_Matrix_AA ***********************************************
Description : Display a monochrome bit map in the given color, with simple 
              anti-aliasing (FXAA algorithm).
              When using this, three color mixing macros must be provided.
           
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              const rmp_u8_t* Matrix - The data matrix.
              rmp_cnt_t Length - The length of the picture.
              rmp_cnt_t Width - The width of the picture.
              rmp_ptr_t Color - The color to display this with.
              rmp_ptr_t Back - The background color, for anti-aliasing.
Output      : None.
Return      : None.
******************************************************************************/
#ifdef RMP_COLOR_25P
#ifdef RMP_COLOR_50P
#ifdef RMP_COLOR_75P
void RMP_Matrix_AA(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, const rmp_u8_t* Matrix,
                   rmp_cnt_t Bit_Order, rmp_cnt_t Length, rmp_cnt_t Width, rmp_ptr_t Color, rmp_ptr_t Back)
{
    rmp_cnt_t Len_Cnt;
    rmp_cnt_t Wid_Cnt;
    rmp_cnt_t Mat_Pos;
    rmp_cnt_t Anti_Alias;
    rmp_ptr_t Color_25;
    rmp_ptr_t Color_50;
    rmp_ptr_t Color_75;
    rmp_ptr_t Total;
    
    if((Matrix==0)||((Length&0x07)!=0)||(Length==0)||(Width==0))
        return;
    
    Mat_Pos=0;
    Total=Length*Width;
    Color_25=RMP_COLOR_25P(Color,Back);
    Color_50=RMP_COLOR_50P(Color,Back);
    Color_75=RMP_COLOR_75P(Color,Back);
    
    /* Natural order */
    if(Bit_Order==RMP_MAT_BIG)
    {
        for(Wid_Cnt=Coord_Y;Wid_Cnt<Width+Coord_Y;Wid_Cnt++)
        {
            for(Len_Cnt=Coord_X;Len_Cnt<Length+Coord_X;Len_Cnt++)
            {
                if(RMP_MAT_BPOS(Matrix,Mat_Pos)!=0)
                    RMP_Point(Len_Cnt,Wid_Cnt,Color);
                else
                {
                    /* Anti-aliasing */
                    Anti_Alias=0;
                    
                    if(Mat_Pos-1>=0)
                    {
                        if((Len_Cnt!=Coord_X)&&(RMP_MAT_BPOS(Matrix,Mat_Pos-1)!=0))
                            Anti_Alias++;
                        if((Mat_Pos-Length>=0)&&(RMP_MAT_BPOS(Matrix,Mat_Pos-Length)!=0))
                            Anti_Alias++;
                    }
                    
                    if(Mat_Pos+1<Total)
                    {
                        if((Len_Cnt!=Coord_X+Length-1)&&(RMP_MAT_BPOS(Matrix,Mat_Pos+1)!=0))
                            Anti_Alias++;
                        if((Mat_Pos+Length<Total)&&(RMP_MAT_BPOS(Matrix,Mat_Pos+Length)!=0))
                            Anti_Alias++;
                    }
                    
                    switch(Anti_Alias)
                    {
                        case 1:RMP_Point(Len_Cnt,Wid_Cnt,Color_25);break;
                        case 2:RMP_Point(Len_Cnt,Wid_Cnt,Color_50);break;
                        case 3:RMP_Point(Len_Cnt,Wid_Cnt,Color_75);break;
                        default:break;
                    }
                }
                
                Mat_Pos++;
            }
        }
    }
    /* Small order */
    else
    {
        for(Wid_Cnt=Coord_Y;Wid_Cnt<Width+Coord_Y;Wid_Cnt++)
        {
            for(Len_Cnt=Coord_X;Len_Cnt<Length+Coord_X;Len_Cnt++)
            {
                if(RMP_MAT_SPOS(Matrix,Mat_Pos)!=0)
                    RMP_Point(Len_Cnt,Wid_Cnt,Color);
                else
                {
                    /* Anti-aliasing */
                    Anti_Alias=0;
                    
                    if(Mat_Pos-1>=0)
                    {
                        if((Len_Cnt!=Coord_X)&&(RMP_MAT_SPOS(Matrix,Mat_Pos-1)!=0))
                            Anti_Alias++;
                        if((Mat_Pos-Length>=0)&&(RMP_MAT_SPOS(Matrix,Mat_Pos-Length)!=0))
                            Anti_Alias++;
                    }
                    
                    if(Mat_Pos+1<Total)
                    {
                        if((Len_Cnt!=Coord_X+Length-1)&&(RMP_MAT_SPOS(Matrix,Mat_Pos+1)!=0))
                            Anti_Alias++;
                        if((Mat_Pos+Length<Total)&&(RMP_MAT_SPOS(Matrix,Mat_Pos+Length)!=0))
                            Anti_Alias++;
                    }
                    
                    switch(Anti_Alias)
                    {
                        case 1:RMP_Point(Len_Cnt,Wid_Cnt,Color_25);break;
                        case 2:RMP_Point(Len_Cnt,Wid_Cnt,Color_50);break;
                        case 3:RMP_Point(Len_Cnt,Wid_Cnt,Color_75);break;
                        default:break;
                    }
                }
                
                Mat_Pos++;
            }
        }
    }
}
#endif
#endif
#endif
#endif
/* End Function:RMP_Matrix_AA ************************************************/

/* Begin Function:RMP_Cursor **************************************************
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
#ifdef RMP_POINT
#ifdef RMP_CTL_WHITE
#ifdef RMP_CTL_LGREY
#ifdef RMP_CTL_GREY
#ifdef RMP_CTL_DGREY
#ifdef RMP_CTL_DARK
#ifdef RMP_CTL_DDARK
#ifdef RMP_CTL_BLACK
void RMP_Cursor(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_ptr_t Style)
{
    rmp_cnt_t Count;
    rmp_cnt_t Size_B;
    rmp_cnt_t Size_W;
    const rmp_u8_t* Black;
    const rmp_u8_t* White;

    /* Normal Mouse - all matrix encoded in (Y,X) */
    static const rmp_u8_t Arrow_B[]=
    {
        0x00,0x10,0x11,0x20,0x22,0x30,0x33,0x40,0x44,0x50,0x55,0x60,0x66,0x70,0x77,0x80,
        0x88,0x90,0x95,0x96,0x97,0x98,0x99,0xA0,0xA2,0xA3,0xA5,0xB0,0xB1,0xB3,0xB6,0xC0,
        0xC1,0xC4,0xC6,0xD0,0xD4,0xD7,0xE5,0xE7,0xF5,0xF6
    };
    static const rmp_u8_t Arrow_W[]=
    {
        0x21,0x31,0x32,0x41,0x42,0x43,0x51,0x52,0x53,0x54,0x61,0x62,0x63,0x64,0x65,0x71,
        0x72,0x73,0x74,0x75,0x76,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x91,0x92,0x93,0x94,
        0xA1,0xA4,0xB4,0xB5,0xC5,0xD5,0xD6,0xE6
    };
    /* Busy mark - no white part */
    static const rmp_u8_t Busy_B[]=
    {
        0x0A,0x0B,0x0C,0x0D,0x0E,0x1A,0x1B,0x1D,0x1E,0x2A,0x2C,0x2E,0x3B,0x3D,0x4C,0x5B,
        0x5D,0x6A,0x6E,0x7A,0x7C,0x7E,0x8A,0x8B,0x8C,0x8D,0x8E
    };
    static const rmp_u8_t Busy_W[]=
    {
        0x1C,0x2B,0x2D,0x3C,0x5C,0x6B,0x6C,0x6D,0x7B,0x7D
    };
    /* Question mark - no white part */
    static const rmp_u8_t Question[]=
    {
        0x0B,0x0C,0x0D,0x1A,0x1B,0x1C,0x1D,0x1E,0x29,0x2A,0x2B,0x2D,0x2E,0x2F,0x39,0x3A,
        0x3E,0x3F,0x4A,0x4D,0x4E,0x4F,0x5C,0x5D,0x5E,0x6C,0x6D,0x8D,0x8E,0x9D,0x9E
    };
    /* Hand-shaped cursor */
    static const rmp_u8_t Hand_B[]=
    {
        0x05,0x06,0x14,0x17,0x24,0x27,0x34,0x37,0x39,0x44,0x47,0x48,0x4A,0x4C,0x51,0x52,
        0x54,0x57,0x5A,0x5B,0x5D,0x5E,0x60,0x63,0x64,0x67,0x6A,0x6D,0x6F,0x70,0x74,0x77,
        0x7A,0x7D,0x7F,0x80,0x8F,0x90,0x9F,0xA0,0xAF,0xB0,0xBF,0xC1,0xCE,0xD1,0xD2,0xD3,
        0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xE1,0xEE,0xF2,0xF3,0xF4,
        0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD
    };
    static const rmp_u8_t Hand_W[]=
    {
        0x15,0x16,0x25,0x26,0x35,0x36,0x45,0x46,0x49,0x55,0x56,0x58,0x59,0x5C,0x61,0x62,
        0x65,0x66,0x68,0x69,0x6B,0x6C,0x6E,0x71,0x72,0x73,0x75,0x76,0x78,0x79,0x7B,0x7C,
        0x7E,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x91,
        0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0xA1,0xA2,0xA3,
        0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xB1,0xB2,0xB3,0xB4,0xB5,
        0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,
        0xC9,0xCA,0xCB,0xCC,0xCD,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,
        0xED
    };
    /* The cursor for text editing - no white part */
    static const rmp_u8_t Text[]=
    {
        0x00,0x01,0x02,0x04,0x05,0x06,0x13,0x23,0x33,0x43,0x53,0x63,0x73,0x83,0x93,0xA3,
        0xB3,0xC3,0xD3,0xE3,0xF0,0xF1,0xF2,0xF4,0xF5,0xF6
    };
    /* The stop cursor */
    static const rmp_u8_t Stop_B[]=
    {
        0x05,0x06,0x07,0x08,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x22,0x23,0x24,0x29,
        0x2A,0x2B,0x31,0x32,0x33,0x34,0x3A,0x3B,0x3C,0x41,0x42,0x43,0x44,0x45,0x4B,0x4C,
        0x50,0x51,0x54,0x55,0x56,0x5C,0x5D,0x60,0x61,0x65,0x66,0x67,0x6C,0x6D,0x70,0x71,
        0x76,0x77,0x78,0x7C,0x7D,0x80,0x81,0x87,0x88,0x89,0x8C,0x8D,0x91,0x92,0x98,0x99,
        0x9A,0x9B,0x9C,0xA1,0xA2,0xA3,0xA9,0xAA,0xAB,0xAC,0xB2,0xB3,0xB4,0xB9,0xBA,0xBB,
        0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD5,0xD6,0xD7,0xD8
    };
    static const rmp_u8_t Stop_W[]=
    {
        0x25,0x26,0x27,0x28,0x35,0x36,0x37,0x38,0x39,0x46,0x47,0x48,0x49,0x4A,0x52,0x53,
        0x57,0x58,0x59,0x5A,0x5B,0x62,0x63,0x64,0x68,0x69,0x6A,0x6B,0x72,0x73,0x74,0x75,
        0x79,0x7A,0x7B,0x82,0x83,0x84,0x85,0x86,0x8A,0x8B,0x93,0x94,0x95,0x96,0x97,0xA4,
        0xA5,0xA6,0xA7,0xA8,0xB5,0xB6,0xB7,0xB8
    };
       /* Cross arrow - no white part  */
    static const rmp_u8_t Adj_ALL[]=
    {
        0x07,0x16,0x17,0x18,0x25,0x26,0x27,0x28,0x29,0x37,0x47,0x52,0x57,0x5C,0x61,0x62,
        0x67,0x6C,0x6D,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,
        0x7D,0x7E,0x81,0x82,0x87,0x8C,0x8D,0x92,0x97,0x9C,0xA7,0xB7,0xC5,0xC6,0xC7,0xC8,
        0xC9,0xD6,0xD7,0xD8,0xE7
    };    
    /* Left to right arrow - no white part */
    static const rmp_u8_t Adj_LR[]=
    {
        0x52,0x5D,0x61,0x62,0x6D,0x6E,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,
        0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,0x81,0x82,0x8D,0x8E,0x92,0x9D
    };
    /* Up to down arrow - no white part */
    static const rmp_u8_t Adj_UD[]=
    {
        0x07,0x16,0x17,0x18,0x25,0x26,0x27,0x28,0x29,0x37,0x47,0x57,0x67,0x77,0x87,0x97,
        0xA7,0xB7,0xC7,0xD5,0xD6,0xD7,0xD8,0xD9,0xE6,0xE7,0xE8,0xF7

    };
    /* Bottom-right to top-left arrow - no white part */
    static const rmp_u8_t Adj_BRUL[]=
    {
        0x00,0x01,0x02,0x03,0x04,0x10,0x11,0x12,0x13,0x20,0x21,0x22,0x30,0x31,0x33,0x40,
        0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xBF,0xCC,0xCE,0xCF,0xDD,0xDE,0xDF,0xEC,
        0xED,0xEE,0xEF,0xFB,0xFC,0xFD,0xFE,0xFF
    };
    /* Bottom-left to top-right arrow - no white part */
    static const rmp_u8_t Adj_BLUR[]=
    {
        0x0B,0x0C,0x0D,0x0E,0x0F,0x1C,0x1D,0x1E,0x1F,0x2D,0x2E,0x2F,0x3C,0x3E,0x3F,0x4B,
        0x4F,0x5A,0x69,0x78,0x87,0x96,0xA5,0xB0,0xB4,0xC0,0xC1,0xC3,0xD0,0xD1,0xD2,0xE0,
        0xE1,0xE2,0xE3,0xF0,0xF1,0xF2,0xF3,0xF4
    };
    /* The crosshair cursor - no white part */
    static const rmp_u8_t Cross[]=
    {
        0x07,0x17,0x27,0x37,0x46,0x47,0x48,0x55,0x57,0x59,0x64,0x6A,0x70,0x71,0x72,0x73,
        0x74,0x75,0x77,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x84,0x8A,0x95,0x97,0x99,0xA6,0xA7,
        0xA8,0xB7,0xC7,0xD7,0xE7
    };
    
    /* Draw them */
    switch(Style)
    {
        case RMP_CUR_NORM:{Size_B=sizeof(Arrow_B);Size_W=sizeof(Arrow_W);Black=Arrow_B;White=Arrow_W;break;}
        case RMP_CUR_BUSY:case RMP_CUR_QUESTION:
        {
            for(Count=0;Count<sizeof(Arrow_B);Count++)                                                              
                RMP_POINT(Coord_X+(Arrow_B[Count]&0x0F),Coord_Y+(Arrow_B[Count]>>4),RMP_CTL_BLACK);
            for(Count=0;Count<sizeof(Arrow_W);Count++)                                                              
                RMP_POINT(Coord_X+(Arrow_W[Count]&0x0F),Coord_Y+(Arrow_W[Count]>>4),RMP_CTL_WHITE);
            
            if(Style==RMP_CUR_BUSY)
            {
                Size_B=sizeof(Busy_B);
                Size_W=sizeof(Busy_W);
                Black=Busy_B;
                White=Busy_W;
            }
            else
            {
                Size_B=sizeof(Question);
                Size_W=0;
                Black=Question;
                White=0;
            }
            break;
        }       
        case RMP_CUR_HAND:{Size_B=sizeof(Hand_B);Size_W=sizeof(Hand_W);Black=Hand_B;White=Hand_W;break;}
        case RMP_CUR_TEXT:{Size_B=sizeof(Text);Size_W=0;Black=Text;White=0;break;}
        case RMP_CUR_STOP:{Size_B=sizeof(Stop_B);Size_W=sizeof(Stop_W);Black=Stop_B;White=Stop_W;break;}
        case RMP_CUR_MOVE:{Size_B=sizeof(Adj_ALL);Size_W=0;Black=Adj_ALL;White=0;break;}
        case RMP_CUR_LR:{Size_B=sizeof(Adj_LR);Size_W=0;Black=Adj_LR;White=0;break;}
        case RMP_CUR_UD:{Size_B=sizeof(Adj_UD);Size_W=0;Black=Adj_UD;White=0;break;}
        case RMP_CUR_ULBR:{Size_B=sizeof(Adj_BRUL);Size_W=0;Black=Adj_BRUL;White=0;break;}
        case RMP_CUR_URBL:{Size_B=sizeof(Adj_BLUR);Size_W=0;Black=Adj_BLUR;White=0;break;}
        case RMP_CUR_CROSS:{Size_B=sizeof(Cross);Size_W=0;Black=Cross;White=0;break;}
        default:return;
    }
    
    for(Count=0;Count<Size_B;Count++)                                                              
        RMP_POINT(Coord_X+(Black[Count]&0x0F),Coord_Y+(Black[Count]>>4),RMP_CTL_BLACK);
    for(Count=0;Count<Size_W;Count++)                                                              
        RMP_POINT(Coord_X+(White[Count]&0x0F),Coord_Y+(White[Count]>>4),RMP_CTL_WHITE);
}
/* End Function: RMP_Cursor **************************************************/

/* Begin Function:RMP_Checkbox_Set ********************************************
Description : Set the checbox (tick it).
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the checkbox(also the width).
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Checkbox_Set(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length)
{
    rmp_cnt_t Count;
    
    /* Draw a tick inside */
    for(Count=0;Count<=2*Length/13;Count++)
    {
        RMP_Line(Coord_X+4*Length/13+Count,Coord_Y+6*Length/13+Count,
                 Coord_X+4*Length/13+Count,Coord_Y+8*Length/13+Count,RMP_CTL_BLACK);
    }
    for(Count=0;Count<=4*Length/13;Count++)
    {
        RMP_Line(Coord_X+6*Length/13+Count,Coord_Y+8*Length/13-Count,
                 Coord_X+6*Length/13+Count,Coord_Y+10*Length/13-Count,RMP_CTL_BLACK);
    }
}
/* End Function:RMP_Checkbox_Set *********************************************/

/* Begin Function:RMP_Checkbox_Clr ********************************************
Description : Clear the checbox (reset it).
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the checkbox(also the width).
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Checkbox_Clr(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length)
{
    rmp_cnt_t Count;
    
    /* Erase the tick inside */
    for(Count=0;Count<=2*Length/13;Count++)
    {
        RMP_Line(Coord_X+4*Length/13+Count,Coord_Y+6*Length/13+Count,
                 Coord_X+4*Length/13+Count,Coord_Y+8*Length/13+Count,RMP_CTL_WHITE);
    }
    for(Count=0;Count<=4*Length/13;Count++)
    {
        RMP_Line(Coord_X+6*Length/13+Count,Coord_Y+8*Length/13-Count,
                 Coord_X+6*Length/13+Count,Coord_Y+10*Length/13-Count,RMP_CTL_WHITE);
    }
}
/* End Function:RMP_Checkbox_Clr *********************************************/

/* Begin Function:RMP_Checkbox ************************************************
Description : Draw the checkbox according to the parameters.
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the checkbox(also the width).
              rmp_ptr_t Status - The checkbox status.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Checkbox(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_ptr_t Status)    
{   
    /* Clear the area */
    RMP_Rectangle(Coord_X,Coord_Y,Length,Length,RMP_CTL_WHITE,RMP_CTL_WHITE);
                                                                     
    /* Draw checkbox outline */
    RMP_Line(Coord_X,Coord_Y,Coord_X+Length-1,Coord_Y,RMP_CTL_DARK);                 
    RMP_Line(Coord_X+Length-1,Coord_Y,Coord_X+Length-1,Coord_Y+Length-1,RMP_CTL_WHITE);
    RMP_Line(Coord_X+Length-1,Coord_Y+Length-1,Coord_X,Coord_Y+Length-1,RMP_CTL_WHITE);
    RMP_Line(Coord_X,Coord_Y+Length-1,Coord_X,Coord_Y,RMP_CTL_DARK);

    RMP_Line(Coord_X+1,Coord_Y+1,Coord_X+Length-2,Coord_Y+1,RMP_CTL_BLACK);
    RMP_Line(Coord_X+Length-2,Coord_Y+1,Coord_X+Length-2,Coord_Y+Length-2,RMP_CTL_DARK);
    RMP_Line(Coord_X+Length-2,Coord_Y+Length-2,Coord_X,Coord_Y+Length-2,RMP_CTL_DARK);
    RMP_Line(Coord_X+1,Coord_Y+Length-2,Coord_X+1,Coord_Y+1,RMP_CTL_BLACK);
    
    if(Status!=0)
        RMP_Checkbox_Set(Coord_X, Coord_Y, Length);
}
/* End Function:RMP_Checkbox *************************************************/

/* Begin Function:RMP_Cmdbtn_Down *********************************************
Description : Turn the command button to a pushed-down version.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the button.
              rmp_cnt_t Width - The width of the button.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Cmdbtn_Down(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_cnt_t Width)
{
    RMP_Line(Coord_X,Coord_Y,Coord_X+Length-1,Coord_Y,RMP_CTL_BLACK);
    RMP_Line(Coord_X+Length-1,Coord_Y,Coord_X+Length-1,Coord_Y+Width-1,RMP_CTL_WHITE);
    RMP_Line(Coord_X+Length-1,Coord_Y+Width-1,Coord_X,Coord_Y+Width-1,RMP_CTL_WHITE);
    RMP_Line(Coord_X,Coord_Y+Width-1,Coord_X,Coord_Y,RMP_CTL_BLACK);
    /* Clear the old shadow */
    RMP_Line(Coord_X+Length-2,Coord_Y+Width-2,Coord_X+1,Coord_Y+Width-2,RMP_CTL_GREY);    
    RMP_Line(Coord_X+Length-2,Coord_Y+1,Coord_X+Length-2,Coord_Y+Width-2,RMP_CTL_GREY);    
    /* The shadow */                            
    RMP_Line(Coord_X+1,Coord_Y+Width-2,Coord_X+1,Coord_Y+1,RMP_CTL_DARK);    
    RMP_Line(Coord_X+1,Coord_Y+1,Coord_X+Length-2,Coord_Y+1,RMP_CTL_DARK);    
}
/* End Function:RMP_Cmdbtn_Down **********************************************/

/* Begin Function:RMP_Cmdbtn_Up ***********************************************
Description : Turn the command button to a popped-up version.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the button.
              rmp_cnt_t Width - The width of the button.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Cmdbtn_Up(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_cnt_t Width)
{
    RMP_Line(Coord_X,Coord_Y,Coord_X+Length-1,Coord_Y,RMP_CTL_WHITE);
    RMP_Line(Coord_X+Length-1,Coord_Y,Coord_X+Length-1,Coord_Y+Width-1,RMP_CTL_BLACK);
    RMP_Line(Coord_X+Length-1,Coord_Y+Width-1,Coord_X,Coord_Y+Width-1,RMP_CTL_BLACK);
    RMP_Line(Coord_X,Coord_Y+Width-1,Coord_X,Coord_Y,RMP_CTL_WHITE);
    /* Clear the old shadow */
    RMP_Line(Coord_X+1,Coord_Y+Width-2,Coord_X+1,Coord_Y+1,RMP_CTL_GREY);    
    RMP_Line(Coord_X+1,Coord_Y+1,Coord_X+Length-2,Coord_Y+1,RMP_CTL_GREY);    
    /* The shadow */
    RMP_Line(Coord_X+Length-2,Coord_Y+Width-2,Coord_X+1,Coord_Y+Width-2,RMP_CTL_DARK);    
    RMP_Line(Coord_X+Length-2,Coord_Y+1,Coord_X+Length-2,Coord_Y+Width-2,RMP_CTL_DARK);    
}
/* End Function:RMP_Cmdbtn_Up ************************************************/

/* Begin Function:RMP_Cmdbtn **************************************************
Description : Draw a command button.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the button.
              rmp_cnt_t Width - The width of the button.
              rmp_ptr_t Status - The status of the command button.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Cmdbtn(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_cnt_t Width, rmp_ptr_t Status)
{
    RMP_Rectangle(Coord_X,Coord_Y,Length,Width,RMP_CTL_GREY,RMP_CTL_GREY);

    if(Status!=0)
        RMP_Cmdbtn_Down(Coord_X, Coord_Y, Length, Width);
    else
        RMP_Cmdbtn_Up(Coord_X, Coord_Y, Length, Width);
}
/* End Function:RMP_Cmdbtn ***************************************************/

/* Begin Function:RMP_Lineedit_Clr ********************************************
Description : Clear a portion of the line edit box.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the line edit box.
              rmp_cnt_t Width - The width of the line edit box.
              rmp_cnt_t Clr_X - The X coordinate to start clearing.
              rmp_cnt_t Clr_Len - The length to clear.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Lineedit_Clr(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length,
                      rmp_cnt_t Width, rmp_cnt_t Clr_X, rmp_cnt_t Clr_Len)
{
    RMP_Rectangle(Clr_X,Coord_Y+1,Clr_Len,Width-2,RMP_CTL_WHITE,RMP_CTL_WHITE);
}
/* End Function:RMP_Lineedit_Clr *********************************************/

/* Begin Function:RMP_Lineedit ************************************************
Description : Draw a line edit box with string on it.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the line edit box.
              rmp_cnt_t Width - The width of the line edit box.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Lineedit(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_cnt_t Width)
{
    RMP_Rectangle(Coord_X,Coord_Y,Length,Width,RMP_CTL_WHITE,RMP_CTL_WHITE);
    
    /* Now draw the border */
    RMP_Line(Coord_X,Coord_Y,Coord_X+Length-1,Coord_Y,RMP_CTL_DARK);
    RMP_Line(Coord_X+Length-1,Coord_Y,Coord_X+Length-1,Coord_Y+Width-1,RMP_CTL_WHITE);
    RMP_Line(Coord_X+Length-1,Coord_Y+Width-1,Coord_X,Coord_Y+Width-1,RMP_CTL_WHITE);
    RMP_Line(Coord_X,Coord_Y+Width-1,Coord_X,Coord_Y,RMP_CTL_DARK);
    /* The shadow */
    RMP_Line(Coord_X+1,Coord_Y+Width-2,Coord_X+1,Coord_Y+1,RMP_CTL_BLACK);    
    RMP_Line(Coord_X+1,Coord_Y+1,Coord_X+Length-2,Coord_Y+1,RMP_CTL_BLACK);
}
/* End Function:RMP_Lineedit *************************************************/

/* Begin Function:RMP_Radiobtn_Circle *****************************************
Description : Draw the radio button outside circle according to the parameters.
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the radio button(also the width).         
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Radiobtn_Circle(rmp_cnt_t Coord_X,rmp_cnt_t Coord_Y,rmp_cnt_t Length)
{   
    rmp_cnt_t Radius;
    rmp_cnt_t Center_X;
    rmp_cnt_t Center_Y;
    rmp_cnt_t Cur_X;
    rmp_cnt_t Cur_Y;
    rmp_cnt_t Error;
    
    /* The radius is the length/2 */
    Radius=Length>>1;
    Center_X=Coord_X+Radius;
    Center_Y=Coord_Y+Radius;
    
    /* Now we begin to draw the inner halfcircle */
    Cur_X=0;
    Cur_Y=Radius-1;
    Error=3-((Radius-1)<<1);  
    
    while(Cur_X<=Cur_Y) 
    {  
        /* This is the upper-left part */
        RMP_POINT(Center_X+Cur_X,Center_Y-Cur_Y,RMP_CTL_DDARK);
        RMP_POINT(Center_X-Cur_X,Center_Y-Cur_Y,RMP_CTL_DDARK);
        RMP_POINT(Center_X-Cur_Y,Center_Y-Cur_X,RMP_CTL_DDARK);
        RMP_POINT(Center_X-Cur_Y,Center_Y+Cur_X,RMP_CTL_DDARK);
        
        /* This is the lower-right part */
        RMP_POINT(Center_X+Cur_X,Center_Y+Cur_Y,RMP_CTL_LGREY);
        RMP_POINT(Center_X-Cur_X,Center_Y+Cur_Y,RMP_CTL_LGREY);  
        RMP_POINT(Center_X+Cur_Y,Center_Y+Cur_X,RMP_CTL_LGREY); 
        RMP_POINT(Center_X+Cur_Y,Center_Y-Cur_X,RMP_CTL_LGREY);  
        
        if(Error<0) 
        {
            Error=Error+(Cur_X<<2)+6;
        }
        else 
        {
            Error=Error+((Cur_X-Cur_Y)<<2);
            Cur_Y--;
        }
        Cur_X++;
    } 
    
    /* Now we begin to draw the outer halfcircle */
    Cur_X=0;
    Cur_Y=Radius;
    Error=3-(Radius<<1);  
    
    while(Cur_X<=Cur_Y) 
    {  
        /* This is the upper-left part */
        RMP_POINT(Center_X+Cur_X,Center_Y-Cur_Y,RMP_CTL_DGREY);
        RMP_POINT(Center_X-Cur_X,Center_Y-Cur_Y,RMP_CTL_DGREY);
        RMP_POINT(Center_X-Cur_Y,Center_Y-Cur_X,RMP_CTL_DGREY);
        RMP_POINT(Center_X-Cur_Y,Center_Y+Cur_X,RMP_CTL_DGREY);
        
        /* This is the lower-right part */
        RMP_POINT(Center_X+Cur_X,Center_Y+Cur_Y,RMP_CTL_WHITE);
        RMP_POINT(Center_X-Cur_X,Center_Y+Cur_Y,RMP_CTL_WHITE);  
        RMP_POINT(Center_X+Cur_Y,Center_Y+Cur_X,RMP_CTL_WHITE); 
        RMP_POINT(Center_X+Cur_Y,Center_Y-Cur_X,RMP_CTL_WHITE);  
        
        if(Error<0) 
        {  
            Error=Error+(Cur_X<<2)+6;  
        } 
        else 
        {  
            Error=Error+((Cur_X-Cur_Y)<<2);  
            Cur_Y--;  
        }  
        Cur_X++;  
    } 
}
/* End Function:RMP_Radiobtn_Circle ******************************************/

/* Begin Function:RMP_Radiobtn_Set ********************************************
Description : Set the radio button (select it).
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the radio button(also the width).
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Radiobtn_Set(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length)
{
    RMP_Circle(Coord_X+Length/2,Coord_Y+Length/2,Length/6,RMP_CTL_BLACK,RMP_CTL_BLACK);
}
/* End Function:RMP_Radiobtn_Set *********************************************/

/* Begin Function:RMP_Radiobtn_Clr ********************************************
Description : Set the radio button (reset it).
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the radio button(also the width).
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Radiobtn_Clr(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length)
{
    RMP_Circle(Coord_X+Length/2,Coord_Y+Length/2,Length/6,RMP_CTL_GREY,RMP_CTL_GREY);
}
/* End Function:RMP_Radiobtn_Clr *********************************************/

/* Begin Function:RMP_Radiobtn ************************************************
Description : Draw the radio button according to the parameters.
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the radio button(also the width).
              rmp_ptr_t Status - The radio button status.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Radiobtn(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_ptr_t Status)
{   
    /* Fill the area with rect */
    RMP_Rectangle(Coord_X,Coord_Y,Length,Length,RMP_CTL_GREY,RMP_CTL_GREY);
    
    /* Draw the circle first */
    RMP_Radiobtn_Circle(Coord_X,Coord_Y,Length);
    
    /* See if the radio button is selected */
    if(Status==RMP_RBTN_SEL)
        RMP_Radiobtn_Set(Coord_X, Coord_Y, Length);
}
/* End Function:RMP_Radiobtn *************************************************/

/* Begin Function:RMP_Progbar_Set ********************************************
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
void RMP_Progbar_Set(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_cnt_t Width,
                     rmp_cnt_t Style, rmp_cnt_t Old_Prog, rmp_cnt_t New_Prog, rmp_ptr_t Fore, rmp_ptr_t Back)
{
    /* The critical values for drawing the bar */
    rmp_ptr_t Old_Pivot;
    rmp_ptr_t New_Pivot;
    
    /* If things does not change, return to save time */
    if(Old_Prog==New_Prog)
        return;
    
    switch(Style)
    {
        case RMP_PBAR_L2R:
        {
            Old_Pivot=(Length-2)*Old_Prog/100;
            New_Pivot=(Length-2)*New_Prog/100;
            /* Progress decreased */
            if(Old_Pivot>New_Pivot)
                RMP_Rectangle(Coord_X+1+New_Pivot,Coord_Y+1,Old_Pivot-New_Pivot,Width-2,Back,Back);
            /* Progress increased */
            else
                RMP_Rectangle(Coord_X+1+Old_Pivot,Coord_Y+1,New_Pivot-Old_Pivot,Width-2,Fore,Fore);
            break;
        }
        case RMP_PBAR_D2U:
        {
            Old_Pivot=(Width-2)*Old_Prog/100;
            New_Pivot=(Width-2)*New_Prog/100;
            /* Progress decreased */
            if(Old_Pivot>New_Pivot)
                RMP_Rectangle(Coord_X+1,Coord_Y-1+Width-Old_Pivot,Length-2,Old_Pivot-New_Pivot,Back,Back);
            /* Progress increased */
            else
                RMP_Rectangle(Coord_X+1,Coord_Y-1+Width-New_Pivot,Length-2,New_Pivot-Old_Pivot,Fore,Fore);
            break;
        }
        case RMP_PBAR_R2L:
        {
            Old_Pivot=(Length-2)*Old_Prog/100;
            New_Pivot=(Length-2)*New_Prog/100;
            /* Progress decreased */
            if(Old_Pivot>New_Pivot)
                RMP_Rectangle(Coord_X-1+Length-Old_Pivot,Coord_Y+1,Old_Pivot-New_Pivot,Width-2,Back,Back);
            /* Progress increased */
            else
                RMP_Rectangle(Coord_X-1+Length-New_Pivot,Coord_Y+1,New_Pivot-Old_Pivot,Width-2,Fore,Fore);
            break;
        }
        case RMP_PBAR_U2D:
        {
            Old_Pivot=(Width-2)*Old_Prog/100;
            New_Pivot=(Width-2)*New_Prog/100;
            /* Progress decreased */
            if(Old_Pivot>New_Pivot)
                RMP_Rectangle(Coord_X+1,Coord_Y+1+New_Pivot,Length-2,Old_Pivot-New_Pivot,Back,Back);
            /* Progress increased */
            else
                RMP_Rectangle(Coord_X+1,Coord_Y+1+Old_Pivot,Length-2,New_Pivot-Old_Pivot,Fore,Fore);
            break;
        }
        default: break;
    }
}
/* End Function:RMP_Progbar_Set **********************************************/

/* Begin Function:RMP_Progbar_Prog ********************************************
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
void RMP_Progbar_Prog(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_cnt_t Width,
                      rmp_cnt_t Style, rmp_cnt_t Prog, rmp_ptr_t Fore, rmp_ptr_t Back)
{
    /* The critical value for drawing the bar */
    rmp_ptr_t Pivot;
    
    switch(Style)
    {
        case RMP_PBAR_L2R:
        {
            Pivot=Length*Prog/100;
            if(Prog!=0)
                RMP_Rectangle(Coord_X,Coord_Y,Pivot,Width,Fore,Fore);
            if(Prog!=100)
                RMP_Rectangle(Coord_X+Pivot,Coord_Y,Length-Pivot,Width,Back,Back);
            break;
        }
        case RMP_PBAR_D2U:
        {
            Pivot=Width*Prog/100;
            if(Prog!=0)
                RMP_Rectangle(Coord_X,Coord_Y+Width-Pivot,Length,Pivot,Fore,Fore);
            if(Prog!=100)
                RMP_Rectangle(Coord_X,Coord_Y,Length,Width-Pivot,Back,Back);
            break;
        }
        case RMP_PBAR_R2L:
        {
            Pivot=Length*Prog/100;
            if(Prog!=0)
                RMP_Rectangle(Coord_X+Length-Pivot,Coord_Y,Pivot,Width,Fore,Fore);
            if(Prog!=100)
                RMP_Rectangle(Coord_X,Coord_Y,Length-Pivot,Width,Back,Back);
            break;
        }
        case RMP_PBAR_U2D:
        {
            Pivot=Width*Prog/100;
            if(Prog!=0)
                RMP_Rectangle(Coord_X,Coord_Y,Length,Pivot,Fore,Fore);
            if(Prog!=100)
                RMP_Rectangle(Coord_X,Coord_Y+Pivot,Length,Width-Pivot,Back,Back);
            break;
        }
        default: break;
    }
}
/* End Function:RMP_Progbar_Prog *********************************************/

/* Begin Function:RMP_Progbar *************************************************
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
void RMP_Progbar(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_cnt_t Width,
                 rmp_cnt_t Style, rmp_cnt_t Prog, rmp_ptr_t Fore, rmp_ptr_t Back)
{
    /* Draw the progress bar according to the style of the progress bar */
    RMP_Progbar_Prog(Coord_X,Coord_Y,Length,Width,Style,Prog,Fore,Back);
    /* Now draw the border */
    RMP_Line(Coord_X,Coord_Y,Coord_X+Length-1,Coord_Y,RMP_CTL_BLACK);
    RMP_Line(Coord_X+Length-1,Coord_Y,Coord_X+Length-1,Coord_Y+Width-1,RMP_CTL_BLACK);
    RMP_Line(Coord_X+Length-1,Coord_Y+Width-1,Coord_X,Coord_Y+Width-1,RMP_CTL_BLACK);
    RMP_Line(Coord_X,Coord_Y+Width-1,Coord_X,Coord_Y,RMP_CTL_BLACK);
}
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
/* End Function:RMP_Progbar **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
