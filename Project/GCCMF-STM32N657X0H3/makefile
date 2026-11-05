###############################################################################
#Filename    : RMP
#Author      : pry
#Date        : 02/11/2026
#Licence     : The Unlicense; see LICENSE for details.
#Description : Generic Makefile (based on gcc). This file is intended
#              to be used with STM32N657X0H3, and the GNU toolchain.
#              The image is built for RAM execution (the chip has no internal
#              flash). The board must be in DEV boot mode (BOOT1 jumper on
#              2-3); flash it with the ST OpenOCD build from CubeIDE and the
#              script in Document/Private/Agent_Workspace/n657_flash.ocd
#              (reset-halt, download to AXISRAM1, jump to Reset_Handler),
#              then read the benchmark from the STLINK VCP (USART1, PE5).
###############################################################################

# Config ######################################################################
TARGET=RMP
CPU=-mcpu=cortex-m55 -mthumb -mfpu=auto -mfloat-abi=hard
CDEFS=-DSTM32N657xx -DUSE_HAL_DRIVER

CFLAGS=-mcmse -O3 -specs=nano.specs -fsigned-char -fno-common -fno-strict-aliasing -fdata-sections -ffunction-sections -ffreestanding
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
INCS+=-I../../../M0A00_Library/STM32Cube_FW_N6_V1.1.0/Drivers/CMSIS/Include
INCS+=-I../../../M0A00_Library/STM32Cube_FW_N6_V1.1.0/Drivers/CMSIS/Device/ST/STM32N6xx/Include
INCS+=-I../../../M0A00_Library/STM32Cube_FW_N6_V1.1.0/Drivers/CMSIS/Device/ST/STM32N6xx/Include/Templates
INCS+=-I../../../M0A00_Library/STM32Cube_FW_N6_V1.1.0/Drivers/STM32N6xx_HAL_Driver/Inc

CSRCS+=../../Source/Kernel/rmp_kernel.c
CSRCS+=../../Source/Test/rmp_benchmark.c
CSRCS+=../../Source/Platform/A8MM/rmp_platform_a8mm.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_N6_V1.1.0/Drivers/CMSIS/Device/ST/STM32N6xx/Source/Templates/system_stm32n6xx_s.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_N6_V1.1.0/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_N6_V1.1.0/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_cortex.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_N6_V1.1.0/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_rcc.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_N6_V1.1.0/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_rcc_ex.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_N6_V1.1.0/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_gpio.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_N6_V1.1.0/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_pwr.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_N6_V1.1.0/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_pwr_ex.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_N6_V1.1.0/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_tim.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_N6_V1.1.0/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_tim_ex.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_N6_V1.1.0/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_uart.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_N6_V1.1.0/Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_uart_ex.c

ASRCS+=../../Source/Platform/A8MM/rmp_platform_a8mm_gcc.s
ASRCS+=../../../M0A00_Library/STM32Cube_FW_N6_V1.1.0/Drivers/CMSIS/Device/ST/STM32N6xx/Source/Templates/gcc/startup_stm32n657xx.s

LDSCRIPT=rmp_platform_stm32n657x0h3.ld
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

# Assemble ASM sources (lower case goes through cpp via $(AS) anyway,
# upper case is preprocessed by the compiler driver regardless)
%.o:%.s
	@echo "    AS      $(notdir $<)"
	@$(AS) -c $(CPU) $(INCS) $(AFLAGS) $(DFLAGS) "$<" -o "$(OBJ)"

%.o:%.S
	@echo "    AS      $(notdir $<)"
	@$(AS) -c $(CPU) $(INCS) $(AFLAGS) $(DFLAGS) "$<" -o "$(OBJ)"

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
