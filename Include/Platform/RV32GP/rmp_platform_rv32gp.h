/******************************************************************************
Filename    : rmp_platform_rv32gp.h
Author      : pry
Date        : 01/04/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "rmp_platform_rv32gp.c".
******************************************************************************/

/* Defines *******************************************************************/
#ifdef __HDR_DEFS__
#ifndef __RMP_PLATFORM_RV32GP_H_DEFS__
#define __RMP_PLATFORM_RV32GP_H_DEFS__
/*****************************************************************************/
/* Basic Types ***************************************************************/
#ifndef __RMP_S32_T__
#define __RMP_S32_T__
typedef signed int rmp_s32_t;
#endif

#ifndef __RMP_S16_T__
#define __RMP_S16_T__
typedef signed short rmp_s16_t;
#endif

#ifndef __RMP_S8_T__
#define __RMP_S8_T__
typedef signed char rmp_s8_t;
#endif

#ifndef __RMP_U32_T__
#define __RMP_U32_T__
typedef unsigned int rmp_u32_t;
#endif

#ifndef __RMP_U16_T__
#define __RMP_U16_T__
typedef unsigned short rmp_u16_t;
#endif

#ifndef __RMP_U8_T__
#define __RMP_U8_T__
typedef unsigned char rmp_u8_t;
#endif
/* End Basic Types ***********************************************************/

/* Begin Extended Types ******************************************************/
#ifndef __RMP_PTR_T__
#define __RMP_PTR_T__
/* The typedef for the pointers - This is the raw style. Pointers must be unsigned */
typedef rmp_u32_t rmp_ptr_t;
#endif

#ifndef __RMP_CNT_T__
#define __RMP_CNT_T__
/* The typedef for the count variables */
typedef rmp_s32_t rmp_cnt_t;
#endif

#ifndef __RMP_RET_T__
#define __RMP_RET_T__
/* The type for process return value */
typedef rmp_s32_t rmp_ret_t;
#endif
/* End Extended Types ********************************************************/

/* System macros *************************************************************/
/* Compiler "extern" keyword setting */
#define EXTERN                          extern
/* The order of bits in one CPU machine word */
#define RMP_WORD_ORDER                  (5U)
/* The maximum length of char printing - no need to change this in most cases */
#define RMP_DEBUG_PRINT_MAX             (255U)
/* Descending stack, 16-byte alignment */
#define RMP_INIT_STACK                  RMP_INIT_STACK_DESCEND(4U)
/* MSB/LSB extraction */
#define RMP_MSB_GET(VAL)                RMP_MSB_Generic(VAL)
#define RMP_LSB_GET(VAL)                RMP_LSB_Generic(VAL)

/* The CPU and application specific macros are here */
#include "rmp_platform_rv32gp_conf.h"
/* End System macros *********************************************************/

/* RV32GPIMAC specific macros **************************************************/

/*****************************************************************************/
/* __RMP_PLATFORM_RV32GP_H_DEFS__ */
#endif
/* __HDR_DEFS__ */
#endif
/* End Defines ***************************************************************/

/* Structs *******************************************************************/
#ifdef __HDR_STRUCTS__
#ifndef __RMP_PLATFORM_RV32GP_H_STRUCTS__
#define __RMP_PLATFORM_RV32GP_H_STRUCTS__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEFS__
#undef __HDR_DEFS__
/*****************************************************************************/
struct RMP_RV32GP_Stack
{
    rmp_ptr_t PC;
    rmp_ptr_t X1;
    rmp_ptr_t MSTATUS;
    rmp_ptr_t X3;
    rmp_ptr_t X4;
    rmp_ptr_t X5;
    rmp_ptr_t X6;
    rmp_ptr_t X7;
    rmp_ptr_t X8;
    rmp_ptr_t X9;
    rmp_ptr_t X10;
    rmp_ptr_t X11;
    rmp_ptr_t X12;
    rmp_ptr_t X13;
    rmp_ptr_t X14;
    rmp_ptr_t X15;
    rmp_ptr_t X16;
    rmp_ptr_t X17;
    rmp_ptr_t X18;
    rmp_ptr_t X19;
    rmp_ptr_t X20;
    rmp_ptr_t X21;
    rmp_ptr_t X22;
    rmp_ptr_t X23;
    rmp_ptr_t X24;
    rmp_ptr_t X25;
    rmp_ptr_t X26;
    rmp_ptr_t X27;
    rmp_ptr_t X28;
    rmp_ptr_t X29;
    rmp_ptr_t X30;
    rmp_ptr_t X31;
};
/*****************************************************************************/
/* __RMP_PLATFORM_RV32GP_H_STRUCTS__ */
#endif
/* __HDR_STRUCTS__ */
#endif
/* End Structs ***************************************************************/

