/*
 * uartRW.c
 *
 *  Created on: Nov 3, 2021
 *      Author: colby
 */
#include "uartRW.h"

void UART_Init(void)
{
    //Enable for UART pins
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        //Enable for UART controller
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
        //Set PE4 to RX, PE5 to TX
        GPIOPinConfigure(GPIO_PE4_U5RX);
        GPIOPinConfigure(GPIO_PE5_U5TX);
        GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
        UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), UART_BAUDRATE,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

        UARTEnable(UART5_BASE);
        UARTIntEnable(UART5_BASE, UART_INT_RX | UART_INT_RT);
}
