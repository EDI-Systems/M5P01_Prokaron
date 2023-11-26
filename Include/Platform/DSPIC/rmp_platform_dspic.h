/******************************************************************************
Filename    : rmp_platform_dspic.h
Author      : pry
Date        : 23/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_dspic.c".
******************************************************************************/

/* Define ********************************************************************/
#ifdef __HDR_DEF__
#ifndef __RMP_PLATFORM_DSPIC_DEF__
#define __RMP_PLATFORM_DSPIC_DEF__
/*****************************************************************************/
/* Basic Types ***************************************************************/
#ifndef __RMP_S32_T__
#define __RMP_S32_T__
typedef signed long rmp_s32_t;
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
typedef unsigned long rmp_u32_t;
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
typedef rmp_u16_t rmp_ptr_t;
#endif

#ifndef __RMP_CNT_T__
#define __RMP_CNT_T__
/* The typedef for the count variables */
typedef rmp_s16_t rmp_cnt_t;
#endif

#ifndef __RMP_RET_T__
#define __RMP_RET_T__
/* The type for process return value */
typedef rmp_s16_t rmp_ret_t;
#endif
/* End Extended Types ********************************************************/

/* System macros *************************************************************/
/* Compiler "extern" keyword setting */
#define EXTERN                  extern
/* The order of bits in one CPU machine word */
#define RMP_WORD_ORDER          (4U)
/* The maximum length of char printing - no need to change this in most cases */
#define RMP_DEBUG_PRINT_MAX     (255U)
/* The offset of the stack when initializing */
#define RMP_INIT_STACK          RMP_INIT_STACK_ASCEND(2U)
/* MSB/LSB extraction */
#define RMP_MSB_GET(VAL)        RMP_DSPIC_MSB_Get(VAL)
#define RMP_LSB_GET(VAL)        RMP_DSPIC_LSB_Get(VAL)

/* The CPU and application specific macros are here */
#include "rmp_platform_dspic_conf.h"
/* End System macros *********************************************************/

/* DSPIC33 class specific macros *********************************************/

/*****************************************************************************/
/* __RMP_PLATFORM_DSPIC_DEF__ */
#endif
/* __HDR_DEF__ */
#endif
/* End Define ****************************************************************/

/* Struct ********************************************************************/
#ifdef __HDR_STRUCT__
#ifndef __RMP_PLATFORM_DSPIC_STRUCT__
#define __RMP_PLATFORM_DSPIC_STRUCT__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEF__
#undef __HDR_DEF__
/*****************************************************************************/
struct RMP_DSPIC_Stack
{
    /* Common registers */
    rmp_ptr_t PCL;
    rmp_ptr_t PCH;
    rmp_ptr_t SR;
    rmp_ptr_t W0;
    rmp_ptr_t W1;
    rmp_ptr_t W2;
    rmp_ptr_t W3;
    rmp_ptr_t W4;
    rmp_ptr_t W5;
    rmp_ptr_t W6;
    rmp_ptr_t W7;
    rmp_ptr_t W8;
    rmp_ptr_t W9;
    rmp_ptr_t W10;
    rmp_ptr_t W11;
    rmp_ptr_t W12;
    rmp_ptr_t W13;
    rmp_ptr_t W14;
    rmp_ptr_t ACCAL;
    rmp_ptr_t ACCAH;
    rmp_ptr_t ACCAU;
    rmp_ptr_t ACCBL;
    rmp_ptr_t ACCBH;
    rmp_ptr_t ACCBU;
    rmp_ptr_t DSRPAG;
    rmp_ptr_t DSWPAG;
    rmp_ptr_t RCOUNT;
    rmp_ptr_t DCOUNT;
    rmp_ptr_t DOSTARTL;
    rmp_ptr_t DOSTARTH;
    rmp_ptr_t DOENDL;
    rmp_ptr_t DOENDH;
    rmp_ptr_t CORCON;
    rmp_ptr_t MODCON;
    rmp_ptr_t XMODSRT;
    rmp_ptr_t XMODEND;
    rmp_ptr_t YMODSRT;
    rmp_ptr_t YMODEND;
    rmp_ptr_t XBREV;
    rmp_ptr_t TBLPAG;
    rmp_ptr_t MSTRPR;
};
/*****************************************************************************/
/* __RMP_PLATFORM_DSPIC_STRUCT__ */
#endif
/* __HDR_STRUCT__ */
#endif
/* End Struct ****************************************************************/

/* Private Variable **********************************************************/
#if(!(defined __HDR_DEF__||defined __HDR_STRUCT__))
#ifndef __RMP_PLATFORM_DSPIC_MEMBER__
#define __RMP_PLATFORM_DSPIC_MEMBER__

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
__EXTERN__ volatile rmp_ptr_t RMP_SP_Val;
__EXTERN__ volatile rmp_ptr_t RMP_Int_Nest;
__EXTERN__ volatile rmp_ptr_t RMP_TBLPAG_Val;
__EXTERN__ volatile rmp_ptr_t RMP_DSRPAG_Val;
__EXTERN__ volatile rmp_ptr_t RMP_DSWPAG_Val;
/*****************************************************************************/

/* End Public Variable *******************************************************/

/* Public Function ***********************************************************/
/*****************************************************************************/
/* Interrupts */
EXTERN void RMP_Int_Disable(void);
EXTERN void RMP_Int_Enable(void);

EXTERN rmp_ptr_t RMP_DSPIC_MSB_Get(rmp_ptr_t Value);
EXTERN rmp_ptr_t RMP_DSPIC_LSB_Get(rmp_ptr_t Value);
EXTERN void _RMP_Start(rmp_ptr_t Entry, rmp_ptr_t Stack);
__EXTERN__ void _RMP_Yield(void);
__EXTERN__ void _RMP_Set_Timer(rmp_ptr_t Ticks);

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
/* __RMP_PLATFORM_DSPIC_MEMBER__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
