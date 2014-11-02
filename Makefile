ENTRYPOINT=0x100000

ASM=nasm
CC=gcc
LD=ld
ASMKFLAGS= -f elf -g
CFLAGS=-c -I./boot -I./dbg -I./lib -I./include\
	-fno-builtin --no-stack-protector -g\
	-D_I386
ARCHFLAGS= -c -I./arch/i386 -I./include\
	-fno-builtin --no-stack-protector -g
LDFALGS=-g -Ttext $(ENTRYPOINT)

KERNELIMG=kernel.bin
OBJS=dbg/dbg.o boot/boot.o kernel/kernel.o\
	arch/i386/cpu.o  arch/i386/screen.o arch/i386/hal.o\
	arch/i386/klib.o\
	lib/string.o

everything: $(KERNELIMG)

all:everything

image:buildimg

clean:
	rm -rf $(OBJS)
	rm $(KERNELIMG)

buildimg:
	sudo mount -o loop a.img /mnt/floppy/
	sudo cp -fd kernel.bin /mnt/floppy/
	sleep 0.2s
	sudo umount /mnt/floppy/

kernel.bin:$(OBJS)
	$(LD) $(LDFLAGS) -o $(KERNELIMG) $(OBJS)

boot/boot.o:boot/boot.S
	$(CC) $(CFLAGS) -o $@ $<

kernel/kernel.o:kernel/kernel.c
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

arch/i386/klib.o: arch/i386/klib.asm
	$(ASM) $(ASMKFLAGS) $< -o $@


umount:
	sudo umount /mnt/floppy
run:
	qemu-system-i386 -fda a.img
debug:
	qemu-system-i386 -s -S -fda a.img
