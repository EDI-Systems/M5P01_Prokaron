/* GNU linker script for STM32L071 */

ENTRY(Reset_Handler)

/* Specify the memory areas */
MEMORY{
	RAM    (xrw)  : ORIGIN = 0x20000000, LENGTH = 20K
	FLASH  (rx)   : ORIGIN = 0x08000000, LENGTH = 128K
	EEPROM (rw)   : ORIGIN = 0x08080000, LENGTH = 6144
}

_estack = ORIGIN(RAM) + LENGTH(RAM);
__stack_size = 4k;

SECTIONS
{
    /* The startup code goes first into FLASH */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >FLASH

    .text :
    {
        . = ALIGN(8);
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        *(.glue_7)         /* glue arm to thumb code */
        *(.glue_7t)        /* glue thumb to arm code */
        *(.eh_frame)
    
        KEEP(*(.init))
        KEEP(*(.fini))
        . = ALIGN(8);
        _etext = .;
        . = ALIGN(8);
        PROVIDE(_sidata = .);
    } >FLASH

    .preinit_array     :
    {
        . = ALIGN(8);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } >FLASH
    .init_array :
    {
        . = ALIGN(8);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
    } >FLASH
    .fini_array :
    {
        . = ALIGN(8);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >FLASH

    .data : 
    {
        . = ALIGN(4);
        PROVIDE(_sdata = .);
        *(.data)
        *(.data*)
        . = ALIGN(4);
        PROVIDE(_edata = .);
    } >RAM AT> FLASH

    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        PROVIDE(_sbss = .);
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
        PROVIDE(_ebss = .);
    } >RAM

    /* User_heap_stack section, used to check that there is enough RAM left */
    .stack :
    {
        . = ALIGN(8);
        PROVIDE( _heap_end = . );    
        . = ALIGN(4);
        PROVIDE(_susrstack = . );
        . = . + __stack_size;
        PROVIDE( _eusrstack = .);
        PROVIDE( __initial_stack$ = .);
        PROVIDE( _RMP_Stack = .);
        PROVIDE( _estack = .);
    } >RAM
}
