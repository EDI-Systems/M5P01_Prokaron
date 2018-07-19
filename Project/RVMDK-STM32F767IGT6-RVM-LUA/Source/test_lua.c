/******************************************************************************
Filename    : test.c
Author      : pry 
Date        : 22/07/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The performance testbench for RMP. Do not modify this file; what
              you need to modify is the test chip header and the platform chip
              header.
******************************************************************************/

/* Includes ******************************************************************/
#include "rmp.h"
#include "lua.h"
#include "lauxlib.h"
#include "time.h"
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
char Buf[1024]={0};

void RMP_Point(cnt_t X, cnt_t Y, ptr_t C)
{

}

void RMP_lua_writestring(char* string, int len)
{
    int i;
    for(i=0;i<len;i++)
        RMP_Putchar(string[i]);
}

void RMP_lua_writeline(void)
{
    RMP_Putchar('\r');
    RMP_Putchar('\n');
}

time_t time(time_t* time)
{
    *time=0;
    return 0;
}

void exit(int status)  
{  
    while(1);
}  
  
int system(const char * string)  
{  
    return 0;  
}  

static void usage_exit(void) {
	while(1);
}
/* Lua test script */  
const char lua_test[] = {   
    "print(\"Hello,I am lua!\\n--this is newline printf\")\n"  
    "function foo()\n"  
    "  local i = 0\n"  
    "  local sum = 1\n"  
    "  while i <= 10 do\n"  
    "    sum = sum * 2\n"  
    "    i = i + 1\n"  
    "  end\n"  
    "return sum\n"  
    "end\n"  
    "print(\"sum =\", foo())\n"  
    "print(\"and sum = 2^11 =\", 2 ^ 11)\n"  
    "print(\"exp(200) =\", math.exp(200))\n"  
};

/* Begin Function:Func_1 ******************************************************
Description : The test function group 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Func_1(void)
{
    lua_State *L;  
      
    L = luaL_newstate();
    luaL_openlibs(L);  
    luaopen_base(L);  
    luaL_dostring(L, lua_test);
    lua_close(L);  
    
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
