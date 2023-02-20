/**
 * Martin Knoetze
 * SN: 3086754
 * CMPT464 Assignment 1
 * Due: February 22nd, 2023
 *
 * timer_module.c
 *
 * This module handles the setup of the timer and its interrupt.
 */

#include "timer_module.h"

#define MS 3000
#define NULL 0x00000000

void (*timer_callback_function)(void) = NULL;

/**
 * function called when a timer interrupt is triggered
 */
void timer_interrupt_handler() {

    timer_disable_and_clear_interrupt();

    if(timer_callback_function != NULL) {
        timer_callback_function();
    }
}

/**
 * Enables the timer and sets the next delay
 *
 * parameters
 *
 *      delay - an unsigned 32 bit integer to set the next delay in milliseconds
 */
void timer_enable_interrupt(uint32_t delay) {
    // Configure timer to use timer A in one shot mode
    TimerConfigure(GPT0_BASE, TIMER_CFG_ONE_SHOT);

    // Load initial time into timer A
    TimerLoadSet(GPT0_BASE, TIMER_A, delay * MS);

    // Enable interrupt with timer A's timeout
    TimerIntEnable(GPT0_BASE,TIMER_TIMA_TIMEOUT);

    // Enable timer A
    TimerEnable(GPT0_BASE,TIMER_A);
}

/**
 * Disables and clears the timer interrupt
 */
void timer_disable_and_clear_interrupt() {
    TimerIntDisable(GPT0_BASE,TIMER_TIMA_TIMEOUT);
    TimerIntClear(GPT0_BASE, TIMER_TIMA_TIMEOUT);
}

/**
 * Sets up the timer
 *
 * parameters
 *
 *      passedInFunction - a function that takes void pointer as a parameter and returns nothing. This function will be called in the timer interrupt function
 *
 *      passedDelay - an integer that sets the delay time in milliseconds
 */
void timer_setup(void (*passedInCallbackFunction)(void), uint32_t delay) {

    timer_callback_function = passedInCallbackFunction;
    // Turn on Peripheral Power domain
    PRCMPowerDomainOn(PRCM_DOMAIN_PERIPH);
    while(PRCMPowerDomainStatus(PRCM_DOMAIN_PERIPH)!= PRCM_DOMAIN_POWER_ON);

    // Set timer to be active in run and sleep mode
    PRCMPeripheralRunEnable(PRCM_PERIPH_TIMER0);
    PRCMPeripheralSleepEnable(PRCM_PERIPH_TIMER0);

    PRCMLoadSet();
    while(!PRCMLoadGet());

    // Set the clock division to 16
    PRCMGPTimerClockDivisionSet(PRCM_CLOCK_DIV_16);
    PRCMLoadSet();
    while ( !PRCMLoadGet() );

    // Register interrupt function with timer A
    TimerIntRegister(GPT0_BASE, TIMER_A, timer_interrupt_handler);

    if (delay > 0) {
        timer_enable_interrupt(delay);
    }
}


