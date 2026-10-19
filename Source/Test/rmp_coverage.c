/******************************************************************************
Filename    : rmp_coverage.c
Author      : pry, zk (zhang kai); ChatGLM
Date        : 22/07/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The white-box coverage test for RMP. Instead of firing interrupts,
              this test simulates them. This test is designed specifically for
              32-bit systems, and may need some tweaks to run on other systems.
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
#include "rmp_test.h"
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
/* FIFO used directly by Cov_Fifo, and also the embedded FIFO
 * whose state is forced to RMP_FIFO_FREE to make RMP_Msgq_Rcv fail */
volatile struct RMP_Fifo Fifo_1;
/* Message queue used by Cov_Msgq; its State and Sem.State are
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
/* The alarm manager and alarms used by Cov_Amgr */
volatile struct RMP_Amgr Amgr_1;
volatile struct RMP_Alrm Alrm_1;
volatile struct RMP_Alrm Alrm_2;
volatile struct RMP_Alrm Alrm_3;
volatile rmp_ptr_t Alrm_1_Cnt=0U;
volatile rmp_ptr_t Alrm_2_Cnt=0U;
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
    RMP_Int_Print(0);
    /* Int-print: tests the Int<0 negative branch and the multi-digit loop */
    RMP_Int_Print(-1);
    /* Int-print: tests the single-digit positive path */
    RMP_Int_Print(7);
    /* Int-print: tests a three-digit positive number */
    RMP_Int_Print(123);
    /* Int-print: tests a large positive number with more loop iterations */
    RMP_Int_Print(12345);
    RMP_ASSERT(RMP_Str_Print((const rmp_s8_t*)"\r\n")==2U);

    /* Hex-print: tests the Uint==0 early-return branch */
    RMP_Hex_Print(0U);
    /* Hex-print: tests the numeric digit (0-9) branch */
    RMP_Hex_Print(0x12345678UL);
    /* Hex-print: tests the hex letter (A-F) branch */
    RMP_Hex_Print(0xABCDEF12UL);
    RMP_ASSERT(RMP_Str_Print((const rmp_s8_t*)"\r\n")==2U);

    /* Str-print: tests the normal loop and its break-on-NUL branch */
    RMP_ASSERT(RMP_Str_Print((const rmp_s8_t*)"Hello, World!")==13U);
    /* Str-print: tests the loop bound against overlong strings */
    RMP_ASSERT(RMP_Str_Print((const rmp_s8_t*)(STR STR STR STR STR STR))==RMP_DBGLOG_MAX);
    RMP_ASSERT(RMP_Str_Print((const rmp_s8_t*)"\r\n")==2U);
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
    /* Yield: unlocked yield switches back to ourself (round-robin requeue) */
    RMP_Thd_Yield();

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
    RMP_Thd_Yield();
    /* Sched-unlock: tests the count==1, pending-yield branch */
    RMP_Sched_Unlock();
}
/* End Function:Cov_Lock *****************************************************/

/* Function:Cov_Tim ***********************************************************
Description : Test the timer helper routines. These are normally only
              used by tickless kernels, but they are plain functions
              and are called directly here for coverage.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Thd_Param_Delay_Func(void)
{
    while(1)
    {
        RMP_Thd_Delay(10000U);
    }
}

void Cov_Thd_Param_Spin_Func(void)
{
    while(1);
}

void Cov_Tim(void)
{
    volatile rmp_ptr_t Timeout_Bkp;

    /* Tim-idle: nothing delays and only the init thread is at prio 0.
     * Note that the "Run[0] head is not the init thread" branch inside
     * _RMP_Tim_Idle is covered at the end of this function: it requires
     * the init thread to be round-robined to the tail of its priority
     * level. The init thread is born at the head of level 0 and every
     * other thread is inserted behind it, so the only way to move it is
     * the dispatcher's timeslice rotation (_RMP_Run_High). Coverage
     * builds shrink the init timeslice to 1 (RMP_INIT_SLICE), so this
     * takes just one tick with a ready prio-0 companion, instead of the
     * full 65500-slice quanta of normal builds. */
    RMP_USE(_RMP_Tim_Idle());
    /* Tim-future: the delay queue is empty */
    RMP_USE(_RMP_Tim_Future());

    /* Thd-crt: a delayed worker parks itself in the delay queue */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Param_Delay_Func, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 10U));
    /* (the worker preempts and parks itself in the delay queue) */

    /* Tim-future: a delay is pending, far in the future */
    RMP_USE(_RMP_Tim_Future());
    /* Tim-idle: a delay is pending */
    RMP_USE(_RMP_Tim_Idle());

    /* Corrupt the timeout into the recent past: the overflow path */
    Timeout_Bkp=Thd_1.Timeout;
    Thd_1.Timeout=RMP_Timestamp-1U;
    RMP_USE(_RMP_Tim_Future());
    /* Corrupt the timeout into the near future: closer than our slice */
    Thd_1.Timeout=RMP_Timestamp+2U;
    RMP_USE(_RMP_Tim_Future());
    Thd_1.Timeout=Timeout_Bkp;

    PASS(RMP_Thd_Del(&Thd_1));

    /* Thd-crt: a worker queued at the same level as the init thread */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Param_Delay_Func, RMP_NULL, Stack_1, sizeof(Stack_1), 0U, 10U));
    /* (does not preempt: prio 0 is below ours) */
    /* Tim-idle: the init level now has another thread queued behind */
    RMP_USE(_RMP_Tim_Idle());
    PASS(RMP_Thd_Del(&Thd_1));

    /* Tim-idle: move the init thread off the head of priority level 0.
     * Create a prio-0 worker that busy-spins (never blocks), then park
     * this thread for a few ticks: the init thread runs as the head of
     * level 0, expires its timeslice (1 slice in coverage builds, see
     * RMP_INIT_SLICE), and is round-robined to the tail by the dispatcher
     * (_RMP_Run_High), leaving the spinning worker at the head. When we
     * wake up (prio 3 preempts prio 0 without rotating it), _RMP_Tim_Idle
     * finally sees a non-init head. The worker must spin instead of
     * delaying: a delaying worker would leave the run queue on its first
     * dispatch, putting the init thread back at the head before we wake.
     * Its timeslice (100) must also outlast our parking window (3 ticks)
     * so that the worker itself never gets rotated to the tail. */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Param_Spin_Func, RMP_NULL, Stack_1, sizeof(Stack_1), 0U, 100U));
    /* (does not preempt: prio 0 is below ours; the worker stays ready) */
    PASS(RMP_Thd_Delay(3U));
    /* (the init thread expired and was rotated; we preempted the worker,
     * which is still the head of priority level 0) */
    RMP_USE(_RMP_Tim_Idle());
    /* (deleting the worker restores the init thread as the only resident) */
    PASS(RMP_Thd_Del(&Thd_1));
}
/* End Function:Cov_Tim ******************************************************/

/* Function:Cov_Thd_Param *****************************************************
Description : Thread simple parameter tests.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Thd_Param_Suspend_Func(void)
{
    RMP_Thd_Suspend(&Thd_1);
    while(1);
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

    /* Thd-crt: recreate the test thread for the parameter tests below.
     * A real entry must be used here: this thread stays ready at a low
     * priority while the test thread blocks on timed sends, and will
     * be dispatched during those windows. A dummy entry such as DUMMY
     * would make the scheduler jump to an arbitrary flash address. */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Param_Delay_Func, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_BOTTOM, 10U));

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

    /* Thd-set: timeslice left unchanged when Slice==RMP_SLICE_MAX */
    PASS(RMP_Thd_Set(&Thd_1, PRIO_HIGHER, RMP_SLICE_MAX));

    /* Thd-rcv: zero-slice nonblocking receive on an empty mailbox */
    FAIL(RMP_Thd_Rcv((rmp_ptr_t*)RMP_NULL, 0U));

    /* Thd-del: final cleanup of the test thread */
    PASS(RMP_Thd_Del(&Thd_1));

    /* Thd-set/suspend/resume/snd/cancel on a freed thread: rejections */
    FAIL(RMP_Thd_Set(&Thd_1, PRIO_BOTTOM, 10U));
    FAIL(RMP_Thd_Suspend(&Thd_1));
    FAIL(RMP_Thd_Resume(&Thd_1));
    FAIL(RMP_Thd_Snd(&Thd_1, 1U, 10U));
    FAIL(RMP_Thd_Cancel(&Thd_1));
}
/* End Function:Cov_Thd_Param ************************************************/

