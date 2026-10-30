  -----------------------------------------------------------------
                      MIPS Commemorative Port
      Those Who do not Learn History are Doomed to Repeat It
  -----------------------------------------------------------------
  This monumental port supports MIPS 32 (Microprocessor without
  Interlocked Pipelined Stages) version I-V, with DSPASE and
  FR32/FR64, while assuming a physical memory model. This port
  respects all of the load, FPU move and branch delay slots and thus
  will be compatible with a wide range of MIPS processors. FPU
  instructions are coded in words to be compatible with big-endian
  variants as well.
  All threads in the system must use the same FPU model; we do not
  support cases where a thread uses FR32 while the other uses FR64.
  While it is possible to support such behavior for compatibility
  with existing binaries, doing so will complicate the code too much
  and compromise WCET.
  The primary MIPS chips nowadays are the PIC32, so this is only
  tested with them. And even MIPS PIC32s are going away; other MIPS
  chips do not belong to microcontroller class and are hence not
  supported here. As a port dedicated to the funeral of MIPS, no
  further platforms except for Sony PLAYSTATION 1 will be added to
  this port.
  -----------------------------------------------------------------
  MIPS in terms of almost anything is pretty dead by now mainly due
  to its fragmented ecosystem. Different from other licensors that
  restrict or even disallow modifications to the ISA, MIPS actively
  encouraged such customizations without due administration. As a 
  general-purpose processor, such customizations probably traded too
  much compatibility for performance; and the performance is nowhere
  near that of DSPs or ASSPs where full customizations do not hurt
  because this their confidential binary interfaces do not need to
  remain compatible over generations. On 8 March 2021, the MIPS
  company officialy announced their full transition to RISC-V,
  finally putting this 38-year-old plagued architecture to its
  eternal rest.
  -----------------------------------------------------------------
  Another reason for the MIPS failure is its marketing strategy
  which lacks concentration. MIPS was never (really) popular in the
  desktop arena because it was expensive, and was never (really)
  popular in the embedded arena because it cost too much silicon
  area (L1I$) and power. They do, however, enjoy limited success in 
  telecommunication ASSPs, computer peripherals, video game consoles
  industry control applications, and notably even supercomputers,
  and these are sufficient to declare a moderate financial success.
  -----------------------------------------------------------------
  However, it does not seem that the RISC-V community is learning
  much from the MIPS failure. Incompatibilities in basic processor
  behaviors, i.e. exception handling, interrupt controller, memory
  protection units, compressed instruction forms have already taken
  off. The only lesson we learn from history is that no lesson will
  be ever learnt; good luck with that, and long live RISC-V!
  -----------------------------------------------------------------
  'nuff said, for language/ISA/API standards, we are basically
  PAYING the standard vendor to restrict our FREEDOM TO diverge so
  we can have FREEDOM FROM a fragmented ecosystem. As a matter of
  fact, a more generalized form of this paradox exists in ubiquity:
  a bad standard is better than no standard at all.