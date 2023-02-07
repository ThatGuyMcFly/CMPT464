/**
 * CMPT464 Lab 3 Exercise 1
 * Martin Knoetze
 * SN: 3086754
 *
 * timer_interrupts.c
 */

#include "timer_interrupts.h"

#define OFF_TIME 12000000
#define ON_TIME 6000000

int led_on = 0x0;

void setup_led() {
    // Enable Peripheral power domain
    PRCMPowerDomainOn(PRCM_DOMAIN_PERIPH);
    while(PRCMPowerDomainStatus(PRCM_DOMAIN_PERIPH) != PRCM_DOMAIN_POWER_ON);

    // Enable GIO peripheral
    PRCMPeripheralRunEnable(PRCM_PERIPH_GPIO);
    PRCMPeripheralSleepEnable(PRCM_PERIPH_GPIO);

    // Make sure that the configurations are propagated
    PRCMLoadSet();
    while ( !PRCMLoadGet() );

    // Set the GIO configuration
    IOCPinTypeGpioOutput(IOID_7);
}

void timer_interrupt_fn() {

    // Disable and clear the interrupt
    TimerIntDisable(GPT0_BASE,TIMER_TIMA_TIMEOUT);
    TimerIntClear(GPT0_BASE, TIMER_TIMA_TIMEOUT);

    // Toggle the green LED
    GPIO_toggleDio(7);


    led_on ^= 0x1;

    // Set new timer count based on if the green LED is on or off
    int time = OFF_TIME;

    if(led_on == 1) {
        time = ON_TIME;
    }

    // Configure timer to use timer A in one shot mode
    TimerConfigure(GPT0_BASE, TIMER_CFG_ONE_SHOT);

    // Load initial time into timer A
    TimerLoadSet(GPT0_BASE, TIMER_A, time);

    // Enable interrupt with timer A's timeout
    TimerIntEnable(GPT0_BASE,TIMER_TIMA_TIMEOUT);

    // Enable timer A
    TimerEnable(GPT0_BASE,TIMER_A);
}

void setup_timer() {
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

    // Configure timer to use one shot mode
    TimerConfigure(GPT0_BASE, TIMER_CFG_ONE_SHOT);

    // Load initial time into timer A
    TimerLoadSet(GPT0_BASE, TIMER_A, OFF_TIME);

    // Clear the interrupt
    TimerIntClear(GPT0_BASE,TIMER_TIMA_TIMEOUT);

    // Register interrupt function with timer A
    TimerIntRegister(GPT0_BASE, TIMER_A, timer_interrupt_fn);

    // Enable interrupt with timer A's timeout
    TimerIntEnable(GPT0_BASE,TIMER_TIMA_TIMEOUT);

    // Enable timer A
    TimerEnable(GPT0_BASE,TIMER_A);
}

void setup_timer_interrupt() {
    setup_led();
    setup_timer();
}
