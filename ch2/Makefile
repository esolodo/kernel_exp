

ARCH ?= X86
CPU ?= i686
BINFMT ?= elf

CC	= $(CPU)-$(BINFMT)-gcc
CFLAGS	= -Wall -O4 -fno-omit-frame-pointer -Wextra -ffreestanding 				\
		-std=c11  -m32 -Iinclude		\
		$(EXTRA_CFLAGS)
LD	= $(CPU)-$(BINFMT)-ld

LDFLAGS = --nmagic -m elf_i386 --output=kernel.bin --script=arch/x86/linker.ld -Map kernel.map
MKDIR = mkdir -p
RM = rm -rf
CP = cp -r

OBJFILES =	multiboot.o arch/x86/head.o arch/x86/step2.o arch/x86/vgaterm.o utils.o \
			sys.o vsprintf.o





.s.o:
	nasm -f elf -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

kernel.bin: $(OBJFILES)
	$(LD) $(LDFLAGS)  $^

clean:
	rm -rf $(OBJFILES) kernel.bin isofiles  os.iso kernel.map

iso:
	mkdir -p isofiles/boot/grub
	echo "set timeout=0" > isofiles/boot/grub/grub.cfg
	echo "set default=0" >> isofiles/boot/grub/grub.cfg
	echo "menuentry "djo" {" >> isofiles/boot/grub/grub.cfg
	echo "multiboot2 /boot/kernel.bin" >> isofiles/boot/grub/grub.cfg
	echo "}" >> isofiles/boot/grub/grub.cfg

	cp kernel.bin isofiles/boot/
	grub2-mkrescue -o os.iso isofiles


