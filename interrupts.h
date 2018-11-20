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
void initUART(void);

/*
 * uart1_rx_handler
 * Interrupt for handling UART communication with HVPS side.
 * Input: UART pointer
 * Output: None
 *
 */
void uart1_rx_handler(mss_uart_instance_t * this_uart);


/*
 * uart0_rx_handler
 * Interrupt for handling UART communication with SmartFusion2 side
 * Input: UART pointer
 * Output: None
 *
 */
void uart0_rx_handler(mss_uart_instance_t * this_uart);


/*
 * Timer1_IRQHandler
 * Interrupt handler for Timer1, counting for periodic status messages to HVPS
 * Input: None
 * Output: None
 *
 */

void Timer1_IRQHandler(void);

#endif /* INTERRUPTS_H_ */