/* Function:Cov_Thd_Oper ******************************************************
Description : Test thread delay, suspend, resume, send and send-ISR against
              a worker exercised in every waiting state, plus the Peek-ISR
              ready-thread paths.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Thd_Rcv_Timeout_Func(void)
{
    rmp_ptr_t Data;

    /* Thd-rcv: blocks on a finite 200-tick timeout */
    RMP_Thd_Rcv(&Data, 200U);
    /* Thd-delay: park the thread in the delaying state afterwards */
    while(1)
    {
        RMP_Thd_Delay(10000U);
    }
}

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
    while(1)
    {
        PASS(RMP_Thd_Delay(10000U));
    }
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

    /* Thd-delay: let the worker run and block on its first receive */
    PASS(RMP_Thd_Delay(10000U));

    /* Sched-lock: simulate ISR context for the ready-thread test */
    RMP_Sched_Lock();
    /* Thd-snd-ISR: hand a message to the blocked receiver; it is woken
     * to the ready state, and cannot preempt us in the ISR context.
     * The message is copied into its mailbox, which is marked full. */
    PASS(RMP_Thd_Snd_ISR(&Thd_1, 1U));
    /* Thd-snd-ISR: the mailbox is full now, rejection path with a ready receiver */
    FAIL(RMP_Thd_Snd_ISR(&Thd_1, 2U));
    /* Sched-unlock: leave the simulated ISR context */
    RMP_Sched_Unlock();

    /* Thd-delay: delay while Thd_1 is ready but cannot preempt us */
    PASS(RMP_Thd_Delay(10000U));

    /* Thd-del: final cleanup of the worker thread */
    PASS(RMP_Thd_Del(&Thd_1));

    /* ---- Scheduler-aware thread operations ---- */

    /* Peek-ISR: only low priorities are ready; the high bitmap word is empty */
    RMP_Sched_Lock();
    RMP_USE(RMP_Thd_Peek_ISR());
    RMP_Sched_Unlock();

    /* Thd-crt: a worker queued at our own priority level */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Param_Delay_Func, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_COV, 10U));
    RMP_Sched_Lock();
    /* Peek-ISR: we are the head of our level, and another thread is queued
     * behind us - the not-alone peek path */
    RMP_USE(RMP_Thd_Peek_ISR());
    RMP_Sched_Unlock();
    /* Thd-del: removing a thread that is not alone on its priority level */
    PASS(RMP_Thd_Del(&Thd_1));

    /* Thd-crt: a higher-priority worker created under the lock, so it
     * cannot preempt us yet - for the not-head peek path */
    RMP_Sched_Lock();
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Param_Delay_Func, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 10U));
    /* Peek-ISR: the current thread is not the head of the highest level */
    RMP_USE(RMP_Thd_Peek_ISR());
    RMP_Sched_Unlock();
    /* (the worker preempts and parks itself in the delay queue) */
    PASS(RMP_Thd_Del(&Thd_1));

    /* Thd-cancel: NULL control block rejection */
    FAIL(RMP_Thd_Cancel(RMP_NULL));
    /* Thd-cancel: not in the delaying state, rejection */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Param_Delay_Func, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_LOWER, 10U));
    FAIL(RMP_Thd_Cancel(&Thd_1));
    PASS(RMP_Thd_Del(&Thd_1));
    /* Thd-cancel: aborting a thread from its delaying state */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Param_Delay_Func, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 10U));
    /* (the worker preempts and parks itself in the delay queue) */
    PASS(RMP_Thd_Cancel(&Thd_1));
    /* (the cancelled worker preempts again and re-parks itself) */
    PASS(RMP_Thd_Del(&Thd_1));
    /* Thd-cancel: freed thread, rejection */
    FAIL(RMP_Thd_Cancel(&Thd_1));

    /* Sem-abort: the thread is delaying, not pending on a semaphore */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Param_Delay_Func, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 10U));
    /* (the worker preempts and parks itself in the delay queue) */
    FAIL(RMP_Sem_Abort(&Thd_1));
    PASS(RMP_Thd_Del(&Thd_1));

    /* ---- Timed receiver scenarios ---- */

    /* Thd-crt: a timed receiver woken by a plain send */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Rcv_Timeout_Func, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 10U));
    /* (the worker preempts and blocks on a timed receive) */
    /* Thd-snd: waking a timed receiver removes it from the delay queue */
    PASS(RMP_Thd_Snd(&Thd_1, 1U, RMP_SLICE_MAX));
    PASS(RMP_Thd_Del(&Thd_1));

    /* Thd-crt: a timed receiver woken from the ISR context */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Rcv_Timeout_Func, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 10U));
    RMP_Sched_Lock();
    /* Thd-snd-ISR: waking a timed receiver from an ISR context */
    PASS(RMP_Thd_Snd_ISR(&Thd_1, 1U));
    RMP_Sched_Unlock();
    PASS(RMP_Thd_Del(&Thd_1));

    /* Thd-crt: a timed receiver left to time out */
    PASS(RMP_Thd_Crt(&Thd_1, Cov_Thd_Rcv_Timeout_Func, RMP_NULL, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 10U));
    /* (the worker preempts and blocks on a timed receive) */
    /* Thd-delay: sleep past the 200-tick receive timeout - the timer
     * handler aborts the receiver from the delay queue, and the receiver
     * returns from its timed receive with a timeout error */
    PASS(RMP_Thd_Delay(300U));
    PASS(RMP_Thd_Del(&Thd_1));
}
/* End Function:Cov_Thd_Oper *************************************************/

/* Function:Cov_Thd_Del *******************************************************
Description : Test thread deletion under various circumstances, hitting
              every thread state branch of the deletion path.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Thd_Del_Delay_Func(void)
{
    /* Thd-delay: puts the thread into the delaying state */
    RMP_Thd_Delay(10000U);
    while(1);
}

void Cov_Thd_Del_Suspend_Func(void)
{
    /* Thd-suspend: puts the thread into the suspended state */
    RMP_Thd_Suspend(&Thd_1);
    while(1);
}

void Cov_Thd_Del_Snd_Func(void)
{
    /* Thd-snd: first send lands in an empty mailbox of the main thread */
    RMP_Thd_Snd(&Thd_Cov, 1U, RMP_SLICE_MAX);
    /* Thd-snd: second send blocks infinitely on a full mailbox */
    RMP_Thd_Snd(&Thd_Cov, 1U, RMP_SLICE_MAX);
    while(1);
}

void Cov_Thd_Del_Snd_Delay_Func(void)
{
    /* Thd-snd: first send lands in an empty mailbox of the main thread */
    RMP_Thd_Snd(&Thd_Cov, 1U, RMP_SLICE_MAX);
    /* Thd-snd: second send blocks with a timeout on a full mailbox */
    RMP_Thd_Snd(&Thd_Cov, 1U, RMP_SLICE_MAX);
    while(1);
}

void Cov_Thd_Del_Rcv_Func(void)
{
    rmp_ptr_t Data;

    /* Thd-rcv: blocks infinitely waiting for a message */
    RMP_Thd_Rcv(&Data, RMP_SLICE_MAX);
    while(1);
}

void Cov_Thd_Del_Rcv_Delay_Func(void)
{
    rmp_ptr_t Data;

    /* Thd-rcv: blocks with a timeout waiting for a message */
    RMP_Thd_Rcv(&Data, 10000U);
    while(1);
}

void Cov_Thd_Del_Rcv_Sender_Func(void)
{
    /* Thd-snd: first send lands in an empty mailbox of Thd_2 */
    RMP_Thd_Snd(&Thd_2, 1U, RMP_SLICE_MAX);
    /* Thd-snd: second send blocks infinitely on Thd_2's full mailbox */
    RMP_Thd_Snd(&Thd_2, 1U, RMP_SLICE_MAX);
    /* Thd-delay: park the thread after the blocking sends are set up */
    RMP_Thd_Delay(10000U);
    while(1);
}

void Cov_Thd_Del_Rcv_Sender_Delay_Func(void)
{
    /* Thd-snd: first send lands in an empty mailbox of Thd_2 */
    RMP_Thd_Snd(&Thd_2, 1U, RMP_SLICE_MAX);
    /* Thd-snd: second send blocks with a timeout on Thd_2's full mailbox */
    RMP_Thd_Snd(&Thd_2, 1U, 10000U);
    /* Thd-delay: park the thread after the blocking sends are set up */
    RMP_Thd_Delay(10000U);
    while(1);
}

void Cov_Thd_Del_Rcv_Receiver_Func(void)
{
    /* Thd-delay: keep Thd_2 delayed so its mailbox stays full */
    RMP_Thd_Delay(10000U);
    while(1);
}

void Cov_Thd_Del_Pend_Func(void)
{
    /* Sem-pend: blocks infinitely on the counting semaphore */
    RMP_Sem_Pend(&Sem_1, RMP_SLICE_MAX);
    while(1);
}

void Cov_Thd_Del_Pend_Delay_Func(void)
{
    /* Sem-pend: blocks with a timeout on the counting semaphore */
    RMP_Sem_Pend(&Sem_1, 10000U);
    while(1);
}

