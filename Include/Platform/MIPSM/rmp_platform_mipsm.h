/******************************************************************************
Filename    : rmp_platform_mipsm.h
Author      : pry
Date        : 23/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_mipsm.c".
******************************************************************************/

/* Defines *******************************************************************/
#ifdef __HDR_DEFS__
#ifndef __RMP_PLATFORM_MIPSM_H_DEFS__
#define __RMP_PLATFORM_MIPSM_H_DEFS__
/*****************************************************************************/
/* Basic Types ***************************************************************/
#ifndef __RMP_S32__
#define __RMP_S32__
typedef signed int rmp_s32_t;
#endif

#ifndef __RMP_S16__
#define __RMP_S16__
typedef signed short rmp_s16_t;
#endif

#ifndef __RMP_S8__
#define __RMP_S8__
typedef signed char rmp_s8_t;
#endif

#ifndef __RMP_U32__
#define __RMP_U32__
typedef unsigned int rmp_u32_t;
#endif

#ifndef __RMP_U16__
#define __RMP_U16__
typedef unsigned short rmp_u16_t;
#endif

#ifndef __RMP_U8__
#define __RMP_U8__
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
#define RMP_DEBUG_PRINT_MAX 255
/* The offset of the stack when initializing */
#define RMP_INIT_STACK           RMP_INIT_STACK_TAIL(64)

/* The CPU and application specific macros are here */
#include "rmp_platform_mipsm_conf.h"
/* End System macros *********************************************************/

/* MIPS-M class specific macros **********************************************/
/* Bits within the CP0 STATUS register */
#define RMP_MIPSM_STATUS_IE         (1<<0)        /* Enable interrupts */
#define RMP_MIPSM_STATUS_EXL        (1<<1)        /* Exception level */
#define RMP_MIPSM_STATUS_MX         (1<<24)       /* Allow DSP instructions */
#define RMP_MIPSM_STATUS_CU1        (0x20000000)  /* Enable CP1 for parts with hardware */
#define RMP_MIPSM_STATUS_FR         (0x04000000)  /* Enable 64 bit floating point registers */

/* Bits within the CP0 CAUSE register */
#define RMP_MIPSM_CAUSE_CORESW0     (0x00000100)
#define RMP_MIPSM_CAUSE_CORESW1     (0x00000200)
/*****************************************************************************/
/* __RMP_PLATFORM_MIPSM_H_DEFS__ */
#endif
/* __HDR_DEFS__ */
#endif
/* End Defines ***************************************************************/

/* Structs *******************************************************************/
#ifdef __HDR_STRUCTS__
#ifndef __RMP_PLATFORM_MIPSM_H_STRUCTS__
#define __RMP_PLATFORM_MIPSM_H_STRUCTS__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEFS__
#undef __HDR_DEFS__
/*****************************************************************************/

/*****************************************************************************/
/* __RMP_PLATFORM_MIPSM_H_STRUCTS__ */
#endif
/* __HDR_STRUCTS__ */
#endif
/* End Structs ***************************************************************/

/* Private Global Variables **************************************************/
#if(!(defined __HDR_DEFS__||defined __HDR_STRUCTS__))
#ifndef __RMP_PLATFORM_MIPSM_MEMBERS__
#define __RMP_PLATFORM_MIPSM_MEMBERS__

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
__EXTERN__ volatile rmp_ptr_t RMP_GP_Val;
__EXTERN__ volatile rmp_ptr_t RMP_SP_Val;
__EXTERN__ volatile rmp_ptr_t RMP_Int_Nest;
/*****************************************************************************/

/* End Public Global Variables ***********************************************/

/* Public C Function Prototypes **********************************************/
/*****************************************************************************/
/* Interrupts */
EXTERN void RMP_Int_Disable(void);
EXTERN void RMP_Int_Enable(void);

EXTERN rmp_ptr_t RMP_MSB_Get(rmp_ptr_t Val);
EXTERN void _RMP_Start(rmp_ptr_t Entry, rmp_ptr_t Stack);
EXTERN void _RMP_Yield(void);
EXTERN void _RMP_Set_Timer(rmp_ptr_t Ticks);

/* Initialization */
__EXTERN__ void _RMP_Stack_Init(rmp_ptr_t Entry, rmp_ptr_t Stack, rmp_ptr_t Arg);
__EXTERN__ void _RMP_Low_Level_Init(void);
__EXTERN__ void RMP_Putchar(char Char);
__EXTERN__ void _RMP_Plat_Hook(void);

/* Platform-dependent hooks */
__EXTERN__ void _RMP_Clear_Soft_Flag(void);
__EXTERN__ void _RMP_Clear_Timer_Flag(void);
/*****************************************************************************/
/* Undefine "__EXTERN__" to avoid redefinition */
#undef __EXTERN__
/* __RMP_PLATFORM_MIPSM_MEMBERS__ */
#endif
/* !(defined __HDR_DEFS__||defined __HDR_STRUCTS__) */
#endif
/* End Public C Function Prototypes ******************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
