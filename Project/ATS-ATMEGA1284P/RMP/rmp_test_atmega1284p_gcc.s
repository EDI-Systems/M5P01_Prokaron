/******************************************************************************
Filename    : rmp_test_atmega1284p_gcc.s
Author      : pry
Date        : 10/04/2012
Description : The extra testing file for this chip.
******************************************************************************/
    
/* Header ********************************************************************/
    .text
    /* AVR-GCC does not honor regular .include directive */
    #include            "avr/io.h"
    #include            "Platform/AVR/rmp_platform_avr_gcc.inc"
/* End Header ****************************************************************/
    
/* Import ********************************************************************/
    .extern             TIM2_Handler
/* End Import ****************************************************************/

/* Export ********************************************************************/
    .global             TIMER0_COMPA_vect
    .global             TIMER2_COMPA_vect
/* End Export ****************************************************************/

/* Function:TIMER0_COMPA_vect *************************************************
Description : Timer 0 interrupt routine for tick timer. Prescaler 256.
Input       : None.
Output      : None.                                      
******************************************************************************/
TIMER0_COMPA_vect:
    RMP_AVR_INT_SAVE_MEGA_EIND
    LDI                 R30,lo8(_RMP_AVR_Tim_Handler)
    LDI                 R31,hi8(_RMP_AVR_Tim_Handler)
    ICALL
    RMP_AVR_INT_LOAD_MEGA_EIND
/* End Function:TIMER0_COMPA_vect ********************************************/

/* Function:TIMER2_COMPA_vect *************************************************
Description : Timer 2 interrupt routine for ISR latency testing. Prescaler 256.
Input       : None.
Output      : None.                                      
******************************************************************************/
TIMER2_COMPA_vect:
    RMP_AVR_INT_SAVE_MEGA_EIND
    LDI                 R30,lo8(TIM2_Handler)
    LDI                 R31,hi8(TIM2_Handler)
    ICALL
    RMP_AVR_INT_LOAD_MEGA_EIND
/* End Function:TIMER2_COMPA_vect ********************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
