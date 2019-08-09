/******************************************************************************
Filename    : rmp_kernel.h
Author      : pry
Date        : 01/04/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The header file for the kernel.
******************************************************************************/

/* Defines *******************************************************************/
#ifdef __HDR_DEFS__
#ifndef __RMP_KERNEL_H_DEFS__
#define __RMP_KERNEL_H_DEFS__
/*****************************************************************************/
/* Constants */
#define RMP_TRUE                    1
#define RMP_FALSE                   0
/* States of threads */
#define RMP_THD_STATE(X)            ((X)&0xFF)
#define RMP_THD_FLAG(X)             ((X)&~0xFF)
#define RMP_THD_STATE_SET(X,S)      ((X)=(RMP_THD_FLAG(X)|(S)))

/* Memory pool position lookup */
#define RMP_MEM_POS(FLI,SLI)        ((SLI)+((FLI)<<3))

/* This thread is currently unused */
#define RMP_THD_FREE                (0)
/* This thread is currently running */
#define RMP_THD_RUNNING             (1)
/* Blocked on a send endpoint */
#define RMP_THD_SNDBLK              (2)
/* Blocked on a send endpoint with a timeout */
#define RMP_THD_SNDDLY              (3)
/* Blocked on its own receive endpoint */
#define RMP_THD_RCVBLK              (4)
/* Blocked on its own receive endpoint with a timeout */
#define RMP_THD_RCVDLY              (5)
/* Just on the timer delay */
#define RMP_THD_DELAYED             (6)
/* Blocked on a semaphore */
#define RMP_THD_SEMBLK              (7)
/* Blocked on a semaphore with a timeout */
#define RMP_THD_SEMDLY              (8)
/* Suspended */
#define RMP_THD_SUSPENDED           (1<<8)
/* Mailbox valid */
#define RMP_THD_MBOXFUL             (RMP_THD_SUSPENDED<<1)
    
/* States of semaphores */
#define RMP_SEM_FREE                (0)
#define RMP_SEM_USED                (1)

/* States of memory blocks */
#define RMP_MEM_FREE                (0)
#define RMP_MEM_USED                (1)

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

/* Power and rounding */
#define RMP_POW2(POW)               (((rmp_ptr_t)1)<<(POW))
#define RMP_ROUND_DOWN(NUM,POW)     (((NUM)>>(POW))<<(POW))
#define RMP_ROUND_UP(NUM,POW)       RMP_ROUND_DOWN((NUM)+RMP_POW2(POW)-1,POW)

/* Word sizes settings */
#define RMP_ALLBITS                 ((rmp_ptr_t)(-1))
#define RMP_WORD_SIZE               RMP_POW2(RMP_WORD_ORDER)
#define RMP_WORD_MASK               (~(RMP_ALLBITS<<RMP_WORD_ORDER))
#define RMP_ALIGN_ORDER             (RMP_WORD_ORDER-3)
#define RMP_ALIGN_MASK              (~(RMP_ALLBITS<<RMP_ALIGN_ORDER))
#define RMP_BITMAP_SIZE             ((RMP_MAX_PREEMPT_PRIO-1)/RMP_WORD_SIZE+1)

/* Stack offset macros */
/* Head offset, for ascending stacks */
#define RMP_INIT_STACK_HEAD(X)      (((rmp_ptr_t)RMP_Init_Stack)+(X)*sizeof(rmp_ptr_t))
/* Tail offset, for descending stacks */
#define RMP_INIT_STACK_TAIL(X)      (((rmp_ptr_t)RMP_Init_Stack)+RMP_INIT_STACK_SIZE-(X)*sizeof(rmp_ptr_t))

/* Get the thread from delay list */
#define RMP_DLY2THD(X)              ((struct RMP_Thd*)(((rmp_ptr_t)(X))-sizeof(struct RMP_List)))

