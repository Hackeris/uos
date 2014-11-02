/*
 * cpu.c
 *
 *  Created on: Nov 2, 2014
 *      Author: hackeris
 */

#include <cpu.h>
#include <klib.h>

static gdtr _gdtr;
//! global descriptor table is an array of descriptors
static gdt_descriptor _gdt[MAX_DESCRIPTORS];

//! returns descriptor in gdt
gdt_descriptor* i86_gdt_get_descriptor(int i) {

	if (i > MAX_DESCRIPTORS)
		return 0;

	return &_gdt[i];
}

//! Setup a descriptor in the Global Descriptor Table
void gdt_set_descriptor(uint32_t i, uint64_t base, uint64_t limit,
		uint8_t access, uint8_t grand) {
	if (i > MAX_DESCRIPTORS)
		return;

	//! null out the descriptor
	memset((void*) &_gdt[i], 0, sizeof(gdt_descriptor));

	//! set limit and base addresses
	_gdt[i].baseLo = (uint16_t) (base & 0xffff);
	_gdt[i].baseMid = (uint8_t) ((base >> 16) & 0xff);
	_gdt[i].baseHi = (uint8_t) ((base >> 24) & 0xff);
	_gdt[i].limit = (uint16_t) (limit & 0xffff);

	//! set flags and grandularity bytes
	_gdt[i].flags = access;
	_gdt[i].grand = (uint8_t) ((limit >> 16) & 0x0f);
	_gdt[i].grand |= grand & 0xf0;
}

//! initialize gdt
int i86_gdt_initialize() {

	//! set up gdtr
	_gdtr.m_limit = (sizeof(gdt_descriptor) * MAX_DESCRIPTORS) - 1;
	_gdtr.m_base = (uint32_t) &_gdt[0];

	//! set null descriptor
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

	gdt_set_descriptor(3, 0, 0xffffffff,
			I86_GDT_DESC_READWRITE | I86_GDT_DESC_EXEC_CODE
					| I86_GDT_DESC_CODEDATA | I86_GDT_DESC_MEMORY
					| I86_GDT_DESC_DPL3,
			I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);

	gdt_set_descriptor(4, 0, 0xffffffff,
			I86_GDT_DESC_READWRITE | I86_GDT_DESC_CODEDATA | I86_GDT_DESC_MEMORY
					| I86_GDT_DESC_DPL3,
			I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);

	//! install gdtr
	gdt_install(&_gdtr);

	return 0;
}

//! interrupt descriptor table
static idt_descriptor _idt[I86_MAX_INTERRUPTS];

//! idtr structure used to help define the cpu's idtr register
static idtr _idtr;

//! default int handler used to catch unregistered interrupts
void i86_default_handler() {
	kprintf("interrupt\n");
}

//! returns interrupt descriptor
idt_descriptor* i86_get_ir(uint32_t i) {

	if (i > I86_MAX_INTERRUPTS)
		return 0;

	return &_idt[i];
}

//! installs a new interrupt handler
int i86_install_ir(uint32_t i, uint16_t flags, uint16_t sel,
		I86_IRQ_HANDLER irq) {

	uint64_t uiBase = (uint64_t) &(*irq);
	if (i > I86_MAX_INTERRUPTS)
		return 0;

	if (!irq)
		return 0;

	//! get base address of interrupt handler

	//! store base address into idt
	_idt[i].baseLo = (uint16_t) (uiBase & 0xffff);
	_idt[i].baseHi = (uint16_t) ((uiBase >> 16) & 0xffff);
	_idt[i].reserved = 0;
	_idt[i].flags = (uint8_t) (flags);
	_idt[i].sel = sel;

	return 0;
}

//! initialize idt
int i86_idt_initialize(uint16_t codeSel) {

	int i;
	//! set up idtr for processor
	_idtr.limit = sizeof(idt_descriptor) * I86_MAX_INTERRUPTS - 1;
	_idtr.base = (uint32_t) &_idt[0];

	//! null out the idt
	memset((void*) &_idt[0], 0,
			sizeof(idt_descriptor) * I86_MAX_INTERRUPTS - 1);

	//! register default handlers
	for (i = 0; i < I86_MAX_INTERRUPTS; i++)
		i86_install_ir(i, I86_IDT_DESC_PRESENT | I86_IDT_DESC_BIT32, codeSel,
				(I86_IRQ_HANDLER) i86_default_handler);

	//! install our idt
	idt_install(&_idtr);

	return 0;
}

void load_default_irq_handler() {

	disable();
	/*	setvect(0, (void (*)(void)) divide_by_zero_fault);
	 setvect(1, (void (*)(void)) single_step_trap);
	 setvect(2, (void (*)(void)) nmi_trap);
	 setvect(3, (void (*)(void)) breakpoint_trap);
	 setvect(4, (void (*)(void)) overflow_trap);
	 setvect(5, (void (*)(void)) bounds_check_fault);
	 setvect(6, (void (*)(void)) invalid_opcode_fault);
	 setvect(7, (void (*)(void)) no_device_fault);
	 setvect(8, (void (*)(void)) double_fault_abort);
	 setvect(10, (void (*)(void)) invalid_tss_fault);
	 setvect(11, (void (*)(void)) no_segment_fault);
	 setvect(12, (void (*)(void)) stack_fault);
	 setvect(13, (void (*)(void)) general_protection_fault);
	 setvect(14, (void (*)(void)) page_fault);
	 setvect(16, (void (*)(void)) fpu_fault);
	 setvect(17, (void (*)(void)) alignment_check_fault);
	 setvect(18, (void (*)(void)) machine_check_abort);
	 setvect(19, (void (*)(void)) simd_fpu_fault);*/
}
