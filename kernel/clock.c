/*
 * clock.c
 *
 *  Created on: 2014-11-4
 *      Author: hackeris
 */

#include <dbg.h>
#include <types.h>
#include <task.h>

#ifdef _I386
#include <../arch/i386/cpu.h>
#include <../arch/i386/klib.h>
#endif


void dump_memory(void* ptr, int size) {
	int i;
	uint32_t* p = (uint32_t*) ptr;
	for (i = 0; i < size / 4; i++) {
		kprintf("%x ", p[i]);
	}
}

void clock_handler() {

	//	to do: add scheduler here
	kprintf("T");
}
