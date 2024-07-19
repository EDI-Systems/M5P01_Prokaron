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

/* Dewarn compiler */
#define RMP_USE(X)                  ((void)(X))

/* Power of 2 */
#define RMP_FIELD(VAL,POW)          (((rmp_ptr_t)(VAL))<<(POW))
#define RMP_POW2(POW)               RMP_FIELD(1U,POW)
/* Word size - RMP_WORD_BYTE != RMP_WORD_CHAR on word-addressing systems */
#define RMP_WORD_BIT                RMP_POW2(RMP_WORD_ORDER)
#define RMP_WORD_BYTE               (RMP_WORD_BIT>>3)
#define RMP_WORD_CHAR               (sizeof(rmp_ptr_t)/sizeof(rmp_u8_t))
/* Bit mask */
#define RMP_MASK_FULL               (~((rmp_ptr_t)0U))
#define RMP_MASK_INTMAX             (RMP_MASK_FULL>>1)
#define RMP_MASK_WORD               (~(RMP_MASK_FULL<<RMP_WORD_ORDER))
/* Rounding */
#define RMP_ROUND_DOWN(NUM,POW)     (((NUM)>>(POW))<<(POW))
#define RMP_ROUND_UP(NUM,POW)       RMP_ROUND_DOWN((NUM)+RMP_POW2(POW)-1U,POW)
/* Bitmap */
#define RMP_BITMAP_SET(BMP,POS)     ((BMP)[(POS)>>RMP_WORD_ORDER]|=RMP_POW2((POS)&RMP_MASK_WORD))
#define RMP_BITMAP_CLR(BMP,POS)     ((BMP)[(POS)>>RMP_WORD_ORDER]&=~RMP_POW2((POS)&RMP_MASK_WORD))
#define RMP_BITMAP_IS_SET(BMP,POS)  (((BMP)[(POS)>>RMP_WORD_ORDER]&RMP_POW2((POS)&RMP_MASK_WORD))!=0U)

/* Maximum logging length */
#define RMP_DBGLOG_MAX              (255U)

/* Debug logging macros */
#if(RMP_DBGLOG_ENABLE==1U)
#define RMP_DBG_I(INT)              RMP_Int_Print((rmp_cnt_t)(INT))
#define RMP_DBG_H(HEX)              RMP_Hex_Print((rmp_ptr_t)(HEX))
#define RMP_DBG_S(STR)              RMP_Str_Print((const rmp_s8_t*)(STR))
#else
#define RMP_DBG_I(INT)              while(0U)
#define RMP_DBG_H(HEX)              while(0U)
#define RMP_DBG_S(STR)              while(0U)
#endif

/* Logging macro */
#ifndef RMP_LOG
#define RMP_LOG_OP(F,L,D,T)         RMP_Log(F,L,D,T)
#else
#define RMP_LOG_OP(F,L,D,T)         RMP_LOG(F,L,D,T)
#endif

/* Assert macro - used only in internal development */
#if(RMP_ASSERT_ENABLE!=0U)
#define RMP_ASSERT(X) \
do \
{ \
    if(!(X)) \
    { \
        RMP_LOG_OP(__FILE__,__LINE__,__DATE__,__TIME__); \
        while(1); \
    } \
} \
while(0)
#else
#define RMP_ASSERT(X) \
do \
{ \
    RMP_USE(X); \
} \
while(0)
#endif
    
/* Coverage marker enabling */
#ifdef RMP_COV_LINE_NUM
#define RMP_COV_WORD_NUM            (RMP_ROUND_UP(RMP_COV_LINE_NUM,RMP_WORD_ORDER)>>RMP_WORD_ORDER)
#define RMP_COV_MARKER()            RMP_BITMAP_SET(RMP_Cov,__LINE__)
#else
#define RMP_COV_MARKER()
#endif

/* Thread state */
#define RMP_THD_STATE(X)            ((X)&((rmp_ptr_t)0xFFU))
#define RMP_THD_FLAG(X)             ((X)&~((rmp_ptr_t)0xFFU))
#define RMP_THD_STATE_SET(X,S)      ((X)=(RMP_THD_FLAG(X)|(S)))

/* Thread structure is unused */
#define RMP_THD_FREE                (0U)
/* Thread is currently ready */
#define RMP_THD_READY               (1U)
/* Blocked on a send endpoint */
#define RMP_THD_SNDBLK              (2U)
/* Blocked on a send endpoint with a timeout */
#define RMP_THD_SNDDLY              (3U)
/* Blocked on its own receive endpoint */
#define RMP_THD_RCVBLK              (4U)
/* Blocked on its own receive endpoint with a timeout */
#define RMP_THD_RCVDLY              (5U)
/* Just on the timer delay */
#define RMP_THD_DELAY               (6U)
/* Blocked on a semaphore */
#define RMP_THD_SEMBLK              (7U)
/* Blocked on a semaphore with a timeout */
#define RMP_THD_SEMDLY              (8U)
/* Suspended */
#define RMP_THD_SUSPEND             RMP_POW2(8U)
/* Mailbox valid */
#define RMP_THD_MBOXFUL             RMP_POW2(9U)
    
