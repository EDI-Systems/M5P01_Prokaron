/******************************************************************************
Filename    : rmp_platform_a7m.h
Author      : pry
Date        : 01/04/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_a7m.c".
******************************************************************************/

/* Defines *******************************************************************/
#ifdef __HDR_DEFS__
#ifndef __RMP_PLATFORM_A7M_H_DEFS__
#define __RMP_PLATFORM_A7M_H_DEFS__
/*****************************************************************************/
/* Basic Types ***************************************************************/
#ifndef __RMP_S32_T__
#define __RMP_S32_T__
typedef signed int rmp_s32_t;
#endif

#ifndef __RMP_S16_T__
#define __RMP_S16_T__
typedef signed short rmp_s16_t;
#endif

#ifndef __RMP_S8_T__
#define __RMP_S8_T__
typedef signed char rmp_s8_t;
#endif

#ifndef __RMP_U32_T__
#define __RMP_U32_T__
typedef unsigned int rmp_u32_t;
#endif

#ifndef __RMP_U16_T__
#define __RMP_U16_T__
typedef unsigned short rmp_u16_t;
#endif

#ifndef __RMP_U8_T__
#define __RMP_U8_T__
typedef unsigned char rmp_u8_t;
#endif
/* End Basic Types ***********************************************************/

/* Begin Extended Types ******************************************************/
#ifndef __RMP_PTR_T__
#define __RMP_PTR_T__
/* The typedef for the pointers - This is the raw style. Pointers must be unsigned */
typedef rmp_u32_t rmp_ptr_t;
#endif

#ifndef __RMP_CNT_T__
#define __RMP_CNT_T__
/* The typedef for the count variables */
typedef rmp_s32_t rmp_cnt_t;
#endif

#ifndef __RMP_RET_T__
#define __RMP_RET_T__
/* The type for process return value */
typedef rmp_s32_t rmp_ret_t;
#endif
/* End Extended Types ********************************************************/

/* System macros *************************************************************/
/* Compiler "extern" keyword setting */
#define EXTERN                          extern
/* The order of bits in one CPU machine word */
#define RMP_WORD_ORDER                  (5U)
/* The maximum length of char printing - no need to change this in most cases */
#define RMP_DEBUG_PRINT_MAX             (255U)
/* Descending stack, 8-byte alignment */
#define RMP_INIT_STACK                  RMP_INIT_STACK_DESCEND(3U)
/* MSB/LSB extraction */
#define RMP_MSB_GET(VAL)                _RMP_A7M_MSB_Get(VAL)
#define RMP_LSB_GET(VAL)                _RMP_A7M_LSB_Get(VAL)

/* The CPU and application specific macros are here */
#include "rmp_platform_a7m_conf.h"
/* End System macros *********************************************************/

/* Cortex-M specific macros **************************************************/
/* MPU operation flag */
#define RMP_A7M_MPU_CLR                 (0)
#define RMP_A7M_MPU_UPD                 (1)
/* MPU definitions */
/* Extract address for/from MPU */
#define RMP_A7M_MPU_ADDR(X)             ((X)&0xFFFFFFE0)
/* Get info from MPU */
#define RMP_A7M_MPU_SZORD(X)            ((((X)&0x3F)>>1)-2)
/* Write info to MPU */
#define RMP_A7M_MPU_VALID               (1<<4)
#define RMP_A7M_MPU_SRDCLR              (0x0000FF00)
#define RMP_A7M_MPU_XN                  (1<<28)
#define RMP_A7M_MPU_RO                  (2<<24)
#define RMP_A7M_MPU_RW                  (3<<24)
#define RMP_A7M_MPU_CACHEABLE           (1<<17)
#define RMP_A7M_MPU_BUFFERABLE          (1<<16)
#define RMP_A7M_MPU_REGIONSIZE(X)       ((X+2)<<1)
#define RMP_A7M_MPU_SZENABLE            (1)
/* Cortex-M (ARMv8) EXC_RETURN values */
#define RMP_A7M_EXC_RET_BASE            (0xFFFFFF80)
/* Whether we are returning to secure stack. 1 means yes, 0 means no */
#define RMP_A7M_EXC_RET_SECURE_STACK    (1<<6)
/* Whether the callee registers are automatically pushed to user stack. 1 means yes, 0 means no */
#define RMP_A7M_EXC_RET_CALLEE_SAVE     (1<<5)
/* Whether the stack frame is standard(contains no FPU data). 1 means yes, 0 means no */
#define RMP_A7M_EXC_RET_STD_FRAME       (1<<4)
/* Are we returning to user mode? 1 means yes, 0 means no */
#define RMP_A7M_EXC_RET_RET_USER        (1<<3)
/* Are we returning to PSP? 1 means yes, 0 means no */
#define RMP_A7M_EXC_RET_RET_PSP         (1<<2)
/* Is this interrupt taken to a secured domain? 1 means yes, 0 means no */
#define RMP_A7M_EXC_INT_SECURE_DOMAIN   (1<<0)
/* FPU type definitions */
#define RMP_A7M_FPU_NONE                (0)
#define RMP_A7M_FPU_VFPV4               (1)
#define RMP_A7M_FPU_FPV5_SP             (2)
#define RMP_A7M_FPU_FPV5_DP             (3)

