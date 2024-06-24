/******************************************************************************
Filename    : rmp_platform_msp430.h
Author      : pry
Date        : 01/04/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_msp430.c".
              This port assumes that all pointers are 16-bit for MSP430 and 32-
              bit for MSP430X. Hence, for MSP430, all function pointers and
              kernel data structures shall be in the first 64k.
******************************************************************************/

/* Define ********************************************************************/
#ifdef __HDR_DEF__
#ifndef __RMP_PLATFORM_MSP430_DEF__
#define __RMP_PLATFORM_MSP430_DEF__
/*****************************************************************************/
/* Basic Type ****************************************************************/
#ifndef __RMP_S32_T__
#define __RMP_S32_T__
typedef signed long rmp_s32_t;
#endif

#ifndef __RMP_S16_T__
#define __RMP_S16_T__
typedef signed int rmp_s16_t;
#endif

#ifndef __RMP_S8_T__
#define __RMP_S8_T__
typedef signed char rmp_s8_t;
#endif

#ifndef __RMP_U32_T__
#define __RMP_U32_T__
typedef unsigned long rmp_u32_t;
#endif

#ifndef __RMP_U16_T__
#define __RMP_U16_T__
typedef unsigned int rmp_u16_t;
#endif

#ifndef __RMP_U8_T__
#define __RMP_U8_T__
typedef unsigned char rmp_u8_t;
#endif
/* End Basic Type ************************************************************/

/* Extended Type *************************************************************/
/* The CPU and application specific macros are here - needed for data types */
#include "rmp_platform_msp430_conf.h"

#ifndef __RMP_PTR_T__
#define __RMP_PTR_T__
/* Pointer */
#if(RMP_MSP430_COP_430X!=0U)
typedef rmp_u32_t rmp_ptr_t;
#else
typedef rmp_u16_t rmp_ptr_t;
#endif
#endif

#ifndef __RMP_CNT_T__
#define __RMP_CNT_T__
/* Counter */
typedef rmp_s16_t rmp_cnt_t;
#endif

#ifndef __RMP_RET_T__
#define __RMP_RET_T__
/* The type for return value */
#if(RMP_MSP430_COP_430X!=0U)
typedef rmp_s32_t rmp_ret_t;
#else
typedef rmp_s16_t rmp_ret_t;
#endif
#endif
/* End Extended Type *********************************************************/

/* System Macro **************************************************************/
/* Compiler "extern" keyword setting */
#define RMP_EXTERN                      extern
/* The order of bits in one CPU machine word */
#if(RMP_MSP430_COP_430X!=0U)
#define RMP_WORD_ORDER                  (5U)
#else
#define RMP_WORD_ORDER                  (4U)
#endif
/* The maximum length of char printing */
#define RMP_DEBUG_PRINT_MAX             (255U)
/* Full descending stack of rmp_ptr_t, 4-byte alignment */
#define RMP_STACK_TYPE                  RMP_STACK_FULL_DESCEND
#define RMP_STACK_ALIGN                 (2U)
#define RMP_STACK_ELEM                  rmp_ptr_t
#define RMP_STACK_STRUCT                struct RMP_MSP430_Stack
/* MSB/LSB extraction */
#define RMP_MSB_GET(VAL)                RMP_MSB_Generic(VAL)
#define RMP_LSB_GET(VAL)                RMP_LSB_Generic(VAL)

/* Interrupt masking/unmasking */
#define RMP_INT_MASK()                  RMP_Int_Disable()
#define RMP_INT_UNMASK()                RMP_Int_Enable()
/* Yield operation */
#if(RMP_MSP430_COP_430X!=0U)
#define RMP_YIELD()                     _RMP_MSP430_Yield_430X()
#else
#define RMP_YIELD()                     _RMP_MSP430_Yield_NONE()
#endif
/* #define RMP_YIELD_ISR() */
/* End System Macro **********************************************************/

/* MSP430 Macro **************************************************************/
#define RMP_MSP430_SR_SCG1              RMP_POW2(7U)
#define RMP_MSP430_SR_SCG0              RMP_POW2(6U)
#define RMP_MSP430_SR_OSCOFF            RMP_POW2(5U)
#define RMP_MSP430_SR_CPUOFF            RMP_POW2(4U)
#define RMP_MSP430_SR_GIE               RMP_POW2(3U)

#define RMP_MSP430X_PCSR(PC,SR)         (((PC)<<16)|(((PC)>>4)&0xF000)|(SR))
/* End MSP430 Macro **********************************************************/
/*****************************************************************************/
/* __RMP_PLATFORM_MSP430_DEF__ */
#endif
/* __HDR_DEF__ */
#endif
/* End Define ****************************************************************/

/* Struct ********************************************************************/
#ifdef __HDR_STRUCT__
#ifndef __RMP_PLATFORM_MSP430_STRUCT__
#define __RMP_PLATFORM_MSP430_STRUCT__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEF__
#undef __HDR_DEF__
/*****************************************************************************/
struct RMP_MSP430_Stack
{
    rmp_ptr_t R4;
    rmp_ptr_t R5;
    rmp_ptr_t R6;
    rmp_ptr_t R7;
    rmp_ptr_t R8;
    rmp_ptr_t R9;
    rmp_ptr_t R10;
    rmp_ptr_t R11;
    rmp_ptr_t R12;
    rmp_ptr_t R13;
    rmp_ptr_t R14;
    rmp_ptr_t R15;
#if(RMP_MSP430_COP_430X!=0U)
    rmp_ptr_t PCSR;
#else
    rmp_ptr_t SR;
    rmp_ptr_t PC;
#endif
};
/*****************************************************************************/
/* __RMP_PLATFORM_MSP430_STRUCT__ */
#endif
/* __HDR_STRUCT__ */
#endif
/* End Struct ****************************************************************/

/* Private Variable **********************************************************/
#if(!(defined __HDR_DEF__||defined __HDR_STRUCT__))
#ifndef __RMP_PLATFORM_MSP430_MEMBER__
#define __RMP_PLATFORM_MSP430_MEMBER__

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
__RMP_EXTERN__ rmp_ptr_t _RMP_MSP430_SP_Kern;
/*****************************************************************************/

/* End Public Variable *******************************************************/

/* Public Function ***********************************************************/
/*****************************************************************************/
/* Interrupts */
RMP_EXTERN void RMP_Int_Disable(void);
RMP_EXTERN void RMP_Int_Enable(void);

#if(RMP_MSP430_COP_430X!=0U)
RMP_EXTERN void _RMP_MSP430_Yield_430X(void);
#else
RMP_EXTERN void _RMP_MSP430_Yield_NONE(void);
#endif

RMP_EXTERN void _RMP_Start(rmp_ptr_t Entry,
                           rmp_ptr_t Stack);

/* Initialization */
__RMP_EXTERN__ rmp_ptr_t _RMP_Stack_Init(rmp_ptr_t Stack,
                                         rmp_ptr_t Size,
                                         rmp_ptr_t Entry,
                                         rmp_ptr_t Param);
__RMP_EXTERN__ void _RMP_Lowlvl_Init(void);
__RMP_EXTERN__ void RMP_Putchar(char Char);
__RMP_EXTERN__ void _RMP_Plat_Hook(void);

/* Timer handler */
__RMP_EXTERN__ void _RMP_MSP430_Tim_Handler(void);
/*****************************************************************************/
/* Undefine "__RMP_EXTERN__" to avoid redefinition */
#undef __RMP_EXTERN__
/* __RMP_PLATFORM_MSP430_MEMBER__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
