################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/MProkaron/Kernel/kernel.c 

OBJS += \
./Kernel/kernel.o 

C_DEPS += \
./Kernel/kernel.d 


# Each subdirectory must supply rules for building sources it contributes
Kernel/kernel.o: /media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/MProkaron/Kernel/kernel.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/ECLIPSE-GCC-LINUX/../../MProkaron/Include" -O3 -g3 -m32 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


