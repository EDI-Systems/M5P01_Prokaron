/******************************************************************************
Filename    : platform_cmx_RVM.h
Author      : pry
Date        : 01/04/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The header of platform-specific part of RMP for RVM.
******************************************************************************/

/* Defines *******************************************************************/
#ifdef __HDR_DEFS__
#ifndef __PLATFORM_CMX_RVM_H_DEFS__
#define __PLATFORM_CMX_RVM_H_DEFS__
/*****************************************************************************/
/* Basic Types ***************************************************************/
#if(DEFINE_BASIC_TYPES==TRUE)

#ifndef __S32__
#define __S32__
typedef signed int  s32;
#endif

#ifndef __S16__
#define __S16__
typedef signed short s16;
#endif

#ifndef __S8__
#define __S8__
typedef signed char  s8;
#endif

#ifndef __U32__
#define __U32__
typedef unsigned int  u32;
#endif

#ifndef __U16__
#define __U16__
typedef unsigned short u16;
#endif

#ifndef __U8__
#define __U8__
typedef unsigned char  u8;
#endif

#endif
/* End Basic Types ***********************************************************/

/* Begin Extended Types ******************************************************/
#ifndef __TID_T__
#define __TID_T__
/* The typedef for the Thread ID */
typedef s32 tid_t;
#endif

#ifndef __PTR_T__
#define __PTR_T__
/* The typedef for the pointers - This is the raw style. Pointers must be unsigned */
typedef u32 ptr_t;
#endif

#ifndef __CNT_T__
#define __CNT_T__
/* The typedef for the count variables */
typedef s32 cnt_t;
#endif

#ifndef __CID_T__
#define __CID_T__
/* The typedef for capability ID */
typedef s32 cid_t;
#endif

#ifndef __RET_T__
#define __RET_T__
/* The type for process return value */
typedef s32 ret_t;
#endif
/* End Extended Types ********************************************************/

/* System macros *************************************************************/
/* Compiler "extern" keyword setting */
#define EXTERN                   extern
/* The order of bits in one CPU machine word */
#define RMP_WORD_ORDER           5
/* The maximum length of char printing - no need to change this in most cases */
#define RMP_KERNEL_DEBUG_MAX_STR 128
/* The offset of the stack when initializing */
#define RMP_INIT_STACK           RMP_INIT_STACK_TAIL(17)

/* The CPU and application specific macros are here */
#include "platform_cmx_RVM_conf.h"
/* End System macros *********************************************************/

/* Hypervisor macros *********************************************************/
#define RVM_MAGIC                0x56495254
#define RVM_VECT_BITMAP          ((RVM_MAX_INTVECT-1)/RMP_WORD_SIZE+1)

/* Hypercalls */
/* Enable interrupts */
#define RVM_HYP_ENAINT           0
/* Disable interrupts */
#define RVM_HYP_DISINT           1
/* Register a vector */
#define RVM_HYP_REGEVT           2
/* Delete a vector */
#define RVM_HYP_DELEVT           3
/* Wait for an interrupt to come */
#define RVM_HYP_WAITEVT          4
/* Trigger a vector */
#define RVM_HYP_SENDEVT          5
/* Returning from a vector */
#define RVM_HYP_QUERYEVT         6
/* Query the VM's ID given its name */
#define RVM_HYP_QUERY            7
/* Send to another VM */
#define RVM_HYP_TIMPROG          8
/* Print to the console */
#define RVM_HYP_PRINT            9

/* System stack size when entering the system - do not need to be changed in most cases */
#define RMP_INT_STACK_ADDR           (((ptr_t)&__initial_sp)+0x400-64*sizeof(ptr_t))
    
