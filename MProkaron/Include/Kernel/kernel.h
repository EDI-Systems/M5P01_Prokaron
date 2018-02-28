/******************************************************************************
Filename    : kernel.h
Author      : pry
Date        : 01/04/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The header file for the kernel.
******************************************************************************/

/* Defines *******************************************************************/
#ifdef __HDR_DEFS__
#ifndef __KERNEL_H_DEFS__
#define __KERNEL_H_DEFS__
/*****************************************************************************/
/* Constants */
#define RMP_TRUE               1
#define RMP_FALSE              0
/* States of threads */
#define RMP_THD_STATE(X)       ((X)&0xFF)
#define RMP_THD_FLAG(X)        ((X)&~0xFF)
#define RMP_THD_STATE_SET(X,S) ((X)=(RMP_THD_FLAG(X)|(S)))

/* This thread is currently unused */
#define RMP_THD_FREE           (0)
/* This thread is currently running */
#define RMP_THD_RUNNING        (1)
/* Blocked on a send endpoint */
#define RMP_THD_SNDBLK         (2)
/* Blocked on a send endpoint with a timeout */
#define RMP_THD_SNDDLY         (3)
/* Blocked on its own receive endpoint */
#define RMP_THD_RCVBLK         (4)
/* Blocked on its own receive endpoint with a timeout */
#define RMP_THD_RCVDLY         (5)
/* Just on the timer delay */
#define RMP_THD_DELAYED        (6)
/* Blocked on a semaphore */
#define RMP_THD_SEMBLK         (7)
/* Blocked on a semaphore with a timeout */
#define RMP_THD_SEMDLY         (8)
/* Suspended */
#define RMP_THD_SUSPENDED      (1<<8)
/* Mailbox valid */
#define RMP_THD_MBOXFUL        (RMP_THD_SUSPENDED<<1)
    
/* States of semaphores */
#define RMP_SEM_FREE           (0)
#define RMP_SEM_USED           (1)

/* Error codes */
/* This error is thread related */
#define RMP_ERR_THD            (-1)
/* This error is priority related */
#define RMP_ERR_PRIO           (-2)
/* This error is timeslice related */
#define RMP_ERR_SLICE          (-3)
/* This error is thread state related */
#define RMP_ERR_STATE          (-4)
/* This error is operation related */
#define RMP_ERR_OPER           (-5)
/* This error is semaphore related */
#define RMP_ERR_SEM            (-6)

/* Word sizes settings */
#define RMP_WORD_SIZE          (((ptr_t)1)<<RMP_WORD_ORDER)
#define RMP_WORD_MASK          (~(((ptr_t)(-1))<<(RMP_WORD_ORDER-1)))
#define RMP_BITMAP_SIZE        ((RMP_MAX_PREEMPT_PRIO-1)/RMP_WORD_SIZE+1)
#define RMP_ALLBITS            ((ptr_t)(-1))

/* Stack offset macros */
/* Head offset, for ascending stacks */
#define RMP_INIT_STACK_HEAD(X)   (((ptr_t)RMP_Init_Stack)+(X)*sizeof(ptr_t))
/* Tail offset, for descending stacks */
#define RMP_INIT_STACK_TAIL(X)   (((ptr_t)RMP_Init_Stack)+RMP_INIT_STACK_SIZE-(X)*sizeof(ptr_t))

/* Get the thread from delay list */
#define RMP_DLY2THD(X)         ((struct RMP_Thd*)(((ptr_t)(X))-sizeof(struct RMP_List)))

/* Printk macros */
#define RMP_PRINTK_I(INT)      RMP_Print_Int((INT))
#define RMP_PRINTK_U(UINT)     RMP_Print_Uint((UINT))
#define RMP_PRINTK_S(STR)      RMP_Print_String((s8*)(STR))
    
/* Assert macro */
#define RMP_ASSERT(X) \
do \
{ \
    if((X)==0) \
    { \
        RMP_PRINTK_S("\r\n***\r\nKernel panic - not syncing:\r\n"); \
        RMP_PRINTK_S(__FILE__); \
        RMP_PRINTK_S(" , Line "); \
        RMP_PRINTK_I(__LINE__); \
        RMP_PRINTK_S("\r\n"); \
        RMP_PRINTK_S(__DATE__); \
        RMP_PRINTK_S(" , "); \
        RMP_PRINTK_S(__TIME__); \
        RMP_PRINTK_S("\r\n"); \
        while(1); \
    } \
} \
while(0)
/*****************************************************************************/
/* __KERNEL_H_DEFS__ */
#endif
/* __HDR_DEFS__ */
#endif
/* End Defines ***************************************************************/

