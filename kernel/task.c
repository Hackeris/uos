/*
 * task.c
 *
 *  Created on: 2014-11-4
 *      Author: hackeris
 */

#ifdef _I386
#include <../arch/i386/cpu.h>
#include <../arch/i386/klib.h>
#endif

#include <task.h>
#include <string.h>

#define	NR_TASKS	2

process proc_table[NR_TASKS];

process* p_proc_ready;

void TestA();
void TestB();

extern gdt_descriptor _gdt[];

/*
 task task_table[NR_TASKS] = { { TestA, STACK_SIZE_TESTA, "TestA" }, { TestB,
 STACK_SIZE_TESTB, "TestB" } };
 */

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

int init_tasks() {

	return 0;
}

int init_proc() {

	process * p_proc = proc_table;

	p_proc->ldt_sel = SELECTOR_FIRST_LDT;
	memcpy(&p_proc->ldts, &_gdt[1], sizeof(gdt_descriptor));
	ldt_set_descriptor(&p_proc->ldts[0], 0, 0xffffffff,
			I86_GDT_DESC_READWRITE | I86_GDT_DESC_EXEC_CODE
					| I86_GDT_DESC_CODEDATA | I86_GDT_DESC_MEMORY,
			I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT
					| I86_GDT_GRAND_LIMITHI_MASK);
	ldt_set_descriptor(&p_proc->ldts[1], 0, 0xffffffff,
			I86_GDT_DESC_READWRITE | I86_GDT_DESC_CODEDATA
					| I86_GDT_DESC_MEMORY,
			I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT
					| I86_GDT_GRAND_LIMITHI_MASK);


	return 0;
}