/* Generic page table flags */
#define RME_PGTBL_ORDER(SIZE,NUM)    (((SIZE)<<(sizeof(ptr_t)*4))|(NUM))
#define RME_PGTBL_READ               (1<<0)
#define RME_PGTBL_WRITE              (1<<1)
#define RME_PGTBL_EXECUTE            (1<<2)
#define RME_PGTBL_CACHEABLE          (1<<3)
#define RME_PGTBL_BUFFERABLE         (1<<4)
#define RME_PGTBL_STATIC             (1<<5)

#define RME_PGTBL_ALL_PERM           (RME_PGTBL_READ|RME_PGTBL_WRITE|RME_PGTBL_EXECUTE| \
                                      RME_PGTBL_CACHEABLE|RME_PGTBL_BUFFERABLE|RME_PGTBL_STATIC)
                                        
/* Generic page size order definitions */
#define RME_PGTBL_SIZE_2B            (1)
#define RME_PGTBL_SIZE_4B            (2)
#define RME_PGTBL_SIZE_8B            (3)
#define RME_PGTBL_SIZE_16B           (4)
#define RME_PGTBL_SIZE_32B           (5)
#define RME_PGTBL_SIZE_64B           (6)
#define RME_PGTBL_SIZE_128B          (7)
#define RME_PGTBL_SIZE_256B          (8)
#define RME_PGTBL_SIZE_512B          (9)
#define RME_PGTBL_SIZE_1K            (10)
#define RME_PGTBL_SIZE_2K            (11)
#define RME_PGTBL_SIZE_4K            (12)
#define RME_PGTBL_SIZE_8K            (13)
#define RME_PGTBL_SIZE_16K           (14)
#define RME_PGTBL_SIZE_32K           (15)
#define RME_PGTBL_SIZE_64K           (16)
#define RME_PGTBL_SIZE_128K          (17)
#define RME_PGTBL_SIZE_256K          (18)
#define RME_PGTBL_SIZE_512K          (19)
#define RME_PGTBL_SIZE_1M            (20)
#define RME_PGTBL_SIZE_2M            (21)
#define RME_PGTBL_SIZE_4M            (22)
#define RME_PGTBL_SIZE_8M            (23)
#define RME_PGTBL_SIZE_16M           (24)
#define RME_PGTBL_SIZE_32M           (25)
#define RME_PGTBL_SIZE_64M           (26)
#define RME_PGTBL_SIZE_128M          (27)
#define RME_PGTBL_SIZE_256M          (28)
#define RME_PGTBL_SIZE_512M          (29)
#define RME_PGTBL_SIZE_1G            (30)
#define RME_PGTBL_SIZE_2G            (31)
#define RME_PGTBL_SIZE_4G            (32)

/* Generic page table entry number definitions */
#define RME_PGTBL_NUM_2             (1)
#define RME_PGTBL_NUM_4             (2)
#define RME_PGTBL_NUM_8             (3)
#define RME_PGTBL_NUM_16            (4)
#define RME_PGTBL_NUM_32            (5)
#define RME_PGTBL_NUM_64            (6)
#define RME_PGTBL_NUM_128           (7)
#define RME_PGTBL_NUM_256           (8)
#define RME_PGTBL_NUM_512           (9)
#define RME_PGTBL_NUM_1K            (10)
#define RME_PGTBL_NUM_2K            (11)
#define RME_PGTBL_NUM_4K            (12)
#define RME_PGTBL_NUM_8K            (13)
#define RME_PGTBL_NUM_16K           (14)
#define RME_PGTBL_NUM_32K           (15)
#define RME_PGTBL_NUM_64K           (16)
#define RME_PGTBL_NUM_128K          (17)
#define RME_PGTBL_NUM_256K          (18)
#define RME_PGTBL_NUM_512K          (19)
#define RME_PGTBL_NUM_1M            (20)
#define RME_PGTBL_NUM_2M            (21)
#define RME_PGTBL_NUM_4M            (22)
/* End Hypervisor macros *****************************************************/
/*****************************************************************************/
/* __PLATFORM_CMX_RVM_H_DEFS__ */
#endif
/* __HDR_DEFS__ */
#endif
/* End Defines ***************************************************************/

