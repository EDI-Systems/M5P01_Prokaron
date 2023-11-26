/******************************************************************************
Filename    : rmp.h
Author      : pry
Date        : 01/04/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The header of the RMP RTOS. This header defines the error codes,
              operation flags and system call numbers in a generic way.
******************************************************************************/

/* Define ********************************************************************/
#ifndef __RMP__
#define __RMP__

#define __HDR_DEF__
#include "rmp_platform.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "rmp_platform.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

#define __HDR_PUBLIC__
#include "rmp_platform.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC__

#endif /* __RMP__ */
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
