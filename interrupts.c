/*
 * interrupts.c
 *
 *  Created on: 23 oct. 2018
 *      Author: Marcus Persson
 */

#include <string.h>
#include <stdio.h>
#include "firmware/drivers/mss_uart/mss_uart.h"
#include "firmware/drivers/mss_timer/mss_timer.h"
#include "functions.h"
#include "interrupts.h"

extern mss_uart_instance_t * const gp_my_uart1;

int *memadr;



 /* UART handler for RX from HVPS */
void uart1_rx_handler(mss_uart_instance_t * this_uart){
	uint8_t rx_buff[16]="";
	uint32_t rx_size;
	uint8_t buffer[50]="";
	static unsigned int writing = 0;

	rx_size = MSS_UART_get_rx(this_uart, rx_buff, sizeof(rx_buff)); /* Get message from external and send it on to computer terminal */
	if(writing == 0)
		buffer[0] = '\0';
	if(rx_buff[0] != 0x0d){
		writing = 1;
		strncat((char*)buffer, rx_buff, rx_size);
	}
	else {
		writing = 0;
		MSS_UART_polled_tx_string(&g_mss_uart0, buffer);
	}
	memset(rx_buff, 0, sizeof(rx_buff)); /* Clear buffer */
}

/* UART handler for RX from external source */

void uart0_rx_handler(mss_uart_instance_t * this_uart){
	unsigned char rx_buff[10] ="";
	unsigned int rx_size;
	uint8_t buffer[50]="";
	static unsigned int writing = 0;
	/* Get commands from terminal on connected computer and send them on to external*/
	rx_size = MSS_UART_get_rx(this_uart, rx_buff, sizeof(rx_buff));
	if(writing == 0)
		buffer[0] = '\0';
	if(rx_buff[0] != 0x0d){
		writing = 1;
		strncat((char*)buffer, rx_buff, rx_size);
	}
	else{
		writing = 0;
		MSS_UART_polled_tx_string(&g_mss_uart1, rx_buff);
	}
	/* Clear buffers */
	memset(rx_buff, 0, sizeof(rx_buff));
}


/* Timer interrupt for sending commands to the HVPS
 * Timing set up in timer1 init
 *
 */
void Timer1_IRQHandler(){
	static uint16_t cntr=0;
	uint8_t send[32];
	uint8_t command[4] = "";
	if(cntr==0){
		/* Command for getting Voltage output */
		strcpy(command, "HGV");
		getarray(send, command);
		MSS_UART_polled_tx_string(gp_my_uart1, send);
		strcpy(send, "");
	}
	if(cntr==1){
		/* Command for getting current output */
		strcpy(command, "HGC");
		getarray(send, command);
		MSS_UART_polled_tx_string(gp_my_uart1, send);
		strcpy(send, "");
	}
	if(cntr==2){
		/* Command for getting Temperature output */
		strcpy(command, "HGT");
		getarray(send, command);
		MSS_UART_polled_tx_string(gp_my_uart1, send);
		strcpy(send, "");
	}
	cntr=(cntr+1)%3;
	MSS_TIM1_clear_irq(); /*interrupt bit needs to be cleared after every call */
}


void init_uart(int* memory){
	/*
	 * Initialize and configure UART and timer
	 * Timer: periodic mode, loads value in load_immediate
	 * UART: 38400 BAUD, 8 bits, 1 stop bit, even parity
	 */

	memadr=*memory;
	MSS_TIM64_init(MSS_TIMER_PERIODIC_MODE);
	MSS_TIM64_load_immediate(0xFFFFFFFF, 0xFFFFFFFF);
	MSS_UART_init(gp_my_uart1, MSS_UART_38400_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_EVEN_PARITY | MSS_UART_ONE_STOP_BIT);
	MSS_UART_init(&g_mss_uart0, MSS_UART_38400_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_EVEN_PARITY | MSS_UART_ONE_STOP_BIT);
	MSS_UART_set_rx_handler(&g_mss_uart0, uart0_rx_handler, MSS_UART_FIFO_SINGLE_BYTE);
	MSS_UART_set_rx_handler(gp_my_uart1, uart1_rx_handler, MSS_UART_FIFO_SINGLE_BYTE);
	MSS_TIM64_enable_irq();
	MSS_TIM64_start();
}
