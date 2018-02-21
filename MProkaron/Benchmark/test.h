
#include "RMP.h"

const struct RVM_Hdr_Pgtbl RMP_Pgtbl[1]=
{
    /* The first page table */
    {
        0                                                                      /* ptr_t Parent */,
        0x00000000                                                             /* ptr_t Addr */,
        RME_PGTBL_ORDER(RME_PGTBL_SIZE_512M,RME_PGTBL_NUM_8)                   /* ptr_t Order */,
        {                                                                      /* u8 Flags[8] */
            RME_PGTBL_ALL_PERM,RME_PGTBL_ALL_PERM,
            RME_PGTBL_ALL_PERM,RME_PGTBL_ALL_PERM,
            RME_PGTBL_ALL_PERM,RME_PGTBL_ALL_PERM,
            RME_PGTBL_ALL_PERM,RME_PGTBL_ALL_PERM
        }
    }
};

const struct RVM_Image RMP_Image=
{
    RVM_MAGIC                                                                  /* const ptr_t Magic */,
    "VM Domain 01"                                                             /* s8 Name[16] */,
    _RMP_Entry                                                                 /* void* User_Entry */,
    &RMP_User_Stack                                                            /* void* User_Stack */,
    0x100                                                                      /* ptr_t User_Size */,
    _RMP_Int                                                                   /* void* Int_Entry */,
    &RMP_Int_Stack                                                             /* void* Int_Stack */,
    0x400                                                                      /* ptr_t Int_Size */,
    
    &RMP_Param                                                                 /* struct RVM_Param* Param */,
    &RMP_Regs                                                                  /* struct RVM_Regs* Regs */,
    &RMP_Flag                                                                  /* struct RVM_Int_Flag* Int_Flags */,
    
    RMP_Console                                                                /* void* Console_Buf */,
    RMP_KERNEL_DEBUG_MAX_STR                                                   /* ptr_t Console_Size */,
    
    1                                                                          /* ptr_t Prio */,
    10                                                                         /* ptr_t Slices */,
    
    1                                                                          /* ptr_t Pgtbl_Num */,
    RMP_Pgtbl                                                                  /* const struct RVM_Hdr_Pgtbl* Pgtbl */,
    0                                                                          /* const struct RVM_Image* const * const Next_Image; */
};

ptr_t RMP_Stack_2[256];
struct RMP_Thd RMP_Thd_2={0};

ptr_t RMP_Stack_3[256];
struct RMP_Thd RMP_Thd_3={0};

ptr_t tval;

/* All mechanisms listed here are subject to further testing. Need another benchmark or etc to measure performance */

/* Begin Function:Test_Func_2 *************************************************
Description : Test func 1.
Input       : None.
Output      : None.
Return      : int - This function never returns.
******************************************************************************/
void Test_Func_2(void)
{
    while(1)
    {
        RMP_Thd_Delay(3000);
        RMP_PRINTK_S("Delayed 3000 cycles\n");
        
        RMP_Thd_Snd(&RMP_Thd_2, tval, RMP_MAX_SLICES);
        tval++;
    };
}
/* End Function:Test_Func_2 **************************************************/

/* Begin Function:Test_Func_1 *************************************************
Description : Test func 1.
Input       : None.
Output      : None.
Return      : int - This function never returns.
******************************************************************************/
void Test_Func_1(void)
{
    ptr_t Data;
    
    while(1)
    {
        RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
        
        RMP_PRINTK_S("Received ");
        RMP_PRINTK_I(Data);
        RMP_PRINTK_S("\n");
    };
}
/* End Function:Test_Func_1 **************************************************/

/* Begin Function:RMP_Init_Hook ***********************************************
Description : The entry of the user thread. This is the first user thread that
              will be created.
              The user threads should never return.
Input       : None.
Output      : None.
Return      : int - This function never returns.
******************************************************************************/
void RMP_Init_Hook(void)
{
    /* Start the second thread here */
    RMP_Thd_Crt(&RMP_Thd_2,(ptr_t)Test_Func_1, (ptr_t)&RMP_Stack_2[238], 0x12345678, 1, 5);
    RMP_Thd_Crt(&RMP_Thd_3,(ptr_t)Test_Func_2, (ptr_t)&RMP_Stack_3[238], 0x12345678, 1, 5);
    
    /* Initialize handlers as needed */
}
/* End Function:RMP_Init_Hook ************************************************/

/* Begin Function:RMP_Init_Idle ***********************************************
Description : The entry of the user thread. This is the first user thread that
              will be created.
              The user threads should never return.
Input       : None.
Output      : None.
Return      : int - This function never returns.
******************************************************************************/
void RMP_Init_Idle(void)
{
    return;
}
/* End Function:RMP_Init_Idle ************************************************/

/* These tests shall be conducted simutaneously to make sure that everything works */
/* Two threads yielding to each other */

/* One thread performing a delay loop flashing one variable(use tracing) */

/* One thread suspends the thread performing delay loop every 10 secs, test suspend + delay */

/* Simple send/Receive test pair */

/* Simple send/Receive test pair with different timeouts, print the log to console */

/* Pubsub semaphore test pair with different timeouts, print the log to console */

/* One thread suspending the send/receive pair with some delay */

/* Priority system */
/* Creation/Unsuspend/Delay/Rcv/Send/Semaphore @ different priority levels, see if priority system works */

/* Hooks */

/* Send from ISR with systick hook 1sec to both systems */

/* Send/Rcv chaining */

/* Measurements should be conducted according to the M7M1 manual */

/* This should be good enough, after we finish the manual we can go to deal with the macros */

/* We gonna finish the documents at home, tonight, be quick */



