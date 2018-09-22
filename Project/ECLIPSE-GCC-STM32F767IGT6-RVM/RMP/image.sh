arm-none-eabi-objcopy -O binary RMP RMP.bin
arm-none-eabi-objdump -S RMP > RMP.asm
../bincopy -i RMP.bin -o VM1.c
