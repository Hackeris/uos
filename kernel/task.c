/*
 * task.c
 *
 *  Created on: 2014Äê11ÔÂ4ÈÕ
 *      Author: hackeris
 */

#ifdef _I386
#include <../arch/i386/cpu.h>
#include <../arch/i386/klib.h>
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
