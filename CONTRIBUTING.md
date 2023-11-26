# Contributing to RMP

:+1::tada: Thank you for your interest in contribuing to this stuff! :tada::+1:

The following is a set of guidelines for contributing to RMP and its packages, which are hosted in the [EDI Organization](http://github.com/EDI-Systems) on GitHub. These are mostly guidelines, not rules. Use your best judgment, and feel free to propose changes to this document in a pull request.

#### Table Of Contents

[Code of Conduct](#code-of-conduct)

[What should I know before I get started?](#what-should-i-know-before-i-get-started)
  * [RMP and Packages](#rmp-and-packages)
  * [RMP Design Decisions](#design-decisions)

[How Can I Contribute?](#how-can-i-contribute)
  * [Reporting Bugs](#reporting-bugs)
  * [Suggesting Enhancements](#suggesting-enhancements)
  * [Your First Code Contribution](#your-first-code-contribution)
  * [Pull Requests](#pull-requests)

[Styleguides](#styleguides)
  * [Git Commit Messages](#git-commit-messages)
  * [C Styleguide](#c-programming-styleguide)
  * [Documentation Styleguide](#documentation-styleguide)

[Additional Notes](#additional-notes)
  * [Issue and Pull Request Labels](#issue-and-pull-request-labels)

## Code of Conduct

This project and everyone participating in it is governed by the [EDI Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code. 

## What should I know before I get started?

### RMP and Packages

RMP is an intentionally small open source RTOS and does not provide advanced features like kernel object management or dynamic memory allocation. It should not have package dependencies, except for the libraries that are provided by the manufacturer. If external libraries are needed for a specific project, it is the user's duty to add such packages to the system.  
RMP is not meant to compete with FreeRTOS, RT-Thread or uC/OS. If you just need a small, fast yet reliable kernel, RMP is your best choice, especially when you need to run an RTOS as the guest RTOS on **[M7M2](https://github.com/EDI-Systems/M7M2_MuAmmonite)/[M7M1](https://github.com/EDI-Systems/M7M1_MuEukaron)**.

### Design Decisions

Different from most operating systems, only the most used arcitectures are supported, such as the Cortex-M, Cortex-R, MIPS and MSP430. Other architectures are on the decline, and will disappear in a couple of years, hence they are not supported.
Unlike FreeRTOS and RT-Thread, RMP is designed by intention to contain no pre-included packages. This simplifies the system and is without extra hassle.
Also, the RMP kernel's IPC is very lightweight, very concise by design. Only two kinds are supported, mailbox and counting semaphore. This also makes it easy to formally verify the system against a extremely high standard, e.g. EAL7+.

## How Can I Contribute?

### Reporting Bugs

Report the bugs as you come across them. Bugs are tracked as [GitHub issues](https://guides.github.com/features/issues/). Before creating bug reports, please check the list as you might find out that you don't need to create one. When you are creating a bug report, please include as many details as possible, because it can help to reproduce, locate and finally fix the bug.  

Explain the problem and include additional details to help maintainers reproduce the problem:

* **Use a clear and descriptive title** for the issue to identify the problem.
* **Describe the exact steps which reproduce the problem** in as many details as possible. For example, start by explaining how you created project, e.g. which MCU you are using and what libraries are used. When listing steps, **don't just say what you did, but explain how you did it**. For example, if you configured the interrupt controller, what exact manufacturer library functions are you using, or you did it by direct register writes?
* **Provide specific examples to demonstrate the steps**. Include links to files or GitHub projects, or copy/pasteable snippets, which you use in those examples. If you're providing snippets in the issue, use [Markdown code blocks](https://help.github.com/articles/markdown-basics/#multiple-lines).
* **Describe the behavior you observed after following the steps** and point out what exactly is the problem with that behavior.
* **Explain which behavior you expected to see instead and why.**
* **Include screenshots and animated GIFs** of the console or the system board, if possible, which show you following the described steps and clearly demonstrate the problem.
* **If you're reporting that RMP crashed**, include a full memory dump in *.hex file. Many toolchains, such as Keil or GDB, generate these files.
* **If the problem is related to performance or memory**, include these profiling figures with your report.
* **If the problem wasn't triggered by a specific step**, describe what you were doing before the problem happened and share more information using the guidelines below.

Provide more context by answering these questions:

* **Can you reproduce the problem?**
* **Did the problem start happening recently** (e.g. after updating to a new version of RMP) or was this always a problem?
* If the problem started happening recently, **can you reproduce the problem in an older version of RMP?** What's the most recent version in which the problem doesn't happen? You can download older versions of RMP from [the releases page](http://github.com/EDI-Systems/M5P1_MuProkaron/releases).
* **Can you reliably reproduce the issue?** If not, provide details about how often the problem happens and under which conditions it normally happens, especially if there is external interference.

Include details about your configuration and environment:

* **Which version of RMP are you using?** 
* **What's the name and version of the hosting OS you're using**?
* **Are you compiling RMP in a virtual machine?** If so, which VM software are you using and which operating systems and versions are used for the host and the guest?

### Suggesting Enhancements

This section guides you through submitting an enhancement suggestion for RMP, including completely new features and minor improvements to existing functionality. Following these guidelines helps maintainers and the community understand your suggestion :pencil: and find related suggestions :mag_right:.

#### How Do I Submit A (Good) Enhancement Suggestion?

Enhancement suggestions are tracked as [GitHub issues](https://guides.github.com/features/issues/). Create an issue on that repository and provide the following information:

* **Use a clear and descriptive title** for the issue to identify the suggestion.
* **Provide a step-by-step description of the suggested enhancement** in as many details as possible.
* **Provide specific examples to demonstrate the steps**. Include copy/pasteable snippets which you use in those examples, as [Markdown code blocks](https://help.github.com/articles/markdown-basics/#multiple-lines).
* **Describe the current behavior** and **explain which behavior you expected to see instead** and why.
* **Explain why this enhancement would be useful** to most RMP users and isn't something that can or should be implemented as a standalone package.
* **List some other RTOSes or applications where this enhancement exists.**
* **Specify which version of RMP and toolchain you're using.**


### Pull Requests

* Fill in [the required template](PULL_REQUEST_TEMPLATE.md)
* Do not include issue numbers in the PR title
* Include screenshots and animated GIFs in your pull request whenever possible.
* Follow the [C](#c-programming-styleguide) styleguides.
* Document new code based on the [Documentation Styleguide](#documentation-styleguide)
* End all files with a newline (otherwise you get tons of warnings on some toolchains)!
* Avoid platform-dependent code, unless you are working on new processor support.

## Styleguides

### Git Commit Messages

* Use the present tense ("Add feature" not "Added feature")
* Limit the first line to 80 characters or less
* Reference issues and pull requests liberally after the first line
* Consider starting the commit message with an applicable emoji:
    * :art: `:art:` when improving the format/structure of the code
    * :racehorse: `:racehorse:` when improving performance
    * :non-potable_water: `:non-potable_water:` when plugging memory leaks
    * :memo: `:memo:` when writing docs
    * :penguin: `:penguin:` when fixing something on Linux or GNU toolchain
    * :apple: `:apple:` when fixing something on macOS or Clang/LLVM toolchain
    * :checkered_flag: `:checkered_flag:` when fixing something on Windows or manufacturer official toolchain
    * :bug: `:bug:` when fixing a bug
    * :fire: `:fire:` when removing code or files
    * :green_heart: `:green_heart:` when fixing the CI build
    * :white_check_mark: `:white_check_mark:` when adding tests
    * :lock: `:lock:` when dealing with reliability or formal proof
    * :arrow_up: `:arrow_up:` when upgrading dependencies or adding features
    * :arrow_down: `:arrow_down:` when downgrading dependencies or removing features
    * :shirt: `:shirt:` when removing linter warnings

### C programming Styleguide

* DO NOT place spaces around the binary operators, especially "=" and "==". Leave them as they are.
    * `a=b` instead of `a = b`
* Feel free to use spaces after commas. If no space is used, it is also fine.
    * both `foo(a, b, c)` and `foo(a,b,c)` is okay.
* Always use (sometimes even excessive) parentheses in bool expressions.
    * `(a==b)||(c==d)` instead of `a==b||c==d`.
* Add headers and enders fo functions, files and code blocks. Refer to the snippets shown below.
* Naming convention must adhere to the current convention.
* For other info, please relate to the source code itself.

A example C header file:
```C
    /******************************************************************************
    Filename    : <Filename>.h
    Author      : <Name>
    Date        : <DD/MM/YYYY>
    Licence     : LGPL v3+; see COPYING for details.
    Description : <File description>
    ******************************************************************************/

    /* Define ********************************************************************/
    #ifdef __HDR_DEF__
    #ifndef __<FILENAME>_DEF__
    #define __<FILENAME>_DEF__
    /*****************************************************************************/
    <Place your defines here>
    /*****************************************************************************/
    /* __<FILENAME>_DEF__ */
    #endif
    /* __HDR_DEF__ */
    #endif
    /* End Define ****************************************************************/

    /* Struct ********************************************************************/
    #ifdef __HDR_STRUCT__
    #ifndef __<FILENAME>_STRUCT__
    #define __<FILENAME>_STRUCT__

    /* Use defines in these headers */
    #define __HDR_DEF__
    <Place external define import headers here>
    #undef __HDR_DEF__

    /* We used structs in these headers */
    <Place external struct import headers here>
    /*****************************************************************************/
    <Place struct definitions here>
    /*****************************************************************************/
    /* __<FILENAME>_STRUCT__ */
    #endif
    /* __HDR_STRUCT__ */
    #endif
    /* End Struct ****************************************************************/

    /* Private Variable **********************************************************/
    #if(!(defined __HDR_DEF__||defined __HDR_STRUCT__))
    #ifndef __<FILENAME>_PUBLIC__
    #define __<FILENAME>_PUBLIC__

    /* In this way we can use the data structures and definitions in the headers */
    #define __HDR_DEF__
    <Place external define import headers here>
    #undef __HDR_DEF__

    #define __HDR_STRUCT__
    <Place external struct import headers here>
    #undef __HDR_STRUCT__

    /* If the header is not used in the public mode */
    #ifndef __HDR_PUBLIC__
    /*****************************************************************************/
    <Place private global variables here>
    /*****************************************************************************/
    /* End Private Variable ******************************************************/

    /* Private Function **********************************************************/ 
    /*****************************************************************************/
    <Place private C function prototypes here>
    /*****************************************************************************/
    #define __EXTERN__
    /* End Private Function ******************************************************/

    /* Public Variable ***********************************************************/
    /* __HDR_PUBLIC__ */
    #else
    #define __EXTERN__ EXTERN 
    /* __HDR_PUBLIC__ */
    #endif

    /*****************************************************************************/
    <Place public global variables here>
    /*****************************************************************************/

    /* End Public Variable *******************************************************/

    /* Public Function ***********************************************************/
    /*****************************************************************************/
    <Place public C function prototypes here>
    /*****************************************************************************/
    /* Undefine "__EXTERN__" to avoid redefinition */
    #undef __EXTERN__
    /* __<FILENAME>_PUBLIC__ */
    #endif
    /* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
    #endif
    /* End Public Function *******************************************************/

    /* End Of File ***************************************************************/

    /* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
```

An example C source file:
```C
    /******************************************************************************
    Filename    : <Filename>.c
    Author      : <Name>
    Date        : <DD/MM/YYYY>
    Licence     : LGPL v3+; see COPYING for details.
    Description : <File description>
    ******************************************************************************/

    /* Include *******************************************************************/
    #define __HDR_DEF__
    <Place the headers you wish to import defines here>
    #undef __HDR_DEF__

    #define __HDR_STRUCT__
    <Place the headers you wish to import structs here>
    #undef __HDR_STRUCT__

    /* Private include */
    <Place the private headers here>

    #define __HDR_PUBLIC__
    <Place the headers you wish to import globals here>
    #undef __HDR_PUBLIC__
    /* End Include ***************************************************************/

    /* Function:<Name> ************************************************************
    Description : <Description of the function>.
    Input       : <Type> <Argument> - <Explanation>.
    Output      : <Type> <Argument> - <Explanation>.
    Return      : <Type> - <Explanation>.
    ******************************************************************************/
    <Type> <Name>(<Parameter-list>)
    {
        <Function body>
    }
    /* End Function:<Name> *******************************************************/

    /* End Of File ***************************************************************/

    /* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
```

### Documentation Styleguide

* All functions and file must embedded documentations and they must follow the changes of code.
* When you changed code significantly, especially their APIs, file an issue to alert possible manual changes.

## Attribution

This CONTRIBUTING is adapted from the [Atom project](https://github.com/atom).

