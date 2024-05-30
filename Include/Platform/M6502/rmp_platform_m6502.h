/******************************************************************************
Filename    : rmp_platform_m6502.h
Author      : pry
Date        : 01/04/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_m6502.c".
              This is a retro port paying tribute to the legendary MOS 6502
              processor, whose existence made personal computers affordable.
              The only supported console of this port will be the Nintendo
              Entertainment System (NES/Famicom), a equally legendary platform
              that made owning high-quality personal gaming consoles possible
              (contemporary consoles included Atari and SEGA). Throughout the
              entire 80's and 90's, owning such a console was the dream of many
              children.
              ------------------------------------------------------------------
              As with many 6502-based systems, NES lacked a true multithreaded,
              preemptive operating system throughout its entire product lifecycle.
              Many cooperative systems exist with one of them advertising optional
              preemption support, however by looking at the code it is clear that
              preemption is not implemented in its NES port. As such, we feel
              obliged to provide a such a port, bringing true multitasking to the
              platform.
              ------------------------------------------------------------------
              The 6502 presents multiple unique technical challenges.
              1. Call stack size issue: 6502 only had a 8-bit pointer, hence the
                 total stack size is 256B. This means that we won't be able to
                 allocate such a stack for each thread, and will have to spill it
                 to software stack before we can do anything. The call stack is an
                 empty descending one.
              2. Zero page register issue: some zero-page registers are used by
                 compilers, and we'd have to save and restore that part as well.
              3. Independent software stack issue: the parameter is not on the
                 call stack because it is way too small. Instead, they are placed
                 onto an independent software-emulated stack. Luckily, this stack
                 is large enough for us, so we're using it to save everything. The
                 software stack is a full descending one.
              The difficulties essentially forces us to write the most unusual
              switching code so far in RMP:
              1. Push working registers to call stack.
              2. Push zero page registers to parameter stack.
              3. Execute interrupt handler. If a context switch is required:
                 3.1 Save call stack to parameter stack.
                 3.2 Save software stack pointer to RMP_SP_Cur.
                 3.3 Call _RMP_Run_High to choose the correct thread.
                 3.4 Restore software stack pointer from RMP_SP_Cur.
                 3.5 Restore call stack from parameter stack.
                     Note that the SP register is also saved onto the stack. This
                     helps us to know how much call stack is used, so we can move
                     exactly that amount without messing up the data boundaries.
                     
                     rmp_u8_t SP
                     rmp_u8_t Y
                     rmp_u8_t X
                     rmp_u8_t A
                     rmp_u8_t PF
                     rmp_ptr_t PC
                     rmp_ptr_t ZP[ZP_SIZE];
              4. Pop zero page registers from parameter stack.
              5. Pop working registers from call stack.
                                     | RMP_SP_Cur              
                                     v
              LO [                    SP Y X A PF PC CALL_STACK ZEROPAGE PARAM_STACK ] HI
              ------------------------------------------------------------------
              Please refrain from trying to use this port on games that require
              good user experience. The NES is simply too meager to run anything
              useful. The kernel itself easily takes up (almost) all the banks
              (24KiB=3x8KiB) with no hope of shrinking it further, forcing the
              programmer to bank code heavily with the last available bank. The
              mapper chosen is Namco (Namcot) 163 (iNES 019), the only one that
              featured a readable free-running counter which makes performance
              measurements possible. It also comes with sound support and CHRRAM,
              which makes it one of the most powerful MMCs available.
              The NES processor was intended to include a 24-bit timer, yet the
              development is unfinished and most circuitry connecting to it was
              cut (according to nesdev.org). Luckily, the mapper has 8KiB of RAM
              plus 512KiB of ROM, giving determined programmers a chance to port
              the system.
******************************************************************************/

/* Define ********************************************************************/
#ifdef __HDR_DEF__
#ifndef __RMP_PLATFORM_M6502_DEF__
#define __RMP_PLATFORM_M6502_DEF__
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
/* Full descending stack of rmp_u8_t, no alignment */
#define RMP_STACK_TYPE                  RMP_STACK_FULL_DESCEND
#define RMP_STACK_ALIGN                 (0U)
#define RMP_STACK_ELEM                  rmp_u8_t
#define RMP_STACK_STRUCT                struct RMP_M6502_Stack
/* MSB/LSB extraction */
#define RMP_MSB_GET(VAL)                RMP_MSB_Generic(VAL)
#define RMP_LSB_GET(VAL)                RMP_LSB_Generic(VAL)

/* The CPU and application specific macros are here */
#include "rmp_platform_M6502_conf.h"
/* End System macros *********************************************************/
/*****************************************************************************/
/* __RMP_PLATFORM_M6502_DEF__ */
#endif
/* __HDR_DEF__ */
#endif
/* End Define ****************************************************************/

/* Struct ********************************************************************/
#ifdef __HDR_STRUCT__
#ifndef __RMP_PLATFORM_M6502_STRUCT__
#define __RMP_PLATFORM_M6502_STRUCT__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEF__
#undef __HDR_DEF__
/*****************************************************************************/
struct RMP_M6502_Stack
{
    rmp_ptr_t Placeholder;
};
/*****************************************************************************/
/* __RMP_PLATFORM_M6502_STRUCT__ */
#endif
/* __HDR_STRUCT__ */
#endif
/* End Struct ****************************************************************/

/* Private Variable **********************************************************/
#if(!(defined __HDR_DEF__||defined __HDR_STRUCT__))
#ifndef __RMP_PLATFORM_M6502_MEMBER__
#define __RMP_PLATFORM_M6502_MEMBER__

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
__EXTERN__ rmp_ptr_t _RMP_M6502_SP_Kern;

__EXTERN__ volatile rmp_u8_t RMP_M6502_Int_Act;
__EXTERN__ volatile rmp_u8_t _RMP_M6502_Yield_Pend;
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

void _RMP_M6502_Yield(void);

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
/* __RMP_PLATFORM_M6502_MEMBER__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