void Cov_Thd_Del(void)
{
    rmp_ptr_t Data;

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
    /* Thd-rcv: drains the message the deleted thread left in our mailbox,
     * through the unblock path with a real data pointer */
    PASS(RMP_Thd_Rcv(&Data, RMP_SLICE_MAX));

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

    /* Sem-crt: recreate the semaphore for the pend/post tests below */
    PASS(RMP_Sem_Crt(&Sem_1, 0U));

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
    PASS(RMP_Sem_Post(&Sem_1, 1U));
    /* Sem-pend: tests the immediate zero-slice pend success path.
     * A successful pend returns the number left in the semaphore, so we
     * post a single count and let the pend drain it to zero. */
    PASS(RMP_Sem_Pend(&Sem_1, 0U));
    /* Sem-post: refill for the timed pend */
    PASS(RMP_Sem_Post(&Sem_1, 1U));
    /* Sem-pend: tests the immediate timed pend success path */
    PASS(RMP_Sem_Pend(&Sem_1, 10U));
    /* Sem-post: refill for the infinite pend */
    PASS(RMP_Sem_Post(&Sem_1, 1U));
    /* Sem-pend: tests the immediate infinite pend success path */
    PASS(RMP_Sem_Pend(&Sem_1, RMP_SLICE_MAX));
    /* Sem-post: refill so the pend-unlock and broadcast tests have counts */
    PASS(RMP_Sem_Post(&Sem_1, 1U));
    /* Sem-bcst: tests the broadcast wakeup path */
    PASS(RMP_Sem_Bcst(&Sem_1));
    /* Sched-lock: lock twice so that pend-unlock sees an over-locked
     * scheduler; calling it with an unlocked scheduler (count zero) is
     * forbidden and trips the kernel assertion inside sched-unlock */
    RMP_Sched_Lock();
    RMP_Sched_Lock();
    /* Sem-pend-unlock: tests the scheduler-not-locked-once rejection branch */
    FAIL(RMP_Sem_Pend_Unlock(&Sem_1, RMP_SLICE_MAX));
    /* Sched-unlock: drop the extra lock taken above */
    RMP_Sched_Unlock();
    /* Sched-lock: provide the single lock the pend-unlock call requires */
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

/* Function:Cov_Sem_Oper ******************************************************
Description : Test the thread and semaphore operations, including semaphore
              deletion while someone is waiting on it, pend aborts, and
              semaphore posts from ISR context.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Test_Set_2(rmp_ptr_t Param)
{
    /* Dispatch on the test case selector passed by the creator */
    switch(Param)
    {
        case 0:
        {
            /* Thd-delay: park the thread in the delaying state for a long,
             * but finite time; an infinite delay (RMP_SLICE_MAX) would be
             * rejected outright by the parameter checks in the kernel */
            RMP_Thd_Delay(10000U);
            while(1);
        }
        case 2:
        {
            /* Sem-pend: blocks infinitely; the thread is deleted while pending */
            RMP_Sem_Pend(&Sem_1, RMP_SLICE_MAX);
            /* Thd-del: self-delete after the pend returns (aborted or woken) */
            RMP_Thd_Del(&Thd_1);
            while(1);
        }
        case 3:
        {
            /* Sem-pend: blocks with a timeout; the thread is deleted while waiting */
            RMP_Sem_Pend(&Sem_1, 10000U);
            /* Thd-del: self-delete after the pend returns (aborted or woken) */
            RMP_Thd_Del(&Thd_1);
            while(1);
        }
    }
}

void Cov_Sem_Oper(void)
{
    rmp_ret_t Sched_Pend_Bkp;
    rmp_ret_t Lock_Cnt_Bkp;

    /* Sem-crt: recreate the semaphore for the deletion-while-pending tests */
    RMP_Sem_Crt(&Sem_1, 0U);
    /* Thd-crt: worker that pends infinitely; used for deletion-while-pending.
     * The worker runs at a higher priority so it gets to block before us. */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, (void*)2, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 5U);
    /* Sem-del: deletes the semaphore while a thread pends on it, aborting
     * the pend; the aborted worker then deletes itself */
    RMP_Sem_Del(&Sem_1);
    /* Sem-crt: recreate the semaphore for the timed-path deletion test */
    RMP_Sem_Crt(&Sem_1, 0U);
    /* Thd-crt: worker that pends with a timeout; deletion hits the timed path */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, (void*)3, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 5U);
    /* Sem-del: deletes the semaphore while a timed pend is waiting on it;
     * the aborted worker then deletes itself */
    RMP_Sem_Del(&Sem_1);
    /* Sem-crt: recreate the semaphore for the timed-pend wakeup round */
    RMP_Sem_Crt(&Sem_1, 0U);
    /* Thd-crt: worker that pends with a timeout again */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, (void*)3, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 5U);
    /* Sem-post: wakes the timed pend so it completes successfully, and the
     * woken worker then deletes itself */
    RMP_Sem_Post(&Sem_1, 1U);

    /* Sem-post: put one count on the semaphore for the immediate pends below */
    RMP_Sem_Post(&Sem_1, 1U);
    /* Sem-pend: timed pend that returns immediately because a count exists */
    RMP_Sem_Pend(&Sem_1, 100U);
    /* Sem-pend: zero-slice pend that returns immediately */
    RMP_Sem_Pend(&Sem_1, 0U);

    /* Thd-crt: worker that blocks on the semaphore for the pend-abort test */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, (void*)2, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 5U);
    /* Sem-abort: aborts the wait while the thread is blocked on the semaphore;
     * the aborted worker then deletes itself */
    RMP_Sem_Abort(&Thd_1);
    /* Thd-crt: boot a worker that pends with a timeout */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, (void*)3, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 5U);
    /* Sem-abort: aborts the wait while the thread is delaying on the semaphore;
     * the aborted worker then deletes itself */
    RMP_Sem_Abort(&Thd_1);
    /* Thd-del: clean up any worker left over (self-deleted ones reject here) */
    RMP_Thd_Del(&Thd_1);

    /* Sem-post-ISR: count too large, rejection branch from ISR context */
    RMP_Sem_Post_ISR(&Sem_1, RMP_SEM_CNT_MAX);
    /* Sem-del: drop the semaphore before the ISR rejection tests */
    RMP_Sem_Del(&Sem_1);
    /* Sched-lock: simulate ISR context for the ISR post tests */
    RMP_Sched_Lock();
    /* Sem-post-ISR: tests the NULL control block rejection branch */
    RMP_Sem_Post_ISR(RMP_NULL, 1U);
    /* Sem-post-ISR: posts to a deleted semaphore, not-in-use rejection branch */
    RMP_Sem_Post_ISR(&Sem_1, 2U);
    /* Sched-unlock: leave the simulated ISR context */
    RMP_Sched_Unlock();

    /* Sem-crt: recreate the semaphore for the ISR wakeup tests */
    RMP_Sem_Crt(&Sem_1, 0U);
    /* Thd-crt: worker blocked on the semaphore for the ISR wakeup test */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, (void*)2, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 5U);
    /* Sched-lock: simulate ISR context; post wakes the blocked thread */
    RMP_Sched_Lock();
    /* Sem-post-ISR: successful post that unblocks the pending thread;
     * the woken worker cannot run until we unlock the scheduler */
    RMP_Sem_Post_ISR(&Sem_1, 1U);
    /* Sched-unlock: leave the simulated ISR context; the woken worker
     * then runs, completes its pend and deletes itself */
    RMP_Sched_Unlock();
    /* Thd-del: clean up the woken worker (self-deleted; rejected here) */
    RMP_Thd_Del(&Thd_1);

    /* Thd-crt: worker pending with a timeout for the ISR wakeup test */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, (void*)3, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 5U);
    /* Sched-lock: simulate ISR context; post wakes the delaying thread */
    RMP_Sched_Lock();
    /* Sem-post-ISR: successful post that wakes the timed pending thread */
    RMP_Sem_Post_ISR(&Sem_1, 1U);
    /* Sched-unlock: leave the simulated ISR context; the woken worker
     * then runs, completes its pend and deletes itself */
    RMP_Sched_Unlock();
    /* Thd-del: clean up the woken worker (self-deleted; rejected here) */
    RMP_Thd_Del(&Thd_1);

    /* Thd-crt: worker that delays instead of pending (Test_Set_2, case 0) */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, (void*)0, Stack_1, sizeof(Stack_1), PRIO_HIGHER, 5U);
    /* Sched-lock: simulate ISR context; the target is not pending */
    RMP_Sched_Lock();
    /* Sem-post-ISR: successful post when the thread is not pending */
    RMP_Sem_Post_ISR(&Sem_1, 1U);
    /* Sched-unlock: leave the simulated ISR context */
    RMP_Sched_Unlock();
    /* Thd-del: clean up the worker, removing it from the delay queue */
    RMP_Thd_Del(&Thd_1);
    /* Sem-del: drop the semaphore before the low-priority test */
    RMP_Sem_Del(&Sem_1);

    /* Sem-crt: recreate the semaphore for the low-priority wakeup test */
    RMP_Sem_Crt(&Sem_1, 0U);
    /* Thd-crt: worker at a lower priority than the test thread */
    RMP_Thd_Crt(&Thd_1, Test_Set_2, (void*)2, Stack_1, sizeof(Stack_1), PRIO_BOTTOM, 5U);
    /* Thd-delay: let the worker block on the semaphore first */
    RMP_Thd_Delay(10000U);
    /* Sched-lock: simulate ISR context; the woken thread cannot preempt us */
    RMP_Sched_Lock();
    /* Sem-post-ISR: success path where the woken thread has lower priority */
    RMP_Sem_Post_ISR(&Sem_1, 1U);
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

    /* Sem-bcst-ISR: force the semaphore back into the used state, then
     * force a pending context switch flag, yield branch */
    Sched_Pend_Bkp=RMP_Sched_Pend;
    RMP_Sched_Pend=1U;
    Sem_1.State=RMP_SEM_USED;
    RMP_Sem_Bcst_ISR(&Sem_1);
    RMP_Sched_Pend=Sched_Pend_Bkp;

    /* Sem-pend-unlock: NULL control block rejection under a single lock;
     * the call consumes the lock when it rejects */
    RMP_Sched_Lock();
    FAIL(RMP_Sem_Pend_Unlock(RMP_NULL,1));

    /* Sem-pend-unlock: scheduler locked twice, so the not-locked-once
     * rejection branch is taken; the call drops one lock before it
     * rejects, so we take one extra lock beforehand to balance it out */
    RMP_Sched_Lock();
    RMP_Sched_Lock();
    FAIL(RMP_Sem_Pend_Unlock(RMP_NULL,1));
    RMP_Sched_Unlock();

    /* Sem-pend-unlock: post a count, lock once, and let the pend
     * consume it immediately through the unlock path */
    RMP_Sem_Post(&Sem_1, 1U);
    RMP_Sched_Lock();
    PASS(RMP_Sem_Pend_Unlock(&Sem_1, 10));

    /* Sem-post-ISR: special case where the post count is zero */
    RMP_Sem_Post_ISR(&Sem_1,0U);

    /* Sem-post-ISR: force a pending context switch flag, yield branch */
    Sched_Pend_Bkp=RMP_Sched_Pend;
    RMP_Sched_Pend=1U;
    RMP_Sem_Post_ISR(&Sem_1,10U);
    RMP_Sched_Pend=Sched_Pend_Bkp;
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

