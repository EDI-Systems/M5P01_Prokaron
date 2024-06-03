/******************************************************************************
Filename    : rmp_platform_rl78.h
Author      : lht
Date        : 04/24/2024
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_rl78.c".
******************************************************************************/

/* Define ********************************************************************/
#ifdef __HDR_DEF__
#ifndef __RMP_PLATFORM_RL78_DEF__
#define __RMP_PLATFORM_RL78_DEF__
#endif
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
#ifndef __RMP_PTR_T__
#define __RMP_PTR_T__
/* The typedef for the pointers - This is the raw style. Pointers must be unsigned */
typedef rmp_u16_t rmp_ptr_t;
#endif

#ifndef __RMP_CNT_T__
#define __RMP_CNT_T__
/* The typedef for the count variables */
typedef long rmp_cnt_t;
#endif

#ifndef __RMP_RET_T__
#define __RMP_RET_T__
/* The type for process return value */
typedef rmp_s16_t rmp_ret_t;
#endif
/* End Extended Types ********************************************************/

/* System macros *************************************************************/
/* Compiler "extern" keyword setting */
#define EXTERN                          extern
/* The order of bits in one CPU machine word */
#define RMP_WORD_ORDER                  (4U)
/* The maximum length of char printing */
#define RMP_DEBUG_PRINT_MAX             (255U)
/* Full descending stack of rmp_ptr_t, word-addressing with 2-byte alignment */
#define RMP_STACK_TYPE                  RMP_STACK_FULL_DESCEND
#define RMP_STACK_ALIGN                 (1U)
#define RMP_STACK_ELEM                  rmp_ptr_t
#define RMP_STACK_STRUCT                struct RMP_RL78_Stack
/* MSB/LSB extraction */
#define RMP_MSB_GET(VAL)                RMP_MSB_Generic(VAL)
#define RMP_LSB_GET(VAL)                RMP_LSB_Generic(VAL)

/* The CPU and application specific macros are here */
#include "rmp_platform_rl78_conf.h"
/* End System macros *********************************************************/
/* __HDR_DEF__ */
#endif
/* End Define ****************************************************************/

/* Struct ********************************************************************/
#ifdef __HDR_STRUCT__
#ifndef __RMP_PLATFORM_RL78_STRUCT__
#define __RMP_PLATFORM_RL78_STRUCT__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEF__
#undef __HDR_DEF__
/*****************************************************************************/
struct RMP_RL78_Stack
{
    /* The former is higher, latter is lower */
    rmp_u16_t CSES;
    rmp_u16_t HL;
    rmp_u16_t DE;
    rmp_u16_t BC;
    rmp_u16_t AX;
    rmp_u16_t PC;
    rmp_u16_t PSWPCH;
};
/*****************************************************************************/
/* __RMP_PLATFORM_RL78_STRUCT__ */
#endif
/* __HDR_STRUCT__ */
#endif
/* End Struct ****************************************************************/

/* Private Variable **********************************************************/
#if(!(defined __HDR_DEF__||defined __HDR_STRUCT__))
#ifndef __RMP_PLATFORM_RL78_MEMBER__
#define __RMP_PLATFORM_RL78_MEMBER__

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
__EXTERN__ rmp_ptr_t _RMP_RL78_SP_Kern;

__EXTERN__ volatile rmp_u8_t RMP_RL78_Int_Act;
__EXTERN__ volatile rmp_u8_t _RMP_RL78_Yield_Pend;

/*****************************************************************************/

/* End Public Variable *******************************************************/

/* Public Function ***********************************************************/
/*****************************************************************************/
/* Interrupts */
EXTERN void RMP_Int_Disable(void);
EXTERN void RMP_Int_Enable(void);
EXTERN void _RMP_Start(rmp_ptr_t Entry, rmp_ptr_t Stack);
EXTERN void _RMP_RL78_Yield(void);
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
/* __RMP_PLATFORM_RL78_MEMBER__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
