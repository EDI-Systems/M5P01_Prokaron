################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/Program_Files_x86/TI/ccs740/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Benchmark/test.obj" \
"./Kernel/kernel.obj" \
"./Lib/cs.obj" \
"./Lib/eusci_a_uart.obj" \
"./Lib/gpio.obj" \
"./Lib/timer_a.obj" \
"./Platform/platform_msp430.obj" \
"./Platform/platform_msp430x_asm.obj" \
"../lnk_msp430fr5994.cmd" \
$(GEN_CMDS__FLAG) \
-llibmpu_init.a \
-llibmath.a \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Benchmark/subdir_vars.mk
-include Kernel/subdir_vars.mk
-include Lib/subdir_vars.mk
-include Platform/subdir_vars.mk
-include subdir_rules.mk
-include Benchmark/subdir_rules.mk
-include Kernel/subdir_rules.mk
-include Lib/subdir_rules.mk
-include Platform/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
RMP.out \

EXE_OUTPUTS__QUOTED += \
"RMP.out" \

BIN_OUTPUTS += \
RMP.hex \

BIN_OUTPUTS__QUOTED += \
"RMP.hex" \


# All Target
all:
	@$(MAKE) --no-print-directory pre-build
	@$(MAKE) --no-print-directory main-build

# Main-build Target
main-build: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "RMP.out"

# Tool invocations
RMP.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: MSP430 Linker'
	"D:/Program_Files_x86/TI/ccs740/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --code_model=large --data_model=large -O4 --opt_for_speed=5 --use_hw_mpy=F5 --advice:power="all" --advice:hw_config="all" --define=__MSP430FR5994__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 -z -m"RMP.map" --heap_size=160 --stack_size=160 --cinit_hold_wdt=on -i"D:/Program_Files_x86/TI/ccs740/ccsv7/ccs_base/msp430/include" -i"D:/Program_Files_x86/TI/ccs740/ccsv7/ccs_base/msp430/lib/5xx_6xx_FRxx" -i"D:/Program_Files_x86/TI/ccs740/ccsv7/ccs_base/msp430/lib/FR59xx" -i"D:/Program_Files_x86/TI/ccs740/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/lib" -i"D:/Program_Files_x86/TI/ccs740/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --priority --reread_libs --define=_MPU_ENABLE --diag_wrap=off --display_error_number --warn_sections --xml_link_info="RMP_linkInfo.xml" --use_hw_mpy=F5 --rom_model -o "RMP.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

RMP.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: MSP430 Hex Utility'
	"D:/Program_Files_x86/TI/ccs740/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/hex430" --memwidth=8 --romwidth=8 -o "RMP.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "Benchmark\test.obj" "Kernel\kernel.obj" "Lib\cs.obj" "Lib\eusci_a_uart.obj" "Lib\gpio.obj" "Lib\timer_a.obj" "Platform\platform_msp430.obj" "Platform\platform_msp430x_asm.obj" 
	-$(RM) "Benchmark\test.d" "Kernel\kernel.d" "Lib\cs.d" "Lib\eusci_a_uart.d" "Lib\gpio.d" "Lib\timer_a.d" "Platform\platform_msp430.d" 
	-$(RM) "Platform\platform_msp430x_asm.d" 
	-@echo 'Finished clean'
	-@echo ' '

pre-build:
	-F:\Code_Library\MCU\Mutatus\M5P1_MuProkaron\Project\CCS-MSP430FR5994\RMP/copy.bat
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

