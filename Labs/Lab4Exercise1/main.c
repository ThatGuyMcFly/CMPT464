/**
 * CMPT464 Lab 4 Exercise 1
 * Martin Knoetze
 * SN: 3086754
 *
 * main.c
 */

#include "timer_module.h"
#include "uart_module.h"
#include "driverlib/aon_batmon.h"

/**
 * Takes a 2 digit number, converts the digits to characters and puts them into the
 * the first and second indices in of the provided array of characters
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
 *
 */
void timerCallback(void *ptr) {
    uint32_t temperature = AONBatMonTemperatureGetDegC();

    char characters[] = "   C\n\r";

    convert_to_char(temperature, characters);

    uart_send_data(characters);
}

int main(void) {
    AONBatMonEnable();
    uart_setup(9600);
    timer_setup(&timerCallback, 5000);
    PRCMSleep();
	return 0;
}
