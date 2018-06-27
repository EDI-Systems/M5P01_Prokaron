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

#ifndef PRINT_LCD_H
#define PRINT_LCD_H

#include <stdbool.h>
#include <lcd.h>

/* Type Definitions *************************************************/
typedef enum
{
    PRINT_CONFIGURATION_LCD
} PRINT_CONFIGURATION;

/*********************************************************************
* Function: bool PRINT_SetConfiguration(PRINT_CONFIGURATION led);
*
* Overview: Configures the print configuration
*
* PreCondition: none
*
* Input: configuration - the print configuration to use.  Some boards
*         may have more than one print configuration enabled
*
* Output: TRUE if successful, FALSE if LED not configured or doesn?t 
*         exist for this board.
*
********************************************************************/
bool PRINT_SetConfiguration(PRINT_CONFIGURATION configuration);
#define PRINT_SetConfiguration(configuration) LCD_Initialize()

/*********************************************************************
* Function: void PRINT_String(char* string, uint16_t length)
*
* Overview: Prints a string until a null terminator is reached or the
*           specified string length is printed.
*
* PreCondition: none
*
* Input: char* string - the string to print.
*        uint16_t length - the length of the string.
*
* Output: None
*
********************************************************************/
void PRINT_String(char* string, uint16_t length);
#define PRINT_String(string, length) LCD_PutString(string, length)

/*********************************************************************
* Function: void PRINT_Char(char charToPrint)
*
* Overview: Prints a character
*
* PreCondition: none
*
* Input: char charToPrint - the character to print
*
* Output: None
*
********************************************************************/
void PRINT_Char(char charToPrint);
#define PRINT_Char(charToPrint) LCD_PutChar(charToPrint)

/*********************************************************************
* Function: void PRINT_ClearStreen()
*
* Overview: Clears the screen, if possible
*
* PreCondition: none
*
* Input: None
*
* Output: None
*
********************************************************************/
void PRINT_ClearScreen(void);
#define PRINT_ClearScreen() LCD_ClearScreen()

/*********************************************************************
* Function: void PRINT_CursorEnable(bool enable)
*
* Overview: Enables/disables the cursor
*
* PreCondition: None
*
* Input: bool - specifies if the cursor should be on or off
*
* Output: None
*
********************************************************************/
void PRINT_CursorEnable(bool);
#define PRINT_CursorEnable(enabled) LCD_CursorEnable(enabled)

#endif //PRINT_LCD_H
