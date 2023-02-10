/**
 * CMPT464 Lab 4 Exercise 1
 * Martin Knoetze
 * SN: 3086754
 *
 * timer_module.h
 */

#ifndef TIMER_MODULE_H_
#define TIMER_MODULE_H_

#include "driverlib/prcm.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/ioc.h"
#include "driverlib/timer.h"
#include "driverlib/cpu.h"

/**
 * Sets up the timer
 *
 * parameters
 *
 *  passedInFunction - a function that takes no parameters and returns nothing. This function will be called in the timer interrupt function
 *
 *  passedDelay - an integer that sets the delay time in milliseconds
 */
void timer_setup(void(*)(void *), int);

#endif
