[section .text]

global	gdt_install
global	enable
global	outportb
global	inportb

gdt_install:
	lgdt	[esp+4]
	ret

enable:
	sti
	ret

disable:
	cli
	ret

outportb:
	mov	al, byte[esp+8]	;value
	mov	dx, word[esp+4]	;port
	out	dx, al
	ret

inportb:
	xor	eax,eax
	mov	dx, word[esp+4]	;port
	in	al, dx
	ret


