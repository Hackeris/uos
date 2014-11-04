/*
 * cpu.h
 *
 *  Created on: Nov 2, 2014
 *      Author: hackeris
 */

#ifndef CPU_H_
#define CPU_H_

#include <types.h>

// maximum amount of descriptors allowed
#define MAX_DESCRIPTORS					3

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
#define I86_GDT_DESC_DPL			0x0060			//01100000

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


// i86 defines 256 possible interrupt handlers (0-255)
#define I86_MAX_INTERRUPTS		256

// must be in the format 0D110, where D is descriptor type
#define I86_IDT_DESC_BIT16		0x06	//00000110
#define I86_IDT_DESC_BIT32		0x0E	//00001110
#define I86_IDT_DESC_RING1		0x40	//01000000
#define I86_IDT_DESC_RING2		0x20	//00100000
#define I86_IDT_DESC_RING3		0x60	//01100000
#define I86_IDT_DESC_PRESENT		0x80	//10000000


typedef void (*i86_irq_handler)(void);

#pragma pack(1)

typedef struct _gdt_descriptor {
	// bits 0-15 of segment limit
	uint16_t limit;
	// bits 0-23 of base address
	uint16_t baseLo;
	uint8_t baseMid;
	// descriptor access flags
	uint8_t flags;
	uint8_t grand;
	// bits 24-32 of base address
	uint8_t baseHi;
} gdt_descriptor;

typedef struct _gdtr {
	// size of gdt
	unsigned short m_limit;
	// base address of gdt
	unsigned int m_base;
} gdtr;

typedef struct _ldt_descriptor {
	// bits 0-15 of segment limit
	uint16_t limit;
	// bits 0-23 of base address
	uint16_t baseLo;
	uint8_t baseMid;
	// descriptor access flags
	uint8_t flags;
	uint8_t grand;
	// bits 24-32 of base address
	uint8_t baseHi;
} ldt_descriptor;

// interrupt descriptor
typedef struct _idt_descriptor {
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

typedef struct _idtr {
	// size of gdt
	unsigned short limit;
	// base address of gdt
	unsigned int base;
} idtr;

#pragma pack(1)

//	stack frame	of process
typedef struct stackframe {

	uint32_t gs;
	uint32_t fs;
	uint32_t es;
	uint32_t ds;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t kernel_esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t retaddr;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp;
	uint32_t ss;
} stack_frame;

//	process
typedef struct _proc {

	stack_frame regs;
	uint16_t ldt_sel;
	ldt_descriptor ldts[2];
	uint32_t pid;
	char name[16];
} proc;

void gdt_set_descriptor(unsigned int i, unsigned int base, unsigned int limit,
		unsigned char access, unsigned char grand);

int i86_gdt_initialize();

// returns interrupt descriptor
extern idt_descriptor* i86_get_ir(uint32_t i);

// installs interrupt handler. When INT is fired, it will call this callback
extern int i86_install_ir(uint32_t i, uint16_t flags, uint16_t sel,
		i86_irq_handler);

// initialize basic idt
extern int i86_idt_initialize(uint16_t codeSel);

int initialize_cpu();

#endif /* CPU_H_ */
