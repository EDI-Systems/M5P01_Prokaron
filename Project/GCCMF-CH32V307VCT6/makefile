###############################################################################
#Filename    : RMP
#Author      : hrs
#Date        : 29/04/2024
#Licence     : LGPL v3+; see COPYING for details.
#Description : Generic Makefile (based on gcc). This file is intended
#              to be used with CH32V307VCT6, and the GNU toolchain.
###############################################################################

# Config ######################################################################
TARGET=RMP
CPU=-march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore

CFLAGS=-O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common
AFLAGS=-O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -x assembler-with-cpp
WFLAGS=-Wunused -Wuninitialized
DFLAGS=-g3
LFLAGS= -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --specs=nano.specs --specs=nosys.specs

OBJDIR=Objects
PREFIX=riscv-none-elf-
# End Config ##################################################################

# Source ######################################################################
INCS+=-I.
INCS+=-I../../Include
INCS+=-I../../../M0A00_Library/CH32V30x/Debug
INCS+=-I../../../M0A00_Library/CH32V30x/Core
INCS+=-I../../../M0A00_Library/CH32V30x/User
INCS+=-I../../../M0A00_Library/CH32V30x/Peripheral/inc

CSRCS+=../../Source/Kernel/rmp_kernel.c
CSRCS+=../../Source/Test/rmp_benchmark.c
CSRCS+=../../Source/Platform/RV32P/rmp_platform_rv32p.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_adc.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_bkp.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_can.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_crc.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_dac.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_dbgmcu.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_dma.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_dvp.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_eth.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_exti.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_flash.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_fsmc.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_gpio.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_i2c.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_iwdg.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_misc.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_opa.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_pwr.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_rcc.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_rng.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_rtc.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_sdio.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_spi.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_tim.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_usart.c
CSRCS+=../../../M0A00_Library/CH32V30x/Peripheral/src/ch32v30x_wwdg.c
CSRCS+=../../../M0A00_Library/CH32V30x/Core/core_riscv.c
CSRCS+=../../../M0A00_Library/CH32V30x/Debug/debug.c
CSRCS+=../../../M0A00_Library/CH32V30x/User/ch32v30x_it.c
CSRCS+=../../../M0A00_Library/CH32V30x/User/system_ch32v30x.c

ASRCS+=./rmp_test_ch32v307vc_gcc.s
ASRCS+=../../Source/Platform/RV32P/rmp_platform_rv32p_gcc.s
ASRCS+=../../../M0A00_Library/CH32V30x/Startup/startup_ch32v30x_D8C_rmp.s

LDSCRIPT=./rmp_platform_ch32v307vc.ld
LIBS=-lc -lnosys
# End Source ##################################################################

# Toolchain ###################################################################
ifdef GCC
CC=$(GCC)/$(PREFIX)gcc
AS=$(GCC)/$(PREFIX)gcc -x assembler-with-cpp
CP=$(GCC)/$(PREFIX)objcopy
LD=$(GCC)/$(PREFIX)gcc
SZ=$(GCC)/$(PREFIX)size
else
CC=$(PREFIX)gcc
AS=$(PREFIX)gcc -x assembler-with-cpp
CP=$(PREFIX)objcopy
LD=$(PREFIX)gcc
SZ=$(PREFIX)size
endif

HEX=$(CP) -O ihex
BIN=$(CP) -O binary -S
# End Toolchain ###############################################################

# User ########################################################################
-include user
# End User ####################################################################

# Build #######################################################################
COBJS=$(CSRCS:%.c=%.o)
CDEPS=$(CSRCS:%.c=%.d)
AOBJS=$(ASRCS:%.s=%.o)
ADEPS=$(ASRCS:%.s=%.d)

DEP=$(OBJDIR)/$(notdir $(@:%.o=%.d))
LST=$(OBJDIR)/$(notdir $(@:%.o=%.lst))
OBJ=$(OBJDIR)/$(notdir $@)
MAP=$(OBJDIR)/$(TARGET).map

# Build all
all: mkdir $(COBJS) $(AOBJS) $(TARGET).elf $(TARGET).hex $(TARGET).bin

# Create output folder
mkdir:
	$(shell if [ ! -e $(OBJDIR) ];then mkdir -p $(OBJDIR); fi)

# Compile C sources
%.o:%.c
	@echo "    CC      $(notdir $<)"
	@$(CC) -c $(CPU) $(CFLAGS) $(WFLAGS) $(DFLAGS) $(INCS) -std=gnu99 -MMD -MP -MF"$(DEP)" -MT"$(DEP)" "$<" -o "$(OBJ)"

# Assemble ASM sources
%.o:%.s
	@echo "    AS      $(notdir $<)"
	@$(AS) -c $(CPU) $(AFLAG) $(WFLAGS) $(DFLAGS) $(INCS) -MMD -MP -MF"$(DEP)" -MT"$(DEP)" "$<" -o "$(OBJ)"

# Link ELF target file and print size
$(TARGET).elf:$(COBJS) $(AOBJS)
	@echo "    LD [P]  $(notdir $@)"
	@$(LD) $(OBJDIR)/*.o $(CPU) $(LFLAGS) $(WFLAGS) $(DFLAGS) -T $(LDSCRIPT) -nostartfiles -Xlinker --gc-sections -Wl,-Map,$(MAP) $(LIBS) -o $(OBJ)
	@$(SZ) $(OBJ)

# Create hex/bin programming files
$(TARGET).hex:$(TARGET).elf
	@echo "    HEX     $(notdir $@)"
	@$(HEX) "$(OBJDIR)/$<" "$(OBJDIR)/$@"

$(TARGET).bin:$(TARGET).elf
	@echo "    BIN     $(notdir $@)"
	@$(BIN) "$(OBJDIR)/$<" "$(OBJDIR)/$@"

# Clean up
clean:
	-rm -rf $(OBJDIR)

# Dependencies
-include $(wildcard $(OBJDIR)/*.d)
# End Build ###################################################################

# End Of File #################################################################

# Copyright (C) Evo-Devo Instrum. All rights reserved #########################

