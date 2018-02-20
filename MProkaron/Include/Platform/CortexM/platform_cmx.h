/******************************************************************************
Filename    : platform_cmx.h
Author      : pry
Date        : 01/04/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The header of "platform_cmx.h".
******************************************************************************/

/* Defines *******************************************************************/
#ifdef __HDR_DEFS__
#ifndef __PLATFORM_CMX_H_DEFS__
#define __PLATFORM_CMX_H_DEFS__
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
#define RMP_KERNEL_DEBUG_MAX_STR 255
/* The offset of the stack when initializing */
#define RMP_INIT_STACK           RMP_INIT_STACK_TAIL(17)

/* The CPU and application specific macros are here */
#include "platform_cmx_conf.h"
/* End System macros *********************************************************/

/* Cortex-M specific macros **************************************************/
/* MPU operation flag */
#define RMP_CMX_MPU_CLR                 (0)
#define RMP_CMX_MPU_UPD                 (1)
/* MPU definitions */
/* Extract address for/from MPU */
#define RMP_CMX_MPU_ADDR(X)             ((X)&0xFFFFFFE0)
/* Get info from MPU */
#define RMP_CMX_MPU_SZORD(X)            ((((X)&0x3F)>>1)-2)
/* Write info to MPU */
#define RMP_CMX_MPU_VALID               (1<<4)
#define RMP_CMX_MPU_SRDCLR              (0x0000FF00)
#define RMP_CMX_MPU_XN                  (1<<28)
#define RMP_CMX_MPU_RO                  (2<<24)
#define RMP_CMX_MPU_RW                  (3<<24)
#define RMP_CMX_MPU_CACHEABLE           (1<<17)
#define RMP_CMX_MPU_BUFFERABLE          (1<<16)
#define RMP_CMX_MPU_REGIONSIZE(X)       ((X+2)<<1)
#define RMP_CMX_MPU_SZENABLE            (1)
/* Cortex-M (ARMv8) EXC_RETURN values */
#define RMP_CMX_EXC_RET_BASE            (0xFFFFFF80)
/* Whether we are returning to secure stack. 1 means yes, 0 means no */
#define RMP_CMX_EXC_RET_SECURE_STACK    (1<<6)
/* Whether the callee registers are automatically pushed to user stack. 1 means yes, 0 means no */
#define RMP_CMX_EXC_RET_CALLEE_SAVE     (1<<5)
/* Whether the stack frame is standard(contains no FPU data). 1 means yes, 0 means no */
#define RMP_CMX_EXC_RET_STD_FRAME       (1<<4)
/* Are we returning to user mode? 1 means yes, 0 means no */
#define RMP_CMX_EXC_RET_RET_USER        (1<<3)
/* Are we returning to PSP? 1 means yes, 0 means no */
#define RMP_CMX_EXC_RET_RET_PSP         (1<<2)
/* Is this interrupt taken to a secured domain? 1 means yes, 0 means no */
#define RMP_CMX_EXC_INT_SECURE_DOMAIN   (1<<0)
/* FPU type definitions */
#define RMP_CMX_FPU_NONE                (0)
#define RMP_CMX_FPU_VFPV4               (1)
#define RMP_CMX_FPU_FPV5_SP             (2)
#define RMP_CMX_FPU_FPV5_DP             (3)

/* Some useful SCB definitions */
#define RMP_CMX_SHCSR_USGFAULTENA       (1<<18)
#define RMP_CMX_SHCSR_BUSFAULTENA       (1<<17)
#define RMP_CMX_SHCSR_MEMFAULTENA       (1<<16)
/* MPU definitions */
#define RMP_CMX_MPU_PRIVDEF             0x00000004
/* NVIC definitions */
#define RMP_CMX_NVIC_GROUPING_P7S1      0
#define RMP_CMX_NVIC_GROUPING_P6S2      1
#define RMP_CMX_NVIC_GROUPING_P5S3      2
#define RMP_CMX_NVIC_GROUPING_P4S4      3
#define RMP_CMX_NVIC_GROUPING_P3S5      4
#define RMP_CMX_NVIC_GROUPING_P2S6      5
#define RMP_CMX_NVIC_GROUPING_P1S7      6
#define RMP_CMX_NVIC_GROUPING_P0S8      7
/* Fault definitions */
/* The NMI is active */
#define RMP_CMX_ICSR_NMIPENDSET         (((ptr_t)1)<<31)
/* Debug event has occurred. The Debug Fault Status Register has been updated */
#define RMP_CMX_HFSR_DEBUGEVT           (((ptr_t)1)<<31)
/* Processor has escalated a configurable-priority exception to HardFault */
#define RMP_CMX_HFSR_FORCED             (1<<30)
/* Vector table read fault has occurred */
#define RMP_CMX_HFSR_VECTTBL            (1<<1)
/* Divide by zero */
#define RMP_CMX_UFSR_DIVBYZERO          (1<<25)
/* Unaligned load/store access */
#define RMP_CMX_UFSR_UNALIGNED          (1<<24)
/* No such coprocessor */
#define RMP_CMX_UFSR_NOCP               (1<<19)
/* Invalid vector return LR or PC value */
#define RMP_CMX_UFSR_INVPC              (1<<18)
/* Invalid IT instruction or related instructions */
#define RMP_CMX_UFSR_INVSTATE           (1<<17)
/* Invalid IT instruction or related instructions */
#define RMP_CMX_UFSR_UNDEFINSTR         (1<<16)
/* The Bus Fault Address Register is valid */
#define RMP_CMX_BFSR_BFARVALID          (1<<15)
/* The bus fault happened during FP lazy stacking */
#define RMP_CMX_BFSR_LSPERR             (1<<13)
/* A derived bus fault has occurred on exception entry */
#define RMP_CMX_BFSR_STKERR             (1<<12)
/* A derived bus fault has occurred on exception return */
#define RMP_CMX_BFSR_UNSTKERR           (1<<11)
/* Imprecise data access error has occurred */
#define RMP_CMX_BFSR_IMPRECISERR        (1<<10)
/* A precise data access error has occurred, and the processor 
 * has written the faulting address to the BFAR */
