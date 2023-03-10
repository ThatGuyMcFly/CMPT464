/**
 * CMPT464 Lab 5
 * Martin Knoetze
 * SN: 3086754
 *
 * uart_module.h
 */

#ifndef UART_MODULE_H_
#define UART_MODULE_H_

#include "driverlib/prcm.h"
#include "driverlib/uart.h"
#include "driverlib/ioc.h"

/**
 * Transmits a string of characters
 */
void uart_send_data(char*);

/**
 * Sets up the UART Module
 */
void uart_setup(uint32_t);

#endif /* UART_MODULE_H_ */
