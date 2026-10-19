/******************************************************************************
Filename    : rmp_coverage.c
Author      : pry, zk (zhang kai); LLM (GLM-5.2)
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The white-box coverage test for RMP. Instead of firing interrupts,
              this test simulates them. This test is designed specifically for
              32-bit systems, and may need some tweaks to run on other systems.
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
#define DUMMY           ((void*)0x1234U)
#define STR             "01234567890123456789012345678901234567890123456789\r\n"
#define PASS(X)         RMP_ASSERT((X)==0)
#define FAIL(X)         RMP_ASSERT((X)!=0)
#define PRIO_BOTTOM     (1U)
#define PRIO_LOWER      (2U)
#define PRIO_COV        (3U)
#define PRIO_HIGHER     (4U)
#define PRIO_TOP        (5U)
/* End Define ****************************************************************/

/* Global ********************************************************************/
/* Stack of the main coverage thread created in RMP_Init_Hook
 * (entry Thd_Cov_Entry, priority PRIO_COV) */
rmp_ptr_t Stack_Cov[512];
/* Stack of the primary worker thread Thd_1 used by nearly all test cases
 * (Cov_Thd_Param, Cov_Thd_Oper, Cov_Thd_Del, Cov_Sem_Oper, Test_Set_2) */
rmp_ptr_t Stack_1[512];
/* Stack of the secondary worker thread Thd_2 (the receiver whose full mailbox
 * blocks Thd_1 in the Cov_Thd_Del sender/receiver pairs) */
rmp_ptr_t Stack_2[512];
/* Kernel objects */
/* Control block reserved for the main coverage thread; used as the nominal
 * send target in the mailbox tests - the self-send rejection case of
 * Cov_Thd_Param, and the blocking sends of Cov_Thd_Del_Snd_Func and
 * Cov_Thd_Del_Snd_Delay_Func send their first message into its mailbox */
volatile struct RMP_Thd Thd_Cov;
/* Control block of the primary worker thread; repeatedly created, deleted
 * and reprioritized by the thread tests, and pended on the semaphore by
 * Test_Set_2 in Cov_Sem_Oper */
volatile struct RMP_Thd Thd_1;
/* Control block of the secondary worker thread Thd_2, used only by the two
 * Cov_Thd_Del cases where a blocked sender must be aborted by deleting
 * its receiver */
volatile struct RMP_Thd Thd_2;
/* Counting semaphore used by Cov_Sem_Param/Cov_Sem_Oper, and by the
 * Cov_Thd_Del cases that delete a thread blocked on a semaphore
 * (SEMBLK/SEMDLY states) */
volatile struct RMP_Sem Sem_1;
/* Extended kernel objects */
/* FIFO used directly by Cov_Fifo_Msgq, and also the embedded FIFO
 * whose state is forced to RMP_FIFO_FREE to make RMP_Msgq_Rcv fail */
volatile struct RMP_Fifo Fifo_1;
/* Message queue used by Cov_Fifo_Msgq; its State and Sem.State are
 * forcibly corrupted to reach the internal error branches */
volatile struct RMP_Msgq Msgq_1;
/* Blocking message queue used by Cov_Bmq; its Msgq and Sem members are
 * forcibly corrupted to reach the internal error branches */
volatile struct RMP_Bmq Bmq_1;
/* Memory pool */
/* Backing store of the memory pool in Cov_Mem: repeatedly zeroed and
 * reinitialized by RMP_Mem_Init, then allocated/freed/reallocated to
 * exercise the TL2F allocator's FLI/SLI paths; TEST_MEM_POOL-sized requests
 * refer to the same array */
volatile rmp_ptr_t Pool[4096];
/* End Global ****************************************************************/

/* Function:Cov_Bit ***********************************************************
Description : Bit manipulation tests.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Bit(void)
{
    /* MSB-finding: tests the 31-24 branch, table entry 0x80 */
    RMP_MSB_Generic(0x80000000UL);
    /* MSB-finding: tests the 23-16 branch, table entry 0x80 */
    RMP_MSB_Generic(0x00800000UL);
    /* MSB-finding: tests the 15-8 branch, table entry 0x04 */
    RMP_MSB_Generic(0x00040000UL);
    /* MSB-finding: tests the 15-8 branch, table entry 0x80 */
    RMP_MSB_Generic(0x00008000UL);
    /* MSB-finding: tests the 7-0 branch, table entry 0x80 */
    RMP_MSB_Generic(0x00000080UL);

    /* LSB-finding: tests the 31-24 branch, table entry 0x80 */
    RMP_LSB_Generic(0x80000000UL);
    /* LSB-finding: tests the 24-16 branch, table entry 0x80 */
    RMP_LSB_Generic(0x00800000UL);
    /* LSB-finding: tests the 15-8 branch, table entry 0x04 */
    RMP_LSB_Generic(0x00040000UL);
    /* LSB-finding: tests the 15-8 branch, table entry 0x80 */
    RMP_LSB_Generic(0x00008000UL);
    /* LSB-finding: tests the 7-0 branch, table entry 0x80 */
    RMP_LSB_Generic(0x00000080UL);
}
/* End Function:Cov_Bit ******************************************************/

/* Function:Cov_Print *********************************************************
Description : Print helper tests.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Print(void)
{
    /* Int-print: tests the Int==0 early-return branch */
    RMP_Int_Print;
    /* Int-print: tests the Int<0 negative branch and the multi-digit loop */
    RMP_Int_Print(-1);
    /* Int-print: tests the single-digit positive path */
    RMP_Int_Print;
    /* Int-print: tests a three-digit positive number */
    RMP_Int_Print;
    /* Int-print: tests a large positive number with more loop iterations */
    RMP_Int_Print;
    RMP_ASSERT(RMP_Str_Print("\r\n")==2U);

    /* Hex-print: tests the Uint==0 early-return branch */
    RMP_Hex_Print(0U);
    /* Hex-print: tests the numeric digit (0-9) branch */
    RMP_Hex_Print(0x12345678UL);
    /* Hex-print: tests the hex letter (A-F) branch */
    RMP_Hex_Print(0xABCDEF12UL);
    RMP_ASSERT(RMP_Str_Print("\r\n")==2U);

    /* Str-print: tests the normal loop and its break-on-NUL branch */
    RMP_ASSERT(RMP_Str_Print("Hello, World!")==13U);
    /* Str-print: tests the loop bound against overlong strings */
    RMP_ASSERT(RMP_Str_Print(STR STR STR STR STR STR)==RMP_DEBUG_PRINT_MAX);
    RMP_ASSERT(RMP_Str_Print("\r\n")==2U);
}
/* End Function:Cov_Print ****************************************************/

/* Function:Cov_Lock **********************************************************
Description : Scheduler lock tests.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Lock(void)
{
    /* Sched-lock: sets the lock count to 1 */
    RMP_Sched_Lock();
    /* Sched-unlock: tests the count==1, no-pending-yield branch */
    RMP_Sched_Unlock();

    /* Sched-lock: first lock, raises the lock count to 1 */
    RMP_Sched_Lock();
    /* Sched-lock: nested lock, raises the lock count to 2 */
    RMP_Sched_Lock();
    /* Sched-unlock: tests the count>1 nested-unlock branch */
    RMP_Sched_Unlock();
    /* Sched-unlock: drops the count back to 0 */
    RMP_Sched_Unlock();

    /* Sched-lock: holds the scheduler locked across a yield request */
    RMP_Sched_Lock();
    /* Yield: pends a scheduler activation while the lock is held */
    RMP_Yield();
    /* Sched-unlock: tests the count==1, pending-yield branch */
    RMP_Sched_Unlock();
}
/* End Function:Cov_Lock *****************************************************/

/* Function:Cov_Thd_Param *****************************************************
Description : Thread simple parameter tests.
Input       : None
Output      : None.
Return      : None
******************************************************************************/
void Cov_Thd_Param_Delay_Func(void)
{
    /* Thd-delay: blocks the test thread on the delay list, wakeup path */
    RMP_Thd_Delay(10000U);
    while;
}

void Cov_Thd_Param_Suspend_Func(void)
{
    /* Thd-suspend: suspends itself, blocking the thread forever */
    RMP_Thd_Suspend(&Thd_1);
    while;
}

