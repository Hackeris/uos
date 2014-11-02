/*
 * cpu.c
 *
 *  Created on: Nov 2, 2014
 *      Author: hackeris
 */

#include <cpu.h>
#include <klib.h>

// Global Descriptor Table (GDT)
static struct gdt_descriptor _gdt[MAX_DESCRIPTORS];

// gdtr data
static struct gdtr _gdtr;

void gdt_set_descriptor(unsigned int i, unsigned int base, unsigned int limit,
		unsigned char access, unsigned char grand) {

	if (i > MAX_DESCRIPTORS)
		return;

	// null out the descriptor
	memset((void*) &_gdt[i], 0, sizeof(struct gdt_descriptor));

	// set limit and base addresses
	_gdt[i].baseLo = base & 0xffff;
	_gdt[i].baseMid = (base >> 16) & 0xff;
	_gdt[i].baseHi = (base >> 24) & 0xff;
	_gdt[i].limit = limit & 0xffff;

	// set flags and grandularity bytes
	_gdt[i].flags = access;
	_gdt[i].grand = (limit >> 16) & 0x0f;
	_gdt[i].grand |= grand & 0xf0;
}

int i86_gdt_initialize() {

	// set up gdtr
	_gdtr.m_limit = (sizeof(struct gdt_descriptor) * MAX_DESCRIPTORS) - 1;
	_gdtr.m_base = (uint32_t) &_gdt[0];

	// set null descriptor
	gdt_set_descriptor(0, 0, 0, 0, 0);

	//! set default code descriptor
	gdt_set_descriptor(1, 0, 0xffffffff,
			I86_GDT_DESC_READWRITE | I86_GDT_DESC_EXEC_CODE
					| I86_GDT_DESC_CODEDATA | I86_GDT_DESC_MEMORY,
			I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);
	//! set default data descriptor
	gdt_set_descriptor(2, 0, 0xffffffff,
	I86_GDT_DESC_READWRITE | I86_GDT_DESC_CODEDATA | I86_GDT_DESC_MEMORY,
	I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);

	gdt_install(&_gdtr);

	return 0;
}
