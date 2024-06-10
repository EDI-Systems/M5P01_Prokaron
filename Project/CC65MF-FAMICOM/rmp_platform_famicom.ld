# Assuming iNES mapper 019, Namco 129, however we just leave the banking
# stuff untouched. If banking is required, some modifications might be needed.
# All other mappers were useless: they lack a readable free-running counter.

SYMBOLS {
    __STACKSIZE__: type = weak, value = $0300; # 3 pages stack
}
MEMORY {
    # This does not count as regular zeropage compiler variables; for PSP only
    SP:     file = "", start = $0002, size = $0002, type = rw, define = yes;
    # This includes regular zeropage compiler variables, just like all other registers
    ZP:     file = "", start = $0004, size = $0018, type = rw, define = yes;

    # INES Cartridge Header
    HEADER: file = %O, start = $0000, size = $0010, fill = yes;

    # 2 16K ROM Banks
    # - startup
    # - code
    # - rodata
    # - data (load)
    #ROM0:   file = %O, start = $8000, size = $7FFA, fill = yes, define = yes;
    ROM0:   file = %O, start = $8000, size = $7C00, fill = yes, define = yes;
    # Common ROM section for OS context switching and banking, etc.
    ROMS:   file = %O, start = $FC00, size = $3FA, fill = yes, define = yes;

    # Hardware Vectors at End of 2nd 8K ROM
    ROMV:   file = %O, start = $FFFA, size = $0006, fill = yes;

    # 1 8k CHR Bank
    ROM2:   file = %O, start = $0000, size = $2000, fill = yes;

    # standard 2k SRAM (-zeropage)
    # $0100-$0200 cpu stack
    # $0200-$0500 3 pages for ppu memory write buffer
    # $0500-$0800 3 pages for cc65 parameter stack
    SRAM:   file = "", start = $0500, size = __STACKSIZE__, define = yes;

    # additional 8K SRAM Bank
    # - data (run)
    # - bss
    # - heap
    RAM:    file = "", start = $6000, size = $2000, define = yes;
}
SEGMENTS {
    PARAMSTK: load = SP,              type = zp;
    ZEROPAGE: load = ZP,              type = zp;
    HEADER:   load = HEADER,          type = ro;
    STARTUP:  load = ROMS,            type = ro,  define   = yes;
    LOWCODE:  load = ROM0,            type = ro,  optional = yes;
    ONCE:     load = ROM0,            type = ro,  optional = yes;
    CODE:     load = ROM0,            type = ro,  define   = yes;
    RODATA:   load = ROM0,            type = ro,  define   = yes;
    DATA:     load = ROM0, run = RAM, type = rw,  define   = yes;
    VECTORS:  load = ROMV,            type = rw;
    CHARS:    load = ROM2,            type = rw;
    BSS:      load = RAM,             type = bss, define   = yes;
}
FEATURES
{
    CONDES: type    = constructor,
            label   = __CONSTRUCTOR_TABLE__,
            count   = __CONSTRUCTOR_COUNT__,
            segment = ONCE;
    CONDES: type    = destructor,
            label   = __DESTRUCTOR_TABLE__,
            count   = __DESTRUCTOR_COUNT__,
            segment = RODATA;
    CONDES: type    = interruptor,
            label   = __INTERRUPTOR_TABLE__,
            count   = __INTERRUPTOR_COUNT__,
            segment = RODATA,
            import  = __CALLIRQ__;
}