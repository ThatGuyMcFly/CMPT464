/**
 * CMPT464 Assignment 1
 * Martin Knoetze
 * SN: 3086754
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

void led_turn_of_all();
void led_turn_on(int);
void led_turn_off(int);
void led_setup();

#endif /* LED_MODULE_H_ */
