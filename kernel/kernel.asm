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


i86_pit_irq:
	pushad
	push 0
	call interruptdone
	add	esp,4
	call timer_handler
	popad
	iretd


i86_default_handler:
	iretd
