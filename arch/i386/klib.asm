[section .text]

global	gdt_install
global	tss_install
global	idt_install
global	ldt_install
global	enable
global	disable
global	outportb
global	inportb
global	geninterrupt


gdt_install:
	mov		eax,[esp+4]
	lgdt	[eax]
	ret

idt_install:
	mov		eax,[esp+4]
	lidt	[eax]
	ret

tss_install:
	mov		eax,[esp+4]
	ltr		ax
	ret

ldt_install:
	mov		eax,[esp+4]
	lldt	ax
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

geninterrupt:
	mov	al,byte[esp+4]
	mov byte[.genint+1],al
	jmp	.genint
.genint:
	int 0
	ret


