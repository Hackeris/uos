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

int hal_initialize() {

	disable();

	initialize_cpu();
	i86_pic_initialize(0x20, 0x28);

	i86_pit_initialize();

	i86_pit_start_counter(100, I86_PIT_OCW_COUNTER0,
			I86_PIT_OCW_MODE_SQUAREWAVEGEN);

	enable();

	return 0;
}

