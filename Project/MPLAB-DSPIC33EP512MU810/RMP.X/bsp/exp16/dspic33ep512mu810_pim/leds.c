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

#include <xc.h>
#include <leds.h>
#include <stdbool.h>

#define LED_D3_LAT      LATAbits.LATA0
#define LED_D4_LAT      LATAbits.LATA1
#define LED_D5_LAT      LATAbits.LATA2
#define LED_D6_LAT      LATAbits.LATA3
#define LED_D7_LAT      LATAbits.LATA4
#define LED_D8_LAT      LATAbits.LATA5
#define LED_D9_LAT      LATAbits.LATA6
#define LED_D10_LAT     LATAbits.LATA7      //Overlaps with S5

#define LED_D3_TRIS     TRISAbits.TRISA0
#define LED_D4_TRIS     TRISAbits.TRISA1
#define LED_D5_TRIS     TRISAbits.TRISA2
#define LED_D6_TRIS     TRISAbits.TRISA3
#define LED_D7_TRIS     TRISAbits.TRISA4
#define LED_D8_TRIS     TRISAbits.TRISA5
#define LED_D9_TRIS     TRISAbits.TRISA6
#define LED_D10_TRIS    TRISAbits.TRISA7    //Overlaps with S5

#define LED_ON  1
#define LED_OFF 0

#define INPUT  1
#define OUTPUT 0
/*********************************************************************
 * Function: void LED_On(LED led);
 *
 * Overview: Turns requested LED on
 *
 * PreCondition: LED configured via LED_Configure()
 *
 * Input: LED led - enumeration of the LEDs available in this
 *        demo.  They should be meaningful names and not the names of
 *        the LEDs on the silkscreen on the board (as the demo code may
 *        be ported to other boards).
 *         i.e. - LED_On(LED_CONNECTION_DETECTED);
 *
 * Output: none
 *
 ********************************************************************/
void LED_On ( LED led )
{
    switch (led)
    {
        case LED_D3:
            LED_D3_LAT = LED_ON ;
            break ;

        case LED_D4:
            LED_D4_LAT = LED_ON ;
            break ;

        case LED_D5:
            LED_D5_LAT = LED_ON ;
            break ;

        case LED_D6:
            LED_D6_LAT = LED_ON ;
            break ;

        case LED_D7:
            LED_D7_LAT = LED_ON ;
            break ;

        case LED_D8:
            LED_D8_LAT = LED_ON ;
            break ;

        case LED_D9:
            LED_D9_LAT = LED_ON ;
            break ;

        case LED_D10:
            LED_D10_LAT = LED_ON ;
            break ;

        case LED_NONE:
            break ;
    }
}
/*********************************************************************
 * Function: void LED_Off(LED led);
 *
 * Overview: Turns requested LED off
 *
 * PreCondition: LED configured via LEDConfigure()
 *
 * Input: LED led - enumeration of the LEDs available in this
 *        demo.  They should be meaningful names and not the names of
 *        the LEDs on the silkscreen on the board (as the demo code may
 *        be ported to other boards).
 *         i.e. - LED_Off(LED_CONNECTION_DETECTED);
 *
 * Output: none
 *
 ********************************************************************/
void LED_Off ( LED led )
{
    switch (led)
    {
        case LED_D3:
            LED_D3_LAT = LED_OFF ;
            break ;

        case LED_D4:
            LED_D4_LAT = LED_OFF ;
            break ;

        case LED_D5:
            LED_D5_LAT = LED_OFF ;
            break ;

        case LED_D6:
            LED_D6_LAT = LED_OFF ;
            break ;

        case LED_D7:
            LED_D7_LAT = LED_OFF ;
            break ;

        case LED_D8:
            LED_D8_LAT = LED_OFF ;
            break ;

        case LED_D9:
            LED_D9_LAT = LED_OFF ;
            break ;

        case LED_D10:
            LED_D10_LAT = LED_OFF ;
            break ;

        case LED_NONE:
            break ;
    }
}
/*********************************************************************
 * Function: void LED_Toggle(LED led);
 *
 * Overview: Toggles the state of the requested LED
 *
 * PreCondition: LED configured via LEDConfigure()
 *
 * Input: LED led - enumeration of the LEDs available in this
 *        demo.  They should be meaningful names and not the names of
 *        the LEDs on the silkscreen on the board (as the demo code may
 *        be ported to other boards).
 *         i.e. - LED_Toggle(LED_CONNECTION_DETECTED);
 *
 * Output: none
 *
 ********************************************************************/
