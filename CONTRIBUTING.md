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
  * [C Styleguide](#javascript-styleguide)
  * [C comment Styleguide](#coffeescript-styleguide)
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
* Follow the [JavaScript](#javascript-styleguide) and [CoffeeScript](#coffeescript-styleguide) styleguides.
* Include thoughtfully-worded, well-structured [Jasmine](https://jasmine.github.io/) specs in the `./spec` folder. Run them using `atom --test spec`. See the [Specs Styleguide](#specs-styleguide) below.
* Document new code based on the [Documentation Styleguide](#documentation-styleguide)
* End all files with a newline
* Avoid platform-dependent code, unless you are working on new processor support.
* Place requires in the following order:
    * Built in Node Modules (such as `path`)
    * Built in Atom and Electron Modules (such as `atom`, `remote`)
    * Local Modules (using relative paths)
* Place class properties in the following order:
    * Class methods and properties (methods starting with a `@` in CoffeeScript or `static` in JavaScript)
    * Instance methods and properties

## Styleguides

### Git Commit Messages

* Use the present tense ("Add feature" not "Added feature")
* Use the imperative mood ("Move cursor to..." not "Moves cursor to...")
* Limit the first line to 72 characters or less
* Reference issues and pull requests liberally after the first line
* When only changing documentation, include `[ci skip]` in the commit title
* Consider starting the commit message with an applicable emoji:
    * :art: `:art:` when improving the format/structure of the code
    * :racehorse: `:racehorse:` when improving performance
    * :non-potable_water: `:non-potable_water:` when plugging memory leaks
    * :memo: `:memo:` when writing docs
    * :penguin: `:penguin:` when fixing something on Linux
    * :apple: `:apple:` when fixing something on macOS
    * :checkered_flag: `:checkered_flag:` when fixing something on Windows
    * :bug: `:bug:` when fixing a bug
    * :fire: `:fire:` when removing code or files
    * :green_heart: `:green_heart:` when fixing the CI build
    * :white_check_mark: `:white_check_mark:` when adding tests
    * :lock: `:lock:` when dealing with security
    * :arrow_up: `:arrow_up:` when upgrading dependencies
    * :arrow_down: `:arrow_down:` when downgrading dependencies
    * :shirt: `:shirt:` when removing linter warnings

### C programming Styleguide

All JavaScript must adhere to [JavaScript Standard Style](https://standardjs.com/).

* Prefer the object spread operator (`{...anotherObj}`) to `Object.assign()`
* Inline `export`s with expressions whenever possible
  ```js
  // Use this:
  export default class ClassName {

  }

  // Instead of:
  class ClassName {

  }
  export default ClassName
  ```

### C comment Styleguide

* Set parameter defaults without spaces around the equal sign
    * `clear = (count=1) ->` instead of `clear = (count = 1) ->`
* Use spaces around operators
    * `count + 1` instead of `count+1`
* Use spaces after commas (unless separated by newlines)
* Use parentheses if it improves code clarity.
* Prefer alphabetic keywords to symbolic keywords:
    * `a is b` instead of `a == b`
* Avoid spaces inside the curly-braces of hash literals:
    * `{a: 1, b: 2}` instead of `{ a: 1, b: 2 }`
* Include a single line of whitespace between methods.
* Capitalize initialisms and acronyms in names, except for the first word, which
  should be lower-case:
  * `getURI` instead of `getUri`
  * `uriToOpen` instead of `URIToOpen`
* Use `slice()` to copy an array
* Add an explicit `return` when your function ends with a `for`/`while` loop and
  you don't want it to return a collected array.
* Use `this` instead of a standalone `@`
  * `return this` instead of `return @`

### Documentation Styleguide

* Use [AtomDoc](https://github.com/atom/atomdoc).
* Use [Markdown](https://daringfireball.net/projects/markdown).
* Reference methods and classes in markdown with the custom `{}` notation:
    * Reference classes with `{ClassName}`
    * Reference instance methods with `{ClassName::methodName}`
    * Reference class methods with `{ClassName.methodName}`

## Attribution

This CONTRIBUTING is adapted from the [Atom project](https://github.com/atom).

