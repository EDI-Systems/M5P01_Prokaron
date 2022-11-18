################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/drivers/fe300prci/fe300prci_driver.c \
/media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/drivers/plic/plic_driver.c \
/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/MProkaron/Platform/RV32IMAC/rmp_platform_rv32imac.c 

S_UPPER_SRCS += \
/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/MProkaron/Platform/RV32IMAC/rmp_platform_rv32imac_asm.S 

OBJS += \
./Platform/fe300prci_driver.o \
./Platform/plic_driver.o \
./Platform/rmp_platform_rv32imac.o \
./Platform/rmp_platform_rv32imac_asm.o 

S_UPPER_DEPS += \
./Platform/rmp_platform_rv32imac_asm.d 

C_DEPS += \
./Platform/fe300prci_driver.d \
./Platform/plic_driver.d \
./Platform/rmp_platform_rv32imac.d 


# Each subdirectory must supply rules for building sources it contributes
Platform/fe300prci_driver.o: /media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/drivers/fe300prci/fe300prci_driver.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv64-unknown-elf-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fno-move-loop-invariants -Wunused -Wuninitialized -Wall -Wmissing-declarations -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wno-sign-conversion -Wno-implicit-fallthrough  -g3 -DDEBUG -DTRACE -DOS_USE_TRACE_UART0 -DSIFIVE_FE310 -DSIFIVE_HIFIVE1_BOARD -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/FreedomStudio-FE310-G000/../../MProkaron/Include" -I/media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/include -I/media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/drivers -I/media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/env/freedom-e300-hifive1 -I/media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/env -Wbad-function-cast -Wno-missing-declarations -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Platform/plic_driver.o: /media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/drivers/plic/plic_driver.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv64-unknown-elf-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fno-move-loop-invariants -Wunused -Wuninitialized -Wall -Wmissing-declarations -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wno-sign-conversion -Wno-implicit-fallthrough  -g3 -DDEBUG -DTRACE -DOS_USE_TRACE_UART0 -DSIFIVE_FE310 -DSIFIVE_HIFIVE1_BOARD -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/FreedomStudio-FE310-G000/../../MProkaron/Include" -I/media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/include -I/media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/drivers -I/media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/env/freedom-e300-hifive1 -I/media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/env -Wbad-function-cast -Wno-missing-declarations -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Platform/rmp_platform_rv32imac.o: /media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/MProkaron/Platform/RV32IMAC/rmp_platform_rv32imac.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv64-unknown-elf-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fno-move-loop-invariants -Wunused -Wuninitialized -Wall -Wmissing-declarations -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wno-sign-conversion -Wno-implicit-fallthrough  -g3 -DDEBUG -DTRACE -DOS_USE_TRACE_UART0 -DSIFIVE_FE310 -DSIFIVE_HIFIVE1_BOARD -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/FreedomStudio-FE310-G000/../../MProkaron/Include" -I/media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/include -I/media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/drivers -I/media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/env/freedom-e300-hifive1 -I/media/pry/Code/Code_Library/MCU/Mutatus/M0P0_Library/FE310-G000/env -Wbad-function-cast -Wno-missing-declarations -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Platform/rmp_platform_rv32imac_asm.o: /media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/MProkaron/Platform/RV32IMAC/rmp_platform_rv32imac_asm.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross Assembler'
	riscv64-unknown-elf-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fno-move-loop-invariants -Wunused -Wuninitialized -Wall -Wmissing-declarations -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wno-sign-conversion -Wno-implicit-fallthrough  -g3 -x assembler-with-cpp -I"/media/pry/Code/Code_Library/MCU/Mutatus/M5P1_MuProkaron/Project/FreedomStudio-FE310-G000/../../MProkaron/Include" -Wa,-adhlns="$@.lst" -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


