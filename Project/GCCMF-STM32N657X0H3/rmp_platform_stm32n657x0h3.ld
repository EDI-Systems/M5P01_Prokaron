/* GNU linker script for STM32N657X0.
   The STM32N6 has no internal flash; the whole image (vector table, code and
   data) is downloaded by the debugger into AXISRAM1 (secure alias) and run
   from there. The first 0x400 bytes of AXISRAM1 are reserved, following the
   ST "LRUN" linker script convention. */

ENTRY(Reset_Handler)

MEMORY
{
    /* Only AXISRAM1 (1 MiB): accessing the SRAM3 area at 0x34200000 hangs the
       AXI matrix on this chip, so keep every section well below 0x34100000 */
    RAM (xrw)      : ORIGIN = 0x34000400, LENGTH = 1023k   /* AXISRAM1, secure alias, 1023 KiB */
}

_Min_Heap_Size = 0x400;  /* required amount of heap  */
_Min_Stack_Size = 0x1000; /* required amount of stack */

/* Highest address of the boot stack (MSP), and its limit for MSPLIM */
_estack = ORIGIN(RAM) + LENGTH(RAM);
_sstack = _estack - _Min_Stack_Size;

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >RAM

    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        *(.glue_7)         /* glue arm to thumb code */
        *(.glue_7t)        /* glue thumb to arm code */
        *(.eh_frame)

        KEEP (*(.init))
        KEEP (*(.fini))
        . = ALIGN(4);
        _etext = .;
    } >RAM

    .ARM.extab :
    {
        . = ALIGN(4);
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(4);
    } >RAM

    .ARM :
    {
        . = ALIGN(4);
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
        . = ALIGN(4);
    } >RAM

    .preinit_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
        . = ALIGN(4);
    } >RAM

    .init_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
        . = ALIGN(4);
    } >RAM

    .fini_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
        . = ALIGN(4);
    } >RAM

    /* Pure RAM image: load address equals virtual address */
    _sidata = LOADADDR(.data);

    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } >RAM

    .noncacheable :
    {
        . = ALIGN(8);
        __snoncacheable = .;
        KEEP(*(.noncacheable))
        . = ALIGN(8);
        __enoncacheable = .;
    } >RAM

    .gnu.sgstubs :
    {
        . = ALIGN(4);
        *(.gnu.sgstubs*)   /* Secure Gateway stubs */
        . = ALIGN(4);
    } >RAM

    . = ALIGN(4);
    .bss :
    {
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } >RAM

    /* Heap and boot stack, used to check that enough RAM is left */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
    } >RAM

    .ARM.attributes 0 : { *(.ARM.attributes) }
}
