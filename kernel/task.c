/*
 * task.c
 *
 *  Created on: 2014��11��4��
 *      Author: hackeris
 */

#ifdef _I386
#include <../arch/i386/cpu.h>
#endif

#include <task.h>

#define	NR_TASKS	2

proc proc_table[2];

task task_table[NR_TASKS];
