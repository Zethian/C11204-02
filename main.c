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

	/* Turn off watchdog */
	SYSREG->WDOG_CR=0;
	initUART();
	while(1){
	}
}
