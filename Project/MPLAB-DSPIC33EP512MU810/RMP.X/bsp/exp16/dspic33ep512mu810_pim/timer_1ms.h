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

#ifndef TIMER_1MS
#define TIMER_1MS

#define TIMER_TICK_INTERVAL_MICRO_SECONDS 1000

/* Type Definitions ***********************************************/
typedef void (*TICK_HANDLER)(void);

typedef enum
{
    TIMER_CONFIGURATION_1MS,
    TIMER_CONFIGURATION_RTCC,
    TIMER_CONFIGURATION_OFF
} TIMER_CONFIGURATIONS;

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
void TIMER_CancelTick(TICK_HANDLER handle);

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
bool TIMER_RequestTick(TICK_HANDLER handle, uint32_t rate);

/*********************************************************************
* Function: bool TIMER_SetConfiguration(TIMER_CONFIGURATIONS configuration)
*
* Overview: Initializes the timer.
*
* PreCondition: TIMER_CONFIGURATIONS - configuration to run
*
* Input:  None
*
* Output: bool - true if successful, false if unsuccessful
*
********************************************************************/
bool TIMER_SetConfiguration(TIMER_CONFIGURATIONS configuration);

#endif //TIMER_1MS
