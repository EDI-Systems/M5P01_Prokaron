# M5P01_Prokaron (RMP) 项目维护指南（供 AI Agents 使用）

> 本文档由 AI agent 撰写并持续维护，用于帮助后续 agents 快速理解本项目。
> 每次对项目做出结构性或重要的修改后，请同步更新本文档。
> 最后更新：2026-11-05（基于 main 分支，版本 R6T1）。
> §11 是 A8MM / STM32N657X0H3（Cortex-M55）移植，§12 是 A8MB / M2351KIAAE（Cortex-M23）移植；
> 两者三工具链均已上板，M2351 使用 HXT→PLL 时钟与免 Keil 的 FMC-ISP 烧录流程。

## 目录

| 节 | 内容 |
|---|---|
| §1 | 项目是什么、姊妹仓库 |
| §2 | 目录结构 |
| §3 | 内核设计要点（调度/IPC/内存/GUI/命名） |
| §4 | 平台移植层结构与支持的架构 |
| §5 | 测试体系与"基准必须存档"硬规矩 |
| §6 | 代码风格 |
| §7 | 构建/验证环境（本机工具链与硬件） |
| §8 | 修改注意事项（经验之谈） |
| §9 | 快速命令（构建/烧录/采集） |
| §10 | 早期 IAR 移植记录（F429 / G071 / H743） |
| §11 | A8MM + STM32N657X0H3（Cortex-M55，三工具链） |
| §12 | A8MB + M2351KIAAE（Cortex-M23，三工具链） |

---

## 1. 项目是什么

**RMP** 是 EDI-Systems 开发的小型实时操作系统内核（RTOS），仓库编号 **M5P01**，当前版本 **R6T1**。核心特点：

- **单文件内核**：全部内核功能（调度、IPC、内存管理、GUI 图形库）都在 `Source/Kernel/rmp_kernel.c` 一个文件中（约 8650 行）。
- **极简 + 形式化验证导向**：只提供 RTOS 必备功能，不提供更多可选组件。项目三阶段路线：① hobbyist 原型（已完成）→ ② 从原型反向提炼形式化模型（**当前阶段**）→ ③ 用 C 子集语义重写内核并证明正确性，产出 IEC 61508 SIL4 / ISO 26262 ASIL D 认证文档。目前已达成 **100% 白盒测试分支覆盖率**，可视为 SIL2 / EAL4 预认证等级。
- **公有领域许可**（The Unlicense），EDI 官方作品。
- **面向微控制器**（不支持微处理器/MMU/多核）；Cortex-A 或带 MMU 的处理器请用姊妹项目 RME（M7M01_Eukaron）；不可抢占的 PIC18 用 RMS 状态机 OS（M2A01_Simpron）。
- 可作为**客户操作系统**运行在 RVM 虚拟机管理器（M7M02_MuAmmonite）之上，为此专门有 `_RVM` 系列移植。
- 厂商 HAL 库（STM32 HAL 等）在独立仓库 **M0A00_Library**，本仓库不包含。

### 姊妹仓库索引
| 仓库 | 作用 |
|---|---|
| M0A00_Library | 厂商硬件抽象层（部分工程编译时需要） |
| M2A01_Simpron | RMS 状态机 OS（无栈协程，PIC18 等替代方案） |
| M7M01_Eukaron | RME 微内核 OS（Cortex-A / MMU 平台） |
| M7M02_MuAmmonite | RVM hypervisor，本仓库 `_RVM` 移植的宿主 |

---

## 2. 目录结构

```
M5P01_Prokaron/
├── Include/                     # 头文件
│   ├── rmp.h                    # 顶层包含入口（三段式包含 platform + kernel 头）
│   ├── Kernel/rmp_kernel.h      # 内核头：类型/宏/错误码/数据结构/API 原型（~920 行）
│   ├── Platform/<ARCH>/         # 各 CPU 架构平台头
│   │   ├── rmp_platform_<arch>.h        # 架构层（类型、栈、YIELD、INT_MASK 等宏）
│   │   └── Chip/<CHIP>/rmp_platform_<chip>.h  # 芯片层（系统参数 + 底层时钟/串口初始化宏）
│   └── Test/Chip/rmp_test_<chip>.h      # 各芯片测试台头（计时器/中断初始化 + 基准结果存档）
├── Source/
│   ├── Kernel/rmp_kernel.c      # ★ 单文件内核（调度/IPC/TLSF 内存/报警/GUI 控件）
│   ├── Platform/<ARCH>/         # 平台移植 .c + .s（汇编上下文切换；按工具链分文件）
│   ├── Hook/rmp_hook.c          # 空白钩子模板（RMP_Init_Hook / RMP_Init_Idle）
│   ├── Hook/rmp_hook_rvm.c      # RVM 版钩子模板
│   └── Test/                    # 四个测试程序（见 §5）
├── Project/                     # 各 IDE/芯片工程，目录名前缀 = 工具链
│   ├── GCCMF-*    # GCC Makefile（F767IGT6-LWIP 等为 lwIP 示例）
│   ├── RVMDK-*    # Keil uVision（STM32F429ZIT6-COV 为覆盖率测试专用工程）
│   ├── EWARM-*    # IAR EWARM（F429/G071/H743/N657X0H3/M2351KIAAE）
│   ├── CCS-*      # Code Composer Studio（MSP430/C28x/TMS570）
│   ├── ATS-*      # Atmel Studio（AVR）
│   ├── MPLAB-*    # MPLAB X（dsPIC33/PIC24/PIC32）
│   ├── MRS-*      # MounRiver Studio（CH32 RISC-V）
│   ├── E2STUDIO-/FreedomStudio-/UNSPIDE-/CC65MF-  # 其他 IDE
│   ├── CPPCHECK-LINT   # cppcheck 静态检查工程（平台用 LINT 虚架构）
│   └── GCCMF-X86L      # ★ 无硬件也能跑的 x86-32 Linux/ptrace 移植（CI 用）
└── Document/
    ├── Public/    # 用户手册（PDF/DOC：CN_M5P01_R6T1 / EN_M5P01_R4T5）+ Demo 图片/NES ROM
    └── Private/   # 内部文档（Dia 图源文件：线程状态转移图等；本 AGENTS.md 也在此）
```

**注意**：每个 `Project/<工程>/` 内含两个关键用户配置文件：`rmp_platform.h`（指向所用的架构/芯片头）和 `rmp_test.h`（指向所用测试芯片头）。工程通过 `-I` 把自己的目录放在 Include 路径最前面来实现"同构复用、按工程配置"。

---

## 3. 内核设计要点（rmp_kernel.c）

- **调度**：抢占式、位图优先级调度。`RMP_Bitmap[]` + 每优先级就绪链表 `RMP_Run[RMP_PREEMPT_PRIO_NUM]`（优先级数由芯片头配置，如 32）。同优先级时间片轮转（`Slice`/`Slice_Left`）。数字越小优先级越高；0 保留给空闲（Init 线程），用户线程从 1 开始。
- **调度锁**：`RMP_Sched_Lock/Unlock` 可嵌套（`RMP_Sched_Lock_Cnt`），基于中断屏蔽 `RMP_INT_MASK()/UNMASK()` 实现；解锁时若有 pending 切换则 `RMP_YIELD()`。内核 API 内部大量依赖此机制保证原子性。
- **上下文切换**：由 `RMP_YIELD()`（通常触发 PendSV/软件中断）→ 汇编完成；`_RMP_Run_High()` 选出最高优先级线程。刻意**没有**"新旧线程相同则跳过切换"的快速路径（作者注释：牺牲平均性能换取最坏执行时间确定性，RTOS 只看 WCET）。
- **线程状态机**（低 8 位状态 + 高位标志）：`FREE/READY/SNDBLK/SNDDLY/RCVBLK/RCVDLY/DELAY/SEMBLK/SEMDLY`，标志位 `SUSPEND/MBOXFUL`。状态转移图见 `Document/Private/Dia/`。
- **IPC 极简**：① **线程邮箱**（每线程 1 个消息槽 `Mail_Snd/Mail_Rcv`，`RMP_Thd_Snd/Rcv`，无额外 RAM）；② **计数信号量**（`RMP_Sem_*`）。扩展对象（由 Sem+Fifo 组合实现）：**FIFO、消息队列 Msgq、阻塞消息队列 Bmq**。刻意不做事件标志组/互斥量等（便于形式化验证，见 CONTRIBUTING.md）。
- **内核对象全部由用户静态分配**（结构体如 `struct RMP_Thd` 放在用户内存），内核不做对象创建/回收管理——这是 RMP 与 FreeRTOS/RT-Thread 的重大区别。
- **定时**：`RMP_Timestamp` 单调递增；延迟链表 `RMP_Delay` 按超时升序；支持 **tickless**（`_RMP_Tim_Elapse/_RMP_Tim_Future` 计算最近到期点再编程定时器）。所有阻塞 API 均可带超时（对应 `*DLY` 状态）。
- **内存管理**：TLSF 变体分配器（FLI/SLI 两级位图 + 空闲链表表），`RMP_Mem_Init/RMP_Malloc/RMP_Free`。池最小 1024 机器字、需字对齐。
- **报警管理器**：`RMP_Amgr/RMP_Alrm`（oneshot/周期回调）。
- **GUI 库**（`RMP_GUI_ENABLE` 条件编译）：画线/圆/矩形/位图矩阵、FXAA 抗锯齿、控件（复选框/按钮/单选/进度条/行编辑）、光标。不消耗额外 RAM。
- **volatile 全覆盖策略**：所有共享数据结构均 volatile，等价于统一内存屏障，牺牲少量性能换 LTO 安全性。**修改内核时不要去掉 volatile**。IAR 会因此报约 80 条 Pa082 警告（语句内多个 volatile 访问顺序未定义）——这是已知且可接受的。
- **可裁剪配置宏**（在芯片头中定义）：`RMP_ASSERT_ENABLE / RMP_CHECK_ENABLE / RMP_DBGLOG_ENABLE / RMP_PREEMPT_PRIO_NUM / RMP_SLICE_MAX / RMP_SEM_CNT_MAX / RMP_INIT_STACK_SIZE / RMP_GUI_* / RMP_STKSEG_ENABLE` 等。