void Cov_Thd_Param(void)
{
    /* Clear the thread control block before use */
    RMP_Clear(&Thd_1, sizeof(Thd_1));

    /* Thd-crt: tests the NULL control block rejection branch */
    FAIL(RMP_Thd_Crt(RMP_NULL, DUMMY, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_BOTTOM, 2U));
    /* Thd-crt: tests the priority-out-of-range rejection branch */
    FAIL(RMP_Thd_Crt(&Thd_1, DUMMY, RMP_NULL, Stack_1, sizeof(Stack_1), RMP_PREEMPT_PRIO_NUM, 1U));
    /* Thd-crt: tests the zero-timeslice rejection branch */
    FAIL(RMP_Thd_Crt(&Thd_1, DUMMY, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_BOTTOM, 0U));
    /* Thd-crt: tests the timeslice-too-large rejection branch */
    FAIL(RMP_Thd_Crt(&Thd_1, DUMMY, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_BOTTOM, RMP_SLICE_MAX));
    /* Thd-crt: tests the NULL stack rejection branch */
    FAIL(RMP_Thd_Crt(&Thd_1, DUMMY, RMP_NULL, RMP_NULL, sizeof(Stack_1), PRIO_BOTTOM, 2U));
    /* Thd-crt: tests the stack-too-small rejection branch */
    FAIL(RMP_Thd_Crt(&Thd_1, DUMMY, RMP_NULL, Stack_1, sizeof(RMP_STACK_STRUCT), PRIO_BOTTOM, 2U));

    /* Sched-lock: hold the scheduler so the created thread cannot preempt us */
    RMP_Sched_Lock();
    /* Thd-crt: tests the successful creation path */
    PASS(RMP_Thd_Crt(&Thd_1, DUMMY, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_BOTTOM, 3U));
    /* Thd-crt: tests the already-created rejection branch */
    FAIL(RMP_Thd_Crt(&Thd_1, DUMMY, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_BOTTOM, 3U));
    /* Thd-del: tests the successful deletion path */
    PASS(RMP_Thd_Del(&Thd_1));
    /* Thd-del: tests the already-deleted rejection branch */
    FAIL(RMP_Thd_Del(&Thd_1));
    /* Thd-del: tests the NULL control block rejection branch */
    FAIL(RMP_Thd_Del(RMP_NULL));
    /* Clear the pending flag set by deletion of a higher priority thread */
    RMP_Sched_Pend=0U;
    /* Sched-unlock: release the scheduler without triggering a yield */
    RMP_Sched_Unlock();

    /* Thd-crt: recreate the test thread for the parameter tests below */
    PASS(RMP_Thd_Crt(&Thd_1, DUMMY, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_BOTTOM, 10U));

    /* Thd-set: tests the NULL control block rejection branch */
    FAIL(RMP_Thd_Set(RMP_NULL, PRIO_BOTTOM, 10U));
    /* Thd-set: tests the zero-timeslice rejection branch */
    FAIL(RMP_Thd_Set(&Thd_1, PRIO_BOTTOM, 0U));
    /* Thd-set: tests the timeslice-too-large rejection branch */
    FAIL(RMP_Thd_Set(&Thd_1, PRIO_BOTTOM, RMP_SLICE_MAX+1U));
    /* Thd-set: tests the successful same-priority update path */
    PASS(RMP_Thd_Set(&Thd_1, PRIO_BOTTOM, 10U));

    /* Thd-suspend: tests the NULL control block rejection branch */
    FAIL(RMP_Thd_Suspend(RMP_NULL));
    /* Thd-suspend: tests the successful suspend path */
    PASS(RMP_Thd_Suspend(&Thd_1));
    /* Thd-suspend: tests the already-suspended rejection branch */
    FAIL(RMP_Thd_Suspend(&Thd_1));

    /* Thd-resume: tests the NULL control block rejection branch */
    FAIL(RMP_Thd_Resume(RMP_NULL));
    /* Thd-resume: tests the successful resume path */
    PASS(RMP_Thd_Resume(&Thd_1));
    /* Thd-resume: tests the not-suspended rejection branch */
    FAIL(RMP_Thd_Resume(&Thd_1));

    /* Thd-delay: tests the zero-time rejection branch */
    FAIL(RMP_Thd_Delay(0U));
    /* Thd-delay: tests the time-too-large rejection branch */
    FAIL(RMP_Thd_Delay(RMP_SLICE_MAX));
    /* Thd-delay: tests the successful self-delay path */
    PASS(RMP_Thd_Delay(10U));

    /* Thd-snd: tests the NULL control block rejection branch */
    FAIL(RMP_Thd_Snd(RMP_NULL, 1U, 10U));
    /* Thd-snd: tests the self-sending rejection branch */
    FAIL(RMP_Thd_Snd(&Thd_Cov, 1U, 10U));
    /* Thd-snd: tests the successful send path */
    PASS(RMP_Thd_Snd(&Thd_1, 1U, 10U));
    /* Thd-snd: tests the mailbox-full rejection branch */
    FAIL(RMP_Thd_Snd(&Thd_1, 1U, 0U));
    /* Thd-snd: tests the wait-timeout rejection branch */
    FAIL(RMP_Thd_Snd(&Thd_1, 1U, 10U));

    /* Thd-del: remove the test thread before the ISR-send tests */
    PASS(RMP_Thd_Del(&Thd_1));
    /* Sched-lock: simulate ISR context by locking the scheduler */
    RMP_Sched_Lock();
    /* Thd-snd-ISR: tests the NULL control block rejection branch */
    FAIL(RMP_Thd_Snd_ISR(RMP_NULL, 1U));
    /* Thd-snd-ISR: tests the deleted-thread rejection branch */
    FAIL(RMP_Thd_Snd_ISR(&Thd_1, 1));
    /* Sched-unlock: leave the simulated ISR context */
    RMP_Sched_Unlock();

    /* Thd-crt: recreate the thread with the delaying entry function */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Param_Delay_Func, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 3U));
    /* Sched-lock: simulate ISR context by locking the scheduler */
    RMP_Sched_Lock();
    /* Thd-snd-ISR: tests the successful ISR-send wakeup path */
    PASS(RMP_Thd_Snd_ISR(&Thd_1, 1U));
    /* Thd-snd-ISR: tests the mailbox-full rejection branch */
    FAIL(RMP_Thd_Snd_ISR(&Thd_1, 1));
    /* Sched-unlock: leave the simulated ISR context */
    RMP_Sched_Unlock();

    /* Thd-del: delete the thread so it can be restarted in delaying state */
    PASS(RMP_Thd_Del(&Thd_1));
    /* Thd-crt: restart the delaying test thread */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Param_Delay_Func, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 3U));

    /* Thd-set: tests the timeslice-too-large rejection on a delayed thread */
    FAIL(RMP_Thd_Set(&Thd_1, PRIO_HIGHER, RMP_SLICE_MAX+1U));
    /* Thd-set: tests the priority-out-of-range rejection on a delayed thread */
    FAIL(RMP_Thd_Set(&Thd_1, RMP_PREEMPT_PRIO_NUM+1U, 10U));
    /* Thd-set: moves a delayed thread across all priority levels */
    PASS(RMP_Thd_Set(&Thd_1, PRIO_BOTTOM, 10U));
    PASS(RMP_Thd_Set(&Thd_1, PRIO_LOWER, 10U));
    PASS(RMP_Thd_Set(&Thd_1, PRIO_COV, 10U));
    PASS(RMP_Thd_Set(&Thd_1, PRIO_HIGHER, 10U));
    PASS(RMP_Thd_Set(&Thd_1, PRIO_TOP, 10U));
    PASS(RMP_Thd_Set(&Thd_1, PRIO_HIGHER, 10U));

    /* Thd-del: delete the thread so it can be restarted in suspended state */
    PASS(RMP_Thd_Del(&Thd_1));
    /* Thd-crt: restart the self-suspending test thread */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Param_Suspend_Func, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 3U));

    /* Thd-set: tests the timeslice-too-large rejection on a suspended thread */
    FAIL(RMP_Thd_Set(&Thd_1, PRIO_HIGHER, RMP_SLICE_MAX+1U));
    /* Thd-set: tests the priority-out-of-range rejection on a suspended thread */
    FAIL(RMP_Thd_Set(&Thd_1, RMP_PREEMPT_PRIO_NUM+1U, 10U));
    /* Thd-set: moves a suspended thread across all priority levels */
    PASS(RMP_Thd_Set(&Thd_1, PRIO_BOTTOM, 10U));
    PASS(RMP_Thd_Set(&Thd_1, PRIO_LOWER, 10U));
    PASS(RMP_Thd_Set(&Thd_1, PRIO_COV, 10U));
    PASS(RMP_Thd_Set(&Thd_1, PRIO_HIGHER, 10U));
    PASS(RMP_Thd_Set(&Thd_1, PRIO_TOP, 10U));
    PASS(RMP_Thd_Set(&Thd_1, PRIO_HIGHER, 10U));

    /* Thd-del: final cleanup of the test thread */
    PASS(RMP_Thd_Del(&Thd_1));
}
/* End Function:Cov_Thd_Param ************************************************/

/* Function:Cov_Thd_Oper ******************************************************
Description : Test thread delay/wakeup.
Input       : None
Output      : None.
Return      : None
******************************************************************************/
void Cov_Thd_Oper_Func(void)
{
    /* Thd-rcv: blocks until the main thread sends, wakeup path */
    PASS(RMP_Thd_Rcv(RMP_NULL, RMP_SLICE_MAX));
    /* Thd-delay: timed delay so the main thread can run in between */
    PASS(RMP_Thd_Delay(200U));

    /* Thd-rcv: second blocking receive, woken by the next send */
    PASS(RMP_Thd_Rcv(RMP_NULL, RMP_SLICE_MAX));
    /* Thd-delay: second timed delay, woken by timeout handling */
    PASS(RMP_Thd_Delay(200U));

    /* Thd-rcv: blocking receive that stays blocked for the suspend tests */
    PASS(RMP_Thd_Rcv(RMP_NULL, RMP_SLICE_MAX));

    /* Infinite delay loop so the main thread can delete us at any state */
    while
        PASS(RMP_Thd_Delay(10000U));
}

void Cov_Thd_Oper(void)
{
    rmp_ptr_t Last;

    /* Clear the thread control block before use */
    RMP_Clear(&Thd_1, sizeof(Thd_1));

    /* Thd-crt: boot the worker thread at a higher priority than ours */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Oper_Func, DUMMY, Stack_1,sizeof(Stack_1), PRIO_HIGHER, 10U));

    /* Thd-snd: fills the mailbox of a thread blocked on receive, wakeup path */
    PASS(RMP_Thd_Snd(&Thd_1, 1U, RMP_SLICE_MAX));
    /* Thd-delay: our delay expires before Thd_1's, so we run first */
    PASS(RMP_Thd_Delay(100U));
    /* Busy-wait 200 ticks so Thd_1's delay terminates and it preempts us */
    Last=RMP_Timestamp;
    while((RMP_Timestamp-Last)<200U);

    /* Thd-suspend: suspends Thd_1 while it is blocked on receive */
    PASS(RMP_Thd_Suspend(&Thd_1));
    /* Thd-snd: sends to a suspended receiver, mailbox gets filled */
    PASS(RMP_Thd_Snd(&Thd_1, 1U, RMP_SLICE_MAX));
    /* Thd-resume: resumes Thd_1 while it is receiving, mailbox already full */
    PASS(RMP_Thd_Resume(&Thd_1));

    /* Thd-snd: fills the mailbox while Thd_1 goes into another delay */
    PASS(RMP_Thd_Snd(&Thd_1, 1U, RMP_SLICE_MAX));
    /* Thd-suspend: suspends Thd_1 while it is delaying */
    PASS(RMP_Thd_Suspend(&Thd_1));
    /* Thd-snd: timed send to a full mailbox of a suspended delayer, timeout path */
    FAIL(RMP_Thd_Snd(&Thd_1, 1U, 100U));
    /* Thd-resume: resumes Thd_1, it wakes with the timed send aborted */
    PASS(RMP_Thd_Resume(&Thd_1));

    /* Thd-snd: sends while Thd_1 is delaying; it receives after waking up */
    PASS(RMP_Thd_Snd(&Thd_1, 1U, RMP_SLICE_MAX));

    /* Thd-del: delete and recreate to reset Thd_1's state machine */
    PASS(RMP_Thd_Del(&Thd_1));
    /* Thd-crt: restart the worker thread */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Oper_Func, DUMMY, Stack_1,sizeof(Stack_1), PRIO_HIGHER, 10U));

    /* Sched-lock: simulate ISR context for the wakeup-from-ISR test */
    RMP_Sched_Lock();
    /* Thd-snd-ISR: wakes a thread blocked on receive from ISR context */
    PASS(RMP_Thd_Snd_ISR(&Thd_1, 1U));
    /* Sched-unlock: leave the simulated ISR context */
    RMP_Sched_Unlock();

    /* Sched-lock: simulate ISR context for the delaying-receiver test */
    RMP_Sched_Lock();
    /* Thd-snd-ISR: sends to a thread that is delaying, no wakeup path */
    PASS(RMP_Thd_Snd_ISR(&Thd_1, 1U));
    /* Sched-unlock: leave the simulated ISR context */
    RMP_Sched_Unlock();

    /* Sched-lock: simulate ISR context for the full-mailbox test */
    RMP_Sched_Lock();
    /* Thd-snd-ISR: tests the mailbox-full rejection branch from ISR context */
    FAIL(RMP_Thd_Snd_ISR(&Thd_1, 1U));
    /* Sched-unlock: leave the simulated ISR context */
    RMP_Sched_Unlock();

    /* Thd-del: delete and recreate at a lower priority than ours */
    RMP_Thd_Del(&Thd_1);
    /* Thd-crt: restart the worker thread, now lower priority */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Oper_Func, DUMMY, Stack_1,sizeof(Stack_1), PRIO_LOWER, 10U));

    /* Sched-lock: simulate ISR context for the ready-thread test */
    RMP_Sched_Lock();
    /* Thd-snd-ISR: sends to a ready thread that cannot preempt us */
    PASS(RMP_Thd_Snd_ISR(&Thd_1, 1U));
    /* Sched-unlock: leave the simulated ISR context */
    RMP_Sched_Unlock();

    /* Thd-delay: delay while Thd_1 is ready but cannot preempt us */
    RMP_Thd_Delay;

    /* Sched-lock: simulate ISR context for the second full-mailbox test */
    RMP_Sched_Lock();
    /* Thd-snd-ISR: mailbox is full again, rejection path with a ready receiver */
    FAIL(RMP_Thd_Snd_ISR(&Thd_1, 1U));
    /* Sched-unlock: leave the simulated ISR context */
    RMP_Sched_Unlock();

    /* Thd-del: final cleanup of the worker thread */
    PASS(RMP_Thd_Del(&Thd_1));
}
/* End Function:Cov_Thd_Oper *************************************************/

