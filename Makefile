CC=i686-elf-gcc
LD=i686-elf-ld
AS=nasm
CFLAGS=-m32 -ffreestanding -O2 -fno-builtin -fno-stack-protector
LDFLAGS=-m elf_i386 -T linker.ld

all: iso

boot.o: boot.s
	nasm -f elf32 boot.s -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

kernel.bin: boot.o kernel.o
	$(LD) $(LDFLAGS) boot.o kernel.o -o kernel.bin

iso:
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o donutos.iso iso

clean:
	rm -rf *.o kernel.bin iso donutos.iso