#define RMP_CMX_BFSR_PRECISERR          (1<<9)
/* A bus fault on an instruction prefetch has occurred. The 
 * fault is signaled only if the instruction is issued */
#define RMP_CMX_BFSR_IBUSERR            (1<<8)
/* The Memory Mnagement Fault Address Register have valid contents */
#define RMP_CMX_MFSR_MMARVALID          (1<<7)
/* A MemManage fault occurred during FP lazy state preservation */
#define RMP_CMX_MFSR_MLSPERR            (1<<5)
/* A derived MemManage fault occurred on exception entry */
#define RMP_CMX_MFSR_MSTKERR            (1<<4)
/* A derived MemManage fault occurred on exception return */
#define RMP_CMX_MFSR_MUNSTKERR          (1<<3)
/* Data access violation. The MMFAR shows the data address that
 * the load or store tried to access */
#define RMP_CMX_MFSR_DACCVIOL           (1<<1)
/* MPU or Execute Never (XN) default memory map access violation on an
 * instruction fetch has occurred. The fault is signalled only if the
 * instruction is issued */
#define RMP_CMX_MFSR_IACCVIOL           (1<<0)

/* These faults cannot be recovered and will lead to termination immediately */
#define RMP_CMX_FAULT_FATAL             (RMP_CMX_UFSR_DIVBYZERO|RMP_CMX_UFSR_UNALIGNED| \
                                         RMP_CMX_UFSR_NOCP|RMP_CMX_UFSR_INVPC| \
                                         RMP_CMX_UFSR_INVSTATE|RMP_CMX_UFSR_UNDEFINSTR| \
                                         RMP_CMX_BFSR_LSPERR|RMP_CMX_BFSR_STKERR| \
                                         RMP_CMX_BFSR_UNSTKERR|RMP_CMX_BFSR_IMPRECISERR| \
                                         RMP_CMX_BFSR_PRECISERR|RMP_CMX_BFSR_IBUSERR)
/*****************************************************************************/
/* __PLATFORM_CMX_H_DEFS__ */
#endif
/* __HDR_DEFS__ */
#endif
/* End Defines ***************************************************************/

/* Structs *******************************************************************/
#ifdef __HDR_STRUCTS__
#ifndef __PLATFORM_CMX_H_STRUCTS__
#define __PLATFORM_CMX_H_STRUCTS__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEFS__
#undef __HDR_DEFS__
/*****************************************************************************/

/*****************************************************************************/
/* __PLATFORM_CMX_H_STRUCTS__ */
#endif
/* __HDR_STRUCTS__ */
#endif
/* End Structs ***************************************************************/

/* Private Global Variables **************************************************/
#if(!(defined __HDR_DEFS__||defined __HDR_STRUCTS__))
#ifndef __PLATFORM_CMX_MEMBERS__
#define __PLATFORM_CMX_MEMBERS__

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

/*****************************************************************************/

/* End Public Global Variables ***********************************************/

/* Public C Function Prototypes **********************************************/
/*****************************************************************************/
/* Interrupts */
EXTERN void RMP_Disable_Int(void);
EXTERN void RMP_Enable_Int(void);

EXTERN ptr_t RMP_MSB_Get(ptr_t Val);
EXTERN void _RMP_Start(ptr_t Entry, ptr_t Stack);
EXTERN void _RMP_Yield(void);

/* Initialization */
__EXTERN__ void _RMP_Stack_Init(ptr_t Entry, ptr_t Stack, ptr_t Arg);
__EXTERN__ void _RMP_Low_Level_Init(void);
__EXTERN__ void RMP_Putchar(char Char);
__EXTERN__ void _RMP_Plat_Hook(void);
/*****************************************************************************/
/* Undefine "__EXTERN__" to avoid redefinition */
#undef __EXTERN__
/* __PLATFORM_CMX_MEMBERS__ */
#endif
/* !(defined __HDR_DEFS__||defined __HDR_STRUCTS__) */
#endif
/* End Public C Function Prototypes ******************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
