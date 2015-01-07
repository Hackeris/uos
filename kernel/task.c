/*
 * task.c
 *
 *  Created on: 2014Äê11ÔÂ4ÈÕ
 *      Author: hackeris
 */

#ifdef _I386
#include <../arch/i386/cpu.h>
#endif

#include <task.h>

#define	NR_TASKS	2

proc proc_table[2];

extern gdt_descriptor _gdt[];

proc* p_proc_ready;

void TestA();
void TestB();

task task_table[NR_TASKS] = { { TestA, STACK_SIZE_TESTA, "TestA" }, { TestB,
		STACK_SIZE_TESTB, "TestB" } };

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

		if (i >= 127)
			i = 0;
		kprintf("%d", i);
		i++;
		milli_delay(50);
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

int i86_ldt_initialize() {

	proc* p_proc = proc_table;
	uint16_t selector_ldt = SELECTOR_LDT_FIRST;
	int i;
	for (i = 0; i < NR_TASKS; i++) {
		gdt_set_descriptor(selector_ldt >> 3, (uint32_t) &p_proc->ldts,
				2 * sizeof(ldt_descriptor) - 1, DA_LDT & 0xff,
				(DA_LDT >> 8) & 0xf0);
		p_proc++;
		selector_ldt += (1 << 3);
	}

	return 0;
}

int init_proc() {

	task* p_task = task_table;
	proc* p_proc = proc_table;
	char* p_task_stack = task_stack + STACK_SIZE_TOTAL;
	uint16_t selector_ldt = SELECTOR_LDT_FIRST;

	uint8_t privilage;
	uint8_t rpl;
	int32_t eflags;

	int i;
	for (i = 0; i < NR_TASKS; i++) {
		p_task = task_table + i;
		privilage = PRIVILEGE_TASK;
		rpl = RPL_TASK;
		eflags = 0x1202;

		strcpy(p_proc->name, p_task->name);
		p_proc->pid = i;
		p_proc->ldt_sel = selector_ldt;
		memcpy(&p_proc->ldts[0], &_gdt[1], sizeof(ldt_descriptor));
		p_proc->ldts[0].flags = (DA_C | privilage << 5);
		memcpy(&p_proc->ldts[1], &_gdt[2], sizeof(ldt_descriptor));
		p_proc->ldts[1].flags = (DA_DRW | privilage << 5);

		p_proc->regs.cs = (8 * 0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ds = (8 * 1 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.es = (8 * 1 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.fs = (8 * 1 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.gs = (8 * 1 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ss = (8 * 1 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;

		p_proc->regs.eip = (uint32_t) p_task->initial_eip;
		p_proc->regs.esp = (uint32_t) p_task_stack;
		p_proc->regs.eflags = eflags;
		p_task_stack -= p_task->stack_size;

		p_proc++;
		p_task++;
		selector_ldt += (1 << 3);
	}

	p_proc_ready = proc_table;

	return 0;
}
