/******************************************************************************
Filename    : rmp_platform_unsp.h
Author      : pry
Date        : 25/05/2024
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_unsp.c".
              This port supports both unSP 1.2 and 2.0 (2.0 have 8 extra regs).
              The only chip that this port plans to test is the SPCE061A, an
              obsolete microcontroller at the heart of the Sunplus University
              Program that vanished decades ago. The chip never had a RTOS port,
              at least not a pronounced one, during its entire product lifetime.
              This port is provided as a monument for the golden age of MCU
              innovation circa 1980-2005, at which time multiple architectures
              rapidly emerged and strived to become popular on the market. Their
              efforts included university programs, competition sponsors, free
              training sessions and free hardware giveouts. Yet, very few were
              successful (and lucky) enough to survive more than two decades.
              As a retro port, this targets the unSPIDE V1.8.4 with GCC 1.0.10,
              (based on vanilla GCC 2.95.2, source released nearly a decade after
              the binary release) released 31/10/2003. It is extracted from the
              "61 board" (version A, self hand soldering required) accessory CD,
              and is the exact one that Sunplus used for its university program.
              Sunplus was a company that focused on MCUs & MPUs, and already had
              streaks of successes in arcade consoles, talking toys and electronic
              dictionaries circa 2k. Many arcade consoles were powered by their
              devices and you can find corresponding simulators even in the MAME;
              they even had an in-house S+ core that could boot the Linux kernel.
              The company then went through a series of organizational changes,
              and its departments were detached subsequently circa 2010 to operate
              as subsidiaries or even independently. The result was a Sunplus that
              concentrated on silicon IP licensing & car HMI and a list of smaller
              companies that focused on their dedicated niche markets. Today, the
              MCU department of the original Sunplus lives under the name of
              Generalplus with moderate financial success in consumer electronics,
              and is still producing legacy unSP (and unSP 2.0) MCUs that this
              port can theoretically support.
              Like C28x, char is 16 bits on this machine. This port is somewhat
              usable as the benchmark occupies under ~35% of ROM. As the chip was
              intended for audio applications, the benchmark will terminate with
              an endless original Sunplus University Program CD BGM playback. You
              will be able to hear the sound from decades ago if you happen to
              have a intact "61 board", a parallel programmer, and a parallel
              port-capable vintage computer running WinXP at hand.
******************************************************************************/

/* Define ********************************************************************/
#ifdef __HDR_DEF__
#ifndef __RMP_PLATFORM_UNSP_DEF__
#define __RMP_PLATFORM_UNSP_DEF__
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
/* The maximum length of char printing */
#define RMP_DEBUG_PRINT_MAX             (255U)
/* Empty descending stack of rmp_ptr_t, word-addressing with 1-word alignment */
#define RMP_STACK_TYPE                  RMP_STACK_EMPTY_DESCEND
#define RMP_STACK_ALIGN                 (0U)
#define RMP_STACK_ELEM                  rmp_ptr_t
#define RMP_STACK_STRUCT                struct RMP_UNSP_Stack
/* MSB/LSB extraction */
#define RMP_MSB_GET(VAL)                RMP_MSB_Generic(VAL)
#define RMP_LSB_GET(VAL)                RMP_LSB_Generic(VAL)

/* The CPU and application specific macros are here */
#include "rmp_platform_unsp_conf.h"
/* End System macros *********************************************************/
/*****************************************************************************/
/* __RMP_PLATFORM_UNSP_DEF__ */
#endif
/* __HDR_DEF__ */
#endif
/* End Define ****************************************************************/

/* Struct ********************************************************************/
#ifdef __HDR_STRUCT__
#ifndef __RMP_PLATFORM_UNSP_STRUCT__
#define __RMP_PLATFORM_UNSP_STRUCT__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEF__
#undef __HDR_DEF__
/*****************************************************************************/
struct RMP_UNSP_Stack
{
    rmp_ptr_t R1;
    rmp_ptr_t R2;
    rmp_ptr_t R3_MRL;
    rmp_ptr_t R4_MRH;
    rmp_ptr_t R5_BP;
#if(RMP_UNSP_COP_SPV2!=0U)
    rmp_ptr_t R8;
    rmp_ptr_t R9;
    rmp_ptr_t R10;
    rmp_ptr_t R11;
    rmp_ptr_t R12;
    rmp_ptr_t R13;
    rmp_ptr_t R14;
    rmp_ptr_t R15;
#endif
    rmp_ptr_t SR_CSDS;
    rmp_ptr_t PC;
    /* The Sunplus compiler always use R1 to push parameters to stack in a
     * last-to-first order, hence it could be said that the first parameter
     * of a function is always passed in R1. However, the canonical calling
     * convention says that it shall be passed on stack, which we abide by. */
    rmp_ptr_t Param;
};
/*****************************************************************************/
/* __RMP_PLATFORM_UNSP_STRUCT__ */
#endif
/* __HDR_STRUCT__ */
#endif
/* End Struct ****************************************************************/

/* Private Variable **********************************************************/
#if(!(defined __HDR_DEF__||defined __HDR_STRUCT__))
#ifndef __RMP_PLATFORM_UNSP_MEMBER__
#define __RMP_PLATFORM_UNSP_MEMBER__

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
__EXTERN__ rmp_ptr_t _RMP_UNSP_SP_Kern;

__EXTERN__ volatile rmp_u8_t RMP_UNSP_Int_Act;
__EXTERN__ volatile rmp_u8_t _RMP_UNSP_Yield_Pend;
/*****************************************************************************/

/* End Public Variable *******************************************************/

/* Public Function ***********************************************************/
/*****************************************************************************/
/* Interrupts */
EXTERN void RMP_Int_Disable(void);
EXTERN void RMP_Int_Enable(void);
EXTERN void RMP_Int_Mask(rmp_u8_t Level);

EXTERN void _RMP_Start(rmp_ptr_t Entry,
                       rmp_ptr_t Stack);
__EXTERN__ void _RMP_Yield(void);

/* Platform specific */
EXTERN void _RMP_UNSP_Yield_SPV1(void);
EXTERN void _RMP_UNSP_Yield_SPV2(void);
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
/* __RMP_PLATFORM_UNSP_MEMBER__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