/* Function:Cov_Thd_Del *******************************************************
Description : Test thread delete under various circumstances.
Input       : None
Output      : None.
Return      : None
******************************************************************************/
void Cov_Thd_Del_Delay_Func(void)
{
    /* Thd-delay: puts the thread into the delaying state */
    RMP_Thd_Delay(10000U);
    while;
}

void Cov_Thd_Del_Suspend_Func(void)
{
    /* Thd-suspend: puts the thread into the suspended state */
    RMP_Thd_Suspend(&Thd_1);
    while;
}

void Cov_Thd_Del_Snd_Func(void)
{
    /* Thd-snd: first send lands in an empty mailbox of the main thread */
    RMP_Thd_Snd(&Thd_Cov, 1U, RMP_SLICE_MAX);
    /* Thd-snd: second send blocks infinitely on a full mailbox */
    RMP_Thd_Snd(&Thd_Cov, 1U, RMP_SLICE_MAX);
    while;
}

void Cov_Thd_Del_Snd_Delay_Func(void)
{
    /* Thd-snd: first send lands in an empty mailbox of the main thread */
    RMP_Thd_Snd(&Thd_Cov, 1U, RMP_SLICE_MAX);
    /* Thd-snd: second send blocks with a timeout on a full mailbox */
    RMP_Thd_Snd(&Thd_Cov, 1U, RMP_SLICE_MAX);
    while;
}

void Cov_Thd_Del_Rcv_Func(void)
{
    rmp_ptr_t Data;

    /* Thd-rcv: blocks infinitely waiting for a message */
    RMP_Thd_Rcv(&Data, RMP_SLICE_MAX);
    while;
}

void Cov_Thd_Del_Rcv_Delay_Func(void)
{
    rmp_ptr_t Data;

    /* Thd-rcv: blocks with a timeout waiting for a message */
    RMP_Thd_Rcv(&Data, 10000U);
    while;
}

void Cov_Thd_Del_Rcv_Sender_Func(void)
{
    /* Thd-snd: first send lands in an empty mailbox of Thd_2 */
    RMP_Thd_Snd(&Thd_2, 1, RMP_SLICE_MAX);
    /* Thd-snd: second send blocks infinitely on Thd_2's full mailbox */
    RMP_Thd_Snd(&Thd_2, 1, RMP_SLICE_MAX);
    /* Thd-delay: park the thread after the blocking sends are set up */
    RMP_Thd_Delay(10000U);
    while;
}

void Cov_Thd_Del_Rcv_Sender_Delay_Func(void)
{
    /* Thd-snd: first send lands in an empty mailbox of Thd_2 */
    RMP_Thd_Snd(&Thd_2, 1, RMP_SLICE_MAX);
    /* Thd-snd: second send blocks with a timeout on Thd_2's full mailbox */
    RMP_Thd_Snd(&Thd_2, 1, 10000U);
    /* Thd-delay: park the thread after the blocking sends are set up */
    RMP_Thd_Delay(10000U);
    while;
}

void Cov_Thd_Del_Rcv_Receiver_Func(void)
{
    /* Thd-delay: keep Thd_2 delayed so its mailbox stays full */
    RMP_Thd_Delay(10000U);
    while;
}

void Cov_Thd_Del_Pend_Func(void)
{
    rmp_ptr_t Data;

    /* Sem-pend: blocks infinitely on the counting semaphore */
    RMP_Sem_Pend(&Sem_1, RMP_SLICE_MAX);
    while;
}

void Cov_Thd_Del_Pend_Delay_Func(void)
{
    rmp_ptr_t Data;

    /* Sem-pend: blocks with a timeout on the counting semaphore */
    RMP_Sem_Pend(&Sem_1, 10000);
    while;
}

void Cov_Thd_Del(void)
{
    /* Thd-crt: worker that delays itself; deletion hits the DELAY state branch */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Del_Delay_Func, DUMMY, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 5U));
    /* Thd-del: deletes a thread from the delay queue */
    PASS(RMP_Thd_Del(&Thd_1));

    /* Thd-crt: worker that suspends itself; deletion hits the suspended branch */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Del_Suspend_Func, DUMMY, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 5U));
    /* Thd-del: deletes a thread that is suspended and ready */
    PASS(RMP_Thd_Del(&Thd_1));

    /* Thd-crt: worker that blocks sending; deletion hits the SNDBLK state branch */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Del_Snd_Func, DUMMY, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 5U));
    /* Thd-del: deletes a thread blocked on an infinite send */
    PASS(RMP_Thd_Del(&Thd_1));
    /* Thd-rcv: drains the message the deleted thread left in our mailbox */
    PASS(RMP_Thd_Rcv(RMP_NULL, RMP_SLICE_MAX));

    /* Thd-crt: worker that blocks sending with timeout; hits the SNDDLY branch */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Del_Snd_Delay_Func, DUMMY, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 5U));
    /* Thd-del: deletes a thread blocked on a timed send */
    PASS(RMP_Thd_Del(&Thd_1));
    /* Thd-rcv: drains the message the deleted thread left in our mailbox */
    PASS(RMP_Thd_Rcv(RMP_NULL, RMP_SLICE_MAX));

    /* Thd-crt: worker that blocks receiving; deletion hits the RCVBLK branch */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Del_Rcv_Func, DUMMY, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 5U));
    /* Thd-del: deletes a thread blocked on an infinite receive */
    PASS(RMP_Thd_Del(&Thd_1));

    /* Thd-crt: worker that blocks receiving with timeout; hits the RCVDLY branch */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Del_Rcv_Delay_Func, DUMMY, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 5U));
    /* Thd-del: deletes a thread blocked on a timed receive */
    PASS(RMP_Thd_Del(&Thd_1));

    /* Thd-crt: receiver whose full mailbox makes Thd_1 block sending to it */
    PASS(RMP_Thd_Crt(&Thd_2, Cov_Thd_Del_Rcv_Receiver_Func, DUMMY, Stack_2, sizeof(Stack_2), PRIO_TOP, 5U));
    /* Thd-crt: sender that blocks infinitely on Thd_2's full mailbox */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Del_Rcv_Sender_Func, DUMMY, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 5U));
    /* Thd-del: deleting the receiver must abort the blocked sender too */
    PASS(RMP_Thd_Del(&Thd_2));
    /* Thd-del: delete the aborted sender */
    PASS(RMP_Thd_Del(&Thd_1));

    /* Thd-crt: receiver whose full mailbox makes Thd_1 block sending to it */
    PASS(RMP_Thd_Crt(&Thd_2, Cov_Thd_Del_Rcv_Receiver_Func, DUMMY, Stack_2, sizeof(Stack_2), PRIO_TOP, 5U));
    /* Thd-crt: sender that blocks with a timeout on Thd_2's full mailbox */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Del_Rcv_Sender_Delay_Func, DUMMY, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 5U));
    /* Thd-del: deleting the receiver must abort the timed sender too */
    PASS(RMP_Thd_Del(&Thd_2));
    /* Thd-del: delete the aborted sender */
    PASS(RMP_Thd_Del(&Thd_1));

    /* Sem-crt: create a counting semaphore for the pending-state tests */
    PASS(RMP_Sem_Crt(&Sem_1, 0U));

    /* Thd-crt: worker that pends infinitely; deletion hits the SEMBLK branch */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Del_Pend_Func, DUMMY, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 5U));
    /* Thd-del: deletes a thread blocked on a semaphore */
    PASS(RMP_Thd_Del(&Thd_1));

    /* Thd-crt: worker that pends with timeout; deletion hits the SEMDLY branch */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Del_Pend_Delay_Func, DUMMY, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 5U));
    /* Thd-del: deletes a thread blocked on a semaphore with timeout */
    PASS(RMP_Thd_Del(&Thd_1));

    /* Sem-del: final cleanup of the test semaphore */
    PASS(RMP_Sem_Del(&Sem_1));
}
/* End Function:Cov_Thd_Del **************************************************/