/* Structs *******************************************************************/
#ifdef __HDR_STRUCTS__
#ifndef __KERNEL_H_STRUCTS__
#define __KERNEL_H_STRUCTS__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEFS__
#undef __HDR_DEFS__
/*****************************************************************************/
/* The list head structure */
struct RMP_List
{
    volatile struct RMP_List* Prev;
    volatile struct RMP_List* Next;
};

struct RMP_Thd
{
    /* The head to insert into the status queue */
    struct RMP_List Run_Head;
    /* This is exclusively for the timer queue */
    struct RMP_List Dly_Head;
    /* The list of threads suspended on it because of sending */
    struct RMP_List Snd_List;
    /* The state of this thread */
    ptr_t State;
    /* If it is running, how many timeslices it have */
    ptr_t Slices;
    /* If it is running, how many ticks does it have remaining */
    ptr_t Slices_Left;
    /* What priority it is running at */
    ptr_t Prio;
    /* The timeout time */
    ptr_t Timeout;
    /* The mailbox value */
    ptr_t Mailbox;
    /* The data to send */  
    ptr_t Data;
    /* The stack address */
    ptr_t Stack;
    /* The return value of certain function calls */
    ret_t Retval;
};

/* The semaphore structure */
struct RMP_Sem
{
    /* The waiting queue */
    struct RMP_List Wait_List;
    /* The state of the semaphore */
    ptr_t State;
    /* The current number of semaphore */
    ptr_t Cur_Num;
};
/*****************************************************************************/
/* __KERNEL_H_STRUCTS__ */
#endif
/* __HDR_STRUCTS__ */
#endif
/* End Structs ***************************************************************/

/* Private Global Variables **************************************************/
#if(!(defined __HDR_DEFS__||defined __HDR_STRUCTS__))
#ifndef __KERNEL_MEMBERS__
#define __KERNEL_MEMBERS__

/* In this way we can use the data structures and definitions in the headers */
#define __HDR_DEFS__

#undef __HDR_DEFS__

#define __HDR_STRUCTS__

#undef __HDR_STRUCTS__

/* If the header is not used in the public mode */
#ifndef __HDR_PUBLIC_MEMBERS__
/*****************************************************************************/
/* The scheduler bitmap */
static volatile ptr_t RMP_Bitmap[RMP_BITMAP_SIZE];
static volatile struct RMP_List RMP_Run[RMP_MAX_PREEMPT_PRIO];
static volatile struct RMP_List RMP_Delay;

/* The timestamp value */
static volatile ptr_t RMP_Tick;
/* Scheduler lock */
static volatile ptr_t RMP_Sched_Lock_Cnt;
static volatile ptr_t RMP_Sched_Locked;
/* Scheduler pending */
static volatile ptr_t RMP_Sched_Pend;
/* Timer events pending */
static volatile ptr_t RMP_Timer_Pend;

/* Init thread */
static volatile ptr_t RMP_Init_Stack[RMP_INIT_STACK_SIZE>>(RMP_WORD_ORDER-3)];
static volatile struct RMP_Thd RMP_Init_Thd;
/*****************************************************************************/
/* End Private Global Variables **********************************************/

/* Private C Function Prototypes *********************************************/ 
/*****************************************************************************/
static void _RMP_Set_Rdy(volatile struct RMP_Thd* Thread);
static void _RMP_Clr_Rdy(volatile struct RMP_Thd* Thread);
static void _RMP_Dly_Ins(volatile struct RMP_Thd* Thread, ptr_t Slices);
static void _RMP_Timer_Proc(void);
/*****************************************************************************/
#define __EXTERN__
/* End Private C Function Prototypes *****************************************/

/* Public Global Variables ***************************************************/
/* __HDR_PUBLIC_MEMBERS__ */
#else
#define __EXTERN__ EXTERN 
/* __HDR_PUBLIC_MEMBERS__ */
#endif

/*****************************************************************************/
/* The current thread - the pointer itself is volatile but not its contents */
__EXTERN__ struct RMP_Thd* volatile RMP_Cur_Thd;
__EXTERN__ volatile ptr_t RMP_Cur_SP;
/*****************************************************************************/

