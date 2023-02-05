/*
 * led_module.c
 *
 *  Created on: Feb. 5, 2023
 *      Author: martin
 */

#include "led_module.h"

/**
 * Turns off both LEDs
 */
void led_turn_off_all() {
    HWREG(GPIO_BASE+GPIO_O_DOUT7_4) &= !(GPIO_DOUT7_4_DIO7 | GPIO_DOUT7_4_DIO6);
}

/**
 * Turns on the specified LED
 *
 * parameters
 *
 *      led_id - an integer representing the id of the LED to be turned on
 *               6 - Red LED
 *               7 - Green LED
 */
void led_turn_on(int led_id) {
    if (led_id == 7) {
        HWREG(GPIO_BASE+GPIO_O_DOUT7_4) |= GPIO_DOUT7_4_DIO7;
    } else if (led_id == 6) {
        HWREG(GPIO_BASE+GPIO_O_DOUT7_4) |= GPIO_DOUT7_4_DIO6;
    }
}

/**
 * Turns off the specified LED
 *
 * parameters
 *
 *      led_id - an integer representing the id of the LED to be turned off
 *               6 - Red LED
 *               7 - Green LED
 */
void led_turn_off(int led_id) {
    if (led_id == 7) {
        HWREG(GPIO_BASE+GPIO_O_DOUT7_4) &= !GPIO_DOUT7_4_DIO7;
    } else if (led_id == 6) {
        HWREG(GPIO_BASE+GPIO_O_DOUT7_4) &= !GPIO_DOUT7_4_DIO6;
    }
}

/**
 * Function to set up the led module
 */
void led_setup() {
    PRCMPowerDomainOn(PRCM_DOMAIN_PERIPH);
    while(PRCMPowerDomainStatus(PRCM_DOMAIN_PERIPH) != PRCM_DOMAIN_POWER_ON);

    PRCMPeripheralRunEnable(PRCM_PERIPH_GPIO);
    PRCMPeripheralSleepEnable(PRCM_PERIPH_GPIO);

    PRCMLoadSet();
    while(!PRCMLoadGet());

    IOCPinTypeGpioOutput(IOID_7);
    IOCPinTypeGpioOutput(IOID_6);

    led_turn_off_all();
}