/* Function:Cov_Sem_Param *****************************************************
Description : Semaphore simple parameter tests.
Input       : None
Output      : None.
Return      : None
******************************************************************************/
void Cov_Sem_Param(void)
{
    /* Clear the semaphore control block before use */
    RMP_Clear(&Sem_1, sizeof(Sem_1));

    /* Sem-crt: tests the NULL control block rejection branch */
    FAIL(RMP_Sem_Crt(RMP_NULL, 0U));
    /* Sem-crt: tests the initial-count-too-large rejection branch */
    FAIL(RMP_Sem_Crt(&Sem_1, RMP_SEM_CNT_MAX));
    /* Sem-crt: tests the successful creation path */
    PASS(RMP_Sem_Crt(&Sem_1, 0U));
    /* Sem-crt: tests the already-created rejection branch */
    FAIL(RMP_Sem_Crt(&Sem_1, 0U));

    /* Sem-del: tests the NULL control block rejection branch */
    FAIL(RMP_Sem_Del(RMP_NULL));
    /* Sem-del: tests the successful deletion path */
    PASS(RMP_Sem_Del(&Sem_1));
    /* Sem-del: tests the already-deleted rejection branch */
    FAIL(RMP_Sem_Del(&Sem_1));

    /* Sem-pend: tests the NULL control block rejection branch */
    FAIL(RMP_Sem_Pend(RMP_NULL, 10U));
    /* Sem-pend: tests the timed-wait timeout path on an empty semaphore */
    FAIL(RMP_Sem_Pend(&Sem_1, 10U));
    /* Sem-pend: tests the zero-slice non-blocking pend path */
    FAIL(RMP_Sem_Pend(&Sem_1, 0U));

    /* Sem-abort: tests the NULL control block rejection branch */
    FAIL(RMP_Sem_Abort(RMP_NULL));
    /* Sem-abort: tests the not-pending rejection branch */
    FAIL(RMP_Sem_Abort(&Thd_1));

    /* Sem-post: tests the NULL control block rejection branch */
    FAIL(RMP_Sem_Post(RMP_NULL, 10U));
    /* Sem-post: tests the zero-count rejection branch */
    FAIL(RMP_Sem_Post(&Sem_1, 0U));
    /* Sem-post: tests the successful post path */
    PASS(RMP_Sem_Post(&Sem_1, 10U));
    /* Sem-pend: tests the immediate zero-slice pend success path */
    PASS(RMP_Sem_Pend(&Sem_1, 0U));
    /* Sem-pend: tests the immediate timed pend success path */
    PASS(RMP_Sem_Pend(&Sem_1, 10U));
    /* Sem-pend: tests the immediate infinite pend success path */
    PASS(RMP_Sem_Pend(&Sem_1, RMP_SLICE_MAX));
    /* Sem-bcst: tests the broadcast wakeup path */
    PASS(RMP_Sem_Bcst(&Sem_1));
    /* Sem-pend-unlock: tests the scheduler-not-locked rejection branch */
    FAIL(RMP_Sem_Pend_Unlock(&Sem_1, RMP_SLICE_MAX));
    /* Sched-lock: provide the locked scheduler the pend-unlock call requires */
    RMP_Sched_Lock();
    /* Sem-pend-unlock: tests the successful locked pend path */
    PASS(RMP_Sem_Pend_Unlock(&Sem_1, 10U));

    /* Sem-post: tests the count-overflow rejection branch */
    FAIL(RMP_Sem_Post(&Sem_1, RMP_SEM_CNT_MAX));
    /* Sem-post: fills the semaphore close to its capacity */
    PASS(RMP_Sem_Post(&Sem_1, RMP_SEM_CNT_MAX*2U/3U));
    /* Sem-post: posting again now exceeds the capacity, rejection branch */
    FAIL(RMP_Sem_Post(&Sem_1, RMP_SEM_CNT_MAX*2U/3U));

    /* Sem-del: final cleanup of the test semaphore */
    PASS(RMP_Sem_Del(&Sem_1));
}
/* End Function:Cov_Sem_Param ************************************************/

/* Function:Test_Set_2 ********************************************************
Description : Helper thread for the semaphore pend and timing tests. The
              thread deletes itself after the pend returns.
Input       : rmp_ptr_t Param - The test case selector.
Output      : None.
Return      : None.
******************************************************************************/
void Test_Set_2(rmp_ptr_t Param)
{
    volatile rmp_ptr_t Middle;

    /* Dispatch on the test case selector passed by the creator */
    switch(Param)
    {
        case 0:
        {
            /* Thd-delay: park the thread so it stays in the delaying state */
            RMP_Thd_Delay;
            while;
        }
        case 2:
        {
            /* Sem-pend: blocks infinitely; the thread is deleted while pending */
            RMP_Sem_Pend(&Sem_1, RMP_SLICE_MAX);
            /* Thd-del: self-delete after the pend returns (aborted or woken) */
            RMP_Thd_Del(&Thd_1);
            while;
        }
        case 3:
        {
            /* Sem-pend: blocks with a timeout; the thread is deleted while waiting */
            RMP_Sem_Pend(&Sem_1, 10000);
            /* Thd-del: self-delete after the pend returns (aborted or woken) */
            RMP_Thd_Del(&Thd_1);
            while;
        }
    }
}

/* Function:Cov_Sem_Oper ******************************************************
Description : Test the thread and semaphore operations, including semaphore
              deletion while someone is waiting on it, pend aborts, and
              semaphore posts from ISR context.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Sem_Oper(void)
{
    rmp_ptr_t Data;
    rmp_ptr_t Temp;
    rmp_ptr_t Stub;
    rmp_ret_t Sched_Pend_Bkp;
    rmp_ret_t Lock_Cnt_Bkp;

    /* Thd-crt: worker that pends infinitely; used for deletion-while-pending */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, (void*)2, Stack_1, sizeof(Stack_1), 3, 5);
    /* Sem-del: deletes the semaphore while a thread pends on it, aborting the pend */
    RMP_Sem_Del(&Sem_1);
    /* Sem-crt: recreate the semaphore for the next round */
    RMP_Sem_Crt(&Sem_1, 0);
    /* Thd-crt: worker that pends with a timeout; deletion hits the timed path */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, (void*)3, Stack_1, sizeof(Stack_1), 3, 5);
    /* Sem-del: deletes the semaphore while a timed pend is waiting on it */
    RMP_Sem_Del(&Sem_1);
    /* Sem-crt: recreate the semaphore for the timed-pend success round */
    RMP_Sem_Crt(&Sem_1, 0);
    /* Thd-crt: worker that pends with a timeout again */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, (void*)3, Stack_1, sizeof(Stack_1), 3, 5);
    /* Sem-post: wakes the timed pend so it completes successfully this time */
    RMP_Sem_Post(&Sem_1, 1);

    /* Sem-post: put one count on the semaphore for the immediate pends below */
    RMP_Sem_Post(&Sem_1, 1);
    /* Sem-pend: timed pend that returns immediately because a count exists */
    RMP_Sem_Pend(&Sem_1, 100);
    /* Sem-pend: zero-slice pend that returns immediately */
    RMP_Sem_Pend(&Sem_1, 0);

    /* Sem-pend-abort: boot a worker that will block on the semaphore */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, (void*)2, Stack_1, sizeof(Stack_1), 3, 5);
    /* Sem-abort: aborts the wait while the thread is blocked on the semaphore */
    RMP_Sem_Abort(&Thd_1);
    /* Thd-del: clean up the aborted worker */
    RMP_Thd_Del(&Thd_1);
    /* Thd-crt: boot a worker that pends with a timeout */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, (void*)3, Stack_1, sizeof(Stack_1), 3, 5);
    /* Sem-abort: aborts the wait while the thread is delaying on the semaphore */
    RMP_Sem_Abort(&Thd_1);
    /* Thd-del: clean up the aborted worker */
    RMP_Thd_Del(&Thd_1);

    /* Sem-post-ISR: count too large, rejection branch from ISR context */
    RMP_Sem_Post_ISR(&Sem_1, RMP_SEM_CNT_MAX);
    /* Sem-del: drop the semaphore before the ISR rejection tests */
    RMP_Sem_Del(&Sem_1);
    /* Sched-lock: simulate ISR context for the ISR post tests */
    RMP_Sched_Lock();
    /* Sem-post-ISR: tests the NULL control block rejection branch */
    RMP_Sem_Post_ISR(0, 1);
    /* Sem-post-ISR: posts to a deleted semaphore, not-in-use rejection branch */
    RMP_Sem_Post_ISR(&Sem_1, 2);
    /* Sched-unlock: leave the simulated ISR context */
    RMP_Sched_Unlock();

    /* Thd-crt: worker blocked on the semaphore for the ISR wakeup test */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, (void*)2, Stack_1, sizeof(Stack_1), 3, 5);
    /* Sched-lock: simulate ISR context; post wakes the blocked thread */
    RMP_Sched_Lock();
    /* Sem-post-ISR: successful post that unblocks the pending thread */
    RMP_Sem_Post_ISR(&Sem_1, 1);
    /* Sched-unlock: leave the simulated ISR context */
    RMP_Sched_Unlock();
    /* Thd-del: clean up the woken worker */
    RMP_Thd_Del(&Thd_1);

    /* Thd-crt: worker pending with a timeout for the ISR wakeup test */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, (void*)3, Stack_1, sizeof(Stack_1), 3, 5);
    /* Sched-lock: simulate ISR context; post wakes the delaying thread */
    RMP_Sched_Lock();
    /* Sem-post-ISR: successful post that wakes the timed pending thread */
    RMP_Sem_Post_ISR(&Sem_1, 1);
    /* Sched-unlock: leave the simulated ISR context */
    RMP_Sched_Unlock();
    /* Thd-del: clean up the woken worker */
    RMP_Thd_Del(&Thd_1);

    /* Thd-crt: worker that delays instead of pending (Test_Set_1, case 2) */
    RMP_Thd_Crt(&Thd_1, Test_Set_1, (void*)2, Stack_1, sizeof(Stack_1), 3, 5);
    /* Sched-lock: simulate ISR context; the target is not pending */
    RMP_Sched_Lock();
    /* Sem-post-ISR: successful post when the thread is not pending */
    RMP_Sem_Post_ISR(&Sem_1, 1);
    /* Sched-unlock: leave the simulated ISR context */
    RMP_Sched_Unlock();
    /* Thd-del: clean up the worker */
    RMP_Thd_Del(&Thd_1);
    /* Sem-del: drop the semaphore before the low-priority test */
    RMP_Sem_Del(&Sem_1);

    /* Sem-crt: recreate the semaphore for the low-priority wakeup test */
    RMP_Sem_Crt(&Sem_1, 0);
    /* Thd-crt: worker at a lower priority than the test thread */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, (void*)2, Stack_1, sizeof(Stack_1), 1, 5);
    /* Thd-delay: let the worker block on the semaphore first */
    RMP_Thd_Delay;
    /* Sched-lock: simulate ISR context; the woken thread cannot preempt us */
    RMP_Sched_Lock();
    /* Sem-post-ISR: success path where the woken thread has lower priority */
    RMP_Sem_Post_ISR(&Sem_1, 1);
    /* Sched-unlock: leave the simulated ISR context */
    RMP_Sched_Unlock();
    /* Thd-del: clean up the worker */
    RMP_Thd_Del(&Thd_1);

    /* Thd-del: clean up the last worker before the raw-state tests */
    RMP_Thd_Del(&Thd_1);
    /* Sem-bcst-ISR: tests the NULL control block rejection branch */
    RMP_Sem_Bcst_ISR(RMP_NULL);
    /* Sem-bcst-ISR: broadcast on a free semaphore, not-in-use branches */
    RMP_Sem_Bcst_ISR(&Sem_1);
    /* Sem-bcst-ISR: force USED state, empty waitlist branches */
    Sem_1.State=RMP_SEM_USED;
    RMP_Sem_Bcst_ISR(&Sem_1);
    Sem_1.State=RMP_SEM_FREE;
    RMP_Sem_Bcst_ISR(&Sem_1);

    /* Sem-bcst-ISR: force a pending context switch flag, yield branch */
    Sched_Pend_Bkp=RMP_Sched_Pend;
    RMP_Sched_Pend=1;
    Sem_1.State=RMP_SEM_USED;
    RMP_Sem_Bcst_ISR(&Sem_1);
    RMP_Sched_Pend=Sched_Pend_Bkp;

    /* Sem-pend-unlock: tests the NULL control block rejection with a nested
     * lock count forced to 2 so the unlock path is not taken */
    Lock_Cnt_Bkp=RMP_Sched_Lock_Cnt;
    RMP_Sched_Lock_Cnt=2;
    RMP_Sem_Pend_Unlock(RMP_NULL,1);
    RMP_Sched_Lock_Cnt=Lock_Cnt_Bkp;
    /* Sem-pend-unlock: lock count 2, pend succeeds but lock stays held */
    Lock_Cnt_Bkp=RMP_Sched_Lock_Cnt;
    RMP_Sched_Lock_Cnt=2;
    RMP_Sem_Pend_Unlock(&Sem_1,1);
    RMP_Sched_Lock_Cnt=Lock_Cnt_Bkp;

    /* Sem-pend-unlock: lock count 1, unlock drops it to zero and yields */
    Lock_Cnt_Bkp=RMP_Sched_Lock_Cnt;
    RMP_Sched_Lock_Cnt=1;
    RMP_Sem_Pend_Unlock(&Sem_1,1);
    RMP_Sched_Lock_Cnt=Lock_Cnt_Bkp;

    /* Sem-post-ISR: special case where the post count is zero */
    RMP_Sem_Post_ISR(&Sem_1,0);

    /* Sem-post-ISR: force a pending context switch flag, yield branch */
    RMP_Sched_Pend=Lock_Cnt_Bkp;
    RMP_Sched_Pend=1;
    RMP_Sem_Post_ISR(&Sem_1,10);
    RMP_Sched_Pend=Lock_Cnt_Bkp;
    /* Sem-bcst: tests the NULL control block rejection branch */
    RMP_Sem_Bcst(RMP_NULL);

    /* Sem-bcst: broadcast success with nobody waiting, branches */
    RMP_Sem_Bcst(&Sem_1);

    /* Sem-bcst: free-state broadcast, not-in-use rejection branch */
    Lock_Cnt_Bkp=Sem_1.State;
    Sem_1.State=RMP_SEM_FREE;
    RMP_Sem_Bcst(&Sem_1);
    Sem_1.State=Lock_Cnt_Bkp;

    /* Sem-cnt: tests the NULL control block rejection branch */
    RMP_Sem_Cnt(RMP_NULL);
    /* Sem-cnt: queries a used semaphore, success path */
    RMP_Sem_Cnt(&Sem_1);

    /* Sem-cnt: queries a free semaphore, not-in-use rejection branch */
    Lock_Cnt_Bkp=Sem_1.State;
    Sem_1.State=RMP_SEM_FREE;
    RMP_Sem_Cnt(&Sem_1);
    Sem_1.State=Lock_Cnt_Bkp;
}
/* End Function:Cov_Sem_Oper *************************************************/