/* Some useful SCB definitions */
#define RMP_A7M_SHCSR_USGFAULTENA       (1<<18)
#define RMP_A7M_SHCSR_BUSFAULTENA       (1<<17)
#define RMP_A7M_SHCSR_MEMFAULTENA       (1<<16)
/* MPU definitions */
#define RMP_A7M_MPU_PRIVDEF             0x00000004
/* NVIC definitions */
#define RMP_A7M_NVIC_GROUPING_P7S1      0
#define RMP_A7M_NVIC_GROUPING_P6S2      1
#define RMP_A7M_NVIC_GROUPING_P5S3      2
#define RMP_A7M_NVIC_GROUPING_P4S4      3
#define RMP_A7M_NVIC_GROUPING_P3S5      4
#define RMP_A7M_NVIC_GROUPING_P2S6      5
#define RMP_A7M_NVIC_GROUPING_P1S7      6
#define RMP_A7M_NVIC_GROUPING_P0S8      7
/* Fault definitions */
/* The NMI is active */
#define RMP_A7M_ICSR_NMIPENDSET         (((rmp_ptr_t)1)<<31)
/* Debug event has occurred. The Debug Fault Status Register has been updated */
#define RMP_A7M_HFSR_DEBUGEVT           (((rmp_ptr_t)1)<<31)
/* Processor has escalated a configurable-priority exception to HardFault */
#define RMP_A7M_HFSR_FORCED             (1<<30)
/* Vector table read fault has occurred */
#define RMP_A7M_HFSR_VECTTBL            (1<<1)
/* Divide by zero */
#define RMP_A7M_UFSR_DIVBYZERO          (1<<25)
/* Unaligned load/store access */
#define RMP_A7M_UFSR_UNALIGNED          (1<<24)
/* No such coprocessor */
#define RMP_A7M_UFSR_NOCP               (1<<19)
/* Invalid vector return LR or PC value */
#define RMP_A7M_UFSR_INVPC              (1<<18)
/* Invalid IT instruction or related instructions */
#define RMP_A7M_UFSR_INVSTATE           (1<<17)
/* Invalid IT instruction or related instructions */
#define RMP_A7M_UFSR_UNDEFINSTR         (1<<16)
/* The Bus Fault Address Register is valid */
#define RMP_A7M_BFSR_BFARVALID          (1<<15)
/* The bus fault happened during FP lazy stacking */
#define RMP_A7M_BFSR_LSPERR             (1<<13)
/* A derived bus fault has occurred on exception entry */
#define RMP_A7M_BFSR_STKERR             (1<<12)
/* A derived bus fault has occurred on exception return */
#define RMP_A7M_BFSR_UNSTKERR           (1<<11)
/* Imprecise data access error has occurred */
#define RMP_A7M_BFSR_IMPRECISERR        (1<<10)
/* A precise data access error has occurred, and the processor 
 * has written the faulting address to the BFAR */
#define RMP_A7M_BFSR_PRECISERR          (1<<9)
/* A bus fault on an instruction prefetch has occurred. The 
 * fault is signaled only if the instruction is issued */
#define RMP_A7M_BFSR_IBUSERR            (1<<8)
/* The Memory Mnagement Fault Address Register have valid contents */
#define RMP_A7M_MFSR_MMARVALID          (1<<7)
/* A MemManage fault occurred during FP lazy state preservation */
#define RMP_A7M_MFSR_MLSPERR            (1<<5)
/* A derived MemManage fault occurred on exception entry */
#define RMP_A7M_MFSR_MSTKERR            (1<<4)
/* A derived MemManage fault occurred on exception return */
#define RMP_A7M_MFSR_MUNSTKERR          (1<<3)
/* Data access violation. The MMFAR shows the data address that
 * the load or store tried to access */
