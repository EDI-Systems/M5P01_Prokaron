###############################################################################
#Filename    : RMP
#Author      : ryp
#Date        : 29/04/2024
#Licence     : LGPL v3+; see COPYING for details.
#Description : Generic Makefile (based on cc65). This file is intended
#              to be used with NES, and the CC65 toolchain.
###############################################################################

# Config ######################################################################
TARGET=RMP
CPU=-t nes

CFLAGS=-Oirs -j --add-source
AFLAGS=
WFLAGS=
DFLAGS=-g
LFLAGS=

OBJDIR=Objects
CC65_HOME=C:/Toolchain/cc65
# End Config ##################################################################

# Source ######################################################################
INCS+=-I.
INCS+=-I../../Include

CSRCS+=../../Source/Kernel/rmp_kernel.c
CSRCS+=../../Source/Test/rmp_benchmark.c
CSRCS+=../../Source/Platform/M6502/rmp_platform_m6502.c

ASRCS+=./rmp_test_nes_cc65.s
ASRCS+=../../Source/Platform/M6502/rmp_platform_m6502_cc65.s

LDSCRIPT=./rmp_platform_nes.ld
LIBS=$(OBJDIR)/rmp.lib
# End Source ##################################################################

# Toolchain ###################################################################
CC=cc65
AS=ca65
LD=ld65
AR=ar65
CP=cp
RM=rm
# End Toolchain ###############################################################

# User ########################################################################
-include user
# End User ####################################################################

# Build #######################################################################
COBJS=$(CSRCS:%.c=%.o)
CASMS=$(CSRCS:%.c=%.s)
AOBJS=$(ASRCS:%.s=%.o)

ASM=$(OBJDIR)/$(notdir $(@:%.o=%.s))
OBJ=$(OBJDIR)/$(notdir $@)
MAP=$(OBJDIR)/$(TARGET).map

# Build all
all: mkdir rmp.lib $(COBJS) $(AOBJS) $(TARGET).nes

# Create output folder
mkdir:
	$(shell if [ ! -e $(OBJDIR) ];then mkdir -p $(OBJDIR); fi)

# Compose new library
rmp.lib:
	@echo "    ASAR    rmp.lib"
	@$(CP) $(CC65_HOME)/lib/nes.lib $(OBJDIR)/rmp.lib
	@$(AS) $(CPU) $(AFLAGS) $(WFLAGS) $(DFLAGS) $(INCS) "crt0.s" -o "$(OBJDIR)/crt0.o"
	@$(AR) a $(OBJDIR)/rmp.lib $(OBJDIR)/crt0.o
	@$(RM) -f $(OBJDIR)/crt0.o

# Compile C sources
%.o:%.c
	@echo "    CCAS    $(notdir $<)"
	@$(CC) $(CPU) $(CFLAGS) $(WFLAGS) $(DFLAGS) $(INCS) "$<" -o "$(ASM)"
	@$(AS) $(CPU) $(AFLAGS) $(WFLAGS) $(DFLAGS) $(INCS) "$(ASM)" -o "$(OBJ)"

# Assemble ASM sources
%.o:%.s
	@echo "    AS      $(notdir $<)"
	@$(AS) $(CPU) $(AFLAGS) $(WFLAGS) $(DFLAGS) $(INCS) "$<" -o "$(OBJ)"

# Link ELF target file and print size
$(TARGET).nes:$(COBJS) $(AOBJS) $(LIBS)
	@echo "    LD [P]  $(notdir $@)"
	@$(LD) $(LFLAGS) $(WFLAGS) -C $(LDSCRIPT) -m $(MAP) --dbgfile $(OBJDIR)/$(TARGET).dbg -o $(OBJ) $(OBJDIR)/*.o $(LIBS)

# Clean up
clean:
	-rm -rf $(OBJDIR)
# End Build ###################################################################

# End Of File #################################################################

# Copyright (C) Evo-Devo Instrum. All rights reserved #########################

