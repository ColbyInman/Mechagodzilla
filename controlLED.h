/*
 * controlLED.h
 *
 *  Created on: Oct 19, 2021
 *      Author: Mechagodzilla
 */

#ifndef CONTROLLED_H_
#define CONTROLLED_H_

#include <inc/hw_memmap.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>

#include "main.h"

void LED_Init(void);

void rLED(void);

void gLED(void);

void bLED(void);

void wLED(void);

void offLED(void);

void dataCollect(void);

void endCollect(void);

void sendData(void);

void endSend(void);

#endif /* CONTROLLED_H_ */
