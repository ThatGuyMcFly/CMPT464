/**
 * CMPT464 Lab 4 Exercise 1
 * Martin Knoetze
 * SN: 3086754
 *
 * adc_module.c
 */

#include "adc_module.h"

#define MICRO_VOLT 10000

/**
 * Reads the temperature from the ADC
 */
uint32_t adc_read_temperature() {

    // Manually trigger the ADC
    AUXADCGenManualTrigger();

    // Wait for the ADC FIFO to not be empty
    while(AUXADCGetFifoStatus() == AUXADC_FIFO_EMPTY_M);

    // Get the value from the FIFO and unadjust the value
    uint32_t fifoValue = AUXADCReadFifo();
    uint32_t gain = AUXADCGetAdjustmentGain(AUXADC_REF_FIXED);
    uint32_t offset = AUXADCGetAdjustmentOffset(AUXADC_REF_FIXED);
    uint32_t unadjustedValue = AUXADCUnadjustValueForGainAndOffset(fifoValue, gain, offset);

    // Covert the unadjusted value into to micro volts
    uint32_t microVolts = AUXADCValueToMicrovolts(AUXADC_FIXED_REF_VOLTAGE_NORMAL, unadjustedValue);

    // Covert the micro volts to celcius
    return microVolts / MICRO_VOLT;
}

/**
 * Sets up the ADC using the DIO 29 pin
 */
void adc_setup()
{
    AUXWUCClockEnable(AUX_WUC_ANAIF_CLOCK);
    AUXWUCClockEnable(AUX_WUC_ADI_CLOCK);
    AUXWUCClockEnable(AUX_WUC_ADC_CLOCK);
    while(AUXWUCClockStatus(AUX_WUC_ADC_CLOCK)==AUX_WUC_CLOCK_OFF);

    IOCPinTypeAux(IOID_29);
    AUXADCSelectInput(ADC_COMPB_IN_AUXIO1);

    AUXADCEnableSync(AUXADC_REF_FIXED,AUXADC_SAMPLE_TIME_1P37_MS,AUXADC_TRIGGER_MANUAL);
}



