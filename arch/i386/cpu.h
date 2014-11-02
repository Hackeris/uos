/*
 * cpu.h
 *
 *  Created on: Nov 2, 2014
 *      Author: hackeris
 */

#ifndef CPU_H_
#define CPU_H_

#include <klib.h>
#include <types.h>

#define MAX_DESCRIPTORS					6
/***	 gdt descriptor access bit flags.	***/
// set access bit
#define I86_GDT_DESC_ACCESS			0x0001			//00000001

// descriptor is readable and writable. default: read only
#define I86_GDT_DESC_READWRITE			0x0002			//00000010

// set expansion direction bit
#define I86_GDT_DESC_EXPANSION			0x0004			//00000100

// executable code segment. Default: data segment
#define I86_GDT_DESC_EXEC_CODE			0x0008			//00001000

// set code or data descriptor. defult: system defined descriptor
#define I86_GDT_DESC_CODEDATA			0x0010			//00010000

// set dpl bits
#define I86_GDT_DESC_DPL0			0x0000			//00000000
#define I86_GDT_DESC_DPL1			0x0020			//00100000
#define I86_GDT_DESC_DPL2			0x0040			//01000000
#define I86_GDT_DESC_DPL3			0x0060			//01100000

// set "in memory" bit
#define I86_GDT_DESC_MEMORY			0x0080			//10000000

/**	gdt descriptor grandularity bit flags	***/

// masks out limitHi (High 4 bits of limit)
#define I86_GDT_GRAND_LIMITHI_MASK		0x0f			//00001111

// set os defined bit
#define I86_GDT_GRAND_OS			0x10			//00010000

// set if 32bit. default: 16 bit
#define I86_GDT_GRAND_32BIT			0x40			//01000000

// 4k grandularity. default: none
#define I86_GDT_GRAND_4K			0x80			//10000000

typedef struct _tag_gdtr {

	uint16_t m_limit;

	uint32_t m_base;

} gdtr;

typedef struct _tag_gdt_descriptor {

	uint16_t limit;

	uint16_t baseLo;
	uint8_t baseMid;

	uint8_t flags;

	uint8_t grand;

	uint8_t baseHi;

} gdt_descriptor, DESCRIPTOR;

extern void gdt_set_descriptor(uint32_t i, uint64_t base, uint64_t limit,
		uint8_t access, uint8_t grand);

extern gdt_descriptor* i86_gdt_get_descriptor(int i);

extern int i86_gdt_initialize();

#define I86_MAX_INTERRUPTS		256

#define I86_IDT_DESC_BIT16		0x06	//00000110
#define I86_IDT_DESC_BIT32		0x0E	//00001110
#define I86_IDT_DESC_RING1		0x40	//01000000
#define I86_IDT_DESC_RING2		0x20	//00100000
#define I86_IDT_DESC_RING3		0x60	//01100000
#define I86_IDT_DESC_PRESENT		0x80	//10000000

typedef void (*I86_IRQ_HANDLER)(void);

typedef struct _tag_idtr {

	uint16_t limit;

	uint32_t base;
} idtr;

//	IDT descriptor
typedef struct _tag_idt_descriptor {
	// bits 0-16 of interrupt routine (ir) address
	uint16_t baseLo;
	// code selector in gdt
	uint16_t sel;
	// reserved, shold be 0
	uint8_t reserved;
	// bit flags. Set with flags above
	uint8_t flags;
	// bits 16-32 of ir address
	uint16_t baseHi;
} idt_descriptor;

typedef void (*I86_IRQ_HANDLER)(void);

extern idt_descriptor* i86_get_ir(uint32_t i);

extern int i86_install_ir(uint32_t i, uint16_t flags, uint16_t sel,
		I86_IRQ_HANDLER);

extern int i86_idt_initialize(uint16_t codeSel);

void load_default_irq_handler();

#endif /* CPU_H_ */
