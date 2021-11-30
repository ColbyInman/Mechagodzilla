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
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <driverlib/uart.h>
#include "driverlib/adc.h"
#include "string.h"
#include <driverlib/timer.h>



void Sensing_Init(void);
void frontDistPrnt(void);
void rightDistPrnt(void);
int IRDistanceCollect(int pin);
void IRDistanceDisplay(void);
void Uturn(void);
void PID(UArg arg0, UArg arg1);



#endif /* SENSING_H_ */