/* Function:Cov_Mem_Free_Class ************************************************
Description : Free-list insertion scenarios for 64-byte blocks, which map to
              FLI = SLI = 0. First two slots are freed while isolated from
              each other by live blocks, so they cannot merge; then two
              adjacent slots are freed, which ensures Slot != Slot->Next
              inside the class list.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Mem_Free_Class(void)
{
    void* Slot_Isolated_First;
    void* Slot_Isolated_Second;
    void* Slot_Adjacent_First;
    void* Slot_Adjacent_Second;

    RMP_Clear(Pool,1024U*sizeof(rmp_ptr_t));
    RMP_Mem_Init(Pool,1024U*sizeof(rmp_ptr_t));
    /* Five 64-byte slots; free the second and the fifth, which are separated
     * by live blocks and therefore cannot be merged with anything */
    RMP_Malloc(Pool, 64U);
    Slot_Isolated_First=RMP_Malloc(Pool, 64U);
    RMP_Malloc(Pool, 64U);
    RMP_Malloc(Pool, 64U);
    Slot_Isolated_Second=RMP_Malloc(Pool, 64U);
    RMP_Malloc(Pool, 64U);
    RMP_Free(Pool,Slot_Isolated_First);
    RMP_Free(Pool,Slot_Isolated_Second);

    /* Six 64-byte slots; free the third and the fourth, which sit right next
     * to each other and therefore merge into one larger free block */
    RMP_Malloc(Pool, 64U);
    Slot_Adjacent_First=RMP_Malloc(Pool, 64U);
    Slot_Adjacent_Second=RMP_Malloc(Pool, 64U);
    RMP_Malloc(Pool, 64U);
    RMP_Malloc(Pool, 64U);
    RMP_Malloc(Pool, 64U);
    RMP_Free(Pool,Slot_Adjacent_First);
    RMP_Free(Pool,Slot_Adjacent_Second);
}
/* End Function:Cov_Mem_Free_Class *******************************************/

/* Function:Cov_Mem_Realloc_Expand ********************************************
Description : Expand an allocated base block by reallocating it to a larger
              size after the donor block right next to it has been freed.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Mem_Realloc_Expand(void)
{
    void* Block_Base;
    void* Block_Donor;

    RMP_Clear(Pool,1024U*4);
    RMP_Mem_Init(Pool,1024U*4);
    Block_Base=RMP_Malloc(Pool, 256U);
    Block_Donor=RMP_Malloc(Pool, 128U);
    RMP_Malloc(Pool, 64U);
    /* Free: the donor leaves free space right of the base block */
    RMP_Free(Pool,Block_Donor);
    RMP_Malloc(Pool, 256U);
    /* Realloc: the base block grows over the freed donor space */
    RMP_Realloc(Pool, Block_Base,512U);

}
/* End Function:Cov_Mem_Realloc_Expand ***************************************/

/* Function:Cov_Mem_Param_Reject **********************************************
Description : Parameter rejection branches of Malloc, Free and Mem_Init,
              including the double-free detection, followed by the Realloc
              edge cases: NULL pointer as a fresh allocation, NULL pool,
              out-of-range pointer, oversized request on an already freed
              block, zero size as a free, and expand-by-move on a live block.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Mem_Param_Reject(void)
{
    volatile rmp_ptr_t Data;
    volatile rmp_ptr_t Stub;

    /* Malloc: size too large for the pool, rejection path */
    RMP_Malloc(Pool, TEST_MEM_POOL*sizeof(rmp_ptr_t)*200);
    /* Free: NULL pointer rejection branch */
    RMP_Free(Pool,RMP_NULL);
    /* Free: pointer out of pool range rejection branch */
    RMP_Free(Pool,(void*)1);
    /* Free: NULL pool rejection branch */
    RMP_Free(RMP_NULL,(void*)100);
    /* Malloc: NULL pool rejection branch */
    RMP_Malloc(RMP_NULL,100U);
    /* Malloc: zero size rejection branch */
    RMP_Malloc(Pool,0U);
    /* Malloc: ordinary allocation to set up the double-free test */
    Data=(rmp_ptr_t)RMP_Malloc(Pool, 256U);
    /* Free: normal free of the block just allocated */
    RMP_Free(Pool,(void*)Data);
    /* Free: double free of the same block, rejection branch */
    RMP_Free(Pool,(void*)Data);
    /* Mem-init: NULL pool rejection branch */
    RMP_Mem_Init(RMP_NULL, TEST_MEM_POOL*sizeof(rmp_ptr_t));
    /* Mem-init: zero size rejection branch */
    RMP_Mem_Init(Pool, 0U);
    /* Mem-init: unaligned memory rejection branch */
    RMP_Mem_Init((void*)(((rmp_ptr_t)Pool)+1), TEST_MEM_POOL*sizeof(rmp_ptr_t));

    /* Realloc: NULL pointer behaves like a fresh allocation */
    RMP_Realloc(Pool,RMP_NULL,128U);
    /* Realloc: NULL pool rejection branch */
    RMP_Realloc(0, (void*)0, 100U);
    /* Realloc: pointer out of range rejection branch */
    RMP_Realloc(Pool, (void*)1, 100U);
    /* Realloc: size far beyond the pool capacity, rejection path
     * (the block itself is already freed at this point) */
    RMP_Realloc(Pool, (void*)Data, TEST_MEM_POOL*sizeof(rmp_ptr_t)/4*3-64);
    /* Realloc: zero size is equivalent to a free */
    RMP_Realloc(Pool, (void*)Data, 0U);
    /* Realloc: operating on a block that was already freed */
    RMP_Realloc(Pool, (void*)Data, 100U);
    /* Realloc: expand by moving the content to a new, larger block */
    Stub=(rmp_ptr_t)RMP_Malloc(Pool, 256U);
    Stub=(rmp_ptr_t)RMP_Realloc(Pool, (void*)Stub, 1024U);
    /* Free: release the stub block we just reallocated */
    RMP_Free(Pool, (void*)Stub);
}
/* End Function:Cov_Mem_Param_Reject *****************************************/

/* Function:Cov_Mem_Exhaust ***************************************************
Description : Drain the pool with 64-byte requests until it refuses, then
              try to reallocate the last good block into space that no
              longer exists.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Mem_Exhaust(void)
{
    volatile rmp_ptr_t Data;
    volatile rmp_ptr_t Last_Good;

    /* Malloc: request nearly the whole pool, found on the first FLI try */
    Data=(rmp_ptr_t)RMP_Malloc(Pool, TEST_MEM_POOL*sizeof(rmp_ptr_t)/5*4);

    /* Test residue smaller than 64 - keep allocating 64 byte until we run out
       of memory. Memory leaking is fine because this is just a test. */
    while(1)
    {
        Data=(rmp_ptr_t)RMP_Malloc(Pool, 64U);
        if(Data==0)
            break;
        /* Last good allocation stored here */
        Last_Good=Data;
    }
    /* Realloc: no space left in the pool, the move fails */
    RMP_Realloc(Pool,(void*)Last_Good,256U);
}
/* End Function:Cov_Mem_Exhaust **********************************************/

