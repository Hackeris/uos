/*
 * pit.c
 *
 *  Created on: Nov 4, 2014
 *      Author: hackeris
 */

#include <klib.h>
#include <pit.h>
#include <hal.h>
#include <../../dbg/dbg.h>

void i86_pit_irq();

void timer_handler() {

	kprintf("timer.");
}

// send command to pit
void i86_pit_send_command(uint8_t cmd) {

	outportb(I86_PIT_REG_COMMAND, cmd);
}

// send data to a counter
void i86_pit_send_data(uint16_t data, uint8_t counter) {

	uint8_t port =
			(counter == I86_PIT_OCW_COUNTER0) ?
					I86_PIT_REG_COUNTER0 :
					((counter == I86_PIT_OCW_COUNTER1) ?
					I86_PIT_REG_COUNTER1 :
															I86_PIT_REG_COUNTER2);

	outportb(port, (uint8_t) data);
}

// read data from counter
uint8_t i86_pit_read_data(uint16_t counter) {

	uint8_t port =
			(counter == I86_PIT_OCW_COUNTER0) ?
					I86_PIT_REG_COUNTER0 :
					((counter == I86_PIT_OCW_COUNTER1) ?
					I86_PIT_REG_COUNTER1 :
															I86_PIT_REG_COUNTER2);

	return inportb(port);
}

// starts a counter
void i86_pit_start_counter(uint32_t freq, uint8_t counter, uint8_t mode) {

	if (freq == 0)
		return;

	uint16_t divisor = (uint16_t) (1193181 / (uint16_t) freq);

	// send operational command
	uint8_t ocw = 0;
	ocw = (ocw & ~I86_PIT_OCW_MASK_MODE) | mode;
	ocw = (ocw & ~I86_PIT_OCW_MASK_RL) | I86_PIT_OCW_RL_DATA;
	ocw = (ocw & ~I86_PIT_OCW_MASK_COUNTER) | counter;
	i86_pit_send_command(ocw);

	// set frequency rate
	i86_pit_send_data(divisor & 0xff, 0);
	i86_pit_send_data((divisor >> 8) & 0xff, 0);
}

// initialize minidriver
void i86_pit_initialize() {

	// Install our interrupt handler (irq 0 uses interrupt 32)
	setvect(32, i86_pit_irq, 0);
}
