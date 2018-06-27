
#include <xc.h>
#include "rtcc.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

uint8_t BSP_RTCC_DecToBCD (uint8_t value);
uint8_t BSP_RTCC_BCDToDec (uint8_t value);

void BSP_RTCC_Initialize (BSP_RTCC_DATETIME * value)
{
   // Turn on the secondary oscillator
   __builtin_write_OSCCONL(0x02);

   // Set the RTCWREN bit
   __builtin_write_RTCWEN();

   RCFGCALbits.RTCPTR0 = 1;
   RCFGCALbits.RTCPTR1 = 1;

   // Set it to the correct time
   if (value->bcdFormat)
   {
       RTCVAL = 0x0000 | value->year;
       RTCVAL = ((uint16_t)(value->month) << 8) | value->day;
       RTCVAL = ((uint16_t)(value->weekday) << 8) | value->hour;
       RTCVAL = ((uint16_t)(value->minute) << 8) | value->second;
   }
   else
   {
       // Set (Reserved : year)
       RTCVAL = BSP_RTCC_DecToBCD (value->year);
       // Set (month : day)
       RTCVAL = (BSP_RTCC_DecToBCD (value->month) << 8) | BSP_RTCC_DecToBCD(value->day);
       // Set (weekday : hour)
       RTCVAL = (BSP_RTCC_DecToBCD (value->weekday) << 8) | BSP_RTCC_DecToBCD(value->hour);
       // Set (minute : second)
       RTCVAL = (BSP_RTCC_DecToBCD (value->minute) << 8) | BSP_RTCC_DecToBCD(value->second);
   }

   // Enable RTCC, clear RTCWREN
   RCFGCAL = 0x8000;
}

void BSP_RTCC_TimeGet (BSP_RTCC_DATETIME * value)
{
    uint16_t registerValue;
    bool checkValue;

    RCFGCALbits.RTCPTR0 = 1;
    RCFGCALbits.RTCPTR1 = 1;

    checkValue = RCFGCALbits.RTCSYNC;

    registerValue = RTCVAL;
    value->year = registerValue & 0xFF;
    registerValue = RTCVAL;
    value->month = registerValue >> 8;
    value->day = registerValue & 0xFF;
    registerValue = RTCVAL;
    value->weekday = registerValue >> 8;
    value->hour = registerValue & 0xFF;
    registerValue = RTCVAL;
    value->minute = registerValue >> 8;
    value->second = registerValue & 0xFF;

    if (checkValue)
    {
        BSP_RTCC_DATETIME tempValue;

        do
        {
            memcpy (&tempValue, value, sizeof (BSP_RTCC_DATETIME));

            RCFGCALbits.RTCPTR0 = 1;
            RCFGCALbits.RTCPTR1 = 1;

            value->year = RTCVAL;
            registerValue = RTCVAL;
            value->month = registerValue >> 8;
            value->day = registerValue & 0xFF;
            registerValue = RTCVAL;
            value->weekday = registerValue >> 8;
            value->hour = registerValue & 0xFF;
            registerValue = RTCVAL;
            value->minute = registerValue >> 8;
            value->second = registerValue & 0xFF;

        } while (memcmp (value, &tempValue, sizeof (BSP_RTCC_DATETIME)));
    }

    if (!value->bcdFormat)
    {
        value->year = BSP_RTCC_BCDToDec (value->year);
        value->month = BSP_RTCC_BCDToDec (value->month);
        value->day = BSP_RTCC_BCDToDec (value->day);
        value->weekday = BSP_RTCC_BCDToDec (value->weekday);
        value->hour = BSP_RTCC_BCDToDec (value->hour);
        value->minute = BSP_RTCC_BCDToDec (value->minute);
        value->second = BSP_RTCC_BCDToDec (value->second);
    }
}

// Note : value must be < 100
uint8_t BSP_RTCC_DecToBCD (uint8_t value)
{
    return (((value / 10)) << 4) | (value % 10);
}

uint8_t BSP_RTCC_BCDToDec (uint8_t value)
{
    return ((value >> 4) * 10) + (value & 0x0F);
}

