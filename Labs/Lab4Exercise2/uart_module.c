/**
 * CMPT464 Lab 4 Exercise 2
 * Martin Knoetze
 * SN: 3086754
 *
 * uart_module.c
 */

#include "uart_module.h"

#define CLOCK 48000000
#define NULL 0x00000000

void(*uart_callback_function)(int32_t) = NULL;

/**
 * Sends an array of characters to be transmitted
 *
 * Parameters
 *
 *      characters - the array of characters to be transmitted
 */
void uart_send_data(char * characters) {

    int index = 0;

    while(characters[index] != '\0'){
        UARTCharPut(UART0_BASE, (uint8_t) characters[index]);
        index++;
    }
}

/**
 * Function for handling the uart interruption and calling the callback functions
 */
void uart_interrupt_handler(){
    int32_t ch;

    if (UARTIntStatus(UART0_BASE, true) != UART_INT_RX) return;

    UARTIntClear (UART0_BASE,UART_INT_RX|UART_INT_TX);

    while(UARTCharsAvail(UART0_BASE)){
        ch = UARTCharGetNonBlocking(UART0_BASE) & 0x000000FF;

        if(uart_callback_function != NULL) {
            uart_callback_function(ch);
        }
    }
}

/**
 * Setup function for the UART Module
 *
 * Parameters
 *
 *      passedInCallback - the function to be called in the interrupt handler. This function needs to take a signed 32 bit integer as a parameter.
 *
 *      baudRate - an unsigned 32 bit integer to set the baud rate
 */
void uart_setup(void(*passedInCallback)(int32_t), uint32_t baudRate) {
    uart_callback_function = passedInCallback;

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
    UARTFIFOLevelSet (UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);

    // UART interrupt handler assignment
    UARTIntRegister(UART0_BASE, uart_interrupt_handler);

    // Enable Interrupts
    UARTIntEnable(UART0_BASE, UART_INT_RX);

    // Enable the UART
    UARTEnable(UART0_BASE);
}
