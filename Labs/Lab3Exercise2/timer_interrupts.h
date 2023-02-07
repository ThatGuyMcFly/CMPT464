/**
 * CMPT464 Lab 3 Exercise 1
 * Martin Knoetze
 * SN: 3086754
 *
 * timer_interrupts.h
 */

#ifndef TIMER_INTERRUPTS_H_
#define TIMER_INTERRUPTS_H_

#include "driverlib/prcm.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/ioc.h"
#include "driverlib/timer.h"
#include "driverlib/cpu.h"

void setup_timer_interrupt();

#endif
