;
; kernel.asm
;
;  Created on: 2014Äê11ÔÂ4ÈÕ
;      Author: hackeris
;


global	i86_pit_irq
global	i86_default_handler

extern interruptdone
extern timer_handler

extern p_proc_ready


i86_pit_irq:
	pushad
	push	ds
	push	es
	push	fs
	push	gs

	push 0
	call interruptdone
	add	esp,4
	call timer_handler

	pop		gs
	pop		fs
	pop		es
	pop		ds
	popad
	iretd


i86_default_handler:
	iretd


restart:
	mov		esp,[p_proc_ready]


