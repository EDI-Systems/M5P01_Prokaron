/******************************************************************************
Filename   : rmp_platform_nes.h
Author     : pry
Date       : 23/05/2024
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for NES.
******************************************************************************/

/* Define ********************************************************************/
/* The NES simple I/O library */
#include <conio.h>

/* Debugging */
#define RMP_ASSERT_CORRECT          (0U)
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 16 is usually sufficient */
#define RMP_PREEMPT_PRIO_NUM        (16U)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (10000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX             (1000U)
/* Are we using custom hooks? */
#define RMP_HOOK_EXTRA              (0U)
/* The stack size of the init thread */
#define RMP_INIT_STACK_SIZE         (128U)
/* The mask/unmask interrupt operations */
#define RMP_INT_MASK()              RMP_Int_Disable()
#define RMP_INT_UNMASK()            RMP_Int_Enable()

/* APU frame counter */
#define RMP_NES_APU_CTRL            (*((volatile rmp_u8_t*)(0x4015U)))
#define RMP_NES_APU_FC              (*((volatile rmp_u8_t*)(0x4017U)))
#define RMP_NES_APU_FC_4STEP        (0x00U)
#define RMP_NES_APU_FC_5STEP        (0x80U)
#define RMP_NES_APU_FC_INHIBIT      (0x40U)

/* Namco 169 timer */
#define RMP_NES_NAMCO169_IRQL       (*((volatile rmp_u8_t*)(0x5000U)))
#define RMP_NES_NAMCO169_IRQH       (*((volatile rmp_u8_t*)(0x5800U)))
#define RMP_NES_NAMCO169_IRQH_EN    (0x80U)

/* Other low-level initialization stuff - clock and serial.
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_Start_Hook). */
#define RMP_M6502_LOWLVL_INIT() \
do \
{ \
    RMP_DBG_S("\r\nPPU initialized. Booting RMP...\r\n"); \
    /* Set up the APU frame counter to generate 60Hz timer interrupts */ \
    RMP_NES_APU_FC=RMP_NES_APU_FC_4STEP; \
    /* Initialize Namco 169 timer */ \
    RMP_NES_NAMCO169_IRQL=0x00U; \
    RMP_NES_NAMCO169_IRQH=0x00U|RMP_NES_NAMCO169_IRQH_EN; \
    while(1); \
} \
while(0)

/* Read control register to clear IRQ status */
#define RMP_M6502_TIM_CLR()         ((void)RMP_NES_APU_CTRL)

/* This is for debugging output */
#define RMP_M6502_PUTCHAR(CHAR) \
do \
{ \
    cputc(CHAR); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
