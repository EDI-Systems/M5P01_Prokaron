<h1 align="center">
	<img width="300" src="/Document/Public/Demo/logo.png" alt="logo">
</h1>

# RMP Real-time kernel
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

Click **[HERE](README.md)** for English version.

&emsp;&emsp;**RMP** 是一个专注于形式化验证和简易可用的小型实时系统。它通过采取形式化方法来确保系统的可靠性（当前验证工作还__未完成__；不过，100%的白盒测试分支覆盖率已经达到。目前的内核可以被看作IEC 61508 SIL2预认证等级，或者EAL 4）。所有的实时操作系统必备的功能它都具备，但是并不在此基础上提供更多可选组件以确保内核的精炼性。这样，得到的内核就是一个最小化的内核，可以很方便地对它进行形式化验证。同时，它还可以作为客户操作系统运行在虚拟机监视器上。

&emsp;&emsp;本系统比以 _FreeRTOS_ 和 _RT-Thread_ 为代表的全功能系统相比要**小**得多，而且理解起来应该也相对容易得多。即便本系统仅仅包含了**一个.C**文件，它仍然提供了**高效的内存管理**，**抗锯齿图形界面库**和**其他实用函数**，并且这些功能的实现都**不消耗任何额外的RAM**！

&emsp;&emsp;本系统的手册可以在 **[这里](/Document/Public)** 找到。

&emsp;&emsp;如果想要参与开发，请阅读 **[参与](CONTRIBUTING.md)** 和 **[规范](CODE_OF_CONDUCT.md)** 两个指导文档。如果要提交拉取请求，请使用 **[拉取请求模板](PULL_REQUEST_TEMPLATE.md)** 。
本软件是EDI的官方作品，因此属于 **公有领域** 。 所有由EDI保留的版权在所有适用的法律条款下尽最大可能地授权给所有实体。