/* End Public Global Variables ***********************************************/

/* Public C Function Prototypes **********************************************/
/*****************************************************************************/
/* This is the entry of user applications */
EXTERN void RMP_Init(void);
__EXTERN__ void _RMP_Get_High_Rdy(void);
__EXTERN__ void _RMP_Tick_Handler(ptr_t Ticks);
__EXTERN__ ptr_t _RMP_Get_Near_Ticks(void);
__EXTERN__ void RMP_Clear(volatile void* Addr, ptr_t Size);

/* Scheduler locking & unlocking */
__EXTERN__ void RMP_Lock_Sched(void);
__EXTERN__ void RMP_Unlock_Sched(void);

/* Debug printing functions */
__EXTERN__ cnt_t RMP_Print_Int(cnt_t Int);
__EXTERN__ cnt_t RMP_Print_Uint(ptr_t Uint);
__EXTERN__ cnt_t RMP_Print_String(s8* String);

/* List operation functions */
__EXTERN__ void RMP_List_Crt(volatile struct RMP_List* Head);
__EXTERN__ void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next);
__EXTERN__ void RMP_List_Ins(volatile struct RMP_List* New,
                             volatile struct RMP_List* Prev,
                             volatile struct RMP_List* Next);

/* System interfaces */
__EXTERN__ void RMP_Yield(void);
__EXTERN__ ret_t RMP_Thd_Crt(volatile struct RMP_Thd* Thread, 
                             void* Entry, void* Stack, void* Arg, 
                             ptr_t Prio, ptr_t Slices);
__EXTERN__ ret_t RMP_Thd_Del(volatile struct RMP_Thd* Thread);
__EXTERN__ ret_t RMP_Thd_Set(volatile struct RMP_Thd* Thread, ptr_t Prio, ptr_t Slices);
__EXTERN__ ret_t RMP_Thd_Suspend(volatile struct RMP_Thd* Thread);
__EXTERN__ ret_t RMP_Thd_Resume(volatile struct RMP_Thd* Thread);

__EXTERN__ ret_t RMP_Thd_Delay(ptr_t Slices);
__EXTERN__ ret_t RMP_Thd_Cancel(volatile struct RMP_Thd* Thread);

__EXTERN__ ret_t RMP_Thd_Snd(volatile struct RMP_Thd* Thread, ptr_t Data, ptr_t Slices);
__EXTERN__ ret_t RMP_Thd_Snd_ISR(volatile struct RMP_Thd* Thread, ptr_t Data);
__EXTERN__ ret_t RMP_Thd_Rcv(ptr_t* Data, ptr_t Slices);

__EXTERN__ ret_t RMP_Sem_Crt(volatile struct RMP_Sem* Semaphore, ptr_t Number);
__EXTERN__ ret_t RMP_Sem_Del(volatile struct RMP_Sem* Semaphore);
__EXTERN__ ret_t RMP_Sem_Pend(volatile struct RMP_Sem* Semaphore, ptr_t Slices);
__EXTERN__ ret_t RMP_Sem_Abort(volatile struct RMP_Thd* Thread);
__EXTERN__ ret_t RMP_Sem_Post(volatile struct RMP_Sem* Semaphore, ptr_t Number);
__EXTERN__ ret_t RMP_Sem_Post_ISR(volatile struct RMP_Sem* Semaphore, ptr_t Number);

/* Hook functions */
#if(RMP_USE_HOOKS==RMP_TRUE)
    EXTERN void RMP_Start_Hook(void);
    EXTERN void RMP_Save_Ctx(void);
    EXTERN void RMP_Load_Ctx(void);
    EXTERN void RMP_Tick_Hook(ptr_t Ticks);
#else
    __EXTERN__ void RMP_Start_Hook(void);
    __EXTERN__ void RMP_Save_Ctx(void);
    __EXTERN__ void RMP_Load_Ctx(void);
    __EXTERN__ void RMP_Tick_Hook(ptr_t Ticks);
#endif

EXTERN void RMP_Init_Hook(void);
EXTERN void RMP_Init_Idle(void);
/*****************************************************************************/
/* Undefine "__EXTERN__" to avoid redefinition */
#undef __EXTERN__
/* __KERNEL_MEMBERS__ */
#endif
/* !(defined __HDR_DEFS__||defined __HDR_STRUCTS__) */
#endif
/* End Public C Function Prototypes ******************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
