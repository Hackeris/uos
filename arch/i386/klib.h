/*
 * klib.h
 *
 *  Created on: Nov 2, 2014
 *      Author: hackeris
 */

#ifndef KLIB_H_
#define KLIB_H_

void gdt_install(void*);

void idt_install(void*);

void tss_install(int);

void enable();

void disable();

void outportb(unsigned short portid, unsigned char value);

unsigned char inportb(unsigned short portid);

void geninterrupt(unsigned int ir);


#endif /* KLIB_H_ */