### 错误码
`RMP_ERR_THD(-1)/PRIO(-2)/SLICE(-3)/STACK(-4)/STATE(-5)/OPER(-6)/SEM(-7)/MEM(-8)/FIFO(-9)/MSGQ(-10)/BMQ(-11)/AMGR(-12)/ALRM(-13)`；成功返回 0。

### 命名规范（严格遵守）
- 用户 API：`RMP_<对象>_<操作>`（如 `RMP_Thd_Crt`、`RMP_Sem_Pend`），无下划线前缀；内部函数 `_RMP_<...>`；ISR 安全版本加 `_ISR` 后缀。
- 类型 `rmp_ptr_t/rmp_cnt_t/rmp_ret_t/...` 由平台头定义（16 位机上 `rmp_ptr_t` 是 16 位！）。

---

## 4. 平台移植层结构

每个架构需提供（以 `A7M` 为例）：

| 文件 | 职责 |
|---|---|
| `Include/Platform/A7M/rmp_platform_a7m.h` | 类型/`RMP_WORD_ORDER`/栈类型与对齐/`RMP_MSB_GET`/`RMP_INT_MASK`/`RMP_YIELD` 抽象宏；声明 `_RMP_Start/_RMP_Stack_Init/_RMP_Lowlvl_Init/RMP_Putchar` |
| `Source/Platform/A7M/rmp_platform_a7m.c` | 上述函数实现（无内联汇编，编译器无关） |
| `Source/Platform/A7M/rmp_platform_a7m_<toolchain>.s` | 汇编上下文切换/启动。**后缀 = 编译器**：`_armcc.s`(Keil)、`_gcc.s`(GCC)、`_iccarm.s`(IAR)。**例外：A8MM/A8MB 都没有 `_armcc.s`**——ARMv8.1-M 起 armasm 私有格式废弃且 AC6 拒绝 Cortex-M55，Keil 直接用 armclang 集成汇编器编译 `_gcc.s`（见 §11.4） |
| `Include/Platform/A7M/Chip/<CHIP>/rmp_platform_<chip>.h` | 芯片级：HAL 包含、系统参数宏、`RMP_A7M_LOWLVL_INIT()` 时钟+串口初始化宏 |

**新增芯片/架构的固定套路**：平台头 → 平台 .c → 各工具链汇编 → Chip 头 → `Include/Test/Chip/rmp_test_<chip>.h` 测试台 → `Project/` 建工程（含 `rmp_platform.h`+`rmp_test.h` 转发文件）→ 测试头注释中存档基准数据 → 更新 README 性能表格（如适用）。

`_RVM` 变体把中断屏蔽/yield 换成 `RVM_Virt_*` 超调用。

### 支持的架构（Platform 目录命名）
| 目录名 | 架构 | 目录名 | 架构 |
|---|---|---|---|
| A6M | ARMv6-M (Cortex-M0) | MP32P | MIPS32 (PIC32) |
| A7M | ARMv7-M (Cortex-M3/4/7) | MSP430 | TI MSP430 |
| A7R | ARMv7-R (Cortex-R, TMS570) | RL78 | Renesas RL78 |
| A8MM | ARMv8-M Mainline (Cortex-M33/M35P/M55/M85，含 MVE/Helium) | RV32P | RISC-V 32 |
| A8MB | ARMv8-M Baseline (Cortex-M23，无 BASEPRI/CLZ/FPU) | | |
| AVR | Microchip AVR | UNSP | 凌阳 SPCE061A |
| C28X | TI C28x DSP | X86L | x86-32 Linux (ptrace) |
| DSPIC | dsPIC33/PIC24 | LINT | cppcheck 专用虚拟架构 |
| M6502 | 6502（红白机 FAMICOM！） | *_RVM | 运行于 RVM hypervisor 上的客户机版 |
---

## 5. 测试体系

| 文件 | 用途 | 说明 |
|---|---|---|
| `Source/Test/rmp_benchmark.c` | 性能基准 | 测 Yield/Mailbox/Sem/FIFO/Msgq/Bmq/Malloc/ISR 变体的 AVG/MAX/MIN 周期数。**禁止修改此文件**，芯片相关内容都在测试头里。结果存档在各 `rmp_test_<chip>.h` 头部注释 |
| `Source/Test/rmp_coverage.c` | 白盒覆盖率 | 模拟中断（非真实触发）遍历所有分支。覆盖率标记 `RMP_COV_MARKER()` → `RMP_Cov[]` 位图。配套 Keil 工程 `RVMDK-STM32F429ZIT6-COV`，编译时定义 `RMP_COV_LINE_NUM` |
| `Source/Test/rmp_tickless.c` | tickless 功能测试 | 需实现 4 个 Hook + 定时器重编程（1 tick=10ms 假设） |
| `Source/Test/rmp_lwip_iperf.c` | lwIP iperf 网络基准 | 用于 `-LWIP` 后缀工程 |

**测试台头** `rmp_test_<chip>.h` 提供：`RMP_CNT_READ()`（读硬件计数器）、`rmp_tim_t` 类型、`TEST_MEM_POOL`、`Timer_Init/Int_Init/Int_Handler/Int_Disable`（真实周期中断源）。

**CI**：CircleCI（`Project/GCCMF-X86L/circleci.config.yml`），安装 gcc-multilib 后 `cd Project/GCCMF-X86L && make`。

**基准结果必须存档（项目硬规矩）**：在板上跑完基准后必须完成两件事，缺一不可：
1. 把完整结果（12 项的 AVG/MAX/MIN）以注释形式**存档进对应 `Include/Test/Chip/rmp_test_<chip>.h` 头部**——每个工具链一段（如 `ARMCLANG 6.23 -O3`、`GCC 13.2.1 -O3`、`IAR 9.40.1 -Oh`），带 ASCII 横幅，格式对齐已有条目；
2. 在 **README.md 与 README_CN.md** 的 "Useful ports" 性能表格中新增/更新对应芯片行（同一芯片不同工具链各一行，后续行用 `...` 缩写芯片名和平台列）。
未存档的跑分等于没跑。修改平台汇编/编译选项等影响性能的内容后，必须重测并同步更新这两处存档。

**已存档三工具链的芯片**：STM32F429ZI / G071RB / H743ZI / N657X0H3 / M2351KIAAE（N657 见 `rmp_test_stm32n657x0h3.h` 与 §11.7，M2351 见 `rmp_test_m2351kiaae.h` 与 §12.10）。

---

## 6. 代码风格（改代码前必读，详见 CONTRIBUTING.md）

- **文件头**：`Filename/Author/Date/Licence/Description` 五字段框注释；文件尾版权注释。
- **分段注释**：`/* Include ***...***/`、`/* Define */`、`/* Global */`、`/* Function:Name ***...***/`（含 Description/Input/Output/Return）成对分隔注释。
- **头文件三段式包含**：`__HDR_DEF__`（宏与类型）→ `__HDR_STRUCT__`（结构体）→ `__HDR_PUBLIC__`（extern 原型），每段 include 后立即 `#undef`。**新建头文件必须沿用**（模板见 CONTRIBUTING.md）。
- 缩进 4 空格；K&R 大括号；宏全大写 `RMP_` 前缀；数字带 `U` 后缀；内核大量用 `#if(X!=0U)` 而非 `#ifdef`。
- 汇编文件按**编译器**命名：`.s` gcc / `_armcc.s` Keil / `_iccarm.s` IAR / `_ticc.s|asm` CCS / `_ccrl.asm` Renesas。
- Git commit 用 gitmoji：`:memo:` 文档、`:green_heart:` CI/测试、`:arrow_up:` 升级、`:rocket:` 新功能、`:bug:` 修 bug。
- 文档必须随代码同步更新；公共 API 变化要同步：`rmp_kernel.h` 原型、实现、覆盖率测试、（必要时）README 与用户手册（Word/PDF 在 Document/Public）。


---

## 7. 构建/验证环境备忘（Windows 本机）

