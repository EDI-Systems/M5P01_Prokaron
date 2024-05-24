/******************************************************************************
Filename    : rmp_platform_avr.h
Author      : pry
Date        : 01/04/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_avr.c".
              This port supports both MegaAVR and XMegaAVR but not TinyAVR.
              Supported cores include AVRe, AVRe+, AVRxm and AVRxt.
              Please refrain from trying to use this port on chips that has
              less than 32kB of Flash, because the kernel uses about 16kB.
              In contrast, the IAR compiler is expected to generate less code
              through the extensive use of static overlay (rather than the GCC
              software stack as most 8-bitters lack SP-relative addressing),
              however this scheme precludes the porting of the kernel.
              This port is supplied as a proof of existence of RMP on even
              8-bit devices rather than to be used in a production setting;
              AVR is not particularly great in term of code density when
              compared with other 8-bitters such as PIC, STM8, and even 8051.
              All kernel functions assume zero for all RAMP/EIND segment 
              registers; still, they are saved and restored as a part of the
              context switch, and are cleared before calling any ISR written
              in C. This is in accordance with the GCC's use of these registers,
              and the user is responsible for clearing them when they are
              changed before calling the kernel APIs.
******************************************************************************/

/* Define ********************************************************************/
#ifdef __HDR_DEF__
#ifndef __RMP_PLATFORM_AVR_DEF__
#define __RMP_PLATFORM_AVR_DEF__
/*****************************************************************************/
/* Basic Types ***************************************************************/
#ifndef __RMP_S32_T__
#define __RMP_S32_T__
typedef signed long rmp_s32_t;
#endif

#ifndef __RMP_S16_T__
#define __RMP_S16_T__
typedef signed int rmp_s16_t;
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
typedef unsigned int rmp_u16_t;
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
#define EXTERN                          extern
/* The order of bits in one CPU machine word */
#define RMP_WORD_ORDER                  (4U)
/* The maximum length of char printing - no need to change this in most cases */
#define RMP_DEBUG_PRINT_MAX             (255U)
/* Descending stack, no alignment */
#define RMP_INIT_STACK                  RMP_INIT_STACK_DESCEND(0U)
/* MSB/LSB extraction */
#define RMP_MSB_GET(VAL)                RMP_MSB_Generic(VAL)
#define RMP_LSB_GET(VAL)                RMP_LSB_Generic(VAL)

/* The CPU and application specific macros are here */
#include "rmp_platform_avr_conf.h"
/* End System macros *********************************************************/
/*****************************************************************************/
/* __RMP_PLATFORM_AVR_DEF__ */
#endif
/* __HDR_DEF__ */
#endif
/* End Define ****************************************************************/

/* Struct ********************************************************************/
#ifdef __HDR_STRUCT__
#ifndef __RMP_PLATFORM_AVR_STRUCT__
#define __RMP_PLATFORM_AVR_STRUCT__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEF__
#undef __HDR_DEF__
/*****************************************************************************/
struct RMP_AVR_Stack
{
#if(RMP_AVR_COP_EIND!=0U)
    rmp_u8_t EIND_ZU;
#endif
    
#if((RMP_AVR_COP_RAMP!=0U)||(RMP_AVR_COP_EIND!=0U))
    rmp_u8_t RAMPD_ZU;
    rmp_u8_t RAMPX_XU;
    rmp_u8_t RAMPY_YU;
    rmp_u8_t RAMPZ_ZU;
#endif

    rmp_u8_t R0;
    rmp_u8_t R1;
    rmp_u8_t R2;
    rmp_u8_t R3;
    rmp_u8_t R4;
    rmp_u8_t R5;
    rmp_u8_t R6;
    rmp_u8_t R7;
    rmp_u8_t R8;
    rmp_u8_t R9;
    rmp_u8_t R10;
    rmp_u8_t R11;
    rmp_u8_t R12;
    rmp_u8_t R13;
    rmp_u8_t R14;
    rmp_u8_t R15;
    rmp_u8_t R16;
    rmp_u8_t R17;
    rmp_u8_t R18;
    rmp_u8_t R19;
    rmp_u8_t R20;
    rmp_u8_t R21;
    rmp_u8_t R22;
    rmp_u8_t R23;
    rmp_u8_t R24;
    rmp_u8_t R25;
    rmp_u8_t R26_XL;
    rmp_u8_t R27_XH;
    rmp_u8_t R28_YL;
    rmp_u8_t SREG_SR;
    rmp_u8_t R29_YH;
    rmp_u8_t R30_ZL;
    rmp_u8_t R31_ZH;
    /* Big-endian for CALL and interrupt entry PC */
#if(RMP_AVR_COP_EIND!=0U)
    rmp_u8_t PCU;
#endif
    rmp_u8_t PCH;
    rmp_u8_t PCL;
};
/*****************************************************************************/
/* __RMP_PLATFORM_AVR_STRUCT__ */
#endif
/* __HDR_STRUCT__ */
#endif
/* End Struct ****************************************************************/

/* Private Variable **********************************************************/
#if(!(defined __HDR_DEF__||defined __HDR_STRUCT__))
#ifndef __RMP_PLATFORM_AVR_MEMBER__
#define __RMP_PLATFORM_AVR_MEMBER__

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
__EXTERN__ rmp_ptr_t _RMP_AVR_SP_Kern;

__EXTERN__ volatile rmp_u8_t RMP_AVR_Int_Act;
__EXTERN__ volatile rmp_u8_t _RMP_AVR_Yield_Pend;
/*****************************************************************************/

/* End Public Variable *******************************************************/

/* Public Function ***********************************************************/
/*****************************************************************************/
/* Interrupts */
EXTERN void RMP_Int_Disable(void);
EXTERN void RMP_Int_Enable(void);
EXTERN void RMP_Int_Mask(rmp_u8_t Level);

EXTERN void _RMP_Start(rmp_ptr_t Entry, rmp_ptr_t Stack);
__EXTERN__ void _RMP_Yield(void);

/* Platform specific */
EXTERN void _RMP_AVR_Yield_MEGA(void);
EXTERN void _RMP_AVR_Yield_MEGA_RAMP(void);
EXTERN void _RMP_AVR_Yield_MEGA_EIND(void);
EXTERN void _RMP_AVR_Yield_XMEGA(void);
EXTERN void _RMP_AVR_Yield_XMEGA_RAMP(void);
EXTERN void _RMP_AVR_Yield_XMEGA_EIND(void);

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
/* __RMP_PLATFORM_AVR_MEMBER__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
