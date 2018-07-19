/******************************************************************************
Filename    : platform_x86_linux.h
Author      : pry
Date        : 01/04/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The header of "platform_x86_linux.h".
******************************************************************************/

/* Defines *******************************************************************/
#ifdef __HDR_DEFS__
#ifndef __PLATFORM_X86_LINUX_H_DEFS__
#define __PLATFORM_X86_LINUX_H_DEFS__
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
#define RMP_INIT_STACK           RMP_INIT_STACK_TAIL(1024)

#include "platform_x86_linux_conf.h"
/* End System macros *********************************************************/

/* Cortex-M specific macros **************************************************/

/*****************************************************************************/
/* __PLATFORM_X86_LINUX_H_DEFS__ */
#endif
/* __HDR_DEFS__ */
#endif
/* End Defines ***************************************************************/

/* Structs *******************************************************************/
#ifdef __HDR_STRUCTS__
#ifndef __PLATFORM_X86_LINUX_H_STRUCTS__
#define __PLATFORM_X86_LINUX_H_STRUCTS__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEFS__
#undef __HDR_DEFS__
/*****************************************************************************/

/*****************************************************************************/
/* __PLATFORM_X86_LINUX_H_STRUCTS__ */
#endif
/* __HDR_STRUCTS__ */
#endif
/* End Structs ***************************************************************/

/* Private Global Variables **************************************************/
#if(!(defined __HDR_DEFS__||defined __HDR_STRUCTS__))
#ifndef __PLATFORM_X86_LINUX_MEMBERS__
#define __PLATFORM_X86_LINUX_MEMBERS__

/* In this way we can use the data structures and definitions in the headers */
#define __HDR_DEFS__

#undef __HDR_DEFS__

#define __HDR_STRUCTS__

#undef __HDR_STRUCTS__

/* If the header is not used in the public mode */
#ifndef __HDR_PUBLIC_MEMBERS__
/*****************************************************************************/
static volatile ptr_t RMP_SysTick_Flag;
static volatile ptr_t RMP_PendSV_Flag;
/*****************************************************************************/
/* End Private Global Variables **********************************************/

/* Private C Function Prototypes *********************************************/ 
/*****************************************************************************/
static void SysTick_Handler(void);
static void PendSV_Handler(void);
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
/* PID */
__EXTERN__ volatile ptr_t RMP_Int_Disabled;
__EXTERN__ volatile pid_t RMP_Sys_PID;
__EXTERN__ volatile pid_t RMP_User_PID;

/* External interrupt handler */
__EXTERN__ void (*volatile RMP_Eint_Handler)(void);
/*****************************************************************************/

/* End Public Global Variables ***********************************************/

/* Public C Function Prototypes **********************************************/
/*****************************************************************************/
/* Interrupts */
__EXTERN__ void RMP_Disable_Int(void);
__EXTERN__ void RMP_Enable_Int(void);

__EXTERN__ ptr_t RMP_MSB_Get(ptr_t Val);
__EXTERN__ void _RMP_Start(ptr_t Entry, ptr_t Stack);
__EXTERN__ void _RMP_Yield(void);

/* Initialization */
__EXTERN__ void _RMP_Stack_Init(ptr_t Entry, ptr_t Stack, ptr_t Arg);
__EXTERN__ void _RMP_Low_Level_Init(void);
__EXTERN__ void RMP_Putchar(char Char);
__EXTERN__ void _RMP_Plat_Hook(void);
/*****************************************************************************/
/* Undefine "__EXTERN__" to avoid redefinition */
#undef __EXTERN__
/* __PLATFORM_X86_LINUX_MEMBERS__ */
#endif
/* !(defined __HDR_DEFS__||defined __HDR_STRUCTS__) */
#endif
/* End Public C Function Prototypes ******************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
