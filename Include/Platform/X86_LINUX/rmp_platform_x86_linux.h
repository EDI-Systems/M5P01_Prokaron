/******************************************************************************
Filename    : rmp_platform_x86_linux.h
Author      : pry
Date        : 01/04/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_x86_linux.c".
******************************************************************************/

/* Define ********************************************************************/
#ifdef __HDR_DEF__
#ifndef __RMP_PLATFORM_X86_LINUX_DEF__
#define __RMP_PLATFORM_X86_LINUX_DEF__
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

/* Extended Types ************************************************************/
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
/* Descending stack, 16-byte alignment */
#define RMP_INIT_STACK                  RMP_INIT_STACK_DESCEND(4U)
/* The maximum length of char printing - no need to change this in most cases */
#define RMP_DEBUG_PRINT_MAX             (255U)
/* MSB/LSB extraction */
#define RMP_MSB_GET(VAL)                RMP_MSB_Generic(VAL)
#define RMP_LSB_GET(VAL)                RMP_LSB_Generic(VAL)

/* The CPU and application specific macros are here */
#include "rmp_platform_x86_linux_conf.h"


/* End System macros *********************************************************/

/*****************************************************************************/
/* __RMP_PLATFORM_X86_LINUX_DEF__ */
#endif
/* __HDR_DEF__ */
#endif
/* End Define ****************************************************************/

/* Struct ********************************************************************/
#ifdef __HDR_STRUCT__
#ifndef __RMP_PLATFORM_X86_LINUX_STRUCT__
#define __RMP_PLATFORM_X86_LINUX_STRUCT__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEF__
#undef __HDR_DEF__
/*****************************************************************************/
struct RMP_X86_LINUX_Stack
{
    /* To avoid conflict with linux header, we use REG_ prefix */
    rmp_ptr_t REG_EBX;
    rmp_ptr_t REG_ECX;
    rmp_ptr_t REG_EDX;
    rmp_ptr_t REG_ESI;
    rmp_ptr_t REG_EDI;
    rmp_ptr_t REG_EBP;
    rmp_ptr_t REG_EAX;
    rmp_ptr_t REG_XDS;
    rmp_ptr_t REG_XES;
    rmp_ptr_t REG_XFS;
    rmp_ptr_t REG_XGS;
    rmp_ptr_t REG_ORIG_EAX;
    rmp_ptr_t REG_EIP;
    rmp_ptr_t REG_ECS;
    rmp_ptr_t REG_EFLAGS;
    rmp_ptr_t REG_XSS;
    rmp_ptr_t REG_Param;
};
/*****************************************************************************/
/* __RMP_PLATFORM_X86_LINUX_STRUCT__ */
#endif
/* __HDR_STRUCT__ */
#endif
/* End Struct ****************************************************************/

/* Private Variable **********************************************************/
#if(!(defined __HDR_DEF__||defined __HDR_STRUCT__))
#ifndef __RMP_PLATFORM_X86_LINUX_MEMBER__
#define __RMP_PLATFORM_X86_LINUX_MEMBER__

/* In this way we can use the data structures and definitions in the headers */
#define __HDR_DEF__

#undef __HDR_DEF__

#define __HDR_STRUCT__

#undef __HDR_STRUCT__

/* If the header is not used in the public mode */
#ifndef __HDR_PUBLIC__
/*****************************************************************************/
static volatile rmp_ptr_t RMP_SysTick_Flag;
static volatile rmp_ptr_t RMP_PendSV_Flag;
/*****************************************************************************/
/* End Private Variable ******************************************************/

/* Private Function **********************************************************/ 
/*****************************************************************************/
static void SysTick_Handler(void);
static void PendSV_Handler(void);
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
/* PID */
__EXTERN__ volatile rmp_ptr_t RMP_Int_Disabled;
__EXTERN__ volatile pid_t RMP_Sys_PID;
__EXTERN__ volatile pid_t RMP_User_PID;

/* External interrupt handler */
__EXTERN__ void (*volatile RMP_Eint_Handler)(void);
/*****************************************************************************/

/* End Public Variable *******************************************************/

/* Public Function ***********************************************************/
/*****************************************************************************/
/* Interrupts */
__EXTERN__ void RMP_Int_Disable(void);
__EXTERN__ void RMP_Int_Enable(void);

__EXTERN__ void _RMP_Start(rmp_ptr_t Entry, rmp_ptr_t Stack);
__EXTERN__ void _RMP_Yield(void);

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
/* __RMP_PLATFORM_X86_LINUX_MEMBER__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