- 工具链安装情况：arm-none-eabi-gcc 13.2.1（`C:\Toolchain\arm-none-eabi-mingw64`）、Keil MDK（`C:\Keil_v5_ARM`，ARMCLANG 6.23）、**IAR EWARM 9.40.1**（安装目录名仍是 `C:\Program Files\IAR Systems\Embedded Workbench 9.2`，但 `iccarm --version` 报 V9.40.1.364——这正是 §5 各芯片 IAR 基准所用的版本，别被目录名误导）、STM32CubeProgrammer、CubeIDE 1.18（自带 ST OpenOCD，N6 烧录必需）等。另有便携 Ruby 3.4.5（`C:\Toolchain\ruby34`），供 §11.5 的 pack2iar 转换用。
- 硬件在环验证：NUCLEO-F429ZI 已通过板载 ST-LINK（SWD）连接，烧录用 `STM32_Programmer_CLI.exe -c port=SWD -d xxx.out -v -rst`，活性检查可热插拔读 SysTick `0xE000E018` 两次比较。**NUCLEO-N657X0-Q** 走完全不同的流程（BOOT 跳线 DEV boot + OpenOCD RAM 载入），见 §11.2。
- **NuMaker-PFM-M2351**（新唐，板载 Nu-Link）：SWD 调试与虚拟串口都走同一根 USB（VID 0416/PID 511D，VCP 枚举为 "Nu-Link Virtual Com Port"，COM 号随插拔变化）。烧录不走 Keil，用 `Document/Private/Agent_Workspace/flash_m2351_isp.py`（OpenOCD + FMC ISP），见 §12.9；串口采集必须先在 PC 端断言 DTR/RTS，见 §12.5。
- 本仓库在 Windows 下无法运行 X86L 端口（需 Linux+gcc-multilib+ptrace）；cppcheck 静态检查工程在 `Project/CPPCHECK-LINT`。
- README 明确警告**不要用 QEMU 测试**。
- `Project/` 下 IDE 的临时文件（`.metadata/`、`JLinkLog.txt`、`.jxbrowser.userdata/` 等）已入库且含大量噪音数据——**搜索代码时务必限定 `Source/`、`Include/` 路径**。

---

## 8. 修改注意事项（经验之谈）

1. **内核单文件原则**：不要拆分 `rmp_kernel.c`，不要往内核加可选组件（网络栈/驱动框架/动态对象管理都被作者拒绝）。
2. 改内核必须同步：① `rmp_coverage.c` 新分支加 `RMP_COV_MARKER()`；② 各配置组合（16/32 位、FPU、`RMP_GUI_ENABLE` 等）可编译；③ 最坏执行时间不能变差；④ 各工具链汇编变体保持行为一致（A7M/A6M 是 armcc/gcc/iccarm 三份，**A8MM/A8MB 只有 gcc/iccarm 两份**，见 §11.1）。
3. **不要"优化"掉 volatile、`RMP_COV_MARKER()`、空 `else` 分支**——空 else 是覆盖率标记的载体。
4. 平台宏（如 `RMP_A7M_LOWLVL_INIT`）是用户可重定向的默认实现，保持宏形式。
5. 基准数据是 README 卖点：任何影响性能的改动需真机重测，并按 §5 的硬规矩同步存档进测试头注释和 README 双语表格。
6. Keil 工程的 `HSE_VALUE="8000000"` 带引号写法是 MDK 命令行去引号约定；**IAR 工程必须写不带引号的 `HSE_VALUE=8000000`**，否则 HAL 中该宏成为字符串导致编译错误（Pe032/Pe513）。
7. 新建 IAR 工程时两类警告都要在 .ewp 里屏蔽：① `Pa082`（volatile 访问顺序，约 80 条）→ ICCARM 块的 `CCDiagSuppress`；② `410`（CODE 段中的 DC 数据定义，DC16 手工编码指令必然触发）→ AARM 块设 `AWarnEnable=1 / AWarnWhat=1 / AWarnOne=410`（AARM 数据块版本须 ≥12）。注意 iasmarm 的 `-w` 参数只能出现一次，**不能**用 AExtraOptionsV2 传 `-w-410`（与构建系统自动生成的 `-w+` 冲突，报 "Option multiply defined"）。
8. 本文档需持续维护：重要修改后更新日期与对应章节。

---

## 9. 快速命令

```bash
# Linux：构建并运行 x86 基准（唯一免硬件运行方式）
cd Project/GCCMF-X86L && make && ./Object/RMP

# Windows：IAR 命令行编译（本仓库已验证）
cd Project/EWARM-STM32F429ZIT6
"C:\Program Files\IAR Systems\Embedded Workbench 9.2\common\bin\iarbuild.exe" RMP.ewp -build Debug

# Windows：烧录到 NUCLEO-F429ZI
"C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe" -c port=SWD -d Debug\Output\RMP.out -v -rst

# Windows：STM32N657X0H3 三工具链构建（RAM 镜像，均 0 错 0 警）
cd Project/GCCMF-STM32N657X0H3 && make                              # GCC 13.2.1
cd Project/RVMDK-STM32N657X0H3 && "C:\Keil_v5_ARM\UV4\UV4.exe" -r RMP.uvprojx -j0 -o build.log
cd Project/EWARM-STM32N657X0H3 && "C:\Program Files\IAR Systems\Embedded Workbench 9.2\common\bin\iarbuild.exe" RMP.ewp -build Debug

# Windows：N657 烧录（DEV boot；OpenOCD 脚本见 §11.2，重链后要改 resume 地址）
# Document/Private/Agent_Workspace/n657_flash.ocd      ← GCC 版
# Document/Private/Agent_Workspace/n657_flash_keil.ocd ← Keil 版
# Document/Private/Agent_Workspace/n657_flash_iar.ocd  ← IAR 版

# Windows：M2351KIAAE 三工具链构建
cd Project/GCCMF-M2351KIAAE && make
cd Project/RVMDK-M2351KIAAE && "C:/Keil_v5_ARM/UV4/UV4.exe" -r RMP.uvprojx -j0 -o build.log
cd Project/EWARM-M2351KIAAE && "C:/Program Files/IAR Systems/Embedded Workbench 9.2/common/bin/iarbuild.exe" RMP.ewp -build Debug

# Windows：M2351 烧录（不依赖 Keil；Keil/GCC/IAR 的 .bin 都能直接喂）
python Document/Private/Agent_Workspace/flash_m2351_isp.py Project/RVMDK-M2351KIAAE/obj/RMP.bin
#   GCC: Project/GCCMF-M2351KIAAE/Object/RMP.bin
#   IAR: Project/EWARM-M2351KIAAE/Debug/Exe/RMP.bin

# Windows：M2351 采集（先烧完再复位；脚本内含 DTR/RTS 断言）
powershell -File Document/Private/Agent_Workspace/capture_m2351_ocd.ps1 -Seconds 30
```
---

## 10. 早期 IAR 移植记录（F429 / G071 / H743，2026-10-21）

本机新增了 STM32F429 的 IAR 工程 `Project/EWARM-STM32F429ZIT6/`（命名规则：`EWARM-` 前缀 = IAR 工具链，与 `GCCMF-`/`RVMDK-` 等并列）：

