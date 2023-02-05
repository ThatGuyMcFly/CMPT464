/**
 * CMPT464 Lab 4 Exercise 1
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

typedef enum {SET_INTERRUPT_ENABLED, SET_INTERRUPT_DISABLED} SET_INTERRUPT;

void uart_send_data(char);
void uart_setup(void(*)(char*), uint32_t, uint32_t, uint32_t);

#endif /* UART_MODULE_H_ */
