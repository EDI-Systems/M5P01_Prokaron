/******************************************************************************
Filename    : rmp_platform_STM32F767IG.ld
Author      : pry
Date        : 24/06/2017
Licence     : LGPL v3+; see COPYING for details.
Description : The scatter file for Cortex-M7 layout. This file is intended 
              to be used with STM32F767IGT6, and the GNU toolchain.
              ROM: 0x08000000 0x00100000
              RAM: 0x20000000 0x00080000
******************************************************************************/

/* Memory Definitions *********************************************************
Description : This section will define the memory layout of the system. 
Component   : ORIGIN - Starting address of the memory region.
              LENGTH - Length of the region.
******************************************************************************/
MEMORY
{
    RAM    (xrw) : ORIGIN = 0x20000000, LENGTH = 512K
    FLASH  (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
}
/* End Memory Definitions ****************************************************/

/* Stack Definitions *********************************************************/
/* The '__stack' definition is required by crt0, do not remove it. */
__stack = ORIGIN(RAM) + LENGTH(RAM);
/* STM specific definition */
_estack = __stack;     

/* Default stack sizes. These are used by the startup in order to allocate stacks for the different modes */
__Main_Stack_Size = 1024 ;
/* "PROVIDE" allows to easily override these values from an object file or the command line */
PROVIDE ( _Main_Stack_Size = __Main_Stack_Size );
__Main_Stack_Limit = __stack - __Main_Stack_Size ;
PROVIDE ( _Main_Stack_Limit = __Main_Stack_Limit ) ;
/* The default stack - There will be a link error if there is not this amount of RAM free at the end */
_Minimum_Stack_Size = 256 ;
/* End Stack Definitions *****************************************************/

/* Heap Definitions **********************************************************/
/* Default heap definitions.
 * The heap start immediately after the last statically allocated 
 * .sbss/.noinit section, and extends up to the main stack limit.
 */
PROVIDE ( _Heap_Begin = _end_noinit ) ;
PROVIDE ( _Heap_Limit = __stack - __Main_Stack_Size ) ;
/* End Heap Definitions ******************************************************/

/* Entry Point Definitions ***************************************************/
/* The entry point is informative, for debuggers and simulators,
 * since the Cortex-M vector points to it anyway. */
ENTRY(Reset_Handler);
SECTIONS
{
/* End Entry Point Definitions ***********************************************/

/* Begin Section:.text ********************************************************
Description : The program code is stored in the .text section, which goes to FLASH.
Location    : Flash
Component   : .text - The code segment.
              .rodata.* - The read-only data segment.
******************************************************************************/
.text : ALIGN(4)
{
    /* the startup code */
    KEEP(*startup_stm32f767xx.o(.isr_vector))
    KEEP(*startup_stm32f767xx.o(.text .text.*))
    /* All remaining code */
    *(.text .text.*)
    /* Read-only data (constants) */
    *(.rodata .rodata.* .constdata .constdata.*)
} > FLASH
/* End Section:.text *********************************************************/

/* Begin Section:.inits *******************************************************
Description : Memory regions initialisation arrays. There are two kinds of arrays
              for each RAM region, one for data and one for bss. Each is iterrated
              at startup and the region initialisation is performed.
              The data array includes:
              - from (LOADADDR())
              - region_begin (ADDR())
              - region_end (ADDR()+SIZEOF())
              The bss array includes:
              - region_begin (ADDR())
              - region_end (ADDR()+SIZEOF())
              WARNING: It is mandatory that the regions are word aligned, 
              since the initialisation code works only on words.
Location    : Flash
Component   : .data.* - The data section initialization section.
              .bss.* - The bss section initialization section.
              .preinit_array - The preinit code to run before constructors.
              .init_array - The constructor code for global C++ objects.
              .fini_array - The destructor code for global C++ objects.
******************************************************************************/
.inits : ALIGN(4)
{
    /* The data array */
    __data_regions_array_start = .;
    LONG(LOADADDR(.data));
    LONG(ADDR(.data));
    LONG(ADDR(.data)+SIZEOF(.data));
    __data_regions_array_end = .;
    
    /* The bss array */
    __bss_regions_array_start = .;
    LONG(ADDR(.bss));
    LONG(ADDR(.bss)+SIZEOF(.bss));
    __bss_regions_array_end = .;

    /* These are the old initialisation sections, intended to contain
     * naked code, with the prologue/epilogue added by crti.o/crtn.o
     * when linking with startup files. The standalone startup code
     * currently does not run these, better use the init arrays below.
     */
    KEEP(*(.init))
    KEEP(*(.fini))
    . = ALIGN(4);

    /* The preinit code, i.e. an array of pointers to initialisation 
     * functions to be performed before constructors.
     */
    PROVIDE_HIDDEN (__preinit_array_start = .);
    /* Used to run the SystemInit() before anything else. */
    KEEP(*(.preinit_array_sysinit .preinit_array_sysinit.*))
    /* Used for other platform inits. */
    KEEP(*(.preinit_array_platform .preinit_array_platform.*))
    /* The application inits. If you need to enforce some order in execution, create new sections, as before. */
    KEEP(*(.preinit_array .preinit_array.*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(4);

    /* The init code, i.e. an array of pointers to static constructors. */
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(4);

    /* The fini code, i.e. an array of pointers to static destructors. */
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array))
    PROVIDE_HIDDEN (__fini_array_end = .);
} > FLASH
/* End Section:.inits ********************************************************/

/* Begin Section:.data ********************************************************
Description : The main initialized data section. The program executes knowing that
              the data is in the RAM but the loader puts the initial values in the
              FLASH (inidata). It is one task of the startup to copy the initial
              values from FLASH to RAM. The RME kernel does not really rely on the
              data section to be initialized because it never uses preinitialized
              global variables.
Location    : RAM
Component   : .data - The sections to put into the RAM.
******************************************************************************/
/* Used by the startup code to initialise the .data section */
_sidata = LOADADDR(.data);
.data : ALIGN(8192)
{
    FILL(0xFF)
    /* This is used by the startup code to initialise the .data section */
    _sdata = . ;            /* STM specific definition */
    __data_start__ = . ;
    *(.data_begin .data_begin.*)
    *(.data .data.*)
    *(.data_end .data_end.*)
    . = ALIGN(4);

    /* This is used by the startup code to initialise the .data section */
    _edata = . ;            /* STM specific definition */
    __data_end__ = . ;
} > RAM AT > FLASH
/* End Section:.data *********************************************************/

/* Begin Section:.bss *********************************************************
Description : The initialised-to-0 data sections. NOLOAD is used to avoid
              the "section `.bss' type changed to PROGBITS" warning. This is the
              main region which is placed in RAM. Actually the RME does not
              initialize its bss because there is no such need.
Location    : RAM
Component   : .bss - The sections to put into the RAM, and initialized to 0.
******************************************************************************/
.bss (NOLOAD) : ALIGN(4)
{
    __bss_start__ = .;         /* standard newlib definition */
    _sbss = .;              /* STM specific definition */
    *(.bss_begin .bss_begin.*)
    *(.bss .bss.*)
    *(COMMON)
    *(.bss_end .bss_end.*)
    . = ALIGN(4);

    __bss_end__ = .;        /* standard newlib definition */
    _ebss = . ;             /* STM specific definition */
} > RAM
/* End Section:.bss **********************************************************/

/* Begin Section:.noinit ******************************************************
Description : The uninitialised data sections. NOLOAD is used to avoid
              the "section `.noinit' type changed to PROGBITS" warning.
Location    : RAM
Component   : .noinit - The sections to put into the RAM, and not initialized.
******************************************************************************/
.noinit (NOLOAD) : ALIGN(4)
{
    _noinit = .;
    *(.noinit .noinit.*) 
    . = ALIGN(4) ;
    _end_noinit = .;
} > RAM
/* Mandatory to be word aligned, _sbrk assumes this */
PROVIDE ( end = _end_noinit ); /* was _ebss */
PROVIDE ( _end = _end_noinit );
PROVIDE ( __end = _end_noinit );
PROVIDE ( __end__ = _end_noinit );
/* End Section:.noinit *******************************************************/

/* Begin Section:._check_stack ************************************************
Description : Used for validation only, do not allocate anything here!
              This is just to check that there is enough RAM left for the Main
              stack. It should generate an error if it's full. This stack, 
              in composite, is the section for kernel stack. All the component
              related layouts will be in the component sections that follow.
              The unused heap is also put here.
Location    : RAM
Component   : Padding of size "_Minimum_Stack_Size" - The stack location.
******************************************************************************/
._check_stack : ALIGN(4)
{
    . = . + _Minimum_Stack_Size ;
} > RAM
/* End Section:._check_stack *************************************************/

/* End Of File ***************************************************************/
}
/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/

