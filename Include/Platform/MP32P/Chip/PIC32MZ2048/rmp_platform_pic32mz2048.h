/******************************************************************************
Filename   : rmp_platform_pic32mz2048.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for PIC32MZ2048EFM100.
******************************************************************************/

/* Define ********************************************************************/
/* The MPLAB libraries */
#include <xc.h>

/* Debugging */
#define RMP_ASSERT_ENABLE           (1U)
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 32 is usually sufficient */
#define RMP_PREEMPT_PRIO_NUM        (32)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (100000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX             (1000U)
/* The stack size of the init thread */
#define RMP_INIT_STACK_SIZE         (2048U)

/* What is the tick timer value? */
#define RMP_MP32P_CORETIM_VAL       (20000U)
/* What is the coprocessor type - set according to your bootcode */
#define RMP_MP32P_COP_DSPASE        (1U)
#define RMP_MP32P_COP_FR32          (0U)
#define RMP_MP32P_COP_FR64          (1U)

/* Other low-level initialization stuff - clock and serial. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_Start_Hook). */
RMP_EXTERN void _RMP_Set_Timer(rmp_ptr_t Tick);
#define RMP_MP32P_LOWLVL_INIT() \
do \
{ \
    /* set PBCLK2 to deliver 40MHz clock for PMP/I2C/UART/SPI */ \
    SYSKEY=0xAA996655UL; \
    SYSKEY=0x556699AAUL; \
    /* 200MHz/5=40MHz */ \
    PB2DIVbits.PBDIV=0x04U; \
    /* Timers use clock PBCLK3, set this to 40MHz */ \
    PB3DIVbits.PBDIV=0x04U; \
    /* Ports use PBCLK4 */ \
    PB4DIVbits.PBDIV=0x00U; \
    SYSKEY=0U; \
    /* Disable interrupts */ \
    __builtin_disable_interrupts(); \
    /* Enable multi-vector interrupts. */ \
    _CP0_BIS_CAUSE(0x00800000UL); \
    INTCONSET=_INTCON_MVEC_MASK; \
    /* We always use the timer 1 for interrupts, IPL=1, subpriority 3 */ \
    /* Core Timer Interrupt _CORE_TIMER_VECTOR 0 OFF000<17:1> IFS0<0> IEC0<0> IPC0<4:2> IPC0<1:0> */ \
    IFS0CLR=_IFS0_CTIF_MASK; \
    IPC0CLR=_IPC0_CTIP_MASK|_IPC0_CTIS_MASK; \
    IPC0SET=(1U<<_IPC0_CTIP_POSITION)|(0U<<_IPC0_CTIS_POSITION); \
    IEC0CLR=_IEC0_CTIE_MASK; \
    IEC0SET=(1U<<_IEC0_CTIE_POSITION); \
    _RMP_Set_Timer(RMP_MP32P_CORETIM_VAL>>1); \
    /* UART1 - RPD15 - 115200-8-N-1 */ \
    U1MODE=0U; \
    U1STAbits.UTXEN=1U; \
    U1MODEbits.BRGH=1U; \
    U1BRG=86U; \
    RPD15Rbits.RPD15R=1U; \
    U1MODESET=(1U<<15); \
} \
while(0)

#define RMP_MP32P_TIM_CLR()         (IFS0CLR=_IFS0_CTIF_MASK)

/* This is for debugging output */
#define RMP_MP32P_PUTCHAR(CHAR) \
do \
{ \
    while(U1STAbits.UTXBF!=0U); \
    U1TXREG=(CHAR); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
