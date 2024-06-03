(* Invariant on IPC: 
Prove: 

1. A thread cannot be on waiting list if it is not in a delayed status.
2. In the thread mailbox, a thread cannot be waiting on the waiting list while being empty.

There might be more of such bugs before we do a full verification. Turns out whitebox testing is insufficient.

*)