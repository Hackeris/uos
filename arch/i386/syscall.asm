;
; kernel.asm
;
;  Created on: 2014-11-4
;      Author: hackeris
;
%include "const.inc"

global	i86_pit_irq
global	i86_default_handler
global	restart

extern interruptdone
extern clock_handler

extern p_proc_ready

[SECTION .text]
i86_pit_irq:
	sub		esp,4
	pushad
	push	ds
	push	es
	push	fs
	push	gs

	push 0
	call interruptdone
	add	esp,4
	call clock_handler

	pop		gs
	pop		fs
	pop		es
	pop		ds
	popad
	add		esp,4

	iretd


i86_default_handler:
	iretd

restart:
	iretd
