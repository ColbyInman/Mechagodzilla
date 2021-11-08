/*
 * movement.h
 *
 *  Created on: Oct 13, 2021
 *      Author: Mechagodzilla
 */

#include <inc/hw_memmap.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include "driverlib/pwm.h"
#include <driverlib/timer.h>

#define SETPOINT 2500

#ifndef MOVEMENT_H
#define MOVEMENT_H

//movement function definitions
void Movement_Init(void);
void setSpeed(int duty);
void slowSpeed(void);
void mediumSpeed(void);
void fastSpeed(void);
void goForward(void);
void turnRight(void);
void reverse(void);
void uturn(void);
void stop(void);
void CalculateSpeed(uint16_t PID, double R_Error);

#endif
