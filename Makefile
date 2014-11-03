ENTRYPOINT=0x100000

ASM=nasm
CC=i586-elf-gcc
LD=i586-elf-ld
ASMKFLAGS= -f elf -g
CFLAGS=-c -elf -I./boot -I./dbg -I./lib -I./include\
	-fno-builtin --no-stack-protector -g\
	-D_I386
ARCHFLAGS= -c -elf -I./arch/i386 -I./include\
	-fno-builtin --no-stack-protector -g
LDFLAGS=-g -Ttext $(ENTRYPOINT)

KERNELIMG=kernel.bin
OBJS=dbg/dbg.o boot/boot.o kernel/main.o\
	arch/i386/screen.o arch/i386/hal.o arch/i386/cpu.o\
	arch/i386/klib.o arch/i386/pic.o arch/i386/pit.o\
	lib/string.o\
	kernel/kernel.o

everything: $(KERNELIMG)

all:everything

image:buildimg

clean:
	rm -rf $(OBJS)
	rm $(KERNELIMG)

buildimg:
	mcopy -o -i a.img kernel.bin ::

kernel.bin:$(OBJS)
	$(LD) $(LDFLAGS) -o $(KERNELIMG) $(OBJS)

boot/boot.o:boot/boot.S
	$(CC) $(CFLAGS) -o $@ $<

kernel/kernel.o: kernel/kernel.asm
	$(ASM) $(ASMKFLAGS) $< -o $@

kernel/main.o:kernel/main.c
	$(CC) $(CFLAGS) -o $@ $<
	
dbg/dbg.o: dbg/dbg.c dbg/dbg.h
	$(CC) $(CFLAGS)	-o $@ $<
	
lib/string.o: lib/string.c
	$(CC) $(CFLAGS)	-o $@ $<	

arch/i386/screen.o: arch/i386/screen.c arch/i386/screen.h
	$(CC) $(ARCHFLAGS)	-o $@ $<

arch/i386/cpu.o: arch/i386/cpu.c arch/i386/cpu.h
	$(CC) $(ARCHFLAGS)	-o $@ $<
	
arch/i386/hal.o: arch/i386/hal.c arch/i386/hal.h
	$(CC) $(ARCHFLAGS)	-o $@ $<

arch/i386/pic.o: arch/i386/pic.c arch/i386/pic.h
	$(CC) $(ARCHFLAGS)	-o $@ $<
	
arch/i386/pit.o: arch/i386/pit.c arch/i386/pit.h
	$(CC) $(ARCHFLAGS)	-o $@ $<

arch/i386/klib.o: arch/i386/klib.asm
	$(ASM) $(ASMKFLAGS) $< -o $@


umount:
	sudo umount /mnt/floppy
run:
	qemu-system-i386 -fda a.img
debug:
	qemu-system-i386 -s -S -fda a.img
