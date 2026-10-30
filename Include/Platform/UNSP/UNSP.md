  -----------------------------------------------------------------
                Sunplus u'nSP Commemorative Port
          The Golden Age of Microcontroller Innovation
  -----------------------------------------------------------------
  This port supports both unSP 1.2 and 2.0 (2.0 has 8 extra regs).
  The only chip that this port plans to test is the SPCE061A, an
  obsolete microcontroller at the heart of the Sunplus University
  Program that vanished decades ago. The chip never had a RTOS port,
  at least not a pronounced one, during its entire product lifetime.
  This port is provided as a monument for the golden age of MCU
  innovation circa 1980-2005, at which time multiple architectures
  rapidly emerged and strived to become popular on the market. Their
  efforts included university programs, competition sponsorships,
  free training sessions and free hardware giveouts. Yet, very few
  were successful (and lucky) enough to survive until today.
  -----------------------------------------------------------------
  Sunplus was a company that focused on MCUs & MPUs, and already had
  streaks of successes in arcade consoles, audio toys and electronic
  dictionaries circa 2k. Many arcade consoles were powered by their
  devices and you can find corresponding simulators even in the MAME;
  they even had an in-house S+ core that could boot the Linux kernel.
  The company then went through a series of organizational changes,
  and its departments were detached subsequently circa 2010 to operate
  as subsidiaries or even independently. The result was a Sunplus that
  concentrated on silicon IP licensing & car HMI and a list of smaller
  companies that focused on their dedicated niche markets. Today, the
  MCU department of the original Sunplus lives under the name of
  Generalplus with moderate financial success in consumer electronics,
  and is still producing legacy unSP (and unSP 2.0) MCUs that this
  port can theoretically support.
  -----------------------------------------------------------------
  In hindsight, the Sunplus unSP enjoyed limited success due to:
  1. Lack of market diversity. The only market which unSP targeted
     is audio toys. Audio toy makers are typically big companies:
     their large-scale chip procurements generate huge revenue, but
     they are unwilling to build an open community of makers. As
     such, Sunplus failed to entertain Key Opinion Customers (KOCs)
     (i.e. makers, development kit builders, small companies, BBS 
     moderators/participants) that run negligible volumes but have
     a huge ecosystem influence. This might be the major pitfall
     that led to the ultimate failure in unSP promotion campaign.
     While it's not wrong to focus on big customers, it is vital
     to notice that their procurement choices also depends on the
     available ecosystem rather than just price or technology.
  2. Esoteric architecture design. The char type on unSP is 16-bits
     instead of 8; while it is somewhat reasonable to use the DSP
     nature of the chip as a scapegoat (many DSPs were designed in
     that way, i.e. C28x), the design exposes the monstrosity of C
     implementation-defined behaviors to users. Many users will be
     caught in surprise when their old code refuse to work, or take
     up twice as much as memory. Additionally, the architecture is
     also too powerful in terms of addressing and too weak in terms
     of code density, which is not a great fit for low-cost MCUs.
     The boasted "49MHz" does not translate directly to MIPS, as
     all instructions are multi-cycle. A data transfer can take up
     to 9 cycles; function call/returns are some 12-13 cycles; the
     vector multiply-add DSP instruction emphasized in marketing
     flyers takes 10N+8 cycles, where N is the vector length. The
     overall effect is a CPU that is not faster than 8-bitters (for
     example, vs. AVR), not to mention true DSP architectures (like
     C28X or BlackFin). Building a pipelined CPU is definitely not
     rocket science, yet Sunplus simply went sideways.
  3. Lack of product lineup. The only publicly available chip for a
     long time is the SPCE061A, which comes in PLCC84 and LQFP80.
     Smaller packages are lackluster, not to even mention RAM/ROM
     portforlio diversity. The chip's I/O peripherals are either
     insufficient or useless for general-purpose control, and too
     many competitors can provide better products. The peripherals
     are also very inflexible, for example, you can only choose
     Fosc/2 or Fosc/256 for timer clock; no middleground exists.
     To make things worse, some peripherals even exhibit silicon
     bugs when pushed to a high but still in-datasheet-spec Fosc.
  4. Lack of publicly available documentation. Except for SPCE061A, 
     Sunplus user manuals are otherwise confidential and difficult
     to acquire without an NDA, which is quite unusual for MCU-class
     products. This makes them essentially impossible to use in 
     startups that struggle to become NDA eligible. Like many ASSP
     providers at that time, Sunplus would (probably) just shun you
     off if your volume is less than 1k/month, and even that amount
     generates negligible revenue for Sunplus. While it may seem
     reasonable to cater big customers only, it is important to see
     that all companies start small and will grow bigger overtime.
     When all open market "small" customers are captured by your
     competitors, it's time for your big customers to turn to your
     competitors; and when they finally turn away, you're probably
     beyond salvation.
  This is not to say that an architecture that failed on the open
  market cannot be financially successful on hidden markets. It is
  however, much much more difficult to be so, and you'd have to
  succumb to (potentially intensive, disruptive, irrational and
  almost always confidential) product customization requests to win
  a big customer. When this is the case, (1) you won't gain much
  goodwill by doing OEM/ODM, (2) you can't sell your products at a
  premium, (3) deep customization will drive you even farther away
  from the open market, and these effects will make you even more
  ill-prepared to win an open battle. Even if you'd overcome them,
  (1) your big customers will consistently try finding open market
  second sources just to prevent you from ransoming them, and (2)
  any sufficiently big customer can and will develop some in-house
  replacements to bring production costs down.
  -----------------------------------------------------------------
  As a retro port, this targets the unSPIDE V1.8.4 with GCC 1.0.10,
  (based on vanilla GCC 2.95.2, open sourced nearly a decade after
  the binary release) released 31/10/2003. It is extracted from the
  "61 board" (version A, self hand soldering required) accessory CD,
  and is the exact one that Sunplus used for its university program.
  Like C28x, char is 16 bits on this machine. This port is somewhat
  usable as the benchmark takes about ~35% of ROM. As the chip was
  intended for audio applications, the benchmark will finish with
  an endless original Sunplus University Program CD BGM playback.
  You will be able to hear the sound from decades ago if you happen
  to have an intact "61 board", a parallel programmer, and a parallel
  port-capable vintage computer (or parallel-port-capable virtual
  machine) running WinXP at hand.