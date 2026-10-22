# M5P01_Prokaron (RMP) 项目维护指南（供 AI Agents 使用）

> 本文档由 AI agent 撰写并持续维护，用于帮助后续 agents 快速理解本项目。
> 每次对项目做出结构性或重要的修改后，请同步更新本文档。
> 最后更新：2026-10-21（基于 main 分支，版本 R6T1）。

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
| `Source/Platform/A7M/rmp_platform_a7m_<toolchain>.s` | 汇编上下文切换/启动。**后缀 = 编译器**：`_armcc.s`(Keil)、`_gcc.s`(GCC)、`_iccarm.s`(IAR) |
| `Include/Platform/A7M/Chip/<CHIP>/rmp_platform_<chip>.h` | 芯片级：HAL 包含、系统参数宏、`RMP_A7M_LOWLVL_INIT()` 时钟+串口初始化宏 |

**新增芯片/架构的固定套路**：平台头 → 平台 .c → 各工具链汇编 → Chip 头 → `Include/Test/Chip/rmp_test_<chip>.h` 测试台 → `Project/` 建工程（含 `rmp_platform.h`+`rmp_test.h` 转发文件）→ 测试头注释中存档基准数据 → 更新 README 性能表格（如适用）。

`_RVM` 变体把中断屏蔽/yield 换成 `RVM_Virt_*` 超调用。

### 支持的架构（Platform 目录命名）
| 目录名 | 架构 | 目录名 | 架构 |
|---|---|---|---|
| A6M | ARMv6-M (Cortex-M0) | MP32P | MIPS32 (PIC32) |
| A7M | ARMv7-M (Cortex-M3/4/7) | MSP430 | TI MSP430 |
| A7R | ARMv7-R (Cortex-R, TMS570) | RL78 | Renesas RL78 |
| A8M | ARMv8-M (Cortex-M23/33) | RV32P | RISC-V 32 |
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
1. 把完整结果（12 项的 AVG/MAX/MIN）以注释形式**存档进对应 `Include/Test/Chip/rmp_test_<chip>.h` 头部**——每个工具链一段（如 `ARMCLANG 6.23 -O3`、`GCC 13.2.1 -O3`、`IAR 9.40.1 -O3`），带 ASCII 横幅，格式对齐已有条目；
2. 在 **README.md 与 README_CN.md** 的 "Useful ports" 性能表格中新增/更新对应芯片行（同一芯片不同工具链各一行，后续行用 `...` 缩写芯片名和平台列）。
未存档的跑分等于没跑。修改平台汇编/编译选项等影响性能的内容后，必须重测并同步更新这两处存档。

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

- 工具链安装情况：Keil MDK（`C:\Keil_v5_ARM`）、**IAR EWARM 9.2.2**（`C:\Program Files\IAR Systems\Embedded Workbench 9.2`）、STM32CubeProgrammer（`C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer`）等。
- 硬件在环验证：NUCLEO-F429ZI 已通过板载 ST-LINK（SWD）连接；烧录用 `STM32_Programmer_CLI.exe -c port=SWD -d xxx.out -v -rst`；活性检查可热插拔读 SysTick `0xE000E018` 两次比较。
- 本仓库在 Windows 下无法运行 X86L 端口（需 Linux+gcc-multilib+ptrace）；cppcheck 静态检查工程在 `Project/CPPCHECK-LINT`。
- README 明确警告**不要用 QEMU 测试**。
- `Project/` 下 IDE 的临时文件（`.metadata/`、`JLinkLog.txt`、`.jxbrowser.userdata/` 等）已入库且含大量噪音数据——**搜索代码时务必限定 `Source/`、`Include/` 路径**。

---

## 8. 修改注意事项（经验之谈）

1. **内核单文件原则**：不要拆分 `rmp_kernel.c`，不要往内核加可选组件（网络栈/驱动框架/动态对象管理都被作者拒绝）。
2. 改内核必须同步：① `rmp_coverage.c` 新分支加 `RMP_COV_MARKER()`；② 各配置组合（16/32 位、FPU、`RMP_GUI_ENABLE` 等）可编译；③ 最坏执行时间不能变差；④ 三个工具链汇编变体（armcc/gcc/iccarm）保持行为一致。
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
```
---

## 10. IAR EWARM 工程记录（2026-10-21 新增）

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

*维护者：AI agent（Cline）。发现文档与代码不符时，以代码为准并修正本文档。*

