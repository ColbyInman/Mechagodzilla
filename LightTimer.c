/*
 * LightTimer.c
 *
 *  Created on: Nov 3, 2021
 *      Author: colby
 */

#define STOP_DIVIDER 5
#define ERROR_DIVIDER 10

#include "LightTimer.h"
#include "sensing.h"
extern double IRdist;

uint32_t startTime, endTime, pinValue;

void identify_color(void) {

   HWREG(TIMER2_BASE + TIMER_O_TAV) = 0;                       // Set Timer2 to 0 to start timing
   GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_1);         // Make PortD pin1 output to charge the sensor
   GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_PIN_1);      // Start charing the sensor
   SysCtlDelay(SysCtlClockGet()/83333);// Wait to finish charging

   startTime = TimerValueGet(TIMER2_BASE, TIMER_A);            // Capture startTime
   GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_1);          // Make PortD pin1 input to time the decaying
   pinValue = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_1);        // Assign the value when sensor is in fully charged state
   while (pinValue & GPIO_PIN_1) {                             // Loop to compare to the current state to fully-charged state
       pinValue = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_1);
   }
   endTime = TimerValueGet(TIMER2_BASE, TIMER_A);              // Capture endTime when fully decayed
   if((endTime - startTime) <= 7000){                           // Above black surface if discharge time > 7000 microseconds
       LightTimerReload();
   }
}

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
        IRDistanceDisplay(IRdist);
    }
    TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet()/STOP_DIVIDER);
}
