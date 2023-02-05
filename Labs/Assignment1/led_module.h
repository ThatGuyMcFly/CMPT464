/*
 * led_module.h
 *
 *  Created on: Feb. 5, 2023
 *      Author: martin
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
