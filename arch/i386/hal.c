/*
 * hal.c
 *
 *  Created on: Nov 2, 2014
 *      Author: hackeris
 */

#include <cpu.h>

int hal_initialize() {

	initialize_cpu();

	return 0;
}

int initialize_cpu() {

	i86_gdt_initialize();

	return 0;
}

