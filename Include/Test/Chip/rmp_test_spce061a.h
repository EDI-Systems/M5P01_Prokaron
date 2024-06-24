/******************************************************************************
Filename    : rmp_test_spce061a.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for SPCE061A.
              The test presents horrible numbers due to the multi-cycle design
              of the unSP architecture. Most instructions are multiple machine
              cycles, which kind of defeats the 49MHz performance argument.
              For example, a single data transfer can take 9 cycles. This is
              not very much better than the conventional 8051's 12 cycles.
              The test can take up to two minutes to finish. If this is deemed
              too long, reduce the rounds to 1000.
              Don't push the timers over 49MHz or you will read garbage!

unSP GCC 1.0.10 (based on vanilla GCC 2.95.2) -O3
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 694 / 1002 / 684
Mailbox                            : 1732 / 2026 / 1706
Semaphore                          : 1548 / 1844 / 1526
FIFO                               : 927 / 1230 / 910
Message queue                      : 2671 / 2952 / 2634
Blocking message queue             : 3709 / 3978 / 3658
Memory allocation/free pair        : 3518 / 4171 / 2557
ISR Mailbox                        : 1619 / 1912 / 1594
ISR Semaphore                      : 1475 / 1770 / 1452
ISR Message queue                  : 2242 / 2528 / 2210
ISR Blocking message queue         : 2889 / 3166 / 2848

unSP GCC 1.0.10 (based on vanilla GCC 2.95.2) -Os
    ___   __  ___ ___
   / _ \ /  |/  // _ \       Simple real-time kernel
  / , _// /|_/ // ___/       Standard benchmark test
 /_/|_|/_/  /_//_/
====================================================
Test (number in CPU cycles)        : AVG / MAX / MIN
Yield                              : 893 / 1198 / 878
Mailbox                            : 1922 / 2214 / 1894
Semaphore                          : 1852 / 2144 / 1826
FIFO                               : 1135 / 1436 / 1118
Message queue                      : 3329 / 3602 / 3282
Blocking message queue             : 4531 / 4788 / 4470
Memory allocation/free pair        : 4049 / 4815 / 2887
ISR Mailbox                        : 1784 / 2076 / 1758
ISR Semaphore                      : 1748 / 2040 / 1720
ISR Message queue                  : 2806 / 3084 / 2766
ISR Blocking message queue         : 3504 / 3772 / 3456
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* Test only 1000 rounds if desired - CPU too slow */
/* #define ROUND_NUM           1000 */
/* How to read counter */
#define RMP_CNT_READ()      (((rmp_tim_t)(*P_TimerA_Data))<<1)
/* Are we testing the memory pool? */
#define TEST_MEM_POOL       (1024U)
#define TEST_MEM_POOL_NOINIT
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The SPCE061 timers we use is 16 bits, so */
typedef rmp_u16_t rmp_tim_t;
/* The pointer is also 16-bit, resort to 32-bit accumulators */
#define PTR_16_BIT
/* Sunplus special - whether to play music */
#define PLAY_MUSIC
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
rmp_ptr_t Stack_1[128];
rmp_ptr_t Stack_2[128];

#ifdef PLAY_MUSIC
rmp_ptr_t Music;
#include "spu_disk_music.h"
void Play_Music(void);
#endif

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
    /* TIMA clock = 1/2 CPU clock - Fosc/2, no gating */
    *P_TimerA_Data=0x0000U;
    *P_TimerA_Ctrl=C_SourceA_Fosc2|C_SourceB_1|C_DutyOff;
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
#ifdef PLAY_MUSIC
    /* Not playing music right now */
    Music=0U;
#endif

    /* TIMB clock = 1/2 CPU clock - Fosc/2 - 1ms per tick */
    *P_TimerB_Data=0x9FFFU;
    *P_TimerB_Ctrl=C_SourceC_Fosc2;
    *P_INT_Mask|=C_IRQ2_TMB;
}

/* The interrupt handler */
void TIMB_Handler(void)
{
    /* Clear TIMB flags & watchdog */
    *P_INT_Clear=C_IRQ2_TMB;
    *P_Watchdog_Clear=C_WDTCLR;
    
#ifdef PLAY_MUSIC
    if(Music==0U)
        Int_Handler();
    else
        Play_Music();
#else
    Int_Handler();
#endif
}
/* End Function:Int_Init *****************************************************/

/* Function:Int_Disable *******************************************************
Description : Disable the periodic interrupt source. This function needs
              to be adapted to your specific hardware.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
/* Play music - without proprietary Sunplus libraries to get the best possible
 * quality on this chip. Also, we're using Timer B instead of Timer A. The chip
 * probably never played music this way throughout its entire product lifetime;
 * all it had was (hugely) lossy compression algorithms that trade audio quality
 * for playtime, lest that the audio will exceed its meager flash capacity. */
#ifdef PLAY_MUSIC
void Play_Music(void)
{
    static rmp_ptr_t Pos=0U;
    rmp_ptr_t Value;
    
    /* Extract 8-bit samples, where the lower byte comes first in a word */
    Value=Sunplus_Univ_Disk_Music[Pos>>1];
    if((Pos&1U)==0U)
    {
        Value=(Value&0x00FFU)<<8;
    }
    else
        Value&=0xFF00U;
        
    /* Make sure not to exceed the original speaker's volume limit; keep DC=0x8000 */
    Value=(Value>>1)+0x4000U;

    /* Firing both DACs at 8kHz - not using ramping so there will be jerks */
    *P_DAC1=Value;
    *P_DAC2=Value;
    
    /* If we played towards the end, play again */
    Pos++;
    if(Pos==(sizeof(Sunplus_Univ_Disk_Music)*2U))
        Pos=0U;
}
#endif

void Int_Disable(void)
{
    /* Do note that INT_Ctrl reads give active flags instead of enabling status;
     * INT_Mask bits, when set, enables the interrupt. Very counterintuitive. */
    *P_INT_Mask&=~C_IRQ2_TMB;
    
#ifdef PLAY_MUSIC
    /* Reinitialize TIMB here to prepare for music playback */
    Music=1U;
    *P_TimerB_Data=0xF9FFU;
    *P_TimerB_Ctrl=C_SourceC_Fosc2;
    /* Music playback through TIMB handler, disabling tick timer */
    *P_INT_Mask=C_IRQ2_TMB;
#endif
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
