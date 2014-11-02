/*
 * hal.c
 *
 *  Created on: Nov 2, 2014
 *      Author: hackeris
 */

#include <cpu.h>
#include <pic.h>

int hal_initialize() {

	initialize_cpu();
	i86_pic_initialize(0x20, 0x28);

	return 0;
}

int initialize_cpu() {

	i86_gdt_initialize();
	i86_idt_initialize(0x8);

	return 0;
}