/* Printk macros */
#define RMP_PRINTK_I(INT)           RMP_Print_Int((INT))
#define RMP_PRINTK_U(UINT)          RMP_Print_Uint((UINT))
#define RMP_PRINTK_S(STR)           RMP_Print_String((rmp_s8_t*)(STR))

/* Built-in graphics */
#ifdef RMP_POINT
#define RMP_TRANS                   (0x01)
#define RMP_MAT_SMALL               (0)
#define RMP_MAT_BIG                 (1)
#define RMP_MAT_BPOS(MAT,POS)       ((MAT)[(POS)>>3]&(1<<(7-((POS)&0x07))))
#define RMP_MAT_SPOS(MAT,POS)       ((MAT)[(POS)>>3]&(1<<((POS)&0x07)))
#define RMP_CBOX_CHECK              (1)
#define RMP_CBTN_DOWN               (1)
#define RMP_RBTN_SEL                (1)
#define RMP_PBAR_L2R                (0)
#define RMP_PBAR_D2U                (1)
#define RMP_PBAR_R2L                (2)
#define RMP_PBAR_U2D                (3)

#define RMP_CUR_NORM                (0)
#define RMP_CUR_BUSY                (1)
#define RMP_CUR_QUESTION            (2)
#define RMP_CUR_HAND                (3)
#define RMP_CUR_TEXT                (4)
#define RMP_CUR_STOP                (5)
#define RMP_CUR_MOVE                (6)
#define RMP_CUR_LR                  (7)
#define RMP_CUR_UD                  (8)
#define RMP_CUR_ULBR                (9)
#define RMP_CUR_URBL                (10)
#define RMP_CUR_CROSS               (11)
#endif
    
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
    
/* Coverage switch - not to be enabled in most cases; user should not touch this */
/* #define RMP_COVERAGE */

/* Test marker macro */
#ifdef RMP_COVERAGE
#define RMP_COVERAGE_LINES          (6144)
#define RMP_COVERAGE_MARKER() \
do \
{ \
    RMP_Coverage[__LINE__]++; \
    RMP_Coverage[0]=RMP_Coverage[__LINE__]; \
} \
while(0)
#else
#define RMP_COVERAGE_MARKER() \
do \
{ \
    \
} \
while(0)
#endif
/*****************************************************************************/
/* __RMP_KERNEL_H_DEFS__ */
#endif
/* __HDR_DEFS__ */
#endif
/* End Defines ***************************************************************/

/* Structs *******************************************************************/
#ifdef __HDR_STRUCTS__
#ifndef __RMP_KERNEL_H_STRUCTS__
#define __RMP_KERNEL_H_STRUCTS__
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
    rmp_ptr_t Slices;
    /* If it is running, how many ticks does it have remaining */
    rmp_ptr_t Slices_Left;
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
    /* The start address of the actual memory pool */
    rmp_ptr_t Start;
    /* The size of this pool, including the header, bitmap and list table */
    rmp_ptr_t Size;
    /* The location of the list table itself */
    struct RMP_List* Table;
    /* The bitmap - This is actually an array that have an indefinite length, and will
     * be decided at runtime. Don't fuss if lint says that this can overflow; it is safe. */
    rmp_ptr_t Bitmap[1];
};
/*****************************************************************************/
/* __RMP_KERNEL_H_STRUCTS__ */
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
#ifdef RMP_COVERAGE
/* For coverage use only */
static volatile rmp_ptr_t RMP_Coverage[RMP_COVERAGE_LINES];
#endif
/* The scheduler bitmap */
static volatile rmp_ptr_t RMP_Bitmap[RMP_BITMAP_SIZE];
static volatile struct RMP_List RMP_Run[RMP_MAX_PREEMPT_PRIO];
static volatile struct RMP_List RMP_Delay;

/* Scheduler lock */
static volatile rmp_ptr_t RMP_Sched_Lock_Cnt;
static volatile rmp_ptr_t RMP_Sched_Locked;
/* Scheduler pending */
static volatile rmp_ptr_t RMP_Sched_Pend;
/* Timer events pending */
static volatile rmp_ptr_t RMP_Timer_Pend;

