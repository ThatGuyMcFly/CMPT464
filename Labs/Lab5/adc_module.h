/**
 * CMPT464 Lab 5
 * Martin Knoetze
 * SN: 3086754
 *
 * adc_module.h
 */

#ifndef ADC_MODULE_H_
#define ADC_MODULE_H_

#include <stdlib.h>
#include "driverlib/ioc.h"
#include "driverlib/uart.h"
#include "driverlib/prcm.h"
#include "driverlib/timer.h"
#include "driverlib/sys_ctrl.h"
#include "inc/hw_memmap.h"
#include "driverlib/osc.h"
#include "driverlib/aux_adc.h"
#include "inc/hw_aux_wuc.h"

/**
 * Reads the temperature from the ADC
 */
uint32_t adc_read_temperature();

/**
 * Sets up the ADC using the DIO 29 pin
 */
void adc_setup();

#endif /* ADC_MODULE_H_ */
