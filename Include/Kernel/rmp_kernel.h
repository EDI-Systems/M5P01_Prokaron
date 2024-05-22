/******************************************************************************
Filename    : rmp_kernel.h
Author      : pry
Date        : 01/04/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The header file for the kernel.
******************************************************************************/

/* Define ********************************************************************/
#ifdef __HDR_DEF__
#ifndef __RMP_KERNEL_DEF__
#define __RMP_KERNEL_DEF__
/*****************************************************************************/
/* Constants */
#define RMP_NULL                    ((void*)0)

/* States of threads */
#define RMP_THD_STATE(X)            ((X)&((rmp_ptr_t)0xFFU))
#define RMP_THD_FLAG(X)             ((X)&~((rmp_ptr_t)0xFFU))
#define RMP_THD_STATE_SET(X, S)     ((X)=(RMP_THD_FLAG(X)|(S)))

/* This thread is currently unused */
#define RMP_THD_FREE                (0U)
/* This thread is currently running */
#define RMP_THD_RUNNING             (1U)
/* Blocked on a send endpoint */
#define RMP_THD_SNDBLK              (2U)
/* Blocked on a send endpoint with a timeout */
#define RMP_THD_SNDDLY              (3U)
/* Blocked on its own receive endpoint */
#define RMP_THD_RCVBLK              (4U)
/* Blocked on its own receive endpoint with a timeout */
#define RMP_THD_RCVDLY              (5U)
/* Just on the timer delay */
#define RMP_THD_DELAYED             (6U)
/* Blocked on a semaphore */
#define RMP_THD_SEMBLK              (7U)
/* Blocked on a semaphore with a timeout */
#define RMP_THD_SEMDLY              (8U)
/* Suspended */
#define RMP_THD_SUSPENDED           RMP_POW2(8U)
/* Mailbox valid */
#define RMP_THD_MBOXFUL             (RMP_THD_SUSPENDED<<1U)
    
/* States of semaphores */
#define RMP_SEM_FREE                (0U)
#define RMP_SEM_USED                (1U)

/* States of memory blocks */
#define RMP_MEM_FREE                (0U)
#define RMP_MEM_USED                (1U)

/* States of FIFOs */
#define RMP_FIFO_FREE               (0U)
#define RMP_FIFO_USED               (1U)

/* States of message queues */
#define RMP_MSGQ_FREE               (0U)
#define RMP_MSGQ_USED               (1U)

/* States of blocking message queues */
#define RMP_BMQ_FREE               	(0U)
#define RMP_BMQ_USED               	(1U)

/* Error codes */
/* This error is thread related */
#define RMP_ERR_THD                 (-1)
/* This error is priority related */
#define RMP_ERR_PRIO                (-2)
/* This error is timeslice related */
#define RMP_ERR_SLICE               (-3)
/* This error is thread state related */
#define RMP_ERR_STATE               (-4)
/* This error is operation related */
#define RMP_ERR_OPER                (-5)
/* This error is semaphore related */
#define RMP_ERR_SEM                 (-6)
/* This error is memory related */
#define RMP_ERR_MEM                 (-7)

/* Extended error codes */
/* This error is FIFO related */
#define RMP_ERR_FIFO                (-8)
/* This error is message queue related */
#define RMP_ERR_MSGQ                (-9)
/* This error is blocking message queue related */
#define RMP_ERR_BMQ                 (-10)

/* Power and rounding */
#define RMP_POW2(POW)               (((rmp_ptr_t)1U)<<(POW))
#define RMP_ROUND_DOWN(NUM,POW)     (((NUM)>>(POW))<<(POW))
#define RMP_ROUND_UP(NUM,POW)       RMP_ROUND_DOWN((NUM)+RMP_POW2(POW)-1U,POW)

/* Word size - only assume 8 bits for char, even if it exceeds 8 bits */
#define RMP_ALLBITS                 (~((rmp_ptr_t)0U))
#define RMP_WORD_SIZE               RMP_POW2(RMP_WORD_ORDER)
#define RMP_WORD_MASK               (~(RMP_ALLBITS<<RMP_WORD_ORDER))

/* Scheduler bitmap */
#define RMP_PRIO_WORD_NUM           (RMP_ROUND_UP(RMP_PREEMPT_PRIO_NUM,RMP_WORD_ORDER)>>RMP_WORD_ORDER)