/* Semaphore state */
#define RMP_SEM_FREE                (0U)
#define RMP_SEM_USED                (1U)

/* Memory block state */
#define RMP_MEM_FREE                (0U)
#define RMP_MEM_USED                (1U)

/* FIFO state */
#define RMP_FIFO_FREE               (0U)
#define RMP_FIFO_USED               (1U)

/* Message queue state */
#define RMP_MSGQ_FREE               (0U)
#define RMP_MSGQ_USED               (1U)

/* Blocking message queue state */
#define RMP_BMQ_FREE                (0U)
#define RMP_BMQ_USED                (1U)

/* Alarm manager state */
#define RMP_AMGR_FREE               (0U)
#define RMP_AMGR_USED               (1U)

/* Alarm delay mode & value */
#define RMP_ALRM_DELAY(X)           ((X)&RMP_MASK_INTMAX)
#define RMP_ALRM_AUTORLD            (0U)
#define RMP_ALRM_ONESHOT            RMP_POW2(RMP_WORD_ORDER-1U)

/* Error codes */
/* This error is thread related */
#define RMP_ERR_THD                 (-1)
/* This error is priority related */
#define RMP_ERR_PRIO                (-2)
/* This error is timeslice related */
#define RMP_ERR_SLICE               (-3)
/* This error is stack related */
#define RMP_ERR_STACK               (-4)
/* This error is thread state related */
#define RMP_ERR_STATE               (-5)
/* This error is operation related */
#define RMP_ERR_OPER                (-6)
/* This error is semaphore related */
#define RMP_ERR_SEM                 (-7)
/* This error is memory related */
#define RMP_ERR_MEM                 (-8)

/* Extended error codes */
/* This error is FIFO related */
#define RMP_ERR_FIFO                (-9)
/* This error is message queue related */
#define RMP_ERR_MSGQ                (-10)
/* This error is blocking message queue related */
#define RMP_ERR_BMQ                 (-11)
/* This error is alarm manager related */
#define RMP_ERR_AMGR                (-12)
/* This error is alarm related */
#define RMP_ERR_ALRM                (-13)

/* Scheduler bitmap */
#define RMP_PRIO_WORD_NUM           (RMP_ROUND_UP(RMP_PREEMPT_PRIO_NUM,RMP_WORD_ORDER)>>RMP_WORD_ORDER)

/* Stack types */
#define RMP_STACK_FULL_ASCEND       (0U)
#define RMP_STACK_FULL_DESCEND      (1U)
#define RMP_STACK_EMPTY_ASCEND      (2U)
#define RMP_STACK_EMPTY_DESCEND     (3U)

/* Thread stack calculation helpers for four kinds of stacks */
#if(RMP_STACK_TYPE==RMP_STACK_FULL_ASCEND)
#define RMP_STACK_END(STK,SZ)       RMP_ROUND_UP((STK)-sizeof(RMP_STACK_ELEM),RMP_STACK_ALIGN)
#define RMP_STACK_PTR(STK,SZ)       (RMP_STACK_END(STK,SZ)+sizeof(RMP_STACK_STRUCT))
#define RMP_STACK_CTX(PTR)          ((RMP_STACK_STRUCT*)((PTR)-sizeof(RMP_STACK_STRUCT)+sizeof(RMP_STACK_ELEM)))
#elif(RMP_STACK_TYPE==RMP_STACK_FULL_DESCEND)
#define RMP_STACK_END(STK,SZ)       (RMP_ROUND_DOWN((STK)+(SZ),RMP_STACK_ALIGN))
#define RMP_STACK_PTR(STK,SZ)       (RMP_STACK_END(STK,SZ)-sizeof(RMP_STACK_STRUCT))
#define RMP_STACK_CTX(PTR)          ((RMP_STACK_STRUCT*)(PTR))
#elif(RMP_STACK_TYPE==RMP_STACK_EMPTY_ASCEND)
#define RMP_STACK_END(STK,SZ)       RMP_ROUND_UP((STK),(RMP_STACK_ALIGN))
#define RMP_STACK_PTR(STK,SZ)       (RMP_STACK_END(STK,SZ)+sizeof(RMP_STACK_STRUCT))
#define RMP_STACK_CTX(PTR)          ((RMP_STACK_STRUCT*)((PTR)-sizeof(RMP_STACK_STRUCT)))
#elif(RMP_STACK_TYPE==RMP_STACK_EMPTY_DESCEND)
#define RMP_STACK_END(STK,SZ)       (RMP_ROUND_DOWN((STK)+(SZ)-sizeof(RMP_STACK_ELEM),RMP_STACK_ALIGN))
#define RMP_STACK_PTR(STK,SZ)       (RMP_STACK_END(STK,SZ)-sizeof(RMP_STACK_STRUCT))
#define RMP_STACK_CTX(PTR)          ((RMP_STACK_STRUCT*)((PTR)+sizeof(RMP_STACK_ELEM)))
#else
#error Please choose a correct stack type.
#endif