- **新增源文件**：`Source/Platform/A7M/rmp_platform_a7m_iccarm.s`——A7M 平台的 IAR 汇编变体（iasmarm 语法，**后缀 = 编译器 iccarm**，与 `_armcc.s`/`_gcc.s` 并列）。与 armcc 版逐函数等价；PendSV 的 FPU 保存/恢复沿用原版手工编码（`DC16 0xBF08/0xED20|0xECB0/0x8A10` = `IT EQ`+条件 VSTM/VLDM），机器码与 Keil 构建**位相同**。
- **iasmarm 语法要点**：没有 `DCI` 伪指令（armasm 才有），CODE 段中用 `DC16` 报 Warning[410]，已在工程中屏蔽（见 §8 第 7 条）；**禁止改用真指令**（`IT EQ`+`VSTMDBEQ`）替代手工编码——无 FPU 配置下真指令汇编失败，会破坏单文件多配置一致性，这正是原作者用 DCI/DC16 手工编码的原因；字面量池需文件尾显式 `LTORG`；`MODULE` 可省 `ENDMOD`（会被忽略并警告）；无 `PROC/ENDP`，导出用 `PUBLIC`、导入用 `EXTERN`。
- **工程文件**：`RMP.eww` + `RMP.ewp`（fileVersion 3；设置块版本号必须与选项名集合自洽：General v31 / ICCARM v35 / AARM v10 / ILINK v22，均已在 EWARM 9.2 实测）。无 `.ewd`，首次用 GUI 打开时自动生成默认调试设置。
- **器件/核**：通用 Cortex-M4（`CoreVariant=39`）+ VFPv4 单精度（`FPU2 state=4`、`DSPExtension=1`）+ 小端；`OGCoreOrChip=0` 用通用核而非具体器件（避免器件表索引依赖）。入口 `__iar_program_start`，启动文件用 ST 官方 IAR 版 `Templates\iar\startup_stm32f429xx.s`（Keil 用的是 `Templates\arm\` armasm 版）。
- **链接**：`IlinkIcfOverride=1` 指向 ST 官方 `stm32f429xx_flash.icf`。注意 ILINK v22 的有效选项名只到 `FillerEnd`，之后版本的名（如 `IlinkBE8Slaves`）会触发 "unknown name" 加载错误。
- **输出布局（对齐 Keil RVMDK）**：`Debug\Output\`（顶层为 `RMP.out`/`RMP.hex`；.o 在 IAR 9.x 原生的 `<组名>_<哈希>.dir` 子目录里，防重名，无法展平）+ `Debug\Listings\`（`RMP.map`）。HEX 用 `ielftool --ihex RMP.out RMP.hex`。
- **与 Keil 的宏差异**：IAR 定义 `HSE_VALUE=8000000`、`PLL_M=8`（不带引号，原因见 §8 第 6 条）。
- **验证状态（2026-10-21 最终）**：iarbuild 编译 **0 错误 / 0 警告**——`Pa082` 由 ICCARM `CCDiagSuppress` 抑制，`410` 由 AARM v12 的 `AWarnEnable=1/AWarnWhat=1/AWarnOne=410` 抑制（命令行合成为 `-w-410`）；iccarm/iasmarm 均为 `--cpu=Cortex-M4 --fpu=VFPv4_sp`。`RMP.out` 已下载到 NUCLEO-F429ZI 并校验、复位运行；SysTick 活性检查通过。
- **gitignore 策略（.gitignore 的 `#IAR specific` 段）**：忽略 `Project/EWARM-*/settings/`（IDE 用户状态 .crun/.dbgdt/.wsdt/.dnx/cspy 脚本）、`*.ewt`（GUI 自动生成的工程模板）、`*.ewd`（GUI 自动生成的默认调试设置，对齐 Keil 段忽略 `*.uvopt*` 的惯例）、`*.ewp.bak/*.eww.bak` 及浏览信息（`*.pbi/*.pbd/*.pbw/*.browse`）；IAR 的 `Debug/` 输出树由既有的 `Project/**/Debug/` 覆盖。**保留入库**：`RMP.eww`/`RMP.ewp`（工程本体）与三个转发头。基准采集工具与原始数据存放在 `Document/Private/Agent_Workspace/`（agent 专属工作区，已在 .gitignore 中整目录忽略，永不入库）。若日后手工配置好 ST-LINK 调试并希望随库分发 `.ewd`，需显式移除该忽略行。
- **基准已采集并存档（IAR 9.40.1 -O3）**：Yield 160、Mail 345、Sem 292、FIFO 188、Msgq 574、Bmq 776、Alrm 775、Mem 416、Mail/I 354、Sem/I 277、Msgq/I 502、Bmq/I 610（周期数）。已按 §5 硬规矩存档进 `rmp_test_stm32f429zi.h` 头部注释（第三段）并在 README.md/README_CN.md 的 F429 条目下新增 IAR 行。
- **基准采集方法（可复用）**：基准只跑一次，须先开串口再复位板子。采集脚本在 `Document/Private/Agent_Workspace/capture_benchmark.ps1`（agent 专属，不入库，支持 `-PortName`/`-Seconds` 参数）：打开 ST-LINK VCP（115200-8-N-1，具体 COM 号随板子枚举变化，用 `Get-CimInstance Win32_SerialPort` 查）→ `STM32_Programmer_CLI -c port=SWD mode=UR -rst`（SWD 与 VCP 互不冲突）→ 循环 `ReadExisting()` 采集（基准约 20 秒跑完，尾部 "10000 overflows" 属正常）→ 写入同目录 `benchmark_capture.txt`。注意命令行窗口 30 秒超时会杀掉前台进程，须用 `Start-Process` 后台运行。

### 10.1 EWARM-STM32G071RBT6 工程（2026-10-21 新增，A6M/Cortex-M0+）

- **新增源文件**：`Source/Platform/A6M/rmp_platform_a6m_iccarm.s`（A6M 的 IAR 变体）。A6M 无 FPU/BASEPRI/CLZ：PendSV 用 `SUBS #36`+双 `STMIA` 方案，MSB/LSB 用内核 C 的 `RMP_MSB_Generic`，全部真指令、无 DC16。
- **工程模板**：直接复制用户 GUI 保存的 F429 最新款 .ewp（fileVersion 4、特定器件选择 + IAR 内置 icf 的路线），再逐项适配。**两工程共性**：`CCDiagSuppress=Pa082`、AARM v12 的 `AWarnEnable=1/AWarnWhat=1/AWarnOne=410`。G071 器件串 `STM32G071RB	ST STM32G071RB`（OGChipSelectEditMenu/GFPUDeviceSlave 两处都有）。
- **核心表（CoreVariant，v33 语境）**：**36=Cortex-M0+**（实测命令行 `--cpu Cortex-M0+` 验证）、34=Cortex-M0、38=M3、39=M4、41=M7、58=ARMv8-M-BL。M0+ 需 `FPU2=0/NrRegs=0/DSPExtension=0`。
- **★ 器件宏陷阱（G0 特有）**：G0 的 `stm32g0xx.h` 要求具体型号宏（如 `STM32G071xx`）。Keil 工程虽只写 `STM32G0xx`，但 **MDK 会按器件选择自动注入 `-DSTM32G071xx`**；IAR 必须自己定义（CCDefines：`STM32G071xx, STM32G0xx, USE_HAL_DRIVER`），否则全部文件 `#error`。
- **★ 分段写入汇编文件的血泪教训**：`rmp_platform_a6m_iccarm.s` 初版因分段写入把 `_RMP_A6M_Yield` 腰斩（`DSB`/`BX LR` 被孤立在 `END` 之后成死文本，函数直接跌落进 PendSV），症状是首次上下文切换即 HardFault、仅 2 字节输出。**写汇编文件必须整体一次成文，或写完后逐函数核对完整性**。本次定位手段：面包屑打印（RMP_Init_Hook 内 A-D + Func_2 首行 E + Idle 首次 I）+ ICSR 读出 HardFault/PendSV 状态 + ielfdumparm 逐字节核对机器码。
- **链接脚本**：G071 起用本地副本 `stm32g071xB_local.icf`（复制 IAR 内置版，**CSTACK 从 0x400 加到 0x1000** 作为稳健余量；原 1KB 并非故障根因，但余量更安全）。使用自定义 icf 时 `IlinkIcfOverride` 必须=1，否则链接器静默使用器件默认 icf。ST 官方 G0 iar 资产：`Templates\iar\startup_stm32g071xx.s` + `Templates\iar\linker\stm32g071xx_flash.icf`（本工程用 IAR 内置改版）。
- **调试配置**：`settings/RMP.Debug.{general,driver}.xcl` 为手工版 cspybat 配置（Cortex-M0+ / STM32G071RB.ddf / STM32G0xx.dmac）。G071 未开 GUI 前 .ewd 不存在，GUI 打开即自动生成（默认 SIM 驱动，需手选 ST-LINK）。
- **验证状态（2026-10-21）**：0 错误 / 0 警告；镜像烧录 NUCLEO-G071RB（Device ID 0x460）校验复位；基准完整跑通并按 §5 硬规矩存档（IAR 9.40.1 -O3：Yield 298、Mail 545、Sem 480、FIFO 252、Msgq 762、Bmq 1012、Alrm 1062、Mail/I 532、Sem/I 471、Msgq/I 643、Bmq/I 757；G071 无 Mem 项），README 双语已加 IAR 行。

### 10.2 EWARM-STM32H743ZIT6 工程（2026-10-21 新增，A7M/Cortex-M7 双精度 FPU）

