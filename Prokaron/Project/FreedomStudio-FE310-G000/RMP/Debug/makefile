################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Platform/subdir.mk
-include Kernel/subdir.mk
-include Benchmark/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
RMP.hex \

SECONDARY_SIZE += \
RMP.siz \


# All Target
all:
	$(MAKE) --no-print-directory pre-build
	$(MAKE) --no-print-directory main-build

# Main-build Target
main-build: RMP.elf secondary-outputs

# Tool invocations
RMP.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU RISC-V Cross C++ Linker'
	riscv64-unknown-elf-g++ -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fno-move-loop-invariants -Wunused -Wuninitialized -Wall -Wmissing-declarations -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wno-sign-conversion -Wno-implicit-fallthrough  -g3 -T "/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/FreedomStudio-FE310-G000/RMP/rmp_platform_FE310-G000.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"RMP.map" --specs=nano.specs --specs=nosys.specs -o "RMP.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

RMP.hex: RMP.elf
	@echo 'Invoking: GNU RISC-V Cross Create Flash Image'
	riscv64-unknown-elf-objcopy -O ihex "RMP.elf"  "RMP.hex"
	@echo 'Finished building: $@'
	@echo ' '

RMP.siz: RMP.elf
	@echo 'Invoking: GNU RISC-V Cross Print Size'
	riscv64-unknown-elf-size --format=berkeley "RMP.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) RMP.elf
	-@echo ' '

pre-build:
	-../copy.sh
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build pre-build

-include ../makefile.targets
