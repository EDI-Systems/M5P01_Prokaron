/******************************************************************************
Filename    : test.c
Author      : pry 
Date        : 22/07/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The performance testbench for RMP. Do not modify this file; what
              you need to modify is the test chip header and the platform chip
              header.
******************************************************************************/

/******************************************************************************
Filename    : test_STM32F767IG_RVM.h
Author      : pry 
Date        : 22/07/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The testbench for STM32F767IG, running in the RVM.
******************************************************************************/

/* Includes ******************************************************************/
#include "RMP.h"
#include "duktape.h"
/* End Includes **************************************************************/

/* Defines *******************************************************************/
/* Where are the initial stacks */
#define THD1_STACK        (&Stack_1[8100])
#define THD2_STACK        (&Stack_2[8100])
/* Are we testing the memory pool? */
#define TEST_MEM_POOL     8192
/* End Defines ***************************************************************/

/* Globals *******************************************************************/
ptr_t Stack_1[8192];
ptr_t Stack_2[8192];
/* End Globals ***************************************************************/

/* Globals *******************************************************************/
/* Kernel objects */
volatile struct RMP_Thd Thd_1;
volatile struct RMP_Thd Thd_2;
volatile struct RMP_Sem Sem_1;
/* Memory pool */
volatile ptr_t Pool[TEST_MEM_POOL]={0};
/* End Globals ***************************************************************/
char Buf[1024];

time_t time(time_t* time)
{
    *time=0;
    return 0;
}

static duk_ret_t native_print(duk_context *ctx) {
	duk_push_string(ctx, " ");
	duk_insert(ctx, 0);
	duk_join(ctx, duk_get_top(ctx) - 1);
	sprintf(Buf,"%s\n", duk_to_string(ctx, -1));
    RMP_PRINTK_S(Buf);
	return 0;
}

static duk_ret_t eval_raw(duk_context *ctx, void *udata) {
	(void) udata;
	duk_eval(ctx);
	return 1;
}

static duk_ret_t tostring_raw(duk_context *ctx, void *udata) {
	(void) udata;
	duk_to_string(ctx, -1);
	return 1;
}

static void usage_exit(void) {
	while(1);
}

/* Begin Function:Func_1 ******************************************************
Description : The test function group 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Func_1(void)
{
    duk_context *ctx;
	int i;
	const char *res;
    
    const char argc=3;
    const char* argv[3]={"sss\n","var fac = function me(x) { return x <= 1 ? 1 : x * me(x-1) }","fac(10)"};

	if (argc < 2) {
		usage_exit();
	}

	ctx = duk_create_heap_default();

	duk_push_c_function(ctx, native_print, DUK_VARARGS);
	duk_put_global_string(ctx, "print");

	for (i = 1; i < argc; i++) {
		sprintf(Buf,"=== eval: '%s' ===\n", argv[i]);
        RMP_PRINTK_S(Buf);
		duk_push_string(ctx, argv[i]);
		duk_safe_call(ctx, eval_raw, NULL, 1 /*nargs*/, 1 /*nrets*/);
		duk_safe_call(ctx, tostring_raw, NULL, 1 /*nargs*/, 1 /*nrets*/);
		res = duk_get_string(ctx, -1);
		sprintf(Buf,"%s\n", res ? res : "null");
        RMP_PRINTK_S(Buf);
		duk_pop(ctx);
	}

	duk_destroy_heap(ctx);
    while(1);
}
/* End Function:Func_1 *******************************************************/

/* Begin Function:Func_2 ******************************************************
Description : The test function group 2.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Func_2(void)
{
    while(1);
}
/* End Function:Func_2 *******************************************************/

/* Begin Function:RMP_Init ****************************************************
Description : The init thread hook functions.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Init_Hook(void)
{
    /* Clean up the structures */
    RMP_Clear(&Thd_1,sizeof(struct RMP_Thd));
    RMP_Clear(&Thd_2,sizeof(struct RMP_Thd));
    RMP_Clear(&Sem_1,sizeof(struct RMP_Sem));
    /* Create counting semaphore */
    RMP_Sem_Crt(&Sem_1,0);
    /* Start threads */
    RMP_Thd_Crt(&Thd_1, Func_1, THD1_STACK, (void*)0x1234, 1, 5);
    RMP_Thd_Crt(&Thd_2, Func_2, THD2_STACK, (void*)0x4321, 1, 5);
}

void RMP_Init_Idle(void)
{
    return;
}
/* End Function:RMP_Init *****************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
