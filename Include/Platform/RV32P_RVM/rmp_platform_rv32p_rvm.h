/******************************************************************************
Filename    : rmp_platform_rv32p_rvm.h
Author      : pry
Date        : 01/04/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_rv32p_rvm.c".
******************************************************************************/

/* Define ********************************************************************/
#ifdef __HDR_DEF__
#ifndef __RMP_PLATFORM_RV32P_RVM_DEF__
#define __RMP_PLATFORM_RV32P_RVM_DEF__
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
/* The maximum length of char printing - no need to change this in most cases */
#define RMP_DEBUG_PRINT_MAX             (255U)
/* Descending stack, 16-byte alignment */
#define RMP_INIT_STACK                  RMP_INIT_STACK_DESCEND(4U)
/* MSB/LSB extraction */
#define RMP_MSB_GET(VAL)                RMP_MSB_Generic(VAL)
#define RMP_LSB_GET(VAL)                RMP_LSB_Generic(VAL)

/* FPU registers */
#define RMP_FPU                         ((volatile struct RVM_RV32P_Cop_Struct*)(RVM_REG->Cop))

/* The virtual machine configs are here */
#include "rvm_guest_conf.h"

/* The CPU and application specific macros are here */
#include "rmp_platform_rv32p_rvm_conf.h"
/* End System macros *********************************************************/
/*****************************************************************************/
/* __RMP_PLATFORM_RV32P_RVM_DEF__ */
#endif
/* __HDR_DEF__ */
#endif
/* End Define ****************************************************************/

/* Struct ********************************************************************/
#ifdef __HDR_STRUCT__
#ifndef __RMP_PLATFORM_RV32P_RVM_STRUCT__
#define __RMP_PLATFORM_RV32P_RVM_STRUCT__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEF__
#undef __HDR_DEF__
/*****************************************************************************/
struct RMP_RV32P_RVM_Stack
{
    rmp_ptr_t Number;
    rmp_ptr_t Param0;
    rmp_ptr_t Param1;
    rmp_ptr_t Param2;
    rmp_ptr_t Param3;
    rmp_ptr_t MSTATUS;
    rmp_ptr_t PC;
    rmp_ptr_t X1_RA;
    rmp_ptr_t X3_GP;
    rmp_ptr_t X4_TP;
    rmp_ptr_t X5_T0;
    rmp_ptr_t X6_T1;
    rmp_ptr_t X7_T2;
    rmp_ptr_t X8_S0_FP;
    rmp_ptr_t X9_S1;
    rmp_ptr_t X10_A0;
    rmp_ptr_t X11_A1;
    rmp_ptr_t X12_A2;
    rmp_ptr_t X13_A3;
    rmp_ptr_t X14_A4;
    rmp_ptr_t X15_A5;
    rmp_ptr_t X16_A6;
    rmp_ptr_t X17_A7;
    rmp_ptr_t X18_S2;
    rmp_ptr_t X19_S3;
    rmp_ptr_t X20_S4;
    rmp_ptr_t X21_S5;
    rmp_ptr_t X22_S6;
    rmp_ptr_t X23_S7;
    rmp_ptr_t X24_S8;
    rmp_ptr_t X25_S9;
    rmp_ptr_t X26_S10;
    rmp_ptr_t X27_S11;
    rmp_ptr_t X28_T3;
    rmp_ptr_t X29_T4;
    rmp_ptr_t X30_T5;
    rmp_ptr_t X31_T6;
};
/*****************************************************************************/
/* __RMP_PLATFORM_RV32P_RVM_STRUCT__ */
#endif
/* __HDR_STRUCT__ */
#endif
/* End Struct ****************************************************************/

/* Private Variable **********************************************************/
#if(!(defined __HDR_DEF__||defined __HDR_STRUCT__))
#ifndef __RMP_PLATFORM_RV32P_RVM_MEMBER__
#define __RMP_PLATFORM_RV32P_RVM_MEMBER__

/* In this way we can use the data structures and definitions in the headers */
#define __HDR_DEF__

#undef __HDR_DEF__

#define __HDR_STRUCT__

#undef __HDR_STRUCT__

/* If the header is not used in the public mode */
#ifndef __HDR_PUBLIC__
/*****************************************************************************/
static rvm_ptr_t RMP_Console_Ptr;
EXTERN const rvm_ptr_t RVM_Desc[];
/*****************************************************************************/
/* End Private Variable ******************************************************/

/* Private Function **********************************************************/ 
/*****************************************************************************/
/* Interrupts */
static void RMP_Ctx_Handler(void);
static void RMP_Tim_Handler(void);
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
EXTERN rmp_ptr_t _RMP_Global;
/*****************************************************************************/

/* End Public Variable *******************************************************/

/* Public Function ***********************************************************/
/*****************************************************************************/
/* Interrupts */
__EXTERN__ void RMP_Int_Enable(void);
__EXTERN__ void RMP_Int_Disable(void);
__EXTERN__ void RMP_Int_Mask(void);
__EXTERN__ void RMP_Int_Unmask(void);

EXTERN void _RMP_Start(rmp_ptr_t Entry,
                       rmp_ptr_t Stack);
EXTERN void _RMP_RV32P_RVM_Yield(void);
__EXTERN__ void _RMP_Yield(void);

/* Platform specific */
EXTERN void _RMP_RV32P_RVM_Yield_NONE(void);
EXTERN void _RMP_RV32P_RVM_Yield_RVF(void);
EXTERN void _RMP_RV32P_RVM_Yield_RVFD(void);

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
/* __RMP_PLATFORM_RV32P_RVM_MEMBER__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/

