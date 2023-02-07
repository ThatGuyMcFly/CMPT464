/**
 * CMPT464 Lab 1
 * Martin Knoetze
 * SN: 3086754
 *
 * button_display.c
 */

#include "button_display.h"

#define MS 16000 // Number of calls to CPUdelay for 1 millisecond

/**
 * Setups up the LED outputs
 */
void output_setup()
{
    // Enable Peripheral power domain
    PRCMPowerDomainOn(PRCM_DOMAIN_PERIPH);
    while(PRCMPowerDomainStatus(PRCM_DOMAIN_PERIPH) != PRCM_DOMAIN_POWER_ON);

    // Enable GIO peripheral
    PRCMPeripheralRunEnable(PRCM_PERIPH_GPIO);
    PRCMPeripheralSleepEnable(PRCM_PERIPH_GPIO);

    // Make sure that the configurations are propagated
    PRCMLoadSet();
    while ( !PRCMLoadGet() );

    // Set the GIO configuration
    IOCPinTypeGpioOutput(IOID_6);
    IOCPinTypeGpioOutput(IOID_7);

    AONBatMonEnable();
}

/**
 * Flashes the green and red LEDs depending on the value and number of bits to be displayed
 */
void display_information(uint32_t value, int number_of_bits)
{
    // Start with a mask of 1 and shift it to where it needs to go
    // depending on the number of bits needed
    uint32_t mask = 0x00000001;
    mask = mask << number_of_bits - 1;

    // Ensure Red and Green LEDs are off before start
    HWREG(GPIO_BASE+GPIO_O_DOUT7_4) &= 0xFEFEFFFF;

    // Iterate over the bits and check them against the mask
    // and toggle the appropriate LED
    for(int i = 0; i < number_of_bits; i ++) {
        uint32_t result = value & mask;

        if (result == mask) {
            // Toggle the green LED for 1
            GPIO_toggleDio(7);
            CPUdelay(300*MS);
            GPIO_toggleDio(7);
        } else if (result == 0){
            // Toggle the red LED for 0
            GPIO_toggleDio(6);
            CPUdelay(300*MS);
            GPIO_toggleDio(6);
        }

        value = value << 1;
        CPUdelay(200*MS);
    }

}

/**
 * The function to call when an interupt is detected
 */
void fn()
{
    // Disable the interrupts to avoid interrupting an in process interrupt
    IOCIntDisable(IOID_13);
    IOCIntDisable(IOID_14);

    if(IOCIntStatus(IOID_13) == 1){
        // Button 1 is for voltage reading
        display_information(AONBatMonBatteryVoltageGet(), 11);
    } else if (IOCIntStatus(IOID_14) == 1) {
        // Button 2 is for temperature reading
        display_information(AONBatMonTemperatureGetDegC(), 16);
    }

    IOCIntClear(IOID_13);
    IOCIntClear(IOID_14);

    IOCIntEnable(IOID_13);
    IOCIntEnable(IOID_14);
}

/**
 * Setups the input buttons
 */
void input_setup()
{
    IOCPinTypeGpioInput(IOID_13);

    IOCIOPortPullSet(IOID_13, IOC_IOPULL_UP);
    IOCIOHystSet   (IOID_13,IOC_HYST_ENABLE);

    IOCIntClear(IOID_13);
    IOCIntRegister(fn);

    IOCIOIntSet(IOID_13,IOC_INT_ENABLE, IOC_FALLING_EDGE);

    IOCPinTypeGpioInput(IOID_14);

    IOCIOPortPullSet(IOID_14, IOC_IOPULL_UP);
    IOCIOHystSet   (IOID_14,IOC_HYST_ENABLE);

    IOCIntClear(IOID_14);
    IOCIntRegister(fn);

    IOCIOIntSet(IOID_14,IOC_INT_ENABLE, IOC_FALLING_EDGE);

    IOCIntEnable(IOID_13);
    IOCIntEnable(IOID_14);
}

void setup_buttons()
{
    output_setup();
    input_setup();
}
