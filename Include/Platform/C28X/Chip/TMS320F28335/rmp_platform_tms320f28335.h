/******************************************************************************
Filename   : rmp_platform_tms320f28335.h
Author     : zr
Date       : 27/04/2024
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for TMS320F28335.
******************************************************************************/

/* Define ********************************************************************/
/* The TI-supplied headers */
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

/* Debugging */
#define RMP_ASSERT_ENABLE           (1U)
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 32 is usually sufficient */
#define RMP_PREEMPT_PRIO_NUM        (32U)
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

/* The tick timer value */
#define RMP_C28X_TICK_VAL           (15000U)
/* FPU type */
#define RMP_C28X_COP_FPU32          (1U)
#define RMP_C28X_COP_FPU64          (0U)

/* Serial baud rate */
#define RMP_C28X_BAUD_RATE          (115200U)
#define RMP_C28X_SCIA_BAUD          (37500000U/(8U*RMP_C28X_BAUD_RATE)-1U)

/* Low-level initialization stuff - PIE table, clock and serial.
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_START_HOOK). */
#define RMP_C28X_LOWLVL_INIT() \
do \
{ \
    /* Disable interrupts and PIE */ \
    EDIS; \
    PieCtrlRegs.PIECTRL.bit.ENPIE=0U; \
    /* Configure system clock to 150MHz */ \
    InitSysCtrl(); \
    /* Initialize vector table and sources but not global interrupt yet */ \
    InitPieVectTable(); \
    IFR=0x0000U; \
    IER=0xFFFFU; \
    /* Initialize RAMFUNC */ \
    MemCopy(&RamfuncsLoadStart,&RamfuncsLoadEnd,&RamfuncsRunStart); \
    \
    /* Initialize flash - need to run in RAM if you boot from Flash! */ \
    FlashRegs.FOPT.bit.ENPIPE=1U; \
    FlashRegs.FBANKWAIT.bit.PAGEWAIT=5U; \
    FlashRegs.FBANKWAIT.bit.RANDWAIT=5U; \
    FlashRegs.FOTPWAIT.bit.OTPWAIT=8U; \
    FlashRegs.FSTDBYWAIT.bit.STDBYWAIT=0x01FFU; \
    FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT=0x01FFU; \
    \
    /* Initialize timer 2 for RTOS use */ \
    InitCpuTimers(); \
    ConfigCpuTimer(&CpuTimer2,150U,1000U); \
    CpuTimer2Regs.TCR.bit.TSS=0U; \
    \
    /* SCIA GPIO & FIFO - 115200-8-N-1 */ \
    SysCtrlRegs.PCLKCR0.bit.SCIAENCLK=1U; \
    InitSciaGpio(); \
    SciaRegs.SCIFFTX.all=0xE040U; \
    SciaRegs.SCIFFRX.all=0x204FU; \
    SciaRegs.SCIFFCT.all=0U; \
    SciaRegs.SCICCR.all=0x0007U; \
    SciaRegs.SCICTL1.all=0x0003U; \
    SciaRegs.SCICTL2.all=0x0003U; \
    SciaRegs.SCICTL2.bit.TXINTENA=0U; \
    SciaRegs.SCICTL2.bit.RXBKINTENA=0U; \
    SciaRegs.SCIHBAUD=RMP_C28X_SCIA_BAUD>>8; \
    SciaRegs.SCILBAUD=RMP_C28X_SCIA_BAUD&0xFFU; \
    SciaRegs.SCICTL1.all=0x0023U; \
} \
while(0)

#define RMP_C28X_TIM_CLR()          (CpuTimer2Regs.TCR.bit.TIF=1U)

/* This is for debugging output via SCI-A */
#define RMP_C28X_PUTCHAR(CHAR) \
do \
{ \
    while(SciaRegs.SCIFFTX.bit.TXFFST!=0U);  \
    SciaRegs.SCITXBUF=(rmp_u8_t)(CHAR);  \
} \
while (0)

/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
