/*
 * interrupts.c
 *
 *  Created on: 23 oct. 2018
 *      Author: Marcus Persson
 */

#include "functions.h"
#include "firmware/drivers/mss_gpio/mss_gpio.h"
#include "firmware/CMSIS/system_m2sxxx.h"
#include "firmware/drivers/mss_uart/mss_uart.h"
#include "firmware/drivers/mss_timer/mss_timer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "interrupts.h"

/* Define GPIO0 as UART_TX and UART_RX */
extern mss_uart_instance_t * const gp_my_uart;
extern mss_uart_instance_t * const gp_my_uart1;
extern uint8_t send[32];
extern clock_t start_t, end_t, total_t;
uint16_t cntr=0;

void initAll(void){
	start_t = clock();
	/*
	 * Initialize and configure UART and timer
	 * Timer: periodic mode, loads value in load_immediate
	 * UART: 38400 BAUD, 8 bits, 1 stop bit, even parity
	 */
	MSS_TIM1_init(MSS_TIMER_PERIODIC_MODE);
	MSS_TIM1_load_immediate(0x0FFFFFFF);
	MSS_UART_init(gp_my_uart1, MSS_UART_38400_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_EVEN_PARITY | MSS_UART_ONE_STOP_BIT);
	MSS_UART_init(gp_my_uart, MSS_UART_38400_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_EVEN_PARITY | MSS_UART_ONE_STOP_BIT);
	MSS_UART_set_rx_handler(gp_my_uart1, uart1_rx_handler, MSS_UART_FIFO_FOURTEEN_BYTES);
	MSS_UART_set_rx_handler(gp_my_uart, uart0_rx_handler, MSS_UART_FIFO_EIGHT_BYTES);
	MSS_TIM1_enable_irq();
	MSS_TIM1_start();
}

 /* UART handler for RX from HVPS */
void uart1_rx_handler(mss_uart_instance_t * this_uart){
	uint8_t rx_buff[16]="";
	uint32_t rx_size;
	uint8_t output[18]="";

	rx_size = MSS_UART_get_rx(this_uart, rx_buff, sizeof(rx_buff)); /* Get message from HVPS and send it on to computer terminal */


	sprintf(output, "%i: %s", total_t, rx_buff);
	MSS_UART_polled_tx_string(gp_my_uart, rx_buff);

	//processData(rx_buff); /* Process data for certain commands */
	memset(rx_buff, 0, sizeof(rx_buff)); /* Clear buffer */
}

/* UART handler for RX from external source */

void uart0_rx_handler(mss_uart_instance_t * this_uart){
	uint8_t rx_buff[16] ="";
	uint32_t rx_size;
	/* Get commands from terminal on connected computer, format them and send them on to HVPS */
	rx_size = MSS_UART_get_rx(this_uart, rx_buff, sizeof(rx_buff));
	/* Runs test to see if the input voltage is too high */
	if(voltageCheck(rx_buff) == -1){
		MSS_UART_polled_tx_string(gp_my_uart, "Voltage setting too high");
		return;
	}
	getarray(send, rx_buff);
	MSS_UART_polled_tx_string(gp_my_uart1, send);
	/* Clear buffers */
	memset(send, 0, sizeof(send));
	memset(rx_buff, 0, sizeof(rx_buff));
}

/* Timer interrupt for sending commands to the HVPS
 * Timing set up in timer1 init
 *
 */
void Timer1_IRQHandler(){
	uint8_t command[4] = "";
	if(cntr%3==0){
		/* Command for getting Voltage output */
		strcpy(command, "HGV");
		getarray(send, command);
		MSS_UART_polled_tx_string(gp_my_uart, send);
		strcpy(send, "");
		cntr=0;
	}
	if(cntr%3==1){
		/* Command for getting current output */
		strcpy(command, "HGC");
		getarray(send, command);
		MSS_UART_polled_tx_string(gp_my_uart, send);
		strcpy(send, "");
	}
	if(cntr%3==2){
		/* Command for getting Temperature output */
		strcpy(command, "HGT");
		getarray(send, command);
		MSS_UART_polled_tx_string(gp_my_uart, send);
		strcpy(send, "");
	}
	cntr++;
	MSS_TIM1_clear_irq(); /*interrupt bit needs to be cleared after every call */
}
