/******************************************************************************
Filename   : platform_x86_linux_conf.h
Author     : pry
Date       : 24/06/2017
Licence    : The Unlicense; see LICENSE for details.
Description: The configuration file for POSIX platform.
******************************************************************************/

/* Defines *******************************************************************/
/* Standard unix headers */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/syscall.h>
#include <linux/ptrace.h>
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 32 is usually sufficient */
#define RMP_MAX_PREEMPT_PRIO         32
/* The maximum number of slices allowed */
#define RMP_MAX_SLICES               100000
/* The maximum number of semaphore counts allowed */
#define RMP_SEM_MAX_NUM              1000
/* Are we using custom hooks? */
#define RMP_USE_HOOKS                RMP_FALSE
/* The stack size of the init thread */
#define RMP_INIT_STACK_SIZE          65535
/* The mask/unmask interrupt operations - on linux this is not supported */
#define RMP_MASK_INT()
#define RMP_UNMASK_INT()

/* What is the tick interval? */
#define RMP_POSIX_TIME_VAL           100

/* This is for debugging output */
#define RMP_POSIX_PUTCHAR(CHAR) \
do \
{ \
	putchar((CHAR)); \
} \
while(0)
/* End Defines ***************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
