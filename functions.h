/*
 * functions.h
 *
 *  Created on: 23 oct. 2018
 *      Author: Marcus Persson
 */
#include "firmware/drivers/mss_gpio/mss_gpio.h"
#include "firmware/CMSIS/system_m2sxxx.h"
#include "firmware/drivers/mss_uart/mss_uart.h"
#include "firmware/drivers/mss_timer/mss_timer.h"

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


/*
 * Sends string to HVPS with some boot settings (not needed in later documentation)
 * Input: None
 * Output: None
 *
 */

void startHVPS(void);

/*
 * Function takes recieved data and processes it to a readable voltage, current or temperature, and sends that to terminal
 * Input: Data, char array
 * Output: None
 *
 */

void processData(uint8_t data[]);


/*
 * Function takes pointer to destination array and commandline received from terminal, and formats it to readable for HVPS
 * Input: Char array pointer, char array command
 * Output: None
 *
 */

void getarray(uint8_t *array, uint8_t cmd[28]);



/*
 * voltageCheck
 * Reads recieved command to see that voltage is not over 55V (limit on MPPC)
 * Input: Command char array
 * Output: Return -1 as int if over 55V, return 0 otherwise
 *
 */
int voltageCheck(uint8_t cmd[28]);

#endif /* FUNCTIONS_H_ */
