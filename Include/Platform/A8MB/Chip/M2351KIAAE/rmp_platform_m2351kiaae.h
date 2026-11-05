/******************************************************************************
Filename   : rmp_platform_m2351kiaae.h
Author     : pry
Date       : 03/11/2026
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for Nuvoton M2351KIAAE (NuMaker-PFM-M2351).
             This is an ARMv8-M BASELINE part (Cortex-M23), handled by the
             A8MB port: no BASEPRI, no CLZ, no FPU.
             The firmware library lives in M0A00_Library/M2351BSP.
******************************************************************************/

/* Define********************************************************************/
/* The firmware library (device header + the StdDriver headers) */
#include "NuMicro.h"
/* The Nuvoton device header aliases the GPIO port pointers to the bare names
 * PA/PB/PC..., and "PC" collides with the Program Counter member of
 * "struct RMP_A8MB_Stack". Only the Nuvoton gpio.c driver needs those short
 * names, and this port drives the pins through SYS->GPB_MFPH directly, so the
 * alias is dropped here. Use GPIOC/GPIOC_S if GPIO port C is ever needed. */
#undef PC

/* Debugging */
/* Assertion */
#define RMP_ASSERT_ENABLE           (1U)
/* Invalid parameter checking */
#define RMP_CHECK_ENABLE            (1U)
/* Debug logging */
#define RMP_DBGLOG_ENABLE           (1U)

/* System */
/* The maximum number of preemption priorities */
#define RMP_PREEMPT_PRIO_NUM        (32U)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (100000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX             (1000U)
/* The stack size of the init thread. The Nuvoton StdDriver reaches deep into
 * the clock tree from Timer_Init (TIMER_Open -> TIMER_GetModuleClock ->
 * CLK_GetPCLK0Freq -> SystemCoreClockUpdate -> CLK_GetPLLClockFreq), which
 * overruns a 256-byte stack and silently corrupts the kernel data that
 * follows it in .bss. */
#define RMP_INIT_STACK_SIZE         (1024U)

/* GUI */
#define RMP_GUI_ENABLE              (0U)
/* Anti-aliasing */
#define RMP_GUI_ANTIALIAS_ENABLE    (0U)
/* Widgets */
#define RMP_GUI_WIDGET_ENABLE       (0U)

/* What is the profile? 0 = ARMv8-M Baseline (Cortex-M23) */
/* What is the NVIC priority grouping? (only 2 priority bits are implemented) */
#define RMP_A8MB_NVIC_GROUPING       RMP_A8MB_NVIC_GROUPING_P2S6
/* What is the Systick value? CPU clock = 48MHz, 1ms tick */
#define RMP_A8MB_SYSTICK_VAL         (48000U)
/* What is the interrupt masking level? Unused on the Baseline profile */
#define RMP_A8MB_INT_MASK_LVL        (0xFFU)
/* What is the FPU type? None on the Cortex-M23 */

/* Other low-level initialization stuff - clock and serial
 * The M2351 tops out at 48MHz. The whole tree is derived from the 12MHz
 * crystal: HXT -> PLL(48MHz) -> HCLK, and UART0 also runs off the PLL so
 * that the baud rate is crystal-accurate. The PLL must be started from HXT;
 * starting it from the 12MHz HIRC hangs in CLK_EnablePLL on this board.
 * UART0 is the Nu-Link VCOM on the NuMaker-PFM-M2351 board (PB12=RX,
 * PB13=TX).
 * This is the default initialization sequence. If you wish to supply your
 * own, just redirect this macro to a custom function, or do your
 * initialization in the initialization hook (RMP_START_HOOK). */
#define RMP_A8MB_LOWLVL_INIT() \
do \
{ \
    /* Unlock the protected system registers */ \
    SYS_UnlockReg(); \
    \
    /* Everything is derived from the 12MHz crystal: HXT feeds the PLL, and \
     * the PLL output is the 48MHz HCLK (the part maximum). */ \
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk); \
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk); \
    CLK_EnablePLL(CLK_PLLCTL_PLLSRC_HXT, FREQ_48MHZ); \
    CLK_WaitClockReady(CLK_STATUS_PLLSTB_Msk); \
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_PLL, CLK_CLKDIV0_HCLK(1)); \
    SystemCoreClock=FREQ_48MHZ; \
    CyclesPerUs=FREQ_48MHZ/1000000UL; \
    \
    /* Enable UART0, clock it from the same PLL, then mux PB12/PB13 */ \
    CLK_EnableModuleClock(UART0_MODULE); \
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_PLL, CLK_CLKDIV0_UART0(1)); \
    SYS->GPB_MFPH=(SYS->GPB_MFPH&(~(UART0_RXD_PB12_Msk|UART0_TXD_PB13_Msk)))| \
                  (UART0_RXD_PB12|UART0_TXD_PB13); \
    SYS_ResetModule(UART0_RST); \
    \
    /* Lock the protected system registers again */ \
    SYS_LockReg(); \
    \
    /* UART initialization */ \
    UART_Open(UART0, 115200); \
    RMP_A8MB_PUTCHAR('\r'); \
    RMP_A8MB_PUTCHAR('\n'); \
    \
    /* Set the priority of timer, svc and faults to the lowest. The Baseline \
     * profile has no separate MemManage/BusFault/UsageFault exceptions (all \
     * faults escalate to HardFault) and the Nuvoton device header only \
     * enumerates the five system exceptions, so those are skipped. */ \
    NVIC_SetPriority(SVCall_IRQn, 3U); \
    NVIC_SetPriority(PendSV_IRQn, 3U); \
    NVIC_SetPriority(SysTick_IRQn, 3U); \
    SysTick_Config(RMP_A8MB_SYSTICK_VAL); \
} \
while(0)
/* This is for debugging output. The TX FIFO must have room before the next
 * byte is written, otherwise consecutive characters overwrite each other. */
#define RMP_A8MB_PUTCHAR(CHAR) \
do \
{ \
    while((UART0->FIFOSTS&UART_FIFOSTS_TXFULL_Msk)!=0U); \
    UART_WRITE(UART0, (uint8_t)(CHAR)); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
