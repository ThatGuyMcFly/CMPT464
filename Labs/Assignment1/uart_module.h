/**
 * Martin Knoetze
 * SN: 3086754
 * CMPT464 Assignment 1
 * Due: February 22nd, 2023
 *
 * uart_module.h
 *
 * This is the header file for the UART module
 */

#ifndef UART_MODULE_H_
#define UART_MODULE_H_

#include "driverlib/prcm.h"
#include "driverlib/uart.h"
#include "driverlib/ioc.h"

typedef enum {SET_INTERRUPT_ENABLED, SET_INTERRUPT_DISABLED} SET_INTERRUPT;

/**
 * Transmits a string of characters
 *
 * Parameters
 *
 *      characters - the string to be transmitted
 */
void uart_send_data(char *);

/**
 * Set whether the uart interrupt is enabled
 *
 *      setInterrupt - a SET_INTERRUPT value for whether to enable the interrupt
 */
void uart_set_interrupt(SET_INTERRUPT);

/**
 * Setup function for the UART Module
 *
 * Parameters
 *
 *      passedInCallback - the function to be called in the interrupt handler. This function returns nothing and takes a char pointer as a parameter
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
void uart_setup(void(*)(char*), uint32_t, uint32_t, uint32_t);

#endif /* UART_MODULE_H_ */
