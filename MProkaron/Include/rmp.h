/******************************************************************************
Filename    : rmp.h
Author      : pry
Date        : 01/04/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The header of the RMP RTOS. This header defines the error codes,
              operation flags and system call numbers in a generic way.
******************************************************************************/

/* Defines *******************************************************************/
#ifndef __RMP_H__
#define __RMP_H__

#define __HDR_DEFS__
#include "Platform/rmp_platform.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEFS__

#define __HDR_STRUCTS__
#include "Platform/rmp_platform.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCTS__

#define __HDR_PUBLIC_MEMBERS__
#include "Platform/rmp_platform.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC_MEMBERS__

#endif /* __RMP_H__ */
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
