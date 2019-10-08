/******************************************************************************
Filename    : rmp_platform_a7m_rvm.h
Author      : pry
Date        : 01/04/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The header of platform-specific part of RMP for RVM hypervisor.
******************************************************************************/

/* Defines *******************************************************************/
#ifdef __HDR_DEFS__
#ifndef __RMP_PLATFORM_A7M_RVM_H_DEFS__
#define __RMP_PLATFORM_A7M_RVM_H_DEFS__
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
#define EXTERN                   extern
/* The order of bits in one CPU machine word */
#define RMP_WORD_ORDER           5
/* The maximum length of char printing - no need to change this in most cases */
#define RMP_KERNEL_DEBUG_MAX_STR 128
/* The offset of the stack when initializing */
#define RMP_INIT_STACK           RMP_INIT_STACK_TAIL(17)

/* The virtual machine configs are here */
#include "rvm_guest_a7m.h"

/* The CPU and application specific macros are here */
#include "rmp_platform_a7m_rvm_conf.h"
/* End System macros *********************************************************/
/*****************************************************************************/
/* __RMP_PLATFORM_A7M_RVM_H_DEFS__ */
#endif
/* __HDR_DEFS__ */
#endif
/* End Defines ***************************************************************/

/* Structs *******************************************************************/
#ifdef __HDR_STRUCTS__
#ifndef __RMP_PLATFORM_A7M_RVM_H_STRUCTS__
#define __RMP_PLATFORM_A7M_RVM_H_STRUCTS__

/* Use defines in these headers */
#define __HDR_DEFS__
#undef __HDR_DEFS__
/*****************************************************************************/

/*****************************************************************************/
/* __RMP_PLATFORM_A7M_RVM_H_STRUCTS__ */
#endif
/* __HDR_STRUCTS__ */
#endif
/* End Structs ***************************************************************/

/* Private Global Variables **************************************************/
#if(!(defined __HDR_DEFS__||defined __HDR_STRUCTS__))
#ifndef __RMP_PLATFORM_A7M_RVM_MEMBERS__
#define __RMP_PLATFORM_A7M_RVM_MEMBERS__

/* In this way we can use the data structures and definitions in the headers */
#define __HDR_DEFS__

#undef __HDR_DEFS__

#define __HDR_STRUCTS__

#undef __HDR_STRUCTS__

/* If the header is not used in the public mode */
#ifndef __HDR_PUBLIC_MEMBERS__
/*****************************************************************************/
static rvm_ptr_t RMP_Console_Ptr;
/*****************************************************************************/
/* End Private Global Variables **********************************************/

/* Private C Function Prototypes *********************************************/ 
/*****************************************************************************/
/* Interrupts */
static void RMP_PendSV_Handler(void);
static void RMP_SysTick_Handler(void);
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
__EXTERN__ void RMP_Enable_Int(void);
__EXTERN__ void RMP_Disable_Int(void);
__EXTERN__ void RMP_Mask_Int(void);
__EXTERN__ void RMP_Unmask_Int(void);

EXTERN rvm_ptr_t RMP_MSB_Get(rvm_ptr_t Val);
EXTERN void _RMP_Start(rvm_ptr_t Entry, rvm_ptr_t Stack);
__EXTERN__ void _RMP_Yield(void);

/* Initialization */
__EXTERN__ void _RMP_Stack_Init(rvm_ptr_t Entry, rvm_ptr_t Stack, rvm_ptr_t Arg);
__EXTERN__ void _RMP_Low_Level_Init(void);
__EXTERN__ void RMP_Putchar(char Char);
__EXTERN__ void _RMP_Plat_Hook(void);
/*****************************************************************************/
/* Undefine "__EXTERN__" to avoid redefinition */
#undef __EXTERN__
/* __RMP_PLATFORM_A7M_RVM_MEMBERS__ */
#endif
/* !(defined __HDR_DEFS__||defined __HDR_STRUCTS__) */
#endif
/* End Public C Function Prototypes ******************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