/* Function:Cov_Mem_Size_Class ************************************************
Description : Ordinary allocations across size classes, the minimum-size
              request, an exact FLI/SLI match, the 511/513 boundary
              conditions, an oversized rejection, and a refill after
              partial frees.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Mem_Size_Class(void)
{
    void* Slot_100;
    void* Slot_200_First;
    void* Slot_200_Second;
    void* Slot_200_Third;
    void* Slot_8000_First;
    void* Slot_6000;
    void* Slot_8000_Second;
    void* Slot_200_Fourth;
    void* Slot_Minimum;
    void* Slot_Exact;
    void* Slot_Below;
    void* Slot_Above;
    void* Slot_Oversize;
    void* Slot_Refill_128;
    void* Slot_Refill_256;
    void* Slot_Cross;

    /* Mem-init: aligned memory, the normal path */
    RMP_Mem_Init((void*)((rmp_ptr_t)Pool), TEST_MEM_POOL*sizeof(rmp_ptr_t));
    /* The else branch is taken because the current FLI/SLI slot already
       contains other blocks; the new block is simply added to the list without
       touching the bitmap state. Memory allocation itself does not lock the
       scheduler. */
    /* Malloc: ordinary allocations in various size classes */
    Slot_100=RMP_Malloc(Pool, 100U);
    Slot_200_First=RMP_Malloc(Pool, 200U);
    Slot_200_Second=RMP_Malloc(Pool, 200U);
    Slot_200_Third=RMP_Malloc(Pool, 200U);
    Slot_8000_First=RMP_Malloc(Pool, 8000U);
    Slot_6000=RMP_Malloc(Pool, 6000U);
    Slot_8000_Second=RMP_Malloc(Pool, 8000U);
    Slot_200_Fourth=RMP_Malloc(Pool, 200U);

    /* Minimum block allocation test */
    Slot_Minimum=RMP_Malloc(Pool, 65U);

    /* Exact FLI/SLI match test */
    Slot_Exact=RMP_Malloc(Pool, 512U);

    /* Boundary condition test */
    Slot_Below=RMP_Malloc(Pool, 511U);
    Slot_Above=RMP_Malloc(Pool, 513U);

    /* Large block allocation test - the pool cannot support such a request */
    Slot_Oversize=RMP_Malloc(Pool, TEST_MEM_POOL*100);

    /* Combined test - free a few blocks first */
    RMP_Free(Pool, Slot_Minimum);
    RMP_Free(Pool, Slot_Exact);

    /* Then allocate different sizes and observe the pool behavior */
    Slot_Refill_128=RMP_Malloc(Pool, 128U);
    Slot_Refill_256=RMP_Malloc(Pool, 256U);

    /* Make sure that during the memory search at least one iteration
       sees Mem->Bitmap[Word] == 0, i.e. no block is available at the current
       processor word level. This requires a scenario where some FLI level has
       no available blocks, and the search algorithm must skip this empty FLI
       level and continue with higher FLI levels. */
    while(RMP_Malloc(Pool, 256)!=NULL);

    /* Now attempt an allocation large enough that the algorithm must cross FLI levels */
    /* Malloc: size forces the search past the first FLI level */
    Slot_Cross=RMP_Malloc(Pool, TEST_MEM_POOL*2);

    /* Free the blocks allocated via RMP_Malloc */
    RMP_Free(Pool, Slot_100);
    RMP_Free(Pool, Slot_200_First);
    RMP_Free(Pool, Slot_200_Second);
    RMP_Free(Pool, Slot_200_Third);
    RMP_Free(Pool, Slot_8000_First);
    RMP_Free(Pool, Slot_6000);
    RMP_Free(Pool, Slot_8000_Second);
    RMP_Free(Pool, Slot_200_Fourth);

    /* Free the minimum block allocation test */
    RMP_Free(Pool, Slot_Minimum);

    /* Free the exact FLI/SLI match test */
    RMP_Free(Pool, Slot_Exact);

    /* Free the boundary condition test */
    RMP_Free(Pool, Slot_Below);
    RMP_Free(Pool, Slot_Above);

    /* Large block allocation test */
    /* Slot_Oversize may not have been allocated since the pool cannot support
       such a large request, so check it is not NULL before freeing */
    if(Slot_Oversize!=NULL)
    {
        RMP_Free(Pool, Slot_Oversize);
    }

    /* Free the combined test */
    RMP_Free(Pool, Slot_Refill_128);
    RMP_Free(Pool, Slot_Refill_256);

    /* Free the FLI-crossing allocation test */
    if(Slot_Cross!=NULL)
    {
        RMP_Free(Pool, Slot_Cross);
    }
}
/* End Function:Cov_Mem_Size_Class *******************************************/

/* Function:Cov_Mem_Del_Shared ************************************************
Description : Cover the last else branch of _RMP_Mem_Del: when a block is
              removed, other blocks still exist in its FLI/SLI class, so the
              class stays non-empty and the Slot == Slot->Next check fails.
              Two 128-byte blocks share one class; freeing the first leaves
              the second inside it.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Mem_Del_Shared(void)
{
    void* Slot_Same_Class_First;
    void* Slot_Same_Class_Second;

    /* Malloc: two blocks of the same size, they share one FLI/SLI class */
    Slot_Same_Class_First=RMP_Malloc(Pool, 128U);
    Slot_Same_Class_Second=RMP_Malloc(Pool, 128U);

    /* Free: both blocks reside in the same FLI/SLI class */
    RMP_Free(Pool, Slot_Same_Class_First);

    /* This should trigger the else branch of _RMP_Mem_Del, since the second
       block still resides in the same FLI/SLI class */

    /* Cleanup */
    /* Free: the second block empties the FLI/SLI class */
    RMP_Free(Pool, Slot_Same_Class_Second);

}
/* End Function:Cov_Mem_Del_Shared *******************************************/

/* Function:Cov_Mem_Realloc_Cases *********************************************
Description : A batch of realloc scenarios on the fully reinitialized pool:
              out-of-range and NULL-pool rejections, an in-place shrink,
              reallocating an already-freed block, growing a block leftwards
              over a freed left neighbor, expanding rightwards over a freed
              right neighbor, and a tight merge whose residual is too small
              to form a new block. The shrunk 16-byte block is deliberately
              kept alive until the end so the intermediate scenarios run
              against the exact pool layout they were designed for.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Mem_Realloc_Cases(void)
{
    void* Slot_Shrink;
    void* Slot_Freed;
    void* Slot_Reallocated;
    void* Block_Left_Guard;
    void* Block_Growing;
    void* Block_Right_Guard;
    void* Block_Base;
    void* Block_Right_Freed;
    void* Block_Expanded;
    void* Block_Initial;
    void* Block_Neighbor_Free;
    void* Block_Neighbor_Held;
    void* Block_Merged;

    /* Realloc: freed-and-recreated out-of-range pointer, residual paths */
    RMP_Realloc(Pool,(void*)1,0U);
    /* Realloc: NULL pool rejection */
    RMP_Realloc(RMP_NULL,(void*)1,64U);
    /* Malloc/Realloc: shrink a block in place, covering the shrink paths */
    Slot_Shrink=RMP_Malloc(Pool,16U);
    RMP_Realloc(Pool,Slot_Shrink,32U);

    /* Malloc: allocate a block and free it for the freed-realloc test */
    Slot_Freed=RMP_Malloc(Pool, 128U);
    /* Free: return the block to the pool */
    RMP_Free(Pool, Slot_Freed);
    /* Realloc: try to reallocate an already freed block */
    Slot_Reallocated=RMP_Realloc(Pool, Slot_Freed, 256U);
    /* Free: release the reallocated block */
    RMP_Free(Pool,Slot_Reallocated);

    /* If Right_Head's address differs from the end of the pool, there are more
       blocks after the current one and the block pointed to by Right_Head
       validly exists. Make sure the reallocated block is not the last block
       in the pool. */
    /* RMP_Mem_Init(Pool, 1024); */
    /* Malloc: three blocks so the reallocated one is not the last */
    Block_Left_Guard=RMP_Malloc(Pool, 100U);
    Block_Growing=RMP_Malloc(Pool, 200U);
    Block_Right_Guard=RMP_Malloc(Pool, 300U);
    /* Free a block and immediately try to reallocate, to trigger the else branch */
    /* Free: the left guard, so the growing block can expand leftwards */
    RMP_Free(Pool,Block_Left_Guard);
    /* Realloc: grow the middle block into the freed space left of it */
    Block_Growing=RMP_Realloc(Pool, Block_Growing, 250U);
    /* Cleanup - free all allocated blocks and destroy the pool */
    /* Free: the left guard (may already be merged if the realloc failed) */
    RMP_Free(Pool,Block_Left_Guard);
    /* Free: the resized middle block */
    RMP_Free(Pool,Block_Growing);
    /* Free: the right guard */
    RMP_Free(Pool,Block_Right_Guard);

    /* Expansion cases */
    /* Allocate the base block, which is about to be resized */
    Block_Base=RMP_Malloc(Pool, 128U);
    /* Allocate the second block right after, then free it immediately so it
       becomes the right free block */
    Block_Right_Freed=RMP_Malloc(Pool, 128U);
    /* Free: the second block becomes the right free neighbor */
    RMP_Free(Pool,Block_Right_Freed);

    /* Try to expand the base block, expecting the right free block handling
       logic to trigger */
    /* Realloc: expand the base block by merging with its right free neighbor */
    Block_Expanded=RMP_Realloc(Pool, Block_Base, 256U);
    /* Cleanup - free the block and destroy the pool */
    /* Free: the expanded block */
    RMP_Free(Pool,Block_Expanded);

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
    /* Allocate the base block from the pool; we will try to expand it later */
    Block_Initial=RMP_Malloc(Pool,64U);

    /* Allocate and free another block right after, to create a free block to
       the right of the base block */
    Block_Neighbor_Free=RMP_Malloc(Pool, 64U);
    Block_Neighbor_Held=RMP_Malloc(Pool, 128U);
    RMP_Free(Pool,Block_Neighbor_Free);
    /* Try to expand the base block, to trigger a merge with the right free block */
    /* Realloc: pick a size that ensures the merge leaves no room for a new block */
    Block_Merged=RMP_Realloc(Pool, Block_Initial, 110U);
    /* Cleanup - free the memory and destroy the pool */
    /* Free: the merged block */
    RMP_Free(Pool,Block_Merged);
    /* Free: the remaining neighbor block */
    RMP_Free(Pool,Block_Neighbor_Held);

    /* Free: release the 16-byte block kept alive since the in-place shrink */
    RMP_Free(Pool,Slot_Shrink);
}
/* End Function:Cov_Mem_Realloc_Cases ****************************************/

