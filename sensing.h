/*
 * sensing.h
 *
 *  Created on: Oct 19, 2021
 *      Author: Mechagodzilla
 */

#ifndef SENSING_H_
#define SENSING_H_



#include <inc/hw_memmap.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>
#include "driverlib/adc.h"
#include "string.h"



void Sensing_Init(void);
void frontDistPrnt(void);
void rightDistPrnt(void);
int IRDistanceCollect(int pin);
void IRDistanceDisplay(int distance);
void Uturn(void);



#endif /* SENSING_H_ */
