################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/MProkaron/Benchmark/rmp_test.c 

OBJS += \
./Benchmark/rmp_test.o 

C_DEPS += \
./Benchmark/rmp_test.d 


# Each subdirectory must supply rules for building sources it contributes
Benchmark/rmp_test.o: /media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/MProkaron/Benchmark/rmp_test.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv64-unknown-elf-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fno-move-loop-invariants -Wunused -Wuninitialized -Wall -Wmissing-declarations -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wno-sign-conversion -Wno-implicit-fallthrough  -g3 -DDEBUG -DTRACE -DOS_USE_TRACE_UART0 -DSIFIVE_FE310 -DSIFIVE_HIFIVE1_BOARD -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/FreedomStudio-FE310-G000/../../MProkaron/Include" -I/media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/include -I/media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/drivers -I/media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/env/freedom-e300-hifive1 -I/media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/env -Wbad-function-cast -Wno-missing-declarations -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


