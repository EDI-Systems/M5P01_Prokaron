# MuProkaron

**RMP** is a small real-time operating system which focuses on formal reliability and simplicity. It achieves reliability by deployment of formal techniques(not completed yet). All the basic functionalities that are necessary for RTOSes are povided, but nothing more. This guarantees that the system is the minimum possible kernel and is also suitable to be used as a guest operating system when hosted on virtual machine monitors.
This operating system is much **leaner** than any other RTOSes, especilly when compared to _FreeRTOS_ or _RT-Thread_, and understanding it should be simple enough.

Read ![Contributing](CONTRIBUTING.md) and ![Code of Conduct](CODE_OF_CONDUCT.md) if you want to contribute, and [Pull Request Template](PULL_REQUEST_TEMPLATE.md) when you make pull requests.
This software is dual-licensed: it is either ![LGPL v3](LICENSE.md) or ![modified MIT license](LICENSE_MODMIT.md). Commercial licenses are also available.

## Quick Demo
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

### Typical performance figures for all supported architectures
|Machine      |Toolchain     |Flash|SRAM|Yield|Mailbox|Semaphore|Mailbox/Int|Semaphore/Int|
|:-----------:|:------------:|:---:|:--:|:---:|:-----:|:-------:|:---------:|:-----------:|
|MSP430       |TI CCS6       |TBT  |TBT |TBT  |TBT    |TBT      |TBT        |TBT          |
|MSP430       |GCC           |TBT  |TBT |TBT  |TBT    |TBT      |TBT        |TBT          |
|Cortex-M0    |Keil uVision 5|TBT  |TBT |TBT  |TBT    |TBT      |TBT        |TBT          |
|Cortex-M0+   |Keil uVision 5|TBT  |TBT |TBT  |TBT    |TBT      |TBT        |TBT          |
|Cortex-M3    |Keil uVision 5|2.60 |1.65|246  |456    |422      |443        |409          |
|Cortex-M3    |GCC           |TBT  |TBT |TBT  |TBT    |TBT      |TBT        |TBT          |
|Cortex-M4    |Keil uVision 5|2.70 |1.66|184  |339    |325      |374        |361          |
|Cortex-M4    |GCC           |TBT  |TBT |TBT  |TBT    |TBT      |TBT        |TBT          |
|Cortex-M7    |Keil uVision 5|6.66 |1.65|170  |256    |230      |274        |268          |
|Cortex-M7    |GCC           |TBT  |TBT |TBT  |TBT    |TBT      |TBT        |TBT          |
|Cortex-R4    |Keil uVision 5|TBT  |TBT |TBT  |TBT    |TBT      |TBT        |TBT          |
|Cortex-R4    |GCC           |TBT  |TBT |TBT  |TBT    |TBT      |TBT        |TBT          |
|Cortex-R5    |Keil uVision 5|TBT  |TBT |TBT  |TBT    |TBT      |TBT        |TBT          |
|Cortex-R5    |GCC           |TBT  |TBT |TBT  |TBT    |TBT      |TBT        |TBT          |
|MIPS M14k    |MPLAB X       |TBT  |TBT |TBT  |TBT    |TBT      |TBT        |TBT          |
|MIPS M14k    |GCC           |TBT  |TBT |TBT  |TBT    |TBT      |TBT        |TBT          |

**Flash and SRAM consumption is calculated in kB, while the other figures are calculated in CPU clock cycles.**  
Yield: The time to yield between different threads.  
Mailbox: The mailbox communication time between two threads.  
Semaphore: The semaphore communication time between two threads.  
Mailbox/Int: The time to send to a mailbox from interrupt.  
Semaphore/Int: The time to post to a semaphore from interrupt.  

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

You need **_Cortex-M or Cortex-R or MIPS or MSP430_** microcontroller development kits to run the tests. This RTOS focuses on value-line MCUs and do not concentrate on high-end MCUs or MPUs. Do not use QEMU simulator to test the projects because they do not behave correctly in many scenarios.  
Other platform supports should be simple to implement, however they are not scheduled yet. For Cortex-A and other CPUs with a memory management unit ([MMU](https://en.wikipedia.org/wiki/Memory_management_unit)), go [M7M1_MuEukaron](https://github.com/EDI-Systems/M7M1_MuEukaron) _Real-Time Multi-Core Microkernel_ instead; M7M1 supports some Cortex-Ms and Cortex-Rs as well.

### Compilation

The **Vendor Toolchain** or **GNU Makefile** projects for various microcontrollers are available in the **_Project_** folder. Refer to the readme files in each folder for specific instructions about how to run them. However, keep in mind that some examples may need vendor-specific libraries such as the STM HAL. Some additional drivers may be required too.

## Running the tests

To run the sample programs, simply download them into the development board and start step-by-step debugging. Some examples will use one or two LEDs to indicate the system status. In that case, it is necessary to fill the LED blinking wrapper functions.

## Deployment

When deploying this into a production system, it is recommended that you read the manual in the **_Documents_** folder carefully to configure all macros correctly.

## Built With

- Keil uVision 5 (armcc)
- Code composer studio
- gcc/clang-llvm

Other toolchains are not recommended nor supported at this point, though it might be possible to support them later on.

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## License

This project is licensed under the GPLv3 License - see the [LICENSE.md](LICENSE.md) file for details. However, commercial licenses are also available.

## EDI Project Information
Mutate - Protero - Prokaron (M5P1 R4T1)