/* Initial stack helpers - end address -1 to avoid empty descending stack issues */
#define RMP_INIT_STACK_START        ((rmp_ptr_t)RMP_Init_Stack)
#define RMP_INIT_STACK_END          (RMP_INIT_STACK_START+RMP_INIT_STACK_SIZE-1U)
#define RMP_INIT_STACK_ASCEND(X)    RMP_ROUND_UP(RMP_INIT_STACK_START+sizeof(rmp_ptr_t),X)
#define RMP_INIT_STACK_DESCEND(X)   RMP_ROUND_DOWN(RMP_INIT_STACK_END-sizeof(rmp_ptr_t),X)

/* Get the thread from delay list - avoid possible interference
 * from struct packing at the head, though this is unlikely */
#define RMP_OFFSET(T,M)             ((rmp_ptr_t)&(((T*)RMP_NULL)->M))
#define RMP_DLY2THD(X)              ((volatile struct RMP_Thd*)(((rmp_ptr_t)(X))-RMP_OFFSET(struct RMP_Thd,Dly_Head)))
/* Detect timer overflow */
#define RMP_DLY_DIFF(X)             ((X)-RMP_Timestamp)
#define RMP_DIFF_OVF(X)             (((X)>(RMP_ALLBITS>>1U))||((X)==0U))

/* Printk macros */
#define RMP_DBG_I(INT)              RMP_Int_Print((rmp_cnt_t)(INT))
#define RMP_DBG_H(UINT)             RMP_Hex_Print((rmp_ptr_t)(UINT))
#define RMP_DBG_S(STR)              RMP_Str_Print((const rmp_s8_t*)(STR))

/* Memory pool */
#define RMP_MEM_WORD_NUM(FLI)       (RMP_ROUND_UP((FLI)<<3,RMP_WORD_ORDER)>>RMP_WORD_ORDER)
#define RMP_MEM_POS(FLI,SLI)        ((SLI)+((FLI)<<3U))

/* Built-in graphics */
#ifdef RMP_POINT
/* Absolute difference between two numbers */
#define RMP_ABS(X,Y)                (((X)>(Y))?((X)-(Y)):((Y)-(X)))
/* "Shift (signed number) Arithmetically Left/Right": UB if done directly, however:
 *  (1) for SAL, we guarantee ABS(X) is smaller than UINT_MAX/2^(B+1);
 *  (2) for SAR, we never shift negative integers. So these operations are safe. */
#define RMP_SAL(X,B)                ((rmp_cnt_t)(((rmp_ptr_t)(X))<<(B)))
#define RMP_SAR(X,B)                ((rmp_cnt_t)(((rmp_ptr_t)(X))>>(B)))
#define RMP_TRANS                   (0x01U)
#define RMP_MAT_SMALL               (0U)
#define RMP_MAT_BIG                 (1U)
#define RMP_MAT_BPOS(MAT,POS)       ((MAT)[((rmp_ptr_t)(POS))>>3U]&RMP_POW2(7U-(((rmp_ptr_t)(POS))&0x07U)))
#define RMP_MAT_SPOS(MAT,POS)       ((MAT)[((rmp_ptr_t)(POS))>>3U]&RMP_POW2(((rmp_ptr_t)(POS))&0x07U))
#define RMP_RBTN_SEL                (1U)
#define RMP_PBAR_L2R                (0U)
#define RMP_PBAR_D2U                (1U)
#define RMP_PBAR_R2L                (2U)
#define RMP_PBAR_U2D                (3U)
/* Cursor X/Y position extraction */
#define RMP_CUR_XPOS(A,X)           ((rmp_cnt_t)((rmp_u8_t)((A)[X]&0x0FU)))
#define RMP_CUR_YPOS(A,Y)           ((rmp_cnt_t)((rmp_u8_t)((A)[X]>>4U)))
#define RMP_CUR_NORM                (0U)
#define RMP_CUR_BUSY                (1U)
#define RMP_CUR_QUESTION            (2U)
#define RMP_CUR_HAND                (3U)
#define RMP_CUR_TEXT                (4U)
#define RMP_CUR_STOP                (5U)
#define RMP_CUR_MOVE                (6U)
#define RMP_CUR_LR                  (7U)
#define RMP_CUR_UD                  (8U)
#define RMP_CUR_ULBR                (9U)
#define RMP_CUR_URBL                (10U)
#define RMP_CUR_CROSS               (11U)
#endif
    
