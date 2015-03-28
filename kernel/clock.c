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

extern process* p_proc_ready;
//extern uint32_t saved_esp;
extern tss _tss;
//extern process proc_table[];

void dump_memory(void* ptr, int size) {
	int i;
	uint32_t* p = (uint32_t*) ptr;
	for (i = 0; i < size / 4; i++) {
		kprintf("%x ", p[i]);
	}
}

void clock_handler() {

	//	to do: add scheduler here
	kprintf("0x%x ", p_proc_ready);
	dump_memory(&_tss, sizeof(_tss));
}
