/*
 * functions.h
 *
 *  Created on: 23 oct. 2018
 *      Author: Marcus Persson
 */


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
 * Function takes pointer to destination array and commandline received from terminal, and formats it to readable for HVPS
 * Input: Char array pointer, char array command
 * Output: None
 *
 */

void getarray(uint8_t *array, uint8_t cmd[28]);




/*
 * HVPS_send_voltage()
 * Takes incoming voltage command and transforms it into hex and formats the HST command to uart.
 * Input: A string literal with voltage in decimal form
 * Output: Return -1 for fail, 0 for pass.
 */

int HVPS_send_voltage(char command[]);

#endif /* FUNCTIONS_H_ */
