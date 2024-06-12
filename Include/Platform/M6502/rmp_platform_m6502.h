/******************************************************************************
Filename    : rmp_platform_m6502.h
Author      : pry
Date        : 10/06/2024
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_m6502.c".
              This is a retro port paying tribute to the legendary MOS 6502
              processor, whose existence made personal computers affordable.
              The only supported console of this port will be the Nintendo
              Family Computer, a.k.a. Nintendo Entertainment System (NES/FC/
              FAMICOM), a equally legendary platform that made owning high-
              quality personal gaming consoles possible (contemporary consoles
              included Atari and SEGA). The RP2A03 that powered the console
              was an unlicensed clone of the MOS 6502, with the binary-coded-
              decimal mode removed to avoid MOS patent issues. Throughout the
              entire 80's and 90's, owning such a console was the dream of
              many children.
              Following the video game crash (a.k.a Atari Shock) of 1983,
              Nintendo attached much importance to the game quality, and each
              FAMICOM game release must be inspected then preapproved by 
              Nintendo. The cartridges were also exclusively supplied by
              Nintendo; later, several renowned studios are allowed to make
              their own cartridges or even expansion chips (the "mapper" name
              is more pronounced; the Nintendo officially called them Memory
              Management Controllers, a.k.a. MMCs).
              Released on 15 July 1983 with the "Mario Bros." (not the "Super
              Mario Bros." that released two years later, FAMICOM was regarded
              as the most iconic and influential console of all time. It was
              finally superseded by Super FAMICOM in 1990 which continued its
              success story. Both were discontinued by Nintendo on May 30, 2003.
              Due to the large number of NES fans, Nintendo replicated the
              console in 2016 as the "NES Classic Edition", which came bundled
              with some games. The replica is not powered by the original chips
              but by a modern emulator.
              According to WikiPedia, a total of 1386 official games (including
              all regional versions) summing up to less than 300MiB were released
              for FAMICOM, with the last one being the "Lion King" on 25 May 1995.
              FAMICOM-related development information can be acquired through
              www.nesdev.org, and many emulators such as iNES, MESEN, FCEUX and
              MAME exist. Even today, new games and applications are still being
              developed for the console.
              ------------------------------------------------------------------
              The existence of mappers was a signature aspect of FAMICOM. The
              FAMICOM consists of a CPU, a PPU, 2KiB PRG RAM and 2KiB CHR RAM,
              and could only address 64KiB of total memory. This was clearly
              insufficient for larger games. The built-in sound generators were
              capable of generating simple waveforms but could not play sampled
              datapoints, which is unsuitable for RPG or SLG games that aimed to
              provide an immersive experience. To these ends, the mappers were
              born. The mappers were circuits that (1) switch PRG/CHR ROM banks,
              (2) supply more working RAM, (3) allow to save games in battery-
              backed RAM, (4) provide editable CHR RAM, (5) enable more sprites,
              (6) expand palettes, (7) provide extra sound channels, (8) provide
              IRQ counters, or even (9) provide additional multipliers. They
              were soldered onto the game cartridge, and were considered a part
              of the game. Later games cartridges often included more resources
              than the base FAMICOM. This business model lowered the price of
              the base console which was helpful for product reception, and then
              charged a premium on the games that generated real revenue.
              The most famous/powerful mappers included:
              1. No mapper (iNES 000): Just the base FAMICOM.
              2. Nintendo MMC5 (iNES 005): The most powerful official mapper.
              3. Konami VRC7 （iNES 085): The best graphics, and great music.
              4. Sunsoft 5B (iNES 069): The best music.
              5. Namco(t) 163 (iNES 019): The most number of sound channels.
              ------------------------------------------------------------------
              As with many 6502-based systems, NES lacked a true multithreaded,
              preemptive operating system throughout its entire product lifecycle.
              Many cooperative systems exist with one of them advertising optional
              preemption support, however by looking at the code it is clear that
              preemption is not implemented in its NES port. As such, we feel
              obliged to provide a such a port, bringing true multitasking to the
              platform. This port would support other systems such as Atari, Acorn
              (which lives as ARM Ltd today) and Commodore with minimal porting.
              ------------------------------------------------------------------
              The 6502 presents multiple unique technical challenges.
              1. Return stack size issue: 6502 only had a 8-bit pointer, hence the
                 total stack size is 256B. This means that we won't be able to
                 allocate such a stack for each thread, and will have to spill it
                 to software stack before we can do anything. The call stack is an
                 EMPTY descending one.
              2. Zero page register issue: some zero-page registers are used by
                 compilers, and we'd have to save and restore that part as well.
              3. Independent parameter stack issue: the parameter is not on the
                 call stack because it is way too small. Instead, they are placed
                 onto an independent software-emulated stack. Luckily, this stack
                 is large enough for us, so we're using it to save everything. The
                 software stack is a FULL descending one.
              The difficulties (and full context switch overheads) essentially
              force us to write the most unusual switching code in RMP: the code
              (1) only does full pushes and pops when a context switch is actually
              required, and (2) copies the return stack (RSTK) to the parameter
              stack (PSTK).
              1. Push working registers to RSTK.
              2. Push zero page registers to PSTK.
              3. Execute interrupt handler with thread stacks.
                 Only when a context switch is required:
                 3.1 Save RSTK and RSP to PSTK.
                 3.2 Save PSP to RMP_SP_Cur.
                 3.3 Switch to kernel PSTK and rewind RSTK.
                 3.4 Call _RMP_Run_High to choose the correct thread.
                 3.5 Restore PSP from RMP_SP_Cur.
                 3.6 Restore RSTK and RSP from PSTK.
              4. Pop zero page registers from parameter stack.
              5. Pop working registers from return stack.
              The stack frame at last would look like:
                                         | RMP_SP_Cur = PSP
                                         v
              LO [        <------        RSP Y X A PF PC -RSTK- -ZP- -PSTK- ] HI
              ------------------------------------------------------------------
              The FAMICOM itself also poses challenges. It lacked a hardware
              cycle counter; the RP2A03 was intended to include a 24-bit timer,
              yet its development is unfinished and most circuitry connecting to
              it was cut (according to nesdev.org). Thus, a mapper containing a
              READABLE counter must be used to obtain performance numbers. The
              mapper chosen is Namco(t) 163 (iNES 019), the only mapper that
              featured a READABLE free-running counter which makes performance
              measurements possible. Many other mappers do feature IRQ timers,
              but they are write-only.
              The chip also comes with multi-channel sound support and extra
              CHR RAM, which makes it one of the most powerful mappers available.
              It was used in many famous games such as the "Sangokushi II: Hanou
              no Tairiku" mainly for its PRG banking and multi-channel sound
              capability.
              -----------------------------------------------------------------
              Please refrain from trying to use this port on games that require
              good user experience. The NES is simply too meager to run anything
              useful. The kernel itself easily takes up (almost) all the banks
              (24KiB=3x8KiB) with no hope of shrinking it further, forcing the
              programmer to bank code heavily with the last available bank. The
              mapper banking registers could count as segment registers, and the
              system should save and restore them in theory, however there are
              simply too many mappers to suooprt one by one. If you need some
              banking, you can enable extra hooks and save/restore them there.
              Also, because we use 16-bit pointers, all function pointer entries
              are supposed to be in the first (or last, depending on the mapper)
              64KiB, so that their bank number is always zero. Long calls and
              returns can make use of trampolines in a common bank.
              To run the system on real hardware, you need to:
              1. Have a real Namco 163 rewritable CHR/PRG catridge. SD card
                 cartridges are untested and not guaranteed to work.
              2. Have a catridge burner that could burn CHR/PRG catridges.
              3. Have a real FAMICOM/NES. Clones are not guaranteed to work.
              4. Have a monitor that accepts TV signal, or modify your FAMICOM
                 so that it outputs AV， RGB or HDMI signals.
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

/* M6502 specific macros *****************************************************/
/* Bits within the CP0 STATUS register */
#define RMP_M6502_PF_ID                 RMP_POW2(2U)    /* Enable interrupts */
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
    /* CPU registers */
    rmp_u8_t RSP;
    rmp_u8_t Y;
    rmp_u8_t X;
    rmp_u8_t A;
    rmp_u8_t PF;
    rmp_u8_t PCL;
    rmp_u8_t PCH;

    /* RSTK starts empty */

    /* Zeropage "register" variables */
    rmp_u8_t ZP[RMP_M6502_ZP_SIZE];

    /* PSTK starts empty */
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
EXTERN void _RMP_M6502_Yield(void);
__EXTERN__ void _RMP_Yield(void);


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
