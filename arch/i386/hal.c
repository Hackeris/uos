/*
 * hal.c
 *
 *  Created on: Nov 2, 2014
 *      Author: hackeris
 */

#include <cpu.h>
#include <pic.h>
#include <pit.h>
#include <klib.h>

void setvect(int intno, void (*vect)(), int flags) {

	// install interrupt handler! This overwrites prev interrupt descriptor
	i86_install_ir(intno, I86_IDT_DESC_PRESENT | I86_IDT_DESC_BIT32 | flags,
			0x8, vect);
}

void interruptdone(unsigned int intno) {

	// insure its a valid hardware irq
	if (intno > 16)
		return;

	// check if we need to send end-of-interrupt to second pic
	if (intno >= 8)
		i86_pic_send_command(I86_PIC_OCW2_MASK_EOI, 1);

	// always send end-of-interrupt to primary pic
	i86_pic_send_command(I86_PIC_OCW2_MASK_EOI, 0);
}

int hal_initialize() {


	initialize_cpu();
	i86_pic_initialize(0x20, 0x28);
	i86_pit_initialize();

	return 0;
}