- **无需新汇编**：H743 与 F429 同属 A7M 平台，直接复用 `Source/Platform/A7M/rmp_platform_a7m_iccarm.s`。
- **工程**：复制 F429 GUI 保存版 .ewp 适配。与 F429 的差异仅四处：`CoreVariant/GFPUCoreSlave2/GBECoreSlave=41`（Cortex-M7）、**`FPU2=7`（FPv5 双精度 d16）**、器件串 `STM32H743ZI\tST STM32H743ZI`、宏 `STM32H743xx, USE_HAL_DRIVER, HSE_VALUE=8000000`（H7 的 Keil 工程无 PLL_M）。核心值均取自 CMSIS DSP 工程的 `cortexM7lfdp` 配置（M7=41，FPU2：0=无/4=VFPv4_sp/6=FPv5_sp/**7=FPv5_d16**）。
- **注意**：H7 的 Keil 包含路径顺序与 F4 不同（`CMSIS\Include` 在 HAL 之前、Device Include 在最后），.ewp 已按 Keil 原序镜像。
- **链接**：IAR 内置 `stm32h743xI.icf`（ZI 型号，处理 H7 复杂内存布局：DTCM/AXI SRAM 等），`IlinkIcfOverride=0` 走器件默认；启动文件用 ST Cube H7 的 `Templates\iar\startup_stm32h743xx.s`。
- **验证状态（2026-10-21）**：**已上板验证**——镜像烧录 NUCLEO-H743ZI（Device ID 0x450，ST-LINK V2，VCP=COM14）校验复位，基准完整跑通（12 项含 Mem）。IAR 9.40.1 -O3 结果已按 §5 硬规矩存档：Yield 176、Mail 308、Sem 264、FIFO 148、Msgq 524、Bmq 718、Alrm 535、Mem 297、Mail/I 284、Sem/I 274、Msgq/I 396、Bmq/I 521，README 双语 IAR 行已同步为真实数据。构建阶段即 0 错误 / 0 警告，首次上板一次点亮（A7M iccarm 汇编在 M7 上无任何适配问题）。

## 11. A8MM 平台与 STM32N657X0H3 移植（2026-11-02/03，Cortex-M55 + Helium）

GCC / Keil / IAR 三工具链均已上板跑通 12 项基准。本节按"平台层 → 板级 → 各工具链工程 → 基准"分层。

### 11.1 平台层（工具链无关）

**新增文件**：
- `Include/Platform/A8MM/rmp_platform_a8mm.h` — 平台头（类型/栈/`RMP_MSB_GET`/`RMP_YIELD`），只覆盖 Mainline
- `Source/Platform/A8MM/rmp_platform_a8mm.c` — 重写 2018 年死代码（旧版引用已删除的 `Platform/CortexM` 头、用 3 参数旧接口）
- `Source/Platform/A8MM/rmp_platform_a8mm_gcc.s` — GNU 语法，GCC 与 Keil/AC6 共用（无 `#if`，无需 cpp）
- `Source/Platform/A8MM/rmp_platform_a8mm_iccarm.s` — iasmarm 语法，IAR 专用
- **没有 `_armcc.s`**：ARMv8.1-M 起 armasm 私有格式废弃，且 AC6 的 armasm 兼容层直接拒绝 Cortex-M55（`armasm does not support CPU 'cortex-m55'`）

A8MM 只支持 **Mainline**（M33/M35P/M55/M85，含 MVE）；Baseline（M23）缺 BASEPRI/CLZ，是独立的 A8MB 端口（见 §12）。

**MVE 上下文切换（结论有 ARM 107706 + FreeRTOS CM55 端口双重佐证）**：
- MVE 新增的架构状态只有 **VPR**，而它由硬件在 extended frame 中**自动压栈**（与 S0-S15/FPSCR 同机制，惰性压栈同样覆盖）
- 软件只需条件保存 S16-S31（沿用 A7M 的 `.hword`/`DC16` 手工编码 VSTMDB/VLDMIA），判断位仍是 **EXC_RETURN bit4**（MVE 没有新增判断位）
- FPCCR 需置 `ASPEN|LSPEN`（芯片头显式写 `FPU->FPCCR`——CMSIS 里它不在 SCB 上）
- 新线程初始 `LR_EXC=0xFFFFFFFD`（标准帧起步），线程首次执行 FP/MVE 指令后硬件自动切换为扩展帧
- 帧长：无 MVE 26 字（0x68），有 MVE 28 字（0x70），VPR 在 **+0x68**、+0x6C 是对齐保留字；软件不寻址帧内槽位

**★ A8MM/A8MB 共有坑：`_RMP_Start` 必须先清零 PSPLIM 再写 PSP。** 系统复位不清核心寄存器，接管前固件（ST ROM monitor / 前一个镜像）残留的 PSPLIM 会让线程态首个异常压栈触发 `UFSR.STKOF` → HardFault。GDB 抓帧定位（CFSR=0x00100000）。

### 11.2 STM32N6 板级工作流（本机实测）

- **无内部 flash**：镜像由调试器载入 AXISRAM1 安全别名（`0x34000400` 起）直接运行。GCC 需 `-mcmse`（ST 的 `system_stm32n6xx_s.c` 用到 `SCB_NS`/CMSE 属性）+ `-mcpu=cortex-m55 -mfpu=auto -mfloat-abi=hard`。
- **必须 DEV boot**：BOOT1 跳线接 **2-3**（BOOT0 保持 1-2）。生产 boot（跑出厂 demo）时调试总线被锁：AP 读返回**假数据**（固定值、总线事务根本没发生）、CubeProgrammer 所有写操作走扇区引擎必失败、只有 DP 层和 UR 连接可用。改跳线后要**拔 USB 断电**一次。
- **★ `0x34200000` 是雷区**（AXISRAM3 未上电，Keil DFP 里该区也标了 `default=0`）：任何访问会把 AXI 矩阵挂死——症状是复读陈旧 TAR、跨会话存活、NRST 救不回，**只有整片断电恢复**（本项目踩过两次）。所有链接脚本都把镜像锁在 `0x34000400–0x34100000`（AXISRAM1 内）。
- **烧录流水线**：CubeProgrammer 对 N6 只读，必须用 CubeIDE 自带的 ST OpenOCD：
  - 工具：`plugins/com.st.stm32cube.ide.mcu.externaltools.openocd.win32_2.4.100.202501161620/tools/bin/openocd.exe`
  - 脚本：`plugins/com.st.stm32cube.ide.mcu.debug.openocd_2.3.100.202501240831/resources/openocd/st_scripts`
  - 关键点：`dapdirect_swd`、**核心在 AP1**、`-defer-examine` + 先写 DP ABORT `0x1E` 清粘性错、`reset halt` 回到纯净 HSI64 时钟、载入+校验、`resume <Reset_Handler+1>`
  - 脚本：`Document/Private/Agent_Workspace/n657_flash.ocd`(GCC) / `n657_flash_keil.ocd` / `n657_flash_iar.ocd`；采集 `capture_n657.ps1`（COM15）
  - **重链后必须同步 resume 地址**（各工具链产物不同：GCC 用 `arm-none-eabi-nm Object/RMP.elf | grep Reset_Handler`，Keil 看 `fromelf`/map，IAR 看 `Debug/Output/RMP.out`）
- ST 的 OpenOCD fork **无 SRST 能力**（"BUG: can't assert SRST"）；ST-LINK GDB Server 对本板不可用（halt 竞争不过时钟门控）。
- **VCP 是 USART1/PE5/AF7**（不是其他 Nucleo 的 USART3/PD8）。出厂 demo 留着 IWDG，芯片头与测试头 TIM4 中断里各喂一次 `IWDG->KR=0xAAAA`（关掉时是 no-op）。`RMP_CNT_READ` 用 DWT CYCCNT（此 CMSIS 版 DWT 结构体无 LAR 成员，M55 复位后默认未锁）。
- **600MHz 时钟树**（照抄官方 NUCLEO-N657X0-Q 例程）：`HAL_PWREx_ConfigSupply(PWR_EXTERNAL_SOURCE_SUPPLY)`（板载 SMPS）+ VOS1 + HSI→PLL1(M4×N75=1200MHz)→IC1/2=CPU 600MHz、SYS 400MHz、HCLK 100MHz、APB1=100MHz；SysTick=600000、TIM4=20000(5kHz)；L1 I/D cache 均开。
  - 频率自验证：基准尾部 `overflows` 行间隔 × 频率 = 655.36（溢出行是 16 位计数器回绕计数）。实测间隔 1.104s → **593.7MHz**（HSI ±1% 容差内）。
  - cache 生效实证：`SCB->CCR(0xE000ED14)=0x00030201`（bit17=IC、bit16=DC）；周期数 1160(64MHz 无缓存)→163(600MHz 有缓存)，周期数与频率无关，7 倍降幅即缓存旁证。

### 11.3 GCC 工程（`Project/GCCMF-STM32N657X0H3/`）

makefile + LRUN 式链接脚本 + 三转发头 + 本地 `stm32n6xx_hal_conf.h`（HAL 来自 `M0A00_Library/STM32Cube_FW_N6_V1.1.0`，注意本机这份是 **Drivers-only 精简版**，没有 `Utilities/`/`Projects/`）。

- `CPU=-mcpu=cortex-m55 -mthumb -mfpu=auto -mfloat-abi=hard`，`CFLAGS` 必带 `-mcmse`
- HAL conf 只开 TIM/UART/GPIO/RCC/CORTEX/**DMA**（N6 的 TIM 头无条件引用 `DMA_HandleTypeDef`）
- 链接脚本：RAM `0x34000400` + 1023k（到 `0x34100000`），`_estack=0x34100000`
- 构建：`make`；产物 `Object/RMP.elf/.bin`，入口用 `arm-none-eabi-nm` 查

### 11.4 Keil 工程（`Project/RVMDK-STM32N657X0H3/`）

**DFP**：`Keil.STM32N6xx_DFP`（`C:\Keil_v5_ARM\Packs\Keil\`——TOOLS.INI 里 RTEPATH 指向的才是真 pack 根，`ARM\PACK\` 只是老式索引，容易误判"没装"）；器件名 `STM32N657X0HxQ`（BGA264，Cortex-M55 + DP_FPU + MVE + TZ）。

- **汇编走 armclang 集成汇编器**（`<ClangAsOpt>2</ClangAsOpt>`），复用 `rmp_platform_a8mm_gcc.s`
- **IAS 与 GNU as 的差异**：`.extern` 后**紧跟**的独立 `/* */` 注释行报 "unexpected token at start of statement"（`.global`/`.section`/标签后都没事）。因此 Import 段的说明集中在段首，且最后一个 `.extern` 与 `/* End Import */` 之间**必须留空行**——这两处别"整理"掉，否则 GCC 照常而 Keil 报错（本项目踩过一次回归）
- uVision 5.42 的器件→汇编 CPU 映射表不认识 M55（armasm 模式会传 `--cpu NOT_SUPPORTED`），但 IAS 模式下由 `<Cpu>` 串的 `CPUTYPE("Cortex-M55")` 决定，**不需要任何 Misc 补丁**
- **MicroLIB 栈符号**：ST 的 N6 **C** 启动（`Templates/arm/startup_stm32n657xx.c`）自己没有 STACK 区，靠 scatter 的 `ARM_LIB_STACK`；但 MicroLIB 的 entry2 还要传统符号 `__initial_sp`。解法：工程内加 `rmp_platform_stm32n657x0h3_sp.s` 用 `.set __initial_sp, 0x34100000`。**不要**关 MicroLIB（`useUlib=0` 会转去要 `Image$$ARM_LIB_STACK$$ZI$$Base/Limit`）
- **scatter 要真正生效**：`<umfTarg>`=**0**（=1 时 uVision 无视 `<ScatterFile>` 自己生成，症状是链接报缺 `Image$$ARM_LIB_STACK$$ZI$$*`）、`<useFile>`=1、路径写 `.\\rmp_platform_stm32n657x0h3.sct`（写 `.\Output\...` 会静默回退）；`-mcmse` 的 veneer 要显式段 `CMSE_VENEER +0 { *(Veneer$$CMSE) }`，否则 L6405E
- **必须包含 `stm32n6xx_hal_dma_ex.c`**（TIM/UART 引用 `HAL_DMAEx_*`；GCC 侧靠 gc-sections 掩盖了这点）
- 构建：`UV4.exe -r RMP.uvprojx -j0 -o build.log`（`-r` 全量重建，增量会掩盖汇编配置错误）；出 bin：`fromelf --bin --output RMP.bin Debug/Output/RMP.axf`

### 11.5 IAR 器件支持安装（本机 IAR 9.40.1 无 STM32N6 内置器件）

**结论**：本机 `Embedded Workbench 9.2` 目录里 `iccarm` 实际是 **V9.40.1.364**，编译器/汇编器/链接器**都支持 Cortex-M55 + MVE**，缺的只是器件描述。IAR 不像 Keil 有独立 DFP——器件支持随 EW 大版本发布（**9.60.3 起原生内置 STM32N6**），否则靠补丁或 CMSIS-Pack。

**已走通的路线：用 IAR 自带的 pack2iar 把 Keil 的 DFP 转成 IAR 格式。**
- pack2iar 本体是 `common/CMSIS-Manager/plugins/com.iar.cmsis.pack2iar_1.0.0.202210260911.jar` 里的 `ruby-src/pack2iar.rb`。Windows 上本应由 `pack2iar.exe`（ocra 打包）执行，**但本机没有这个 exe**，需自备 Ruby
- 依赖 `nokogiri`/`rubyzip`/`builder`；**Ruby 4.0 没有 nokogiri 预编译包**，用 **RubyInstaller 3.4.5 便携版**（`C:\Toolchain\ruby34\rubyinstaller-3.4.5-1-x64`，解压即用、不含 DevKit）可命中 `x64-mingw-ucrt` 预编译包
- 命令：`ruby pack2iar.rb -p <pack根目录> -o <输出> -d --generate-pack` → 本次 N6 得到 **96 个 `.iar` 文件**（24 器件的 .i79/.ddf/.icf）
- 转换质量（N657X0HxQ）：`[CORE] name=Cortex-M55`、`fpu=VFPv5_d16`、`SIMD=true`、`trustzone=true`、`[FLASH LOADER]` 空（正确）、DDF 的 `[SfrInclude]` 指向 `STM32N657.svd`、`debug __ap="1"`（与实测 AP1 一致）
- **必须手工改 ICF**：pack2iar 产出的是通用模板（`IROM` 为空、`IRAM` 指非安全别名 `0x24180400`）。已改为 `intvec@0x34000400`、ROM `0x34000400–0x3407FFFF`、RAM `0x34080000–0x340FFFFF`、CSTACK `0x1000`、HEAP `0x400`
- **落地**：
  - pack 已装到 `%USERPROFILE%\IAR-CMSIS-Packs\Keil\STM32N6xx_DFP\1.2.0\`（含 `.iar`）→ 供 **CMSIS-Pack 模式**
  - **原生器件模式**需写 IAR 安装目录（管理员）：`Document/Private/Agent_Workspace/install_iar_n657_device.ps1 -AlsoAliasH3`。脚本把 `.i79/.menu/.ddf/.icf/.svd` 复制到位，并把 `${CMSIS_PACK_PATH_...}$` 改写成 `$TOOLKIT_DIR$`/`$CUR_DIR$`。写完**重启 IAR**
  - 可分发产物：`Agent_Workspace/Keil.STM32N6xx_DFP.1.2.0_iar.pack`（3.2MB，含 `.iar`）
- **升级警告**：原生器件文件在安装目录内，**升级/重装 EWARM 会被覆盖**，需重跑脚本。升到 9.60.3+ 后器件名会变成 IAR 原生风格（如 `STM32N657X0`），与 pack2iar 的通配名 `STM32N657X0HxQ` 不同

### 11.6 IAR 工程（`Project/EWARM-STM32N657X0H3/`）

依赖 §11.5 的器件支持（工程里器件串就是 `STM32N657X0H3`）。

**iasmarm 汇编要点**：`MODULE`/`SECTION .text:CODE:NOROOT(3)`/`THUMB`/`REQUIRE8`/`PRESERVE8`；标签顶格**无冒号**；`PUBLIC`/`EXTERN`；用 **`DC16`**（不是 armasm 的 `DCI`）手工编码 IT+VSTM/VLDM——CODE 段里必然触发 **Warning[410]**，靠 AARM 块 `AWarnEnable=1/AWarnWhat=1/AWarnOne=410` 抑制（**不能**用 AExtraOptionsV2 传 `-w-410`，与自动生成的 `-w+` 冲突报 Option multiply defined）；文件尾必须显式 `LTORG` 再 `END`。`MSR PSPLIM` 用真指令即可。

**工程字段**（模板取自 `EWARM-STM32F429ZIT6`，fileVersion 4 / General v36 / ICCARM v38 / AARM v12 / ILINK v27）：
- `OGChipSelectEditMenu`/`GFPUDeviceSlave` = `STM32N657X0H3<TAB>ST STM32N657X0H3`
- `OGCoreOrChip=1`（器件模式，`--cpu` 由 .i79 的 `[CORE]` 决定）；`CoreVariant/GBECoreSlave/GFPUCoreSlave2/OG_32_64DeviceCoreSlave=59`、`FPU2=7`（FPv5 d16）、`DSPExtension=1`
- `CCDefines` = `STM32N657xx, USE_HAL_DRIVER`（**不要** HSE_VALUE/PLL_M）
- `IlinkIcfOverride=1` + `IlinkIcfFile=$PROJ_DIR$\rmp_platform_stm32n657x0h3.icf`（本地副本，上界 `0x340FFFFF`）
- 启动文件 = ST 的 `Templates\iar\startup_stm32n657xx.s`（内含 `MSR MSPLIM` + `sfe(CSTACK)`）
- HAL 源同样必须含 `stm32n6xx_hal_dma_ex.c`；`CCDiagSuppress=Pa082`

**构建**：`iarbuild.exe RMP.ewp -build Debug` → 0 错 0 警；产物 `Debug/Output/RMP.out`，出 bin 用 `ielftool RMP.out --bin RMP.bin`。

**生成脚本坑**（`Agent_Workspace/gen_ewarm_n657.py` + `fix_ewarm_n657.py`）：F429 模板有**两组** `<group>`，用 `rindex` 只会替换尾部、残留旧 F429 文件组（症状：构建出现 `startup_stm32f429xx.s`/`rmp_platform_a7m_iccarm.s`），必须用 `index` 从第一个 group 替换到 EOF；ICF 路径在模板里是正斜杠 `$TOOLKIT_DIR$/config/...`，别按反斜杠匹配。

### 11.7 三工具链基准对比（600MHz，L1 cache 开，均已上板）

| 测试 | GCC 13.2.1 -O3 | ARMCLANG 6.23 -O3 | IAR 9.40.1 -Oh |
|---|---|---|---|
| Yield | 163 | 155 | 162 |
| Mailbox | 298 | 281 | 303 |
| Semaphore | 252 | 228 | 262 |
| FIFO | 135 | 130 | 154 |
| Message queue | 391 | 421 | 456 |
| Blocking message queue | 524 | 571 | 607 |
| Alarm (1/2/3/5/7) | 527 | 555 | 631 |
| Memory alloc/free | 307 | 309 | 330 |
| ISR Mailbox | 289 | 282 | 290 |
| ISR Semaphore | 256 | 246 | 260 |
| ISR Message queue | 336 | 364 | 375 |
| ISR Blocking msgq | 427 | 454 | 449 |

均与 Cortex-M7@400MHz 同量级（M7 Yield 164）。

已按 §5 存档进 `rmp_test_stm32n657x0h3.h` 头部注释与 README 双语表格（三行：GCC/Keil/IAR）。

**内核修复提示**：`RMP_Sem_Post/_ISR` 的溢出检查在无符号类型下必须写成 `(Num_Cur+Number)<Num_Cur`；写成 `<=Number` 会把 `Num_Cur==0` 的合法首次 Post 也拦掉，症状是 Semaphore 阶段调度位图全零 → `rmp_kernel.c:884` 断言 panic（两个工具链同现，曾误判为 Keil 移植问题）。


## 12. Nuvoton M2351KIAAE 移植（A8MB / Cortex-M23，2026-11-05）

GCC / Keil / IAR 三工具链均已上板跑通 12 项基准，烧录不依赖 Keil。

### 12.1 端口与平台层（A8MB）

- 独立端口 **A8MB**（`Include/Platform/A8MB/`、`Source/Platform/A8MB/`），与 Mainline 的 A8MM 平行：无 BASEPRI、无 CLZ、无 FPU，只有 5 个系统异常（MemManage/BusFault/UsageFault 全部升级为 HardFault），有 PSPLIM。
- 每个端口只有一份实现、一个剖面，没有 `RMP_A8M_COP_MAINLINE` 这类选择宏：
  - `RMP_MSB_GET`/`RMP_LSB_GET` 直接用内核的 `RMP_MSB_Generic`/`RMP_LSB_Generic`（A8MB 不提供 CLZ 版）；
  - `RMP_INT_MASK`/`RMP_INT_UNMASK` 指向本端口的 `RMP_Int_Mask`，内部用 PRIMASK 实现（`RMP_Int_Unmask` 紧跟其后，16 位条件分支不越界）。
- 汇编两份，都没有 `#if`，都保持小写 `.s`（不依赖 cpp）：
  - `rmp_platform_a8mb_gcc.s` — GNU 语法，GCC 与 Keil/AC6 共用
  - `rmp_platform_a8mb_iccarm.s` — iasmarm 语法，IAR 专用
