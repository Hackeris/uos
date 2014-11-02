/*
 * hal.c
 *
 *  Created on: Nov 2, 2014
 *      Author: hackeris
 */

#ifdef _I386
#include <cpu.h>
#endif

int hal_initialize() {

	initialize_cpu();

	return 0;
}

int initialize_cpu() {

	i86_gdt_initialize();
	i86_idt_initialize(0x8);

	return 0;
}


