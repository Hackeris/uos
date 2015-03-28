/*
 * exception.c
 *
 *  Created on: Mar 28, 2015
 *      Author: hackeris
 */

#include <dbg.h>
#include <exception.h>
#include <cpu.h>

#define kernel_panic kprintf

//! divide by 0 fault
void _cdecl divide_by_zero_fault(uint32_t eip, uint32_t cs, uint32_t flags) {

	intstart();
	kernel_panic("Divide by 0 at physical address [0x%x:0x%x] EFLAGS [0x%x]",
			cs, eip, flags);
	for (;;)
		;
}

//! single step
void _cdecl single_step_trap(uint32_t cs, uint32_t eip, uint32_t flags) {

	intstart();
	kernel_panic("Single step at physical address [0x%x:0x%x] EFLAGS [0x%x]",
			cs, eip, flags);
	for (;;)
		;
}

//! non maskable interrupt trap
void _cdecl nmi_trap(uint32_t cs, uint32_t eip, uint32_t flags) {

	intstart();
	kernel_panic("NMI trap at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs,
			eip, flags);
	for (;;)
		;
}

//! breakpoint hit
void _cdecl breakpoint_trap(uint32_t cs, uint32_t eip, uint32_t flags) {

	intstart();
	kernel_panic(
			"Breakpoint trap at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs,
			eip, flags);
	for (;;)
		;
}

//! overflow
void _cdecl overflow_trap(uint32_t cs, uint32_t eip, uint32_t flags) {

	intstart();
	kernel_panic("Overflow trap at physical address [0x%x:0x%x] EFLAGS [0x%x]",
			cs, eip, flags);
	for (;;)
		;
}

//! bounds check
void _cdecl bounds_check_fault(uint32_t cs, uint32_t eip, uint32_t flags) {

	intstart();
	kernel_panic(
			"Bounds check fault at physical address [0x%x:0x%x] EFLAGS [0x%x]",
			cs, eip, flags);
	for (;;)
		;
}

//! invalid opcode / instruction
void _cdecl invalid_opcode_fault(uint32_t cs, uint32_t eip, uint32_t flags) {

	intstart();
	kernel_panic("Invalid opcode at physical address [0x%x:0x%x] EFLAGS [0x%x]",
			cs, eip, flags);
	for (;;)
		;
}

//! device not available
void _cdecl no_device_fault(uint32_t cs, uint32_t eip, uint32_t flags) {

	intstart();
	kernel_panic(
			"Device not found fault at physical address [0x%x:0x%x] EFLAGS [0x%x]",
			cs, eip, flags);
	for (;;)
		;
}

//! double fault
void _cdecl double_fault_abort(uint32_t eip, uint32_t cs, uint32_t flags,
		uint32_t err) {

	intstart();
	kernel_panic("Double fault at physical address [0x%x:0x%x] EFLAGS [0x%x]",
			cs, eip, flags);
	for (;;)
		;
}

//! invalid Task State Segment (TSS)
void _cdecl invalid_tss_fault(uint32_t eip, uint32_t cs, uint32_t flags,
		uint32_t err) {

	intstart();
	kernel_panic("Invalid TSS at physical address [0x%x:0x%x] EFLAGS [0x%x]",
			cs, eip, flags);
	for (;;)
		;
}

//! segment not present
void _cdecl no_segment_fault(uint32_t eip, uint32_t cs, uint32_t flags,
		uint32_t err) {

	intstart();
	kernel_panic(
			"Invalid segment at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs,
			eip, flags);
	for (;;)
		;
}

//! stack fault
void _cdecl stack_fault(uint32_t eip, uint32_t cs, uint32_t flags, uint32_t err) {

	intstart();
	kernel_panic("Stack fault at physical address [0x%x:0x%x] EFLAGS [0x%x]",
			cs, eip, flags);
	for (;;)
		;
}

//! general protection fault
void _cdecl general_protection_fault(uint32_t eip, uint32_t cs, uint32_t flags,
		uint32_t err) {

	intstart();
	kernel_panic(
			"General Protection Fault at physical address [0x%x:0x%x] EFLAGS [0x%x]",
			cs, cs, cs);
	for (;;)
		;
}

//! page fault
void _cdecl page_fault(uint32_t eip, uint32_t cs, uint32_t flags, uint32_t err) {

	intstart();

	int faultAddr = 0;

	__asm("movl %cr2,%eax");
	__asm("movl %%eax,%0":"=r"(faultAddr));

	kernel_panic("Page Fault at 0x%x:0x%x refrenced memory at 0x%x", cs, eip,
			faultAddr);
	for (;;)
		;
}

//! Floating Point Unit (FPU) error
void interrupt _cdecl fpu_fault(uint32_t cs, uint32_t eip, uint32_t flags) {

	intstart();
	kernel_panic("FPU Fault at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs,
			eip, flags);
	for (;;)
		;
}

//! alignment check
void interrupt _cdecl alignment_check_fault(uint32_t eip, uint32_t cs,
		uint32_t flags, uint32_t err) {

	intstart();
	kernel_panic(
			"Alignment Check at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs,
			eip, flags);
	for (;;)
		;
}

//! machine check
void interrupt _cdecl machine_check_abort(uint32_t cs, uint32_t eip,
		uint32_t flags) {

	intstart();
	kernel_panic("Machine Check at physical address [0x%x:0x%x] EFLAGS [0x%x]",
			cs, eip, flags);
	for (;;)
		;
}

//! Floating Point Unit (FPU) Single Instruction Multiple Data (SIMD) error
void interrupt _cdecl simd_fpu_fault(uint32_t cs, uint32_t eip, uint32_t flags) {

	intstart();
	kernel_panic("FPU SIMD fault at physical address [0x%x:0x%x] EFLAGS [0x%x]",
			cs, eip, flags);
	for (;;)
		;
}

void install_def_irq() {

	setvect(0, (void (*)(void)) divide_by_zero_fault);
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
	setvect(19, (void (*)(void)) simd_fpu_fault);
}