- 与 A8MM 的实质差异：`PendSV_Handler` 用 `SUBS #36 + STMIA + MOV` 手工搬 9 个字（Baseline 的 STMIA 只认低寄存器，也没有 `STMDB`）；无 FPU，硬件帧恒为 8 字。
- `NuMicro.h` 把 GPIO PORTC 定义成裸宏 `PC`，撞 `struct RMP_A8MB_Stack` 的 `PC` 成员——芯片头 include 之后必须 `#undef PC`。
- **三个工程各有一个 `rmp_stubs.c`（约 20 行），不是可选项**：Nuvoton 原厂的 `startup_M2351.S` 在 `HardFault_Handler` 里 `BLX ProcessHardFault`、在 `SH_DoCommand` 里 `BLX SH_Return`，但 startup **自己并不定义**这两个符号；BSP 只在 `retarget.c`（944 行，带 printf/semihosting）里给。既然约束是"不换厂商 startup"，就必须自己补。实测把该文件从工程里摘掉，三个工具链都会报 `undefined reference to ProcessHardFault` / `SH_Return`（GCC 下报在 `startup_M2351.S:346` 与 `:482`）。
  - 签名必须匹配厂商原型：`uint32_t ProcessHardFault(uint32_t LR, uint32_t MSP, uint32_t PSP)`、`int32_t SH_Return(int32_t R0, int32_t R1, int32_t* Out)`。写成 `void(void)` 虽然也能链接（因为永不返回），但属于 ABI 不匹配。
  - **不要**在 `ProcessHardFault` 里往 `0x20000000` 写标记——那是 `.bss` 内存池的起点，真出 fault 时会顺带破坏内核数据。

