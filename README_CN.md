# MuProkaron

![Build](https://travis-ci.org/EDI-Systems/M5P1_MuProkaron.svg?branch=master) 
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/1684/badge)](https://bestpractices.coreinfrastructure.org/projects/1684) 
[![Join the chat at https://gitter.im/M5P1_MuProkaron/Lobby](https://badges.gitter.im/M5P1_MuProkaron/Lobby.svg)](https://gitter.im/M5P1_MuProkaron/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Click **[HERE](README.md)** for English version.

**RMP** 是一个专注于形式化验证和简易可用的小型实时系统。它通过采取形式化方法来确保系统的可靠性（当前验证工作还未完成）。所有的实时操作系统必备的功能它都具备，但是并不在此基础上提供更多可选组件以确保内核的精炼性。这样，得到的内核就是一个最小化的内核，可以很方便地对它进行形式化验证。同时，它还可以作为客户操作系统运行在虚拟机监视器上。本系统比以_FreeRTOS_和_RT-Thread_为代表的全功能系统相比要**小**得多，而且理解起来应该也相对容易得多。

如果想要参与开发，请阅读 **[参与](CONTRIBUTING.md)** 和 **[规范](CODE_OF_CONDUCT.md)** 两个指导文档。如果要提交拉取请求，请使用 **[拉取请求模板](PULL_REQUEST_TEMPLATE.md)** 。
本软件采用 **三种不同的授权** ：你可以选择 **[LGPL v3](LICENSE.md)** ，也可以选择 **[经修改的MIT协议](MODMIT.md)** 。 如果有特殊需求， 也可以联系我们请求**商业授权**。

对于那些由微控制器厂商提供的硬件抽象层软件包，请到 **[M0P0_Library](https://github.com/EDI-Systems/M0P0_Library)** 软件仓库自行下载。

## 快速演示
### 可在Linux上直接运行的Demo
从 **[这里](Project/ECLIPSE-GCC-LINUX/RMP/Debug/RMP)** 下载预编译的x86-32位Linux系统上可运行的二进制，观察性能测试的运行结果！

### 基础线程操作
**创建一个线程**
```C
    RMP_Thd_Crt(&Thd_1            /* 线程控制块 */, 
                Func_1            /* 线程入口 */,
                &Stack_1[238]     /* 线程栈地址 */,
                (void*)0x12345678 /* 参数 */,
                1                 /* 优先级 */, 
                5                 /* 时间片 */);
```
**删除一个线程**
```C
    RMP_Thd_Del(&Thd_1            /* 线程控制块 */);
```
**悬起一个线程**
```C
    RMP_Thd_Suspend(&Thd_1        /* 线程控制块 */);
```
**解除线程悬起**
```C
    RMP_Thd_Resume(&Thd_1         /* 线程控制块 */);
```

### 线程延时
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
### 从一个线程向另一个线程的邮箱发送
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

### 计数信号量
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

### 内存池操作
```C
    /* Initialize memory pool */
    RMP_Mem_Init(Pool, Pool_Size);

    /* Allocate from the pool */
    Mem=RMP_Malloc(Pool, Alloc_Size);

    /* Free allocated memory */
    RMP_Free(Pool, Mem);
```

### 所有被支持架构上的典型性能数据
|Machine      |Toolchain     |Flash|SRAM|Yield|Mail |Sem  |Mail/Int|Sem/Int|Mem  |
|:-----------:|:------------:|:---:|:--:|:---:|:---:|:---:|:------:|:-----:|:---:|
|MSP430       |TI CCS6       |2.90 |0.64|495  |906  |786  |830     |736    |1575 |
|MSP430       |GCC           |TBT  |TBT |TBT  |TBT  |TBT  |TBT     |TBT    |     |
|Cortex-M0    |Keil uVision 5|4.94 |1.65|374  |663  |616  |659     |617    |N/A  |
|Cortex-M0+   |Keil uVision 5|6.25 |1.65|334  |607  |544  |588     |552    |N/A  |
|Cortex-M3    |Keil uVision 5|2.60 |1.65|246  |456  |422  |443     |409    |308  |
|Cortex-M3    |GCC           |TBT  |TBT |TBT  |TBT  |TBT  |TBT     |TBT    |     |
|Cortex-M4    |Keil uVision 5|2.70 |1.66|184  |339  |325  |374     |361    |244  |
|Cortex-M4    |GCC           |TBT  |TBT |TBT  |TBT  |TBT  |TBT     |TBT    |     |
|Cortex-M7    |Keil uVision 5|6.66 |1.65|170  |256  |230  |274     |268    |180  |
|Cortex-M7    |GCC           |TBT  |TBT |TBT  |TBT  |TBT  |TBT     |TBT    |     |
|Cortex-R4    |Keil uVision 5|TBT  |TBT |TBT  |TBT  |TBT  |TBT     |TBT    |     |
|Cortex-R4    |GCC           |TBT  |TBT |TBT  |TBT  |TBT  |TBT     |TBT    |     |
|Cortex-R5    |Keil uVision 5|TBT  |TBT |TBT  |TBT  |TBT  |TBT     |TBT    |     |
|Cortex-R5    |GCC           |TBT  |TBT |TBT  |TBT  |TBT  |TBT     |TBT    |     |
|MIPS M14k    |XC32-GCC      |17.2 |2.46|264  |358  |340  |421     |415    |213  |
|X86-LINUX    |GCC           |N/A  |N/A |33000|35000|33000|35000   |33000  |136  |

**Flash和SRAM消耗以kB计，其他数据以CPU指令周期计。这里列出的所有值都是典型（有意义的系统配置）值而非绝对意义上的最小值，因为纯技术层面的最小配置在实际工程中很少是真正有用的。**

- MSP430平台使用MSP430FR5994进行评估。
- Cortex-M0平台使用STM32F030F4P6进行评估。
- Cortex-M0+平台使用STM32L053C8T6进行评估。
- Cortex-M3平台使用STM32F103RET6进行评估。
- Cortex-M4平台使用STM32F405RGT6进行评估。
- Cortex-M7平台使用STM32F767IGT6进行评估。
- MIPS M14k平台使用PIC32MZ2048EFM100进行评估。
- X86 Linux平台使用Ubuntu 16.04和i7-4820k @ 3.7GHz进行评估。

所有的编译器优化选项都被设为最高（通常是-O3），而且时间优化选项也被打开。 
- Yield   ：两线程间进行切换所用的时间。  
- Mail    ：两线程间使用邮箱进行发送-接收操作的耗时。  
- Sem     ：两线程间使用计数信号量进行发布-获取操作的耗时。  
- Mail/Int：从中断发送到某线程邮箱的耗时。 
- Sem/Int ：从中断发布信号量的耗时。 
- Mem     ：进行一次内存操作（比如分配或释放）的用时。 

## 新手上路

下面的说明会帮助你在本地快速建立一个可用来评估测试本系统的工程。请参看系统的中文文档以获取更多信息。

### 准备工作

要运行测试，你需要一块基于 **_Cortex-M或Cortex-R或MIPS或MSP430_** 的开发板。本RTOS主要面向资源受限的MCU，不提供对高端MCU和，MPU和CPU的特别支持。不要使用QEMU模拟器来测试本系统，因为QEMU有很多不完善之处，与真正的硬件行为并不一致。
如果你没有开发板，那么RMP也有一个 **_基于x86处理器的Linux移植_** 。然而，该移植使用了[ptrace](https://en.wikipedia.org/wiki/Ptrace)系统调用和[信号](https://en.wikipedia.org/wiki/Signal_(IPC))系统，因此并不很快，这一点可以从性能测试的数据看出。
对于其他平台的支持应该是容易实现的，但是当前并没有支持计划。对于那些Cortex-A和具备内存管理单元（[MMU](https://en.wikipedia.org/wiki/Memory_management_unit)）的其他处理器，可以使用[M7M1_MuEukaron](https://github.com/EDI-Systems/M7M1_MuEukaron) _实时多核心微内核_；M7M1也支持一部分的Cortex-M和全部的Cortex-R。

### 编译指南

在 **_Project_** 文件夹下能够找到多种微控制器的移植好的 **厂商集成开发环境** 或 **Eclipse** 的工程样板。参看各个工程文件夹下的自述文件以获取更多关于如何编译和运行该工程的信息。某些工程需要额外的厂商硬件抽象层库的支持，它们可以在 **[M0P0_Library](https://github.com/EDI-Systems/M0P0_Library)** 软件仓库被找到。

## 运行测试

要运行测试，只要将测试下载到对应的开发板并开始单步调试即可。某些例子会采用两个LED来指示系统当前的状态，此时要填充LED的点亮和熄灭函数来运行该示例。

## 生产部署

当部署本系统到生产环境时，请仔细阅读本系统自带的手册，以确保各项配置正确。本系统的手册可以在 **_Documents_** 文件夹下找到。

## 支持的工具链

- Keil uVision 5 (armcc)
- Code composer studio
- MPLAB X XC32
- gcc/clang-llvm

其他的工具链现在不推荐或者当前不受支持，虽然要增加新的支持应该也很简单。

## 参与项目

请阅读[CONTRIBUTING.md](CONTRIBUTING.md)文档来获得关于行为规范和提交代码的相关信息。

## EDI 工程信息
演进 - 远古 - 原核 (M5P1 R4T1)
