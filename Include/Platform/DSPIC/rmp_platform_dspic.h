/******************************************************************************
Filename    : rmp_platform_dspic.h
Author      : pry
Date        : 23/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_dspic.c".
              1. This port assumes that all pointers are 16-bit. Hence, all
                 function pointers and kernel data structures shall be in the 
                 first 64KiB.
              2. This port makes use of the IPL and does not disable interrupts
                 all the time. However, this assumes the IPL of all kernel-aware
                 ISRs is 1, which must be adhered to when initializing them,
              3. The DSPIC architecture does not allow context switching of the
                 internal DO stack, and this cannot be worked around, Possible
                 mitigations are to make sure (1) no DO instructions are used,
                 and this is easy because compiler does not emit them, (2) only
                 one of the threads will ever make of the DO instruction, and
                 this is easy too because that thread usually carries all the
                 DSP workload.
              4. On 30F and 33F devices, the SR overflow bits are read-only and
                 this must be accounted for when context switching the DSP tasks.
                 Still, it is recommended that only one thread makes use of the
                 DSP features,
              5. For address modulo features involving MODCON, it is the user's
                 responsibility to ensure that kernel data structures are out
                 of such modulo regions (i.e. the modulo only applies to 
                 certain user-defined data structures).
******************************************************************************/

/* Define ********************************************************************/
#ifdef __HDR_DEF__
#ifndef __RMP_PLATFORM_DSPIC_DEF__
#define __RMP_PLATFORM_DSPIC_DEF__
/*****************************************************************************/
/* Basic Type ****************************************************************/
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
/* End Basic Type ************************************************************/

/* Extended Type *************************************************************/
#ifndef __RMP_PTR_T__
#define __RMP_PTR_T__
/* Pointer */
typedef rmp_u16_t rmp_ptr_t;
#endif

#ifndef __RMP_CNT_T__
#define __RMP_CNT_T__
/* Counter */
typedef rmp_s16_t rmp_cnt_t;
#endif

#ifndef __RMP_RET_T__
#define __RMP_RET_T__
/* Return value */
typedef rmp_s16_t rmp_ret_t;
#endif
/* End Extended Type *********************************************************/

/* System Macro **************************************************************/
/* Compiler "extern" keyword setting */
#define RMP_EXTERN                          extern
/* The order of bits in one CPU machine word */
#define RMP_WORD_ORDER                  (4U)
/* The maximum length of char printing */
#define RMP_DEBUG_PRINT_MAX             (255U)
/* Empty ascending stack of rmp_ptr_t, 4-byte alignment */
#define RMP_STACK_TYPE                  RMP_STACK_EMPTY_ASCEND
#define RMP_STACK_ALIGN                 (2U)
#define RMP_STACK_ELEM                  rmp_ptr_t
#define RMP_STACK_STRUCT                struct RMP_DSPIC_Stack
/* MSB/LSB extraction */
#define RMP_MSB_GET(VAL)                RMP_DSPIC_MSB_Get(VAL)
#define RMP_LSB_GET(VAL)                RMP_DSPIC_LSB_Get(VAL)

/* The CPU and application specific macros are here */
#include "rmp_platform_dspic_conf.h"

/* Interrupt masking/unmasking */
#define RMP_INT_MASK()                  RMP_Int_Mask(0x01U)
#define RMP_INT_UNMASK()                RMP_Int_Mask(0x00U)
/* Yield operation */
#if(RMP_DSPIC_COP_24F_24H!=0U)
#define RMP_YIELD()                     _RMP_DSPIC_Yield_24F_24H()
#elif(RMP_DSPIC_COP_24E!=0U)
#define RMP_YIELD()                     _RMP_DSPIC_Yield_24E()
#elif(RMP_DSPIC_COP_30F_33F!=0U)
#define RMP_YIELD()                     _RMP_DSPIC_Yield_30F_33F()
#elif(RMP_DSPIC_COP_33E_33C!=0U)
#define RMP_YIELD()                     _RMP_DSPIC_Yield_33E_33C()
#endif
/* #define RMP_YIELD_ISR() */