### 12.2 时钟（全部由 12MHz 晶振起）

```c
CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);            CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);
CLK_EnablePLL(CLK_PLLCTL_PLLSRC_HXT, FREQ_48MHZ);  CLK_WaitClockReady(CLK_STATUS_PLLSTB_Msk);
CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_PLL, CLK_CLKDIV0_HCLK(1));
CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_PLL, CLK_CLKDIV0_UART0(1));
```

整条链：**HXT(12MHz 晶振) → PLL(48MHz) → HCLK/PCLK0 → TIMER0**，UART0 也挂在同一个 PLL 上，所以时基与波特率都是晶振级精度。

- **PLL 必须从 HXT 起**。`CLK_SetCoreClock(FREQ_48MHZ)` 默认用 HIRC 当 PLL 源，在本板会死在 `CLK_EnablePLL`（PC 停在 `clk.c` 该函数内，线程模式）；用 HXT 起则 HXTSTB/PLLSTB 都能等到。
- **UART0 挂 PLL 是安全的**：`UART_Open` 对 PLL 源有专门处理——`au32ClkTbl[1] = CLK_GetPLLClockFreq()`，按运行期真实 PLL 频率算分频。（早先"UART 不能挂 PLL、时钟表里 PLL 项是 0"的结论是错的，已实测推翻。）
- 运行期核对（OpenOCD `mdw`）：HCLKSEL=2(PLL)、HCLKDIV=0、PCLKDIV=0、TMR0SEL=2(PCLK0)、UART0SEL=1(PLL)。
- **时间基准实测**（基准固件自己打印 `10000 overflows`，每 65536 个 TIMER0 计数算一次）：5 个间隔均值 **13.6502 s**（PLL 版）/ 13.6528 s（早期 HIRC48 版），对 48MHz 理论值 13.6533 s 分别差 -0.02% / -0.004%；另用 SWD 轮询 `TIMER0->CNT` 积分得 47.93 MHz。两法互相印证 CPU 时钟就是 48MHz。

### 12.3 初始线程栈必须 ≥1024 字节（血泪）

`RMP_INIT_STACK_SIZE` 若是 256，会被 Nuvoton 的调用链撑爆：

```
RMP_Init_Hook → Timer_Init → TIMER_Open → TIMER_GetModuleClock
             → CLK_GetPCLK0Freq → SystemCoreClockUpdate → CLK_GetPLLClockFreq
```

溢出约 12 字节，而 `RMP_Init_Stack` 后面紧挨着的就是 `.bss` 里的 `RMP_Delay`；链表头 `Next` 被写成 5，下一个 SysTick 遍历延时队列时 `RMP_DLY2THD(5)` 得到 `0xFFFFFFFD`，读 `[r0+44]` 直接 HardFault（PC 落在项目自带的 `ProcessHardFault` 桩里；CFSR/HFSR 读 0 是因为读的是非安全别名，M2351 的安全态 SCB 在 `0xE002EDxx`）。

定位手法（可复用）：对 `RMP_Delay` 设写观察点 `wp 0x20008178 8 w`，命中时打 `reg pc/psp`，再用 `addr2line` 一查便知是栈下溢。

### 12.4 基准计时器（TIMER0）

- TIMER0 是 24 位计数器，`RMP_CNT_READ()` 取低 16 位，`rmp_tim_t` 用 `rmp_u16_t`，差值按 65536 回绕——与 STM32F103RB 端口同一做法。仓库基准的 ISR 收尾统计本来就按 16 位计数器写（注释里的 655.36），别用 u32。
- `TIMER_Open(TIMER0, TIMER_CONTINUOUS_MODE, 1U)` 会按 1Hz 自动算成 3 分频 + `CMP=16e6`，**必须**再覆盖：

```c
TIMER0->CTL &= ~TIMER_CTL_PSC_Msk;   /* 1 count = 1 cycle */
TIMER0->CMP  = 0xFFFFFFUL;           /* 24 位全域自由运行 */
```

  重载值不是 65536 的整数倍时，低 16 位跨重载的差值会算错（曾出现 428284 / -16776752 这类数字）。
- 运行期核对：`0x40050000`(CTL)=`0x5A000000`（PSC=0、连续模式、CNTEN）、`0x40050004`(CMP)=`0x00FFFFFF`、`0x4005000C`(CNT) 在跑。TIMER0 基址是 `0x40050000`（`APBPERIPH_BASE+0x10000`），不是 `0x40010000`。
- Cortex-M23 **没有** DWT CYCCNT（`DWT_CTRL` bit25 NOCYCCNT=1），不能照抄 N657 的 `DWT->CYCCNT`。

### 12.5 串口（Nu-Link VCOM）

- PB12=UART0_RXD、PB13=UART0_TXD（复用功能号 6），`SYS->GPB_MFPH` 写前要 `SYS_UnlockReg()`。
- **PC 端打开串口后必须断言 DTR 和 RTS**，否则 Nu-Link VCOM 一个字节都不转发。症状极具误导性：UART 寄存器（BAUD 正确、FIFOSTS.TXEMPTYF 置位）、引脚复用、甚至用调试器直接 `mww 0x40070000 0x55` 写 DATA 都正常，但 PC 端就是 0 字节；`DtrEnable=$true` 后立即收到。
- 快速隔离链路：`openocd ... -c "halt" -c "mww 0x40070000 0x55"`，PC 端能收到就说明板子没问题，问题在固件。

### 12.6 GCC 工程（`Project/GCCMF-M2351KIAAE/`）

