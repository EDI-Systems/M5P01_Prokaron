################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Benchmark/test.obj: F:/Code_Library/MCU/Mutatus/M5P1_MuProkaron/MProkaron/Benchmark/test.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program_Files_x86/TI/ccs740/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --code_model=large --data_model=large -O4 --opt_for_speed=5 --use_hw_mpy=F5 --include_path="D:/Program_Files_x86/TI/ccs740/ccsv7/ccs_base/msp430/include" --include_path="F:/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/CCS-MSP430FR5994/RMP" --include_path="F:/Code_Library/MCU/Mutatus/M5P1_MuProkaron/MProkaron/Include" --include_path="F:/Code_Library/MCU/Mutatus/M0P0_Library/MSP430FR5xx_6xx" --include_path="D:/Program_Files_x86/TI/ccs740/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="all" --advice:hw_config="all" --define=__MSP430FR5994__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="Benchmark/test.d_raw" --obj_directory="Benchmark" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


