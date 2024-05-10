(*Goals of the scheduler proof:
  1. given the state transition diagram, there will not be transitions to undefined states.
  2. at any time, the thread being scheduled belong to the highest priority class.
  3. a RUNNING thread will be scheduled in bounded time given sufficient execution time at its priority level.
  4. all scheduler activations will be done in a bounded time, that is, w/WCET.
  5. the semantics of stub-based and vector-based yields are essentially the same and is a precise simulation of the true_yield_and_enable_interrupt specification.
  6. all system calls in normal or interrupt context that have RMP_Sched_Pend set never carry this flag to normal execution, that is, will request a context switch before returning to thread code execution.
  *)

(*
  
  5. Yield semantics:
  For normal system calls, the scheduling point is at (or within) the call of RMP_Sched_Unlock.
  For interrupt system calls or the timer handler, the scheduling point is always at the return of it.
  
  In normal system calls, the RMP_Sched_Unlock could behave as:
  (1) yield_and_enable_interrupts -> enable_interrupts (regular architecture)
  (2) enable_interrupts -> yield (Cortex-M like PendSV interrupt architecture)
  
  In interrupt system calls, the scheduling point could behave as:
  (1) yield_and_enable_interrupts (regular architecture)
  (2) enable_interrupts -> yield (Cortex-M like PendSV interrupt architecture)
  
  For normal system calls, we see that enable_interrupts is idempotent and now for both normal and interrupt system calls the situation is the same.
  
  For all system calls, the scheduling point could behave as:
  (1) yield_and_enable_interrupts (regular architecture)
  (2) enable_interrupts -> yield (Cortex-M like PendSV interrupt architecture)
  
  The problem here is something could go between enable_interrupts and yield in (2).
  However, if we set the PendSV to be the highest (nonpreemptive) priority amongst all interrupts, then the enable_interrupts -> yield is equivalent to yield_and_enable_interrupts.
  Hence we have proved that the yield implementations does not change its semantics, and such semantics is applied to all context switch points.

    Functions that contain RMP_Sched_Pend:

    Setters in the system:
    _RMP_Run_Ins()
    _RMP_Run_Del()
    RMP_Yield()
    {
        RMP_Sched_Pend=1;
    }

    Resetter in the system:
    main()
    _RMP_Run_High()
    {
        RMP_Sched_Pend=0;
    }

    Yield trigger at the end of all thread system calls:
    RMP_Sched_Unlock()
    {
        if(RMP_Sched_Pend!=0)
            _RMP_Yield();
            
        ENABLE_INTERRUPT.
    }

    ISRs that are self-sufficient:
    RMP_Thd_Snd_ISR()
    RMP_Sem_Post_ISR()
    RMP_Sem_Bcst_ISR()
    _RMP_Tim_Handler()
    {
        if(a pend is possible on the execution path)
        {
            RMP_Sched_Pend=1;


            if(RMP_Sched_Pend!=0)
                _RMP_Yield();
        }
    }

    Assertion checks to guarantee a state:
    RMP_Thd_Del()
    RMP_Thd_Suspend()
    {
        RMP_ASSERT((Thread!=RMP_Thd_Cur)||(RMP_Sched_Pend!=0U));
    }

    Suspend is a horizontal state that masks on _RMP_Run_Ins or _RMP_Run_Del.
    Further refactoring of code may be needed to layerize them so we can push by refinement.

*)