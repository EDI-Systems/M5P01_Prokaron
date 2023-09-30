################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/core_riscv.c 

OBJS += \
./Core/core_riscv.o 

C_DEPS += \
./Core/core_riscv.d 


# Each subdirectory must supply rules for building sources it contributes
Core/%.o: ../Core/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g3 -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP/.." -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP/../../../Include" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\Debug" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\Core" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\User" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

