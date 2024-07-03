/******************************************************************************
Filename    : rmp_platform_x86l.h
Author      : pry wyh
Date        : 01/04/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_x86l.c".
******************************************************************************/

/* Define ********************************************************************/
#ifdef __HDR_DEF__
#ifndef __RMP_PLATFORM_X86L_DEF__
#define __RMP_PLATFORM_X86L_DEF__
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
/* Full descending stack of rmp_ptr_t, 16-byte alignment */
#define RMP_STACK_TYPE                  RMP_STACK_FULL_DESCEND
#define RMP_STACK_ALIGN                 (4U)
#define RMP_STACK_ELEM                  rmp_ptr_t
#define RMP_STACK_STRUCT                struct RMP_X86L_Stack
/* MSB/LSB extraction */
#define RMP_MSB_GET(VAL)                RMP_MSB_Generic(VAL)
#define RMP_LSB_GET(VAL)                RMP_LSB_Generic(VAL)

/* The CPU and application specific macros are here */
#include "rmp_platform_x86l_conf.h"

/* Interrupt masking/unmasking */
#define RMP_INT_MASK()                  RMP_Int_Disable()
#define RMP_INT_UNMASK()                RMP_Int_Enable()
/* Yield operation */
#define RMP_YIELD()                     _RMP_X86L_Yield()
#define RMP_YIELD_ISR()                 RMP_YIELD()
/* End System Macro **********************************************************/
/*****************************************************************************/
/* __RMP_PLATFORM_X86L_DEF__ */
#endif
/* __HDR_DEF__ */
#endif
/* End Define ****************************************************************/

/* Struct ********************************************************************/
#ifdef __HDR_STRUCT__
#ifndef __RMP_PLATFORM_X86L_STRUCT__
#define __RMP_PLATFORM_X86L_STRUCT__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEF__
#undef __HDR_DEF__
/*****************************************************************************/
struct RMP_X86L_Stack
{
    /* To avoid conflict with Linux headers, we use REG_ prefix */
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
/* __RMP_PLATFORM_X86L_STRUCT__ */
#endif
/* __HDR_STRUCT__ */
#endif
/* End Struct ****************************************************************/

/* Private Variable **********************************************************/
#if(!(defined __HDR_DEF__||defined __HDR_STRUCT__))
#ifndef __RMP_PLATFORM_X86L_MEMBER__
#define __RMP_PLATFORM_X86L_MEMBER__

/* In this way we can use the data structures and definitions in the headers */
#define __HDR_DEF__

#undef __HDR_DEF__

#define __HDR_STRUCT__

#undef __HDR_STRUCT__

/* If the header is not used in the public mode */
#ifndef __HDR_PUBLIC__
/*****************************************************************************/
static volatile rmp_ptr_t RMP_X86L_Tick_Flag;
static volatile rmp_ptr_t RMP_X86L_Switch_Flag;
static volatile rmp_ptr_t RMP_X86L_Eint_Flag;
/*****************************************************************************/
/* End Private Variable ******************************************************/

/* Private Function **********************************************************/ 
/*****************************************************************************/
static void RMP_X86L_Tick_Handler(void);
static void RMP_X86L_Switch_Handler(void);
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
/* Interrupt */
__RMP_EXTERN__ volatile rmp_ptr_t RMP_X86L_Int_Mask;
/* PID */
__RMP_EXTERN__ volatile pid_t RMP_X86L_Vct_PID;
__RMP_EXTERN__ volatile pid_t RMP_X86L_Usr_PID;

/* RMP_EXTERNal interrupt handler */
__RMP_EXTERN__ void (*volatile RMP_X86L_Eint_Handler)(void);
/*****************************************************************************/

/* End Public Variable *******************************************************/

/* Public Function ***********************************************************/
/*****************************************************************************/
/* Interrupts */
__RMP_EXTERN__ void RMP_Int_Disable(void);
__RMP_EXTERN__ void RMP_Int_Enable(void);

__RMP_EXTERN__ void _RMP_Start(rmp_ptr_t Entry,
                               rmp_ptr_t Stack);
__RMP_EXTERN__ void _RMP_X86L_Yield(void);

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
/* __RMP_PLATFORM_X86L_MEMBER__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