void LED_Toggle ( LED led )
{
    switch (led)
    {
        case LED_D3:
            LED_D3_LAT ^= 1 ;
            break ;

        case LED_D4:
            LED_D4_LAT ^= 1 ;
            break ;

        case LED_D5:
            LED_D5_LAT ^= 1 ;
            break ;

        case LED_D6:
            LED_D6_LAT ^= 1 ;
            break ;

        case LED_D7:
            LED_D7_LAT ^= 1 ;
            break ;

        case LED_D8:
            LED_D8_LAT ^= 1 ;
            break ;

        case LED_D9:
            LED_D9_LAT ^= 1 ;
            break ;

        case LED_D10:
            LED_D10_LAT ^= 1 ;
            break ;

        case LED_NONE:
            break ;
    }
}
/*********************************************************************
 * Function: bool LED_Get(LED led);
 *
 * Overview: Returns the current state of the requested LED
 *
 * PreCondition: LED configured via LEDConfigure()
 *
 * Input: LED led - enumeration of the LEDs available in this
 *        demo.  They should be meaningful names and not the names of
 *        the LEDs on the silkscreen on the board (as the demo code may
 *        be ported to other boards).
 *         i.e. - LED_Get(LED_CONNECTION_DETECTED);
 *
 * Output: true if on, false if off
 *
 ********************************************************************/
bool LED_Get ( LED led )
{
    switch (led)
    {
        case LED_D3:
            return ( ( LED_D3_LAT == LED_ON ) ? true : false ) ;

        case LED_D4:
            return ( ( LED_D4_LAT == LED_ON ) ? true : false ) ;

        case LED_D5:
            return ( ( LED_D5_LAT == LED_ON ) ? true : false ) ;

        case LED_D6:
            return ( ( LED_D6_LAT == LED_ON ) ? true : false ) ;

        case LED_D7:
            return ( ( LED_D7_LAT == LED_ON ) ? true : false ) ;

        case LED_D8:
            return ( ( LED_D8_LAT == LED_ON ) ? true : false ) ;

        case LED_D9:
            return ( ( LED_D9_LAT == LED_ON ) ? true : false ) ;

        case LED_D10:
            return ( ( LED_D10_LAT == LED_ON ) ? true : false ) ;

        case LED_NONE:
            return false ;

    }
    return false ;
}
/*********************************************************************
 * Function: void LED_Enable(LED led);
 *
 * Overview: Configures the LED for use by the other LED API
 *
 * PreCondition: none
 *
 * Input: LED led - enumeration of the LEDs available in this
 *        demo.  They should be meaningful names and not the names of
 *        the LEDs on the silkscreen on the board (as the demo code may
 *        be ported to other boards).
 *
 * Output: none
 *
 ********************************************************************/
void LED_Enable ( LED led )
{
    switch (led)
    {
        case LED_D3:
            LED_D3_TRIS = OUTPUT ;
            break ;

        case LED_D4:
            LED_D4_TRIS = OUTPUT ;
            break ;

        case LED_D5:
            LED_D5_TRIS = OUTPUT ;
            break ;

        case LED_D6:
            LED_D6_TRIS = OUTPUT ;
            break ;

        case LED_D7:
            LED_D7_TRIS = OUTPUT ;
            break ;

        case LED_D8:
            LED_D8_TRIS = OUTPUT ;
            break ;

        case LED_D9:
            LED_D9_TRIS = OUTPUT ;
            break ;

        case LED_D10:
            LED_D10_TRIS = OUTPUT ;
            break ;

        case LED_NONE:
            break ;
    }
}