/* Assert macro - used only in internal development */
#if(RMP_ASSERT_CORRECT==0U)
#define RMP_ASSERT(X) \
do \
{ \
    if(!(X)) \
    { \
        RMP_DBG_S("\r\n***\r\nKernel panic - not syncing:\r\n"); \
        RMP_DBG_S(__FILE__); \
        RMP_DBG_S(" , Line "); \
        RMP_DBG_I(__LINE__); \
        RMP_DBG_S("\r\n"); \
        RMP_DBG_S(__DATE__); \
        RMP_DBG_S(" , "); \
        RMP_DBG_S(__TIME__); \
        RMP_DBG_S("\r\n"); \
        while(1); \
    } \
} \
while(0)
#else
#define RMP_ASSERT(X) \
do \
{ \
    (void)(X); \
} \
while(0)
#endif
    
/* Coverage marker enabling */
#ifdef RMP_COVERAGE_LINE_NUM
#define RMP_COVERAGE_WORD_NUM       (RMP_ROUND_UP(RMP_COVERAGE_LINE_NUM,RMP_WORD_ORDER)>>RMP_WORD_ORDER)
#define RMP_COVERAGE_MARKER()       (RMP_Coverage[__LINE__>>RMP_WORD_ORDER]|=RMP_POW2(__LINE__&RMP_WORD_MASK))
#else
#define RMP_COVERAGE_MARKER()
#endif
/*****************************************************************************/
/* __RMP_KERNEL_DEF__ */
#endif
/* __HDR_DEF__ */
#endif
/* End Define ****************************************************************/

/* Struct ********************************************************************/
#ifdef __HDR_STRUCT__
#ifndef __RMP_KERNEL_STRUCT__
#define __RMP_KERNEL_STRUCT__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEF__
#undef __HDR_DEF__
/*****************************************************************************/
/* The list head structure */
struct RMP_List
{
    volatile struct RMP_List* Prev;
    volatile struct RMP_List* Next;
};

/* Thread structure - 15 words */
struct RMP_Thd
{
    /* The head to insert into the status queue */
    struct RMP_List Run_Head;
    /* This is exclusively for the timer queue */
    struct RMP_List Dly_Head;
    /* The list of threads suspended on it because of sending */
    struct RMP_List Snd_List;
    /* The state of this thread */
    rmp_ptr_t State;
    /* If it is running, how many timeslices it have */
    rmp_ptr_t Slice;
    /* If it is running, how many ticks does it have remaining */
    rmp_ptr_t Slice_Left;
    /* What priority it is running at */
    rmp_ptr_t Prio;
    /* The timeout time */
    rmp_ptr_t Timeout;
    /* The mailbox value */
    rmp_ptr_t Mailbox;
    /* The data to send */  
    rmp_ptr_t Data;
    /* The stack address */
    rmp_ptr_t Stack;
    /* The return value of certain function calls */
    rmp_ret_t Retval;
};

/* The semaphore structure */
struct RMP_Sem
{
    /* The waiting queue */
    struct RMP_List Wait_List;
    /* The state of the semaphore */
    rmp_ptr_t State;
    /* The current number of semaphore */
    rmp_ptr_t Cur_Num;
};

/* The FIFO structure */
struct RMP_Fifo
{
    /* The datablocks */
    struct RMP_List Head;
    /* The current number of datablocks */
    rmp_ptr_t Cur_Num;
    /* The state of the FIFO */
    rmp_ptr_t State;
};

/* The message queue structure */
struct RMP_Msgq
{
    struct RMP_Sem Sem;
    struct RMP_Fifo Fifo;
    /* The state of the message queue */
    rmp_ptr_t State;
};

/* The blocking message queue structure */
struct RMP_Bmq
{
    struct RMP_Sem Sem;
    struct RMP_Msgq Msgq;
    /* The state of the blocking message queue */
    rmp_ptr_t State;
};

/* The head struct of a memory block */
struct RMP_Mem_Head
{
    /* This is what is used in TLSF LUT */
    struct RMP_List Head;
    /* Is this block used at the moment? */
    rmp_ptr_t State;
    /* The pointer to the tail */
    volatile struct RMP_Mem_Tail* Tail;
};

