/**
 * Martin Knoetze
 * SN: 3086754
 * CMPT464 Assignment 1
 * Due: February 22nd, 2023
 *
 * main.c
 *
 * This Program simulates an air conditioning control system. The user enters a desired
 * temperature between 20 and 30 degrees, and the system triggers either a heating or
 * cooling period to reach the desired temperature. If the user enters "FF  " then the program
 * will go back into sleep mode at the end of a full period. The program will output a character
 * representing the period it is in as well as lighting an LED on the board to represent
 * the current period.
 *
 * This is the main file of the program. This file handles the logic of switching between
 * the heating and cooling periods, and generating the output for the periods.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "led_module.h"
#include "uart_module.h"
#include "timer_module.h"

// Macro for defining the string to expect from the user to stop the system
#define STOP_STRING "FF  "

// Integers for keeping track of the current and chosen temperatures
int tCurrent = 0;
int tChosen = 0;

int stopFlag = 0; // flag for keeping track of whether or not the user has chosen to stop the system

typedef enum {OFF, ON} subPeriod; // enum for keeping track of the subperiod

typedef enum {SLEEP, ACTIVE} mode; // enum for keeping track of the mode

// struct for handling the 2 periods of the program
typedef struct {
    int led_id;
    char character;
    int delays[2];
} period;

// Setup the periods and the current period pointer
period heatingPeriod;

period coolingPeriod;

period * currentPeriodPtr;

// initialize the current sub period to off
subPeriod currentSubPeriod = OFF;

// initialize the current mode to sleep
mode currentMode = SLEEP;

/**
 * Handles when a new temperature is entered
 */
void handleNewChosenTemperature() {
    // Set the current period based on the current and chosen temperatures
    if(tCurrent < tChosen) {
        currentPeriodPtr = &heatingPeriod;
    } else if (tCurrent > tChosen) {
        currentPeriodPtr = &coolingPeriod;
    }

    // If in sleep mode, set mode to active, print a new line and enable the time interrupt
    if (currentMode == SLEEP) {
        // set current mode to active
        currentMode = ACTIVE;

        // print a new line
        uart_send_data("\n\r\0");

        // call function to enable the interrupt with 0 delay to immediately trigger the timer interrupt
        timer_enable_interrupt(0);
    }
}

/**
 * The callback functions to be called by the uart interrupt handler
 *
 * parameters
 *
 *      characters - string of characters sent from the uart interrupt function
 */
void uart_callback(char * characters) {
    int newTemp = 0;
    // Check if the characters entered are the stop string
    if (strcmp(characters, STOP_STRING) == 0) {
        // Set the stop flag
        stopFlag = 1;
    } else if(characters[2] == ' ' && characters[3] == ' ') {
        // Set tChosen to the integer value of the first two characters passed in
        newTemp = (int)(characters[0] - '0') * 10;
        newTemp += (int)(characters[1] - '0');
    }

    // Ensure the chosen temperature is in the appropriate range
    if(newTemp >= 20 && newTemp <= 30 && newTemp != tCurrent) {
        tChosen = newTemp;
        // Call the function to handle a new temperature
        handleNewChosenTemperature();
    } else if (stopFlag != 1) {
        // print a new line if invalid temperature and not stop value
        uart_send_data("\n\r\0");
    }
}

/**
 * Converts a 2 digit unsigned 32 bit integer into a two character string
 *
 * parameters
 *
 *      data - an integer to be converted to characters
 *
 *      characters - a string of characters into which the 2 converted digits will be placed
 *
 *      index - the starting index of where to place the 2 converted digits
 */
void convert_to_char(int data, char * characters, int index) {

    // Isolate the first and second digits into their own variables
    int digit_1 = data / 10;
    int digit_2 = data - digit_1 * 10;

    // Convert the digits to characters
    char character1 = '0' + digit_1;
    char character2 = '0' + digit_2;

    // Put the characters in order starting at the specified index
    characters[index] = character1;
    characters[index + 1] = character2;
}

