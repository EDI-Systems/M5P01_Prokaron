/******************************************************************************
Filename    : rmp_test.h
Author      : pry
Date        : 19/10/2026
Licence     : The Unlicense; see LICENSE for details.
Description : The configuration file for testing program.
******************************************************************************/

/* Config Include ************************************************************/
/* The coverage test defines its own stacks and simulates all the
 * interrupts, so the benchmark testbench is not pulled in here. Only
 * the memory pool size used by the coverage test is defined, and it
 * must match the "Pool" array declared in "rmp_coverage.c". */
#define TEST_MEM_POOL     (4096U)
/* End Config Include ********************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/