- `CPU=-mcpu=cortex-m23 -mthumb -mfloat-abi=soft`（M23 无 FPU，只能软浮点）。
- 源文件：`rmp_kernel.c` + `rmp_benchmark.c` + `rmp_platform_a8mb.c` + `rmp_stubs.c`，加 BSP 的 `system_M2351.c`/`clk.c`/`sys.c`/`uart.c`/`timer.c`；汇编是 `rmp_platform_a8mb_gcc.s` + BSP 的 GCC 版 `startup_M2351.S`。
- 链接脚本 `APROM.ld`：FLASH `0x0` + 512K、RAM `0x20000000` + 96K。
- 固件库来自 `M0A00_Library/M2351BSP`，不依赖 Keil DFP。
- 构建：`make`；产物 `Object/RMP.elf` 与 `Object/RMP.bin`。

### 12.7 Keil 工程（`Project/RVMDK-M2351KIAAE/`）

- 器件包 `Nuvoton.NuMicroM23_DFP.1.0.9`，`CPUTYPE("Cortex-M23")`，汇编走 GNU syntax（`<ClangAsOpt>2</ClangAsOpt>`），复用 `rmp_platform_a8mb_gcc.s`；汇编不需要任何档位宏。
- **必须关闭 MicroLIB**（`<useUlib>0</useUlib>`）：MicroLIB 的 `entry2.o(__vectab_stack_and_reset_area)` 自带一套向量表和栈区，与 Nuvoton 原厂 startup 的向量表/栈冲突，链接报
  `L6223E: Ambiguous selectors found for entry2.o(__vectab_stack_and_reset_area) from Exec regions ER_IROM1 and RW_STACK`。
  关闭后 `Image$...`/`__initial_sp` 才正常。（此前"不允许关闭 MicroLIB"的约束在 M2351 上无法满足，这是唯一可行解。）
- 保留 Nuvoton 原厂 `startup_M2351.S`（Reset_Handler → SystemInit → `__main`），只自定义 scatter；关键是 `.bss.STACK` 必须放进 UNINIT 区，否则 C 库 `__scatterload` 把栈清零，它自己放在栈上的散装表指针被擦掉：

```
LR_IROM1 0x00000000 0x00080000  {
  ER_IROM1 0x00000000 0x00080000  {
   *.o (RESET, +First)
   *(InRoot$Sections)
   .ANY (+RO)
   .ANY (+XO)
  }
  RW_STACK 0x20017000 UNINIT 0x00001000  { *(.bss.STACK) }
  ARM_LIB_STACK 0x20016000 EMPTY 0x00001000  { }
  RW_IRAM1 0x20000000 0x00016000  { .ANY (+RW +ZI) }
}
```

  链接结果核对：向量表首字 `0x20018000`（= `RW_STACK` 顶），`Reset_Handler` 在 `0x2ac`。
- 优化与 N657 工程保持一致：`<Optim>4</Optim>` + `<oTime>1</oTime>`（AC6 即 -O3 -otime）。
- 构建：`UV4.exe -r RMP.uvprojx -j0 -o build.log`；产物 `obj/RMP.axf` 与 `obj/RMP.bin`。

### 12.8 IAR 工程（`Project/EWARM-M2351KIAAE/`）

- **器件支持来自 Nuvoton 官方的 IAR 器件包**，落在 `arm/config/devices/Nuvoton/M2351/`（扁平目录），menu tag 是 `M2351 series`；配套 linker 模板 `linker/Nuvoton/M2351.icf`、flash loader `flashloader/Nuvoton/M2351_APROM.board` 一并到位。不需要 pack2iar（那只用于 IAR 数据库里根本没有的器件，见 §11.5）。
  - **器件串必须与当前器件库匹配**：官方包是 `M2351 series<TAB>Nuvoton M2351 series`；IAR 9.40.1 自带的旧版库（`devices/Nuvoton/M2350/M2351.i79`）用的是 `M2351<TAB>Nuvoton M2351`。写错时不会报"找不到器件"，而是**静默退回默认核**，症状是 58 条汇编/内建错误（`Error[446] ... does not support this status register` 或 `Ta148 __iar_builtin_DSB is not available in this architecture`）。换器件包后先看 `devices/Nuvoton/*/M2351.menu` 的 tag。
- 工程以 Nuvoton 官方 IAR 样例为底：`M0A00_Library/M2351BSP/SampleCode/AttackDetection/CLK_FrequencyDetection/IAR/CLK_FrequencyDetection.ewp`，替换文件组与包含路径即可。
  - 自带 ICF 是 TrustZone 模板（APROM 只有 16KB、IRAM 8KB），必须换成自己的 `rmp_platform_m2351kiaae.icf`（APROM 0x0-0x7FFFF，IRAM 0x20000000-0x20017FFF，直接抄官方样例的 `m2351.icf`）。
- **需要独立的 iasmarm 汇编** `Source/Platform/A8MB/rmp_platform_a8mb_iccarm.s`：IAR 不认 GNU 语法。
- 警告抑制：`CCDiagSuppress=Pa082`（volatile 访问顺序），与 N657 的 IAR 工程一致；`Pa050` 那条不存在，别写错。
- 工程里带了一份从官方样例移植的 `RMP.ewd`（Nu-Link 驱动 + flash loader），IAR IDE 里直接 Download / F5 即可；按仓库策略 `*.ewd` 被 `.gitignore` 忽略，所以只在本地存在。
- 构建：`iarbuild.exe RMP.ewp -build Debug`；产物 `Debug/Exe/RMP.out` 与 `Debug/Exe/RMP.bin`。

### 12.9 烧录与采集（不依赖 Keil）

**统一烧录**：`python Document/Private/Agent_Workspace/flash_m2351_isp.py <image.bin>`

- M2351 没有 OpenOCD flash driver，脚本用 SWD 直接驱动 FMC 的 ISP 引擎：2 KB 页擦除（`ISPCMD=0x22`）+ 32 位编程（`ISPCMD=0x21`），随后 `dump_image` 回读逐字节比对，0 差异才报成功，最后 `reset resume`。
- **必须 `ISPCTL |= ISPEN|APUEN`（0x9）**：只开 ISPEN 时擦除/编程会被静默忽略（表现为擦完首字仍是旧内容、verify 大面积不符）。
- 三种工具链的 bin 都能直接喂：Keil `obj/RMP.bin`、GCC `Object/RMP.bin`、IAR `Debug/Exe/RMP.bin`（都是只含 flash 段的 raw 镜像，`.bss` 本来就不在文件里）。实测 15 KB≈43 s、22 KB≈62 s。
- OpenOCD 内嵌的是 **Jim Tcl**：`expr` 必须带花括号、`mdw` 返回值有尾随空格、`incr` 别用十六进制增量。

**采集**：`capture_m2351_ocd.ps1`（先烧完，再 OpenOCD `reset halt; resume` 复位、同时读串口）。

- **必须在 PC 端断言 DTR/RTS**（见 §12.5）；也**不能与 UV4 烧录并行**——烧录时 VCP 会重枚举，句柄失效后 `ReadExisting()` 静默返回空。

**不要走的两条路**：

- `UV4 -f` 烧 IAR 的 `.out` 会报 `Flash Download failed`（编程其实成功，回读 0 差异）；它是对 IAR ELF 的校验环节不认，不是内容问题（试过剥离 RAM 段、改段名、包成单段 ELF，一律照报）。
- `CSpyBat`：参数格式已摸到（libsupport 用 `--plugin=<armbat.dll>`、`--cpu/--endian` 放 `--backend` 之后），但 `swtdarm_nulink.dll`/`swtdarm_cmsisdap.dll` 都不导出 `DriverApi_6`，且 `--download_only` 会擦空芯片（实测过）。IAR 下载走 IDE 的 Download/F5。

**备用（Keil 独占）**：把 `C:/Keil_v5_ARM/ARM/NULink/Nu_Link_Driver.ini` 的 `[ChipSelect] ChipName` 改成 `M2351`，`taskkill /F /IM UV4.exe` 后 `UV4 -f`；只对 Keil/GCC 的 ELF 有效。

### 12.10 基准结果（HXT→PLL 48MHz，无 cache，APROM 运行，三工具链均上板）

| 测试 | Keil AC6 -O3 | GCC 13.2.1 -O3 | IAR 9.40.1 -Oh |
|---|---|---|---|
| Yield | 213 | 265 | 228 |
| Mailbox | 390 | 554 | 512 |
| Semaphore | 349 | 538 | 387 |
| FIFO | 197 | 210 | 211 |
| Message queue | 675 | 706 | 642 |
| Blocking message queue | 957 | 1008 | 887 |
| Alarm (1/2/3/5/7) | 777 | 763 | 900 |
| Memory alloc/free | 498 | 674 | 501 |
| ISR Mailbox | 441 | 492 | 444 |
| ISR Semaphore | 339 | 444 | 366 |
| ISR Message queue | 535 | 565 | 522 |
| ISR Blocking msgq | 655 | 751 | 663 |

Keil 两次烧录运行 AVG 完全一致，IAR 复跑也一致。串口末尾的 `10000 overflows` 是基准自己在统计 16 位计时器回绕次数，属正常输出。完整 AVG/MAX/MIN 已按 §5 存档在 `Include/Test/Chip/rmp_test_m2351kiaae.h`（三段：GCC / Keil / IAR），README 双语表格各三行。

---

*维护者：AI agent（Cline）。发现文档与代码不符时，以代码为准并修正本文档。*