/* Function:Cov_Mem_Realloc_Residual ******************************************
Description : Five self-contained pool scenarios around the Res_Size logic
              of RMP_Realloc: the no-resize shortcut when Mem_Size equals
              Rounded_Size, a shrink that splits off a residual block, an
              expansion whose new block allocation fails, a shrink whose
              residual block is large enough to be reinserted, and a shrink
              where Res_Size is just above the minimum viable block size.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Mem_Realloc_Residual(void)
{
    void* Block_Small;
    void* Block_Target;

    /*
       Rounded_Size - the adjusted (rounded up) size.
       Mem_Size - the original size of the block being reallocated.
       Res_Size - used to compute whether, after expanding the block and
       possibly merging adjacent free blocks, enough residual space remains to
       form a new free block.
    */
    RMP_Clear(Pool,1024U*sizeof(rmp_ptr_t));

    /* Realloc where Mem_Size==Rounded_Size, the no-resize shortcut */
    RMP_Mem_Init(Pool,1024U*sizeof(rmp_ptr_t));
    Block_Small=RMP_Malloc(Pool, 64U);
    Block_Target=RMP_Malloc(Pool, 128U);
    RMP_Free(Pool,Block_Small);
    Block_Target=RMP_Realloc(Pool,Block_Target,128U);

    /* Realloc shrink that splits off a residual block */
    RMP_Clear(Pool,1024U*sizeof(rmp_ptr_t));
    RMP_Mem_Init(Pool,1024U*sizeof(rmp_ptr_t));
    Block_Small=RMP_Malloc(Pool, 64U);
    Block_Target=RMP_Malloc(Pool, 256U);
    RMP_Free(Pool,Block_Small);
    Block_Target=RMP_Realloc(Pool,Block_Target,128U);

    /* Realloc expansion where the new block allocation fails */
    RMP_Clear(Pool,1024U*sizeof(rmp_ptr_t));
    RMP_Mem_Init(Pool,1024U*sizeof(rmp_ptr_t));
    Block_Small=RMP_Malloc(Pool, 64U);
    RMP_Malloc(Pool, 256U);
    Block_Target=RMP_Realloc(Pool,Block_Small,1024U*sizeof(rmp_ptr_t)+10086);

    /* Realloc shrink where the residual block is large enough to be reinserted */
    RMP_Clear(Pool,1024U*sizeof(rmp_ptr_t));
    RMP_Mem_Init(Pool,1024U*sizeof(rmp_ptr_t));
    Block_Small=RMP_Malloc(Pool, 128U);
    RMP_Malloc(Pool, 256U);
    Block_Target=RMP_Realloc(Pool,Block_Small,64U);

    /* Res_Size = Mem_Size - Rounded_Size */
    /* if(Res_Size < (sizeof(struct RMP_Mem_Head)+64U+sizeof(struct RMP_Mem_Tail))) not satisfied */
    /* i.e. make Res_Size >= 84 */
    RMP_Clear(Pool,1024U*sizeof(rmp_ptr_t));
    RMP_Mem_Init(Pool,1024U*4);
    Block_Target=RMP_Malloc(Pool, 256U);
    RMP_Malloc(Pool, 256U);
    Block_Target=RMP_Realloc(Pool,Block_Target,64U);
}
/* End Function:Cov_Mem_Realloc_Residual *************************************/

/* Function:Cov_Mem ***********************************************************
Description : Test the memory pool operations, including initialization,
              allocation, free and reallocation, covering the underlying
              FLI/SLI bitmap and linked list paths. The scenarios are split
              into focused sub-tests; this function only sequences them.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Mem(void)
{
    /* In the RMP_Mem structure, Table is a pointer to an RMP_List array that
       serves as the allocation table of the memory pool. Its purpose is to keep
       track of the allocation state, i.e. the free block lists of every size
       class (defined by the First-Level Index (FLI) and the Second-Level Index
       (SLI)). Each list head represents a set of free blocks within a certain
       size range that are available for allocation. Specifically, every element
       of the Table array is an RMP_List structure acting as the head node of a
       doubly-linked list, and each head corresponds to one FLI/SLI combination,
       representing a group of free blocks of similar size. */
    Cov_Mem_Free_Class();
    Cov_Mem_Realloc_Expand();
    Cov_Mem_Param_Reject();
    Cov_Mem_Exhaust();
    Cov_Mem_Size_Class();
    Cov_Mem_Del_Shared();
    Cov_Mem_Realloc_Cases();
    Cov_Mem_Realloc_Residual();
}
/* End Function:Cov_Mem ******************************************************/

/* Function:Cov_Fifo **********************************************************
Description : Test the FIFO operations: creation, deletion, read, write,
              ISR-context write and count, including the state and NULL
              parameter rejection branches.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Fifo(void)
{
    struct RMP_List Node;
    volatile struct RMP_List* Receive;

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
    RMP_Fifo_Read(&Fifo_1,&Receive);
    RMP_Fifo_Del(&Fifo_1);

    /* Abnormal case: read from a FIFO in free state; expect RMP_ERR_FIFO */
    RMP_Fifo_Crt(&Fifo_1);
    Fifo_1.State=RMP_FIFO_FREE;
    RMP_Fifo_Read(&Fifo_1,&Receive);
    RMP_Fifo_Del(&Fifo_1);

    /* FIFO is empty */
    RMP_Fifo_Crt(&Fifo_1);
    RMP_Fifo_Read(&Fifo_1,&Receive);
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
}
/* End Function:Cov_Fifo *****************************************************/

/* Function:Cov_Msgq **********************************************************
Description : Test the message queue operations: creation, deletion, send,
              ISR-context send, receive with timeout and count, including
              the state, semaphore and NULL parameter rejection branches.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Msgq(void)
{
    struct RMP_List Node;
    volatile struct RMP_List* Receive;

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
    RMP_Msgq_Rcv(RMP_NULL,RMP_NULL,1U);
    /* Msgq-rcv: tests the NULL node rejection branch */
    RMP_Msgq_Rcv(&Msgq_1,RMP_NULL,1U);
    /* Msgq-rcv: normal receive path */
    RMP_Msgq_Rcv(&Msgq_1,&Receive,10U);
    /* Msgq-rcv: force free state so the in-use check fails */
    Msgq_1.State=RMP_SEM_FREE;
    RMP_Msgq_Rcv(&Msgq_1,&Receive,10U);
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
    RMP_Msgq_Rcv(&Msgq_1,&Receive,20U);
    RMP_Msgq_Del(&Msgq_1);

    /* if(RMP_Fifo_Read(&(Queue->Fifo), Node)<0) */
    RMP_Clear(&Msgq_1, sizeof(struct RMP_Msgq));
    RMP_Msgq_Crt(&Msgq_1);
    /* Sem-post-ISR: post one count so the pend succeeds and is not caught by the pend timeout check */
    RMP_Sem_Post_ISR(&Msgq_1.Sem, 1U);
    /* Msgq-rcv: force free FIFO so the internal read fails */
    Msgq_1.Fifo.State=RMP_FIFO_FREE;
    RMP_Msgq_Rcv(&Msgq_1,&Receive,100U);
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
/* End Function:Cov_Msgq *****************************************************/

/* Function:Cov_Bmq ***********************************************************
Description : Test the blocking message queue (Bmq) operations.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Bmq_Race_Func(void)
{
    volatile struct RMP_List* Node;

    /* Sched-lock: hold the scheduler across the wake, the drain and the
     * delete, so the woken main thread resumes into a deleted queue */
    RMP_Sched_Lock();
    /* Sem-post: wake the blocked main thread; it cannot run yet */
    RMP_Sem_Post(&Bmq_1.Sem, 1U);
    /* Fifo-read: drain the queued message directly, or the FIFO would
     * refuse the deletion below (it deletes only empty queues) */
    RMP_Fifo_Read(&Bmq_1.Msgq.Fifo, &Node);
    /* Bmq-del: delete the queue while the woken thread cannot run */
    RMP_Bmq_Del(&Bmq_1);
    /* Sched-unlock: let the woken main thread resume */
    RMP_Sched_Unlock();
    /* Thd-delay: park the thread in the delaying state */
    while(1)
    {
        RMP_Thd_Delay(10000U);
    }
}

