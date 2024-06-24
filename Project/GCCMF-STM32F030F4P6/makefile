###############################################################################
#Filename    : RMP
#Author      : hrs
#Date        : 29/11/2023 16:55:31
#Licence     : LGPL v3+; see COPYING for details.
#Description : Generic Makefile (based on gcc). This file is intended
#              to be used with STM32F030F4, and the GNU toolchain.
###############################################################################

# Config ######################################################################
TARGET=RMP
CPU=-mcpu=cortex-m0 -mthumb
CDEFS=-DSTM32F030x6 -DUSE_HAL_DRIVER

CFLAGS=-Os -specs=nano.specs -fsigned-char -fno-common -fno-strict-aliasing -fdata-sections -ffunction-sections -ffreestanding
AFLAGS=-fdata-sections -ffunction-sections
WFLAGS=-Wall -Wno-strict-aliasing
DFLAGS=-g3
LFLAGS=-specs=nano.specs -specs=nosys.specs -Wl,--gc-sections,--cref

OBJDIR=Object
PREFIX=arm-none-eabi-
# End Config ##################################################################

# Source ######################################################################
INCS+=-I.
INCS+=-I../../Include
INCS+=-I../../../M0A00_Library/STM32Cube_FW_F0_V1.9.0/Drivers/CMSIS/Include
INCS+=-I../../../M0A00_Library/STM32Cube_FW_F0_V1.9.0/Drivers/CMSIS/Device/ST/STM32F0xx/Include
INCS+=-I../../../M0A00_Library/STM32Cube_FW_F0_V1.9.0/Drivers/STM32F0xx_haL_Driver/Inc
INCS+=-I../../../M0A00_Library/STM32Cube_FW_F0_V1.9.0/Drivers/STM32F0xx_HAL_Driver/Inc/Conf

CSRCS+=../../Source/Kernel/rmp_kernel.c
CSRCS+=../../Source/Test/rmp_benchmark.c
CSRCS+=../../Source/Platform/A6M/rmp_platform_a6m.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F0_V1.9.0/Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/system_stm32f0xx.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F0_V1.9.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F0_V1.9.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F0_V1.9.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F0_V1.9.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F0_V1.9.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F0_V1.9.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F0_V1.9.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F0_V1.9.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dma.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F0_V1.9.0/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c

ASRCS+=../../Source/Platform/A6M/rmp_platform_a6m_gcc.s
ASRCS+=../../../M0A00_Library/STM32Cube_FW_F0_V1.9.0/Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/gcc/startup_stm32f030x6.s

LDSCRIPT=rmp_platform_stm32f030f4.ld
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
	@$(CC) -c $(CPU) $(CDEFS) $(INCS) $(CFLAGS) $(DFLAGS) -MMD -MP -MF "$(DEP)" -Wa,-a,-ad,-alms="$(LST)" "$<" -o "$(OBJ)"

# Assemble ASM sources
%.o:%.s
	@echo "    AS      $(notdir $<)"
	@$(AS) -c $(CPU) $(INCS) $(AFLAGS) $(DFLAGS) "$<" -o "$(OBJ)"

# Link ELF target file and print size
$(TARGET).elf:$(COBJS) $(AOBJS)
	@echo "    LD [P]  $(notdir $@)"
	@$(LD) $(OBJDIR)/*.o $(CPU) $(LFLAGS) $(DFLAGS) -T $(LDSCRIPT) -Wl,-Map=$(MAP) $(LIBS) -o $(OBJ)
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

