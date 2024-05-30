;
; Start-up code for cc65 (NES version)
;
; by Groepaz/Hitmen <groepaz@gmx.net>
; based on code by Ullrich von Bassewitz <uz@cc65.org>
;

        .export         _exit
        .export         __STARTUP__ : absolute = 1      ; Mark as startup

        .import         initlib, donelib, callmain
        .import         push0, _main, zerobss, copydata
        .import         ppubuf_flush

        ; Linker-generated symbols
        .import         __RAM_START__, __RAM_SIZE__
        .import         __SRAM_START__, __SRAM_SIZE__
        .import         __ROM0_START__, __ROM0_SIZE__
        .import         __STARTUP_LOAD__,__STARTUP_RUN__, __STARTUP_SIZE__
        .import         __CODE_LOAD__,__CODE_RUN__, __CODE_SIZE__
        .import         __RODATA_LOAD__,__RODATA_RUN__, __RODATA_SIZE__

; ------------------------------------------------------------------------
; Character data
; ------------------------------------------------------------------------
        .forceimport    NESfont

        .include        "zeropage.inc"
        .include        "nes.inc"


; ------------------------------------------------------------------------
; 16-byte INES header

.segment        "HEADER"

;    +--------+------+------------------------------------------+
;    | Offset | Size | Content(s)                               |
;    +--------+------+------------------------------------------+
;    |   0    |  3   | 'NES'                                    |
;    |   3    |  1   | $1A                                      |
;    |   4    |  1   | 16K PRG-ROM page count                   |
;    |   5    |  1   | 8K CHR-ROM page count                    |
;    |   6    |  1   | ROM Control Byte #1                      |
;    |        |      |   %####vTsM                              |
;    |        |      |    |  ||||+- 0=Horizontal mirroring      |
;    |        |      |    |  ||||   1=Vertical mirroring        |
;    |        |      |    |  |||+-- 1=SRAM enabled              |
;    |        |      |    |  ||+--- 1=512-byte trainer present  |
;    |        |      |    |  |+---- 1=Four-screen mirroring     |
;    |        |      |    |  |                                  |
;    |        |      |    +--+----- Mapper # (lower 4-bits)     |
;    |   7    |  1   | ROM Control Byte #2                      |
;    |        |      |   %####0000                              |
;    |        |      |    |  |                                  |
;    |        |      |    +--+----- Mapper # (upper 4-bits)     |
;    |  8-15  |  8   | $00                                      |
;    | 16-..  |      | Actual 16K PRG-ROM pages (in linear      |
;    |  ...   |      | order). If a trainer exists, it precedes |
;    |  ...   |      | the first PRG-ROM page.                  |
;    | ..-EOF |      | CHR-ROM pages (in ascending order).      |
;    +--------+------+------------------------------------------+

        .byte   $4e,$45,$53,$1a ; "NES"^Z
        .byte   2               ; ines prg  - Specifies the number of 16k prg banks.
        .byte   1               ; ines chr  - Specifies the number of 8k chr banks.
        .byte   %00110011       ; ines mir  - Specifies VRAM mirroring of the banks.
        .byte   %00010000       ; ines map  - Specifies the NES mapper used - Namco (Namcot) 163 (019).
        .byte   0,0,0,0,0,0,0,0 ; 8 zeroes - The first byte is actually PRGRAM, but iNES imply 8KiB.


; ------------------------------------------------------------------------
; Place the startup code in a special segment.

.segment        "STARTUP"

start:

; Set up the CPU and System-IRQ - don't touch PPU; cc65 will do it for us.

        sei
        cld

; Initialize Namco (Namcot) 163: enable PRG SRAM mapping, but disable sound.

        ldx     #$40            ; Disable WRAM write protect
        stx     $F800
        ldx     #$0             ; Map in correct PRG pages
        stx     $E000
        ldx     #$1
        stx     $E800
        ldx     #$2
        stx     $F000
        ldx     #$0             ; Map in correct CHR pages
        stx     $8000
        ldx     #$1
        stx     $8800
        ldx     #$2
        stx     $9000
        ldx     #$3
        stx     $9800
        ldx     #$4
        stx     $A000
        ldx     #$5
        stx     $A800
        ldx     #$6
        stx     $B000
        ldx     #$7
        stx     $B800

; Clear flag variables

        ldx     #0
        stx     VBLANK_FLAG     ; clear variables
        stx     ringread
        stx     ringwrite
        stx     ringcount
        ldx     #$ff
        txs                     ; set stack pointer - full descending stack

; Clear internal ring buffer RAM.
        lda     #$20
@l:     sta     ringbuff,x
        sta     ringbuff+$0100,x
        sta     ringbuff+$0200,x
        inx
        bne     @l

; Clear the BSS data.

        jsr     zerobss

; Initialize the data.

        jsr     copydata

; Set up the stack.

        lda     #<(__SRAM_START__ + __SRAM_SIZE__)
        ldx     #>(__SRAM_START__ + __SRAM_SIZE__)
        sta     sp
        stx     sp+1            ; Set argument stack ptr

; Call the module constructors.

        jsr     initlib

; Push the command-line arguments; and, call main().

        jsr     callmain

; Call the module destructors. This is also the exit() entry.

_exit:  jsr     donelib         ; Run module destructors

; Reset the NES.

        jmp start

; ------------------------------------------------------------------------
; System V-Blank Interrupt
; Updates PPU Memory (buffered).
; Updates VBLANK_FLAG and tickcount.
; We treat this as an kernel-agnostic FIQ that may preempt anything.
; ------------------------------------------------------------------------

nmi:    pha
        tya
        pha
        txa
        pha

        lda     #1
        sta     VBLANK_FLAG

        inc     tickcount
        bne     @s
        inc     tickcount+1

@s:     jsr     ppubuf_flush

        ; Reset the video counter.
        lda     #$20
        sta     PPU_VRAM_ADDR2
        lda     #$00
        sta     PPU_VRAM_ADDR2

        ; Reset scrolling.
        sta     PPU_VRAM_ADDR1
        sta     PPU_VRAM_ADDR1

        pla
        tax
        pla
        tay
        pla

; Interrupt exit

irq:
        rti


; ------------------------------------------------------------------------
; Hardware vectors
; ------------------------------------------------------------------------
        .import IRQ_Handler
.segment "VECTORS"

        .word   nmi         ; $fffa vblank nmi
        .word   start       ; $fffc reset
        .word   IRQ_Handler ; $fffe irq / brk
        ;.word   irq         ; $fffe irq / brk