/**
 * Sends the current and chosen temperature to the uart module to be transmitted
 */
void send_temperature_message() {
    // Create a default string with empty spaces for the temperature values
    char temperatureString[] = "(  ,  )\n\r";

    // Convert the current temperature and put the 2 digits in the
    // string starting at index 1
    convert_to_char(tCurrent, temperatureString, 1);

    // Convert the chosen temperature and put the 2 digits in the
    // string starting at index 4
    convert_to_char(tChosen, temperatureString, 4);

    // Send the string to the UART for transmission
    uart_send_data(temperatureString);
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
 * increments or decrements the current temperature depending on the current period
 */
void increment_decrement_temperature() {

    if (currentPeriodPtr->character == 'C') {
        // Decrement the temperature if in cooling period
        tCurrent -= random_0_1();
    } else if (currentPeriodPtr->character == 'H') {
        // Increment the temperature if in heating period
        tCurrent += random_0_1();
    }
}

/**
 * Function for handling the off period functionality
 */
void handleOnSubPeriod(){
    // disable the interrupt during the on period
    uart_set_interrupt(SET_INTERRUPT_DISABLED);

    // Check if current temperature has reached the desired temperature,
    // or if the stop value has been entered
    if (tCurrent != tChosen && stopFlag != 1) {

        // Create a string to send to the uart for transmission
        char character_to_send[] = " \0";

        // Set the first character in the string to the character for the period
        // the program is currently in
        character_to_send[0] = currentPeriodPtr->character;

        uart_send_data(character_to_send);

        // turn on the LED for the current period
        led_turn_on(currentPeriodPtr->led_id);

        // update temperature
        increment_decrement_temperature();

        // Enable the timer interrupt with the appropriate delay
        timer_enable_interrupt(currentPeriodPtr->delays[currentSubPeriod]);
    } else {
        // If current temperature has reached the chosen temperature
        // or the user has chosen to stop the process

        // clear the stop flag
        stopFlag = 0;

        // set current mode to sleep
        currentMode = SLEEP;

        // set current sub period to off
        currentSubPeriod = OFF;

        // re-enable the uart interrupt
        uart_set_interrupt(SET_INTERRUPT_ENABLED);

        // disable and clear the timer interrupts
        timer_disable_and_clear_interrupt();
    }
}

/**
 * Function for handling the on sub period functionality
 */
void handleOffSubPeriod() {
    // enable to uart interrupt
    uart_set_interrupt(SET_INTERRUPT_ENABLED);

    // turn off the led for the current period
    led_turn_off(currentPeriodPtr->led_id);

    // send the temperature message to the user
    send_temperature_message();

    // enabled the interrupt with the appropriate delay
    timer_enable_interrupt(currentPeriodPtr->delays[currentSubPeriod]);
}

/**
 * The callback function to be called by the timer interrupt function
 */
void timer_callback() {
    if (currentMode == ACTIVE) {
        if (currentSubPeriod == OFF) {
            // Switch the sub period the program is in
            currentSubPeriod = ON;
            handleOnSubPeriod();
        } else {
            // Switch the sub period the program is in
            currentSubPeriod = OFF;
            handleOffSubPeriod();
        }
    }
}

/**
 * Main function for the program
 */
int main(void)
{
    // Set the values for the heating period
    heatingPeriod.led_id = 6;      // Red LED
    heatingPeriod.character = 'H';
    heatingPeriod.delays[0] = 400; // OFF delay
    heatingPeriod.delays[1] = 600; // ON delay

    // Set the values for the cooling period
    coolingPeriod.led_id = 7;      // Green LED
    coolingPeriod.character = 'C';
    coolingPeriod.delays[0] = 500; // OFF delay
    coolingPeriod.delays[1] = 500; // ON delay

    led_setup();
    uart_setup(&uart_callback, 115200, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
    timer_setup(&timer_callback, 0);
    PRCMSleep();
	return 0;
}
