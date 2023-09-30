################################################################################
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include Kernel/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
RMP.hex \

SECONDARY_LIST += \
RMP.lst \

SECONDARY_SIZE += \
RMP.siz \


# 所有目标
all: RMP.elf secondary-outputs

# 工具调用
RMP.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g3 -T "D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"RMP.map" --specs=nano.specs --specs=nosys.specs -o "RMP.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
RMP.hex: RMP.elf
	@	riscv-none-embed-objcopy -O ihex "RMP.elf"  "RMP.hex"
	@	@
RMP.lst: RMP.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "RMP.elf" > "RMP.lst"
	@	@
RMP.siz: RMP.elf
	@	riscv-none-embed-size --format=berkeley "RMP.elf"
	@	@
# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) RMP.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
