/******************************************************************************
Filename    : rmp_test_famicom.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for Nintendo Family Computer (FAMICOM), also known
              as the Nintendo Entertainment System (NES).
              This test is, expectedly, a crawling turtle. We're only running
              200 rounds for this reason, and the FAMICOM is quite predictable
              so more rounds are unnecessary. Never run above 1000 rounds or
              results take forever to appear.
              FAMICOM is clearly not suitable for a multi-tasking preemptive
              operating system: simple state machines fits its caliber much 
              better. Dynamic memory allocations are also very slow, and it's
              best to go without them.
              This test also demonstrates how capable the game developers were
              back in the 80's. Different from modern microcontrollers such as
              DSPIC and AVR, the FAMICOM's weakness is in its CPU frequency 
              rather than its memory capacity. The CPU runs at 1.8MHz and is
              laughably slow when compared with AVR who is some 10x faster.
              Such underclocking are due to the manufacturing process, and
              the CPU could be overclocked to some 3 MHz, still far below what
              can be achieved by modern 8-bitters. This causes slowdowns in
              certain games such as the Contra Force when multiple sprites 
              appear and overload the background computation.

cc65 V2.19 (Git 2c4d4d3) -Oirs (MESEN simulator 2.0.0)
=RMP= RT kernel
 Std benchmark
================
CY : A / T / B
Y  : 4060 / 5481 / 2334
M  : 5439 / 6275 / 5292
S  : 5424 / 7190 / 3185
F  : 2040 / 3752 / 1881
Q  : 7728 / 8266 / 6196
B  : 10443 / 11239 / 9666
MM : 7446 / 7711 / 7216
MI : 4836 / 4914 / 4822
SI : 5185 / 5262 / 5170
QI : 7227 / 7297 / 7205
BI : 8355 / 8424 / 8332

cc65 V2.19 (Git 2c4d4d3) -Oirs (FCEUX simulator 2.6.6)
=RMP= RT kernel
 Std benchmark
================
CY : A / T / B
Y  : 4060 / 5481 / 2334
M  : 5439 / 6277 / 5292
S  : 5424 / 7190 / 3185
F  : 2017 / 3752 / 1881
Q  : 7726 / 8266 / 6196
B  : 10443 / 11239 / 9666
MM : 7450 / 7711 / 7218
MI : 4835 / 4914 / 4822
SI : 5184 / 5262 / 5170
QI : 7228 / 7297 / 7205
BI : 8358 / 8424 / 8332
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Test only 200 rounds if desired - CPU too slow */
#define ROUND_NUM                   200
/* Overflow number set to 1000 times - CPU too slow */
#define OVERFLOW_NUM                1000U
/* Use small terminal for this test to print to FAMICOM screen */
#define SMALL_TERMINAL
/* Counter read wrapper */
#define RMP_CNT_READ()              Timer_Read()
/* Memory pool test switch, test without initialization */
#define TEST_MEM_POOL               (2048U)
#define TEST_MEM_POOL_NOINIT
/* Minimal build switch */
/* #define MINIMAL_SIZE */
/* Pointer size switch */
#define PTR_16_BIT
/* Timestamp data type */
typedef rmp_u16_t rmp_tim_t;

/* Namco 169 timer used both as a timestamp counter and to measure latency */
#define RMP_FAMICOM_NAMCO_IRQL      (*((volatile rmp_u8_t*)(0x5000U)))
#define RMP_FAMICOM_NAMCO_IRQH      (*((volatile rmp_u8_t*)(0x5800U)))
#define RMP_FAMICOM_NAMCO_IRQH_EN   (0x80U)
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
rmp_ptr_t Stack_1[512];
rmp_ptr_t Stack_2[512];
rmp_u8_t Namco_MSB;
rmp_u8_t Namco_IRQ_Test;

void Timer_Init(void);
void Int_Init(void);
void Int_Disable(void);
/* End Global ****************************************************************/

/* Function:Timer_Init ********************************************************
Description : Initialize the timer for timing measurements. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Timer_Init(void)
{
    /* NAMCO clock = CPU clock */
    Namco_MSB=0U;
    Namco_IRQ_Test=0U;
    RMP_FAMICOM_NAMCO_IRQL=0x00U;
    RMP_FAMICOM_NAMCO_IRQH=0x00U|RMP_FAMICOM_NAMCO_IRQH_EN;
}

/* Timer read helper */
rmp_tim_t Timer_Read(void)
{
    rmp_u8_t Most;
    rmp_u8_t High;
    rmp_u8_t Low;

    /* We can't be sure that when we read the lower bits the higher bits will
     * stay still. Thus, we keep reading until the higher bits are stable.
     * This will take some toll on measurements, of course. */
     do
     {
        Most=Namco_MSB;
        High=RMP_FAMICOM_NAMCO_IRQH;
        Low=RMP_FAMICOM_NAMCO_IRQL;
     }
     while((High!=RMP_FAMICOM_NAMCO_IRQH)||(Most!=Namco_MSB));

     /* Assemble the final return value */
     High=(High&0x7FU)|Most;
     return (((rmp_tim_t)High)<<8)|Low;
}
/* End Function:Timer_Init ***************************************************/

/* Function:Int_Init **********************************************************
Description : Initialize an periodic interrupt source. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Init(void)
{
    /* NAMCO clock = CPU clock */
    Namco_IRQ_Test=1U;
}

/* The generic interrupt handler */
void NAMCO_IRQ_Handler(void)
{
    /* Reprogram the NAMCO counter immediately to keep track of time */
    RMP_FAMICOM_NAMCO_IRQH=0x00U|RMP_FAMICOM_NAMCO_IRQH_EN;
    RMP_FAMICOM_NAMCO_IRQL=0x00U;

    /* Increase the MSB counter, a new round comes */
    if(Namco_MSB!=0U)
        Namco_MSB=0U;
    else
        Namco_MSB=0x80U;
    
    /* Only call this when we're testing interrupts */
    if(Namco_IRQ_Test!=0U)
        Int_Handler();
}
/* End Function:Int_Init *****************************************************/

/* Function:Int_Disable *******************************************************
Description : Disable the periodic interrupt source. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Disable(void)
{
    Namco_IRQ_Test=0U;
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
