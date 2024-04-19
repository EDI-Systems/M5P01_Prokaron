/******************************************************************************
Filename    : sys_arch.h
Author      : hrs 
Date        : 04/09/2024
Licence     : The Unlicense; see LICENSE for details.
Description : Define LwIP porting related data structures.
******************************************************************************/



/* Define ********************************************************************/
#include "rmp.h"

#ifndef LWIP_ARCH_SYS_ARCH_H
#define LWIP_ARCH_SYS_ARCH_H
#define LWIP_COMPAT_MUTEX_ALLOWED

#define LWIP_COMPAT_MUTEX               (1U)
#define MSG_ARRAY_SIZE                  (1024U)
#define SYS_POOL_SIZE                   (1024*16U)
#define SYS_SEM_NULL                    RMP_NULL
#define SYS_MBOX_NULL                   RMP_NULL

typedef struct RMP_Sem sys_sem_t;
typedef struct RMP_Msgq sys_mbox_t;
typedef struct RMP_Thd* sys_thread_t;
typedef struct sys_mbox_msg_struct
{
    struct RMP_List head;
    void* msg;
}sys_mbox_msg_t;

#if LWIP_NETCONN_SEM_PER_THREAD
#define LWIP_NETCONN_THREAD_SEM_GET() sys_arch_netconn_sem_get()
#define LWIP_NETCONN_THREAD_SEM_ALLOC() sys_arch_netconn_sem_alloc()
#define LWIP_NETCONN_THREAD_SEM_FREE() sys_arch_netconn_sem_free()
#endif /* LWIP_NETCONN_SEM_PER_THREAD */

#endif /* LWIP_ARCH_SYS_ARCH_H */
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
