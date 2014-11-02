/*
 * hal.c
 *
 *  Created on: Nov 2, 2014
 *      Author: hackeris
 */

#ifdef _I386
#include <cpu.h>
#endif

int initalize_cpu() {

	i86_gdt_initialize();

	return 0;
}
