<h1 align="center">
	<img width="300" src="Document/Public/Demo/logo.png" alt="logo">
</h1>

# One RTOS (RMP)
[![Github release](https://img.shields.io/github/release/EDI-Systems/M5P01_Prokaron.svg)](https://github.com/EDI-Systems/M5P01_Prokaron/releases/latest)
[![Github commits](https://img.shields.io/github/commits-since/EDI-Systems/M5P01_Prokaron/main@{30day}.svg)](https://github.com/EDI-Systems/M5P01_Prokaron/compare/main@{30day}...main)
![language](https://img.shields.io/badge/language-C-orange.svg)
[![Build](https://dl.circleci.com/status-badge/img/circleci/NY2jeZsLjp6awG9G8CrUsi/Cusv1XhXjvcKuDsRnRrmZC/tree/main.svg?style=shield)](https://dl.circleci.com/status-badge/redirect/circleci/NY2jeZsLjp6awG9G8CrUsi/Cusv1XhXjvcKuDsRnRrmZC/tree/main)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/1684/badge)](https://bestpractices.coreinfrastructure.org/projects/1684) 
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/69a95540047e47d08670fe3ea85dcaaa)](https://app.codacy.com/gh/EDI-Systems/M5P01_Prokaron/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
[![Join the chat at https://gitter.im/M5P01_Prokaron/Lobby](https://badges.gitter.im/M5P01_Prokaron/Lobby.svg)](https://gitter.im/M5P01_Prokaron/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Click **[HERE](README.md)** for English version.

&emsp;&emsp;**RMP** 是一个专注于形式化验证和简易可用的小型实时系统。它通过采取形式化方法来确保系统的可靠性（当前验证工作还未完成；不过，100%的白盒测试分支覆盖率已经达到。目前的内核可以被看作IEC 61508 SIL2预认证等级，或者EAL 4）。所有的实时操作系统必备的功能它都具备，但是并不在此基础上提供更多可选组件以确保内核的精炼性。这样，得到的内核就是一个最小化的内核，可以很方便地对它进行形式化验证。同时，它还可以作为客户操作系统运行在虚拟机监视器上。

&emsp;&emsp;本系统比以 _FreeRTOS_ 和 _RT-Thread_ 为代表的全功能系统相比要**小**得多，而且理解起来应该也相对容易得多。即便本系统仅仅包含了**一个.C**文件，它仍然提供了**高效的内存管理**，**抗锯齿图形界面库**和**其他实用函数**，并且这些功能的实现都**不消耗任何额外的RAM**！

&emsp;&emsp;本系统的手册可以在 **[这里](https://github.com/EDI-Systems/M5P01_Prokaron/blob/main/Documents/M5P1_%E8%BD%BB%E9%87%8F%E7%BA%A7%E5%AE%9E%E6%97%B6%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E%E4%B9%A6.pdf)** 找到。

&emsp;&emsp;如果想要参与开发，请阅读 **[参与](CONTRIBUTING.md)** 和 **[规范](CODE_OF_CONDUCT.md)** 两个指导文档。如果要提交拉取请求，请使用 **[拉取请求模板](PULL_REQUEST_TEMPLATE.md)** 。
本软件是EDI的官方作品，因此属于 **公有领域** 。 所有由EDI保留的版权在所有适用的法律条款下尽最大可能地授权给所有实体。

&emsp;&emsp;对于那些由微控制器厂商提供的硬件抽象层软件包，请到 **[M0A00_Library](https://github.com/EDI-Systems/M0A00_Library)** 软件仓库自行下载。

## 快速演示
### 可在Linux上直接运行的Demo
&emsp;&emsp;从 **[这里](Project/ECLIPSE-GCC-LINUX/RMP/Debug/RMP)** 下载预编译的x86-32位Linux系统上可运行的二进制，观察性能测试的运行结果！

### 内建图形库：控件，界面样例和FXAA抗锯齿细节
![Controls](/Document/Public/Demo/Controls.png)
![Calculator](/Document/Public/Demo/Calc.png)
![FXAA](Document/Public/Demo/FXAA.png)

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
        RMP_Thd_Crt(&Thd_1, Func_1, &Stack_1[238], (void*)0x12345678, 1, 5);
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
        RMP_Thd_Crt(&Thd_1, Func_1, &Stack_1[238], (void*)0x12345678, 1, 5);
        RMP_Thd_Crt(&Thd_2, Func_2, &Stack_2[238], (void*)0x87654321, 1, 5);
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

&emsp;&emsp;Flash和SRAM消耗以kB计，其他数据以CPU指令周期计。下表列出的所有值都是**典型（有意义的系统配置）值**而非绝对意义上的**最小值**，因为纯技术层面的最小配置在实际工程中很少是真正有用的。HAL库所造成的额外存储器消耗也被计算在内。  

&emsp;&emsp;本系统的**绝对最小值**在**1.6k ROM和432Byte RAM**左右，这个大小是在HC32L136K8TA（Cortex-M0+）的移植上达到的，并包括了第一个线程的60Byte的线程控制块和256Byte的线程栈，以及64Byte的内核中断响应用栈。操作系统内核和最精简的HAL库一共仅占用了**52Byte**存储。如果你对这个数字还有不满意，那么可以**不使用厂商提供的HAL库**而自己写一个版本。  

|架构          |工具链        |Flash|SRAM|Yield|Mail |Sem  |Mail/Int|Sem/Int|Mem  |
|:-----------:|:------------:|:---:|:--:|:---:|:---:|:---:|:------:|:-----:|:---:|
|DSPIC33E     |XC16-GCC      |4.46 |1.15|526  |828  |750  |914     |884    |579  |
|MSP430       |TI CCS7       |2.90 |0.64|495  |906  |786  |830     |736    |1575 |
|Cortex-M0    |Keil uVision 5|4.94 |1.65|374  |663  |616  |659     |617    |N/A  |
|Cortex-M0+   |Keil uVision 5|6.25 |1.65|334  |607  |544  |588     |552    |N/A  |
|Cortex-M3    |Keil uVision 5|5.31 |1.65|252  |513  |448  |465     |418    |311  |
|Cortex-M4    |Keil uVision 5|5.46 |1.66|188  |386  |353  |361     |329    |233  |
|Cortex-M7    |Keil uVision 5|6.66 |1.65|196  |288  |277  |296     |296    |183  |
|Cortex-M7    |GCC           |7.71 |1.98|176  |313  |276  |290     |268    |193  |
|Cortex-M7-RVM|Keil uVision 5|2.09 |2.29|1068 |1256 |1195 |884     |866    |176  |
|Cortex-M7-RVM|GCC           |2.15 |2.10|1103 |1277 |1225 |907     |866    |177  |
|Cortex-R4    |TI CCS7       |15.1 |1.42|281  |458  |406  |424     |368    |274  |
|Cortex-R5    |TI CCS7       |18.2 |3.72|305  |471  |426  |472     |432    |267  |
|MIPS M14k    |XC32-GCC      |17.2 |2.46|263  |378  |358  |430     |420    |211  |
|RV32IMAC     |GCC           |2.24 |2.89|261  |585  |506  |~800**  |~800** |N/A  |
|X86-LINUX    |GCC           |N/A  |N/A |33000|35000|33000|35000   |33000  |136  |

*作为对比，RT-Linux 4.12在Cortex-M7上的最好线程切换时间是25000时钟周期。这是使用futex测得的；如使用其他IPC如管道等，则结果更差。  

**该值仅供参考；评估所使用的器件依赖于SPI Flash来运行代码，有时候一个指令缓存落空就会导致45000周期的延迟。传统上，依赖于外部SPI Flash的器件在做测量时需要很大的内部内存来运行这些代码，但是该器件没有如此多的内存可供使用。


- DSPIC33E平台使用DSPIC33EP512MU810进行评估。
- MSP430平台使用MSP430FR5994进行评估。
- Cortex-M0平台使用STM32F030F4P6进行评估。
- Cortex-M0+平台使用STM32L053C8T6进行评估。
- Cortex-M3平台使用STM32F103RET6进行评估。
- Cortex-M4平台使用STM32F405RGT6进行评估。
- Cortex-M7平台使用STM32F767IGT6进行评估。
- Cortex-M7-RVM平台使用STM32F767IGT6进行评估，而且RMP此时是作为客户机运行在 **[RVM](https://github.com/EDI-Systems/M7M2_MuAmmonite)** 嵌入式虚拟机监视器上。
- Cortex-R4平台使用TMS570LS0432进行评估。
- Cortex-R5平台使用TMS570LC4357进行评估。
- MIPS M14k平台使用PIC32MZ2048EFM100进行评估。
- RV32IMAC平台使用FE310-G000进行评估。
- X86 Linux平台使用Ubuntu 16.04和i7-4820k @ 3.7GHz进行评估。

&emsp;&emsp;所有的编译器优化选项都被设为最高（通常是-O3），而且时间优化选项也被打开。 
- Yield   ：两线程间进行切换所用的时间。  
- Mail    ：两线程间使用邮箱进行发送-接收操作的耗时。  
- Sem     ：两线程间使用计数信号量进行发布-获取操作的耗时。  
- Mail/Int：从中断发送到某线程邮箱的耗时。 
- Sem/Int ：从中断发布信号量的耗时。 
- Mem     ：进行一次内存操作（比如分配或释放）的用时。 

### 将来可能支持的新架构
|架构           |原因           |支持优先度                     |
|:-------------:|:------------:|:----------------------------:|
|RL78           |常用16位单片机 |:star::star::star::star::star:|
|TI C2000       |常用DSP架构   |:star::star::star::star:      |
|MicroBlaze     |常用软核      |:star::star:                  |
|NIOS II        |常用软核      |:star:                        |

### 明确不考虑支持的架构
|架构           |原因           |替代方案                                                             |
|:-------------:|:------------:|:------------------------------------------------------------------:|
|PIC18          |硬件堆栈       |使用[RMS状态机操作系统](https://github.com/EDI-Systems/M2A1_MuSimpron)|
|AVR32          |正在消亡       |使用大众化的Cortex-M和Cortex-R                                        |
|ARMv5          |被替代        |使用更新的Cortex-M和Cortex-R                                          |
|x86-64         |高级系统       |使用[RME微内核操作系统](https://github.com/EDI-Systems/M7M1_MuEukaron)|
|Cortex-A       |高级系统       |使用[RME微内核操作系统](https://github.com/EDI-Systems/M7M1_MuEukaron)|
|Coldfire       |正在消亡      |使用大众化的Cortex-M和Cortex-R                                        |
|PowerPC        |正在消亡      |使用大众化的Cortex-M和Cortex-R                                        |
|RX100/600/600S |小众架构      |使用大众化的Cortex-M和Cortex-R                                        |
|Tricore        |小众架构      |使用大众化的Cortex-M和Cortex-R                                        |
|MB91460        |小众架构      |使用大众化的Cortex-M和Cortex-R                                        |

## 新手上路

&emsp;&emsp;下面的说明会帮助你在本地快速建立一个可用来评估测试本系统的工程。请参看系统的中文文档以获取更多信息。

### 准备工作

&emsp;&emsp;要运行测试，你需要一块基于 **_Cortex-M或Cortex-R或MIPS或MSP430_** 的开发板。本RTOS主要面向资源受限的MCU，不提供对高端MCU，MPU和CPU的特别支持。不要使用QEMU模拟器来测试本系统，因为QEMU有很多不完善之处，与真正的硬件行为并不一致。

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

- Keil uVision 5 (armcc)
- Code composer studio
- MPLAB X XC32
- GCC/Clang-LLVM

&emsp;&emsp;其他的工具链现在不推荐或者当前不受支持，虽然要增加新的支持应该也很简单。

## 参与项目

&emsp;&emsp;请阅读[CONTRIBUTING.md](CONTRIBUTING.md)文档来获得关于行为规范和提交代码的相关信息。

## EDI 工程信息
- M5P01 R5T2

## 杰出贡献者
- 宋磊锋 - ARM Cortex M3/4/7的GCC汇编支持.
- 侯润生 - ARM Cortex-M4/7 的RVM支持，以及lwIP示例.
- 王逸鹤 - 稳定的x86/linux/ptrace移植.
- 张然 - C28x数字信号处理器移植.
- 张凯 - 白盒测试用例。
