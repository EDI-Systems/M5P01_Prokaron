/******************************************************************************
Filename    : rmp_test_spce061a.h
Author      : pry 
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The testbench for SPCE061A.
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
/* How to read counter */
#define RMP_CNT_READ()      ((rmp_tim_t)((*P_TimerA_Data)<<1))
/* Are we testing the memory pool? */
#define TEST_MEM_POOL       1536
/* Are we doing minimal measurements? */
/* #define MINIMAL_SIZE */
/* The AVR timers we use is 16 bits, so */
typedef rmp_u16_t rmp_tim_t;
/* The pointer is also 16-bit, resort to 32-bit accumulators */
#define PTR_16_BIT
/* Sunplus special - whether to play music */
#define PLAY_MUSIC
/* End Define ****************************************************************/

/* Global ********************************************************************/
#ifndef MINIMAL_SIZE
void Int_Handler(void);
rmp_ptr_t Stack_1[80];
rmp_ptr_t Stack_2[80];

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

    /* TIMB clock = 1/2 CPU clock - Fosc/2 - 0.5 us per tick */
    *P_TimerB_Data=0xCFFFU;
    *P_TimerB_Ctrl=C_SourceC_Fosc2;
    *P_INT_Ctrl|=C_IRQ2_TMB;
}

/* The interrupt handler */
void TIMB_Handler(void)
{
    /* Clear TIMB flags */
    *P_INT_Clear=C_IRQ2_TMB;
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
 * probably never played music this way throughout its entire product lifetime. */
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
    
    /* Firing both DACs at 8kHz - not using ramping so there will be jerks */
    *P_DAC1=Value;
    *P_DAC2=Value;
    
    /* Clear watchdog */
    *P_Watchdog_Clear=C_WDTCLR;
    
    /* If we played towards the end, play again */
    Pos++;
    if(Pos==(sizeof(Sunplus_Univ_Disk_Music)*2U))
        Pos=0U;
}
#endif

void Int_Disable(void)
{
    *P_INT_Ctrl&=~C_IRQ2_TMB;
#ifdef PLAY_MUSIC
    /* Reinitialize TIMB here to prepare for music playback */
    Music=1;
    *P_TimerB_Data=0xF3FFU;
    *P_TimerB_Ctrl=C_SourceC_Fosc2;
    /* Music playback through TIMB handler, disabling all other interrupts */
    *P_INT_Ctrl=C_IRQ2_TMB;
#endif
}
#endif
/* End Function:Int_Disable **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