&emsp;&emsp;对于那些由微控制器厂商提供的硬件抽象层软件包，请到 **[M0A00_Library](https://github.com/EDI-Systems/M0A00_Library)** 软件仓库自行下载。

## 快速演示
### 可在Linux上直接运行的Demo
&emsp;&emsp;到 **[这里](/Project/GCCMF-X86-LINUX)** 编译x86-32位Linux系统上可运行的二进制，观察性能测试的运行结果！

### 可在红白机（FAMICOM/NES）上直接运行的Demo
&emsp;&emsp;从 **[这里](/Document/Public/Demo/RMP.nes)** 下载“游戏”镜像，把它装入你喜欢的模拟器（或者支持南梦宫163扩展芯片的烧录卡并插入实机），观察性能测试的运行结果！

![FAMICOM](/Document/Public/Demo/FAMICOM.png)

&emsp;&emsp;南梦宫163扩展芯片是必需的，因为系统需要利用它内部的IRQ计时器来测量性能。在所有扩展芯片中，唯一支持可读取计时器的只有南梦宫163，这颗芯片也被用于“星球大战（Star Wars）”、“三国志II 霸王的大陆”等知名游戏。南梦宫163有时也被叫做Namcot 163，或者iNES扩展019。

### 内建图形库：控件，界面样例和FXAA抗锯齿细节
![Controls](/Document/Public/Demo/Controls.png)
![Calculator](/Document/Public/Demo/Calc.png)
![FXAA](/Document/Public/Demo/FXAA.png)

### 基础线程操作
**创建一个线程**
```C
    RMP_Thd_Crt(&Thd_1            /* 线程控制块 */,
                Func_1            /* 线程入口 */,
                &Stack_1          /* 线程栈地址 */,
                sizeof(Stack_1),  /* 线程栈大小 */,
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
### 从一个线程向另一个线程的邮箱发送
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

### 计数信号量
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

### 内存池操作
```C
    /* 初始化内存池 */
    RMP_Mem_Init(Pool, Pool_Size);

    /* 从内存池分配内存 */
    Mem=RMP_Malloc(Pool, Alloc_Size);

    /* 向内存池归还内存 */
    RMP_Free(Pool, Mem);
```

### 所有支持架构上的典型性能数据

&emsp;&emsp;本系统的**绝对最小值**在**1.6k ROM和432Byte RAM**左右，这个大小是在STM32F030F4（Cortex-M0+）的移植上达到的，并包括了第一个线程的60Byte的线程控制块和256Byte的线程栈，以及64Byte的内核中断响应用栈。操作系统内核和最精简的HAL库一共仅占用了**52Byte**存储。如果你对这个数字仍不满意，那么可以**不使用厂商提供的HAL库**而自己写一个版本。

&emsp;&emsp;当前的能完成基准测试的最小概念性移植是ATMEGA328P上完成的，它仅有**32k Flash和2k SRAM**。

&emsp;&emsp;在**真实项目环境中**内核的时间性能如下所示。所有的编译器优化选项都被设为最高（通常是-O3），而且时间优化选项也被打开。所有值均为 CPU 周期的平均值；[测试头文件](Include/Test/Chip)中记录的最坏执行时间约等于该值加上定时器中断造成的最坏干扰。
- Yield    : 两线程间的切换操作。
- Mail     : 两线程间使用邮箱进行发送-接收操作。
- Sem      : 两线程间使用计数信号量进行发布-获取操作。
- FIFO     : 在单个线程内进行FIFO读/写。
- Msgq     : 两线程间的消息队列通信。
- Bmq      : 两线程间的阻塞消息队列通信。
- Mail/I   : 从中断发送到某线程邮箱。
- Sem/I    : 从中断发布信号量。
- Msgq/I   : 从中断发送到消息队列。
- Bmq/I    : 从中断发送到阻塞消息队列。
- Mem      : 进行一对内存分配/释放操作。

&ensp;&ensp;`Msgq`和`Bmq`的区别在于前者只有接收方可以阻塞，后者双方都可以阻塞。

|芯片          |架构        |工具链  |Yield|Mail |Sem  |FIFO |Msgq |Bmq  |Mail/I|Sem/I|Msgq/I |Bmq/I |Mem  |
|:-----------:|:----------:|:----:|:---:|:---:|:---:|:---:|:---:|:---:|:----:|:---:|:-----:|:----:|:---:|
|RP2A03/FC-84 |MOS6502     |CC65  |4073 |5435 |5435 |2028 |7726 |10445|4831  |5180 |7220   |8350  |7484 |
|RP2A03/MESEN |...         |...   |4060 |5439 |5424 |2040 |7728 |10443|4836  |5185 |7227   |8355  |7446 |
|ATMEGA328P   |AVR         |GCC   |408  |719  |686  |313  |1065 |1318 |624   |626  |905    |1073  |N/A  |
|ATMEGA1284P  |...         |...   |437  |751  |717  |314  |1098 |1352 |637   |639  |921    |1087  |1680 |
|ATMEGA2560   |...         |...   |449  |774  |736  |326  |1131 |1396 |656   |654  |942    |1117  |1686 |
|SPCE061A     |unSP        |...   |694  |1732 |1548 |927  |2671 |3709 |1619  |1475 |2242   |2889  |3518 |
|R5F104PJ     |RL78        |CCRL  |261  |565  |520  |308  |924  |1225 |539   |500  |789    |964   |1854 |
|PIC24FJ128   |PIC24F      |XC16  |152  |334  |271  |168  |468  |654  |274   |213  |352    |461   |379  |
|DSPIC33EP512 |DSPIC33E    |...   |214  |447  |353  |219  |608  |851  |368   |278  |455    |602   |448  |
|MSP430F149   |MSP430      |CCS   |312  |641  |573  |312  |985  |1278 |528   |487  |739    |898   |N/A  |
|MSP430FR5994 |MSP430X     |...   |468  |1054 |891  |492  |1573 |2072 |891   |784  |1176   |1464  |3291 |
|STM32F030F4  |Cortex-M0   |Keil  |362  |763  |666  |379  |1196 |1609 |689   |616  |950    |1211  |N/A  |
|...          |...         |GCC   |366  |802  |690  |396  |1246 |1685 |705   |622  |954    |1200  |N/A  |
|HC32L136K8   |Cortex-M0+  |Keil  |211  |422  |370  |219  |646  |873  |403   |350  |532    |673   |542  |
|STM32L071CB  |Cortex-M0+  |Keil  |335  |581  |532  |253  |892  |1167 |554   |524  |756    |945   |N/A  |
|...          |...         |GCC   |337  |656  |600  |284  |947  |1260 |578   |602  |794    |1003  |N/A  |
|STM32F103RE  |Cortex-M3   |Keil  |203  |438  |385  |226  |684  |930  |392   |354  |542    |707   |518  |
|...          |...         |GCC   |TBD  |TBD  |TBD  |TBD  |TBD  |TBD  |TBD   |TBD  |TBD    |TBD   |TBD  |
|STM32F405RG  |Cortex-M4   |Keil  |180  |345  |321  |180  |667  |886  |309   |302  |498    |626   |455  |
|...          |...         |GCC   |196  |388  |345  |192  |677  |953  |381   |349  |566    |743   |411  |
|STM32F767IG  |Cortex-M7   |Keil  |176  |329  |277  |174  |510  |694  |328   |259  |413    |516   |334  |
|...          |...         |GCC   |182  |335  |288  |156  |473  |643  |313   |264  |375    |514   |332  |
|TMS570LS0432 |Cortex-R4   |CCS   |306  |493  |460  |193  |686  |897  |480   |464  |592    |736   |533  |
|TMS570LC4357 |Cortex-R5   |...   |275  |479  |467  |216  |746  |998  |440   |435  |595    |763   |482  |
|PIC32MZ2048  |MIPS        |XC32  |190  |345  |305  |150  |475  |620  |295   |260  |370    |465   |365  |
|...          |MIPS-FR64   |..    |475  |630  |585  |160  |775  |935  |400   |360  |490    |585   |371  |
|TMS320F2812  |C28x        |CCS   |217  |493  |407  |229  |706  |954  |436   |381  |583    |727   |939  |
|TMS320F28335 |C28x/FPU32  |...   |246  |513  |440  |235  |751  |1001 |440   |413  |622    |770   |946  |
|CH32V307     |RV32IMAC    |GCC   |209  |386  |336  |172  |538  |698  |350   |306  |436    |555   |433  |
|...          |RV32IMAFC   |...   |217  |398  |341  |172  |557  |705  |358   |307  |444    |556   |433  |
|i9-7980XE    |X86-LINUX   |...   |TBD  |TBD  |TBD  |TBD  |TBD  |TBD  |TBD   |TBD  |TBD    |TBD   |TBD  |

&ensp;&ensp;**[RVM](https://github.com/EDI-Systems/M7M02_Ammonite)** 嵌入式虚拟机监视器虚拟化版本：

|芯片          |架构        |工具链  |Yield |Mail |Sem  |FIFO |Msgq |Bmq |Mail/I|Sem/I|Msgq/I |Bmq/I |Mem  |
|:-----------:|:----------:|:----:|:---:|:---:|:---:|:---:|:---:|:---:|:----:|:---:|:-----:|:----:|:---:|
|STM32L071CB  |Cortex-M0+  |Keil  |469  |841  |717  |391  |1143 |1529 |1484  |1376 |1646   |1867  |N/A  |
|...          |...         |GCC   |TBD  |TBD  |TBD  |TBD  |TBD  |TBD  |TBD   |TBD  |TBD    |TBD   |TBD  |
|STM32F405RG  |Cortex-M4   |Keil  |TBD  |TBD  |TBD  |TBD  |TBD  |TBD  |TBD   |TBD  |TBD    |TBD   |TBD  |
|...          |...         |GCC   |TBD  |TBD  |TBD  |TBD  |TBD  |TBD  |TBD   |TBD  |TBD    |TBD   |TBD  |
|STM32F767IG  |Cortex-M7   |Keil  |221  |403  |348  |150  |589  |765  |942   |962  |1134   |1227  |334  |
|...          |...         |GCC   |TBD  |TBD  |TBD  |TBD  |TBD  |TBD  |TBD   |TBD  |TBD    |TBD   |TBD  |
|CH32V307     |RV32IMAFC   |GCC   |TBD  |TBD  |TBD  |TBD  |TBD  |TBD  |TBD   |TBD  |TBD    |TBD   |TBD  |

&emsp;&emsp;作为对比，RT-Linux 4.12在Cortex-M7上的最好线程切换时间是25000时钟周期（它太大了以至于只能从FMC SDRAM来运行，所以这个对比事实上很不公平）。这是使用futex测得的；如使用其他IPC如管道等，则结果更差。 

&emsp;&emsp;以上实验均**不包含任何作弊手段**（比如激进的针对特定工具链的优化、不经过调度器而切换线程、使用平均情况较快但最坏执行时间不可控的调度器，甚至是使用RMS式的无栈协程切换），且在测试头文件中报告的最坏执行时间均为真实值。尽管我们为了更普遍地进行对比而在列出平均值，但对RTOS而言，最坏执行时间是唯一有意义的时间，因此一切有利于平均性能但不利于最坏执行时间的启发式优化都不适用。如果追求速度是最高目的，**不可能有操作系统比RMS或DOS快**，因为RMS的理论切换时间是0（所有协程均内联且只有一个状态时），而作为单任务操作系统的DOS则根本不存在切换时间。 

### 明确不考虑支持的架构
|架构           |原因           |替代方案                                                             |
|:-------------:|:------------:|:------------------------------------------------------------------:|
|PIC18          |硬件堆栈       |使用[RMS状态机操作系统](https://github.com/EDI-Systems/M2A01_Simpron) |
|AVR32          |正在消亡       |使用大众化的Cortex-M和Cortex-R                                       |
|x86-64         |高级系统       |使用[RME微内核操作系统](https://github.com/EDI-Systems/M7M01_Eukaron) |
|Cortex-A       |高级系统       |使用[RME微内核操作系统](https://github.com/EDI-Systems/M7M01_Eukaron) |

## 新手上路

&emsp;&emsp;下面的说明会帮助你在本地快速建立一个可用来评估测试本系统的工程。请参看系统的中文文档以获取更多信息。

### 准备工作

&emsp;&emsp;要运行测试，你需要一块基于 **_Cortex-M或RISC-V或MIPS或MSP430_** 的开发板。本RTOS主要面向资源受限的MCU，不提供对高端MCU，MPU和CPU的特别支持。不要使用QEMU模拟器来测试本系统，因为QEMU有很多不完善之处，与真正的硬件行为并不一致。

&emsp;&emsp;如果你没有开发板，那么RMP也有一个 **_基于x86处理器的Linux移植_** 。然而，该移植使用了[ptrace](https://en.wikipedia.org/wiki/Ptrace)系统调用和[信号](https://en.wikipedia.org/wiki/Signal_(IPC))系统，因此并不很快，这一点可以从性能测试的数据看出。

&emsp;&emsp;对于其他平台的支持应该也是容易实现的，但是当前并没有支持计划。对于那些Cortex-A和具备内存管理单元（[MMU](https://en.wikipedia.org/wiki/Memory_management_unit)）的其他处理器，可以使用[M7M1_MuEukaron](https://github.com/EDI-Systems/M7M1_MuEukaron) _实时多核心微内核_；M7M1也支持一部分的Cortex-M和全部的Cortex-R。

### 编译指南

&emsp;&emsp;在 **_Project_** 文件夹下能够找到多种微控制器的移植好的 **厂商集成开发环境** 或 **Eclipse** 的工程样板。参看各个工程文件夹下的自述文件以获取更多关于如何编译和运行该工程的信息。某些工程需要额外的厂商硬件抽象层库的支持，它们可以在 **[M0A00_Library](https://github.com/EDI-Systems/M0A00_Library)** 软件仓库被找到。

## 运行测试

&emsp;&emsp;要运行测试，只要将测试下载到对应的开发板并开始单步调试即可。某些例子会采用两个LED来指示系统当前的状态，此时要填充LED的点亮和熄灭函数来运行该示例。

&emsp;&emsp;要使用本系统内建的图形库，请参考用户手册的相关章节。

## 生产部署

&emsp;&emsp;当部署本系统到生产环境时，请仔细阅读本系统自带的手册，以确保各项配置正确。本系统的手册可以在 **_Documents_** 文件夹下找到。

## 支持的工具链

- Keil uVision (ARMCC/ARMCLANG)
- Code Composer Studio
- MPLAB X XC16/XC32
- GCC/Clang-LLVM

&emsp;&emsp;其他的工具链现在不推荐或者当前不受支持，虽然要增加新的支持应该也很简单。

## 参与项目

&emsp;&emsp;请阅读[CONTRIBUTING.md](CONTRIBUTING.md)文档来获得关于行为规范和提交代码的相关信息。

## EDI 工程信息
- M5P01 R5T2

## 杰出贡献者
- 宋磊锋 - ARM Cortex-M3/4/7 的GCC汇编支持。
- 侯润升 - ARM Cortex-M4/7 的RVM支持，以及lwIP示例。
- 王逸鹤 - 稳定的x86/linux/ptrace移植。
- 张然 - C28x数字信号处理器移植。
- 张凯 - 白盒测试用例。
- 刘皓天 - RL78的移植。