/* The tail struct of a memory block */
struct RMP_Mem_Tail
{
    /* This is for tailing the memory */
    volatile struct RMP_Mem_Head* Head;
};

/* The memory control header block structure */
struct RMP_Mem
{
    /* The number of FLIs in the system */
    rmp_ptr_t FLI_Num;
    /* The base address of the actual memory pool */
    rmp_ptr_t Base;
    /* The size of this pool, including the header, bitmap and list table */
    rmp_ptr_t Size;
    /* The location of the list table itself */
    struct RMP_List* Table;
    /* The bitmap - This is actually an array that have an indefinite length, and will
     * be decided at runtime. Don't fuss if lint says that this can overflow; it is safe. */
    rmp_ptr_t Bitmap[1];
};
/*****************************************************************************/
/* __RMP_KERNEL_STRUCT__ */
#endif
/* __HDR_STRUCT__ */
#endif
/* End Struct ****************************************************************/

/* Private Variable **********************************************************/
#if(!(defined __HDR_DEF__||defined __HDR_STRUCT__))
#ifndef __RMP_KERNEL_MEMBER__
#define __RMP_KERNEL_MEMBER__

/* In this way we can use the data structures and definitions in the headers */
#define __HDR_DEF__

#undef __HDR_DEF__

#define __HDR_STRUCT__

#undef __HDR_STRUCT__

/* If the header is not used in the public mode */
#ifndef __HDR_PUBLIC__
/*****************************************************************************/
#ifdef RMP_COVERAGE_LINE_NUM
/* For coverage use only */
static volatile rmp_ptr_t RMP_Coverage[RMP_COVERAGE_WORD_NUM];
#endif
/* The scheduler bitmap */
static volatile rmp_ptr_t RMP_Bitmap[RMP_PRIO_WORD_NUM];
static volatile struct RMP_List RMP_Run[RMP_PREEMPT_PRIO_NUM];
static volatile struct RMP_List RMP_Delay;

/* Init thread stack and structure */
static rmp_u8_t RMP_Init_Stack[RMP_INIT_STACK_SIZE];
static volatile struct RMP_Thd RMP_Init_Thd;
/*****************************************************************************/
/* End Private Variable ******************************************************/

/* Private Function **********************************************************/ 
/*****************************************************************************/
static void _RMP_Run_Ins(volatile struct RMP_Thd* Thread);
static void _RMP_Run_Del(volatile struct RMP_Thd* Thread);
static void _RMP_Dly_Ins(volatile struct RMP_Thd* Thread,
                         rmp_ptr_t Slice);
static void _RMP_Timer_Proc(void);
static rmp_ret_t _RMP_Sem_Pend_Core(volatile struct RMP_Sem* Semaphore,
                                    rmp_ptr_t Slice);
static void _RMP_Sem_Unblock(volatile struct RMP_Sem* Semaphore);
static void _RMP_Mem_Block(volatile struct RMP_Mem_Head* Addr,
                           rmp_ptr_t Size);
static void _RMP_Mem_Ins(volatile void* Pool,
                         volatile struct RMP_Mem_Head* Mem_Head);
static void _RMP_Mem_Del(volatile void* Pool,
                         volatile struct RMP_Mem_Head* Mem_Head);
static rmp_ret_t _RMP_Mem_Search(volatile void* Pool,
                                 rmp_ptr_t Size,
                                 rmp_ptr_t* FLI_Level,
                                 rmp_ptr_t* SLI_Level);

#if((defined RMP_CTL_WHITE)&&(defined RMP_CTL_LGREY)&& \
    (defined RMP_CTL_GREY)&&(defined RMP_CTL_DGREY)&& \
    (defined RMP_CTL_DARK)&&(defined RMP_CTL_DDARK)&& \
    (defined RMP_CTL_BLACK))
static void RMP_Radiobtn_Circle(rmp_cnt_t Coord_X,
                                rmp_cnt_t Coord_Y,
                                rmp_cnt_t Length);