/* Stack pointer and context extraction */
#define RMP_STACK_CALC(PTR,CTX,STK,SZ) \
do \
{ \
    RMP_USE(SZ); \
    (PTR)=RMP_STACK_PTR(STK,SZ); \
    (CTX)=RMP_STACK_CTX(PTR); \
} \
while(0)

/* Initial thread's stack pointer */
#define RMP_INIT_STACK              RMP_STACK_END((rmp_ptr_t)RMP_Init_Stack,RMP_INIT_STACK_SIZE)

/* Get the thread from delay list - avoid possible interference
 * from struct packing at the head, though this is unlikely */
#define RMP_OFFSET(T,M)             ((rmp_ptr_t)&(((T*)RMP_NULL)->M))
#define RMP_DLY2THD(X)              ((volatile struct RMP_Thd*)(((rmp_ptr_t)(X))-RMP_OFFSET(struct RMP_Thd,Dly_Head)))
/* Detect overflow */
#define RMP_DIFF(X,T)               ((X)-(T))
#define RMP_DLY_DIFF(X)             RMP_DIFF(X,RMP_Timestamp)
#define RMP_DIFF_OVF(X)             (((X)>RMP_MASK_INTMAX)||((X)==0U))

/* Memory pool */
/* Table */
#define RMP_MEM_WORD_NUM(FLI)       (RMP_ROUND_UP((FLI)<<3,RMP_WORD_ORDER)>>RMP_WORD_ORDER)
#define RMP_MEM_POS(FLI,SLI)        ((SLI)+((FLI)<<3U))
/* Tail initialization */
#define RMP_MEM_TAIL_INIT(HEAD,SZ)  ((volatile struct RMP_Mem_Tail*)(((rmp_ptr_t)(HEAD))+((rmp_ptr_t)(SZ))-sizeof(struct RMP_Mem_Tail)))
/* Convert head to something else */
#define RMP_MEM_HEAD2SIZE(HEAD)     (((rmp_ptr_t)((HEAD)->Tail))-((rmp_ptr_t)(HEAD))-sizeof(struct RMP_Mem_Head))
#define RMP_MEM_HEAD2END(HEAD)      (((rmp_ptr_t)((HEAD)->Tail))+sizeof(struct RMP_Mem_Tail))
#define RMP_MEM_HEAD2RIGHT(HEAD)    ((volatile struct RMP_Mem_Head*)(((rmp_ptr_t)((HEAD)->Tail))+sizeof(struct RMP_Mem_Tail)))
#define RMP_MEM_HEAD2LEFT(HEAD)     (((volatile struct RMP_Mem_Tail*)(((rmp_ptr_t)(HEAD))-sizeof(struct RMP_Mem_Tail)))->Head)
/* Convert between usable area size and the whole block size */
#define RMP_MEM_SIZE2WHOLE(SZ)      (sizeof(struct RMP_Mem_Head)+((rmp_ptr_t)(SZ))+sizeof(struct RMP_Mem_Tail))
#define RMP_MEM_WHOLE2SIZE(WHOLE)   (((rmp_ptr_t)(WHOLE))-sizeof(struct RMP_Mem_Head)-sizeof(struct RMP_Mem_Tail))
/* Convert between usable area pointer and the while block pointer */
#define RMP_MEM_PTR2HEAD(PTR)       ((volatile struct RMP_Mem_Head*)(((rmp_ptr_t)(PTR))-sizeof(struct RMP_Mem_Head)))
/* Difference between pointers */
#define RMP_MEM_PTR_DIFF(PTR1,PTR2) (((rmp_ptr_t)(PTR1))-((rmp_ptr_t)(PTR2)))

/* Built-in graphics */
#if(RMP_GUI_ENABLE!=0U)
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
/* List head structure - next before prev for faster traversal */
struct RMP_List
{
    volatile struct RMP_List* Next;
    volatile struct RMP_List* Prev;
};

/* Thread structure - 15 words */
struct RMP_Thd
{
    /* The head to insert into the status queue */
    struct RMP_List Run_Head;
    /* This is exclusively for the timer queue */
    struct RMP_List Dly_Head;
    /* The threads blocked on its mailbox because of sending */
    struct RMP_List Snd_List;
    /* The state of this thread */
    rmp_ptr_t State;
#if(RMP_STKSEG_ENABLE!=0U)
    /* The current stack segment */
    rmp_ptr_t Segment;
#endif
    /* The current stack address */
    rmp_ptr_t Stack;
    /* How many timeslices to replenish */
    rmp_ptr_t Slice;
    /* How many timeslices are remaining in this round */
    rmp_ptr_t Slice_Left;
    /* The thread priority */
    rmp_ptr_t Prio;
    /* The timeout time */
    rmp_ptr_t Timeout;
    /* The mailbox send/recv cache */
    rmp_ptr_t Mail_Snd;
    rmp_ptr_t Mail_Rcv;
    /* The return value of certain fallible calls */
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
    rmp_ptr_t Num_Cur;
};

