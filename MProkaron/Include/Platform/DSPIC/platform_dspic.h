/******************************************************************************
Filename    : platform_dspic.h
Author      : pry
Date        : 23/02/2018
Licence     : LGPL v3+; see COPYING for details.
Description : The header of "platform_dspic.c".
******************************************************************************/

/* Defines *******************************************************************/
#ifdef __HDR_DEFS__
#ifndef __PLATFORM_DSPIC_H_DEFS__
#define __PLATFORM_DSPIC_H_DEFS__
/*****************************************************************************/
/* Basic Types ***************************************************************/
#if(DEFINE_BASIC_TYPES==TRUE)

#ifndef __S32__
#define __S32__
typedef signed long s32;
#endif

#ifndef __S16__
#define __S16__
typedef signed short s16;
#endif

#ifndef __S8__
#define __S8__
typedef signed char s8;
#endif

#ifndef __U32__
#define __U32__
typedef unsigned long u32;
#endif

#ifndef __U16__
#define __U16__
typedef unsigned short u16;
#endif

#ifndef __U8__
#define __U8__
typedef unsigned char u8;
#endif

#endif
/* End Basic Types ***********************************************************/

/* Begin Extended Types ******************************************************/
#ifndef __PTR_T__
#define __PTR_T__
/* The typedef for the pointers - This is the raw style. Pointers must be unsigned */
typedef u16 ptr_t;
#endif

#ifndef __CNT_T__
#define __CNT_T__
/* The typedef for the count variables */
typedef s16 cnt_t;
#endif

#ifndef __RET_T__
#define __RET_T__
/* The type for process return value */
typedef s16 ret_t;
#endif
/* End Extended Types ********************************************************/

/* System macros *************************************************************/
/* Compiler "extern" keyword setting */
#define EXTERN                   extern
/* The order of bits in one CPU machine word */
#define RMP_WORD_ORDER           4
/* The maximum length of char printing - no need to change this in most cases */
#define RMP_KERNEL_DEBUG_MAX_STR 255
/* The offset of the stack when initializing */
#define RMP_INIT_STACK           RMP_INIT_STACK_HEAD(64)

/* The CPU and application specific macros are here */
#include "platform_dspic_conf.h"
/* End System macros *********************************************************/

/* DSPIC33 class specific macros *********************************************/

/*****************************************************************************/
/* __PLATFORM_DSPIC_H_DEFS__ */
#endif
/* __HDR_DEFS__ */
#endif
/* End Defines ***************************************************************/

/* Structs *******************************************************************/
#ifdef __HDR_STRUCTS__
#ifndef __PLATFORM_DSPIC_H_STRUCTS__
#define __PLATFORM_DSPIC_H_STRUCTS__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEFS__
#undef __HDR_DEFS__
/*****************************************************************************/

/*****************************************************************************/
/* __PLATFORM_DSPIC_H_STRUCTS__ */
#endif
/* __HDR_STRUCTS__ */
#endif
/* End Structs ***************************************************************/

/* Private Global Variables **************************************************/
#if(!(defined __HDR_DEFS__||defined __HDR_STRUCTS__))
#ifndef __PLATFORM_DSPIC_MEMBERS__
#define __PLATFORM_DSPIC_MEMBERS__

/* In this way we can use the data structures and definitions in the headers */
#define __HDR_DEFS__

#undef __HDR_DEFS__

#define __HDR_STRUCTS__

#undef __HDR_STRUCTS__

/* If the header is not used in the public mode */
#ifndef __HDR_PUBLIC_MEMBERS__
/*****************************************************************************/

/*****************************************************************************/
/* End Private Global Variables **********************************************/

/* Private C Function Prototypes *********************************************/ 
/*****************************************************************************/

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
__EXTERN__ volatile ptr_t RMP_SP_Val;
__EXTERN__ volatile ptr_t RMP_Int_Nest;
__EXTERN__ volatile ptr_t RMP_TBLPAG_Val;
__EXTERN__ volatile ptr_t RMP_DSRPAG_Val;
__EXTERN__ volatile ptr_t RMP_DSWPAG_Val;
/*****************************************************************************/

/* End Public Global Variables ***********************************************/

/* Public C Function Prototypes **********************************************/
/*****************************************************************************/
/* Interrupts */
EXTERN void RMP_Disable_Int(void);
EXTERN void RMP_Enable_Int(void);

EXTERN ptr_t RMP_MSB_Get(ptr_t Val);
EXTERN void _RMP_Start(ptr_t Entry, ptr_t Stack);
__EXTERN__ void _RMP_Yield(void);
__EXTERN__ void _RMP_Set_Timer(ptr_t Ticks);

/* Initialization */
__EXTERN__ void _RMP_Stack_Init(ptr_t Entry, ptr_t Stack, ptr_t Arg);
__EXTERN__ void _RMP_Low_Level_Init(void);
__EXTERN__ void RMP_Putchar(char Char);
__EXTERN__ void _RMP_Plat_Hook(void);

/* Platform-dependent hooks */
__EXTERN__ void _RMP_Clear_Soft_Flag(void);
__EXTERN__ void _RMP_Clear_Timer_Flag(void);
/*****************************************************************************/
/* Undefine "__EXTERN__" to avoid redefinition */
#undef __EXTERN__
/* __PLATFORM_DSPIC_MEMBERS__ */
#endif
/* !(defined __HDR_DEFS__||defined __HDR_STRUCTS__) */
#endif
/* End Public C Function Prototypes ******************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
