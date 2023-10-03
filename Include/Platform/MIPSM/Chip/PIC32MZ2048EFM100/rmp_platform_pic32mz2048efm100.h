/******************************************************************************
Filename   : rmp_platform_pic32mz2048efm100.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for PIC32MZ2048EFM100.
******************************************************************************/

/* Defines *******************************************************************/
/* The MPLAB libraries */
#include <xc.h>

/* Debugging */
#define RMP_ASSERT_CORRECT          (0U)
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 32 is usually sufficient */
#define RMP_PREEMPT_PRIO_NUM        (32)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (100000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX             (1000U)
/* Are we using custom hooks? */
#define RMP_HOOK_EXTRA              (0U)
/* The stack size of the init thread */
#define RMP_INIT_STACK_SIZE         (2048U)
/* The mask/unmask interrupt operations */
#define RMP_INT_MASK()              RMP_Int_Disable()
#define RMP_INT_UNMASK()            RMP_Int_Enable()

/* What is the tick timer value? */
#define RMP_MIPSM_TICK_VAL          (20000U)
/* Do we initialize the rest of the registers when initializing the thread? */
#define RMP_MIPSM_INIT_EXTRA        (1U)

/* Other low-level initialization stuff - clock and serial. 
 * This is the default initialization sequence. If you wish to supply
 * your own, just redirect this macro to a custom function, or do your
 * initialization stuff in the initialization hook (RMP_Start_Hook). */
#define RMP_MIPSM_LOWLVL_INIT() \
do \
{ \
    /* set PBCLK2 to deliver 40MHz clock for PMP/I2C/UART/SPI */ \
	SYSKEY=0xAA996655UL; \
	SYSKEY=0x556699AAUL; \
	/* 200MHz/5=40MHz */ \
	PB2DIVbits.PBDIV=0b100; \
	/* Timers use clock PBCLK3, set this to 40MHz */ \
	PB3DIVbits.PBDIV=0b100; \
	/* Ports use PBCLK4 */ \
	PB4DIVbits.PBDIV=0b000; \
	SYSKEY=0; \
	/* Disable interrupts */ \
	__builtin_disable_interrupts(); \
	/* Enable multi-vector interrupts. */ \
	_CP0_BIS_CAUSE(0x00800000U); \
	INTCONSET=_INTCON_MVEC_MASK; \
    /* We always use the timer 1 for interrupts */ \
    /* Core Timer Interrupt _CORE_TIMER_VECTOR 0 OFF000<17:1> IFS0<0> IEC0<0> IPC0<4:2> IPC0<1:0> */ \
    /* Core Software Interrupt 0 _CORE_SOFTWARE_0_VECTOR 1 OFF001<17:1> IFS0<1> IEC0<1> IPC0<12:10> IPC0<9:8> */ \
    /* Clear the software interrupt flags */ \
	IFS0CLR=_IFS0_CTIF_MASK|_IFS0_CS0IF_MASK; \
	/* Set both interrupt priority - priority 1, subpriority 3, lowest allowed */ \
	IPC0CLR=_IPC0_CTIP_MASK|_IPC0_CTIS_MASK| \
            _IPC0_CS0IP_MASK|_IPC0_CS0IS_MASK; \
	IPC0SET=(1<<_IPC0_CTIP_POSITION)|(0<<_IPC0_CTIS_POSITION)| \
            (1<<_IPC0_CS0IP_POSITION)|(0<<_IPC0_CS0IS_POSITION); \
	IEC0CLR=_IEC0_CTIE_POSITION|_IEC0_CS0IE_MASK; \
	IEC0SET=(1<<_IEC0_CTIE_POSITION)|(1<<_IEC0_CS0IE_POSITION); \
    /* Initialize UART1 */ \
    U1MODE=0; \
    U1STAbits.UTXEN=1; \
    U1MODEbits.BRGH=1; \
    U1BRG=86; /* 115200 - Refer to datasheet for calculation */ \
    /* RPD15 used as TX port */ \
    RPD15Rbits.RPD15R=1; \
    U1MODESET=(1<<15); \
} \
while(0)

#define RMP_MIPSM_CLEAR_SOFT_FLAG()    do{IFS0CLR=_IFS0_CS0IF_MASK;}while(0)
#define RMP_MIPSM_CLEAR_TIMER_FLAG()   do{IFS0CLR=_IFS0_CTIF_MASK;}while(0)

/* This is for debugging output */
#define RMP_MIPSM_PUTCHAR(CHAR) \
do \
{ \
    while(U1STAbits.UTXBF!=0); \
    U1TXREG=(CHAR); \
} \
while(0)
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