/* Init thread */
static volatile rmp_ptr_t RMP_Init_Stack[RMP_INIT_STACK_SIZE>>(RMP_WORD_ORDER-3)];
static volatile struct RMP_Thd RMP_Init_Thd;
/*****************************************************************************/
/* End Private Global Variables **********************************************/

/* Private C Function Prototypes *********************************************/ 
/*****************************************************************************/
static void _RMP_Set_Rdy(volatile struct RMP_Thd* Thread);
static void _RMP_Clr_Rdy(volatile struct RMP_Thd* Thread);
static void _RMP_Dly_Ins(volatile struct RMP_Thd* Thread, rmp_ptr_t Slices);
static void _RMP_Timer_Proc(void);
static void _RMP_Mem_Block(volatile struct RMP_Mem_Head* Addr, rmp_ptr_t Size);
static void _RMP_Mem_Ins(volatile void* Pool, volatile struct RMP_Mem_Head* Mem_Head);
static void _RMP_Mem_Del(volatile void* Pool, volatile struct RMP_Mem_Head* Mem_Head);
static rmp_ret_t _RMP_Mem_Search(volatile void* Pool, rmp_ptr_t Size, rmp_cnt_t* FLI_Level, rmp_cnt_t* SLI_Level);

#ifdef RMP_CTL_WHITE
#ifdef RMP_CTL_LGREY
#ifdef RMP_CTL_GREY
#ifdef RMP_CTL_DGREY
#ifdef RMP_CTL_DARK
#ifdef RMP_CTL_DDARK
#ifdef RMP_CTL_BLACK
static void RMP_Radiobtn_Circle(rmp_cnt_t Coord_X,rmp_cnt_t Coord_Y,rmp_cnt_t Length);
static void RMP_Progbar_Prog(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_cnt_t Width,
                             rmp_cnt_t Style, rmp_cnt_t Prog, rmp_ptr_t Fore, rmp_ptr_t Back);
#endif
#endif
#endif
#endif
#endif
#endif
#endif
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
/* The current tick counter value - can be read by the application to determine their time */
__EXTERN__ volatile rmp_ptr_t RMP_Tick;
/* The current thread - the pointer itself is volatile but not its contents */
__EXTERN__ struct RMP_Thd* volatile RMP_Cur_Thd;
__EXTERN__ volatile rmp_ptr_t RMP_Cur_SP;
/*****************************************************************************/

/* End Public Global Variables ***********************************************/

/* Public C Function Prototypes **********************************************/
/*****************************************************************************/
/* This is the entry of user applications */
EXTERN void RMP_Init(void);
__EXTERN__ void _RMP_Get_High_Rdy(void);
__EXTERN__ void _RMP_Tick_Handler(rmp_ptr_t Ticks);
__EXTERN__ rmp_ptr_t _RMP_Get_Near_Ticks(void);
__EXTERN__ void RMP_Clear(volatile void* Addr, rmp_ptr_t Size);

/* Some helpers */
__EXTERN__ rmp_ptr_t RMP_RBIT_Get(rmp_ptr_t Val);
__EXTERN__ rmp_ptr_t RMP_LSB_Get(rmp_ptr_t Val);

/* Scheduler locking & unlocking */
__EXTERN__ void RMP_Lock_Sched(void);
__EXTERN__ void RMP_Unlock_Sched(void);

/* Debug printing functions */
__EXTERN__ rmp_cnt_t RMP_Print_Int(rmp_cnt_t Int);
__EXTERN__ rmp_cnt_t RMP_Print_Uint(rmp_ptr_t Uint);
__EXTERN__ rmp_cnt_t RMP_Print_String(rmp_s8_t* String);

/* List operation functions */
__EXTERN__ void RMP_List_Crt(volatile struct RMP_List* Head);
__EXTERN__ void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next);
__EXTERN__ void RMP_List_Ins(volatile struct RMP_List* New,
                             volatile struct RMP_List* Prev,
                             volatile struct RMP_List* Next);

