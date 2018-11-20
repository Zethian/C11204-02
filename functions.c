/*
 * functions.c
 *
 *  Created on: 23 oct. 2018
 *      Author: Marcus Persson
 */
#include "firmware/drivers/mss_gpio/mss_gpio.h"
#include "firmware/CMSIS/system_m2sxxx.h"
#include "firmware/drivers/mss_uart/mss_uart.h"
#include "firmware/drivers/mss_timer/mss_timer.h"
#include "interrupts.h"
#include "functions.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

const uint8_t stx = 0x02;
const uint8_t etx = 0x03;
const uint8_t CR = 0x0D;
uint8_t chkstr[2];
uint8_t send[32];
uint16_t rec=0;

mss_uart_instance_t * const gp_my_uart = &g_mss_uart0;
mss_uart_instance_t * const gp_my_uart1 = &g_mss_uart1;




void getarray(uint8_t *array, uint8_t cmd[28]){
	const uint8_t stx = 0x02;
	const uint8_t etx = 0x03;
	const uint8_t CR = 0x0D;
	uint16_t chksm=0x00;
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
	uint32_t output;
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

int voltageCheck(uint8_t cmd[28]){
	uint8_t data[4] = "";
	uint16_t val = 0;
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

int checkSumControl(uint8_t cmd[28]){
	uint16_t i=0;
	uint16_t sum = 0x00;
	uint8_t check[2] = "";
	while(cmd[i]!= 0x03){
		sum+=cmd[i];
		i++;
	}
	sum+=cmd[i];
	sum = (sum & 0xFF); /* Mask so only lower 2 bytes get sent */
	sprintf(check, "%X", sum);
	if(cmd[i++] == check[0] && cmd[i] == check[1]){
		return 0;
	}
	else
		return -1;
}


int HVPS_send_command(char command[]){
	char HST[30]="HST03E803E8000000006BC900C8";
	long voltage = strol(command, NULL, 10);
	char hexvolt[4];
	sprintf(hexvolt, "%X", voltage);
	for (int i=0; i<4; i++){
		HST[19+i]=hexvolt[i];
	}
	if(voltageCheck(HST) == -1)
		return -1;
	getarray(send, HST);
	MSS_UART_polled_tx_string(gp_my_uart1, HST);
	return 0;
}

/*
 * 	uint8_t rx_buff[16] ="";
	uint32_t rx_size;
	/* Get commands from terminal on connected computer, format them and send them on to HVPS
	rx_size = MSS_UART_get_rx(this_uart, rx_buff, sizeof(rx_buff));
	/* Runs test to see if the input voltage is too high
	if(voltageCheck(rx_buff) == -1){
		MSS_UART_polled_tx_string(gp_my_uart, "Voltage setting too high");
		return;
	}
	getarray(send, rx_buff);
	MSS_UART_polled_tx_string(gp_my_uart1, send);
	/* Clear buffers
	memset(send, 0, sizeof(send));
	memset(rx_buff, 0, sizeof(rx_buff));
 */
