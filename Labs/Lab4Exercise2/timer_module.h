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

void timer_setup(void(*)(void), int);

#endif
