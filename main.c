/*
 * main.c
 * Main file for C11204 High-Voltage supply communication
 *
 *  Created on: 5 sep. 2018
 *      Author: Marcus Persson
 */


#include "interrupts.h"
#include "functions.h"


int main(void){
	/* Change to pass address instead of null pointer */
	init_uart(NULL);
	while(1){
	}
}
