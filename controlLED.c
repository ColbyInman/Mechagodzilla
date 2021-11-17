/*
 * controlLED.c
 *
 *  Created on: Oct 19, 2021
 *      Author: Mechagodzilla
 */

#include "controlLED.h"
#include "LightTimer.h"

void LED_Init(void)
{
    //Enable for LEDs
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    //Set LED pins to output
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_6|GPIO_PIN_7);
}

void rLED(void)
{//turns on rLED and off the rest of the LEDs
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x2);
}

void gLED(void)
{//turns on gLED and off the rest of the LEDs
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x8);
}

void bLED(void)
{//turns on bLED and off the rest of the LEDs
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x4);
}
void wLED(void)
{//turns on all the LEDs
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0xE);
}

void offLED(void)
{//turns off all of the LEDs
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x0);
}

void dataCollect(void)
{
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_PIN_6);
}

void endCollect(void)
{
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0x0);
}

void sendData(void)
{
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_7, GPIO_PIN_7);
}

void endSend(void)
{
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_7, 0x0);
}
