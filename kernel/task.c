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

void TestA();
void TestB();


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

	return 0;
}

int init_proc() {

	return 0;
}
