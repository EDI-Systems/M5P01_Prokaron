/******************************************************************************
Filename    : rmp_platform_mp32p.h
Author      : pry
Date        : 23/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_mp32p.c".
              This supports MIPS version 5, with DSPASE and FR32/FR64, while
              assuming a physical memory model. This port respects the load, FPU
              move and branch delay slots and thus will be compatible with a
              wide range of MIPS processors. FPU instructions are coded in words
              to be compatible with big-endian variants as well.
              All threads in the system must use the same FPU model; we do not
              support cases where a thread uses FR32 while the other uses FR64.
              While it is possible to support such behavior for compatibility
              with existing binaries, doing so will complicate the code too much
              and compromise WCET.
              The primary MIPS chips nowadays are the PIC32, so this is only
              tested with them. And even MIPS PIC32s are going away; other MIPS
              chips do not belong to microcontroller class and are hence not
              supported here. As a port dedicated to the funeral of MIPS, no
              further chips will be added to this port.
              -----------------------------------------------------------------
              MIPS in terms of microcontrollers is pretty dead by now mainly due
              to its fragmented ecosystem. Different from other licensors that
              restrict or even disallow modifications to the ISA, MIPS actively
              encouraged such customizations without due administration. As a 
              general-purpose processor, such customizations probably traded too
              much compatibility for performance; and the performance is nowhere
              near that of DSPs or ASSPs where full customizations do not hurt
              because this their confidential binary interfaces do not need to
              remain compatible over generations.
              -----------------------------------------------------------------
              Another reason for the MIPS failure is its marketing strategy
              which lacks concentration. MIPS was never (really) popular in the
              desktop arena because it was expensive, and was never (really)
              popular in the embedded arena because it cost too much silicon
              area (L1I$) and power. They do, however, enjoy limited success in 
              telecommunication ASSPs, computer peripherals, video game consoles
              and industry control applications, and this sufficient to declare
              a moderate financial success.
              -----------------------------------------------------------------
              However, it does not seem that the RISC-V community is learning
              much from the MIPS failure. Incompatibilities in basic processor
              behaviors, i.e. exception handling, interrupt controller, memory
              protection units, compressed instruction forms have already taken
              off. The only lesson we learn from history is that no lesson will
              be ever learnt; good luck with that, and long live RISC-V!
              -----------------------------------------------------------------
              Nuff'said, for language/ISA/API standards, we are basically PAYING
              the standard vendor to RESTRICT our freedom TO diverge so we can
              HAVE freedom FROM a fragmented ecosystem. As a matter of fact, a
              more generalized form of this freedom paradox exists in ubiquity.
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
/* The maximum length of char printing */
#define RMP_DEBUG_PRINT_MAX             (255U)
/* Full descending stack of rmp_ptr_t, 8-byte alignment */
#define RMP_STACK_TYPE                  RMP_STACK_FULL_DESCEND
#define RMP_STACK_ALIGN                 (3U)
#define RMP_STACK_ELEM                  rmp_ptr_t
#define RMP_STACK_STRUCT                struct RMP_MP32P_Stack
/* MSB/LSB extraction */
#define RMP_MSB_GET(VAL)                _RMP_MP32P_MSB_Get(VAL)
#define RMP_LSB_GET(VAL)                _RMP_MP32P_LSB_Get(VAL)

/* The CPU and application specific macros are here */
#include "rmp_platform_mp32p_conf.h"

/* Detect wrong configurations here */
#if((RMP_MP32P_COP_FR32!=0U)&&(RMP_MP32P_COP_FR64!=0U))
#error Must choose no more than one FPU type.
#endif
/* End System macros *********************************************************/

/* MIPS-M class specific macros **********************************************/
/* Bits within the CP0 STATUS register */
#define RMP_MP32P_STATUS_IE             RMP_POW2(0U)    /* Enable interrupts */
#define RMP_MP32P_STATUS_EXL            RMP_POW2(1U)    /* Exception level */
#define RMP_MP32P_STATUS_MX             RMP_POW2(24U)   /* Allow DSP instructions */
#define RMP_MP32P_STATUS_CU0            RMP_POW2(28U)   /* Enable exception control manipulation */
#define RMP_MP32P_STATUS_CU1            RMP_POW2(29U)   /* Enable FPU (CP1) for parts with it */
#define RMP_MP32P_STATUS_FR             RMP_POW2(26U)   /* Enable 64 bit floating point registers */

/* Bits within the CP1 FCSR register */
#define RMP_MP32P_FCSR_MAC2008          RMP_POW2(20U)   /* Fused MAC support */
#define RMP_MP32P_FCSR_ABS2008          RMP_POW2(19U)   /* Compliant ABS support */
#define RMP_MP32P_FCSR_NAN2008          RMP_POW2(18U)   /* Compliant NAN support */
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
    /* MIPS FR64 FPU normal mode */
