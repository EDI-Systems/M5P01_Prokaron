/******************************************************************************
Filename    : SWIIC_conf.h
Author      : lzy
Date        : 27/11/2018
Licence     : None.
Description : The configuration header for IIC simulation driver.
******************************************************************************/

/* Compiler keywords *********************************************************/
#define EXTERN                              extern
/* End Compiler keywords *****************************************************/

/* Configuration Typedefs ****************************************************/
typedef unsigned char                       swiic_u8_t;
typedef unsigned int               		    swiic_ptr_t;
typedef int                        		    swiic_ret_t;
/* End Configuration Typedefs ************************************************/

/* Debugging Defines *********************************************************/
/* #define SWIIC_ASSERT_CORRECT */
/* #define SWIIC_LOG_FUNC_NAME              Log */
/* End Debugging Defines *****************************************************/

/* Configuration Defines *****************************************************/
#define SWIIC_STATIC 
#define SWIIC_DELAY_TIME_VAL                0x01
#define SWIIC_ACK_WAIT_VAL                  0x15
#define SWIIC_DEV_WAIT_VAL                  0x05

#define SWIIC_SDA_READ_FUNC_NAME            SDA_Read
#define SWIIC_SDA_WRITE_FUNC_NAME           SDA_Write
#define SWIIC_SCL_READ_FUNC_NAME            SCL_Read
#define SWIIC_SCL_WRITE_FUNC_NAME           SCL_Write
#define SWIIC_DELAY_FUNC_NAME               RMP_Thd_Delay

/* End Configuration Defines *************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
