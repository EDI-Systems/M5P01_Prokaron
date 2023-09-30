################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v30x_it.c \
../User/system_ch32v30x.c 

OBJS += \
./User/ch32v30x_it.o \
./User/system_ch32v30x.o 

C_DEPS += \
./User/ch32v30x_it.d \
./User/system_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g3 -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP/.." -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP/../../../Include" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\Debug" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\Core" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\User" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

