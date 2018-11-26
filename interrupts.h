/*
 * interrupts.h
 *
 *  Created on: 23 oct. 2018
 *      Author: Marcus Persson
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "firmware/drivers/mss_uart/mss_uart.h"

/*
 * Initalizes all interrupts and timestamp timer
 * Input: None
 * Output: None
 *
 */
void init_uart(int* memory);



#endif /* INTERRUPTS_H_ */
