/**
 * CMPT464 Lab 4 Exercise 2
 * Martin Knoetze
 * SN: 3086754
 *
 * uart_module.c
 */

#include "uart_module.h"
#include <stdio.h>
#include <stdlib.h>

#define CLOCK 48000000

void(*uart_callback_function)(char *) = NULL;

uint32_t txTriggerLevel;

uint32_t rxTriggerLevel;

const size_t rxSize[5] = {4, 8, 16, 24, 28};

/**
 * Function for handling the uart interruption and calling the callback functions
 */
void uart_rx_interrupt_handler(){

    // Ensure the interrupt was caused by a receive interrupt
    if (UARTIntStatus(UART0_BASE, true) != UART_INT_RX) return;

    // clear the uart interrupt
    UARTIntClear (UART0_BASE,UART_INT_RX|UART_INT_TX);

    // create a size equal to the number of characters that will trigger the interrupt plus one for the null terminator
    size_t size = rxSize[rxTriggerLevel >> 3] + 1;

    // Allocate the required memory for a character array of size
    char * characters = (char *) malloc(size);

    // Read in the received characters and terminate array with null
    for (int i = 0; i < size; i++) {
        if(UARTCharsAvail(UART0_BASE)) {
            characters[i] = (char)(UARTCharGetNonBlocking(UART0_BASE) & 0x000000FF);
        } else {
            characters[i] = 0x0;
            break;
        }
    }

    // If there is a callback function, send the character array to the function
    if(uart_callback_function != NULL) {
        uart_callback_function(characters);
    }

    // Free the memory allocated for the character array
    free(characters);
}

/**
 * Transmits a string of characters
 *
 * Parameters
 *
 *      characters - the string to be transmitted
 */
void uart_send_data(char * characters) {

    int index = 0;

    while(characters[index] != '\0'){
        UARTCharPut(UART0_BASE, (uint8_t) characters[index]);
        index++;
    }
}

/**
 * Set whether the uart interrupt is enabled
 *
 *      setInterrupt - a SET_INTERRUPT value for whether to enable the interrupt
 */
void uart_set_interrupt(SET_INTERRUPT setInterrupt){

    if (setInterrupt == SET_INTERRUPT_ENABLED) {
        // Enable Interrupts
        UARTIntEnable(UART0_BASE, UART_INT_RX);
    } else if (setInterrupt == SET_INTERRUPT_DISABLED) {
        // Enable Interrupts
        UARTIntDisable(UART0_BASE, UART_INT_RX);
    }

}

/**
 * Setup function for the UART Module
 *
 * Parameters
 *
 *      passedInCallback - the function to be called in the interrupt handler. This function needs to take a char pointer as a parameter
 *
 *      baudRate - an unsigned 32 bit integer to set the baud rate
 *
 *      txFifoTriggerLevel - an unsigned 32 bit integer to set the transmit trigger level
 *                           ref uart.h - UART_FIFO_TX1_8
 *                           ref uart.h - UART_FIFO_TX2_8
 *                           ref uart.h - UART_FIFO_TX4_8
 *                           ref uart.h - UART_FIFO_TX6_8
 *                           ref uart.h - UART_FIFO_TX7_8
 *
 *      rxFifoTriggerLevel - an unsigned 32 bit integer to set the receive trigger level
 *                           ref uart.h - UART_FIFO_RX1_8
 *                           ref uart.h - UART_FIFO_RX2_8
 *                           ref uart.h - UART_FIFO_RX4_8
 *                           ref uart.h - UART_FIFO_RX6_8
 *                           ref uart.h - UART_FIFO_RX7_8
 */
void uart_setup(void(*passed_in_callback)(char *), uint32_t baudRate, uint32_t txFifoTriggerLevel, uint32_t rxFifoTriggerLevel) {
    uart_callback_function = passed_in_callback;
    txTriggerLevel = txFifoTriggerLevel;
    rxTriggerLevel = rxFifoTriggerLevel;

    PRCMPowerDomainOn(PRCM_DOMAIN_SERIAL);
    while(PRCMPowerDomainStatus(PRCM_DOMAIN_SERIAL)!= PRCM_DOMAIN_POWER_ON);

    // Set the UART to be active in run and sleep mode
    PRCMPeripheralRunEnable(PRCM_PERIPH_UART0);
    PRCMPeripheralSleepEnable(PRCM_PERIPH_UART0);

    PRCMLoadSet();
    while(!PRCMLoadGet());

    // Set and enable the UART pins
    IOCPinTypeUart(UART0_BASE, IOID_2, IOID_3, IOID_UNUSED, IOID_UNUSED);

    // Disable the UART
    UARTDisable(UART0_BASE);

    uint32_t config = UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE;

    // Set the UART configuration
    UARTConfigSetExpClk(UART0_BASE, CLOCK, baudRate, config);

    // Disable hardware flow control
    UARTHwFlowControlDisable(UART0_BASE);

    // Set FIFO Thresholds
    UARTFIFOLevelSet (UART0_BASE, txFifoTriggerLevel, rxFifoTriggerLevel);

    // UART interrupt handler assignment
    UARTIntRegister(UART0_BASE, uart_rx_interrupt_handler);

    // Enable Interrupts
    UARTIntEnable(UART0_BASE, UART_INT_RX);

    // Enable the UART
    UARTEnable(UART0_BASE);
}
