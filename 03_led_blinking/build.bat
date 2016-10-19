@ECHO OFF

arm-none-eabi-gcc -O0 -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s -nostartfiles -g start.S main.c -o kernel.elf
arm-none-eabi-objcopy kernel.elf -O binary kernel.img
arm-none-eabi-objdump -D kernel.elf > kernel.dump

