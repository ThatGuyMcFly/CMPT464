/**
 * CMPT464 Assignment 1
 * Martin Knoetze
 * SN: 3086754
 *
 * main.c
 */

#include "led_module.h"
#include "uart_module.h"
#include "timer_module.h"

int tCurrent = 0;
int tChosen = 0;

typedef enum {ON, OFF} SUB_PERIOD;

typedef struct {
    int led_id;
    char character;
    int delays[2];
    SUB_PERIOD currentSubPeriod;
} period;

period heatingPeriod;
period coolingPeriod;

period * currentPeriodPtr;

//void active_mode() {
//    TimerIntEnable(GPT0_BASE,TIMER_TIMA_TIMEOUT);
//}

//void sleep_mode() {
//    TimerIntDisable(GPT0_BASE,TIMER_TIMA_TIMEOUT);
//}

/**
 * The callback functions to be called by the uart interrupt handler
 *
 * parameters
 *
 *      characters - string of characters sent from the uart interrupt function
 */
void uart_callback(char * characters) {

    tChosen = (int)(characters[0] - '0') * 10;
    tChosen += (int)(characters[1] - '0');

    if(tChosen >= 20 && tChosen <= 30) {

        if(tCurrent < tChosen) {
            currentPeriodPtr = &heatingPeriod;
        } else if (tCurrent > tChosen) {
            currentPeriodPtr = &coolingPeriod;
        } else {
            return;
        }

        currentPeriodPtr->currentSubPeriod = ON;

        led_turn_on(currentPeriodPtr->led_id);

        uart_send_data(currentPeriodPtr->character);

        timer_enable_interrupt(currentPeriodPtr->delays[currentPeriodPtr->currentSubPeriod]);

    }

}

/**
 * The callback function to be called by the timer interrupt function
 */
void timer_callback() {

    if (currentPeriodPtr->currentSubPeriod == ON) {
        UARTInterruptEnable()
    }

}

int main(void)
{
    heatingPeriod.led_id = 6;
    heatingPeriod.character = 'H';
    heatingPeriod.delays[0] = 600;
    heatingPeriod.delays[1] = 400;
    heatingPeriod.currentSubPeriod = OFF;

    coolingPeriod.led_id = 6;
    coolingPeriod.character = 'C';
    coolingPeriod.delays[0] = 500;
    coolingPeriod.delays[1] = 500;
    coolingPeriod.currentSubPeriod = OFF;

    led_setup();
    uart_setup(&uart_callback, 115200, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
    timer_setup(&timer_callback, 0);
    PRCMSleep();
	return 0;
}
