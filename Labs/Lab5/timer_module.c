/**
 * CMPT464 Lab 5
 * Martin Knoetze
 * SN: 3086754
 *
 * timer_module.c
 */

#include "timer_module.h"

#define MS 3000
#define NULL 0x00000000

void (*callback)(void *ptr) = NULL;
int delay = 0;

/**
 * function called when a timer interrupt is triggered
 */
void timer_interrupt_fn() {

    // Disable and clear the interrupt
    TimerIntDisable(GPT0_BASE,TIMER_TIMA_TIMEOUT);
    TimerIntClear(GPT0_BASE, TIMER_TIMA_TIMEOUT);

    if(callback != NULL) {
        callback(NULL);
    }

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
 * Sets up the timer
 *
 * parameters
 *
 *  passedInFunction - a function that takes no parameters and returns nothing. This function will be called in the timer interrupt function
 *
 *  passedDelay - an integer that sets the delay time in milliseconds
 */
void timer_setup(void (*passedInCallbackFunction)(void *ptr), int passedDelay) {
    delay = passedDelay;
    callback = passedInCallbackFunction;
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

    // Configure timer to use timer A in one shot mode
    TimerConfigure(GPT0_BASE, TIMER_CFG_ONE_SHOT);

    // Load initial time into timer A
    TimerLoadSet(GPT0_BASE, TIMER_A, delay*MS);

    // Register interrupt function with timer A
    TimerIntRegister(GPT0_BASE, TIMER_A, timer_interrupt_fn);

    // Enable interrupt with timer A's timeout
    TimerIntEnable(GPT0_BASE,TIMER_TIMA_TIMEOUT);

    // Enable timer A
    TimerEnable(GPT0_BASE,TIMER_A);
}