void Cov_Bmq(void)
{
    struct RMP_List Node;
    volatile struct RMP_List* Receive;
    volatile struct RMP_List Race_Node;

    RMP_Clear(&Bmq_1, sizeof(struct RMP_Bmq));

    /* RMP_Bmq_Crt */
    /* Bmq-crt: tests the NULL queue rejection branch */
    RMP_Bmq_Crt(RMP_NULL,1U);
    /* Bmq-crt: normal creation path */
    RMP_Bmq_Crt(&Bmq_1,10U);
    /* Do not delete Bmq_1 here, so that the next creation sees a USED state */
    /* Bmq-crt: already-in-use rejection branch */
    RMP_Bmq_Crt(&Bmq_1,10U);
    RMP_Bmq_Del(&Bmq_1);
    /* Bmq-crt: zero limit rejection branch, expect RMP_ERR_OPER */
    RMP_Bmq_Crt(&Bmq_1,0U);

    /* RMP_Bmq_Del */
    /* Bmq-del: tests the NULL control block rejection branch */
    RMP_Bmq_Del(RMP_NULL);
    /* Bmq-del: queue is not in use, rejection branch */
    RMP_Bmq_Del(&Bmq_1);

    /* if(RMP_Msgq_Del(&(Queue->Msgq))<0) - Attempt to delete the message queue first */
    RMP_Bmq_Crt(&Bmq_1,10U);
    /* Bmq-del: abnormal case - force free state so the internal Msgq_Del fails */
    Bmq_1.Msgq.State=RMP_MSGQ_FREE;
    RMP_Bmq_Del(&Bmq_1);
    Bmq_1.Msgq.State=RMP_MSGQ_USED;
    /* Bmq-del: delete correctly this time */
    RMP_Bmq_Del(&Bmq_1);

    /* RMP_Bmq_Snd */
    RMP_Clear(&Bmq_1, sizeof(struct RMP_Bmq));
    /* Bmq-snd: tests the NULL queue rejection branch */
    RMP_Bmq_Snd(RMP_NULL,&Node,10U);
    /* Bmq-snd: tests the NULL node rejection branch */
    RMP_Bmq_Snd(&Bmq_1,RMP_NULL,10U);
    /* Bmq-snd: state != RMP_BMQ_USED rejection branch */
    RMP_Bmq_Snd(&Bmq_1,&Node,20U);
    RMP_Clear(&Bmq_1, sizeof(struct RMP_Bmq));
    RMP_Bmq_Crt(&Bmq_1,5U);
    /* Bmq-snd: semaphore state != RMP_SEM_USED, makes _RMP_Sem_Pend_Core
       return RMP_ERR_SEM, satisfying the 4885 condition */
    Bmq_1.Sem.State=RMP_SEM_FREE;
    RMP_Bmq_Snd(&Bmq_1,&Node,20U);
    /* Restore */
    Bmq_1.Sem.State=RMP_SEM_USED;
    /* Bmq-snd: normal send path */
    RMP_Bmq_Snd(&Bmq_1,&Node,20U);
    /* if(RMP_Msgq_Snd(&(Queue->Msgq), Node)<0) */
    /* Bmq-snd: abnormal case - RMP_Msgq_Snd returns RMP_ERR_MSGQ */
    Bmq_1.Msgq.State=RMP_MSGQ_FREE;
    RMP_Bmq_Snd(&Bmq_1,&Node,20U);
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
    RMP_Bmq_Crt(&Bmq_1,5U);
    /* Bmq-snd-ISR: abnormal case - Sem.Num_Cur is zero */
    Bmq_1.Sem.Num_Cur=0;
    RMP_Bmq_Snd_ISR(&Bmq_1,&Node);
    Bmq_1.Sem.Num_Cur=5;
    /* Bmq-snd-ISR: normal send path */
    RMP_Bmq_Snd_ISR(&Bmq_1,&Node);
    /* Bmq-del: delete correctly */
    RMP_Bmq_Del(&Bmq_1);

    /* RMP_Bmq_Rcv */
    RMP_Clear(&Bmq_1, sizeof(struct RMP_Bmq));
    /* Bmq-rcv: tests the NULL queue rejection branch */
    RMP_Bmq_Rcv(RMP_NULL,RMP_NULL,10U);
    /* Bmq-rcv: tests the NULL node rejection branch */
    RMP_Bmq_Rcv(&Bmq_1,RMP_NULL,10U);
    /* Bmq-rcv: state != RMP_BMQ_USED rejection branch */
    RMP_Bmq_Rcv(&Bmq_1,&Receive,10U);
    RMP_Bmq_Crt(&Bmq_1,10U);
    /* Bmq-rcv: receive from an empty queue, timeout path */
    RMP_Bmq_Rcv(&Bmq_1,&Receive,100U);
    /* Bmq-snd-ISR: queue three nodes so the next receive succeeds */
    RMP_Bmq_Snd_ISR(&Bmq_1,&Node);
    RMP_Bmq_Snd_ISR(&Bmq_1,&Node);
    RMP_Bmq_Snd_ISR(&Bmq_1,&Node);
    /* Bmq-rcv: normal receive path with data available */
    RMP_Bmq_Rcv(&Bmq_1,&Receive,100U);
    /* Bmq-del: delete correctly */
    RMP_Bmq_Del(&Bmq_1);

    /* RMP_Bmq_Cnt */
    /* Bmq-cnt: tests the NULL control block rejection branch */
    RMP_Bmq_Cnt(RMP_NULL);
    /* Bmq-cnt: query on a free queue, rejection branch */
    RMP_Bmq_Cnt(&Bmq_1);
    RMP_Bmq_Crt(&Bmq_1,10U);
    /* Bmq-cnt: query on a used queue, success path */
    RMP_Bmq_Cnt(&Bmq_1);
    /* Bmq-del: delete correctly */
    RMP_Bmq_Del(&Bmq_1);
    /* Bmq-cnt: query on a cleared structure, free-state branch */
    RMP_Clear(&Bmq_1, sizeof(struct RMP_Bmq));
    RMP_Bmq_Cnt(&Bmq_1);

    /* Deletion race: the sender resumes into a deleted queue */
    PASS(RMP_Bmq_Crt(&Bmq_1, 1U));
    /* Bmq-snd: fill the only slot */
    PASS(RMP_Bmq_Snd(&Bmq_1, &Race_Node, RMP_SLICE_MAX));
    /* Thd-crt: the racing deleter worker */
    PASS(RMP_Thd_Crt(&Thd_2, Cov_Bmq_Race_Func, RMP_NULL, Stack_2, sizeof(Stack_2), PRIO_LOWER, 10U));
    /* Bmq-snd: blocks on the empty semaphore. The worker wakes us by
     * posting a slot back, drains the queued message, and deletes the
     * queue, all under one scheduler lock, so we resume into the
     * deleted queue and the send fails through the deletion race
     * guard. No PASS here: the send is expected to return an error. */
    RMP_Bmq_Snd(&Bmq_1, &Race_Node, 100U);
    /* Thd-del: clean up the parked racing worker */
    PASS(RMP_Thd_Del(&Thd_2));
}
/* End Function:Cov_Bmq ******************************************************/

