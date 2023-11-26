/******************************************************************************
Filename    : rmp_platform_mp32p.h
Author      : pry
Date        : 23/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_mp32p.c".
******************************************************************************/

/* Define ********************************************************************/
#ifdef __HDR_DEF__
#ifndef __RMP_PLATFORM_MP32P_DEF__
#define __RMP_PLATFORM_MP32P_DEF__
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
/* The maximum length of char printing - no need to change this in most cases */
#define RMP_DEBUG_PRINT_MAX             (255)
/* Descending stack, 8-byte alignment */
#define RMP_INIT_STACK                  RMP_INIT_STACK_DESCEND(3U)
/* MSB/LSB extraction */
#define RMP_MSB_GET(VAL)                _RMP_MP32P_MSB_Get(VAL)
#define RMP_LSB_GET(VAL)                _RMP_MP32P_LSB_Get(VAL)

/* The CPU and application specific macros are here */
#include "rmp_platform_mp32p_conf.h"
/* End System macros *********************************************************/

/* MIPS-M class specific macros **********************************************/
/* Bits within the CP0 STATUS register */
#define RMP_MP32P_STATUS_IE             (1U<<0)         /* Enable interrupts */
#define RMP_MP32P_STATUS_EXL            (1U<<1)         /* Exception level */
#define RMP_MP32P_STATUS_MX             (1U<<24)        /* Allow DSP instructions */
#define RMP_MP32P_STATUS_CU1            (0x20000000U)   /* Enable CP1 for parts with hardware */
#define RMP_MP32P_STATUS_FR             (0x04000000U)   /* Enable 64 bit floating point registers */

/* Bits within the CP0 CAUSE register */
#define RMP_MP32P_CAUSE_CORESW0         (0x00000100U)
#define RMP_MP32P_CAUSE_CORESW1         (0x00000200U)
/*****************************************************************************/
/* __RMP_PLATFORM_MP32P_DEF__ */
#endif
/* __HDR_DEF__ */
#endif
/* End Define ****************************************************************/

/* Struct ********************************************************************/
#ifdef __HDR_STRUCT__
#ifndef __RMP_PLATFORM_MP32P_STRUCT__
#define __RMP_PLATFORM_MP32P_STRUCT__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEF__
#undef __HDR_DEF__
/*****************************************************************************/
struct RMP_MP32P_Stack
{
    rmp_ptr_t R1_AT;
    rmp_ptr_t R2_V0;
    rmp_ptr_t R3_V1;
    rmp_ptr_t R4_A0;
    rmp_ptr_t R5_A1;
    rmp_ptr_t R6_A2;
    rmp_ptr_t R7_A3;
    rmp_ptr_t R8_T0;
    rmp_ptr_t R9_T1;
    rmp_ptr_t R10_T2;
    rmp_ptr_t R11_T3;
    rmp_ptr_t R12_T4;
    rmp_ptr_t R13_T5;
    rmp_ptr_t R14_T6;
    rmp_ptr_t R15_T7;
    rmp_ptr_t R16_S0;
    rmp_ptr_t R17_S1;
    rmp_ptr_t R18_S2;
    rmp_ptr_t R19_S3;
    rmp_ptr_t R24_T8;
    rmp_ptr_t R25_T9;
    rmp_ptr_t R30_FP;
    rmp_ptr_t R31_RA;
    rmp_ptr_t R26_K0;
    rmp_ptr_t R27_K1;
    rmp_ptr_t R28_GP;
    rmp_ptr_t LO;
    rmp_ptr_t HI;
    rmp_ptr_t STATUS;
    rmp_ptr_t PC;
    rmp_ptr_t R20_S4;
    rmp_ptr_t R21_S5;
    rmp_ptr_t R22_S6;
    rmp_ptr_t R23_S7;
};
/*****************************************************************************/
/* __RMP_PLATFORM_MP32P_STRUCT__ */
#endif
/* __HDR_STRUCT__ */
#endif
/* End Struct ****************************************************************/

/* Private Variable **********************************************************/
#if(!(defined __HDR_DEF__||defined __HDR_STRUCT__))
#ifndef __RMP_PLATFORM_MP32P_PUBLIC__
#define __RMP_PLATFORM_MP32P_PUBLIC__

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
__EXTERN__ volatile rmp_ptr_t RMP_GP_Val;
__EXTERN__ volatile rmp_ptr_t RMP_SP_Val;
__EXTERN__ volatile rmp_ptr_t RMP_Int_Nest;
/*****************************************************************************/

/* End Public Variable *******************************************************/

/* Public Function ***********************************************************/
/*****************************************************************************/
/* Interrupts */
EXTERN void RMP_Int_Disable(void);
EXTERN void RMP_Int_Enable(void);

EXTERN rmp_ptr_t _RMP_MP32P_MSB_Get(rmp_ptr_t Value);
EXTERN rmp_ptr_t _RMP_MP32P_LSB_Get(rmp_ptr_t Value);
EXTERN void _RMP_Start(rmp_ptr_t Entry, rmp_ptr_t Stack);
EXTERN void _RMP_Yield(void);
EXTERN void _RMP_Set_Timer(rmp_ptr_t Ticks);

/* Initialization */
__EXTERN__ rmp_ptr_t _RMP_Stack_Init(rmp_ptr_t Stack,
                                     rmp_ptr_t Size,
                                     rmp_ptr_t Entry,
                                     rmp_ptr_t Param);
__EXTERN__ void _RMP_Lowlvl_Init(void);
__EXTERN__ void RMP_Putchar(char Char);
__EXTERN__ void _RMP_Plat_Hook(void);

/* Platform-dependent hooks */
__EXTERN__ void _RMP_Clear_Soft_Flag(void);
__EXTERN__ void _RMP_Clear_Timer_Flag(void);
/*****************************************************************************/
/* Undefine "__EXTERN__" to avoid redefinition */
#undef __EXTERN__
/* __RMP_PLATFORM_MP32P_PUBLIC__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