/* System interfaces */
__EXTERN__ void RMP_Yield(void);
__EXTERN__ rmp_ret_t RMP_Thd_Crt(volatile struct RMP_Thd* Thread, 
                             void* Entry, void* Stack, void* Arg, 
                             rmp_ptr_t Prio, rmp_ptr_t Slices);
__EXTERN__ rmp_ret_t RMP_Thd_Del(volatile struct RMP_Thd* Thread);
__EXTERN__ rmp_ret_t RMP_Thd_Set(volatile struct RMP_Thd* Thread, rmp_ptr_t Prio, rmp_ptr_t Slices);
__EXTERN__ rmp_ret_t RMP_Thd_Suspend(volatile struct RMP_Thd* Thread);
__EXTERN__ rmp_ret_t RMP_Thd_Resume(volatile struct RMP_Thd* Thread);

__EXTERN__ rmp_ret_t RMP_Thd_Delay(rmp_ptr_t Slices);
__EXTERN__ rmp_ret_t RMP_Thd_Cancel(volatile struct RMP_Thd* Thread);

__EXTERN__ rmp_ret_t RMP_Thd_Snd(volatile struct RMP_Thd* Thread, rmp_ptr_t Data, rmp_ptr_t Slices);
__EXTERN__ rmp_ret_t RMP_Thd_Snd_ISR(volatile struct RMP_Thd* Thread, rmp_ptr_t Data);
__EXTERN__ rmp_ret_t RMP_Thd_Rcv(rmp_ptr_t* Data, rmp_ptr_t Slices);

__EXTERN__ rmp_ret_t RMP_Sem_Crt(volatile struct RMP_Sem* Semaphore, rmp_ptr_t Number);
__EXTERN__ rmp_ret_t RMP_Sem_Del(volatile struct RMP_Sem* Semaphore);
__EXTERN__ rmp_ret_t RMP_Sem_Pend(volatile struct RMP_Sem* Semaphore, rmp_ptr_t Slices);
__EXTERN__ rmp_ret_t RMP_Sem_Abort(volatile struct RMP_Thd* Thread);
__EXTERN__ rmp_ret_t RMP_Sem_Post(volatile struct RMP_Sem* Semaphore, rmp_ptr_t Number);
__EXTERN__ rmp_ret_t RMP_Sem_Post_ISR(volatile struct RMP_Sem* Semaphore, rmp_ptr_t Number);
                             
/* Memory interfaces */
__EXTERN__ rmp_ret_t RMP_Mem_Init(volatile void* Pool, rmp_ptr_t Size);
__EXTERN__ void* RMP_Malloc(volatile void* Pool, rmp_ptr_t Size);
__EXTERN__ void RMP_Free(volatile void* Pool, void* Mem_Ptr);
__EXTERN__ void* RMP_Realloc(volatile void* Pool, void* Mem_Ptr, rmp_ptr_t Size);

/* Built-in graphics */
#ifdef RMP_POINT
EXTERN void RMP_POINT(rmp_cnt_t,rmp_cnt_t,rmp_ptr_t);
__EXTERN__ void RMP_Line(rmp_cnt_t Start_X, rmp_cnt_t Start_Y, rmp_cnt_t End_X, rmp_cnt_t End_Y, rmp_ptr_t Color);
__EXTERN__ void RMP_Dot_Line(rmp_cnt_t Start_X, rmp_cnt_t Start_Y, rmp_cnt_t End_X,rmp_cnt_t End_Y, rmp_ptr_t Dot, rmp_ptr_t Space);
__EXTERN__ void RMP_Rectangle(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_cnt_t Width, rmp_ptr_t Border, rmp_ptr_t Fill);
__EXTERN__ void RMP_Round_Rect(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y,
                               rmp_cnt_t Length, rmp_cnt_t Width, rmp_cnt_t Round, rmp_ptr_t Color);