#if(RMP_MP32P_COP_FR64!=0U)
    rmp_ptr_t F0[2];
    rmp_ptr_t F1[2];
    rmp_ptr_t F2[2];
    rmp_ptr_t F3[2];
    rmp_ptr_t F4[2];
    rmp_ptr_t F5[2];
    rmp_ptr_t F6[2];
    rmp_ptr_t F7[2];
    rmp_ptr_t F8[2];
    rmp_ptr_t F9[2];
    rmp_ptr_t F10[2];
    rmp_ptr_t F11[2];
    rmp_ptr_t F12[2];
    rmp_ptr_t F13[2];
    rmp_ptr_t F14[2];
    rmp_ptr_t F15[2];
    rmp_ptr_t F16[2];
    rmp_ptr_t F17[2];
    rmp_ptr_t F18[2];
    rmp_ptr_t F19[2];
    rmp_ptr_t F20[2];
    rmp_ptr_t F21[2];
    rmp_ptr_t F22[2];
    rmp_ptr_t F23[2];
    rmp_ptr_t F24[2];
    rmp_ptr_t F25[2];
    rmp_ptr_t F26[2];
    rmp_ptr_t F27[2];
    rmp_ptr_t F28[2];
    rmp_ptr_t F29[2];
    rmp_ptr_t F30[2];
    rmp_ptr_t F31[2];
    /* Make stack 8-byte aligned even in interrupts */
    rmp_ptr_t FCSR;
    rmp_ptr_t DUMMY;
    /* MIPS FR32 FPU compatibility mode */
#elif(RMP_MP32P_COP_FR32!=0U)
    rmp_ptr_t F0;
    rmp_ptr_t F1;
    rmp_ptr_t F2;
    rmp_ptr_t F3;
    rmp_ptr_t F4;
    rmp_ptr_t F5;
    rmp_ptr_t F6;
    rmp_ptr_t F7;
    rmp_ptr_t F8;
    rmp_ptr_t F9;
    rmp_ptr_t F10;
    rmp_ptr_t F11;
    rmp_ptr_t F12;
    rmp_ptr_t F13;
    rmp_ptr_t F14;
    rmp_ptr_t F15;
    rmp_ptr_t F16;
    rmp_ptr_t F17;
    rmp_ptr_t F18;
    rmp_ptr_t F19;
    rmp_ptr_t F20;
    rmp_ptr_t F21;
    rmp_ptr_t F22;
    rmp_ptr_t F23;
    rmp_ptr_t F24;
    rmp_ptr_t F25;
    rmp_ptr_t F26;
    rmp_ptr_t F27;
    rmp_ptr_t F28;
    rmp_ptr_t F29;
    rmp_ptr_t F30;
    rmp_ptr_t F31;
    /* Make stack 8-byte aligned even in interrupts */
    rmp_ptr_t FCSR;
    rmp_ptr_t DUMMY;
#endif
    
    /* MIPS DSP ASE extension */
#if(RMP_MP32P_COP_DSPASE!=0U)
    rmp_ptr_t A3_LO;
    rmp_ptr_t A3_HI;
    rmp_ptr_t A2_LO;
    rmp_ptr_t A2_HI;
    rmp_ptr_t A1_LO;
    rmp_ptr_t A1_HI;
#endif
    
    rmp_ptr_t LO;
    rmp_ptr_t HI;
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
    rmp_ptr_t R20_S4;
    rmp_ptr_t R21_S5;
    rmp_ptr_t R24_T8;
    rmp_ptr_t R25_T9;
    rmp_ptr_t R26_K0;
    rmp_ptr_t R27_K1;
    rmp_ptr_t R28_GP;
    rmp_ptr_t R30_FP;
    rmp_ptr_t R31_RA;
    rmp_ptr_t STATUS;
    rmp_ptr_t PC;
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
#ifndef __RMP_PLATFORM_MP32P_MEMBER__
#define __RMP_PLATFORM_MP32P_MEMBER__

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
__EXTERN__ volatile rmp_ptr_t _RMP_MP32P_SP_Kern;
__EXTERN__ volatile rmp_ptr_t _RMP_MP32P_GP_Kern;

__EXTERN__ volatile rmp_ptr_t RMP_MP32P_Int_Act;
__EXTERN__ volatile rmp_ptr_t _RMP_MP32P_Yield_Pend;
/*****************************************************************************/

/* End Public Variable *******************************************************/

/* Public Function ***********************************************************/
/*****************************************************************************/
/* Interrupts */
EXTERN void RMP_Int_Disable(void);
EXTERN void RMP_Int_Enable(void);
EXTERN void RMP_Int_Mask(rmp_ptr_t Level);

EXTERN rmp_ptr_t _RMP_MP32P_MSB_Get(rmp_ptr_t Value);
EXTERN rmp_ptr_t _RMP_MP32P_LSB_Get(rmp_ptr_t Value);

EXTERN void _RMP_Start(rmp_ptr_t Entry, rmp_ptr_t Stack);

EXTERN void _RMP_MP32P_Yield_NONE(void);
EXTERN void _RMP_MP32P_Yield_DSPASE(void);
EXTERN void _RMP_MP32P_Yield_FR32(void);
EXTERN void _RMP_MP32P_Yield_FR64(void);
EXTERN void _RMP_MP32P_Yield_DSPASE_FR32(void);
EXTERN void _RMP_MP32P_Yield_DSPASE_FR64(void);
__EXTERN__ void _RMP_Yield(void);

/* Initialization */
__EXTERN__ rmp_ptr_t _RMP_Stack_Init(rmp_ptr_t Stack,
                                     rmp_ptr_t Size,
                                     rmp_ptr_t Entry,
                                     rmp_ptr_t Param);
__EXTERN__ void _RMP_Lowlvl_Init(void);
__EXTERN__ void RMP_Putchar(char Char);
__EXTERN__ void _RMP_Plat_Hook(void);

/* Timer handler routine */
__EXTERN__ void _RMP_MP32P_Tim_Handler(void);
/*****************************************************************************/
/* Undefine "__EXTERN__" to avoid redefinition */
#undef __EXTERN__
/* __RMP_PLATFORM_MP32P_MEMBER__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
