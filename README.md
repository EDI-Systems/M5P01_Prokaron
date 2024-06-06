<h1 align="center">
	<img width="300" src="/Document/Public/Demo/logo.png" alt="logo">
</h1>

# RMP Real-time Kernel
<div align="center">

[![Github release](https://img.shields.io/github/release/EDI-Systems/M5P01_Prokaron.svg)](https://github.com/EDI-Systems/M5P01_Prokaron/releases/latest)
[![Github commits](https://img.shields.io/github/commits-since/EDI-Systems/M5P01_Prokaron/main@{30day}.svg)](https://github.com/EDI-Systems/M5P01_Prokaron/compare/main@{30day}...main)
[![Discord](https://img.shields.io/badge/chat-Discord-purple)](https://discord.gg/VxCFSFC6bW)

</div>
<div align="center">

![language](https://img.shields.io/badge/language-C-orange.svg)
[![Build](https://dl.circleci.com/status-badge/img/circleci/NY2jeZsLjp6awG9G8CrUsi/Cusv1XhXjvcKuDsRnRrmZC/tree/main.svg?style=shield)](https://dl.circleci.com/status-badge/redirect/circleci/NY2jeZsLjp6awG9G8CrUsi/Cusv1XhXjvcKuDsRnRrmZC/tree/main)
[![OpenSSF Best Practices](https://www.bestpractices.dev/projects/1684/badge)](https://www.bestpractices.dev/projects/1684)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/69a95540047e47d08670fe3ea85dcaaa)](https://app.codacy.com/gh/EDI-Systems/M5P01_Prokaron/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)

</div>

点击 **[这里](README_CN.md)** 查看中文版。

&ensp;&ensp;**RMP** is a small real-time operating system which focuses on formal reliability and simplicity. It achieves reliability by deployment of formal techniques(**not completed yet**; only whitebox testing with 100% branch coverage done. The kernel can be regarded as pre-certified IEC 61508 SIL2, or EAL 4). All the basic functionalities that are necessary for RTOSes are provided, but nothing more. This guarantees that the system is the minimum possible kernel and is also suitable to be used as a guest operating system when hosted on virtual machine monitors.  

&ensp;&ensp;This operating system is much **leaner** than any other RTOSes, especially when compared to _FreeRTOS_ or _RT-Thread_, and understanding it should be simple enough. Yet it provides a complete set of functions that you may need during resource-constrained microcontroller development, such as **efficient memory management**, **anti-aliasing graphics**, and **various helper functions**. All these features come in a **single .C** file, and are **without any extra RAM consumption**!

&ensp;&ensp;The manual of the operating system can be found **[here](/Document/Public)**.

&ensp;&ensp;Read **[Contributing](CONTRIBUTING.md)** and **[Code of Conduct](CODE_OF_CONDUCT.md)** if you want to contribute, and **[Pull Request Template](PULL_REQUEST_TEMPLATE.md)** when you make pull requests.
This software is an official work of EDI, and thus belongs to the **public domain**. All copyrights reserved by EDI are granted to all entities under all applicable laws to the maximum extent.

&ensp;&ensp;For vendor-supplied packages and hardware abstraction libraries, please refer to the **[M0A00_Library](https://github.com/EDI-Systems/M0A00_Library)** repo to download and use them properly.

## Quick Demo
### Linux minimal runnable binary
&ensp;&ensp;Compile the 32-bit linux binary **[here](/Project/GCCMF-X86-LINUX)** and watch benchmark results!

### Built-in graphics : widgets, example and FXAA anti-aliasing
![Controls](/Document/Public/Demo/Controls.png)
![Calculator](/Document/Public/Demo/Calc.png)
![FXAA](/Document/Public/Demo/FXAA.png)

### Basic thread operations
**Create a thread**
```C
    RMP_Thd_Crt(&Thd_1            /* Thread control block */,
                Func_1            /* Thread entry */,
                &Stack_1          /* Stack address */,
                sizeof(Stack_1),  /* Stack size */,
                (void*)0x12345678 /* Parameter */,
                1                 /* Priority */, 
                5                 /* Timeslices */);
```
**Delete a thread**
```C
    RMP_Thd_Del(&Thd_1            /* Thread control block */);
```
**Suspend a thread**
```C
    RMP_Thd_Suspend(&Thd_1        /* Thread control block */);
```
**Resume a thread**
```C
    RMP_Thd_Resume(&Thd_1         /* Thread control block */);
```

### Delaying a thread
![Delay](/Document/Public/Demo/Delay.gif)
```C
    void Func_1(void* Param)
    {
        RMP_LOG_S("Parameter passed is ");
        RMP_LOG_H((ptr_t)Param);
        RMP_LOG_S("\r\n");
        while(1)
        {
            RMP_Thd_Delay(30000);
            RMP_LOG_S("Delayed 30000 cycles\r\n\r\n");
        };
    }

    void RMP_Init_Hook(void)
    {
        RMP_Thd_Crt(&Thd_1, Func_1, &Stack_1, sizeof(Stack_1), (void*)0x12345678, 1, 5);
    }
```
### Send from one thread to another
![Send](/Document/Public/Demo/Send.gif)
```C
    void Func_1(void* Param)
    {
        ptr_t Time=0;
        while(1)
        {
            RMP_Thd_Delay(30000);
            RMP_Thd_Snd(&Thd_2, Time, RMP_SLICE_MAX);
            Time++;
        };
    }

    void Func_2(void* Param)
    {
        ptr_t Data;
        while(1)
        {
            RMP_Thd_Rcv(&Data, RMP_SLICE_MAX);
            RMP_LOG_S("Received ");
            RMP_LOG_I(Data);
            RMP_LOG_S("\n");
        };
    }

    void RMP_Init_Hook(void)
    {
        RMP_Thd_Crt(&Thd_1, Func_1, &Stack_1, sizeof(Stack_1), (void*)0x12345678, 1, 5);
        RMP_Thd_Crt(&Thd_2, Func_2, &Stack_2, sizeof(Stack_2), (void*)0x87654321, 1, 5);
    }
```

### Counting semaphores
![Semaphore](/Document/Public/Demo/Semaphore.gif)
```C
    void Func_1(void* Param)
    {
        while(1)
        {
            RMP_Thd_Delay(30000);
            RMP_Sem_Post(&Sem_1, 1);
        };
    }

    void Func_2(void* Param)
    {
        ptr_t Data;
        while(1)
        {
            RMP_Sem_Pend(&Sem_1, RMP_SLICE_MAX);
            RMP_LOG_S("Semaphore successfully acquired!\r\n\r\n");
        };
    }

    void RMP_Init_Hook(void)
    {
        RMP_Sem_Crt(&Sem_1,0);
        RMP_Thd_Crt(&Thd_1, Func_1, &Stack_1, sizeof(Stack_1), (void*)0x12345678, 1, 5);
        RMP_Thd_Crt(&Thd_2, Func_2, &Stack_2, sizeof(Stack_2), (void*)0x87654321, 1, 5);
    }
```

### Memory pool operations
```C
    /* Initialize memory pool */
    RMP_Mem_Init(Pool, Pool_Size);

    /* Allocate from the pool */
    Mem=RMP_Malloc(Pool, Alloc_Size);

    /* Free allocated memory */
    RMP_Free(Pool, Mem);
```

### Typical performance figures for all supported architectures

&ensp;&ensp;The **absolute minimum** value for RMP is about **1.6k ROM and 432 byte RAM**, which is reached on the STM32F030F4 (Cortex-M0) port, and this number even included the 60-byte thread control block and 256-byte stack of the first thread, and a 64-byte kernel interrupt response stack. The OS kernel and the stripped down HAL only consumes **52 bytes** of memory combined. If you are willing to push this limit even further, then the **manufacturer HAL is a rip-off for you** and you can roll your own.

&ensp;&ensp;The current minimal proof-of-concept implementation that can finish the benchmark test is achieved with ATMEGA328P. It only has a meager **32k Flash and 2k SRAM**. 

&ensp;&ensp;The timing performance of the kernel in __real action__ is shown as follows. All compiler options are the highest optimization (usually -O3 with LTO when available) and optimized for time, and all values are __average case__ in CPU cycles; the __WCET__ registered in [test header files](Include/Test/Chip) is roughly equivalent to this value plus the tick timer interrupt interference.
- Yield    : Yield from one thread to another.
- Mail     : Mailbox communication from one thread to another.
- Sem      : Semaphore communication from one thread to another.
- FIFO     : FIFO read/write pair within a single thread.
- Msgq     : Message queue communication from one thread to another.
- Bmq      : Blocking message queue communication from one thread to another.
- Mail/I   : Send to a mailbox from interrupt.
- Sem/I    : Post to a semaphore from interrupt.
- Msgq/I   : Send to a message queue from interrupt.
- Bmq/I    : Send to a blocking message queue from interrupt.
- Mem      : A pair of memory pool malloc/free.

&ensp;&ensp;The difference between `Msgq` and `Bmq` is, in `Msgq`, only the receiver may block, whereas in `Bmq` both may block.

|Chipname     |Platform    |Build |Yield|Mail |Sem  |FIFO |Msgq |Bmq  |Mail/I|Sem/I|Msgq/I |Bmq/I |Mem  |
|:-----------:|:----------:|:----:|:---:|:---:|:---:|:---:|:---:|:---:|:----:|:---:|:-----:|:----:|:---:|
|ATMEGA328P   |AVR         |GCC   |408  |719  |686  |313  |1065 |1318 |624   |626  |905    |1073  |N/A  |
|ATMEGA1284P  |...         |...   |437  |751  |717  |314  |1098 |1352 |637   |639  |921    |1087  |1680 |
|ATMEGA2560   |...         |...   |449  |774  |736  |326  |1131 |1396 |656   |654  |942    |1117  |1686 |
|SPCE061A     |unSP        |GCC   |694  |1732 |1548 |927  |2671 |3709 |1619  |1475 |2242   |2889  |3518 |
|PIC24FJ128   |PIC24F      |XC16  |152  |334  |271  |168  |468  |654  |274   |213  |352    |461   |379  |
|DSPIC33EP512 |DSPIC33E    |XC16  |214  |447  |353  |219  |608  |851  |368   |278  |455    |602   |448  |
|MSP430F149   |MSP430      |CCS   |312  |641  |573  |312  |985  |1278 |528   |487  |739    |898   |N/A  |
|MSP430FR5994 |MSP430X     |CCS   |468  |1054 |891  |492  |1573 |2072 |891   |784  |1176   |1464  |3291 |
|STM32F030F4  |Cortex-M0   |Keil  |362  |763  |666  |379  |1196 |1609 |689   |616  |950    |1211  |N/A  |
|...          |...         |GCC   |366  |802  |690  |396  |1246 |1685 |705   |622  |954    |1200  |N/A  |
|STM32L071CB  |Cortex-M0+  |Keil  |335  |581  |532  |253  |892  |1167 |554   |524  |756    |945   |N/A  |
|...          |...         |GCC   |337  |656  |600  |284  |947  |1260 |578   |602  |794    |1003  |N/A  |
|STM32F103RE  |Cortex-M3   |Keil  |203  |438  |385  |226  |684  |930  |392   |354  |542    |707   |518  |
|...          |...         |GCC   |TBD  |TBD  |TBD  |TBD  |TBD  |TBD  |TBD   |TBD  |TBD    |TBD   |TBD  |
|STM32F405RG  |Cortex-M4   |Keil  |180  |345  |321  |180  |667  |886  |309   |302  |498    |626   |455  |
|...          |...         |GCC   |196  |388  |345  |192  |677  |953  |381   |349  |566    |743   |411  |
|STM32F767IG  |Cortex-M7   |Keil  |176  |329  |277  |174  |510  |694  |328   |259  |413    |516   |334  |
|...          |...         |GCC   |182  |335  |288  |156  |473  |643  |313   |264  |375    |514   |332  |
|TMS570LS0432 |Cortex-R4   |CCS   |306  |493  |460  |193  |686  |897  |480   |464  |592    |736   |533  |
|TMS570LC4357 |Cortex-R5   |CCS   |275  |479  |467  |216  |746  |998  |440   |435  |595    |763   |482  |
|PIC32MZ2048  |MIPS M14k   |XC32  |260  |392  |370  |146  |540  |672  |440   |420  |530    |620   |364  |
|TMS320F2812  |C28x        |CCS   |217  |493  |407  |229  |706  |954  |436   |381  |583    |727   |939  |
|TMS320F28335 |C28x/FPU32  |...   |246  |513  |440  |235  |751  |1001 |440   |413  |622    |770   |946  |
|CH32V307     |RV32IMAC    |GCC   |232  |407  |372  |172  |578  |725  |369   |327  |468    |582   |432  |
|...          |RV32IMAFC   |GCC   |305  |484  |445  |176  |650  |804  |403   |374  |508    |626   |429  |
|i9-7980XE    |X86-LINUX   |GCC   |TBD  |TBD  |TBD  |TBD  |TBD  |TBD  |TBD   |TBD  |TBD    |TBD   |TBD  |

&ensp;&ensp;The **[RVM](https://github.com/EDI-Systems/M7M02_Ammonite)** embedded hypervisor virtualized versions:

|Chipname     |Platform    |Build |Yield|Mail |Sem  |FIFO |Msgq |Bmq  |Mail/I|Sem/I|Msgq/I |Bmq/I |Mem  |
|:-----------:|:----------:|:----:|:---:|:---:|:---:|:---:|:---:|:---:|:----:|:---:|:-----:|:----:|:---:|
|STM32L071CB  |Cortex-M0+  |Keil  |469  |841  |717  |391  |1143 |1529 |1484  |1376 |1646   |1867  |N/A  |
|...          |...         |GCC   |TBD  |TBD  |TBD  |TBD  |TBD  |TBD  |TBD   |TBD  |TBD    |TBD   |TBD  |
|STM32F405RG  |Cortex-M4   |Keil  |TBD  |TBD  |TBD  |TBD  |TBD  |TBD  |TBD   |TBD  |TBD    |TBD   |TBD  |
|...          |...         |GCC   |TBD  |TBD  |TBD  |TBD  |TBD  |TBD  |TBD   |TBD  |TBD    |TBD   |TBD  |
|STM32F767IG  |Cortex-M7   |Keil  |221  |403  |348  |150  |589  |765  |942   |962  |1134   |1227  |334  |
|...          |...         |GCC   |TBD  |TBD  |TBD  |TBD  |TBD  |TBD  |TBD   |TBD  |TBD    |TBD   |TBD  |
|CH32V307     |RV32IMAFC   |GCC   |TBD  |TBD  |TBD  |TBD  |TBD  |TBD  |TBD   |TBD  |TBD    |TBD   |TBD  |

&ensp;&ensp;In contrast, RT-Linux 4.12's best context switch time on Cortex-M7 is bigger than 25000 cycles (note that has to run from FMC SDRAM due to its sheer size, so this is not fair comparison). This is measured with futex; if other forms of IPC such as pipes are used, this time is even longer.

&ensp;&ensp;__No cheating methods__ (such as toolchain-specific peephole optimizations that harm portability, cooperative switches that don't invoke the scheduler, scheduler designs that are fast in average case but have unbounded WCET, or even RMS-style stackless coroutine switches) are used in the experiments, and the reported WCETs in test headers are real. Despite the fact that we list the average case values for generic comparisons, it is important to realize that __only WCETs matter__ in a RTOS; optimizations that help the average case but hurt the worst-case are never suitable for such kernels. If maximum speed is your utmost goal, __no system is faster than RMS or DOS__; the theoretical context switch time of the RMS is zero (when all tasks have a single state and are inlined), while DOS does not need context switches altogether because it only allows one execution flow.

### Possible new platform supports
|Platform   |Reason                 |Priority            |
|:---------:|:---------------------:|:------------------:|
|RL78       |Largely used 16-bit MCU|:star: :star: :star:|

### Architectures NOT Supported
|Architecture   |Reason                |Workaround                                                                     |
|:-------------:|:--------------------:|:-----------------------------------------------------------------------------:|
|PIC18          |Hardware stack        |Use [RMS State-machine based OS](https://github.com/EDI-Systems/M2A01_Simpron)|
|AVR32          |In decline            |Use more popular Cortex-M and RISC-Vs                                        |
|ARMv5          |New versions available|Use newer Cortex-M and RISC-Vs                                               |
|x86-64         |Advanced system       |Use [RME Microkernel-based OS](https://github.com/EDI-Systems/M7M01_Eukaron)  |
|Cortex-A       |Advanced system       |Use [RME Microkernel-based OS](https://github.com/EDI-Systems/M7M01_Eukaron)  |
|PowerPC        |In decline            |Use more popular Cortex-M and RISC-Vs                                        |
|RX100/600/600S |Rarely used           |Use more popular Cortex-M and RISC-Vs                                        |
|Tricore        |Rarely used           |Use more popular Cortex-M and RISC-Vs                                        |
|MB91460        |Rarely used           |Use more popular Cortex-M and RISC-Vs                                        |

## Getting Started

&ensp;&ensp;These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

&ensp;&ensp;You need **_Cortex-M or RISC-V or MIPS or MSP430_** microcontroller development kits to run the tests. This RTOS focuses on value-line MCUs and do not concentrate on high-end MCUs or MPUs. Do not use QEMU simulator to test the projects because they do not behave correctly in many scenarios.  

&ensp;&ensp;If you don't have a development board, a **_x86-based Linux port_** of RMP is also available. However, running RMP on top of linux uses the [ptrace](https://en.wikipedia.org/wiki/Ptrace) system call and [signal](https://en.wikipedia.org/wiki/Signal_(IPC)) system, thus it is not particularly fast. Just run the example and observe benchmark output.  

&ensp;&ensp;Other platform supports should be simple to implement, however they are not scheduled yet. For Cortex-A and other CPUs with a memory management unit ([MMU](https://en.wikipedia.org/wiki/Memory_management_unit)), go [M7M01_Eukaron](https://github.com/EDI-Systems/M7M01_Eukaron) _Real-Time Multi-Core Microkernel_ instead; M7M01 supports some microcontrollers as well.

### Compilation

&ensp;&ensp;The **Vendor Toolchain** or **Eclipse** projects for various microcontrollers are available in the **_Project_** folder. Refer to the readme files in each folder for specific instructions about how to run them. However, keep in mind that some examples may need vendor-specific libraries such as the STMicroelectronics HAL. Some additional drivers may be required too. These can be found in **[M0A00_Library](https://github.com/EDI-Systems/M0A00_Library)** repo.

## Running the Tests

&ensp;&ensp;To run the sample programs, simply download them into the development board and start step-by-step debugging. Some examples will use one or two LEDs to indicate the system status. In that case, it is necessary to fill the LED blinking wrapper functions.  

&ensp;&ensp;To use the graphics library and other advanced features, please refer to the user manual.

## Deployment

&ensp;&ensp;&ensp;When deploying this into a production system, it is recommended that you read the manual in the **_Documents_** folder carefully to configure all macros correctly.

## Built With

- Keil uVision (ARMCC/ARMCLANG)
- Code Composer Studio
- MPLAB X XC16/XC32
- GCC/Clang-LLVM

&ensp;&ensp;Other toolchains are not recommended nor supported at this point, though it might be possible to support them later on.

## Contributing

&ensp;&ensp;Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## EDI Project Information
- M5P01 R5T2

## Starring Contributors
- Leifeng Song - ARM Cortex-M3/4/7 assembly port.
- Runsheng Hou - ARM Cortex-M4/7 RVM port and lwIP demo.
- Yihe Wang - Stable x86/linux/ptrace port.
- Ran Zhang - C28x DSP port.
- Kai Zhang - White-box testing.