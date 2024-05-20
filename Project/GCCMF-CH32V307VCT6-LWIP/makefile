###############################################################################
#Filename    : RMP
#Author      : hrs
#Date        : 19/05/2024
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
INCS+=-I../MRS-CH32V307VCT6-LWIP
INCS+=-I../../Include
INCS+=-I../../../M0A00_Library/CH32V30x/Debug
INCS+=-I../../../M0A00_Library/CH32V30x/Core
INCS+=-I../../../M0A00_Library/CH32V30x/User
INCS+=-I../../../M0A00_Library/CH32V30x/Peripheral/inc
INCS+=-I../../../M0A00_Library/lwIP_V2.2.0/src/apps
INCS+=-I../../../M0A00_Library/lwIP_V2.2.0/src/include

CSRCS+=../../Source/Kernel/rmp_kernel.c
CSRCS+=../../Source/Test/rmp_lwip_iperf.c
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
CSRCS+=../MRS-CH32V307VCT6-LWIP/arch/ethernetif.c
CSRCS+=../MRS-CH32V307VCT6-LWIP/arch/sys_arch.c

ASRCS+=../MRS-CH32V307VCT6-LWIP/rmp_lwip_ch32v307vc_gcc.s
ASRCS+=../../Source/Platform/RV32P/rmp_platform_rv32p_gcc.s
ASRCS+=../../../M0A00_Library/CH32V30x/Startup/startup_ch32v30x_D8C_rmp.s

LDSCRIPT=../MRS-CH32V307VCT6-LWIP/rmp_platform_ch32v307vc.ld
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

