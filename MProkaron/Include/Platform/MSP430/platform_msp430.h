/******************************************************************************
Filename    : platform_msp430.h
Author      : pry
Date        : 01/04/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The header of "platform_msp430.c".
******************************************************************************/

/* Defines *******************************************************************/
#ifdef __HDR_DEFS__
#ifndef __PLATFORM_MSP430_H_DEFS__
#define __PLATFORM_MSP430_H_DEFS__
/*****************************************************************************/
/* Basic Types ***************************************************************/
#if(DEFINE_BASIC_TYPES==TRUE)

#ifndef __S32__
#define __S32__
typedef signed long s32;
#endif

#ifndef __S16__
#define __S16__
typedef signed int s16;
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
typedef unsigned int u16;
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
#if(RMP_MSP430_X==RMP_TRUE)
typedef u32 ptr_t;
#else
typedef u16 ptr_t;
#endif
#endif

#ifndef __CNT_T__
#define __CNT_T__
/* The typedef for the count variables */
typedef s16 cnt_t;
#endif

#ifndef __RET_T__
#define __RET_T__
#if(RMP_MSP430_X==RMP_TRUE)
/* The type for return value */
typedef s32 ret_t;
#else
typedef s16 ret_t;
#endif
#endif
/* End Extended Types ********************************************************/

/* System macros *************************************************************/
/* Compiler "extern" keyword setting */
#define EXTERN                   extern
/* The order of bits in one CPU machine word */
#define RMP_WORD_ORDER           4
/* The maximum length of char printing - no need to change this in most cases */
#define RMP_KERNEL_DEBUG_MAX_STR 128
/* The offset of the stack when initializing */
#define RMP_INIT_STACK           RMP_INIT_STACK_TAIL(12)

/* The CPU and application specific macros are here */
#include "platform_msp430_conf.h"
/* End System macros *********************************************************/

/* MSP430 specific macros ****************************************************/
#define RMP_MSP430_SR_SCG1      (1<<7)
#define RMP_MSP430_SR_SCG0      (1<<6)
#define RMP_MSP430_SR_OSCOFF    (1<<5)
#define RMP_MSP430_SR_CPUOFF    (1<<4)
#define RMP_MSP430_SR_GIE       (1<<3)

#define RMP_MSP430X_PCSR(PC,SR) (((PC)<<16)|(((PC)>>4)&0xF000)|(SR))
/*****************************************************************************/
/* __PLATFORM_MSP430_H_DEFS__ */
#endif
/* __HDR_DEFS__ */
#endif
/* End Defines ***************************************************************/

/* Structs *******************************************************************/
#ifdef __HDR_STRUCTS__
#ifndef __PLATFORM_MSP430_H_STRUCTS__
#define __PLATFORM_MSP430_H_STRUCTS__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEFS__
#undef __HDR_DEFS__
/*****************************************************************************/

/*****************************************************************************/
/* __PLATFORM_MSP430_H_STRUCTS__ */
#endif
/* __HDR_STRUCTS__ */
#endif
/* End Structs ***************************************************************/

/* Private Global Variables **************************************************/
#if(!(defined __HDR_DEFS__||defined __HDR_STRUCTS__))
#ifndef __PLATFORM_MSP430_MEMBERS__
#define __PLATFORM_MSP430_MEMBERS__

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

/*****************************************************************************/

/* End Public Global Variables ***********************************************/

/* Public C Function Prototypes **********************************************/
/*****************************************************************************/
/* Interrupts */
EXTERN void RMP_Disable_Int(void);
EXTERN void RMP_Enable_Int(void);

__EXTERN__ ptr_t RMP_MSB_Get(ptr_t Val);
EXTERN void _RMP_Start(ptr_t Entry, ptr_t Stack);
__EXTERN__ void _RMP_Yield(void);

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
/* __PLATFORM_MSP430_MEMBERS__ */
#endif
/* !(defined __HDR_DEFS__||defined __HDR_STRUCTS__) */
#endif
/* End Public C Function Prototypes ******************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