__EXTERN__ void RMP_Circle(rmp_cnt_t Center_X, rmp_cnt_t Center_Y, rmp_cnt_t Radius, rmp_ptr_t Border, rmp_ptr_t Fill);
__EXTERN__ void RMP_Matrix(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, const rmp_u8_t* Matrix,
                           rmp_cnt_t Bit_Order, rmp_cnt_t Length, rmp_cnt_t Width, rmp_ptr_t Color);
/* This is only exported when color mixing macros available */
#ifdef RMP_COLOR_25P
#ifdef RMP_COLOR_50P
#ifdef RMP_COLOR_75P
__EXTERN__ void RMP_Matrix_AA(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, const rmp_u8_t* Matrix,
                              rmp_cnt_t Bit_Order, rmp_cnt_t Length, rmp_cnt_t Width, rmp_ptr_t Color, rmp_ptr_t Back);
#endif
#endif
#endif
/* These are only provided when all used colors are predefined */
#ifdef RMP_CTL_WHITE
#ifdef RMP_CTL_LGREY
#ifdef RMP_CTL_GREY
#ifdef RMP_CTL_DGREY
#ifdef RMP_CTL_DARK
#ifdef RMP_CTL_DDARK
#ifdef RMP_CTL_BLACK
/* Built-in easy controls */
__EXTERN__ void RMP_Cursor(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_ptr_t Style);
__EXTERN__ void RMP_Checkbox_Set(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length);
__EXTERN__ void RMP_Checkbox_Clr(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length);
__EXTERN__ void RMP_Checkbox(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_ptr_t Status);

__EXTERN__ void RMP_Cmdbtn_Down(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_cnt_t Width);
__EXTERN__ void RMP_Cmdbtn_Up(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_cnt_t Width);
__EXTERN__ void RMP_Cmdbtn(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_cnt_t Width, rmp_ptr_t Status);

__EXTERN__ void RMP_Lineedit_Clr(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length,
                                 rmp_cnt_t Width, rmp_cnt_t Clr_X, rmp_cnt_t Clr_Len);
__EXTERN__ void RMP_Lineedit(rmp_cnt_t Coord_X,rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_cnt_t Width);

__EXTERN__ void RMP_Radiobtn_Set(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length);
__EXTERN__ void RMP_Radiobtn_Clr(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length);
__EXTERN__ void RMP_Radiobtn(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_ptr_t Status);

__EXTERN__ void RMP_Progbar_Set(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_cnt_t Width,
                                rmp_cnt_t Style, rmp_cnt_t Old_Prog, rmp_cnt_t New_Prog, rmp_ptr_t Fore, rmp_ptr_t Back);
__EXTERN__ void RMP_Progbar(rmp_cnt_t Coord_X, rmp_cnt_t Coord_Y, rmp_cnt_t Length, rmp_cnt_t Width,
                            rmp_cnt_t Style, rmp_cnt_t Prog, rmp_ptr_t Fore, rmp_ptr_t Back);
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif

/* Other utilities */
#ifdef __RMP_U8_T__
#ifdef __RMP_U16_T__
__EXTERN__ rmp_ptr_t RMP_CRC16(const rmp_u8_t* Data, rmp_ptr_t Length);
#endif
#endif

/* Hook functions */
#if(RMP_USE_HOOKS==RMP_TRUE)
    EXTERN void RMP_Start_Hook(void);
    EXTERN void RMP_Save_Ctx(void);
    EXTERN void RMP_Load_Ctx(void);
    EXTERN void RMP_Sched_Hook(void);
    EXTERN void RMP_Tick_Hook(rmp_ptr_t Ticks);
#else
    __EXTERN__ void RMP_Save_Ctx(void);
    __EXTERN__ void RMP_Load_Ctx(void);
#endif

EXTERN void RMP_Init_Hook(void);
EXTERN void RMP_Init_Idle(void);

/* Coverage test */
#ifdef RMP_COVERAGE
__EXTERN__ void RMP_Print_Coverage(void);
#endif
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