/* Function:Cov_Mem ***********************************************************
Description : Test the memory pool operations, including initialization,
              allocation, free and reallocation, covering the underlying
              FLI/SLI bitmap and linked list paths.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Mem(void)
{
    volatile rmp_ptr_t Data;
    volatile rmp_ptr_t Temp;
    volatile rmp_ptr_t Stub;
    volatile rmp_ret_t iii=0;

    /* In the RMP_Mem structure, Table is a pointer to an RMP_List array that
       serves as the allocation table of the memory pool. Its purpose is to keep
       track of the allocation state, i.e. the free block lists of every size
       class (defined by the First-Level Index (FLI) and the Second-Level Index
       (SLI)). Each list head represents a set of free blocks within a certain
       size range that are available for allocation. Specifically, every element
       of the Table array is an RMP_List structure acting as the head node of a
       doubly-linked list, and each head corresponds to one FLI/SLI combination,
       representing a group of free blocks of similar size. */

    /* When freeing a block whose both neighbors are occupied, the block
       is inserted via _RMP_Mem_Ins with Size = 64, and 64 maps to FLI = SLI = 0
       (a C++ helper was written to visualize FLI/SLI directly instead of using
       the debugger). Hence it suffices to free two 64-byte blocks in a row that
       cannot be merged. */
    for(iii=0;iii<1024;iii++)
    {
        Pool[iii]=0U;
    }
    RMP_Mem_Init(Pool,1024U*sizeof(rmp_ptr_t));
    void* p4=RMP_Malloc(Pool, 64);
    void* p5=RMP_Malloc(Pool, 64);
    void* p6=RMP_Malloc(Pool, 64);
    void* p7=RMP_Malloc(Pool, 64);
    void* p8=RMP_Malloc(Pool, 64);
    void* p9=RMP_Malloc(Pool, 64);
    RMP_Free(Pool,p5);
    RMP_Free(Pool,p8);

    /* Similar to the case above, but the two freed 64-byte blocks are
       adjacent and thus mergeable, which ensures Slot != Slot->Next */
    void* t4=RMP_Malloc(Pool, 64);
    void* t5=RMP_Malloc(Pool, 64);
    void* t6=RMP_Malloc(Pool, 64);
    void* t7=RMP_Malloc(Pool, 64);
    void* t8=RMP_Malloc(Pool, 64);
    void* t9=RMP_Malloc(Pool, 64);
    RMP_Free(Pool,t5);
    RMP_Free(Pool,t6);

    for(iii=0;iii<1024;iii++)
    {
        Pool[iii]=0U;
    }
    RMP_Mem_Init(Pool,1024U*4);
    void* p1=RMP_Malloc(Pool, 256);
    void* p2=RMP_Malloc(Pool, 128);
    void* p3=RMP_Malloc(Pool, 64);
    RMP_Free(Pool,p2);
    RMP_Malloc(Pool, 256);
    RMP_Realloc(Pool, p1,512);

    /* Malloc: size too large for the pool, rejection path */
    RMP_Malloc(Pool, TEST_MEM_POOL*sizeof(rmp_ptr_t)*200);
    /* Free: NULL pointer rejection branch */
    RMP_Free(Pool,RMP_NULL);
    /* Free: pointer out of pool range rejection branch */
    RMP_Free(Pool,(void*)1);
    /* Free: NULL pool rejection branch */
    RMP_Free(RMP_NULL,(void*)100);
    /* Malloc: NULL pool rejection branch */
    RMP_Malloc(RMP_NULL,100);
    /* Malloc: zero size rejection branch */
    RMP_Malloc(Pool,0);
    /* Malloc: ordinary allocation to set up the double-free test */
    Data=(rmp_ptr_t)RMP_Malloc(Pool, 256);
    /* Free: normal free of the block just allocated */
    RMP_Free(Pool,(void*)Data);
    /* Free: double free of the same block, rejection branch */
    RMP_Free(Pool,(void*)Data);
    /* Mem-init: NULL pool rejection branch */
    RMP_Mem_Init(RMP_NULL, TEST_MEM_POOL*sizeof(rmp_ptr_t));
    /* Mem-init: zero size rejection branch */
    RMP_Mem_Init(Pool, 0);
    /* Mem-init: unaligned memory rejection branch */
    RMP_Mem_Init((void*)(((rmp_ptr_t)Pool)+1), TEST_MEM_POOL*sizeof(rmp_ptr_t));

    /* Realloc: NULL pointer behaves like a fresh allocation */
    RMP_Realloc(Pool,RMP_NULL,128);
    /* Realloc: NULL pool rejection branch */
    RMP_Realloc(0, (void*)0, 100);
    /* Realloc: pointer out of range rejection branch */
    RMP_Realloc(Pool, (void*)1, 100);
    /* Realloc: shrink with right side allocated, residual stub below 64 bytes */
    RMP_Realloc(Pool, (void*)Data, TEST_MEM_POOL*sizeof(rmp_ptr_t)/4*3+Temp-64);
    /* Realloc: zero size is equivalent to a free */
    RMP_Realloc(Pool, (void*)Data, 0);
    /* Realloc: operating on a block that was already freed */
    RMP_Realloc(Pool, (void*)Data, 100);
    /* Realloc: expand by moving the content to a new, larger block */
    Stub=(rmp_ptr_t)RMP_Realloc(Pool, (void*)Stub, TEST_MEM_POOL*sizeof(rmp_ptr_t)/2);
    /* Free: release the stub block we just reallocated */
    RMP_Free(Pool, (void*)Stub);

    /* Malloc: request nearly the whole pool, found on the first FLI try */
    Data=(rmp_ptr_t)RMP_Malloc(Pool, TEST_MEM_POOL*sizeof(rmp_ptr_t)/5*4);

    /* Test residue smaller than 64 - keep allocating 64 byte until we run out
       of memory. Memory leaking is fine because this is just a test. */
    while
    {
        Data=(rmp_ptr_t)RMP_Malloc(Pool, 64);
        if(Data==0)
            break;
        /* Last good allocation stored here */
        Temp=Data;
    }
    /* Realloc: no space left in the pool, the move fails */
    RMP_Realloc(Pool,(void*)Temp,256);

    /* Mem-init: aligned memory, the normal path */
    RMP_Mem_Init((void*)((rmp_ptr_t)Pool), TEST_MEM_POOL*sizeof(rmp_ptr_t));
    /* The else branch is taken because the current FLI/SLI slot already
       contains other blocks; the new block is simply added to the list without
       touching the bitmap state. Memory allocation itself does not lock the
       scheduler. */
    /* Malloc: ordinary allocations in various size classes */
    void* block1=RMP_Malloc(Pool, 100);
    void* block2=RMP_Malloc(Pool, 200);
    void* block3=RMP_Malloc(Pool, 200);
    void* block4=RMP_Malloc(Pool, 200);
    void* block5=RMP_Malloc(Pool, 8000);
    void* block6=RMP_Malloc(Pool, 6000);
    void* block8=RMP_Malloc(Pool, 8000);
    void* block9=RMP_Malloc(Pool, 200);

    /* Minimum block allocation test */
    void* ptr1=RMP_Malloc(Pool, 65);

    /* Exact FLI/SLI match test */
    void* ptr2=RMP_Malloc(Pool, 512);

    /* Boundary condition test */
    void* ptr3=RMP_Malloc(Pool, 511);
    void* ptr4=RMP_Malloc(Pool, 513);

    /* Large block allocation test - the pool cannot support such a request */
    void* ptr5=RMP_Malloc(Pool, TEST_MEM_POOL*100);

    /* Combined test - free a few blocks first */
    RMP_Free(Pool, ptr1);
    RMP_Free(Pool, ptr2);

    /* Then allocate different sizes and observe the pool behavior */
    void* ptr6=RMP_Malloc(Pool, 128);
    void* ptr7=RMP_Malloc(Pool, 256);

    /* Make sure that during the memory search at least one iteration
       sees Mem->Bitmap[Word] == 0, i.e. no block is available at the current
       processor word level. This requires a scenario where some FLI level has
       no available blocks, and the search algorithm must skip this empty FLI
       level and continue with higher FLI levels. */
    while(RMP_Malloc(Pool, 256)!=NULL);

    /* Now attempt an allocation large enough that the algorithm must cross FLI levels */
    /* Malloc: size forces the search past the first FLI level */
    void* ptr=RMP_Malloc(Pool, TEST_MEM_POOL*2);

    /* Free the blocks allocated via RMP_Malloc */
    RMP_Free(Pool, block1);
    RMP_Free(Pool, block2);
    RMP_Free(Pool, block3);
    RMP_Free(Pool, block4);
    RMP_Free(Pool, block5);
    RMP_Free(Pool, block6);
    RMP_Free(Pool, block8);
    RMP_Free(Pool, block9);

    /* Free the minimum block allocation test */
    RMP_Free(Pool, ptr1);

    /* Free the exact FLI/SLI match test */
    RMP_Free(Pool, ptr2);

    /* Free the boundary condition test */
    RMP_Free(Pool, ptr3);
    RMP_Free(Pool, ptr4);

    /* Large block allocation test */
    /* ptr5 may not have been allocated since the pool cannot support such a
       large request, so check it is not NULL before freeing */
    if(ptr5!=NULL)
    {
        RMP_Free(Pool, ptr5);
    }

    /* Free the combined test */
    RMP_Free(Pool, ptr6);
    RMP_Free(Pool, ptr7);

    /* Free the FLI-crossing allocation test */
    if(ptr!=NULL)
    {
        RMP_Free(Pool, ptr);
    }

    /* To cover the last else branch of _RMP_Mem_Del, we need a scenario where,
       when _RMP_Mem_Del removes a block, other blocks still exist in its FLI/SLI
       class. After the deletion the corresponding FLI/SLI class is still
       non-empty, so the Slot == Slot->Next check evaluates to false and the
       else branch is taken. */
    /* Malloc: two blocks of the same size, they share one FLI/SLI class */
    void* mem1=RMP_Malloc(Pool, 128);
    void* mem2=RMP_Malloc(Pool, 128);

    /* Free: both blocks reside in the same FLI/SLI class */
    RMP_Free(Pool, mem1);

    /* This should trigger the else branch of _RMP_Mem_Del, since the second
       block still resides in the same FLI/SLI class */

    /* Cleanup */
    /* Free: the second block empties the FLI/SLI class */
    RMP_Free(Pool, mem2);

    /* Realloc: freed-and-recreated out-of-range pointer, residual paths */
    RMP_Realloc(Pool,(void*)1,0);
    /* Malloc/Realloc: shrink a block in place, covering the shrink paths */
    void* ptr_re=RMP_Malloc(Pool,16);
    RMP_Realloc(Pool,ptr_re,32);

    /* Malloc: allocate a block and free it for the freed-realloc test */
    void* mem_block=RMP_Malloc(Pool, 128);
    /* Free: return the block to the pool */
    RMP_Free(Pool, mem_block);
    /* Realloc: try to reallocate an already freed block */
    void* realloc_block=RMP_Realloc(Pool, mem_block, 256);
    /* Free: release the reallocated block */
    RMP_Free(Pool,realloc_block);

    /* If Right_Head's address differs from the end of the pool, there are more
       blocks after the current one and the block pointed to by Right_Head
       validly exists. Make sure the reallocated block is not the last block
       in the pool. */
    /* RMP_Mem_Init(Pool, 1024); */
    /* Malloc: three blocks so the reallocated one is not the last */
    void* b1=RMP_Malloc(Pool, 100);
    void* b2=RMP_Malloc(Pool, 200);
    void* b3=RMP_Malloc(Pool, 300);
    /* Free a block and immediately try to reallocate, to trigger the else branch */
    /* Free: the first block, so the second can grow leftwards */
    RMP_Free(Pool, b1);
    /* Realloc: grow the second block into the freed space left of it */
    b2=RMP_Realloc(Pool, b2, 250);
    /* Cleanup - free all allocated blocks and destroy the pool */
    /* Free: the first block (may already be merged if the realloc failed) */
    RMP_Free(Pool, b1);
    /* Free: the resized second block */
    RMP_Free(Pool, b2);
    /* Free: the third block */
    RMP_Free(Pool, b3);

    /* Expansion cases */
    /* Allocate the first block, which is about to be resized */
    void* firstBlock=RMP_Malloc(Pool, 128);
    /* Allocate the second block right after, then free it immediately so it
       becomes the right free block */
    void* secondBlock=RMP_Malloc(Pool, 128);
    /* Free: the second block becomes the right free neighbor */
    RMP_Free(Pool, secondBlock);

    /* Try to expand the first block, expecting the right free block handling
       logic to trigger */
    /* Realloc: expand the first block by merging with its right free neighbor */
    void* resizedFirstBlock=RMP_Realloc(Pool, firstBlock, 256);
    /* Cleanup - free the block and destroy the pool */
    /* Free: the expanded block */
    RMP_Free(Pool, resizedFirstBlock);

    /*
       When attempting to expand a block that has a right adjacent free block
       (Right_Head), the residual space (Res_Size) after merging the two blocks
       is too small to form a valid new block. That is, although a free block is
       available for merging on the right, the merged space is still insufficient
       for Rounded_Size, and the residual space is also insufficient to create a
       new block (less than sizeof(struct RMP_Mem_Head)+64U+sizeof(struct RMP_Mem_Tail), i.e. 84).
    */
    /* Logically this should be fine, but Res_Size always came out surprisingly
       large - solved: the entire right side was free. */
    /* RMP_Mem_Init(Pool, 256); */
    /* Allocate the first block from the pool; we will try to expand it later */
    void* initialBlock=RMP_Malloc(Pool,64);

    /* Allocate and free another block right after, to create a free block to
       the right of initialBlock */
    void* adjacentFreeBlock=RMP_Malloc(Pool, 64);
    void* adjacentFreeBlock2=RMP_Malloc(Pool, 128);
    RMP_Free(Pool, adjacentFreeBlock);
    /* Try to expand initialBlock, to trigger a merge with the right free block */
    /* Realloc: pick a size that ensures the merge leaves no room for a new block */
    void* resizedBlock=RMP_Realloc(Pool, initialBlock, 110);
    /* Cleanup - free the memory and destroy the pool */
    /* Free: the resized block */
    RMP_Free(Pool, resizedBlock);
    /* Free: the remaining neighbor block */
    RMP_Free(Pool, adjacentFreeBlock2);

    /* Free: release the shrunk block, covering the free paths */
    RMP_Free(Pool,ptr_re);

    /*
       Rounded_Size - the adjusted (rounded up) size.
       Mem_Size - the original size of the block being reallocated.
       Res_Size - used to compute whether, after expanding the block and
       possibly merging adjacent free blocks, enough residual space remains to
       form a new free block.
    */
    for(iii=0;iii<1024;iii++)
    {
        Pool[iii]=0U;
    }

    /* Realloc where Mem_Size==Rounded_Size, the no-resize shortcut */
    RMP_Mem_Init(Pool,1024U*sizeof(rmp_ptr_t));
    void* ppp1=RMP_Malloc(Pool, 64);
    void* ppp2=RMP_Malloc(Pool, 128);
    RMP_Free(Pool,ppp1);
    ppp2=RMP_Realloc(Pool,ppp2,128);

    /* Realloc shrink that splits off a residual block */
    for(iii=0;iii<1024;iii++)
    {
        Pool[iii]=0U;
    }
    RMP_Mem_Init(Pool,1024U*sizeof(rmp_ptr_t));
    void* ppp3=RMP_Malloc(Pool, 64);
    void* ppp4=RMP_Malloc(Pool, 256);
    RMP_Free(Pool,ppp3);
    ppp2=RMP_Realloc(Pool,ppp4,128);

    /* Realloc expansion where the new block allocation fails */
    for(iii=0;iii<1024;iii++)
    {
        Pool[iii]=0U;
    }
    RMP_Mem_Init(Pool,1024U*sizeof(rmp_ptr_t));
    void* ppp5=RMP_Malloc(Pool, 64);
    void* ppp6=RMP_Malloc(Pool, 256);
    ppp2=RMP_Realloc(Pool,ppp5,1024U*sizeof(rmp_ptr_t)+10086);

    /* Realloc shrink where the residual block is large enough to be reinserted */
    for(iii=0;iii<1024;iii++)
    {
        Pool[iii]=0U;
    }
    RMP_Mem_Init(Pool,1024U*sizeof(rmp_ptr_t));
    void* ppp7=RMP_Malloc(Pool, 128);
    void* ppp8=RMP_Malloc(Pool, 256);
    ppp2=RMP_Realloc(Pool,ppp7,64);

    /* Res_Size = Mem_Size - Rounded_Size */
    /* if(Res_Size < (sizeof(struct RMP_Mem_Head)+64U+sizeof(struct RMP_Mem_Tail))) not satisfied */
    /* i.e. make Res_Size >= 84 */
    for(iii=0;iii<1024;iii++)
    {
        Pool[iii]=0U;
    }
    RMP_Mem_Init(Pool,1024U*4);
    void* ppp9=RMP_Malloc(Pool, 256);
    void* ppp10=RMP_Malloc(Pool, 256);
    ppp2=RMP_Realloc(Pool,ppp9,64);
}
/* End Function:Cov_Mem ******************************************************/

