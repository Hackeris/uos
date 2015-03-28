/*
 * cpu.c
 *
 *  Created on: Nov 2, 2014
 *      Author: hackeris
 */

#include <klib.h>
#include <cpu.h>
#include <string.h>
#include <exception.h>

// Global Descriptor Table (GDT)
gdt_descriptor _gdt[MAX_DESCRIPTORS];
// gdtr data
static gdtr _gdtr;

tss _tss;

//	convert segment to physical address
uint32_t seg2phys(uint16_t seg) {

	gdt_descriptor* pdesc = &_gdt[seg >> 3];
	return (pdesc->baseHi << 24 | pdesc->baseMid << 16 | pdesc->baseLo);
}

void gdt_set_descriptor(unsigned int i, unsigned int base, unsigned int limit,
		unsigned char access, unsigned char grand) {

	if (i > MAX_DESCRIPTORS)
		return;

	// null out the descriptor
	memset((void*) &_gdt[i], 0, sizeof(gdt_descriptor));

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

void ldt_set_descriptor(ldt_descriptor* desc, unsigned int base,
		unsigned int limit, unsigned char access, unsigned char grand) {
	// null out the descriptor
	memset((void*) desc, 0, sizeof(ldt_descriptor));

	// set limit and base addresses
	desc->limit = limit & 0xffff;

	desc->baseLo = base & 0xffff;
	desc->baseMid = (base >> 16) & 0xff;

	// set flags and grandularity bytes
	desc->flags = access;
	desc->grand = (limit >> 16) & 0x0f;
	desc->grand |= grand & 0xf0;

	desc->baseHi = (base >> 24) & 0xff;
}

int i86_gdt_initialize() {

	// set up gdtr
	_gdtr.m_limit = (sizeof(gdt_descriptor) * MAX_DESCRIPTORS) - 1;
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

// interrupt descriptor table
static idt_descriptor _idt[I86_MAX_INTERRUPTS];

// idtr structure used to help define the cpu's idtr register
static idtr _idtr;

// returns interrupt descriptor
idt_descriptor* i86_get_ir(uint32_t i) {

	if (i > I86_MAX_INTERRUPTS)
		return 0;

	return &_idt[i];
}

// installs a new interrupt handler
int i86_install_ir(uint32_t i, uint16_t flags, uint16_t sel,
		i86_irq_handler irq) {

	if (i > I86_MAX_INTERRUPTS)
		return 0;

	if (!irq)
		return 0;

	// get base address of interrupt handler
	uint32_t uiBase = (uint32_t) irq;

	// store base address into idt
	_idt[i].baseLo = (uint16_t) (uiBase & 0xffff);
	_idt[i].baseHi = (uint16_t) ((uiBase >> 16) & 0xffff);
	_idt[i].reserved = 0;
	_idt[i].flags = (uint8_t) (flags);
	_idt[i].sel = sel;

	return 0;
}

void i86_default_handler();

// initialize idt
int i86_idt_initialize(uint16_t codeSel) {

	// set up idtr for processor
	_idtr.limit = sizeof(idt_descriptor) * I86_MAX_INTERRUPTS - 1;
	_idtr.base = (uint32_t) &_idt[0];

	// null out the idt
	memset((void*) &_idt[0], 0,
			sizeof(idt_descriptor) * I86_MAX_INTERRUPTS - 1);

	// register default handlers
	int i;
	for (i = 0; i < I86_MAX_INTERRUPTS; i++)
		i86_install_ir(i, I86_IDT_DESC_PRESENT | I86_IDT_DESC_BIT32, codeSel,
				(i86_irq_handler) i86_default_handler);

	// install our idt
	idt_install(&_idtr);

	return 0;
}

int initialize_cpu() {

	i86_gdt_initialize();
	i86_idt_initialize(0x8);
	install_def_irq();

	return 0;
}
