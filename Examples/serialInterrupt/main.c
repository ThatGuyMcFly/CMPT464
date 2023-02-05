/*
 * uart_interrupt.c
 *
 *  Created on: Jan 19, 2022
 *      Author: elmorsym
 */
#include "driverlib/ioc.h"
#include "driverlib/uart.h"
#include "driverlib/prcm.h"

// UART Interrupt
void UART_Interrupt_Handler()
{
    if(UARTIntStatus(UART0_BASE,  true) !=    UART_INT_RX) return;

    UARTIntClear    (UART0_BASE,UART_INT_RX|UART_INT_TX);

    while(UARTCharsAvail(UART0_BASE))
    {
        int32_t ch =   UARTCharGetNonBlocking(UART0_BASE) & 0x000000FF;
        UARTCharPut(UART0_BASE, (uint8_t) ch);
    }
}

void setup_Interrupt()
{
    // 1. Step 1 in page 1460
    PRCMPowerDomainOn(PRCM_DOMAIN_SERIAL);
    while(PRCMPowerDomainStatus(PRCM_DOMAIN_SERIAL) != PRCM_DOMAIN_POWER_ON);
    //
    PRCMPeripheralRunEnable(PRCM_PERIPH_UART0);
    PRCMPeripheralSleepEnable(PRCM_PERIPH_UART0);
    PRCMLoadSet();
    while ( !PRCMLoadGet() );

    // Enable UART Pins
    IOCPinTypeUart(UART0_BASE , IOID_2, IOID_3, IOID_19, IOID_18);

    // Disable UART
    UARTDisable(UART0_BASE);

    //UART Configuration
    UARTConfigSetExpClk(UART0_BASE,48000000,115200, UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE);
    //
    UARTHwFlowControlDisable(UART0_BASE);

    // Set FIFO Thresholds
    UARTFIFOLevelSet    (UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);

    // UART interrupt handler assignment
    UARTIntRegister(UART0_BASE,     UART_Interrupt_Handler);

    // Enable Interrupts
    UARTIntEnable(UART0_BASE , UART_INT_RX);

    //Last step
    UARTEnable(UART0_BASE);
}

int main(void)
{
    setup_Interrupt();
    PRCMSleep();
    return 0;
}

