/*
 * uartRW.h
 *
 *  Created on: Nov 3, 2021
 *      Author: colby
 */

#ifndef UARTRW_H_
#define UARTRW_H_

#define UART_BAUDRATE 115200

#include <inc/hw_memmap.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <inc/hw_memmap.h>
#include <driverlib/pin_map.h>

void UART_Init(void);


#endif /* UARTRW_H_ */