/* Structs *******************************************************************/
#ifdef __HDR_STRUCTS__
#ifndef __PLATFORM_CMX_RVM_H_STRUCTS__
#define __PLATFORM_CMX_RVM_H_STRUCTS__

struct RVM_Reg_Struct
{
    ptr_t SP;
    ptr_t R4;
    ptr_t R5;
    ptr_t R6;
    ptr_t R7;
    ptr_t R8;
    ptr_t R9;
    ptr_t R10;
    ptr_t R11;
    ptr_t LR;
};

/* The coprocessor register set structure. In Cortex-M, if there is a 
 * single-precision FPU, then the FPU S0-S15 is automatically pushed */
struct RVM_Cop_Struct
{
    ptr_t S16;
    ptr_t S17;
    ptr_t S18;
    ptr_t S19;
    ptr_t S20;
    ptr_t S21;
    ptr_t S22;
    ptr_t S23;
    ptr_t S24;
    ptr_t S25;
    ptr_t S26;
    ptr_t S27;
    ptr_t S28;
    ptr_t S29;
    ptr_t S30;
    ptr_t S31;
};

struct RVM_Regs
{
    struct RVM_Reg_Struct Reg;
    struct RVM_Cop_Struct Cop_Reg;
};

/* We used structs in the header */
struct RVM_Param
{
    ptr_t Number;
    ptr_t Param[4];
};

/* The page table layout is statically decided, the first one being the top-level */
struct RVM_Hdr_Pgtbl
{
    /* Its parent's position */
    ptr_t Parent;
    /* The start address of the mapping */
    ptr_t Addr;
    /* The size order and number order, combined into one word */
    ptr_t Order;
    /* The initially mapped pages' data. The only thing stored here
     * is the RVM standard flags */
    u8 Flags[8];
};

struct RVM_Int_Flag
{
    ptr_t Flags[RVM_VECT_BITMAP];
};

/* The image structure */
struct RVM_Image
{
    /* The magic number;always equal to VIRT(0x56495254) */
    const ptr_t Magic;
    /* The name is always 16 bytes long. This is fixed and is not changeable */
    s8 Name[16];

    /* The entry, stack and stack size of the virtual machines */
    void* User_Entry;
    void* User_Stack;
    ptr_t User_Size;
    void* Int_Entry;
    void* Int_Stack;
    ptr_t Int_Size;
    
    /* The parameter space, register set space and interrupt flag space */
    struct RVM_Param* Param;
    struct RVM_Regs* Regs;
    struct RVM_Int_Flag* Int_Flags;
    
    /* The console space and size */
    void* Console_Buf;
    ptr_t Console_Size;
    
    /* The priority and timeslices */
    ptr_t Prio;
    ptr_t Slices;

    /* The number of page tables in this image, and where are they stored */
    ptr_t Pgtbl_Num;
    const struct RVM_Hdr_Pgtbl* Pgtbl;

    /* Is there any other images? If there is, here is a pointer to the start
     * of the next one. This is a constant pointer to a constant pointer to a 
     * constant structure */
    const struct RVM_Image* const * const Next_Image;
};

/* Use defines in these headers */
#define __HDR_DEFS__
#undef __HDR_DEFS__
/*****************************************************************************/

/*****************************************************************************/
/* __PLATFORM_CMX_RVM_H_STRUCTS__ */
#endif
/* __HDR_STRUCTS__ */
#endif
/* End Structs ***************************************************************/

/* Private Global Variables **************************************************/
#if(!(defined __HDR_DEFS__||defined __HDR_STRUCTS__))
#ifndef __PLATFORM_CMX_RVM_MEMBERS__
#define __PLATFORM_CMX_RVM_MEMBERS__

/* In this way we can use the data structures and definitions in the headers */
#define __HDR_DEFS__

#undef __HDR_DEFS__

#define __HDR_STRUCTS__

#undef __HDR_STRUCTS__

