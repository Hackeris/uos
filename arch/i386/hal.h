/*
 * hal.h
 *
 *  Created on: Nov 2, 2014
 *      Author: hackeris
 */

#ifndef HAL_H_
#define HAL_H_

void setvect(int intno, void (*vect)(), int flags);

void interruptdone(unsigned int intno);

int initalize_cpu();

#endif /* HAL_H_ */
