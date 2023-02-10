/**
 * CMPT464 Lab 5
 * Martin Knoetze
 * SN: 3086754
 *
 * main.c
 */

#include "timer_module.h"
#include "uart_module.h"
#include "adc_module.h"

/**
 * Takes a 2 digit number, converts the digits to characters and puts them into the
 * the first and second indices in of the provided array of characters
 */
void convert_to_char(uint32_t data, char * characters) {

    // Breaks the 2 digit integer into it's individual digits
    int digit_1 = data / 10;
    int digit_2 = data - digit_1 * 10;

    // Converts the integer to its character equivalent
    char character1 = '0' + digit_1;
    char character2 = '0' + digit_2;

    // Puts the characters into the string
    characters[0] = character1;
    characters[1] = character2;
}

/**
 * Function to be called in the timer interrupt
 */
void timerCallback(void *ptr) {
    uint32_t temperature = adc_read_temperature();

    char characters[] = "   C\n\r";

    convert_to_char(temperature, characters);

    uart_send_data(characters);
}

/**
 * Main function that sets up the modules
 */
int main(void) {
    adc_setup();
    uart_setup(9600);
    timer_setup(&timerCallback, 2000);
    PRCMSleep();
	return 0;
}
