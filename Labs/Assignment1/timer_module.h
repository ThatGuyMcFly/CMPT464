/**
 * CMPT464 Assignment 1
 * Martin Knoetze
 * SN: 3086754
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

void timer_enable_interrupt(uint32_t);
void timer_disable_and_clear_interrupt();
void timer_setup(void(*)(void), uint32_t);

#endif
