################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/Program_Files_x86/TI/ccs740/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Benchmark/rmp_test.obj" \
"./Kernel/rmp_kernel.obj" \
"./Lib/HL_esm.obj" \
"./Lib/HL_gio.obj" \
"./Lib/HL_notification.obj" \
"./Lib/HL_pinmux.obj" \
"./Lib/HL_rti.obj" \
"./Lib/HL_sci.obj" \
"./Lib/HL_sys_core.obj" \
"./Lib/HL_sys_intvecs.obj" \
"./Lib/HL_sys_mpu.obj" \
"./Lib/HL_sys_pmu.obj" \
"./Lib/HL_sys_startup.obj" \
"./Lib/HL_sys_vim.obj" \
"./Lib/HL_system.obj" \
"./Platform/rmp_platform_crx.obj" \
"./Platform/rmp_platform_crx_asm.obj" \
"../TMS570LC43xxFlashLnk.cmd" \
$(GEN_CMDS__FLAG) \
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
	@echo 'Invoking: ARM Linker'
	"D:/Program_Files_x86/TI/ccs740/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7R5 --code_state=32 --float_support=VFPv3D16 -O4 --opt_for_speed=5 -g --diag_warning=225 --diag_wrap=off --display_error_number --enum_type=packed --abi=eabi -z -m"RMP.map" --heap_size=0x800 --stack_size=0x800 -i"D:/Program_Files_x86/TI/ccs740/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/lib" -i"D:/Program_Files_x86/TI/ccs740/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="RMP_linkInfo.xml" --rom_model --be32 -o "RMP.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

RMP.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"D:/Program_Files_x86/TI/ccs740/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armhex"  -o "RMP.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "Benchmark\rmp_test.obj" "Kernel\rmp_kernel.obj" "Lib\HL_esm.obj" "Lib\HL_gio.obj" "Lib\HL_notification.obj" "Lib\HL_pinmux.obj" "Lib\HL_rti.obj" "Lib\HL_sci.obj" "Lib\HL_sys_core.obj" "Lib\HL_sys_intvecs.obj" "Lib\HL_sys_mpu.obj" "Lib\HL_sys_pmu.obj" "Lib\HL_sys_startup.obj" "Lib\HL_sys_vim.obj" "Lib\HL_system.obj" "Platform\rmp_platform_crx.obj" "Platform\rmp_platform_crx_asm.obj" 
	-$(RM) "Benchmark\rmp_test.d" "Kernel\rmp_kernel.d" "Lib\HL_esm.d" "Lib\HL_gio.d" "Lib\HL_notification.d" "Lib\HL_pinmux.d" "Lib\HL_rti.d" "Lib\HL_sci.d" "Lib\HL_sys_startup.d" "Lib\HL_sys_vim.d" "Lib\HL_system.d" "Platform\rmp_platform_crx.d" 
	-$(RM) "Lib\HL_sys_core.d" "Lib\HL_sys_intvecs.d" "Lib\HL_sys_mpu.d" "Lib\HL_sys_pmu.d" "Platform\rmp_platform_crx_asm.d" 
	-@echo 'Finished clean'
	-@echo ' '

pre-build:
	-F:\Code_Library\MCU\Mutatus\M5P1_MuProkaron\Project\CCS-TMS5704357BZWTQQ1\RMP/copy.bat
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

