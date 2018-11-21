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

	initUART(0x60000000);
	while(1){
	}
}
