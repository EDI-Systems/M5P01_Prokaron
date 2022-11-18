
MEMORY
{
    FLASH (rxai!w) : ORIGIN = 0x20400000, LENGTH = 12M
    RAM (wxa!ri) : ORIGIN = 0x80000000, LENGTH = 16K
}

/*
 * Explicitly define the RISC-V architecture.
 */
OUTPUT_ARCH("riscv")

/* 
 * The entry point is important for debuggers and simulators, the
 * hardware has its own notion of the startup address.
 */
ENTRY(_Reset_Entry)

/*
 * The '__stack' definition is required by newlib crt0; do not remove it.
 * The stack is located at the very end of the RAM region.
 */
__stack = ORIGIN(RAM) + LENGTH(RAM);
__stack_size = DEFINED(__stack_size) ? __stack_size : 2K;

/*
 * The ELF object file format uses program headers, also knows as segments.
 * The program headers describe how the program should be loaded into memory. 
 * You can print them out by using the objdump program with the ‘-p’ option.
 */
PHDRS
{
  phdr_flash PT_LOAD;
  phdr_ram_init PT_LOAD;
  phdr_ram PT_NULL;
}

SECTIONS
{
    /*
     * On RISC-V devices the startup address is a regular address, so this
     * must be the very first section.
     * Apart from the assembly code used to initialise the stack,
     * do not place anything else into this section.
     */ 
    .reset_entry : ALIGN(4)
    {
        KEEP(*(SORT_NONE(.reset_entry) SORT_NONE(.reset_entry.*)))
    } >FLASH AT>FLASH :phdr_flash

    /* 
     * Memory regions initialization arrays.
     *
     * There are two kinds of arrays for each RAM region, one for 
     * data and one for bss. Each is iterated at startup and the   
     * region initialization is performed.
     * 
     * The data array includes:
     * - from (LOADADDR())
     * - region_begin (ADDR())
     * - region_end (ADDR()+SIZEOF())
     *
     * The bss array includes:
     * - region_begin (ADDR())
     * - region_end (ADDR()+SIZEOF())
     *
     * WARNING: It is mandatory that the regions are word aligned, 
     * since the initialization code works only on words.
     */
    .mem_inits : ALIGN(4)
    {
        LONG(LOADADDR(.data));
        LONG(ADDR(.data));
        LONG(ADDR(.data)+SIZEOF(.data));   
        LONG(ADDR(.bss));
        LONG(ADDR(.bss)+SIZEOF(.bss));
    } >FLASH AT>FLASH :phdr_flash
    
    /*
     * The program code.
     */
    .text : ALIGN(4)
    {
        *(.text.unlikely .text.unlikely.*)
        *(.text.startup .text.startup.*)
        *(.text .text.*)            
        *(.gnu.linkonce.t.*)
    } >FLASH AT>FLASH :phdr_flash

    . = ALIGN(4);
      PROVIDE(__etext = .);
      PROVIDE(_etext = .);
      PROVIDE(etext = .);
 
     /*
      * Read-only data (constants) 
      */
    .rodata : ALIGN(4)
    {
        *(.rodata .rodata.*)         
        *(.constdata .constdata.*)         
        *(.gnu.linkonce.r.*)
    } >FLASH AT>FLASH :phdr_flash

    . = ALIGN(4);
    PROVIDE( _data = . );
    
    /*
     * The initialised data section.
     *
     * The program executes knowing that the data is in RAM
     * but the loader puts the initial values in FLASH.
     * The startup will copy the initial values from FLASH to RAM.
     */
    .data : ALIGN(4)
    {
        FILL(0xFF)
        
        __data_start__ = . ;            /* Standard newlib definition. */
        __data_begin__ = . ;            /* µOS++ specific */
        *(.data_begin .data_begin.*)    /* µOS++ __data_begin_guard */

        *(.data .data.*)
        *(.gnu.linkonce.d.*)
        
        . = ALIGN(8);
        /* 
         * RISC-V specific; the compiler optimises memory accesses
         * in the +/- 2K range around __global_pointer$ to GP relative.
         * For this to work, GP must be loaded during startup with the
         * address of __global_pointer$.
         * This optimisation favours a 4K range. Newlib places
         * several impure and malloc pointers in the .sdata section.
         */
        PROVIDE( __global_pointer$ = . + (4K / 2) );
        
        *(.sdata .sdata.*)
        *(.gnu.linkonce.s.*)

        /* RISC-V specific; not sure if needed. */
        . = ALIGN(8);
        *(.srodata.cst16)
        *(.srodata.cst8)
        *(.srodata.cst4)
        *(.srodata.cst2)
        *(.srodata .srodata.*)
        
        *(.data_end .data_end.*)        /* µOS++ __data_end_guard; must be last */        
        . = ALIGN(4);
        __data_end__ = . ;                /* Standard newlib definition. */
    } >RAM AT>FLASH :phdr_ram_init

    /* 
     * This address is used by the µOS++ startup code to 
     * initialise the .data section.
     */
    __data_load_addr__ = LOADADDR(.data);

      . = ALIGN(4);
      PROVIDE( __edata = . );
      PROVIDE( _edata = . );
      PROVIDE( edata = . );
    
    /*
     * The uninitialised data sections. NOLOAD is used to avoid
     * the "section `.bss' type changed to PROGBITS" warning
     */

    /* The primary uninitialised data section. */
    .bss (NOLOAD) : ALIGN(4)
    {
        __bss_start = .;                 /* Standard newlib definition. */
        __bss_start__ = .;                 /* Standard newlib definition. */
           __bss_begin__ = .;                 /* µOS++ specific */
         *(.bss_begin .bss_begin.*)     /* µOS++ __bss_begin_guard */ 

        *(.sbss .sbss.*)
        *(.gnu.linkonce.sb.*)
        
        *(.bss .bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
        
        *(.bss_end .bss_end.*)             /* µOS++ __bss_end_guard; must be last */
        . = ALIGN(4);
        __bss_end__ = .;                /* Standard newlib definition. */
        __bss_end = .;                    /* Standard newlib definition. */
     } >RAM AT>RAM :phdr_ram

    /* 
     * Similar to .bss, but not initialised to zero. µOS++ extension.
     */
    .noinit (NOLOAD) : ALIGN(4)
    {
        __noinit_begin__ = .;            /* µOS++ extension. */
        
        *(.noinit .noinit.*) 
        
         . = ALIGN(4) ;
        __noinit_end__ = .;               /* µOS++ extension. */
    } >RAM AT>RAM :phdr_ram
    
    /* _sbrk() expects at least word alignment. */
      . = ALIGN(8);
    PROVIDE( __end = . );
    PROVIDE( _end = . );
    PROVIDE( end = . ); 
    
    PROVIDE( __heap_begin__ = . );        /* µOS++ extension. */
    
      .stack __stack - __stack_size :
      {
        PROVIDE( _heap_end = . );        /* Standard newlib definition. */
        PROVIDE( __heap_end__ = . );    /* µOS++ extension. */
        . += __stack_size;
      } >RAM AT>RAM :phdr_ram
    

    /* 
     * Stabs debugging sections.
     */
    .stab          0 : { *(.stab) }
    .stabstr       0 : { *(.stabstr) }
    .stab.excl     0 : { *(.stab.excl) }
    .stab.exclstr  0 : { *(.stab.exclstr) }
    .stab.index    0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment       0 : { *(.comment) }
    
    /*
     * DWARF debug sections.
     * Symbols in the DWARF debugging sections are relative to the beginning
     * of the section so we begin them at 0.  
     */
    /* DWARF 1 */
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    /* GNU DWARF 1 extensions */
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    /* DWARF 1.1 and DWARF 2 */
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    /* DWARF 2 */
    .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }
    /* SGI/MIPS DWARF 2 extensions */
    .debug_weaknames 0 : { *(.debug_weaknames) }
    .debug_funcnames 0 : { *(.debug_funcnames) }
    .debug_typenames 0 : { *(.debug_typenames) }
    .debug_varnames  0 : { *(.debug_varnames) }    
}

