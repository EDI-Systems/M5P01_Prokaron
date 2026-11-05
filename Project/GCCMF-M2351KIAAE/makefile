###############################################################################
#Filename    : RMP
#Author      : pry
#Date        : 03/11/2026
#Licence     : The Unlicense; see LICENSE for details.
#Description : Generic Makefile (based on gcc). This file is intended
#              to be used with Nuvoton M2351KIAAE (NuMaker-PFM-M2351), and the
#              GNU toolchain. The Cortex-M23 is an ARMv8-M BASELINE core, so
#              this project builds the A8MB port.
#              Firmware library: M0A00_Library/M2351BSP.
###############################################################################

# Config ######################################################################
TARGET=RMP
CPU=-mcpu=cortex-m23 -mthumb -mfloat-abi=soft
CDEFS=
CFLAGS=-O3 -specs=nano.specs -fsigned-char -fno-common -fno-strict-aliasing -fdata-sections -ffunction-sections -ffreestanding
AFLAGS=-fdata-sections -ffunction-sections
WFLAGS=-Wall -Wno-strict-aliasing
DFLAGS=-g3
LFLAGS=-specs=nano.specs -specs=nosys.specs -Wl,--gc-sections,--cref

OBJDIR=Object
PREFIX=arm-none-eabi-
# End Config ##################################################################

# Source ######################################################################
BSP=../../../M0A00_Library/M2351BSP/Library

INCS+=-I.
INCS+=-I../../Include
INCS+=-I$(BSP)/CMSIS/Include
INCS+=-I$(BSP)/Device/Nuvoton/M2351/Include
INCS+=-I$(BSP)/StdDriver/inc

CSRCS+=../../Source/Kernel/rmp_kernel.c
CSRCS+=../../Source/Test/rmp_benchmark.c
CSRCS+=../../Source/Platform/A8MB/rmp_platform_a8mb.c
CSRCS+=rmp_stubs.c
CSRCS+=$(BSP)/Device/Nuvoton/M2351/Source/system_M2351.c
CSRCS+=$(BSP)/StdDriver/src/clk.c
CSRCS+=$(BSP)/StdDriver/src/sys.c
CSRCS+=$(BSP)/StdDriver/src/uart.c
CSRCS+=$(BSP)/StdDriver/src/timer.c

ASRCS+=../../Source/Platform/A8MB/rmp_platform_a8mb_gcc.s
ASRCS+=$(BSP)/Device/Nuvoton/M2351/Source/GCC/startup_M2351.S

LDSCRIPT=APROM.ld
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
AOBJS=$(ASRCS:%.s=%.o) $(ASRCS:%.S=%.o)
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
	@$(CC) -c $(CPU) $(CDEFS) $(INCS) $(CFLAGS) $(DFLAGS) -MMD -MP -MF "$(DEP)" -Wa,-a,-ad,-alms="$(LST)" "$<" -o "$(OBJ)"

# Assemble ASM sources (lower case: already preprocessed-agnostic, upper case:
# goes through the C preprocessor)
%.o:%.s
	@echo "    AS      $(notdir $<)"
	@$(AS) -c $(CPU) $(INCS) $(AFLAGS) $(DFLAGS) "$<" -o "$(OBJ)"

%.o:%.S
	@echo "    AS      $(notdir $<)"
	@$(AS) -c $(CPU) $(INCS) $(AFLAGS) $(DFLAGS) -MMD -MP -MF "$(DEP)" "$<" -o "$(OBJ)"

# Link ELF target file and print size
$(TARGET).elf:$(COBJS) $(AOBJS)
	@echo "    LD [P]  $(notdir $@)"
	@$(LD) $(OBJDIR)/*.o $(CPU) $(LFLAGS) $(DFLAGS) -T $(LDSCRIPT) -Wl,-Map=$(MAP) $(LIBS) -o "$(OBJ)"
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
