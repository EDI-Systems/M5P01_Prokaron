(* Invariants on IPC: 
Prove: 

1. A thread cannot be on waiting list if it is not in a delayed status.
2. In the thread mailbox, a thread cannot be waiting on the waiting list while being empty.

The recent mailbox bug indicate that there might be more of such bugs before we do a full verification. Turns out whitebox testing is insufficient, which is just expected. Testing only says that you didn't find any bug, but it doesn't guarantee that there is no bug.

1. The issues that needs to be solved w.r.t. the IPC designs are:
   1. what are the dependencies of the IPC implementations? Currently the Thread Mailbox has nobody depending on it, while the Semaphore is used as a base for everything.
   2. The semaphore state machine is easier to prove than the thread mailbox machine. But again, what needs to go into the specifications?
      Reachability (valid states) and unreachability (invalid states)?
      For systems, unreachability is more important for reachability, because reachability ultimately depends on the application programmer's intention. The proof just need to say, for example, whatever the programmers do, certain bad things won't happen. The key here is thus (1) what counts as bad (the mailbox bug certainly counts as one; this is very ez), and (2) assume all sequences of operations, the state is unreachable, and (3) supply these guarantees in a way that the programmers can leverage to prove properties about their respective systems.
   3. The delayed flag seem to be interposed onto the basic state.
      How's the RCVDLY different from SEMDLY?
      It would seem that the only difference being what is waited on.
      How's the RCVDLY different from RCVBLK?
      It would seem that the only difference being timeout calculations.
      Can we prove certain properties about delays and reuse them in all the proofs?
   4. How're we modeling clocks? 
      Clock equivalence - Doesn't really matter at what time things happen, but their orders were important.
      The only that might involve
      
For example, start from semaphores, we have in natural language

   1. Send operations:
      - Receiver==0
        - Semaphore++, return SUCCESS.
      - Receiver!=0
        - Semaphore++, UNBLOCK earliest receiver, return SUCCESS.
      - Additional goal: multi-send is equivalent to multiple single sends.
   2. Rcv operations:
      - Semaphore==0
        - Timeout==0
          - return FAILED.
        - Timeout!=0
          - Timeout==FINITE
              - Timeout not arrived
                - No send operation
                  - BLOCK receiver.
                - Send operation
                  - UNBLOCK receiver, Semaphore--, return SUCCESS.
              - Timeout arrived
                - UNBLOCK receiver, return FAILED.
          - Timeout==INFINITE
            - No send operation
              - BLOCK receiver.
            - Send operation
              - UNBLOCK receiver, Semaphore--, return SUCCESS.
      - Semaphore!=0
        - Semaphore--, return SUCCESS.

    Prove:
    (1) Send operations will eventually RETURN.
    (2) Recv operations that specify a FINITE timeout will eventually RETURN.
    (3) Recv operations will succeed iff it "got" a semaphore. Define "got" -> All SUCCESS RETURNs are preceded by Semaphore-- and all FAILED RETURNs must not be preceded by that.
    
    Should we ever verify with the threads, or not?
    
    Verifying a finite number of threads is quite easy. What about an infinite number of threads?
    1. Prove that the property holds for two threads, one sender, one receiver.
    2. Assume that the property holds for N threads.
    3. Prove that the property holds for N+1 threads.

    Informal proof:
    (1) Send operations will eventually RETURN.
        Given that the processor will proceed until it BLOCKs, the Send operation have no blockpoints, and the RETURN state with SUCCESS is guaranteed to be reachable, no matter how many threads are there.
    (2) Recv operations that specify a FINITE timeout will eventually RETURN. <- This proof thinks that no threads will ever supply some infinite timeout. When there could be, it can get even more complicated.
        (i) Given 1 sender, 1 receiver, and the receiver just specified a FINITE timeout wait.
            - There are semaphores. The receiver will return success immediately.
            - There are no semaphores. The receiver will block.
              - The sender sends within timeout. The receiver unblocks will success as always, total wait time < TIMEOUT, FINITE.
              - The sender does not send within timeout. The receiver unblocks with FAIL, total wait time = timeout, FINITE.
            Qed.
        (ii) Assume forall M,N, given M senders and N receivers on the same semaphore with all sequences is FINITE, we try to prove that (iii-1) and (iii-2) holds:
        (iii-1) given some fixed M, N+1 (it won't hurt to assume that the new thread will receive at last, so waked at last), the FINITE property holds. <- Does this generate all sequences in the universe? It does... but this is informal thinking.
            - It is either
              - The additional thread will be unblocked by a send before its timeout, FINITE, or
              - The additional thread will be unblocked with a timeout, FINITE.
            - and the timeout/receive exiting behavior of whatever threads before it does not change at all (this thread if woken up, must be the last one).
            Qed.
        (iii-2) given some fixed N, given M+1 (it won't hurt to assume that the new thread will send at last), the FINITE property holds.
            - It is either
              - The new sender sends at last.
                - May unblock any receiver.
                  - No receiver was unblocked by this send. This send can't do harm then, so still FINITE.
                  - Next receiver was unblocked by this send, but by TIMEOUT. Then it stays the same, so still FINITE.
                - and the timeout/receive exiting behavior of whatever threads before and after that next thread does not change at all.
                Qed.
              - The new sender did not send.
                - The receivers will be unblocked by just what they were unblocked with, no timeout/receive exiting behavior ever changes at all, so still FINITE.
                Qed.
            Qed.
         Given (i),(ii),(iii-1),(iii-2),
         Qed.
    (3) All RETURN states are preceded by Semaphore--. Hence this is ok.
*)