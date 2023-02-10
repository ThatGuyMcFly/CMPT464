/**
 * CMPT464 Assignment 1
 * Martin Knoetze
 * SN: 3086754
 *
 * main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "led_module.h"
#include "uart_module.h"
#include "timer_module.h"

#define STOP_VALUE 0xff

int tCurrent = 24;
int tChosen = 0;

typedef enum {ON, OFF} SUB_PERIOD;

typedef struct {
    int led_id;
    char character;
    int delays[2];
    SUB_PERIOD currentSubPeriod;
} mode;

mode heatingMode;
mode coolingMode;

mode * currentModePtr;

/**
 * The callback functions to be called by the uart interrupt handler
 *
 * parameters
 *
 *      characters - string of characters sent from the uart interrupt function
 */
void uart_callback(char * characters) {

    char * stopString = "FF  ";

    if(strcmp(characters, stopString) != 0) {
        tChosen = (int)(characters[0] - '0') * 10;
        tChosen += (int)(characters[1] - '0');
    } else {
        tChosen = STOP_VALUE;
    }

    if(tChosen >= 20 && tChosen <= 30) {

        if(tCurrent < tChosen) {
            currentModePtr = &heatingMode;
        } else if (tCurrent > tChosen) {
            currentModePtr = &coolingMode;
        } else {
            return;
        }

        currentModePtr->currentSubPeriod = ON;

        led_turn_on(currentModePtr->led_id);

        uart_send_data(currentModePtr->character);

        timer_enable_interrupt(currentModePtr->delays[currentModePtr->currentSubPeriod]);
    }

}

/**
 * Generates a 0 or 1 randomly
 */
int random_0_1() {
    time_t t;

    srand((unsigned) time(&t));

    return (rand() % 2);
}

/**
 * Converts a 2 digit unsigned 32 bit integer into a two character string
 */
void convert_to_char(uint32_t data, char * characters) {

    int digit_1 = data / 10;
    int digit_2 = data - digit_1 * 10;

    char character1 = '0' + digit_1;
    char character2 = '0' + digit_2;

    characters[0] = character1;
    characters[1] = character2;
}

/**
 * Sends the current and chosen temperature to the uart module to be transmitted
 */
void send_temperature_message() {
    char temperatureString[2];
    uart_send_data('(');

    convert_to_char((uint32_t) tCurrent, temperatureString);

    for (int i = 0; i < 2; i++) {
        uart_send_data(temperatureString[i]);
    }

    uart_send_data(',');

    convert_to_char((uint32_t) tChosen, temperatureString);

    for (int i = 0; i < 2; i++) {
        uart_send_data(temperatureString[i]);
    }

    uart_send_data(')');
}

/**
 * increments or decrements the current temperature depending on the current mode
 */
void increment_decrement_temperature() {
    if (currentModePtr->character == 'C') {
        tCurrent -= random_0_1();
    } else if (currentModePtr->character == 'H') {
        tCurrent += random_0_1();
    }
}

/**
 * The callback function to be called by the timer interrupt function
 */
void timer_callback() {

    if (currentModePtr->currentSubPeriod == OFF) {

        uart_set_interrupt(SET_INTERRUPT_ENABLED);

        if (tCurrent != tChosen && tCurrent != STOP_VALUE) {
            currentModePtr->currentSubPeriod = ON;

            uart_send_data(currentModePtr->character);

            led_turn_on(currentModePtr->led_id);

            increment_decrement_temperature();

            timer_enable_interrupt(currentModePtr->delays[currentModePtr->currentSubPeriod]);
        } else {
            timer_disable_and_clear_interrupt();
        }

    } else {

        uart_set_interrupt(SET_INTERRUPT_DISABLED);

        currentModePtr->currentSubPeriod = OFF;

        led_turn_off(currentModePtr->led_id);

        send_temperature_message();

        timer_enable_interrupt(currentModePtr->delays[currentModePtr->currentSubPeriod]);
    }

}

int main(void)
{
    heatingMode.led_id = 6;
    heatingMode.character = 'H';
    heatingMode.delays[0] = 400;
    heatingMode.delays[1] = 600;
    heatingMode.currentSubPeriod = OFF;

    coolingMode.led_id = 7;
    coolingMode.character = 'C';
    coolingMode.delays[0] = 500;
    coolingMode.delays[1] = 500;
    coolingMode.currentSubPeriod = OFF;

    led_setup();
    uart_setup(&uart_callback, 115200, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
    timer_setup(&timer_callback, 0);
    PRCMSleep();
	return 0;
}
