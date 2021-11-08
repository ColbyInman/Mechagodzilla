/*
 * LightTimer.h
 *
 *  Created on: Nov 3, 2021
 *      Author: colby
 */

#ifndef LIGHTTIMER_H_
#define LIGHTTIMER_H_
#include <inc/hw_memmap.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <driverlib/sysctl.h>
#include <driverlib/timer.h>
#include <driverlib/gpio.h>

void LightTimer_Init(void);
void LightTimerReload(void);
void identify_color(void);


#endif /* LIGHTTIMER_H_ */
