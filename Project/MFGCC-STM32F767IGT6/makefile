###############################################################################
#Filename    : RMP
#Author      : hrs
#Date        : 29/11/2023 16:55:31
#Licence     : LGPL v3+; see COPYING for details.
#Description : Generic Makefile (based on gcc). This file is intended
#              to be used with STM32F405RG, and the GNU toolchain.
###############################################################################

# Config ######################################################################
TARGET=RMP
CPU=-mcpu=cortex-m7 -mthumb
CDEFS=-DSTM32F767xx -DUSE_HAL_DRIVER

CFLAGS=-O3 -specs=nano.specs -fsigned-char -fno-common -fno-strict-aliasing -fdata-sections -ffunction-sections -ffreestanding
AFLAGS=-fdata-sections -ffunction-sections
WFLAGS=-Wall -Wno-strict-aliasing
DFLAGS=-g3 -gdwarf-2
LFLAGS=-specs=nano.specs -specs=nosys.specs -Wl,--gc-sections,--cref

OBJDIR=Objects
PREFIX=arm-none-eabi-
# End Config ##################################################################

# Source ######################################################################
INCS+=-I./Include
INCS+=-I../../Include
INCS+=-I../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/CMSIS/Include
INCS+=-I../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/CMSIS/Device/ST/STM32F7xx/Include
INCS+=-I../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Inc
INCS+=-I../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Inc/Conf

CSRCS+=../../Source/Kernel/rmp_kernel.c
CSRCS+=../../Source/Test/rmp_benchmark.c
CSRCS+=../../Source/Platform/A7M/rmp_platform_a7m.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_rcc.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_cortex.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_gpio.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_tim.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_tim_ex.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_dma.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_dma_ex.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_uart.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_pwr_ex.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/system_stm32f7xx.c

ASRCS+=../../Source/Platform/A7M/rmp_platform_a7m_gcc.s
ASRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/gcc/startup_stm32f767xx.s

LDSCRIPT=rmp_platform_stm32f767ig.ld
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
	$(CC) -c $(CPU) $(CDEFS) $(INCS) $(CFLAGS) -MMD -MP -MF "$(DEP)" -Wa,-a,-ad,-alms="$(LST)" "$<" -o "$(OBJ)"

# Assemble ASM sources
%.o:%.s
	$(AS) -c $(CPU) $(INCS) $(AFLAGS) "$<" -o "$(OBJ)"

# Link ELF target file and print size
$(TARGET).elf:$(COBJS) $(AOBJS)
	$(LD) $(OBJDIR)/*.o $(CPU) $(LFLAGS) -T $(LDSCRIPT) -Wl,-Map=$(MAP) $(LIBS) -o $(OBJ)
	$(SZ) $(OBJ)

# Create hex/bin programming files
$(TARGET).hex:$(TARGET).elf
	$(HEX) "$(OBJDIR)/$<" "$(OBJDIR)/$@"

$(TARGET).bin:$(TARGET).elf
	$(BIN) "$(OBJDIR)/$<" "$(OBJDIR)/$@"

# Clean up
clean:
	-rm -rf $(OBJDIR)

# Dependencies
-include $(wildcard $(OBJDIR)/*.d)
# End Build ###################################################################

# End Of File #################################################################

# Copyright (C) Evo-Devo Instrum. All rights reserved #########################

