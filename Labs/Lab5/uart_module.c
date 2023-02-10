/**
 * CMPT464 Lab 5
 * Martin Knoetze
 * SN: 3086754
 *
 * uart_module.c
 */

#include "uart_module.h"

#define CLOCK 48000000


/**
 * Transmits a string of characters
 */
void uart_send_data(char * characters) {

    int index = 0;

    while(characters[index] != '\0'){
        UARTCharPut(UART0_BASE, (uint8_t) characters[index]);
        index++;
    }
}

/**
 * Sets up the UART Module
 */
void uart_setup(uint32_t baudRate) {
    PRCMPowerDomainOn(PRCM_DOMAIN_SERIAL);
    while(PRCMPowerDomainStatus(PRCM_DOMAIN_SERIAL)!= PRCM_DOMAIN_POWER_ON);

    // Set timer to be active in run and sleep mode
    PRCMPeripheralRunEnable(PRCM_PERIPH_UART0);
    PRCMPeripheralSleepEnable(PRCM_PERIPH_UART0);

    PRCMLoadSet();
    while(!PRCMLoadGet());

    IOCPinTypeUart(UART0_BASE, IOID_2, IOID_3, IOID_UNUSED, IOID_UNUSED);

    UARTDisable(UART0_BASE);

    uint32_t config = UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE;

    UARTConfigSetExpClk(UART0_BASE, CLOCK, baudRate, config);

    UARTHwFlowControlDisable(UART0_BASE);

    UARTEnable(UART0_BASE);
}