/* Private Global Variables **************************************************/
#if(!(defined __HDR_DEFS__||defined __HDR_STRUCTS__))
#ifndef __RMP_PLATFORM_RV32GP_MEMBERS__
#define __RMP_PLATFORM_RV32GP_MEMBERS__

/* In this way we can use the data structures and definitions in the headers */
#define __HDR_DEFS__

#undef __HDR_DEFS__

#define __HDR_STRUCTS__

#undef __HDR_STRUCTS__

/* If the header is not used in the public mode */
#ifndef __HDR_PUBLIC_MEMBERS__
/*****************************************************************************/

/*****************************************************************************/
/* End Private Global Variables **********************************************/

/* Private C Function Prototypes *********************************************/ 
/*****************************************************************************/

/*****************************************************************************/
#define __EXTERN__
/* End Private C Function Prototypes *****************************************/

/* Public Global Variables ***************************************************/
/* __HDR_PUBLIC_MEMBERS__ */
#else
#define __EXTERN__ EXTERN 
/* __HDR_PUBLIC_MEMBERS__ */
#endif

/*****************************************************************************/

/*****************************************************************************/

/* End Public Global Variables ***********************************************/

/* Public C Function Prototypes **********************************************/
/*****************************************************************************/
/* Interrupts */
EXTERN void RMP_Int_Disable(void);
EXTERN void RMP_Int_Enable(void);
EXTERN void RMP_Int_Mask(rmp_ptr_t Level);

EXTERN void _RMP_Start(rmp_ptr_t Entry, rmp_ptr_t Stack);
__EXTERN__ void _RMP_Yield(void);

/* Platform specific */
EXTERN rmp_ptr_t __RMP_RV32GP_MCAUSE_Get(void);
EXTERN void __RMP_RV32GP_MTVEC_Set(rmp_ptr_t MTVEC);
EXTERN rmp_ptr_t __RMP_RV32GP_MCYCLE_Get(void);
EXTERN void __RMP_RV32GP_Mem_FENCE(void);
EXTERN void __RMP_RV32GP_Handler(void);

/* Initialization */
__EXTERN__ rmp_ptr_t _RMP_Stack_Init(rmp_ptr_t Stack,
                                     rmp_ptr_t Size,
                                     rmp_ptr_t Entry,
                                     rmp_ptr_t Param);
__EXTERN__ void _RMP_Lowlvl_Init(void);
__EXTERN__ void RMP_Putchar(char Char);
__EXTERN__ void _RMP_Plat_Hook(void);

/* Interrupt handler */
__EXTERN__ void __RMP_RV32GP_Ctx_Handler(void);
__EXTERN__ void __RMP_RV32GP_Tim_Handler(void);
__EXTERN__ void __RMP_RV32GP_Vct_Handler(rmp_ptr_t Mcause);
__EXTERN__ void _RMP_RV32GP_Handler(void);
/*****************************************************************************/
/* Undefine "__EXTERN__" to avoid redefinition */
#undef __EXTERN__
/* __RMP_PLATFORM_RV32GP_MEMBERS__ */
#endif
/* !(defined __HDR_DEFS__||defined __HDR_STRUCTS__) */
#endif
/* End Public C Function Prototypes ******************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
