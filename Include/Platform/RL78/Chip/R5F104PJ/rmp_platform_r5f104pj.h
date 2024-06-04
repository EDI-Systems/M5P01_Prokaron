/******************************************************************************
Filename   : rmp_platform_r5f104pj.h
Author     : lht 
Date       : 24/04/2024
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for R5F104PJ.
******************************************************************************/

/* Define ********************************************************************/
/* The iodefine library */
#include "iodefine.h"

/* Debugging */
#define RMP_ASSERT_CORRECT          (0U)
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 32 is usually sufficient */
#define RMP_PREEMPT_PRIO_NUM        (16U)
/* The maximum number of slices allowed */
#define RMP_SLICE_MAX               (32000U)
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_CNT_MAX             (100U)
/* Are we using custom hooks? */
#define RMP_HOOK_EXTRA              (0U)
/* The stzck size of the init thread */
#define RMP_INIT_STACK_SIZE         (256U)
/* The mask/unmask interrupt operations - can be adjusted to fit your own levels */
#define RMP_INT_MASK()              RMP_Int_Disable()
#define RMP_INT_UNMASK()            RMP_Int_Enable()
/* What is the Systick value? */
#define RMP_RL78_SYSTICK_VAL        (1000U)

/* Lowlevel init */
#define RMP_RL78_LOWLVL_INIT() \
{ \
    PIOR0=0x00U; \
    PIOR1=0x00U; \
    /* Set fMX */ \
    CMC=0x40U|0x01U; \
    OSTS=0x07U; \
    MSTOP=0U; \
    while((OSTC&0xFFU)!=0xFFU); \
    /* Set fMAIN */ \
    MCM0=0U; \
    /* Set fSUB */ \
    XTSTOP=1U; \
    OSMC=0x10U; \
    /* Set fCLK */ \
    CSS=0U; \
    /* Set fIH */ \
    HIOSTOP=0U; \
    WDTIMK=1U; \
    IAWCTL=0x00U; \
    /* SAU0 and UART1 initial setting */ \
    SAU0EN=1U; \
    SPS0=0x0004U; \
    ST0|=0x0004U; \
    /* Disable interrupt INTST1,INTSR1,INTSRE1 */ \
    STMK1=1U; \
    STIF1=0U; \
    SRMK1=1U; \
    SRIF1=0U; \
    SREMK1=1U; \
    SREIF1=0U; \
    /* Set INTST1 low priority */ \
    STPR11=1U; \
    STPR01=1U; \
    SMR02=0x0020U|0x0002U|0x0001U; \
    SCR02=0x8000U|0x0080U|0x0010U|0x0007U; \
    SDR02=0xCE00U; \
    SO0|=0x0004U; \
    SOL0|=0x0000U; \
    /* Enable UART1 output */ \
    SOE0|=0x0004U; \
    /* Set TxD1 pin */ \
    PMC0&=0xFBU; \
    P0|=0x04U; \
    PM0&=0xFBU; \
    /* Output level normal */ \
    SO0|=0x0004U; \
    /* Enable UART1 output */ \
    SOE0|=0x0004U; \
    /* Enable UART1 transmit */ \
    SS0|=0x0004U; \
} \
while(0)
#define RMP_RL78_TIM_CLR()          (TMIF00=0U)

#define RMP_RL78_PUTCHAR(CHAR) \
{ \
    TXD1=CHAR; \
    while(STIF1==0U); \
    STIF1=0U; \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
