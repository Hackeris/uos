/*
 * klib.h
 *
 *  Created on: Nov 2, 2014
 *      Author: hackeris
 */

#ifndef KLIB_H_
#define KLIB_H_

extern void gdt_install(void*);
extern void enable();
extern void disable();
void outportb (unsigned short portid, unsigned char value);
unsigned char inportb (unsigned short portid);

#endif /* KLIB_H_ */
