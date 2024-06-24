/******************************************************************************
Filename    : rmp_platform_a7m.h
Author      : pry
Date        : 01/04/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_a7m.c".
******************************************************************************/

/* Define ********************************************************************/
#ifdef __HDR_DEF__
#ifndef __RMP_PLATFORM_A7M_DEF__
#define __RMP_PLATFORM_A7M_DEF__
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
/* The maximum length of char printing */
#define RMP_DEBUG_PRINT_MAX             (255U)
/* Full descending stack of rmp_ptr_t, 8-byte alignment */
#define RMP_STACK_TYPE                  RMP_STACK_FULL_DESCEND
#define RMP_STACK_ALIGN                 (3U)
#define RMP_STACK_ELEM                  rmp_ptr_t
#define RMP_STACK_STRUCT                struct RMP_A7M_Stack
/* MSB/LSB extraction */
#define RMP_MSB_GET(VAL)                _RMP_A7M_MSB_Get(VAL)
#define RMP_LSB_GET(VAL)                _RMP_A7M_LSB_Get(VAL)

/* The CPU and application specific macros are here */
#include "rmp_platform_a7m_conf.h"

/* Interrupt masking/unmasking */
#define RMP_INT_MASK()                  RMP_Int_Mask(RMP_A7M_INT_MASK_LVL)
#define RMP_INT_UNMASK()                RMP_Int_Mask(0x00U)
/* Yield operation */
#define RMP_YIELD()                     (RMP_A7M_NVIC_INT_CTRL=RMP_A7M_NVIC_PENDSVSET)
#define RMP_YIELD_ISR()                 RMP_YIELD()
/* End System Macro **********************************************************/

/* ARMv7-M Macro *************************************************************/
/* Fault enabling switch */
#define RMP_A7M_SHCSR_USGFAULTENA       RMP_POW2(18U)
#define RMP_A7M_SHCSR_BUSFAULTENA       RMP_POW2(17U)
#define RMP_A7M_SHCSR_MEMFAULTENA       RMP_POW2(16U)

/* NVIC grouping */
#define RMP_A7M_NVIC_GROUPING_P7S1      (0U)
#define RMP_A7M_NVIC_GROUPING_P6S2      (1U)
#define RMP_A7M_NVIC_GROUPING_P5S3      (2U)
#define RMP_A7M_NVIC_GROUPING_P4S4      (3U)
#define RMP_A7M_NVIC_GROUPING_P3S5      (4U)
#define RMP_A7M_NVIC_GROUPING_P2S6      (5U)
#define RMP_A7M_NVIC_GROUPING_P1S7      (6U)
#define RMP_A7M_NVIC_GROUPING_P0S8      (7U)

/* PendSV trigger */
#define RMP_A7M_NVIC_INT_CTRL           (*((volatile rmp_ptr_t*)0xE000ED04U))
#define RMP_A7M_NVIC_PENDSVSET          (0x10000000U)
/* End ARMv7-M Macro *********************************************************/
/*****************************************************************************/
/* __RMP_PLATFORM_A7M_DEF__ */
#endif
/* __HDR_DEF__ */
#endif
/* End Define ****************************************************************/

/* Struct ********************************************************************/
#ifdef __HDR_STRUCT__
#ifndef __RMP_PLATFORM_A7M_STRUCT__
#define __RMP_PLATFORM_A7M_STRUCT__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEF__
#undef __HDR_DEF__
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
/* __RMP_PLATFORM_A7M_STRUCT__ */
#endif
/* __HDR_STRUCT__ */
#endif
/* End Struct ****************************************************************/

/* Private Variable **********************************************************/
#if(!(defined __HDR_DEF__||defined __HDR_STRUCT__))
#ifndef __RMP_PLATFORM_A7M_MEMBER__
#define __RMP_PLATFORM_A7M_MEMBER__

/* In this way we can use the data structures and definitions in the headers */
#define __HDR_DEF__

#undef __HDR_DEF__

#define __HDR_STRUCT__

#undef __HDR_STRUCT__

/* If the header is not used in the public mode */
#ifndef __HDR_PUBLIC__
/*****************************************************************************/

/*****************************************************************************/
/* End Private Variable ******************************************************/

/* Private Function **********************************************************/ 
/*****************************************************************************/

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
/* Interrupts */
RMP_EXTERN void RMP_Int_Disable(void);
RMP_EXTERN void RMP_Int_Enable(void);
RMP_EXTERN void RMP_Int_Mask(rmp_ptr_t Level);

RMP_EXTERN rmp_ptr_t _RMP_A7M_MSB_Get(rmp_ptr_t Value);
RMP_EXTERN rmp_ptr_t _RMP_A7M_LSB_Get(rmp_ptr_t Value);
RMP_EXTERN void _RMP_Start(rmp_ptr_t Entry, rmp_ptr_t Stack);

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
/* __RMP_PLATFORM_A7M_MEMBER__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