/* Function:Cov_Fifo_Msgq *****************************************************
Description : Test the FIFO and message queue operations.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Fifo_Msgq()
{
    struct RMP_List* Node;

    /* RMP_Fifo_Crt */
    /* NULL control block */
    RMP_Fifo_Crt(RMP_NULL);

    /* Fifo_1 is already in RMP_FIFO_USED state because it was created
       in RMP_Init_Hook; expect RMP_ERR_FIFO */
    RMP_Fifo_Crt(&Fifo_1);

    /* RMP_Fifo_Del */
    /* NULL control block */
    RMP_Fifo_Del(RMP_NULL);
    RMP_Fifo_Del(&Fifo_1);
    /* Recreate Fifo_1, then try to delete a FIFO in RMP_FIFO_FREE state;
       expect RMP_ERR_FIFO */
    RMP_Fifo_Crt(&Fifo_1);
    Fifo_1.State=RMP_FIFO_FREE;
    RMP_Fifo_Del(&Fifo_1);

    /* See if the FIFO has any elements */
    /* Write one element into the FIFO first */
    RMP_Fifo_Crt(&Fifo_1);
    RMP_Fifo_Write(&Fifo_1,&Node);
    RMP_Fifo_Del(&Fifo_1);

    /* RMP_Fifo_Read */
    RMP_Fifo_Crt(&Fifo_1);
    RMP_Fifo_Read(RMP_NULL,RMP_NULL);
    RMP_Fifo_Read(&Fifo_1,RMP_NULL);
    RMP_Fifo_Read(&Fifo_1,&Node);
    RMP_Fifo_Del(&Fifo_1);

    /* Abnormal case: read from a FIFO in free state; expect RMP_ERR_FIFO */
    RMP_Fifo_Crt(&Fifo_1);
    Fifo_1.State=RMP_FIFO_FREE;
    RMP_Fifo_Read(&Fifo_1,&Node);
    RMP_Fifo_Del(&Fifo_1);

    /* FIFO is empty */
    RMP_Fifo_Crt(&Fifo_1);
    RMP_Fifo_Read(&Fifo_1,&Node);
    RMP_Fifo_Del(&Fifo_1);

    /* RMP_Fifo_Write */
    /* NULL parameters */
    RMP_Fifo_Crt(&Fifo_1);
    RMP_Fifo_Write(RMP_NULL,RMP_NULL);
    RMP_Fifo_Write(&Fifo_1,RMP_NULL);
    RMP_Fifo_Del(&Fifo_1);
    /* Not in RMP_FIFO_USED state; expect RMP_ERR_FIFO */
    RMP_Fifo_Crt(&Fifo_1);
    Fifo_1.State=RMP_FIFO_FREE;
    RMP_Fifo_Write(&Fifo_1,&Node);
    RMP_Fifo_Del(&Fifo_1);

    /* RMP_Fifo_Write_ISR */
    /* NULL parameters */
    RMP_Fifo_Crt(&Fifo_1);
    RMP_Fifo_Write_ISR(RMP_NULL,RMP_NULL);
    RMP_Fifo_Write_ISR(&Fifo_1,RMP_NULL);
    RMP_Fifo_Write_ISR(&Fifo_1,&Node);
    RMP_Fifo_Del(&Fifo_1);

    /* Not in RMP_FIFO_USED state; expect RMP_ERR_FIFO */
    RMP_Fifo_Crt(&Fifo_1);
    Fifo_1.State=RMP_FIFO_FREE;
    RMP_Fifo_Write_ISR(&Fifo_1,&Node);
    RMP_Fifo_Del(&Fifo_1);

    /* RMP_Fifo_Cnt */
    RMP_Fifo_Crt(&Fifo_1);
    RMP_Fifo_Cnt(RMP_NULL);
    RMP_Fifo_Cnt(&Fifo_1);
    Fifo_1.State=RMP_FIFO_FREE;
    RMP_Fifo_Cnt(&Fifo_1);
    RMP_Fifo_Del(&Fifo_1);

    /* RMP_Msgq_Crt */
    /* Msgq-crt: tests the NULL control block rejection branch */
    RMP_Msgq_Crt(RMP_NULL);
    /* Msgq-crt: normal creation so the next call hits the already-used branch */
    RMP_Msgq_Crt(&Msgq_1);
    /* Msgq-del: force free state so the delete sees an unused queue */
    Msgq_1.State=RMP_MSGQ_FREE;
    RMP_Msgq_Del(&Msgq_1);
    /* Restore normal state */
    Msgq_1.State=RMP_MSGQ_USED;
    RMP_Msgq_Del(&Msgq_1);

    /* RMP_Msgq_Del */
    /* Msgq-del: tests the NULL control block rejection branch */
    RMP_Msgq_Del(RMP_NULL);
    /* Msgq-crt: recreate the queue so the delete hits the in-use check */
    RMP_Msgq_Crt(&Msgq_1);
    RMP_Msgq_Del(&Msgq_1);

    /* Msgq-del: put a node in the FIFO so the delete must flush it */
    RMP_Msgq_Crt(&Msgq_1);
    /* Msgq-snd: queue one node before deleting; expect success */
    RMP_Msgq_Snd(&Msgq_1,&Node);
    RMP_Msgq_Del(&Msgq_1);

    /* RMP_Msgq_Snd */
    /* Msgq-snd: tests the NULL queue rejection branch */
    RMP_Msgq_Snd(RMP_NULL,RMP_NULL);
    /* Msgq-snd: tests the NULL node rejection branch */
    RMP_Msgq_Snd(&Msgq_1,RMP_NULL);
    /* Msgq-snd: normal send path */
    RMP_Msgq_Snd(&Msgq_1,&Node);
    Msgq_1.State=RMP_MSGQ_FREE;
    /* Msgq-snd: free-state queue, not-in-use rejection branch */
    RMP_Msgq_Snd(&Msgq_1,&Node);
    /* Restore normal state */
    Msgq_1.State=RMP_MSGQ_USED;
    /* corrupt the queue so the internal semaphore post fails */
    RMP_Clear(&Msgq_1, sizeof(struct RMP_Msgq));
    RMP_Clear(&Sem_1, sizeof(struct RMP_Sem));
    /* Calling RMP_Sem_Post will return an error here. Normally this requires
       the semaphore count to be at its maximum possible value, so that a
       further post fails due to semaphore overflow. */
    RMP_Msgq_Crt(&Msgq_1);
    Msgq_1.Sem.State=RMP_SEM_FREE;
    /* Setting Sem.State to RMP_SEM_FREE makes RMP_Sem_Post(&(Queue->Sem), 1U)
       return RMP_ERR_SEM (-6) */
    /* Msgq-snd: send whose internal semaphore post fails, rejection path */
    RMP_Msgq_Snd(&Msgq_1,&Node);

    /* RMP_Msgq_Snd_ISR */
    RMP_Clear(&Msgq_1, sizeof(struct RMP_Msgq));
    /* Msgq-snd-ISR: tests the NULL queue rejection branch */
    RMP_Msgq_Snd_ISR(RMP_NULL,RMP_NULL);
    RMP_Msgq_Crt(&Msgq_1);
    /* Msgq-snd-ISR: tests the NULL node rejection branch */
    RMP_Msgq_Snd_ISR(&Msgq_1,RMP_NULL);
    RMP_Msgq_Del(&Msgq_1);

    /* create then send against a free-state queue */
    RMP_Msgq_Crt(&Msgq_1);
    /* Msgq-snd-ISR: force free state so the in-use check fails */
    Msgq_1.State=RMP_MSGQ_FREE;
    RMP_Msgq_Snd_ISR(&Msgq_1,&Node);
    Msgq_1.State=RMP_MSGQ_USED;
    RMP_Msgq_Del(&Msgq_1);

    RMP_Msgq_Crt(&Msgq_1);
    RMP_Msgq_Snd_ISR(&Msgq_1,&Node);
    RMP_Msgq_Del(&Msgq_1);

    RMP_Msgq_Crt(&Msgq_1);
    Msgq_1.Sem.State=RMP_SEM_FREE;
    /* Setting Sem.State to RMP_SEM_FREE makes RMP_Sem_Post(&(Queue->Sem), 1U)
       return RMP_ERR_SEM (-6) */
    RMP_Msgq_Snd_ISR(&Msgq_1,&Node);
    Msgq_1.Sem.State=RMP_MSGQ_USED;
    RMP_Msgq_Del(&Msgq_1);

    /* RMP_Msgq_Rcv */
    RMP_Msgq_Crt(&Msgq_1);
    /* Msgq-rcv: tests the NULL queue rejection branch */
    RMP_Msgq_Rcv(RMP_NULL,RMP_NULL,1);
    /* Msgq-rcv: tests the NULL node rejection branch */
    RMP_Msgq_Rcv(&Msgq_1,RMP_NULL,1);
    /* Msgq-rcv: normal receive path */
    RMP_Msgq_Rcv(&Msgq_1,&Node,10);
    /* Msgq-rcv: force free state so the in-use check fails */
    Msgq_1.State=RMP_SEM_FREE;
    RMP_Msgq_Rcv(&Msgq_1,&Node,10);
    Msgq_1.State=RMP_MSGQ_USED;
    RMP_Msgq_Del(&Msgq_1);

    /* if(RMP_Sem_Pend_Unlock(&(Queue->Sem), Slice)<0) */
    /* Try to acquire the semaphore; only when we succeed do we proceed. The
       whole queue may have been deleted in the meantime, so this must be taken
       into account. Note that the potential semaphore-FIFO race is impossible
       here: unlike condition variables, semaphores are counted, so if a thread
       captures a semaphore, something is guaranteed to be in the FIFO. */
    RMP_Msgq_Crt(&Msgq_1);
    /* Msgq-rcv: cannot acquire the semaphore in time, timeout path
       (nothing was posted) */
    RMP_Msgq_Rcv(&Msgq_1,&Node,20);
    RMP_Msgq_Del(&Msgq_1);

    /* if(RMP_Fifo_Read(&(Queue->Fifo), Node)<0) */
    RMP_Clear(&Msgq_1, sizeof(struct RMP_Msgq));
    RMP_Msgq_Crt(&Msgq_1);
    /* Sem-post-ISR: post one count so the pend succeeds and is not caught by the pend timeout check */
    RMP_Sem_Post_ISR(&Msgq_1.Sem, 1);
    /* Msgq-rcv: force free FIFO so the internal read fails */
    Msgq_1.Fifo.State=RMP_FIFO_FREE;
    RMP_Msgq_Rcv(&Msgq_1,&Node,100);
    /* Restore normal state */
    Msgq_1.Fifo.State=RMP_FIFO_USED;
    RMP_Msgq_Del(&Msgq_1);

    /* RMP_Msgq_Cnt - Get the number of nodes in the message queue */
    /* Msgq-cnt: tests the NULL control block rejection branch */
    RMP_Msgq_Cnt(RMP_NULL);
    RMP_Clear(&Msgq_1, sizeof(struct RMP_Msgq));
    /* Msgq-crt: fresh creation so the count query succeeds */
    RMP_Msgq_Crt(&Msgq_1);
    /* Msgq-cnt: force free state so the in-use check fails */
    Msgq_1.State=RMP_FIFO_FREE;
    RMP_Msgq_Cnt(&Msgq_1);
    /* Restore normal state */
    Msgq_1.State=RMP_FIFO_USED;
    RMP_Msgq_Del(&Msgq_1);

    /* count query on a normal, empty queue */
    RMP_Clear(&Msgq_1, sizeof(struct RMP_Msgq));
    RMP_Msgq_Crt(&Msgq_1);
    RMP_Msgq_Cnt(&Msgq_1);
    RMP_Msgq_Del(&Msgq_1);
}
/* End Function:Cov_Fifo_Msgq ************************************************/

