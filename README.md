<h1 align="center">
	<img width="300" src="https://raw.githubusercontent.com/EDI-Systems/M5P1_MuProkaron/master/Documents/Demo/logo.png" alt="logo">
</h1>

# One RTOS (RMP)
[![Github release](https://img.shields.io/github/release/EDI-Systems/M5P1_MuProkaron.svg)](https://github.com/EDI-Systems/M5P1_MuProkaron/releases/latest)
[![Github commits](https://img.shields.io/github/monthly-commits/EDI-Systems/M5P1_MuProkaron.svg)](https://github.com/EDI-Systems/M5P1_MuProkaron/compare/master@{30day}...master)
![language](https://img.shields.io/badge/language-C-orange.svg)
![Build](https://travis-ci.org/EDI-Systems/M5P1_MuProkaron.svg?branch=master) 
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/1684/badge)](https://bestpractices.coreinfrastructure.org/projects/1684) 
[![Join the chat at https://gitter.im/M5P1_MuProkaron/Lobby](https://badges.gitter.im/M5P1_MuProkaron/Lobby.svg)](https://gitter.im/M5P1_MuProkaron/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

点击 **[这里](README_CN.md)** 查看中文版。

&ensp;&ensp;&ensp;&ensp;**RMP** is a small real-time operating system which focuses on formal reliability and simplicity. It achieves reliability by deployment of formal techniques(not completed yet). All the basic functionalities that are necessary for RTOSes are provided, but nothing more. This guarantees that the system is the minimum possible kernel and is also suitable to be used as a guest operating system when hosted on virtual machine monitors.  

&ensp;&ensp;&ensp;&ensp;This operating system is much **leaner** than any other RTOSes, especially when compared to _FreeRTOS_ or _RT-Thread_, and understanding it should be simple enough. Yet it provides a complete set of functions that you may need during resource-constrained microcontroller development, such as **efficient memory management**, **anti-aliasing graphics**, and **various helper functions**. All these features come in a **single .C** file, and are **without any extra RAM consumption**!

&ensp;&ensp;&ensp;&ensp;The manual of the operating system can be found **[here](https://github.com/EDI-Systems/M5P1_MuProkaron/blob/master/Documents/M5P1_Light-Weight-RTOS-User-Manual.pdf)**.

&ensp;&ensp;&ensp;&ensp;Read **[Contributing](CONTRIBUTING.md)** and **[Code of Conduct](CODE_OF_CONDUCT.md)** if you want to contribute, and **[Pull Request Template](PULL_REQUEST_TEMPLATE.md)** when you make pull requests.
This software is **triple-licensed**: it is either **[LGPL v3](LICENSE.md)** or **[modified MIT license](MODMIT.md)**. **Commercial** licenses are also available upon request.

&ensp;&ensp;&ensp;&ensp;For vendor-supplied packages and hardware abstraction libraries, please refer to the **[M0P0_Library](https://github.com/EDI-Systems/M0P0_Library)** repo to download and use them properly.

## Quick Demo
### Linux minimal runnable binary
&ensp;&ensp;&ensp;&ensp;Download the precompiled 32-bit linux binary **[here](Project/ECLIPSE-GCC-LINUX/RMP/Debug/RMP)** and watch benchmark results!

### Built-in graphics : widgets, example and FXAA anti-aliasing
![Controls](https://raw.githubusercontent.com/EDI-Systems/M5P1_MuProkaron/master/Documents/Demo/Controls.png)
![Calculator](https://raw.githubusercontent.com/EDI-Systems/M5P1_MuProkaron/master/Documents/Demo/Calc.png)
![FXAA](https://raw.githubusercontent.com/EDI-Systems/M5P1_MuProkaron/master/Documents/Demo/FXAA.png)

### Basic thread operations
**Create a thread**
```C
    RMP_Thd_Crt(&Thd_1            /* Thread control block */, 
                Func_1            /* Thread entry */,
                &Stack_1[238]     /* Stack address */,
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
![Delay](https://raw.githubusercontent.com/EDI-Systems/M5P1_MuProkaron/master/Documents/Demo/Delay.gif)
```C
    void Func_1(void* Param)
    {
        RMP_PRINTK_S("Parameter passed is ");
        RMP_PRINTK_U((ptr_t)Param);
        RMP_PRINTK_S("\r\n");
        while(1)
        {
            RMP_Thd_Delay(30000);
            RMP_PRINTK_S("Delayed 30000 cycles\r\n\r\n");
        };
    }

    void RMP_Init_Hook(void)
    {
        RMP_Thd_Crt(&Thd_1, Func_1, &Stack_1[238], (void*)0x12345678, 1, 5);
    }
```
### Send from one thread to another
![Send](https://raw.githubusercontent.com/EDI-Systems/M5P1_MuProkaron/master/Documents/Demo/Send.gif)
```C
    void Func_1(void* Param)
    {
        ptr_t Time=0;
        while(1)
        {
            RMP_Thd_Delay(30000);
            RMP_Thd_Snd(&Thd_2, Time, RMP_MAX_SLICES);
            Time++;
        };
    }

    void Func_2(void* Param)
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

    void RMP_Init_Hook(void)
    {
        RMP_Thd_Crt(&Thd_1, Func_1, &Stack_1[238], (void*)0x12345678, 1, 5);
        RMP_Thd_Crt(&Thd_2, Func_2, &Stack_2[238], (void*)0x87654321, 1, 5);
    }
```

### Counting semaphores
![Semaphore](https://raw.githubusercontent.com/EDI-Systems/M5P1_MuProkaron/master/Documents/Demo/Semaphore.gif)
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
            RMP_Sem_Pend(&Sem_1, RMP_MAX_SLICES);
            RMP_PRINTK_S("Semaphore successfully acquired!\r\n\r\n");
        };
    }

    void RMP_Init_Hook(void)
    {
        RMP_Sem_Crt(&Sem_1,0);
        RMP_Thd_Crt(&Thd_1, Func_1, &Stack_1[238], (void*)0x12345678, 1, 5);
        RMP_Thd_Crt(&Thd_2, Func_2, &Stack_2[238], (void*)0x87654321, 1, 5);
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
|Machine      |Toolchain     |Flash|SRAM|Yield|Mail |Sem  |Mail/Int|Sem/Int|Mem  |
|:-----------:|:------------:|:---:|:--:|:---:|:---:|:---:|:------:|:-----:|:---:|
|MSP430       |TI CCS7       |2.90 |0.64|495  |906  |786  |830     |736    |1575 |
|Cortex-M0    |Keil uVision 5|4.94 |1.65|374  |663  |616  |659     |617    |N/A  |
|Cortex-M0+   |Keil uVision 5|6.25 |1.65|334  |607  |544  |588     |552    |N/A  |
|Cortex-M3    |Keil uVision 5|5.31 |1.65|252  |513  |448  |465     |418    |311  |
|Cortex-M4    |Keil uVision 5|5.46 |1.66|188  |386  |353  |361     |329    |233  |
|Cortex-M7    |Keil uVision 5|6.66 |1.65|196  |288  |277  |296     |296    |183  |
|Cortex-M7-RVM|Keil uVision 5|2.09 |2.29|1068 |1179 |1221 |1402    |1444   |176  |
|Cortex-R4    |TI CCS7       |15.1 |1.42|281  |458  |406  |424     |368    |274  |
|Cortex-R5    |TI CCS7       |18.2 |3.72|305  |471  |426  |472     |432    |267  |
|MIPS M14k    |XC32-GCC      |17.2 |2.46|264  |358  |340  |421     |415    |213  |
|X86-LINUX    |GCC           |N/A  |N/A |33000|35000|33000|35000   |33000  |136  |

*As a comparison, RT-Linux 4.12's best context switch time on Cortex-M7 is bigger than 25000 cycles. This is measured with futex; if other forms of IPC such as pipes are used, this time is even longer.
<!-- |MSP430       |GCC           |     |    |     |     |     |        |       |     | -->
<!-- |RL78         |GCC           |     |    |     |     |     |        |       |     | -->
<!-- |Cortex-M3    |GCC           |     |    |     |     |     |        |       |     | -->
<!-- |Cortex-M4    |GCC           |     |    |     |     |     |        |       |     | -->
<!-- |Cortex-M7    |GCC           |     |    |     |     |     |        |       |     | -->
<!-- |Cortex-R4    |GCC           |     |    |     |     |     |        |       |     | -->
<!-- |Cortex-R5    |GCC           |     |    |     |     |     |        |       |     | -->

&ensp;&ensp;&ensp;&ensp;**Flash and SRAM consumption is calculated in kB, while the other figures are calculated in CPU clock cycles. All values listed here are typical (useful system) values, not minimum values, because minimum values on system size seldom make any real sense. HAL library are also included in the size numbers. The absolute minimum value for RMP is about 1k ROM/0.5k RAM.**

- MSP430 is evaluated with MSP430FR5994.
- Cortex-M0 is evaluated with STM32F030F4P6.
- Cortex-M0+ is evaluated with STM32L053C8T6.
- Cortex-M3 is evaluated with STM32F103RET6.
- Cortex-M4 is evaluated with STM32F405RGT6.
- Cortex-M7 is evaluated with STM32F767IGT6.
- Cortex-M7-RVM is evaluated with STM32F767IGT6, and the RMP runs as a guest OS in the **[RVM](https://github.com/EDI-Systems/M7M2_MuAmmonite)** embedded hypervisor.
- Cortex-R4 is evaluated with TMS570LS0432.
- Cortex-R5 is evaluated with TMS570LC4357.
- MIPS M14k is evaluated with PIC32MZ2048EFM100.
- X86 Linux is evaluated with Ubuntu 16.04 on i7-4820k @ 3.7GHz.

&ensp;&ensp;&ensp;&ensp;All compiler options are the highest optimization (usually -O3) and optimized for time. 
- Yield    : The time to yield between different threads.  
- Mail     : The mailbox communication time between two threads.  
- Sem      : The semaphore communication time between two threads.  
- Mail/Int : The time to send to a mailbox from interrupt.  
- Sem/Int  : The time to post to a semaphore from interrupt.  
- Mem      : The time to do an operation on memory, e.g. allocation/free.  

### Possible New Architecture Supports
|Architecture   |Reason                 |Priority                      |
|:-------------:|:---------------------:|:----------------------------:|
|PIC24/33/dsPIC |Largely used 16-bit MCU|:star::star::star::star::star:|
|RL78           |Largely used 16-bit MCU|:star::star::star:            |
|TI C2000       |Largely used DSP       |:star::star:                  |
|RISC-V         |New RISC architecture  |:star::star::star::star:      |
|MicroBlaze     |Largely used soft core |:star:                        |
|NIOS II        |Largely used soft core |:star:                        |

### Architectures NOT Supported
|Architecture   |Reason                |Workaround                                                                     |
|:-------------:|:--------------------:|:-----------------------------------------------------------------------------:|
|PIC18          |Hardware stack        |Use [RMS State-machine based OS](https://github.com/EDI-Systems/M2A1_MuSimpron)|
|AVR32          |In decline            |Use more popular Cortex-M and Cortex-Rs                                        |
|ARMv5          |New versions available|Use newer Cortex-M and Cortex-Rs                                               |
|x86-64         |Advanced system       |Use [RME Microkernel-based OS](https://github.com/EDI-Systems/M7M1_MuEukaron)  |
|Cortex-A       |Advanced system       |Use [RME Microkernel-based OS](https://github.com/EDI-Systems/M7M1_MuEukaron)  |
|Coldfire       |In decline            |Use more popular Cortex-M and Cortex-Rs                                        |
|PowerPC        |In decline            |Use more popular Cortex-M and Cortex-Rs                                        |
|RX100/600/600S |Rarely used           |Use more popular Cortex-M and Cortex-Rs                                        |
|Tricore        |Rarely used           |Use more popular Cortex-M and Cortex-Rs                                        |
|MB91460        |Rarely used           |Use more popular Cortex-M and Cortex-Rs                                        |

## Getting Started

&ensp;&ensp;&ensp;&ensp;These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

&ensp;&ensp;&ensp;&ensp;You need **_Cortex-M or Cortex-R or MIPS or MSP430_** microcontroller development kits to run the tests. This RTOS focuses on value-line MCUs and do not concentrate on high-end MCUs or MPUs. Do not use QEMU simulator to test the projects because they do not behave correctly in many scenarios.  

&ensp;&ensp;&ensp;&ensp;If you don't have a development board, a **_x86-based Linux port_** of RMP is also available. However, running RMP on top of linux uses the [ptrace](https://en.wikipedia.org/wiki/Ptrace) system call and [signal](https://en.wikipedia.org/wiki/Signal_(IPC)) system, thus it is not particularly fast. Just run the example and observe benchmark output.  

&ensp;&ensp;&ensp;&ensp;Other platform supports should be simple to implement, however they are not scheduled yet. For Cortex-A and other CPUs with a memory management unit ([MMU](https://en.wikipedia.org/wiki/Memory_management_unit)), go [M7M1_MuEukaron](https://github.com/EDI-Systems/M7M1_MuEukaron) _Real-Time Multi-Core Microkernel_ instead; M7M1 supports some Cortex-Ms and Cortex-Rs as well.

### Compilation

&ensp;&ensp;&ensp;&ensp;The **Vendor Toolchain** or **Eclipse** projects for various microcontrollers are available in the **_Project_** folder. Refer to the readme files in each folder for specific instructions about how to run them. However, keep in mind that some examples may need vendor-specific libraries such as the STMicroelectronics HAL. Some additional drivers may be required too. These can be found in **[M0P0_Library](https://github.com/EDI-Systems/M0P0_Library)** repo.

## Running the tests

&ensp;&ensp;&ensp;&ensp;To run the sample programs, simply download them into the development board and start step-by-step debugging. Some examples will use one or two LEDs to indicate the system status. In that case, it is necessary to fill the LED blinking wrapper functions.  

&ensp;&ensp;&ensp;&ensp;To use the graphics library and other advanced features, please refer to the user manual.

## Deployment

&ensp;&ensp;&ensp;When deploying this into a production system, it is recommended that you read the manual in the **_Documents_** folder carefully to configure all macros correctly.

## Built With

- Keil uVision 5 (armcc)
- Code composer studio
- MPLAB X XC32
- gcc/clang-llvm

&ensp;&ensp;&ensp;&ensp;Other toolchains are not recommended nor supported at this point, though it might be possible to support them later on.

## Contributing

&ensp;&ensp;&ensp;&ensp;Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## EDI Project Information
&ensp;&ensp;&ensp;&ensp;Mutate - Protero - Prokaron (M5P1 R4T1)
