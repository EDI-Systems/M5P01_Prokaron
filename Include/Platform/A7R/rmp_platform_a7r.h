/******************************************************************************
Filename    : rmp_platform_a7r.h
Author      : pry
Date        : 01/04/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_a7r.c".
******************************************************************************/

/* Define ********************************************************************/
#ifdef __HDR_DEF__
#ifndef __RMP_PLATFORM_A7R_DEF__
#define __RMP_PLATFORM_A7R_DEF__
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
#define EXTERN                          extern
/* The order of bits in one CPU machine word */
#define RMP_WORD_ORDER                  (5U)
/* The maximum length of char printing */
#define RMP_DEBUG_PRINT_MAX             (255U)
/* Full descending stack of rmp_ptr_t, 8-byte alignment */
#define RMP_STACK_TYPE                  RMP_STACK_FULL_DESCEND
#define RMP_STACK_ALIGN                 (3U)
#define RMP_STACK_ELEM                  rmp_ptr_t
#define RMP_STACK_STRUCT                struct RMP_A7R_Stack
/* MSB/LSB extraction */
#define RMP_MSB_GET(VAL)                _RMP_A7R_MSB_Get(VAL)
#define RMP_LSB_GET(VAL)                _RMP_A7R_LSB_Get(VAL)

/* The CPU and application specific macros are here */
#include "rmp_platform_a7r_conf.h"
/* End System Macro **********************************************************/

/* ARMv7-R specific macros ***************************************************/
/* Mode definitions */
#define RMP_A7R_SYS                     (0x1F)
#define RMP_A7R_USR                     (0x10)
#define RMP_A7R_SVC                     (0x13)
#define RMP_A7R_IRQ                     (0x12)
#define RMP_A7R_FIQ                     (0x11)
#define RMP_A7R_ABT                     (0x17)
#define RMP_A7R_UND                     (0x1B)

/* CPSR bit definitions */
#define RMP_A7R_CPSR_E                  (1<<9)
#define RMP_A7R_CPSR_A                  (1<<8)
#define RMP_A7R_CPSR_I                  (1<<7)
#define RMP_A7R_CPSR_F                  (1<<6)
#define RMP_A7R_CPSR_T                  (1<<5)
#define RMP_A7R_CPSR_M(X)               ((X))
/*****************************************************************************/
/* __RMP_PLATFORM_A7R_DEF__ */
#endif
/* __HDR_DEF__ */
#endif
/* End Define ****************************************************************/

/* Struct ********************************************************************/
#ifdef __HDR_STRUCT__
#ifndef __RMP_PLATFORM_A7R_STRUCT__
#define __RMP_PLATFORM_A7R_STRUCT__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEF__
#undef __HDR_DEF__
/*****************************************************************************/
struct RMP_A7R_Stack
{
    rmp_ptr_t R0;
    rmp_ptr_t R1;
    rmp_ptr_t R2;
    rmp_ptr_t R3;
    rmp_ptr_t R4;
    rmp_ptr_t R5;
    rmp_ptr_t R6;
    rmp_ptr_t R7;
    rmp_ptr_t R8;
    rmp_ptr_t R9;
    rmp_ptr_t R10;
    rmp_ptr_t R11;
    rmp_ptr_t R12;
    rmp_ptr_t LR;
    rmp_ptr_t PC;
    rmp_ptr_t CPSR;
};
/*****************************************************************************/
/* __RMP_PLATFORM_A7R_STRUCT__ */
#endif
/* __HDR_STRUCT__ */
#endif
/* End Struct ****************************************************************/

/* Private Variable **********************************************************/
#if(!(defined __HDR_DEF__||defined __HDR_STRUCT__))
#ifndef __RMP_PLATFORM_A7R_MEMBER__
#define __RMP_PLATFORM_A7R_MEMBER__

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
#define __EXTERN__
/* End Private Function ******************************************************/

/* Public Variable ***********************************************************/
/* __HDR_PUBLIC__ */
#else
#define __EXTERN__ EXTERN 
/* __HDR_PUBLIC__ */
#endif

/*****************************************************************************/

/*****************************************************************************/

/* End Public Variable *******************************************************/

/* Public Function ***********************************************************/
/*****************************************************************************/
/* Interrupts */
EXTERN void RMP_Int_Disable(void);
EXTERN void RMP_Int_Enable(void);
EXTERN void RMP_Int_Mask(rmp_ptr_t Level);

EXTERN rmp_ptr_t _RMP_A7R_MSB_Get(rmp_ptr_t Value);
EXTERN rmp_ptr_t _RMP_A7R_LSB_Get(rmp_ptr_t Value);
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
/* __RMP_PLATFORM_A7R_MEMBER__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
