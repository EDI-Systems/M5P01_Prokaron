/******************************************************************************
Filename    : rmp_platform_a7m_rvm.h
Author      : pry
Date        : 01/04/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The header of platform-specific part of RMP for RVM hypervisor.
******************************************************************************/

/* Define ********************************************************************/
#ifdef __HDR_DEF__
#ifndef __RMP_PLATFORM_A7M_RVM_DEF__
#define __RMP_PLATFORM_A7M_RVM_DEF__
/*****************************************************************************/
/* Basic Type ****************************************************************/
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
/* End Basic Type ************************************************************/

/* Extended Type *************************************************************/
#ifndef __RMP_PTR_T__
#define __RMP_PTR_T__
/* Pointer */
typedef rmp_u32_t rmp_ptr_t;
#endif

#ifndef __RMP_CNT_T__
#define __RMP_CNT_T__
/* Counter */
typedef rmp_s32_t rmp_cnt_t;
#endif

#ifndef __RMP_RET_T__
#define __RMP_RET_T__
/* Return value */
typedef rmp_s32_t rmp_ret_t;
#endif
/* End Extended Type *********************************************************/

/* System Macro **************************************************************/
/* Compiler "extern" keyword setting */
#define RMP_EXTERN                      extern
/* The order of bits in one CPU machine word */
#define RMP_WORD_ORDER                  (5U)
/* Full descending stack of rmp_ptr_t, 8-byte alignment */
#define RMP_STKSEG_ENABLE               (0U)
#define RMP_STACK_TYPE                  RMP_STACK_FULL_DESCEND
#define RMP_STACK_ALIGN                 (3U)
#define RMP_STACK_ELEM                  rmp_ptr_t
#define RMP_STACK_STRUCT                struct RMP_A7M_RVM_Stack
/* MSB/LSB extraction */
#define RMP_MSB_GET(VAL)                _RMP_A7M_RVM_MSB_Get(VAL)
#define RMP_LSB_GET(VAL)                _RMP_A7M_RVM_LSB_Get(VAL)

/* FPU registers */
#define RMP_FPU                         ((volatile struct RVM_A7M_Cop_Struct*)(RVM_REG->Cop))

/* The CPU and application specific macros are here */
#include "rmp_platform_a7m_rvm_conf.h"

/* Interrupt masking/unmasking */
#define RMP_INT_MASK()                  RVM_Virt_Int_Mask()
#define RMP_INT_UNMASK()                RVM_Virt_Int_Unmask()
/* Yield operations */
#if(RMP_A7M_RVM_FAST_YIELD!=0U)
#define RMP_YIELD()                     _RMP_A7M_RVM_Yield()
#define RMP_YIELD_ISR()                 RVM_Virt_Yield()
#else
#define RMP_YIELD()                     RVM_Virt_Yield()
#define RMP_YIELD_ISR()                 RMP_YIELD()
#endif
/* End System Macro **********************************************************/
/*****************************************************************************/
/* __RMP_PLATFORM_A7M_RVM_DEF__ */
#endif
/* __HDR_DEF__ */
#endif
/* End Define ****************************************************************/

/* Struct ********************************************************************/
#ifdef __HDR_STRUCT__
#ifndef __RMP_PLATFORM_A7M_RVM_STRUCT__
#define __RMP_PLATFORM_A7M_RVM_STRUCT__

/* Use defines in these headers */
#define __HDR_DEF__
#undef __HDR_DEF__
/*****************************************************************************/
struct RMP_A7M_RVM_Stack
{
    rmp_ptr_t Number;
    rmp_ptr_t Param0;
    rmp_ptr_t Param1;
    rmp_ptr_t Param2;
    rmp_ptr_t Param3;
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
/* __RMP_PLATFORM_A7M_RVM_STRUCT__ */
#endif
/* __HDR_STRUCT__ */
#endif
/* End Struct ****************************************************************/

/* Private Variable **********************************************************/
#if(!(defined __HDR_DEF__||defined __HDR_STRUCT__))
#ifndef __RMP_PLATFORM_A7M_RVM_MEMBER__
#define __RMP_PLATFORM_A7M_RVM_MEMBER__

/* In this way we can use the data structures and definitions in the headers */
#define __HDR_DEF__

#undef __HDR_DEF__

#define __HDR_STRUCT__

#undef __HDR_STRUCT__

/* If the header is not used in the public mode */
#ifndef __HDR_PUBLIC__
/*****************************************************************************/
static rvm_ptr_t RMP_Console_Ptr;
RMP_EXTERN const rvm_ptr_t RVM_Desc[];
/*****************************************************************************/
/* End Private Variable ******************************************************/

/* Private Function **********************************************************/ 
/*****************************************************************************/
/* External print in process main file */
#if(RVM_DBGLOG_ENABLE!=0U)
RVM_EXTERN void RVM_Putchar(char Char);
#endif
/* Interrupts */
static void RMP_PendSV_Handler(void);
static void RMP_SysTick_Handler(void);
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

/*****************************************************************************/

/* End Public Variable *******************************************************/

/* Public Function ***********************************************************/
/*****************************************************************************/
__RMP_EXTERN__ void RMP_Int_Enable(void);
__RMP_EXTERN__ void RMP_Int_Disable(void);
RMP_EXTERN void RVM_Virt_Int_Mask(void);
RMP_EXTERN void RVM_Virt_Int_Unmask(void);

RMP_EXTERN rmp_ptr_t _RMP_A7M_RVM_MSB_Get(rmp_ptr_t Value);
RMP_EXTERN rmp_ptr_t _RMP_A7M_RVM_LSB_Get(rmp_ptr_t Value);
RMP_EXTERN void _RMP_Start(rmp_ptr_t Entry,
                           rmp_ptr_t Stack);
RMP_EXTERN void _RMP_A7M_RVM_Yield_Swt(void);
__RMP_EXTERN__ void _RMP_A7M_RVM_Yield_Err(void);
__RMP_EXTERN__ void _RMP_A7M_RVM_Yield(void);
RMP_EXTERN void RVM_Virt_Yield(void);

/* Initialization */
__RMP_EXTERN__ rmp_ptr_t _RMP_Stack_Init(rmp_ptr_t Stack,
                                         rmp_ptr_t Size,
                                         rmp_ptr_t Entry,
                                         rmp_ptr_t Param);
__RMP_EXTERN__ void _RMP_Lowlvl_Init(void);
__RMP_EXTERN__ void RMP_Putchar(char Char);
__RMP_EXTERN__ void _RMP_Plat_Hook(void);
/*****************************************************************************/
/* Undefine "__RMP_EXTERN__" to avoid redefinition */
#undef __RMP_EXTERN__
/* __RMP_PLATFORM_A7M_RVM_MEMBER__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
