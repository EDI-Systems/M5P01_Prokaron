(*Functions that contain Sched_Pend:

Setters:
_RMP_Run_Ins()
_RMP_Run_Del()
RMP_Yield
Sched_Pend=1.

Resetters:
main
_RMP_Run_High
Sched_Pend=0.

Yield trigger for all thread system calls:
RMP_Sched_Unlock（）
{
    if(Sched_Pend!=0)
        _RMP_Yield();
}

ISRs that are self-sufficient:
RMP_Thd_Snd_ISR
RMP_Sem_Post_ISR
RMP_Sem_Bcst_ISR
_RMP_Tim_Handler（）
{
   if（Pend_Possible）
    {
        Sched_Pend=1...


        if(Sched_Pend!=0)
            _RMP_Yield();
    }
}

Assertion checks to guarantee a state:
RMP_Thd_Del
RMP_Thd_Suspend
RMP_ASSERT((Thread!=RMP_Thd_Cur)||(RMP_Sched_Pend!=0U));
RMP_ASSERT((Thread!=RMP_Thd_Cur)||(RMP_Sched_Pend!=0U));

Invariant: any Sched_Pend=1 will be followed by a _RMP_Yield before returning to thread code.
Suspend is a horizontal state that masks on _RMP_Run_Ins or _RMP_Run_Del.

Further refactoring of code may be needed to layerize them so we can push by refinement.
*)