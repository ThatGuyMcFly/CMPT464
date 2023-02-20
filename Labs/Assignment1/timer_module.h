/**
 * Martin Knoetze
 * SN: 3086754
 * CMPT464 Assignment 1
 * Due: February 22nd, 2023
 *
 * timer_module.h
 *
 * This is the header file for the timer module
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
 * Enables the timer and sets the next delay
 *
 * parameters
 *
 *      delay - an unsigned 32 bit integer to set the next delay in milliseconds
 */
void timer_enable_interrupt(uint32_t);

/**
 * Disables and clears the timer interrupt
 */
void timer_disable_and_clear_interrupt();

/**
 * Sets up the timer
 *
 * parameters
 *
 *      passedInFunction - a function that takes void pointer as a parameter and returns nothing. This function will be called in the timer interrupt function
 *
 *      passedDelay - an integer that sets the delay time in milliseconds
 */
void timer_setup(void(*)(void), uint32_t);

#endif
