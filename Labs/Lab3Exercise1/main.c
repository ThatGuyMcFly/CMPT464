/**
 * CMPT464 Lab 3 Exercise 1
 * Martin Knoetze
 * SN: 3086754
 *
 * main.c
 */

#include "timer_interrupts.h"

int main(void)
{
    setup_timer_interrupt();
    PRCMSleep();
    return 0;
}
