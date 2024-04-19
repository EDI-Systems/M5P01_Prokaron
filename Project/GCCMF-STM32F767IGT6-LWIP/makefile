###############################################################################
#Filename    : RMP
#Author      : hrs
#Date        : 18/4/2024 16:55:31
#Licence     : LGPL v3+; see COPYING for details.
#Description : Generic Makefile (based on gcc). This file is intended
#              to be used with STM32F767IG, and the GNU toolchain.
###############################################################################

# Config ######################################################################
TARGET=RMP_LWIP_IPERF
CPU=-mcpu=cortex-m7 -mthumb
CDEFS=-DSTM32F767xx -DUSE_HAL_DRIVER

CFLAGS=-O3 -specs=nano.specs -fsigned-char -fno-common -fno-strict-aliasing -fdata-sections -ffunction-sections -ffreestanding
AFLAGS=-fdata-sections -ffunction-sections
WFLAGS=-Wall -Wno-strict-aliasing
DFLAGS=-g3
LFLAGS=-specs=nano.specs -specs=nosys.specs -Wl,--gc-sections,--cref

OBJDIR=Objects
PREFIX=arm-none-eabi-
# End Config ##################################################################

# Source ######################################################################
INCS+=-I.
INCS+=-I../RVMDK-STM32F767IGT6-LWIP
INCS+=-I../../Include
INCS+=-I../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/CMSIS/Include
INCS+=-I../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/CMSIS/Device/ST/STM32F7xx/Include
INCS+=-I../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Inc
INCS+=-I../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Inc/Conf
INCS+=-I../../../M0A00_Library/lwIP_V2.2.0/src/apps
INCS+=-I../../../M0A00_Library/lwIP_V2.2.0/src/include
INCS+=-I../../../M5P03_Slime/Softbus/SWIIC
INCS+=-I../../../M5P03_Slime/Expansion/PCF8574

CSRCS+=../../Source/Kernel/rmp_kernel.c
CSRCS+=../../Source/Platform/A7M/rmp_platform_a7m.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_cortex.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_dma.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_eth.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_gpio.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_pwr_ex.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_rcc.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_rtc.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_rtc_ex.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_tim.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_tim_ex.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_uart.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Src/STM32F7xx_hal_usart.c
CSRCS+=../../../M0A00_Library/STM32Cube_FW_F7_V1.11.0/Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/system_stm32f7xx.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/api/api_lib.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/api/api_msg.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/api/err.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/api/if_api.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/api/netbuf.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/api/netdb.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/api/netifapi.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/api/sockets.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/api/tcpip.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/altcp.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/altcp_alloc.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/altcp_tcp.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/def.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/dns.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/inet_chksum.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/init.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/ip.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/mem.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/memp.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/netif.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/pbuf.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/raw.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/stats.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/sys.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/tcp.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/tcp_in.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/tcp_out.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/timeouts.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/udp.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/ipv4/autoip.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/ipv4/dhcp.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/ipv4/etharp.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/ipv4/icmp.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/ipv4/igmp.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/ipv4/ip4.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/ipv4/ip4_addr.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/core/ipv4/ip4_frag.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/netif/ethernet.c
CSRCS+=../../../M0A00_Library/lwIP_V2.2.0/src/apps/lwiperf/lwiperf.c
CSRCS+=../../../M5P03_Slime/Softbus/SWIIC/swiic.c
CSRCS+=../../../M5P03_Slime/Expansion/PCF8574/pcf8574.c
CSRCS+=../RVMDK-STM32F767IGT6-LWIP/arch/ethernetif.c
CSRCS+=../RVMDK-STM32F767IGT6-LWIP/arch/sys_arch.c
CSRCS+=../../Source/Test/rmp_lwip_iperf.c

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