/* The FIFO structure */
struct RMP_Fifo
{
    struct RMP_List Head;
    /* The current number of datablocks */
    rmp_ptr_t Num_Cur;
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

/* The alarm manager structure */
struct RMP_Amgr
{
    struct RMP_Sem Mutex;
    /* The state of the alarm manager */
    rmp_ptr_t State;
    /* Current timestamp */
    rmp_ptr_t Timestamp;
    /* Current number of alarms */
    rmp_ptr_t Num_Cur;
    /* The alarms, in a timeout ascending order */
    struct RMP_List Alrm;
};

/* The alarm structure */
struct RMP_Alrm
{
    struct RMP_List Head;
    /* The delay time and mode of the alarm */
    rmp_ptr_t Delay;
    /* The timeout moment */
    rmp_ptr_t Timeout;
    /* The manager of the alarm */
    volatile struct RMP_Amgr* Amgr;
    /* Timeout callback hook */
    void (*Hook)(volatile struct RMP_Amgr*,
                 volatile struct RMP_Alrm*,
                 rmp_cnt_t);
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
    /* Pointer to the memory header */
    volatile struct RMP_Mem_Head* Head;
};

/* The memory control header block structure */
struct RMP_Mem
{
    /* The number of FLIs in the system */
    rmp_ptr_t FLI_Num;
    /* The base address of the actual memory pool */
    rmp_ptr_t Base;
    /* The total size of this pool, including the header, bitmap and list table */
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
#ifdef RMP_COV_LINE_NUM
/* For kernel coverage use only */
static volatile rmp_ptr_t RMP_Cov[RMP_COV_WORD_NUM];
#endif

/* Scheduler bitmap - might be modified in interrupts */
static volatile rmp_ptr_t RMP_Bitmap[RMP_PRIO_WORD_NUM];
/* Running list for each priority - might be modified in interrupts */
static volatile struct RMP_List RMP_Run[RMP_PREEMPT_PRIO_NUM];
/* Delay list - might be modified in interrupts */
static volatile struct RMP_List RMP_Delay;

/* Init thread stack and structure */
static rmp_u8_t RMP_Init_Stack[RMP_INIT_STACK_SIZE];
static volatile struct RMP_Thd RMP_Init_Thd;
/*****************************************************************************/
/* End Private Variable ******************************************************/

/* Private Function **********************************************************/ 
/*****************************************************************************/
static void _RMP_Run_Ins(volatile struct RMP_Thd* Thread,
                         rmp_ptr_t State);
static void _RMP_Run_Del(volatile struct RMP_Thd* Thread,
                         rmp_ptr_t State);
static void _RMP_Dly_Ins(volatile struct RMP_Thd* Thread,
                         rmp_ptr_t Slice);

static void _RMP_Thd_Remove(volatile struct RMP_Thd* Thread,
                            rmp_ptr_t Delay_Queue);
static rmp_ptr_t _RMP_Thd_Unblock(volatile struct RMP_Thd* Thd_Cur,
                                  rmp_ptr_t* Data,
                                  rmp_ptr_t State);
static rmp_ret_t _RMP_Sem_Pend_Core(volatile struct RMP_Sem* Semaphore,
                                    rmp_ptr_t Slice);

static void _RMP_Mem_Block(volatile struct RMP_Mem_Head* Head,
                           rmp_ptr_t Size,
                           rmp_ptr_t State);
static void _RMP_Mem_Ins(volatile void* Pool,
                         volatile struct RMP_Mem_Head* Head);
static void _RMP_Mem_Del(volatile void* Pool,
                         volatile struct RMP_Mem_Head* Head);
static rmp_ret_t _RMP_Mem_Search(volatile void* Pool,
                                 rmp_ptr_t Size,
                                 rmp_ptr_t* FLI_Level,
                                 rmp_ptr_t* SLI_Level);

static void _RMP_Amgr_Ins(volatile struct RMP_Amgr* Amgr,
                          volatile struct RMP_Alrm* Alrm,
                          rmp_ptr_t Timestamp);
static void _RMP_Amgr_Expire(volatile struct RMP_Amgr* Amgr,
                             volatile struct RMP_Alrm* Alrm,
                             rmp_cnt_t Overdue,
                             rmp_ptr_t Timestamp);

#if(RMP_GUI_WIDGET_ENABLE!=0U)
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
#define __RMP_EXTERN__
/* End Private Function ******************************************************/

/* Public Variable ***********************************************************/
/* __HDR_PUBLIC__ */
#else
#define __RMP_EXTERN__ RMP_EXTERN 
/* __HDR_PUBLIC__ */
#endif

/*****************************************************************************/
#ifdef RMP_COV_LINE_NUM
/* For kernel coverage use only */
static volatile rmp_ptr_t RMP_Cov[RMP_COV_WORD_NUM];
#endif

/* Timestamp - may wraparound but this is fine */
__RMP_EXTERN__ volatile rmp_ptr_t RMP_Timestamp;
/* Scheduler lock count - doesn't really need to be volatile from a kernel
 * standpoint, and transparent interrupts that read it will fetch from memory
 * anyway, but we wish to indicate that interrupts may read it by volatile. */
__RMP_EXTERN__ volatile rmp_ptr_t RMP_Sched_Lock_Cnt;
/* Scheduler pending - needs to be volatile; might be modified in interrupts */
__RMP_EXTERN__ volatile rmp_ptr_t RMP_Sched_Pend;

/* The current thread - the pointer as well as its contents are volatile. From
 * a pure kernel standpoint, the pointer itself doesn't need to be volatile,
 * because when we switch back to the same thread the pointer must be pointing
 * to the thread's own structure, thus the compiler-cached pointer value is
 * correct even when function boundaries are not compiler barriers (may happen
 * in LTO). However, we still make it volatile here and consciously cache it
 * at the beginning of every function that might use it to indicate that it
 * may have changed in the middle (even though it always changes back). */
__RMP_EXTERN__ volatile struct RMP_Thd* volatile RMP_Thd_Cur;
#if(RMP_STKSEG_ENABLE!=0U)
/* Current thread's stack segment */
__RMP_EXTERN__ volatile rmp_ptr_t RMP_SS_Cur;
#endif
/* Current thread's stack pointer */
__RMP_EXTERN__ volatile rmp_ptr_t RMP_SP_Cur;
/*****************************************************************************/

/* End Public Variable *******************************************************/

/* Public Function ***********************************************************/
/*****************************************************************************/
/* Debug printing */
#if(RMP_DBGLOG_ENABLE!=0U)
__RMP_EXTERN__ rmp_cnt_t RMP_Int_Print(rmp_cnt_t Int);
__RMP_EXTERN__ rmp_cnt_t RMP_Hex_Print(rmp_ptr_t Uint);
__RMP_EXTERN__ rmp_cnt_t RMP_Str_Print(const rmp_s8_t* String);
#endif

/* Default logging */
#ifndef RMP_LOG
__RMP_EXTERN__ void RMP_Log(const char* File,
                            long Line,
                            const char* Date,
                            const char* Time);
#endif

/* Coverage test - internal use */
#ifdef RMP_COV_LINE_NUM
__RMP_EXTERN__ void RMP_Cov_Print(void);
#endif
                 
/* Misc helper */
__RMP_EXTERN__ void RMP_Clear(volatile void* Addr,
                              rmp_ptr_t Size);

/* Bit manipulation */
__RMP_EXTERN__ rmp_ptr_t RMP_MSB_Generic(rmp_ptr_t Value);
__RMP_EXTERN__ rmp_ptr_t RMP_LSB_Generic(rmp_ptr_t Value);
                            
/* List operation */
__RMP_EXTERN__ void RMP_List_Crt(volatile struct RMP_List* Head);
__RMP_EXTERN__ void RMP_List_Del(volatile struct RMP_List* Prev,
                                 volatile struct RMP_List* Next);
__RMP_EXTERN__ void RMP_List_Ins(volatile struct RMP_List* New,
                                 volatile struct RMP_List* Prev,
                                 volatile struct RMP_List* Next);
                                 
/* Scheduler utility */
__RMP_EXTERN__ void _RMP_Run_High(void);
__RMP_EXTERN__ void _RMP_Tim_Handler(rmp_ptr_t Slice);
__RMP_EXTERN__ void _RMP_Tim_Elapse(rmp_ptr_t Slice);
__RMP_EXTERN__ rmp_ptr_t _RMP_Tim_Future(void);
__RMP_EXTERN__ rmp_ret_t _RMP_Tim_Idle(void);

/* Scheduler locking & unlocking */
__RMP_EXTERN__ void RMP_Sched_Lock(void);
__RMP_EXTERN__ void RMP_Sched_Unlock(void);

/* First thread - in case the user needs to boot similar threads */
__RMP_EXTERN__ void RMP_Init(void);

/* Basic thread interface */
__RMP_EXTERN__ rmp_ret_t RMP_Thd_Crt(volatile struct RMP_Thd* Thread, 
                                     void* Entry,
                                     void* Param,
#if(RMP_STKSEG_ENABLE!=0U)
                                     rmp_ptr_t Segment,
#endif
                                     void* Stack,
                                     rmp_ptr_t Size,
                                     rmp_ptr_t Prio,
                                     rmp_ptr_t Slice);
__RMP_EXTERN__ rmp_ret_t RMP_Thd_Del(volatile struct RMP_Thd* Thread);
__RMP_EXTERN__ rmp_ret_t RMP_Thd_Set(volatile struct RMP_Thd* Thread,
                                     rmp_ptr_t Prio,
                                     rmp_ptr_t Slice);

__RMP_EXTERN__ void RMP_Thd_Yield(void);
__RMP_EXTERN__ rmp_ret_t RMP_Thd_Suspend(volatile struct RMP_Thd* Thread);
__RMP_EXTERN__ rmp_ret_t RMP_Thd_Resume(volatile struct RMP_Thd* Thread);

__RMP_EXTERN__ rmp_ret_t RMP_Thd_Delay(rmp_ptr_t Slice);
__RMP_EXTERN__ rmp_ret_t RMP_Thd_Cancel(volatile struct RMP_Thd* Thread);
__RMP_EXTERN__ void RMP_Thd_Loop(rmp_ptr_t Loop);

__RMP_EXTERN__ rmp_ret_t RMP_Thd_Snd(volatile struct RMP_Thd* Thread,
                                     rmp_ptr_t Data,
                                     rmp_ptr_t Slice);
__RMP_EXTERN__ rmp_ret_t RMP_Thd_Snd_ISR(volatile struct RMP_Thd* Thread,
                                         rmp_ptr_t Data);
__RMP_EXTERN__ rmp_ret_t RMP_Thd_Rcv(rmp_ptr_t* Data,
                                     rmp_ptr_t Slice);

__RMP_EXTERN__ rmp_ret_t RMP_Sem_Crt(volatile struct RMP_Sem* Semaphore,
                                     rmp_ptr_t Number);
__RMP_EXTERN__ rmp_ret_t RMP_Sem_Del(volatile struct RMP_Sem* Semaphore);
__RMP_EXTERN__ rmp_ret_t RMP_Sem_Post(volatile struct RMP_Sem* Semaphore,
                                      rmp_ptr_t Number);
__RMP_EXTERN__ rmp_ret_t RMP_Sem_Post_ISR(volatile struct RMP_Sem* Semaphore,
                                          rmp_ptr_t Number);
__RMP_EXTERN__ rmp_ret_t RMP_Sem_Bcst(volatile struct RMP_Sem* Semaphore);
__RMP_EXTERN__ rmp_ret_t RMP_Sem_Bcst_ISR(volatile struct RMP_Sem* Semaphore);
__RMP_EXTERN__ rmp_ret_t RMP_Sem_Pend(volatile struct RMP_Sem* Semaphore,
                                      rmp_ptr_t Slice);
__RMP_EXTERN__ rmp_ret_t RMP_Sem_Pend_Unlock(volatile struct RMP_Sem* Semaphore,
                                             rmp_ptr_t Slice);
__RMP_EXTERN__ rmp_ret_t RMP_Sem_Abort(volatile struct RMP_Thd* Thread);
__RMP_EXTERN__ rmp_ret_t RMP_Sem_Cnt(volatile struct RMP_Sem* Semaphore);

/* Memory interface - not protected at all */
__RMP_EXTERN__ rmp_ret_t RMP_Mem_Init(volatile void* Pool,
                                      rmp_ptr_t Size);
__RMP_EXTERN__ void* RMP_Malloc(volatile void* Pool,
                                rmp_ptr_t Size);
__RMP_EXTERN__ void RMP_Free(volatile void* Pool,
                             void* Mem_Ptr);
__RMP_EXTERN__ void* RMP_Realloc(volatile void* Pool,
                                 void* Mem_Ptr,
                                 rmp_ptr_t Size);
                             
/* Extended queue interface - not atomic */
__RMP_EXTERN__ rmp_ret_t RMP_Fifo_Crt(volatile struct RMP_Fifo* Fifo);
__RMP_EXTERN__ rmp_ret_t RMP_Fifo_Del(volatile struct RMP_Fifo* Fifo);
__RMP_EXTERN__ rmp_ret_t RMP_Fifo_Read(volatile struct RMP_Fifo* Fifo,
                                       volatile struct RMP_List** Node);
__RMP_EXTERN__ rmp_ret_t RMP_Fifo_Write(volatile struct RMP_Fifo* Fifo,
                                        volatile struct RMP_List* Node);
__RMP_EXTERN__ rmp_ret_t RMP_Fifo_Write_ISR(volatile struct RMP_Fifo* Fifo,
                                            volatile struct RMP_List* Node);
__RMP_EXTERN__ rmp_ret_t RMP_Fifo_Cnt(volatile struct RMP_Fifo* Fifo);

__RMP_EXTERN__ rmp_ret_t RMP_Msgq_Crt(volatile struct RMP_Msgq* Queue);
__RMP_EXTERN__ rmp_ret_t RMP_Msgq_Del(volatile struct RMP_Msgq* Queue);
__RMP_EXTERN__ rmp_ret_t RMP_Msgq_Snd(volatile struct RMP_Msgq* Queue,
                                      volatile struct RMP_List* Node);
__RMP_EXTERN__ rmp_ret_t RMP_Msgq_Snd_ISR(volatile struct RMP_Msgq* Queue,
                                          volatile struct RMP_List* Node);
__RMP_EXTERN__ rmp_ret_t RMP_Msgq_Rcv(volatile struct RMP_Msgq* Queue,
                                      volatile struct RMP_List** Node,
                                      rmp_ptr_t Slice);
__RMP_EXTERN__ rmp_ret_t RMP_Msgq_Cnt(volatile struct RMP_Msgq* Queue);

__RMP_EXTERN__ rmp_ret_t RMP_Bmq_Crt(volatile struct RMP_Bmq* Queue,
                                     rmp_ptr_t Limit);
__RMP_EXTERN__ rmp_ret_t RMP_Bmq_Del(volatile struct RMP_Bmq* Queue);
__RMP_EXTERN__ rmp_ret_t RMP_Bmq_Snd(volatile struct RMP_Bmq* Queue,
                                     volatile struct RMP_List* Node,
                                     rmp_ptr_t Slice);
__RMP_EXTERN__ rmp_ret_t RMP_Bmq_Snd_ISR(volatile struct RMP_Bmq* Queue,
                                         volatile struct RMP_List* Node);
__RMP_EXTERN__ rmp_ret_t RMP_Bmq_Rcv(volatile struct RMP_Bmq* Queue,
                                     volatile struct RMP_List** Node,
                                     rmp_ptr_t Slice);
__RMP_EXTERN__ rmp_ret_t RMP_Bmq_Cnt(volatile struct RMP_Bmq* Queue);
                                     
/* Alarm manager interface - not atomic */
__RMP_EXTERN__ rmp_ret_t RMP_Amgr_Crt(volatile struct RMP_Amgr* Amgr);
__RMP_EXTERN__ rmp_ret_t RMP_Amgr_Del(volatile struct RMP_Amgr* Amgr);
__RMP_EXTERN__ rmp_ret_t RMP_Amgr_Proc(volatile struct RMP_Amgr* Amgr,
                                       rmp_ptr_t Tick);
__RMP_EXTERN__ rmp_ret_t RMP_Amgr_Cnt(volatile struct RMP_Amgr* Amgr);

__RMP_EXTERN__ rmp_ret_t RMP_Alrm_Init(volatile struct RMP_Alrm* Alrm,
                                       rmp_ptr_t Delay,
                                       rmp_ptr_t Mode,
                                       void (*Hook)(volatile struct RMP_Amgr*,
                                                    volatile struct RMP_Alrm*,
                                                    rmp_cnt_t));
__RMP_EXTERN__ rmp_ret_t RMP_Alrm_Set(volatile struct RMP_Amgr* Amgr,
                                      volatile struct RMP_Alrm* Alrm);
__RMP_EXTERN__ rmp_ret_t RMP_Alrm_Clr(volatile struct RMP_Amgr* Amgr,
                                      volatile struct RMP_Alrm* Alrm);
__RMP_EXTERN__ rmp_ret_t RMP_Alrm_Trg(volatile struct RMP_Amgr* Amgr,
                                      volatile struct RMP_Alrm* Alrm);
                                 
/* Mandatory external hook */
RMP_EXTERN void RMP_Init_Hook(void);
RMP_EXTERN void RMP_Init_Idle(void);

/* GUI interface */
#if(RMP_GUI_ENABLE!=0U)
#ifndef RMP_POINT
#error RMP : Point draw function (RMP_POINT) is missing. 
#endif

__RMP_EXTERN__ void RMP_Line(rmp_cnt_t Begin_X,
                             rmp_cnt_t Begin_Y,
                             rmp_cnt_t End_X,
                             rmp_cnt_t End_Y,
                             rmp_ptr_t Color);
__RMP_EXTERN__ void RMP_Dot_Line(rmp_cnt_t Begin_X,
                                 rmp_cnt_t Begin_Y,
                                 rmp_cnt_t End_X,
                                 rmp_cnt_t End_Y,
                                 rmp_ptr_t Dot,
                                 rmp_ptr_t Space);
__RMP_EXTERN__ void RMP_Rectangle(rmp_cnt_t Coord_X,
                                  rmp_cnt_t Coord_Y,
                                  rmp_cnt_t Length,
                                  rmp_cnt_t Width,
                                  rmp_ptr_t Border,
                                  rmp_ptr_t Fill);
__RMP_EXTERN__ void RMP_Round_Rect(rmp_cnt_t Coord_X,
                                   rmp_cnt_t Coord_Y,
                                   rmp_cnt_t Length,
                                   rmp_cnt_t Width,
                                   rmp_cnt_t Round,
                                   rmp_ptr_t Color);
__RMP_EXTERN__ void RMP_Circle(rmp_cnt_t Center_X,
                               rmp_cnt_t Center_Y,
                               rmp_cnt_t Radius,
                               rmp_ptr_t Border,
                               rmp_ptr_t Fill);
__RMP_EXTERN__ void RMP_Matrix(rmp_cnt_t Coord_X,
                               rmp_cnt_t Coord_Y,
                               const rmp_u8_t* Matrix,
                               rmp_ptr_t Bit_Order,
                               rmp_cnt_t Length,
                               rmp_cnt_t Width,
                               rmp_ptr_t Color);
                               
/* Anti-aliasing requires color mixing macro */
#if(RMP_GUI_ANTIALIAS_ENABLE!=0U)
#ifndef RMP_COLOR_25P
#error RMP : 25%-75% mixing function (RMP_COLOR_25P) is missing. 
#endif
#ifndef RMP_COLOR_50P
#error RMP : 50%-50% mixing function (RMP_COLOR_50P) is missing. 
#endif
#ifndef RMP_COLOR_75P
#error RMP : 75%-25% mixing function (RMP_COLOR_75P) is missing. 
#endif

__RMP_EXTERN__ void RMP_Matrix_AA(rmp_cnt_t Coord_X,
                                  rmp_cnt_t Coord_Y,
                                  const rmp_u8_t* Matrix,
                                  rmp_ptr_t Bit_Order,
                                  rmp_cnt_t Length,
                                  rmp_cnt_t Width,
                                  rmp_ptr_t Color,
                                  rmp_ptr_t Back);
#endif

/* Widget requires color macro */
#if(RMP_GUI_WIDGET_ENABLE!=0U)
#ifndef RMP_COLOR_WHITE
#error RMP: White color value (RMP_COLOR_WHITE) is missing. 
#endif
#ifndef RMP_COLOR_LGREY
#error RMP: Lighter grey color value (RMP_COLOR_LGREY) is missing. 
#endif
#ifndef RMP_COLOR_GREY
#error RMP: Grey color value (RMP_COLOR_GREY) is missing. 
#endif
#ifndef RMP_COLOR_DGREY
#error RMP: Darker grey color value (RMP_COLOR_DGREY) is missing. 
#endif
#ifndef RMP_COLOR_DARK
#error RMP: Dark color value (RMP_COLOR_DARK) is missing. 
#endif
#ifndef RMP_COLOR_DDARK
#error RMP: Darker dark color value (RMP_COLOR_DDARK) is missing. 
#endif
#ifndef RMP_COLOR_BLACK
#error RMP: Black color value (RMP_COLOR_BLACK) is missing. 
#endif

__RMP_EXTERN__ void RMP_Cursor(rmp_cnt_t Coord_X,
                               rmp_cnt_t Coord_Y,
                               rmp_ptr_t Style);
__RMP_EXTERN__ void RMP_Checkbox_Set(rmp_cnt_t Coord_X,
                                     rmp_cnt_t Coord_Y,
                                     rmp_cnt_t Length);
__RMP_EXTERN__ void RMP_Checkbox_Clr(rmp_cnt_t Coord_X,
                                     rmp_cnt_t Coord_Y,
                                     rmp_cnt_t Length);
__RMP_EXTERN__ void RMP_Checkbox(rmp_cnt_t Coord_X,
                                 rmp_cnt_t Coord_Y,
                                 rmp_cnt_t Length,
                                 rmp_ptr_t Status);

__RMP_EXTERN__ void RMP_Cmdbtn_Down(rmp_cnt_t Coord_X,
                                    rmp_cnt_t Coord_Y,
                                    rmp_cnt_t Length,
                                    rmp_cnt_t Width);
__RMP_EXTERN__ void RMP_Cmdbtn_Up(rmp_cnt_t Coord_X,
                                  rmp_cnt_t Coord_Y,
                                  rmp_cnt_t Length,
                                  rmp_cnt_t Width);
__RMP_EXTERN__ void RMP_Cmdbtn(rmp_cnt_t Coord_X,
                               rmp_cnt_t Coord_Y,
                               rmp_cnt_t Length,
                               rmp_cnt_t Width,
                               rmp_ptr_t Status);

__RMP_EXTERN__ void RMP_Lineedit_Clr(rmp_cnt_t Coord_Y,
                                     rmp_cnt_t Width,
                                     rmp_cnt_t Clr_X,
                                     rmp_cnt_t Clr_Len);
__RMP_EXTERN__ void RMP_Lineedit(rmp_cnt_t Coord_X,
                                 rmp_cnt_t Coord_Y,
                                 rmp_cnt_t Length,
                                 rmp_cnt_t Width);

__RMP_EXTERN__ void RMP_Radiobtn_Set(rmp_cnt_t Coord_X,
                                     rmp_cnt_t Coord_Y,
                                     rmp_cnt_t Length);
__RMP_EXTERN__ void RMP_Radiobtn_Clr(rmp_cnt_t Coord_X,
                                 rmp_cnt_t Coord_Y,
                                 rmp_cnt_t Length);
__RMP_EXTERN__ void RMP_Radiobtn(rmp_cnt_t Coord_X,
                                 rmp_cnt_t Coord_Y,
                                 rmp_cnt_t Length,
                                 rmp_ptr_t Status);

__RMP_EXTERN__ void RMP_Progbar_Set(rmp_cnt_t Coord_X,
                                    rmp_cnt_t Coord_Y,
                                    rmp_cnt_t Length,
                                    rmp_cnt_t Width,
                                    rmp_cnt_t Style,
                                    rmp_cnt_t Old_Prog,
                                    rmp_cnt_t New_Prog,
                                    rmp_ptr_t Fore,
                                    rmp_ptr_t Back);
__RMP_EXTERN__ void RMP_Progbar(rmp_cnt_t Coord_X,
                                rmp_cnt_t Coord_Y,
                                rmp_cnt_t Length,
                                rmp_cnt_t Width,
                                rmp_cnt_t Style,
                                rmp_cnt_t Prog,
                                rmp_ptr_t Fore,
                                rmp_ptr_t Back);
#endif
#endif
/*****************************************************************************/
/* Undefine "__RMP_EXTERN__" to avoid redefinition */
#undef __RMP_EXTERN__
/* __RMP_KERNEL_MEMBER__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
