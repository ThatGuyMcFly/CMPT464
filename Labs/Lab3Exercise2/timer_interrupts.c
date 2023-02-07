/**
 * CMPT464 Lab 3 Exercise 1
 * Martin Knoetze
 * SN: 3086754
 *
 * timer_interrupts.c
 */

#include "timer_interrupts.h"

#define MS 3000

typedef struct {
    int id;
    int interval;
} led_state;

const led_state LED_STATE_ARRAY[] = { {7, 100}, {0, 900}, {6, 100}, {0, 2995} };

const int LED_STATE_ARRAY_LENGTH = sizeof(LED_STATE_ARRAY)/sizeof(LED_STATE_ARRAY[0]);

int led_state_index = 0x0;

/**
 * Sets up the LEDs
 */
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
    IOCPinTypeGpioOutput(IOID_6);
}

/**
 * Turns on the specified LED
 *
 * parameters
 *  int led_id - The id of the LED to be turned on
 */
void toggle_led(int led_id) {
    if(led_id == 7) {
        // Turn on green LED
        HWREG(GPIO_BASE+GPIO_O_DOUT7_4) |= GPIO_DOUT7_4_DIO7;
    } else if (led_id == 6) {
        // Turn on red LED
        HWREG(GPIO_BASE+GPIO_O_DOUT7_4) |= GPIO_DOUT7_4_DIO6;
    } else {
        //Turn off both LEDs
        HWREG(GPIO_BASE+GPIO_O_DOUT7_4) &= 0xFEFEFFFF;
    }
}

/**
 * Interrupt service routine for the timer interrupts
 */
void timer_interrupt_fn() {

    // Disable and clear the interrupt
    TimerIntDisable(GPT0_BASE,TIMER_TIMA_TIMEOUT);
    TimerIntClear(GPT0_BASE, TIMER_TIMA_TIMEOUT);

    // Increment the index for the led state
    led_state_index = (led_state_index + 1) % LED_STATE_ARRAY_LENGTH;

    toggle_led(LED_STATE_ARRAY[led_state_index].id);

    // Configure timer to use timer A in one shot mode
    TimerConfigure(GPT0_BASE, TIMER_CFG_ONE_SHOT);

    // Load initial time into timer A
    TimerLoadSet(GPT0_BASE, TIMER_A, LED_STATE_ARRAY[led_state_index].interval * MS);

    // Enable interrupt with timer A's timeout
    TimerIntEnable(GPT0_BASE,TIMER_TIMA_TIMEOUT);

    // Enable timer A
    TimerEnable(GPT0_BASE,TIMER_A);
}

/**
 * Sets up the timer
 */
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

    toggle_led(LED_STATE_ARRAY[led_state_index].id);

    // Configure timer to use timer A in one shot mode
    TimerConfigure(GPT0_BASE, TIMER_CFG_ONE_SHOT);

    // Load initial time into timer A
    TimerLoadSet(GPT0_BASE, TIMER_A, LED_STATE_ARRAY[led_state_index].interval * MS);

    // Register interrupt function with timer A
    TimerIntRegister(GPT0_BASE, TIMER_A, timer_interrupt_fn);

    // Enable interrupt with timer A's timeout
    TimerIntEnable(GPT0_BASE,TIMER_TIMA_TIMEOUT);

    // Enable timer A
    TimerEnable(GPT0_BASE,TIMER_A);
}

/**
 * Sets up the timer and LEDs
 */
void setup_timer_interrupt() {
    setup_led();
    setup_timer();
}
