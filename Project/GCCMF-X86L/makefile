###############################################################################
#Filename    : RMP
#Author      : hrs
#Date        : 17/12/2023 16:15:00
#Licence     : LGPL v3+; see COPYING for details.
#Description : Generic Makefile (based on gcc). This file is intended
#              to be used with Linux, and the GNU toolchain.
###############################################################################

# Config ######################################################################
TARGET=RMP
CPU=-m32

CFLAGS=-O3 -fmessage-length=0
WFLAGS=-Wall
DFLAGS=-g3

OBJDIR=Object
# End Config ##################################################################

# Source ######################################################################
INCS+=-I.
INCS+=-I/usr/include/x86_64-linux-gnu
INCS+=-I../../Include

CSRCS+=../../Source/Platform/X86L/rmp_platform_x86l.c
CSRCS+=../../Source/Kernel/rmp_kernel.c
CSRCS+=../../Source/Test/rmp_benchmark.c

LIBS=-lpthread
# End Source ##################################################################

# User ########################################################################
-include user
# End User ####################################################################

# Build #######################################################################
COBJS=$(CSRCS:%.c=%.o)
CDEPS=$(CSRCS:%.c=%.d)

DEP=$(OBJDIR)/$(notdir $(@:%.o=%.d))
OBJ=$(OBJDIR)/$(notdir $@)

# Build all
all: mkdir $(COBJS) $(TARGET)

# Create output folder
mkdir:
	$(shell if [ ! -e $(OBJDIR) ];then mkdir -p $(OBJDIR); fi)

# Compile C sources
%.o:%.c
	@echo "    CC      $(notdir $<)"
	@gcc -c $(CPU) $(INCS) $(CFLAGS) $(DFLAGS) $(WFLAGS) -MMD -MP -MF "$(DEP)" "$<" -o "$(OBJ)";

$(TARGET): $(COBJS)
	@echo "    BIN     $(notdir $@)"
	@gcc $(OBJDIR)/*.o $(CPU) -static $(LIBS) -o $(OBJ)

# Clean up
clean:
	-rm -rf $(OBJDIR)

# Dependencies
-include $(wildcard $(OBJDIR)/*.d)
# End Build ###################################################################

# End Of File #################################################################

# Copyright (C) Evo-Devo Instrum. All rights reserved #########################
