################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
D:/Code_Library/M5P01_Prokaron/Source/Platform/RV32IMAC/rmp_platform_rv32imac_gcc.s 

C_SRCS += \
D:/Code_Library/M5P01_Prokaron/Source/Kernel/rmp_kernel.c \
D:/Code_Library/M5P01_Prokaron/Source/Platform/RV32IMAC/rmp_platform_rv32imac.c \
D:/Code_Library/M5P01_Prokaron/Source/Benchmark/rmp_test.c 

OBJS += \
./Kernel/rmp_kernel.o \
./Kernel/rmp_platform_rv32imac.o \
./Kernel/rmp_platform_rv32imac_gcc.o \
./Kernel/rmp_test.o 

S_DEPS += \
./Kernel/rmp_platform_rv32imac_gcc.d 

C_DEPS += \
./Kernel/rmp_kernel.d \
./Kernel/rmp_platform_rv32imac.d \
./Kernel/rmp_test.d 


# Each subdirectory must supply rules for building sources it contributes
Kernel/rmp_kernel.o: D:/Code_Library/M5P01_Prokaron/Source/Kernel/rmp_kernel.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g3 -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP/.." -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP/../../../Include" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\Debug" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\Core" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\User" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
Kernel/rmp_platform_rv32imac.o: D:/Code_Library/M5P01_Prokaron/Source/Platform/RV32IMAC/rmp_platform_rv32imac.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g3 -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP/.." -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP/../../../Include" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\Debug" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\Core" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\User" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
Kernel/rmp_platform_rv32imac_gcc.o: D:/Code_Library/M5P01_Prokaron/Source/Platform/RV32IMAC/rmp_platform_rv32imac_gcc.s
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g3 -x assembler-with-cpp -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
Kernel/rmp_test.o: D:/Code_Library/M5P01_Prokaron/Source/Benchmark/rmp_test.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g3 -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP/.." -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP/../../../Include" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\Debug" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\Core" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\User" -I"D:\Code_Library\M5P01_Prokaron\Project\MRS-CH32V307VCT6\RMP\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