/* Function:Cov_Bmq ***********************************************************
Description : Test the blocking message queue (Bmq) operations.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Bmq(void)
{
    struct RMP_List* Node;

    RMP_Clear(&Bmq_1, sizeof(struct RMP_Bmq));

    /* RMP_Bmq_Crt */
    /* Bmq-crt: tests the NULL queue rejection branch */
    RMP_Bmq_Crt(RMP_NULL,1);
    /* Bmq-crt: normal creation path */
    RMP_Bmq_Crt(&Bmq_1,10);
    /* Do not delete Bmq_1 here, so that the next creation sees a USED state */
    /* Bmq-crt: already-in-use rejection branch */
    RMP_Bmq_Crt(&Bmq_1,10);
    RMP_Bmq_Del(&Bmq_1);
    /* Bmq-crt: zero limit rejection branch, expect RMP_ERR_OPER */
    RMP_Bmq_Crt(&Bmq_1,0U);

    /* RMP_Bmq_Del */
    /* Bmq-del: tests the NULL control block rejection branch */
    RMP_Bmq_Del(RMP_NULL);
    /* Bmq-del: queue is not in use, rejection branch */
    RMP_Bmq_Del(&Bmq_1);

    /* if(RMP_Msgq_Del(&(Queue->Msgq))<0) - Attempt to delete the message queue first */
    RMP_Bmq_Crt(&Bmq_1,10);
    /* Bmq-del: abnormal case - force free state so the internal Msgq_Del fails */
    Bmq_1.Msgq.State=RMP_MSGQ_FREE;
    RMP_Bmq_Del(&Bmq_1);
    Bmq_1.Msgq.State=RMP_MSGQ_USED;
    /* Bmq-del: delete correctly this time */
    RMP_Bmq_Del(&Bmq_1);

    /* RMP_Bmq_Snd */
    RMP_Clear(&Bmq_1, sizeof(struct RMP_Bmq));
    /* Bmq-snd: tests the NULL queue rejection branch */
    RMP_Bmq_Snd(RMP_NULL,&Node,10);
    /* Bmq-snd: tests the NULL node rejection branch */
    RMP_Bmq_Snd(&Bmq_1,RMP_NULL,10);
    /* Bmq-snd: state != RMP_BMQ_USED rejection branch */
    RMP_Bmq_Snd(&Bmq_1,&Node,20);
    RMP_Clear(&Bmq_1, sizeof(struct RMP_Bmq));
    RMP_Bmq_Crt(&Bmq_1,5);
    /* Bmq-snd: semaphore state != RMP_SEM_USED, makes _RMP_Sem_Pend_Core
       return RMP_ERR_SEM, satisfying the 4885 condition */
    Bmq_1.Sem.State=RMP_SEM_FREE;
    RMP_Bmq_Snd(&Bmq_1,&Node,20);
    /* Restore */
    Bmq_1.Sem.State=RMP_SEM_USED;
    /* Bmq-snd: normal send path */
    RMP_Bmq_Snd(&Bmq_1,&Node,20);
    /* if(RMP_Msgq_Snd(&(Queue->Msgq), Node)<0) */
    /* Bmq-snd: abnormal case - RMP_Msgq_Snd returns RMP_ERR_MSGQ */
    Bmq_1.Msgq.State=RMP_MSGQ_FREE;
    RMP_Bmq_Snd(&Bmq_1,&Node,20);
    /* Bmq-del: delete correctly */
    RMP_Bmq_Del(&Bmq_1);

    /* RMP_Bmq_Snd_ISR */
    RMP_Clear(&Bmq_1, sizeof(struct RMP_Bmq));
    /* Bmq-snd-ISR: tests the NULL queue rejection branch */
    RMP_Bmq_Snd_ISR(RMP_NULL,RMP_NULL);
    /* Bmq-snd-ISR: tests the NULL node rejection branch */
    RMP_Bmq_Snd_ISR(&Bmq_1,RMP_NULL);
    /* Bmq-snd-ISR: queue not created, rejection branch */
    RMP_Bmq_Snd_ISR(&Bmq_1,&Node);
    RMP_Bmq_Crt(&Bmq_1,5);
    /* Bmq-snd-ISR: abnormal case - Sem.Cur_Num is zero */
    Bmq_1.Sem.Cur_Num=0;
    RMP_Bmq_Snd_ISR(&Bmq_1,&Node);
    Bmq_1.Sem.Cur_Num=5;
    /* Bmq-snd-ISR: normal send path */
    RMP_Bmq_Snd_ISR(&Bmq_1,&Node);
    /* Bmq-del: delete correctly */
    RMP_Bmq_Del(&Bmq_1);

    /* RMP_Bmq_Rcv */
    RMP_Clear(&Bmq_1, sizeof(struct RMP_Bmq));
    /* Bmq-rcv: tests the NULL queue rejection branch */
    RMP_Bmq_Rcv(RMP_NULL,RMP_NULL,10);
    /* Bmq-rcv: tests the NULL node rejection branch */
    RMP_Bmq_Rcv(&Bmq_1,RMP_NULL,10);
    /* Bmq-rcv: state != RMP_BMQ_USED rejection branch */
    RMP_Bmq_Rcv(&Bmq_1,&Node,10);
    RMP_Bmq_Crt(&Bmq_1,10);
    /* Bmq-rcv: receive from an empty queue, timeout path */
    RMP_Bmq_Rcv(&Bmq_1,&Node,100);
    /* Bmq-snd-ISR: queue three nodes so the next receive succeeds */
    RMP_Bmq_Snd_ISR(&Bmq_1,&Node);
    RMP_Bmq_Snd_ISR(&Bmq_1,&Node);
    RMP_Bmq_Snd_ISR(&Bmq_1,&Node);
    /* Bmq-rcv: normal receive path with data available */
    RMP_Bmq_Rcv(&Bmq_1,&Node,100);
    /* Bmq-del: delete correctly */
    RMP_Bmq_Del(&Bmq_1);

    /* RMP_Bmq_Cnt */
    /* Bmq-cnt: tests the NULL control block rejection branch */
    RMP_Bmq_Cnt(RMP_NULL);
    /* Bmq-cnt: query on a free queue, rejection branch */
    RMP_Bmq_Cnt(&Bmq_1);
    RMP_Bmq_Crt(&Bmq_1,10);
    /* Bmq-cnt: query on a used queue, success path */
    RMP_Bmq_Cnt(&Bmq_1);
    /* Bmq-del: delete correctly */
    RMP_Bmq_Del(&Bmq_1);
    /* Bmq-cnt: query on a cleared structure, free-state branch */
    RMP_Clear(&Bmq_1, sizeof(struct RMP_Bmq));
    RMP_Bmq_Cnt(&Bmq_1);
}
/* End Function:Cov_Bmq ******************************************************/

