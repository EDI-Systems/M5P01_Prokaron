/********************************************************************
 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC(R) Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *******************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <xc.h>
#include <string.h>
#include <timer_1ms.h>

/* Compiler checks and configuration *******************************/
#ifndef TIMER_MAX_1MS_CLIENTS
    #define TIMER_MAX_1MS_CLIENTS 1
#endif

/* Definitions *****************************************************/
#define STOP_TIMER_IN_IDLE_MODE     0x2000
#define TIMER_SOURCE_INTERNAL       0x0000
#define TIMER_SOURCE_EXTERNAL       0x0002
#define TIMER_ON                    0x8000
#define GATED_TIME_DISABLED         0x0000
#define TIMER_16BIT_MODE            0x0000

#define TIMER_PRESCALER_1           0x0000
#define TIMER_PRESCALER_8           0x0010
#define TIMER_PRESCALER_64          0x0020
#define TIMER_PRESCALER_256         0x0030
#define TIMER_INTERRUPT_PRIORITY    0x0001
#define TIMER_INTERRUPT_PRIORITY_4  0x0004

/* Type Definitions ************************************************/
typedef struct
{

    TICK_HANDLER handle ;
    uint32_t rate ;
    uint32_t count ;
} TICK_REQUEST ;

/* Variables *******************************************************/
TICK_REQUEST requests[TIMER_MAX_1MS_CLIENTS] ;
/*********************************************************************
 * Function: void TIMER_CancelTick(TICK_HANDLER handle)
 *
 * Overview: Cancels a tick request.
 *
 * PreCondition: None
 *
 * Input:  handle - the function that was handling the tick request
 *
 * Output: None
 *
 ********************************************************************/
void TIMER_CancelTick ( TICK_HANDLER handle )
{
    uint8_t i ;

    for (i = 0 ; i < TIMER_MAX_1MS_CLIENTS ; i++)
    {
        if (requests[i].handle == handle)
        {
            requests[i].handle = NULL ;
        }
    }
}
/*********************************************************************
 * Function: bool TIMER_RequestTick(TICK_HANDLER handle, uint32_t rate)
 *
 * Overview: Requests to receive a periodic event.
 *
 * PreCondition: None
 *
 * Input:  handle - the function that will be called when the time event occurs
 *         rate - the number of ticks per event.
 *
 * Output: bool - true if successful, false if unsuccessful
 *
 ********************************************************************/
bool TIMER_RequestTick ( TICK_HANDLER handle , uint32_t rate )
{
    uint8_t i ;

    for (i = 0 ; i < TIMER_MAX_1MS_CLIENTS ; i++)
    {
        if (requests[i].handle == NULL)
        {
            requests[i].handle = handle ;
            requests[i].rate = rate ;
            requests[i].count = 0 ;

            return true ;
        }
    }

    return false ;
}
/*********************************************************************
 * Function: bool TIMER_SetConfiguration(TIMER_CONFIGURATIONS configuration)
 *
 * Overview: Initializes the timer.
 *
 * PreCondition: None
 *
 * Input:  None
 *
 * Output: bool - true if successful, false if unsuccessful
 *
 ********************************************************************/
bool TIMER_SetConfiguration ( TIMER_CONFIGURATIONS configuration )
{
    switch (configuration)
    {
        case TIMER_CONFIGURATION_1MS:
            memset ( requests , 0 , sizeof (requests ) ) ;

            IPC2bits.T3IP = TIMER_INTERRUPT_PRIORITY ;
            IFS0bits.T3IF = 0 ;

            TMR3 = 0 ;

            PR3 = 2000 ;
            T3CON = TIMER_ON |
                    STOP_TIMER_IN_IDLE_MODE |
                    TIMER_SOURCE_INTERNAL |
                    GATED_TIME_DISABLED |
                    TIMER_16BIT_MODE |
                    TIMER_PRESCALER_8 ;

            IEC0bits.T3IE = 1 ;

            return true ;

        case TIMER_CONFIGURATION_RTCC:
            memset ( requests , 0 , sizeof (requests ) ) ;

            IPC0bits.T1IP = TIMER_INTERRUPT_PRIORITY_4 ;
            IFS0bits.T1IF = 0 ;

            TMR1 = 0 ;

            PR1 = 0x8000 ;
            T1CON = TIMER_ON |
                    TIMER_SOURCE_EXTERNAL |
                    GATED_TIME_DISABLED |
                    TIMER_16BIT_MODE |
                    TIMER_PRESCALER_1 ;

            IEC0bits.T1IE = 1 ;

            return true ;


        case TIMER_CONFIGURATION_OFF:
            IEC0bits.T3IE = 0 ;
            return true ;
    }

    return false ;
}
/****************************************************************************
  Function:
    void __attribute__((__interrupt__, auto_psv)) _T3Interrupt(void)

  Description:
    Timer ISR, used to update application state. If no transfers are pending
    new input request is scheduled.
  Precondition:
    None

  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
 ***************************************************************************/
void __attribute__ ( ( __interrupt__ , auto_psv ) ) _T3Interrupt ( void )
{
    uint8_t i ;

    for (i = 0 ; i < TIMER_MAX_1MS_CLIENTS ; i++)
    {
        if (requests[i].handle != NULL)
        {
            requests[i].count++ ;

            if (requests[i].count == requests[i].rate)
            {
                requests[i].handle ( ) ;
                requests[i].count = 0 ;
            }
        }
    }

    IFS0bits.T3IF = 0 ;
}
