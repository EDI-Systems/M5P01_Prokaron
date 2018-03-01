################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/MProkaron/Platform/X86_LINUX/platform_x86_linux.c 

OBJS += \
./Platform/platform_x86_linux.o 

C_DEPS += \
./Platform/platform_x86_linux.d 


# Each subdirectory must supply rules for building sources it contributes
Platform/platform_x86_linux.o: /media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/MProkaron/Platform/X86_LINUX/platform_x86_linux.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/ECLIPSE-GCC-LINUX/../../MProkaron/Include" -O3 -g3 -m32 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