#define RMP_A7M_MFSR_DACCVIOL           (1<<1)
/* MPU or Execute Never (XN) default memory map access violation on an
 * instruction fetch has occurred. The fault is signalled only if the
 * instruction is issued */
#define RMP_A7M_MFSR_IACCVIOL           (1<<0)

/* These faults cannot be recovered and will lead to termination immediately */
#define RMP_A7M_FAULT_FATAL             (RMP_A7M_UFSR_DIVBYZERO|RMP_A7M_UFSR_UNALIGNED| \
                                         RMP_A7M_UFSR_NOCP|RMP_A7M_UFSR_INVPC| \
                                         RMP_A7M_UFSR_INVSTATE|RMP_A7M_UFSR_UNDEFINSTR| \
                                         RMP_A7M_BFSR_LSPERR|RMP_A7M_BFSR_STKERR| \
                                         RMP_A7M_BFSR_UNSTKERR|RMP_A7M_BFSR_IMPRECISERR| \
                                         RMP_A7M_BFSR_PRECISERR|RMP_A7M_BFSR_IBUSERR)
/*****************************************************************************/
/* __RMP_PLATFORM_A7M_H_DEFS__ */
#endif
/* __HDR_DEFS__ */
#endif
/* End Defines ***************************************************************/

/* Structs *******************************************************************/
#ifdef __HDR_STRUCTS__
#ifndef __RMP_PLATFORM_A7M_H_STRUCTS__
#define __RMP_PLATFORM_A7M_H_STRUCTS__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEFS__
#undef __HDR_DEFS__
/*****************************************************************************/
struct RMP_A7M_Stack
{
    rmp_ptr_t R4;
    rmp_ptr_t R5;
    rmp_ptr_t R6;
    rmp_ptr_t R7;
    rmp_ptr_t R8;
    rmp_ptr_t R9;
    rmp_ptr_t R10;
    rmp_ptr_t R11;
    rmp_ptr_t LR_EXC;
    rmp_ptr_t R0;
    rmp_ptr_t R1;
    rmp_ptr_t R2;
    rmp_ptr_t R3;
    rmp_ptr_t R12;
    rmp_ptr_t LR;
    rmp_ptr_t PC;
    rmp_ptr_t XPSR;
};
/*****************************************************************************/
/* __RMP_PLATFORM_A7M_H_STRUCTS__ */
#endif
/* __HDR_STRUCTS__ */
#endif
/* End Structs ***************************************************************/

/* Private Global Variables **************************************************/
#if(!(defined __HDR_DEFS__||defined __HDR_STRUCTS__))
#ifndef __RMP_PLATFORM_A7M_MEMBERS__
#define __RMP_PLATFORM_A7M_MEMBERS__

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
EXTERN void RMP_Int_Disable(void);
EXTERN void RMP_Int_Enable(void);
EXTERN void RMP_Int_Mask(rmp_ptr_t Level);

EXTERN rmp_ptr_t _RMP_A7M_MSB_Get(rmp_ptr_t Value);
EXTERN rmp_ptr_t _RMP_A7M_LSB_Get(rmp_ptr_t Value);
EXTERN void _RMP_Start(rmp_ptr_t Entry, rmp_ptr_t Stack);
EXTERN void _RMP_Yield(void);

/* Initialization */
__EXTERN__ rmp_ptr_t _RMP_Stack_Init(rmp_ptr_t Stack,
                                     rmp_ptr_t Size,
                                     rmp_ptr_t Entry,
                                     rmp_ptr_t Param);
__EXTERN__ void _RMP_Lowlvl_Init(void);
__EXTERN__ void RMP_Putchar(char Char);
__EXTERN__ void _RMP_Plat_Hook(void);
/*****************************************************************************/
/* Undefine "__EXTERN__" to avoid redefinition */
#undef __EXTERN__
/* __RMP_PLATFORM_A7M_MEMBERS__ */
#endif
/* !(defined __HDR_DEFS__||defined __HDR_STRUCTS__) */
#endif
/* End Public C Function Prototypes ******************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
