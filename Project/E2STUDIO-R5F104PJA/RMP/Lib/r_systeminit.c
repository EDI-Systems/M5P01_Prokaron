/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2023 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_systeminit.c
* Version      : CodeGenerator for RL78/G14 V2.05.07.02 [17 Nov 2023]
* Device(s)    : R5F104PJ
* Tool-Chain   : CCRL
* Description  : This file implements system initializing function.
* Creation Date: 2024/5/13
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include <Lib/iodefine.h>


/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Systeminit
* Description  : This function initializes every macro.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Systeminit(void)
{
    unsigned char reset_flag;
    reset_flag=RESF;
    unsigned char temp_stab_set;
    unsigned char temp_stab_wait;
    PIOR0=0x00U;
    PIOR1=0x00U;
    /* Set fMX */
    CMC=0x40U|0x00U|0x01U|0x00U;
    OSTS=0x07U;
    MSTOP=0U;
    temp_stab_set=0xFFU;
    do
    {
        temp_stab_wait=OSTC;
        temp_stab_wait&=temp_stab_set;
    }
    while(temp_stab_wait!=temp_stab_set);
    /* Set fMAIN */
    MCM0=0U;
    /* Set fSUB */
    XTSTOP=1U;
    OSMC=0x10U;
    /* Set fCLK */
    CSS=0U;
    /* Set fIH */
    HIOSTOP=0U;
    WDTIMK=1U;
    IAWCTL=0x00U;
    /* SAU0 and UART1 initial setting */
    SAU0EN=1U;
    SPS0=0x0004|0x0000;
    ST0|=0x0004;
    /*disable interrupt INTST1,INTSR1,INTSRE1*/
    STMK1=1U;
    STIF1=0U;
    SRMK1=1U;
    SRIF1=0U;
    SREMK1=1U;
    SREIF1=0U;
    /* Set INTST1 low priority */
    STPR11=1U;
    STPR01=1U;
    SMR02=0x0020|0x0000|0x0000|0x0002|0x0001;
    SCR02=0x8000|0x0000|0x0000|0x0080|0x0010|0x0007;
    SDR02=0xCE00;
    SO0|=0x0004;
    SOL0|=0x0000;
    /* enable UART1 output */
    SOE0|=0x0004;
    /* Set TxD1 pin */
    PMC0&=0xFBU;
    P0|=0x04U;
    PM0&=0xFBU;
    /* output level normal */
    SO0|=0x0004;
    /* enable UART1 output */
    SOE0|=0x0004;
    /* enable UART1 transmit */
    SS0|=0x0004;
}


/***********************************************************************************************************************
* Function Name: hdwinit
* Description  : This function initializes hardware setting.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void hdwinit(void)
{
    R_Systeminit();
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
