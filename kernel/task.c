/*
 * task.c
 *
 *  Created on: 2014-11-4
 *      Author: hackeris
 */
#define _I386

#ifdef _I386
#include <../arch/i386/cpu.h>
#include <../arch/i386/klib.h>
#endif

#include <task.h>
#include <string.h>

process proc_table[NR_TASKS];

process* p_proc_ready;
//process* proc_ready;

void TestA();
void TestB();

extern gdt_descriptor _gdt[];
extern tss _tss;

void milli_delay(unsigned int t) {
	int i;
	for (i = 0; i < t; i++) {
		int j;
		for (j = 0; j < 100000; j++)
			;
	}
}

void TestA() {
	char i = 0;
	while (1) {
		__asm("movl $0xcccccccc,%eax");
		__asm("movl $0xcdcd,%ebx");
	}
}

void TestB() {
	char i = 0;
	while (1) {

		if (i >= 127)
			i = 0;
		kprintf("%d", i);
		i++;
		milli_delay(100);
	}
}

char task_stack[STACK_SIZE_TOTAL];

int init_tasks() {

	//	for tss
	memset(&_tss, 0, sizeof(tss));
	_tss.ss0 = SELECTOR_KERNEL_DS;
	_tss.iobase = sizeof(tss);
	gdt_set_descriptor(SELECTOR_TSS >> 3, (uint32_t) &_tss, sizeof(tss) - 1,
			I86_GDT_DESC_EXEC_CODE | I86_GDT_DESC_ACCESS | I86_GDT_DESC_MEMORY,
			I86_GDT_GRAND_LIMITHI_MASK);
	//	for ldt
	gdt_set_descriptor(SELECTOR_FIRST_LDT >> 3, (uint32_t) &proc_table[0].ldts,
			2 * sizeof(ldt_descriptor) - 1,
			I86_GDT_DESC_READWRITE | I86_GDT_DESC_MEMORY,
			I86_GDT_GRAND_LIMITHI_MASK);
	gdt_set_descriptor((SELECTOR_FIRST_LDT + 8) >> 3,
			(uint32_t) &proc_table[1].ldts, 2 * sizeof(ldt_descriptor) - 1,
			I86_GDT_DESC_READWRITE | I86_GDT_DESC_MEMORY,
			I86_GDT_GRAND_LIMITHI_MASK);

	init_proc();

	tss_install(SELECTOR_TSS);

	_tss.esp0 = &p_proc_ready->regs;

	return 0;
}

int init_proc() {

	process * p_proc = proc_table;
	p_proc_ready = p_proc;
	//proc_ready = p_proc_ready;

	kprintf("p_proc_ready: 0x%x\n", p_proc_ready);

	p_proc->ldt_sel = SELECTOR_FIRST_LDT;
	memcpy(&p_proc->ldts, &_gdt[1], sizeof(gdt_descriptor));
	ldt_set_descriptor(&p_proc->ldts[0], 0, 0xffffffff,
			I86_GDT_DESC_READWRITE | I86_GDT_DESC_EXEC_CODE
					| I86_GDT_DESC_CODEDATA | I86_GDT_DESC_MEMORY,
			I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);
	ldt_set_descriptor(&p_proc->ldts[1], 0, 0xffffffff,
			I86_GDT_DESC_READWRITE | I86_GDT_DESC_CODEDATA | I86_GDT_DESC_MEMORY,
			I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);
	p_proc->regs.cs = 0 | SELECTOR_LDT_MASK;
	p_proc->regs.ds = 8 | SELECTOR_LDT_MASK;
	p_proc->regs.es = 8 | SELECTOR_LDT_MASK;
	p_proc->regs.fs = 8 | SELECTOR_LDT_MASK;
	p_proc->regs.gs = 8 | SELECTOR_LDT_MASK;
	p_proc->regs.eip = (uint32_t) TestA;
	p_proc->regs.esp = (uint32_t) task_stack + STACK_SIZE_TOTAL;
	p_proc->regs.eflags = 0x1202; //	IF=1, IOPL=1, bit 2 is always 1

	p_proc++;

	p_proc->ldt_sel = SELECTOR_FIRST_LDT + 8;
	memcpy(&p_proc->ldts, &_gdt[1], sizeof(gdt_descriptor));
	ldt_set_descriptor(&p_proc->ldts[0], 0, 0xffffffff,
			I86_GDT_DESC_READWRITE | I86_GDT_DESC_EXEC_CODE
					| I86_GDT_DESC_CODEDATA | I86_GDT_DESC_MEMORY,
			I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);
	ldt_set_descriptor(&p_proc->ldts[1], 0, 0xffffffff,
			I86_GDT_DESC_READWRITE | I86_GDT_DESC_CODEDATA | I86_GDT_DESC_MEMORY,
			I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);
	p_proc->regs.cs = 0 | SELECTOR_LDT_MASK;
	p_proc->regs.ds = 8 | SELECTOR_LDT_MASK;
	p_proc->regs.es = 8 | SELECTOR_LDT_MASK;
	p_proc->regs.fs = 8 | SELECTOR_LDT_MASK;
	p_proc->regs.gs = 8 | SELECTOR_LDT_MASK;
	p_proc->regs.eip = (uint32_t) TestB;
	p_proc->regs.esp = (uint32_t) task_stack + STACK_SIZE_TOTAL
			+ STACK_SIZE_TESTA;
	p_proc->regs.eflags = 0x1202; //	IF=1, IOPL=1, bit 2 is always 1

	return 0;
}