static void RMP_Progbar_Prog(rmp_cnt_t Coord_X,
                             rmp_cnt_t Coord_Y,
                             rmp_cnt_t Length,
                             rmp_cnt_t Width,
                             rmp_cnt_t Style,
                             rmp_cnt_t Prog,
                             rmp_ptr_t Fore,
                             rmp_ptr_t Back);
#endif
/*****************************************************************************/
#define __EXTERN__
/* End Private Function ******************************************************/

/* Public Variable ***********************************************************/
/* __HDR_PUBLIC__ */
#else
#define __EXTERN__ EXTERN 
/* __HDR_PUBLIC__ */
#endif

/*****************************************************************************/
/* The current tick counter value */
__EXTERN__ volatile rmp_ptr_t RMP_Timestamp;
/* Scheduler lock count */
__EXTERN__ volatile rmp_ptr_t RMP_Sched_Lock_Cnt;
/* Scheduler pending */
__EXTERN__ volatile rmp_ptr_t RMP_Sched_Pend;
/* Timer events pending */
__EXTERN__ volatile rmp_ptr_t RMP_Timer_Pend;

/* The current thread - the pointer itself is volatile but not its contents */
__EXTERN__ volatile struct RMP_Thd* volatile RMP_Thd_Cur;
__EXTERN__ volatile rmp_ptr_t RMP_SP_Cur;
/*****************************************************************************/

/* End Public Variable *******************************************************/

/* Public Function ***********************************************************/
/*****************************************************************************/
/* Bit manipualtions */
__EXTERN__ rmp_ptr_t RMP_MSB_Generic(rmp_ptr_t Value);
__EXTERN__ rmp_ptr_t RMP_LSB_Generic(rmp_ptr_t Value);
__EXTERN__ rmp_ptr_t RMP_RBT_Generic(rmp_ptr_t Value);
/* Debug printing functions */
__EXTERN__ rmp_cnt_t RMP_Int_Print(rmp_cnt_t Int);
__EXTERN__ rmp_cnt_t RMP_Hex_Print(rmp_ptr_t Uint);
__EXTERN__ rmp_cnt_t RMP_Str_Print(const rmp_s8_t* String);
/* List operation functions */
__EXTERN__ void RMP_List_Crt(volatile struct RMP_List* Head);
__EXTERN__ void RMP_List_Del(volatile struct RMP_List* Prev,
                             volatile struct RMP_List* Next);
__EXTERN__ void RMP_List_Ins(volatile struct RMP_List* New,
                             volatile struct RMP_List* Prev,
                             volatile struct RMP_List* Next);
                             
/* This is the entry of user applications */
EXTERN void RMP_Init(void);
__EXTERN__ void _RMP_Run_High(void);
__EXTERN__ void _RMP_Tim_Handler(rmp_ptr_t Slice);
__EXTERN__ void _RMP_Tim_Elapse(rmp_ptr_t Slice);
__EXTERN__ rmp_ptr_t _RMP_Tim_Future(void);
__EXTERN__ rmp_ret_t _RMP_Tim_Idle(void);
__EXTERN__ void RMP_Clear(volatile void* Addr,
                          rmp_ptr_t Size);

/* Scheduler locking & unlocking */
__EXTERN__ void RMP_Sched_Lock(void);
__EXTERN__ void RMP_Sched_Unlock(void);


/* System interfaces */
__EXTERN__ void RMP_Yield(void);
__EXTERN__ rmp_ret_t RMP_Thd_Crt(volatile struct RMP_Thd* Thread, 
                                 void* Entry,
                                 void* Param, 
                                 void* Stack,
                                 rmp_ptr_t Size,
                                 rmp_ptr_t Prio,
                                 rmp_ptr_t Slice);
__EXTERN__ rmp_ret_t RMP_Thd_Del(volatile struct RMP_Thd* Thread);
__EXTERN__ rmp_ret_t RMP_Thd_Set(volatile struct RMP_Thd* Thread,
                                 rmp_ptr_t Prio,
                                 rmp_ptr_t Slice);
__EXTERN__ rmp_ret_t RMP_Thd_Suspend(volatile struct RMP_Thd* Thread);
__EXTERN__ rmp_ret_t RMP_Thd_Resume(volatile struct RMP_Thd* Thread);

__EXTERN__ rmp_ret_t RMP_Thd_Delay(rmp_ptr_t Slice);
__EXTERN__ rmp_ret_t RMP_Thd_Cancel(volatile struct RMP_Thd* Thread);