/* Function:Cov_Amgr **********************************************************
Description : Test the alarm manager and alarm operations.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Cov_Amgr_Hook(volatile struct RMP_Amgr* Amgr,
                   volatile struct RMP_Alrm* Alrm,
                   rmp_cnt_t Timestamp)
{
    RMP_USE(Amgr);
    RMP_USE(Alrm);
    RMP_USE(Timestamp);

    if(Alrm==&Alrm_1)
    {
        Alrm_1_Cnt++;
    }
    else
    {
        Alrm_2_Cnt++;
    }
}

void Cov_Amgr(void)
{
    volatile struct RMP_Alrm* Popped;
    /* A second manager used for the cross-mounted rejection tests */
    volatile struct RMP_Amgr Amgr_2;
    volatile rmp_ptr_t State_Bkp;

    /* Amgr-crt/cnt/del/proc/pop: NULL control block rejections */
    FAIL(RMP_Amgr_Crt(RMP_NULL));
    FAIL(RMP_Amgr_Cnt(RMP_NULL));
    FAIL(RMP_Amgr_Del(RMP_NULL));
    FAIL(RMP_Amgr_Proc(RMP_NULL, 1U));
    FAIL(RMP_Amgr_Pop(RMP_NULL, (volatile struct RMP_Alrm**)&Popped));

    /* Alrm-init: parameter rejections */
    FAIL(RMP_Alrm_Init(RMP_NULL, 1U, RMP_ALRM_AUTORLD, Cov_Amgr_Hook));
    FAIL(RMP_Alrm_Init(&Alrm_1, 0U, RMP_ALRM_AUTORLD, Cov_Amgr_Hook));
    FAIL(RMP_Alrm_Init(&Alrm_1, RMP_MASK_INTMAX+1U, RMP_ALRM_AUTORLD, Cov_Amgr_Hook));
    FAIL(RMP_Alrm_Init(&Alrm_1, 1U, 1U, Cov_Amgr_Hook));
    FAIL(RMP_Alrm_Init(&Alrm_1, 1U, RMP_ALRM_AUTORLD, RMP_NULL));

    /* Alrm-set/clr/trg: NULL pointer rejections */
    FAIL(RMP_Alrm_Set(RMP_NULL, &Alrm_1));
    FAIL(RMP_Alrm_Set(&Amgr_1, RMP_NULL));
    FAIL(RMP_Alrm_Clr(RMP_NULL, &Alrm_1));
    FAIL(RMP_Alrm_Clr(&Amgr_1, RMP_NULL));
    FAIL(RMP_Alrm_Trg(RMP_NULL, &Alrm_1));
    FAIL(RMP_Alrm_Trg(&Amgr_1, RMP_NULL));

    /* Amgr-crt: success, then already-created rejection */
    PASS(RMP_Amgr_Crt(&Amgr_1));
    FAIL(RMP_Amgr_Crt(&Amgr_1));
    /* Amgr-cnt: empty manager count */
    PASS(RMP_Amgr_Cnt(&Amgr_1));
    /* Amgr-proc: zero and oversized tick rejections */
    FAIL(RMP_Amgr_Proc(&Amgr_1, 0U));
    FAIL(RMP_Amgr_Proc(&Amgr_1, RMP_MASK_INTMAX+1U));

    /* Amgr-proc/set/clr/trg: broken mutex, the pend must fail */
    State_Bkp=Amgr_1.Mutex.State;
    Amgr_1.Mutex.State=RMP_SEM_FREE;
    FAIL(RMP_Amgr_Proc(&Amgr_1, 1U));
    FAIL(RMP_Alrm_Set(&Amgr_1, &Alrm_1));
    FAIL(RMP_Alrm_Clr(&Amgr_1, &Alrm_1));
    FAIL(RMP_Alrm_Trg(&Amgr_1, &Alrm_1));
    Amgr_1.Mutex.State=State_Bkp;

    /* Alrm-init: a long one-shot, a short autoreloading and a medium
     * one-shot alarm. The mixed delays exercise both paths of the
     * insertion traversal: inserting the short autoreloader behind the
     * long one-shot walks to the list head (no early pivot), while
     * inserting the medium one-shot behind the short autoreloader takes
     * the early-break path.
     * Historically the one-shot flag was RMP_POW2(RMP_WORD_ORDER-1),
     * which overlapped the delay field: an autoreloading alarm with a
     * delay carrying that bit (e.g. 50) silently behaved as one-shot.
     * The flag now lives on the top bit of the word, so every delay
     * is safe. */
    PASS(RMP_Alrm_Init(&Alrm_1, 100U, RMP_ALRM_ONESHOT, Cov_Amgr_Hook));
    PASS(RMP_Alrm_Init(&Alrm_2, 32U, RMP_ALRM_AUTORLD, Cov_Amgr_Hook));
    PASS(RMP_Alrm_Init(&Alrm_3, 64U, RMP_ALRM_ONESHOT, Cov_Amgr_Hook));

    /* Alrm-set: register the short alarm into the empty queue */
    PASS(RMP_Alrm_Set(&Amgr_1, &Alrm_1));
    /* Alrm-init: already-mounted alarm rejection */
    FAIL(RMP_Alrm_Init(&Alrm_1, 2U, RMP_ALRM_AUTORLD, Cov_Amgr_Hook));
    /* Alrm-set: inserting the long alarm behind the short one takes
     * the early-break path of the insertion traversal */
    PASS(RMP_Alrm_Set(&Amgr_1, &Alrm_2));
    /* Alrm-set: re-registering a mounted alarm pulls and reinserts it */
    PASS(RMP_Alrm_Set(&Amgr_1, &Alrm_2));
    /* Alrm-set: register the slow one-shot alarm at the queue tail */
    PASS(RMP_Alrm_Set(&Amgr_1, &Alrm_3));
    /* Amgr-cnt: three alarms are now managed */
    RMP_Amgr_Cnt(&Amgr_1);
    /* Amgr-del: non-empty manager rejection */
    FAIL(RMP_Amgr_Del(&Amgr_1));

    /* Alrm-set/clr/trg: cross-mounted rejections through a second manager */
    RMP_Clear(&Amgr_2, sizeof(Amgr_2));
    PASS(RMP_Amgr_Crt(&Amgr_2));
    FAIL(RMP_Alrm_Set(&Amgr_2, &Alrm_1));
    FAIL(RMP_Alrm_Clr(&Amgr_2, &Alrm_1));
    FAIL(RMP_Alrm_Trg(&Amgr_2, &Alrm_1));
    PASS(RMP_Amgr_Del(&Amgr_2));

    /* Alrm-clr: unregister the slow one-shot alarm - the success path.
     * The manager backlink is cleared here, so the alarm object becomes
     * recyclable again. */
    PASS(RMP_Alrm_Clr(&Amgr_1, &Alrm_3));
    RMP_Amgr_Cnt(&Amgr_1);
    /* Alrm-clr: re-cancelling a cancelled alarm is rejected */
    FAIL(RMP_Alrm_Clr(&Amgr_1, &Alrm_3));
    /* Alrm-init: a cancelled alarm can be initialized again for reuse */
    PASS(RMP_Alrm_Init(&Alrm_3, 2U, RMP_ALRM_ONESHOT, Cov_Amgr_Hook));

    /* Alrm-trg: trigger the short one-shot alarm */
    PASS(RMP_Alrm_Trg(&Amgr_1, &Alrm_1));
    /* Alrm-trg: the one-shot alarm was freed by its expiration, rejection */
    FAIL(RMP_Alrm_Trg(&Amgr_1, &Alrm_1));
    /* Alrm-trg: trigger the long autoreloading alarm */
    PASS(RMP_Alrm_Trg(&Amgr_1, &Alrm_2));

    /* Amgr-proc: advance the manager timestamps; a few increments let
     * nothing expire, then a big one pushes past the 32-tick autoreloading
     * alarm so it expires and re-arms itself through the tick handler */
    RMP_Amgr_Proc(&Amgr_1, 1U);
    RMP_Amgr_Proc(&Amgr_1, 1U);
    RMP_Amgr_Proc(&Amgr_1, 2U);
    FAIL(RMP_Amgr_Proc(&Amgr_1, 0U));
    RMP_Amgr_Proc(&Amgr_1, 60U);
    RMP_Amgr_Proc(&Amgr_1, 1U);

    /* Amgr-pop: dump the alarm that the autoreloading hook re-armed.
     * The popped alarms are NOT put back, or this would loop forever. */
    PASS(RMP_Amgr_Pop(&Amgr_1, (volatile struct RMP_Alrm**)&Popped));
    /* Amgr-pop: nothing left to dump, empty-queue rejection branch */
    FAIL(RMP_Amgr_Pop(&Amgr_1, (volatile struct RMP_Alrm**)&Popped));
    /* Amgr-pop: NULL output pointer rejection branch */
    FAIL(RMP_Amgr_Pop(&Amgr_1, (volatile struct RMP_Alrm**)RMP_NULL));

    /* Alrm-clr: both alarms are unregistered now, rejection branches */
    FAIL(RMP_Alrm_Clr(&Amgr_1, &Alrm_1));
    FAIL(RMP_Alrm_Clr(&Amgr_1, &Alrm_2));
    /* Amgr-del: empty manager success, then already-deleted rejections */
    PASS(RMP_Amgr_Del(&Amgr_1));
    FAIL(RMP_Amgr_Del(&Amgr_1));
    FAIL(RMP_Amgr_Cnt(&Amgr_1));
    FAIL(RMP_Amgr_Proc(&Amgr_1, 1U));
    FAIL(RMP_Amgr_Pop(&Amgr_1, (volatile struct RMP_Alrm**)&Popped));
    FAIL(RMP_Alrm_Set(&Amgr_1, &Alrm_1));
    FAIL(RMP_Alrm_Clr(&Amgr_1, &Alrm_1));
    FAIL(RMP_Alrm_Trg(&Amgr_1, &Alrm_1));
}
/* End Function:Cov_Amgr *****************************************************/

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

    /* Run every coverage test defined in this file, in definition order.
     * A line naming the next test is printed before it runs, so that the
     * position of any kernel panic can be attributed to a test. */
    /* Bit manipulation tests */
    RMP_DBG_S("=== Bit manipulation ===\r\n");
    Cov_Bit();
    /* Print helper tests */
    RMP_DBG_S("=== Print helpers ===\r\n");
    Cov_Print();
    /* Scheduler lock tests */
    RMP_DBG_S("=== Scheduler lock ===\r\n");
    Cov_Lock();
    /* Timer helper tests */
    RMP_DBG_S("=== Timer helpers ===\r\n");
    Cov_Tim();
    /* Thread parameter tests */
    RMP_DBG_S("=== Thread parameters ===\r\n");
    Cov_Thd_Param();
    /* Thread operation tests */
    RMP_DBG_S("=== Thread operations ===\r\n");
    Cov_Thd_Oper();
    /* Thread deletion tests */
    RMP_DBG_S("=== Thread deletion ===\r\n");
    Cov_Thd_Del();
    /* Semaphore parameter tests */
    RMP_DBG_S("=== Semaphore parameters ===\r\n");
    Cov_Sem_Param();
    /* Semaphore operation tests */
    RMP_DBG_S("=== Semaphore operations ===\r\n");
    Cov_Sem_Oper();
    /* Memory pool tests */
    RMP_DBG_S("=== Memory pool ===\r\n");
    Cov_Mem();
    /* FIFO tests */
    RMP_DBG_S("=== FIFO ===\r\n");
    Cov_Fifo();
    /* Message queue tests */
    RMP_DBG_S("=== Message queue ===\r\n");
    Cov_Msgq();
    /* Blocking message queue tests */
    RMP_DBG_S("=== Blocking message queue ===\r\n");
    Cov_Bmq();
    /* Alarm manager and alarm tests */
    RMP_DBG_S("=== Alarm manager ===\r\n");
    Cov_Amgr();

    /* Print all covered line numbers to the serial port */
    RMP_Cov_Print();

    /* Should stop here; keep delaying so that lower priority worker
     * threads can finish their panic prints, if any */
    while(1)
    {
        RMP_Thd_Delay(100000U);
    }
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
/* End Function:RMP_Init_Hook ************************************************/

/* Function:RMP_Init_Idle *****************************************************
Description : The idle hook of the init thread. The coverage test has nothing
              to do here.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Init_Idle(void)
{
    return;
}
/* End Function:RMP_Init_Idle ************************************************/

/* End Of File ***************************************************************/
