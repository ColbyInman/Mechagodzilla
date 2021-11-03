/*
 * empty.h
 *
 *  Created on: Oct 3, 2021
 *      Author: Mechagodzilla
 */

#ifndef MAIN_H_
#define MAIN_H_

/* Standard C Libraries */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Constants */
// This one gives all the GPIO_PORTx_BASE variables and the UARTx_BASE variables
#include <inc/hw_memmap.h>

/* DriverLib */
// All the SysCtl Functions
#include <driverlib/sysctl.h>
// Adds more constants like GPIO_PA0_U5RX and GPIO_PA0_U5TX for configuring pins
#include <driverlib/pin_map.h>
// GPIO Peripheral Driver Library
#include <driverlib/gpio.h>
// UART Peripheral Driver Library
#include <driverlib/uart.h>
// Hardware interrupt handler Library
#include <inc/hw_ints.h>
#include "driverlib/interrupt.h"
#include "driverlib/pwm.h"
#include <driverlib/timer.h>

#include "math.h"

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include "movement.h"
#include "sensing.h"
#include "controlLED.h"
#include "uartRW.h"
#include "LightTimer.h"
//#include "WatchdogSensing.h"

typedef void (*Actions)(void);

typedef struct UserCommand
{
   char *Command;
   Actions act;
}UserCommand_t;

//basic function definitions
void set(void){}
void dist(void){}
void InputFunction(void);
void PID();

#endif /* MAIN_H_ */