__EXTERN__ rmp_ret_t RMP_Thd_Snd(volatile struct RMP_Thd* Thread,
                                 rmp_ptr_t Data,
                                 rmp_ptr_t Slice);
__EXTERN__ rmp_ret_t RMP_Thd_Snd_ISR(volatile struct RMP_Thd* Thread,
                                     rmp_ptr_t Data);
__EXTERN__ rmp_ret_t RMP_Thd_Rcv(rmp_ptr_t* Data,
                                 rmp_ptr_t Slice);

__EXTERN__ rmp_ret_t RMP_Sem_Crt(volatile struct RMP_Sem* Semaphore,
                                 rmp_ptr_t Number);
__EXTERN__ rmp_ret_t RMP_Sem_Del(volatile struct RMP_Sem* Semaphore);
__EXTERN__ rmp_ret_t RMP_Sem_Pend(volatile struct RMP_Sem* Semaphore,
                                  rmp_ptr_t Slice);
__EXTERN__ rmp_ret_t RMP_Sem_Pend_Unlock(volatile struct RMP_Sem* Semaphore,
                                         rmp_ptr_t Slice);
__EXTERN__ rmp_ret_t RMP_Sem_Abort(volatile struct RMP_Thd* Thread);
__EXTERN__ rmp_ret_t RMP_Sem_Post(volatile struct RMP_Sem* Semaphore,
                                  rmp_ptr_t Number);
__EXTERN__ rmp_ret_t RMP_Sem_Post_ISR(volatile struct RMP_Sem* Semaphore,
                                      rmp_ptr_t Number);
__EXTERN__ rmp_ret_t RMP_Sem_Bcst(volatile struct RMP_Sem* Semaphore);
__EXTERN__ rmp_ret_t RMP_Sem_Bcst_ISR(volatile struct RMP_Sem* Semaphore);
__EXTERN__ rmp_ret_t RMP_Sem_Cnt(volatile struct RMP_Sem* Semaphore);

/* Memory interfaces */
__EXTERN__ rmp_ret_t RMP_Mem_Init(volatile void* Pool,
                                  rmp_ptr_t Size);
__EXTERN__ void* RMP_Malloc(volatile void* Pool,
                            rmp_ptr_t Size);
__EXTERN__ void RMP_Free(volatile void* Pool,
                         void* Mem_Ptr);
__EXTERN__ void* RMP_Realloc(volatile void* Pool,
                             void* Mem_Ptr,
                             rmp_ptr_t Size);
                             
/* Extended system interfaces - these are not real primitives */
__EXTERN__ rmp_ret_t RMP_Fifo_Crt(volatile struct RMP_Fifo* Fifo);
__EXTERN__ rmp_ret_t RMP_Fifo_Del(volatile struct RMP_Fifo* Fifo);
__EXTERN__ rmp_ret_t RMP_Fifo_Read(volatile struct RMP_Fifo* Fifo,
                                   volatile struct RMP_List** Node);
__EXTERN__ rmp_ret_t RMP_Fifo_Write(volatile struct RMP_Fifo* Fifo,
                                    volatile struct RMP_List* Node);
__EXTERN__ rmp_ret_t RMP_Fifo_Write_ISR(volatile struct RMP_Fifo* Fifo,
                                        volatile struct RMP_List* Node);
__EXTERN__ rmp_ret_t RMP_Fifo_Cnt(volatile struct RMP_Fifo* Fifo);

__EXTERN__ rmp_ret_t RMP_Msgq_Crt(volatile struct RMP_Msgq* Queue);
__EXTERN__ rmp_ret_t RMP_Msgq_Del(volatile struct RMP_Msgq* Queue);
__EXTERN__ rmp_ret_t RMP_Msgq_Snd(volatile struct RMP_Msgq* Queue,
                                  volatile struct RMP_List* Node);
__EXTERN__ rmp_ret_t RMP_Msgq_Snd_ISR(volatile struct RMP_Msgq* Queue,
                                      volatile struct RMP_List* Node);
__EXTERN__ rmp_ret_t RMP_Msgq_Rcv(volatile struct RMP_Msgq* Queue,
                                  volatile struct RMP_List** Node,
                                  rmp_ptr_t Slice);
