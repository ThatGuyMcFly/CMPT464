/**
 * Martin Knoetze
 * SN: 3086754
 * CMPT464 Assignment 1
 * Due: February 22nd, 2023
 *
 * led_module.h
 *
 * This is the header file for the LED module
 */


#ifndef LED_MODULE_H_
#define LED_MODULE_H_

#include "driverlib/prcm.h"
#include "driverlib/gpio.h"
#include "driverlib/ioc.h"

/**
 * Turns off both LEDs
 */
void led_turn_of_all();

/**
 * Turns on the specified LED
 *
 * parameters
 *
 *      led_id - an integer representing the id of the LED to be turned on
 *               6 - Red LED
 *               7 - Green LED
 */
void led_turn_on(int);

/**
 * Turns off the specified LED
 *
 * parameters
 *
 *      led_id - an integer representing the id of the LED to be turned off
 *               6 - Red LED
 *               7 - Green LED
 */
void led_turn_off(int);

/**
 * Function to set up the led module
 */
void led_setup();

#endif /* LED_MODULE_H_ */
