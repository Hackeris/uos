/*
 * task.h
 *
 *  Created on: Jan 7, 2015
 *      Author: hackeris
 */

#ifndef TASK_H_
#define TASK_H_

#include <types.h>

typedef void (*PTASK_FUN)();

#define SELECTOR_TSS		0x18
#define SELECTOR_FIRST_LDT	0x20


#define STACK_SIZE_TESTA	0x500
#define STACK_SIZE_TESTB	0x500

#define STACK_SIZE_TOTAL	STACK_SIZE_TESTA+STACK_SIZE_TESTB

typedef struct _task {
	PTASK_FUN initial_eip;
	uint32_t stack_size;
	char name[20];
} task;

//	in syscall.asm
void restart();

int init_proc();

int init_tasks();

#endif /* TASK_H_ */