__EXTERN__ rmp_ret_t RMP_Msgq_Cnt(volatile struct RMP_Msgq* Queue);

__EXTERN__ rmp_ret_t RMP_Bmq_Crt(volatile struct RMP_Bmq* Queue,
                                 rmp_ptr_t Limit);
__EXTERN__ rmp_ret_t RMP_Bmq_Del(volatile struct RMP_Bmq* Queue);
__EXTERN__ rmp_ret_t RMP_Bmq_Snd(volatile struct RMP_Bmq* Queue,
                                 volatile struct RMP_List* Node,
                                 rmp_ptr_t Slice);
__EXTERN__ rmp_ret_t RMP_Bmq_Snd_ISR(volatile struct RMP_Bmq* Queue,
                                     volatile struct RMP_List* Node);
__EXTERN__ rmp_ret_t RMP_Bmq_Rcv(volatile struct RMP_Bmq* Queue,
                                 volatile struct RMP_List** Node,
                                 rmp_ptr_t Slice);
__EXTERN__ rmp_ret_t RMP_Bmq_Cnt(volatile struct RMP_Bmq* Queue);


/* Built-in graphics */
#ifdef RMP_POINT
EXTERN void RMP_POINT(rmp_cnt_t Coord_X,
                      rmp_cnt_t Coord_Y,
                      rmp_ptr_t Color);
__EXTERN__ void RMP_Line(rmp_cnt_t Begin_X,
                         rmp_cnt_t Begin_Y,
                         rmp_cnt_t End_X,
                         rmp_cnt_t End_Y,
                         rmp_ptr_t Color);
__EXTERN__ void RMP_Dot_Line(rmp_cnt_t Begin_X,
                             rmp_cnt_t Begin_Y,
                             rmp_cnt_t End_X,
                             rmp_cnt_t End_Y,
                             rmp_ptr_t Dot,
                             rmp_ptr_t Space);
__EXTERN__ void RMP_Rectangle(rmp_cnt_t Coord_X,
                              rmp_cnt_t Coord_Y,
                              rmp_cnt_t Length,
                              rmp_cnt_t Width,
                              rmp_ptr_t Border,
                              rmp_ptr_t Fill);
__EXTERN__ void RMP_Round_Rect(rmp_cnt_t Coord_X,
                               rmp_cnt_t Coord_Y,
                               rmp_cnt_t Length,
                               rmp_cnt_t Width,
                               rmp_cnt_t Round,
                               rmp_ptr_t Color);
__EXTERN__ void RMP_Circle(rmp_cnt_t Center_X,
                           rmp_cnt_t Center_Y,
                           rmp_cnt_t Radius,
                           rmp_ptr_t Border,
                           rmp_ptr_t Fill);
__EXTERN__ void RMP_Matrix(rmp_cnt_t Coord_X,
                           rmp_cnt_t Coord_Y,
                           const rmp_u8_t* Matrix,
                           rmp_ptr_t Bit_Order,
                           rmp_cnt_t Length,
                           rmp_cnt_t Width,
                           rmp_ptr_t Color);
/* This is only exported when color mixing macros available */
#if((defined RMP_COLOR_25P)&&(defined RMP_COLOR_50P)&&(defined RMP_COLOR_75P))
__EXTERN__ void RMP_Matrix_AA(rmp_cnt_t Coord_X,
                              rmp_cnt_t Coord_Y,
                              const rmp_u8_t* Matrix,
                              rmp_ptr_t Bit_Order,
                              rmp_cnt_t Length,
                              rmp_cnt_t Width,
                              rmp_ptr_t Color,
                              rmp_ptr_t Back);
#endif
/* These are only provided when all used colors are predefined */
#if((defined RMP_CTL_WHITE)&&(defined RMP_CTL_LGREY)&& \
    (defined RMP_CTL_GREY)&&(defined RMP_CTL_DGREY)&& \
    (defined RMP_CTL_DARK)&&(defined RMP_CTL_DDARK)&& \
    (defined RMP_CTL_BLACK))
/* Built-in easy controls */
__EXTERN__ void RMP_Cursor(rmp_cnt_t Coord_X,
                           rmp_cnt_t Coord_Y,
                           rmp_ptr_t Style);
