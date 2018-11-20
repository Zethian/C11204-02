/*
 * main.c
 * Main file for C11204-01 High-Voltage supply communication
 *
 *  Created on: 5 sep. 2018
 *      Author: Marcus Persson
 *
 *      Connect USB to computer with terminal open
 *      38400 BAUD, 8 data bits, 1 stop bit, even parity no flow control, data order LSB
 *      Send commands according to C11204-01 command reference
 *      Only command bytes needed, checksum, start, stop and delimiter is not needed, added by the program.
 */


#include "firmware/drivers/mss_gpio/mss_gpio.h"
#include "firmware/CMSIS/system_m2sxxx.h"
#include "firmware/drivers/mss_uart/mss_uart.h"
#include "firmware/drivers/mss_timer/mss_timer.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* Variables used over several functions defined as global */

uint8_t chkstr[2];
uint8_t send[40];
uint16_t rec=0;


/* Prototype declaration */
void processData(uint8_t data[]);
void getarray(uint8_t *array, uint8_t cmd[28]);

/* Define GPIO0 as UART_TX and UART_RX */
mss_uart_instance_t * const gp_my_uart = &g_mss_uart0;
mss_uart_instance_t * const gp_my_uart1 = &g_mss_uart1;

/* Checks for received voltage level, rejects if higher than 55V */
int voltageCheck(uint8_t cmd[28]){
	uint8_t data[4] = "";
	uint32_t val = 0;
	if((cmd[0]=='H' && cmd[1]=='S' && cmd[2]=='T')) {
		for(int i=0; i<4; i++){
			data[i] = cmd[i+19];
		}
	}
	else if(cmd[0]=='H' && cmd[1]=='B' && cmd[2]=='V'){
		for(int i=0; i<4; i++){
			data[i] = cmd[i+3];
		}
	}
	val=strtol(data, NULL, 16);
	val=round(val*(1.812/pow(10, 3)));
	if(val > 55)
		return -1;
	else
		return 0;
}

 /* UART handler for RX from HVPS */
void uart1_rx_handler(mss_uart_instance_t * this_uart){
	uint8_t rx_buff[16]="";
	uint32_t rx_size;
	uint8_t output[18]="";
	rx_size = MSS_UART_get_rx(this_uart, rx_buff, sizeof(rx_buff)); /* Get message from HVPS and send it on to computer terminal */

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

/*
 * Gets the combined array from all the parts of the message by adding start and stop bits, delimiter and calculating the checksum
 */
void getarray(uint8_t *array, uint8_t cmd[28]){
	const uint8_t stx = 0x02;
	const uint8_t etx = 0x03;
	const uint8_t CR = 0x0D;
	uint8_t chksm=0x00;
	/* Memmove is used with offset for the adress because strcat did not give the proper format when sending it on to the HVPS */
	memmove(array, &stx, 1);
	memmove(array+1, cmd, strlen(cmd));
	memmove(array+1+strlen(cmd), &etx, 1);
	for(int i = 0; array[i-1]!=0x03; i++){
		chksm+=array[i];
	}
	chksm = (chksm & 0xFF); /* Mask so only lower 2 bytes get sent */
	sprintf(chkstr, "%X", chksm);
	memmove(array+2+strlen(cmd), chkstr, 2);
	memmove(array+4+strlen(cmd), &CR, 1);
	strcpy(cmd, "");

}

/* Processes received data from the RX */
void processData(uint8_t data[]){
	uint64_t output;
	uint8_t out[4] = "";
	for(int i = 0; i<4; i++){
		out[i]=data[i+4];
	}
	output=strtol(out, NULL, 16);
	if(data[1]=='h' && data[2] == 's' && data[3] == 't'){
		rec=1;
		strcpy(send, "High-Voltage on");
	}
	else if(data[1] =='h' && data[2] == 'g' && data[3] == 'v'){
		output=round(output*(1.812/pow(10, 3)));
		sprintf(send, "Voltage output is: %i V\n", output);

	}
	else if(data[1] =='h' && data[2] == 'g' && data[3] == 'c'){
		output=round(output*(5.194/pow(10, 3)));
		sprintf(send, "Current output is: %i A\n", output);

	}
	else if(data[1] =='h' && data[2] == 'g' && data[3] == 't'){
		output=round(((output*1.907/pow(10,5))-1.035)/(0-5.5/pow(10,3)));
		sprintf(send, "MPCC Temperature is: %i C\n", output);

	}
	MSS_UART_polled_tx_string(gp_my_uart, send);
	memset(send, 0, sizeof(send));

}

/* Timer interrupt for sending commands to the HVPS
 * Timing set up in timer1 init
 * Gets status messages every call, with different voltage, current and temperature getting sent in a circular way
 */
void Timer1_IRQHandler(){
	uint8_t command[4] = "";

	static int cntr=0;
	if(cntr%3==0){
		/* Command for getting Voltage output */
		strcpy(command, "HGV");
		getarray(send, command);
		MSS_UART_polled_tx_string(gp_my_uart1, send);
		strcpy(send, "");
		cntr=0;
	}
	if(cntr%3==1){
		/* Command for getting current output */
		strcpy(command, "HGC");
		getarray(send, command);
		MSS_UART_polled_tx_string(gp_my_uart1, send);
		strcpy(send, "");
	}
	if(cntr%3==2){
		/* Command for getting Temperature output */
		strcpy(command, "HGT");
		getarray(send, command);
		MSS_UART_polled_tx_string(gp_my_uart1, send);
		strcpy(send, "");
	}
	cntr++;
	MSS_TIM1_clear_irq(); /*interrupt bit needs to be cleared after every call */
}


/*
 * Setting Temperature correction factor to 0 (off) and voltage output to 70V
 */
void startHVPS(void){
	char temp[50] = "";
	strcpy(temp, "HST03E803E8000000006BC900C8");
	getarray(send, temp); /*get required string from function */
	MSS_UART_polled_tx_string(gp_my_uart1, send);
	while(rec==0){ /* wait for HVPS to confirm started */
		break;
	}
	strcpy(send, "");
}

int main(void){

	/* Turn off watchdog */
	SYSREG->WDOG_CR=0;

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
	//startHVPS();


	MSS_TIM1_start();
	/* Send command for status */
	while(1){
	}
}
