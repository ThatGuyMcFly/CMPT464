/**
 * CMPT464 Lab 1
 * Martin Knoetze
 * SN: 3086754
 */

#include "inc/hw_memmap.h"
#include "inc/hw_wdt.h"
#include "inc/hw_gpio.h"
#include "driverlib/watchdog.h"
#include "driverlib/prcm.h"
#include "driverlib/ioc.h"
#include "driverlib/cpu.h"

#define MS 16000 // Number of calls to CPUdelay for 1 millisecond

/**
 * main.c
 */
int main(void)
{
    // HWREG(WDT_BASE + WDT_0_CTL) &= 0xFFFFFFFD;
    WatchdogResetDisable();

    // Turn on GPIO domain
    PRCMPowerDomainOn(PRCM_DOMAIN_PERIPH);
    while(PRCMPowerDomainStatus(PRCM_DOMAIN_PERIPH)!= PRCM_DOMAIN_POWER_ON);

    PRCMPeripheralRunEnable(PRCM_PERIPH_GPIO);

    PRCMLoadSet();
    while(!PRCMLoadGet());

    // Set the Red and Green LEDs to receive output
    IOCPinTypeGpioOutput(IOID_6);
    IOCPinTypeGpioOutput(IOID_7);

    // Ensure Red and Green LEDs are off before start
    HWREG(GPIO_BASE+GPIO_O_DOUT7_4) &= 0xFEFEFFFF;

    while(1)
    {

        // Red LED off for 400ms
        HWREG(GPIO_BASE+GPIO_O_DOUT7_4) ^= GPIO_DOUT7_4_DIO6;
        CPUdelay(400*MS);

        // Red and Green LED off for 600ms
        HWREG(GPIO_BASE+GPIO_O_DOUT7_4) ^= GPIO_DOUT7_4_DIO6;
        CPUdelay(600*MS);

        // Green LED on for 2s
        HWREG(GPIO_BASE+GPIO_O_DOUT7_4) ^= GPIO_DOUT7_4_DIO7;
        CPUdelay(2000*MS);

        // Red and Green LED on for 1s
        HWREG(GPIO_BASE+GPIO_O_DOUT7_4) ^= GPIO_DOUT7_4_DIO7;
        CPUdelay(1000*MS);
    }

    return 0;
}
