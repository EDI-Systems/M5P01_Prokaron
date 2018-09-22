################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/MProkaron/Kernel/rmp_kernel.c 

OBJS += \
./Kernel/rmp_kernel.o 

C_DEPS += \
./Kernel/rmp_kernel.d 


# Each subdirectory must supply rules for building sources it contributes
Kernel/rmp_kernel.o: /media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/MProkaron/Kernel/rmp_kernel.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DSTM32F767xx -DUSE_HAL_DRIVER -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/ECLIPSE-GCC-STM32F767IGT6/../../MProkaron/Include" -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/ECLIPSE-GCC-STM32F767IGT6/../../../M0P0_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Inc" -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/ECLIPSE-GCC-STM32F767IGT6/../../../M0P0_Library/STM32Cube_FW_F7_V1.11.0/Drivers/STM32F7xx_HAL_Driver/Inc/Conf" -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/ECLIPSE-GCC-STM32F767IGT6/../../../M0P0_Library/STM32Cube_FW_F7_V1.11.0/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/ECLIPSE-GCC-STM32F767IGT6/../../../M0P0_Library/STM32Cube_FW_F7_V1.11.0/Drivers/CMSIS/Include" -O3 -g3 -Wall -mcpu=cortex-m7 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mno-unaligned-access -c -fmessage-length=0 -ffreestanding  -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-strict-aliasing -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


