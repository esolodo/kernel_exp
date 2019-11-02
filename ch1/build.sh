#!/bin/bash
nasm -f elf64  multiboot.asm
nasm -f elf64  boot.asm
ld --nmagic --output=kernel.bin --script=linker.ld multiboot.o boot.o
mkdir -p isofiles/boot/grub

cat << EOF > isofiles/boot/grub/grub.cfg
set timeout=0
set default=0

menuentry "MyHelloWorld" {
    multiboot2 /boot/kernel.bin
    boot
}
EOF

cp kernel.bin isofiles/boot/
grub2-mkrescue -o os.iso isofiles
