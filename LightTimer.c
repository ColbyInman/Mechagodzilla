/*
 * LightTimer.c
 *
 *  Created on: Nov 3, 2021
 *      Author: colby
 */

#define STOP_DIVIDER 5
#define ERROR_DIVIDER 10

#include "LightTimer.h"



void LightTimer_Init(void)
{
    //Light sensor configure
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    //Set LED pins to output
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_1);

    //Timer Configure
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1))
    {
    }

    TimerConfigure(TIMER1_BASE, TIMER_CFG_ONE_SHOT);
    TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet()/STOP_DIVIDER);
    TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
    TimerIntEnable(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER1_BASE, TIMER_A);
}

void LightTimerReload(void)
{
    if(TimerValueGet(TIMER1_BASE, TIMER_A) < SysCtlClockGet()/ERROR_DIVIDER)
    {
        //sensing error vals
    }
    TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet()/STOP_DIVIDER);
}
