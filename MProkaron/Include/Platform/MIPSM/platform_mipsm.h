/******************************************************************************
Filename    : platform_mipsm.h
Author      : pry
Date        : 23/02/2018
Licence     : LGPL v3+; see COPYING for details.
Description : The header of "platform_mipsm.c".
******************************************************************************/

/* Defines *******************************************************************/
#ifdef __HDR_DEFS__
#ifndef __PLATFORM_MIPSM_H_DEFS__
#define __PLATFORM_MIPSM_H_DEFS__
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
#define RMP_INIT_STACK           RMP_INIT_STACK_TAIL(64)

/* The CPU and application specific macros are here */
#include "platform_mipsm_conf.h"
/* End System macros *********************************************************/

/* MIPS-M class specific macros **********************************************/
/* Bits within the CP0 STATUS register */
#define RMP_MIPSM_STATUS_IE         (1<<0)        /* Enable interrupts */
#define RMP_MIPSM_STATUS_EXL        (1<<1)        /* Exception level */
#define RMP_MIPSM_STATUS_MX         (1<<24)       /* Allow DSP instructions */
#define RMP_MIPSM_STATUS_CU1        (0x20000000)  /* Enable CP1 for parts with hardware */
#define RMP_MIPSM_STATUS_FR         (0x04000000)  /* Enable 64 bit floating point registers */

/* Bits within the CP0 CAUSE register */
#define RMP_MIPSM_CAUSE_CORESW0     (0x00000100)  /*  */
#define RMP_MIPSM_CAUSE_CORESW1     (0x00000200)  /*  */

/* The EXL bit is set to ensure interrupts do not occur while the context of
the first task is being restored. */
#if ( __mips_hard_float == 1 )
    #define portINITIAL_SR			( portIE_BIT | portEXL_BIT | portMX_BIT | portFR_BIT | portCU1_BIT )
#else
    #define portINITIAL_SR			( portIE_BIT | portEXL_BIT | portMX_BIT )
#endif
/*****************************************************************************/
/* __PLATFORM_MIPSM_H_DEFS__ */
#endif
/* __HDR_DEFS__ */
#endif
/* End Defines ***************************************************************/

/* Structs *******************************************************************/
#ifdef __HDR_STRUCTS__
#ifndef __PLATFORM_MIPSM_H_STRUCTS__
#define __PLATFORM_MIPSM_H_STRUCTS__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEFS__
#undef __HDR_DEFS__
/*****************************************************************************/

/*****************************************************************************/
/* __PLATFORM_MIPSM_H_STRUCTS__ */
#endif
/* __HDR_STRUCTS__ */
#endif
/* End Structs ***************************************************************/

/* Private Global Variables **************************************************/
#if(!(defined __HDR_DEFS__||defined __HDR_STRUCTS__))
#ifndef __PLATFORM_MIPSM_MEMBERS__
#define __PLATFORM_MIPSM_MEMBERS__

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
__EXTERN__ volatile ptr_t RMP_GP_Val;
__EXTERN__ volatile ptr_t RMP_SP_Val;
__EXTERN__ volatile ptr_t RMP_Old_SP_Val;
__EXTERN__ volatile ptr_t RMP_Int_Nest;

/*****************************************************************************/

/* End Public Global Variables ***********************************************/

/* Public C Function Prototypes **********************************************/
/*****************************************************************************/
/* Interrupts */
EXTERN void RMP_Disable_Int(void);
EXTERN void RMP_Enable_Int(void);

EXTERN ptr_t RMP_MSB_Get(ptr_t Val);
EXTERN void _RMP_Start(ptr_t Entry, ptr_t Stack);
EXTERN void _RMP_Yield(void);
EXTERN void _RMP_Set_Timer(ptr_t Ticks);

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
/* __PLATFORM_MIPSM_MEMBERS__ */
#endif
/* !(defined __HDR_DEFS__||defined __HDR_STRUCTS__) */
#endif
/* End Public C Function Prototypes ******************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
