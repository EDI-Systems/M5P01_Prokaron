/******************************************************************************
Filename    : PCF8574_conf.h
Author      : pry
Date        : 03/11/2018
Licence     : LGPL v3+; see COPYING for details.
Description : The configuration header for PCF8574-series chip driver.
******************************************************************************/

/* Compiler keywords *********************************************************/
#define EXTERN                              extern
/* End Compiler keywords *****************************************************/

/* Configuration Typedefs ****************************************************/
typedef unsigned char                       pcf8574_u8_t;
typedef unsigned int                        pcf8574_ptr_t;
typedef int                                 pcf8574_ret_t;
/* End Configuration Typedefs ************************************************/

/* Debugging Defines *********************************************************/
/* #define PCF8574_ASSERT_CORRECT */
/* #define PCF8574_LOG_FUNC_NAME            RDV_Log  */
/* End Debugging Defines *****************************************************/

/* Configuration Defines *****************************************************/
#define PCF8574_STATIC

#define PCF8574_IIC_ADDR_VAL                0x40
#define PCF8574_IIC_READ_FUNC_NAME          SWIIC_Read
#define PCF8574_IIC_WRITE_FUNC_NAME         SWIIC_Write

/* End Configuration Defines *************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
