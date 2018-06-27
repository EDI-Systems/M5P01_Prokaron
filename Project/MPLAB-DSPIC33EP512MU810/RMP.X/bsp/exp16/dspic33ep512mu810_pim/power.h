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

#ifndef BSP_POWER_H
#define BSP_POWER_H

/** Type defintions *********************************/
typedef enum
{
    POWER_SOURCE_USB,
    POWER_SOURCE_MAINS
} POWER_SOURCE;


/*********************************************************************
* Function: POWER_SOURCE POWER_SourceGet(void)
*
* Overview: Gets the current source of power for the board
*
* PreCondition: None
*
* Input: None
*
* Output: POWER_SOURCE - the current source of power for the board
*
********************************************************************/
POWER_SOURCE POWER_SourceGet(void);
#define POWER_SourceGet() POWER_SOURCE_MAINS

#endif //BSP_POWER_H