/* Function:Thd_Cov_Entry *****************************************************
Description : The entry of the main coverage test thread. Runs all the
              coverage test cases in sequence, then prints the coverage
              result and stops.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Thd_Cov_Entry(void)
{
    /* Print table header */
    RMP_DBG_S("\r\n    ___   __  ___ ___\r\n");
    RMP_DBG_S("   / _ \\ /  |/  // _ \\       Simple real-time kernel\r\n");
    RMP_DBG_S("  / , _// /|_/ // ___/       Coverage test\r\n");
    RMP_DBG_S(" /_/|_|/_/  /_//_/\r\n");
    RMP_DBG_S("====================================================\r\n");

    /* Run every coverage test defined in this file, in definition order */
    /* Bit manipulation tests */
    Cov_Bit();
    /* Print helper tests */
    Cov_Print();
    /* Scheduler lock tests */
    Cov_Lock();
    /* Thread parameter tests */
    Cov_Thd_Param();
    /* Thread operation tests */
    Cov_Thd_Oper();
    /* Thread deletion tests */
    Cov_Thd_Del();
    /* Semaphore parameter tests */
    Cov_Sem_Param();
    /* Semaphore operation tests */
    Cov_Sem_Oper();
    /* Memory pool tests */
    Cov_Mem();
    /* FIFO and message queue tests */
    Cov_Fifo_Msgq();
    /* Blocking message queue tests */
    Cov_Bmq();

    /* Print all covered line numbers to the serial port */
    RMP_Cov_Print();

    /* Should stop here */
    while(1);
}
/* End Function:Thd_Cov_Entry ************************************************/

/* Function:RMP_Init_Hook *****************************************************
Description : The init thread hook functions.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Init_Hook(void)
{
    /* Clean up the structures */
    RMP_Clear(&Thd_Cov, sizeof(struct RMP_Thd));
    /* Start main coverage thread */
    RMP_Thd_Crt(&Thd_Cov,
                (void*)Thd_Cov_Entry, (void*)0x1234U,
                Stack_Cov, sizeof(Stack_Cov),
                PRIO_COV, 50U);
}

void RMP_Init_Idle(void)
{
    return;
}
/* End Function:RMP_Init_Hook ************************************************/

/* End Of File ***************************************************************/