__EXTERN__ void RMP_Checkbox_Set(rmp_cnt_t Coord_X,
                                 rmp_cnt_t Coord_Y,
                                 rmp_cnt_t Length);
__EXTERN__ void RMP_Checkbox_Clr(rmp_cnt_t Coord_X,
                                 rmp_cnt_t Coord_Y,
                                 rmp_cnt_t Length);
__EXTERN__ void RMP_Checkbox(rmp_cnt_t Coord_X,
                             rmp_cnt_t Coord_Y,
                             rmp_cnt_t Length,
                             rmp_ptr_t Status);

__EXTERN__ void RMP_Cmdbtn_Down(rmp_cnt_t Coord_X,
                                rmp_cnt_t Coord_Y,
                                rmp_cnt_t Length,
                                rmp_cnt_t Width);
__EXTERN__ void RMP_Cmdbtn_Up(rmp_cnt_t Coord_X,
                              rmp_cnt_t Coord_Y,
                              rmp_cnt_t Length,
                              rmp_cnt_t Width);
__EXTERN__ void RMP_Cmdbtn(rmp_cnt_t Coord_X,
                           rmp_cnt_t Coord_Y,
                           rmp_cnt_t Length,
                           rmp_cnt_t Width,
                           rmp_ptr_t Status);

__EXTERN__ void RMP_Lineedit_Clr(rmp_cnt_t Coord_Y,
                                 rmp_cnt_t Length,
                                 rmp_cnt_t Width,
                                 rmp_cnt_t Clr_X,
                                 rmp_cnt_t Clr_Len);
__EXTERN__ void RMP_Lineedit(rmp_cnt_t Coord_X,
                             rmp_cnt_t Coord_Y,
                             rmp_cnt_t Length,
                             rmp_cnt_t Width);

__EXTERN__ void RMP_Radiobtn_Set(rmp_cnt_t Coord_X,
                                 rmp_cnt_t Coord_Y,
                                 rmp_cnt_t Length);
__EXTERN__ void RMP_Radiobtn_Clr(rmp_cnt_t Coord_X,
                                 rmp_cnt_t Coord_Y,
                                 rmp_cnt_t Length);
__EXTERN__ void RMP_Radiobtn(rmp_cnt_t Coord_X,
                             rmp_cnt_t Coord_Y,
                             rmp_cnt_t Length,
                             rmp_ptr_t Status);

__EXTERN__ void RMP_Progbar_Set(rmp_cnt_t Coord_X,
                                rmp_cnt_t Coord_Y,
                                rmp_cnt_t Length,
                                rmp_cnt_t Width,
                                rmp_cnt_t Style,
                                rmp_cnt_t Old_Prog,
                                rmp_cnt_t New_Prog,
                                rmp_ptr_t Fore,
                                rmp_ptr_t Back);
__EXTERN__ void RMP_Progbar(rmp_cnt_t Coord_X,
                            rmp_cnt_t Coord_Y,
                            rmp_cnt_t Length,
                            rmp_cnt_t Width,
                            rmp_cnt_t Style,
                            rmp_cnt_t Prog,
                            rmp_ptr_t Fore,
                            rmp_ptr_t Back);
#endif
#endif

/* Other utilities */
#ifdef __RMP_U8_T__
#ifdef __RMP_U16_T__
__EXTERN__ rmp_ptr_t RMP_CRC16(const rmp_u8_t* Data,
                               rmp_ptr_t Length);
#endif
#endif

/* Hook functions */
#if(RMP_HOOK_EXTRA!=0U)
EXTERN void RMP_Start_Hook(void);
EXTERN void RMP_Ctx_Save(void);
EXTERN void RMP_Ctx_Load(void);
EXTERN void RMP_Sched_Hook(void);
EXTERN void RMP_Tim_Hook(rmp_ptr_t Slice);
EXTERN void RMP_Dly_Hook(rmp_ptr_t Slice);
#endif

EXTERN void RMP_Init_Hook(void);
EXTERN void RMP_Init_Idle(void);

/* Coverage test */
#ifdef RMP_COVERAGE_LINE_NUM
__EXTERN__ void RMP_Coverage_Print(void);
#endif
/*****************************************************************************/
/* Undefine "__EXTERN__" to avoid redefinition */
#undef __EXTERN__
/* __RMP_KERNEL_MEMBER__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
