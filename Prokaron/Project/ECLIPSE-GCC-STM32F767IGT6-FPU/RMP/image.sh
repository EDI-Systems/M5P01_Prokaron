arm-none-eabi-objcopy -O binary RMP RMP.bin
arm-none-eabi-objdump -S RMP > RMP.asm
# Try to flash this in with st-flash
st-flash write RMP.bin 0x08000000
