#include "rmp.h"
#include "math.h"


struct RMP_Thd Thd_1;
struct RMP_Thd Thd_2;
rmp_ptr_t Stack_1[256];
rmp_ptr_t Stack_2[256];

void Func_1(void)
{
    volatile float a=0.1234;
    while(1)
    {
        /* Do some floating point calculations */
        a=cosf(a);
    };
}

void Func_2(void)
{
    volatile float a=4;
    while(1)
    {
        /* Do some floating point calculations */
        a=a+1;
        a=a-1;
    };
}

void RMP_Init_Hook(void)
{
    /* Clean up the structures */
    RMP_Clear(&Thd_1,sizeof(struct RMP_Thd));
    RMP_Clear(&Thd_2,sizeof(struct RMP_Thd));
    /* Start threads */
    RMP_Thd_Crt(&Thd_1, Func_1, &Stack_1[201], (void*)0x1234, 1, 5);
    RMP_Thd_Crt(&Thd_2, Func_2, &Stack_2[201], (void*)0x4321, 1, 3);
}

void RMP_Init_Idle(void)
{
    return;
}

