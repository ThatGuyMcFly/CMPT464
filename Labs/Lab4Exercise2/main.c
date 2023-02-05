/**
 * CMPT464 Lab 4 Exercise 2
 * Martin Knoetze
 * SN: 3086754
 *
 * main.c
 */


#include "timer_module.h"
#include "uart_module.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 10

struct loop_buffer {
    int in;
    int out;
    int size;
    char characters[BUFFER_SIZE];
};

struct loop_buffer my_buffer;

/**
 * The function to be called by the uart module when a uart interrupt is triggered
 *
 * parameters
 *      character - a signed 32 bit integer passed in from the uart interrupt
 */
void uart_callback(int32_t character) {

    if (my_buffer.size != BUFFER_SIZE) {
        my_buffer.characters[my_buffer.in] = (char) character;
        my_buffer.in = (my_buffer.in + 1) % BUFFER_SIZE;
        my_buffer.size++;
    }
}

/**
 * The function to be called by the timer module when a timer interrupt is triggered
 */
void timer_callback() {
    while (my_buffer.out != my_buffer.in || my_buffer.size == BUFFER_SIZE) {
        // Allocate one char's worth of memory to be and assign it the current out character
        char * character = (char *)malloc(1);
        *character = my_buffer.characters[my_buffer.out];

        // Update the index to which the out pointer is pointing to
        my_buffer.out = (my_buffer.out + 1) % BUFFER_SIZE;
        my_buffer.size--;

        // Send the character to be transmitted
        uart_send_data(character);

        // free the memory allocated for the character
        free(character);
    }
}

int main(void) {
    // initialize the values loop buffer
    my_buffer.in = 0;
    my_buffer.out = 0;
    my_buffer.size = 0;

    // initialize the uart and timer modules
    uart_setup(&uart_callback, 9600);
    timer_setup(&timer_callback, 5000);

    PRCMSleep();

	return 0;
}