/* If the header is not used in the public mode */
#ifndef __HDR_PUBLIC_MEMBERS__
/*****************************************************************************/

/*****************************************************************************/
/* End Private Global Variables **********************************************/

/* Private C Function Prototypes *********************************************/ 
/*****************************************************************************/
static ret_t RMP_Hypercall(ptr_t Number, ptr_t Param1, ptr_t Param2, ptr_t Param3, ptr_t Param4);
static ret_t _RMP_Get_Int(void);

/* Interrupts */
static void RMP_PendSV_Handler(void);
static void RMP_SysTick_Handler(void);
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
/* The debug console buffer and pointer */
__EXTERN__ s8 RMP_Console[RMP_KERNEL_DEBUG_MAX_STR];
__EXTERN__ ptr_t RMP_Console_Ptr;

/* The initial stack pointer as defined in assembly */
EXTERN ptr_t RMP_User_Stack;
EXTERN ptr_t __initial_sp;
EXTERN ptr_t RMP_Int_Stack;
/* The hypercall parameter& structure */
__EXTERN__ struct RVM_Param RMP_Param;
__EXTERN__ struct RVM_Regs RMP_Regs;
/* Just map everything into it */
EXTERN const struct RVM_Hdr_Pgtbl RMP_Pgtbl[1];
EXTERN const struct RVM_Image RMP_Image;

/* The interrupt flag used by here */
__EXTERN__ ptr_t RMP_Ctxsw;
/* The interrupt handler registration table */
__EXTERN__ ptr_t RMP_Vect[RVM_MAX_INTVECT];
/* The interrupt flag registration table */
__EXTERN__ struct RVM_Int_Flag RMP_Flag;
/*****************************************************************************/

/* End Public Global Variables ***********************************************/

/* Public C Function Prototypes **********************************************/
/*****************************************************************************/
EXTERN void RMP_Disable_Int(void);
EXTERN void RMP_Enable_Int(void);

EXTERN ptr_t RMP_MSB_Get(ptr_t Val);
EXTERN ptr_t _RMP_Fetch_And(ptr_t* Ptr, ptr_t Operand);
EXTERN void _RMP_Start(ptr_t Entry, ptr_t Stack);
EXTERN void _RMP_Yield(void);
EXTERN void _RMP_Int_Rcv(void);


/* Initialization */
__EXTERN__ void _RMP_Stack_Init(ptr_t Entry, ptr_t Stack, ptr_t Arg);
__EXTERN__ void _RMP_Low_Level_Init(void);
__EXTERN__ void RMP_Putchar(char Char);
__EXTERN__ void _RMP_Plat_Hook(void);

/* Hypercalls */
EXTERN void _RMP_Hypercall(void);
EXTERN void _RMP_Entry(void);

/* Interrupt handler entry */
__EXTERN__ void _RMP_Int(void);

__EXTERN__ void RMP_Wait_Int(void);
__EXTERN__ ret_t RMP_Hyp_Reg_Evt(ptr_t Int_Num, ptr_t VMID);
__EXTERN__ ret_t RMP_Hyp_Del_Evt(cnt_t Evt_ID);
__EXTERN__ ret_t RVM_Hyp_Send_Evt(ptr_t Evt_ID);
__EXTERN__ ret_t RVM_Hyp_Query_Evt(ptr_t VMID);
__EXTERN__ ret_t RVM_Hyp_Query(s8* Name);
__EXTERN__ ret_t RVM_Hyp_Tim_Prog(ptr_t Period);
__EXTERN__ ret_t RVM_Hyp_Print(void);
/*****************************************************************************/
/* Undefine "__EXTERN__" to avoid redefinition */
#undef __EXTERN__
/* __PLATFORM_CMX_RVM_MEMBERS__ */
#endif
/* !(defined __HDR_DEFS__||defined __HDR_STRUCTS__) */
#endif
/* End Public C Function Prototypes ******************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