/* Detect wrong configurations here */
#define RMP_DSPIC_COP_NUM               (RMP_DSPIC_COP_24F_24H+ \
                                         RMP_DSPIC_COP_24E+ \
                                         RMP_DSPIC_COP_30F_33F+ \
                                         RMP_DSPIC_COP_33E_33C)
#if(RMP_DSPIC_COP_NUM!=1U)
#error Must choose a single CPU type.
#endif
/* End System Macro **********************************************************/

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
    rmp_ptr_t PCHSRL;
    rmp_ptr_t SR;
    rmp_ptr_t CORCON;
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
    rmp_ptr_t RCOUNT;
    rmp_ptr_t TBLPAG;
    /* Specific visibility registers */
#if((RMP_DSPIC_COP_24F_24H!=0U)||(RMP_DSPIC_COP_30F_33F!=0U))
    rmp_ptr_t PSVPAG;
#elif((RMP_DSPIC_COP_24E!=0U)||(RMP_DSPIC_COP_33E_33C!=0U))
    rmp_ptr_t DSRPAG;
    rmp_ptr_t DSWPAG;
#endif
    /* Specific DSP/addressing registers */
#if((RMP_DSPIC_COP_30F_33F!=0U)||(RMP_DSPIC_COP_33E_33C!=0U))
    rmp_ptr_t ACCAL;
    rmp_ptr_t ACCAH;
    rmp_ptr_t ACCAU;
    rmp_ptr_t ACCBL;
    rmp_ptr_t ACCBH;
    rmp_ptr_t ACCBU;
    rmp_ptr_t MODCON;
    rmp_ptr_t XMODSRT;
    rmp_ptr_t XMODEND;
    rmp_ptr_t YMODSRT;
    rmp_ptr_t YMODEND;
    rmp_ptr_t XBREV;
#endif
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
#define __RMP_EXTERN__
/* End Private Function ******************************************************/

/* Public Variable ***********************************************************/
/* __HDR_PUBLIC__ */
#else
#define __RMP_EXTERN__ RMP_EXTERN 
/* __HDR_PUBLIC__ */
#endif

/*****************************************************************************/
__RMP_EXTERN__ rmp_ptr_t _RMP_DSPIC_SP_Kern;
__RMP_EXTERN__ rmp_ptr_t _RMP_DSPIC_CORCON_Kern;
__RMP_EXTERN__ rmp_ptr_t _RMP_DSPIC_TBLPAG_Kern;
__RMP_EXTERN__ rmp_ptr_t _RMP_DSPIC_DSRPAG_Kern;
__RMP_EXTERN__ rmp_ptr_t _RMP_DSPIC_PSVDSWPAG_Kern;
/*****************************************************************************/

/* End Public Variable *******************************************************/

/* Public Function ***********************************************************/
/*****************************************************************************/
/* Interrupts */
RMP_EXTERN void RMP_Int_Disable(void);
RMP_EXTERN void RMP_Int_Enable(void);
RMP_EXTERN void RMP_Int_Mask(rmp_ptr_t Level);

RMP_EXTERN rmp_ptr_t RMP_DSPIC_MSB_Get(rmp_ptr_t Value);
RMP_EXTERN rmp_ptr_t RMP_DSPIC_LSB_Get(rmp_ptr_t Value);
RMP_EXTERN void _RMP_Start(rmp_ptr_t Entry, rmp_ptr_t Stack);
RMP_EXTERN void _RMP_DSPIC_Yield_24F_24H(void);
RMP_EXTERN void _RMP_DSPIC_Yield_24E(void);
RMP_EXTERN void _RMP_DSPIC_Yield_30F_33F(void);
RMP_EXTERN void _RMP_DSPIC_Yield_33E_33C(void);

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
/* __RMP_PLATFORM_DSPIC_MEMBER__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
