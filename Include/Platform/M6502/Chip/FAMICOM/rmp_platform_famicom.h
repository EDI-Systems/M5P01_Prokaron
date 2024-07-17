/******************************************************************************
Filename   : rmp_platform_famicom.h
Author     : pry
Date       : 23/05/2024
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for FAMICOM.
******************************************************************************/

/* Define ********************************************************************/
/* The cc65 simple I/O library */
#include <conio.h>

/* Debugging */
/* Assertion */
#define RMP_ASSERT_ENABLE           (1U)
/* Use minimal logging or we have ROM overflow */
#define RMP_LOG(F,L,D,T)            RMP_DBG_I(L)
/* Invalid parameter checking */
#define RMP_CHECK_ENABLE            (1U)
/* Debug logging */
#define RMP_DBGLOG_ENABLE           (1U)

/* System */
/* The maximum number of preemption priorities */
#define RMP_PREEMPT_PRIO_NUM        (16U)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (10000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX             (1000U)
/* The stack size of the init thread */
#define RMP_INIT_STACK_SIZE         (256U)

/* GUI */
#define RMP_GUI_ENABLE              (0U)
/* Anti-aliasing */
#define RMP_GUI_ANTIALIAS_ENABLE    (0U)
/* Widgets */
#define RMP_GUI_WIDGET_ENABLE       (0U)

/* Zeropage size - must match what the compiler/linker script says */
#define RMP_M6502_ZP_SIZE           (0x001AU)

/* APU frame counter */
#define RMP_FAMICOM_APU_CTRL        (*((volatile rmp_u8_t*)(0x4015U)))
#define RMP_FAMICOM_APU_FC          (*((volatile rmp_u8_t*)(0x4017U)))
#define RMP_FAMICOM_APU_FC_4STEP    (0x00U)
#define RMP_FAMICOM_APU_FC_5STEP    (0x80U)
#define RMP_FAMICOM_APU_FC_INHIBIT  (0x40U)

/* Other low-level initialization stuff - clock and serial.
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_START_HOOK). */
#define RMP_M6502_LOWLVL_INIT() \
do \
{ \
    RMP_DBG_S("PPU initialized.\r\n"); \
    RMP_DBG_S("Booting kernel...\r\n"); \
    /* APU frame counter will generate 60Hz interrupts */ \
    RMP_FAMICOM_APU_FC=RMP_FAMICOM_APU_FC_4STEP; \
} \
while(0)

/* Read control register to clear IRQ status */
#define RMP_M6502_TIM_CLR() \
do \
{ \
    /* Use two ack methods to stay compatible with emulators */ \
    (void)RMP_FAMICOM_APU_FC; \
    RMP_FAMICOM_APU_FC=RMP_FAMICOM_APU_FC_INHIBIT; \
    RMP_FAMICOM_APU_FC=RMP_FAMICOM_APU_FC_4STEP; \
} \
while(0)

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
