/*
 * controlLED.c
 *
 *  Created on: Oct 19, 2021
 *      Author: Mechagodzilla
 */

#include "controlLED.h"

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
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x0);
}
