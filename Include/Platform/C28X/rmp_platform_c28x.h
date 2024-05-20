/******************************************************************************
Filename    : rmp_platform_c28x.h
Author      : zr (ran zhang)
Date        : 27/04/2024
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_c28x.c".
******************************************************************************/

/* Define ********************************************************************/
#ifdef __HDR_DEF__
#ifndef __RMP_PLATFORM_C28X_DEF__
#define __RMP_PLATFORM_C28X_DEF__
/*****************************************************************************/
/* Basic Types ***************************************************************/
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
/* End Basic Types ***********************************************************/

/* Extended Types ************************************************************/
/* The CPU and application specific macros are here */
#include "rmp_platform_c28x_conf.h"

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
/* The type for return value */
typedef rmp_s32_t rmp_ret_t;
#endif
/* End Extended Types ********************************************************/

/* System macros *************************************************************/
/* Compiler "extern" keyword setting */
#define EXTERN                          extern
/* The order of bits in one CPU machine word */
#define RMP_WORD_ORDER                  (5U)
/* The maximum length of char printing - no need to change this in most cases */
#define RMP_DEBUG_PRINT_MAX             (128U)
/* Descending stack, 2-word alignment */
#define RMP_INIT_STACK                  RMP_INIT_STACK_ASCEND(1U)
/* MSB/LSB extraction */
#define RMP_MSB_GET(VAL)                RMP_MSB_Generic(VAL)
#define RMP_LSB_GET(VAL)                RMP_LSB_Generic(VAL)
/* End System macros *********************************************************/
/*****************************************************************************/
/* __RMP_PLATFORM_C28X_DEF__ */
#endif
/* __HDR_DEF__ */
#endif
/* End Define ****************************************************************/

/* Struct ********************************************************************/
#ifdef __HDR_STRUCT__
#ifndef __RMP_PLATFORM_C28X_STRUCT__
#define __RMP_PLATFORM_C28X_STRUCT__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEF__
#undef __HDR_DEF__
/*****************************************************************************/
struct RMP_C28X_Stack
{
    /* Hardware stacked */
    rmp_ptr_t T_ST0;
    rmp_ptr_t ACC;
    rmp_ptr_t P;
    rmp_ptr_t AR1_AR0;
    rmp_ptr_t DP_ST1;
    rmp_ptr_t DBGSTAT_IER;
    rmp_ptr_t PC;
    /* Software stacked */
    rmp_ptr_t RPC;
    rmp_ptr_t AR1H_AR0H;
    rmp_ptr_t XAR2;
    rmp_ptr_t XAR3;
    rmp_ptr_t XAR4;
    rmp_ptr_t XAR5;
    rmp_ptr_t XAR6;
    rmp_ptr_t XAR7;
    rmp_ptr_t XT;
    /* FPU registers */
#if(RMP_C28X_COP_FPU64!=0U)
    rmp_ptr_t STF;
    rmp_ptr_t RB;
    rmp_ptr_t R0L;
    rmp_ptr_t R0H;
    rmp_ptr_t R1L;
    rmp_ptr_t R1H;
    rmp_ptr_t R2L;
    rmp_ptr_t R2H;
    rmp_ptr_t R3L;
    rmp_ptr_t R3H;
    rmp_ptr_t R4L;
    rmp_ptr_t R4H;
    rmp_ptr_t R5L;
    rmp_ptr_t R5H;
    rmp_ptr_t R6L;
    rmp_ptr_t R6H;
    rmp_ptr_t R7L;
    rmp_ptr_t R7H;
#elif(RMP_C28X_COP_FPU32!=0U)
    rmp_ptr_t STF;
    rmp_ptr_t RB;
    rmp_ptr_t R0H;
    rmp_ptr_t R1H;
    rmp_ptr_t R2H;
    rmp_ptr_t R3H;
    rmp_ptr_t R4H;
    rmp_ptr_t R5H;
    rmp_ptr_t R6H;
    rmp_ptr_t R7H;
#endif
};

/*****************************************************************************/
/* __RMP_PLATFORM_C28X_STRUCT__ */
#endif
/* __HDR_STRUCT__ */
#endif
/* End Struct ****************************************************************/

/* Private Variable **********************************************************/
#if(!(defined __HDR_DEF__||defined __HDR_STRUCT__))
#ifndef __RMP_PLATFORM_C28X_MEMBER__
#define __RMP_PLATFORM_C28X_MEMBER__

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
__EXTERN__ rmp_u16_t _RMP_C28X_SP_Kern;

__EXTERN__ volatile rmp_u16_t RMP_C28X_Int_Act;
__EXTERN__ volatile rmp_u16_t _RMP_C28X_Yield_Pend;
/*****************************************************************************/

/* End Public Variable *******************************************************/

/* Public Function ***********************************************************/
/*****************************************************************************/
/* Interrupts */
EXTERN void RMP_Int_Disable(void);
EXTERN void RMP_Int_Enable(void);

EXTERN void _RMP_Start(rmp_ptr_t Entry, rmp_ptr_t Stack);
EXTERN void _RMP_C28X_Yield_NONE(void);
EXTERN void _RMP_C28X_Yield_FPU32(void);
EXTERN void _RMP_C28X_Yield_FPU64(void);
__EXTERN__ void _RMP_Yield(void);

/* Initialization */
__EXTERN__ rmp_ptr_t _RMP_Stack_Init(rmp_ptr_t Stack,
                                     rmp_ptr_t Size,
                                     rmp_ptr_t Entry,
                                     rmp_ptr_t Param);
__EXTERN__ void _RMP_Lowlvl_Init(void);
__EXTERN__ void RMP_Putchar(char Char);
__EXTERN__ void _RMP_Plat_Hook(void);

/* Timer handler */
__EXTERN__ void _RMP_C28X_Tim_Handler(void);
/*****************************************************************************/
/* Undefine "__EXTERN__" to avoid redefinition */
#undef __EXTERN__
/* __RMP_PLATFORM_C28X_MEMBER__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
