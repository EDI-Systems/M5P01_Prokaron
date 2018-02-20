# RTOS-Mutate-Prokaron (M5P1 R4T1)

**RMP** is a small real-time operating system which focuses on formal reliability and simplicity. It achieves reliability by deployment of formal techniques(not completed yet). All the basic functionalities that are necessary for RTOSes are povided, but nothing more. This guarantees that the system is the minimum possible kernel and is also suitable to be used as a guest operating system when hosted on virtual machine monitors.
This operating system is much **leaner** than any other RTOSes, especilly when compared to _FreeRTOS_ or _RT-Thread_, and understanding it should be simple enough.
**_The formal proof is not completed yet, and the system is in pre-beta quality._**

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

You need **_Cortex-M or MIPS or PowerPC or MSP430_** microcontroller development kits to run the tests. Do not use QEMU simulators because they do not behave correctly in many scenarios.
Other platform supports should be simple to implement, however they are not scheduled yet.

### Compilation

The **Keil uVision** or **GNU Makefile** projects for various microcontrollers are available in the **_Project_** folder. Refer to the readme files in each folder for specific instructions about how to run them. However, keep in mind that some examples may need vendor-specific libraries such as the STM HAL.

## Running the tests

To run the sample programs, simply download them into the development board and start step-by-step debugging. Some examples will use one or two LEDs to indicate the system status. In that case, it is necessary to fill the LED blinking wrapper functions.

## Deployment

When deploying this into a production system, it is recommended that you read the manual in the **_Documents_** folder carefully to configure all macros correctly.

## Built With

- Keil uVision 5 (armcc)
- gcc/clang-llvm

Other toolchains are not recommended nor supported at this point, though it might be possible to support them later on.

## Contributing

Please read [CONTRIBUTING.md] for details on our code of conduct, and the process for submitting pull requests to us.

## License

This project is licensed under the GPLv3 License - see the [LICENSE.md](LICENSE.md) file for details. However, commercial licenses are also available.

