################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/media/pry/Code/Code_Library/MCU/Mutatus/M7M2_MuAmmonite/MAmmonite/Guest/CortexM/rvm_guest_cmx.c 

S_UPPER_SRCS += \
/media/pry/Code/Code_Library/MCU/Mutatus/M7M2_MuAmmonite/MAmmonite/Guest/CortexM/rvm_guest_cmx_asm_gcc.S 

OBJS += \
./Lib/rvm_guest_cmx.o \
./Lib/rvm_guest_cmx_asm_gcc.o 

C_DEPS += \
./Lib/rvm_guest_cmx.d 


# Each subdirectory must supply rules for building sources it contributes
Lib/rvm_guest_cmx.o: /media/pry/Code/Code_Library/MCU/Mutatus/M7M2_MuAmmonite/MAmmonite/Guest/CortexM/rvm_guest_cmx.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DSTM32F767xx -DUSE_HAL_DRIVER -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/ECLIPSE-GCC-STM32F767IGT6-RVM/../../MProkaron/Include" -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/ECLIPSE-GCC-STM32F767IGT6-RVM/../../MProkaron/Include/Platform/CortexM_RVM" -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/ECLIPSE-GCC-STM32F767IGT6-RVM/../../../M0P0_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Inc" -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/ECLIPSE-GCC-STM32F767IGT6-RVM/../../../M0P0_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Inc/Conf" -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/ECLIPSE-GCC-STM32F767IGT6-RVM/../../../M0P0_Library/STM32Cube_FW_F7_V1.11.0/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/ECLIPSE-GCC-STM32F767IGT6-RVM/../../../M0P0_Library/STM32Cube_FW_F7_V1.11.0/Drivers/CMSIS/Include" -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/ECLIPSE-GCC-STM32F767IGT6-RVM/../../../M7M2_MuAmmonite/MAmmonite/Guest/CortexM" -O3 -g3 -Wall -mcpu=cortex-m7 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mno-unaligned-access -c -fmessage-length=0 -ffreestanding  -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-strict-aliasing -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Lib/rvm_guest_cmx_asm_gcc.o: /media/pry/Code/Code_Library/MCU/Mutatus/M7M2_MuAmmonite/MAmmonite/Guest/CortexM/rvm_guest_cmx_asm_gcc.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


