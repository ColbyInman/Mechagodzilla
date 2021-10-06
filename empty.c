/*
 *  ======= main ========
 *
 *  Created on: Sep 29, 2021
 *  Author:     MechaGodzilla Team
 *
 */

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

#include "empty.h"

// Set the baud rate
#define UART_BAUDRATE 115200
/*
typedef void (*Actions)(void);

typedef struct _tagUserCommand
{
   char            *Command;
   Actions act;
} UserCommand;
*/
/*
void set(void);
void (*set_ptr)(void) = set;
void dist(void);
void (*dist_ptr)(void) = dist;
void rLED(void);
void (*rLED_ptr)(void) = rLED;
void gLED(void);
void (*gLED_ptr)(void) = gLED;
void bLED(void);
void (*bLED_ptr)(void) = bLED;
void wLED(void);
void (*wLED_ptr)(void) = wLED;
void offLED(void);
void (*offLED_ptr)(void) = offLED;
void frontDistPrnt(void);
void (*frontDistPrnt_ptr)(void) = frontDistPrnt;
void rightDistPrnt(void);
void (*rightDistPrnt_ptr)(void) = rightDistPrnt;
void run(void);
void (*run_ptr)(void) = run;
void reverse(void);
void (*reverse_ptr)(void) = reverse;
void uturn(void);
void (*uturn_ptr)(void) = uturn;
void stop(void);
void (*stop_ptr)(void) = stop;
*/

struct UserCommand arr_cmd[] = {
		{"AX",set},
		{"BX",dist},
		{"CX",rLED},
		{"DX",gLED},
		{"EX",bLED},
		{"FX",wLED},
		{"GX",offLED},
		{"HX",frontDistPrnt},
		{"IX",rightDistPrnt},
		{"JX",run},
		{"KX",reverse},
		{"LX",uturn},
		{"MX",stop}
};


// *** Board Initialization Function ***
void Board_Init() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    GPIOPinConfigure(GPIO_PE4_U5RX);
    GPIOPinConfigure(GPIO_PE5_U5TX);
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), UART_BAUDRATE,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTEnable(UART5_BASE);
    UARTIntEnable(UART5_BASE, UART_INT_RX | UART_INT_RT);
}

void rLED(void)
{
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x2);
}

void gLED(void)
{
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x8);
}

void bLED(void)
{
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x4);
}
void wLED(void)
{
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0xE);
}

void offLED(void)
{
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x0);
}

// ======== main ========

char instructions[2];
int main(void)
{
    Board_Init();

    while(1) {
        // Set all three GPIO LED pins on or off at the same time
        //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0xC);

        //SysCtlDelay((10-num)*800000);

        //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);

        //SysCtlDelay((10-num)*800000);

    }
}
void UART_Read()
{
	char tempChar;
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART5_BASE, true);
	int i;
	//char BlinkingAt[12] = "Blinking at ";
	//char hertz[6] = " hertz";
	UARTIntClear(UART5_BASE, ui32Status);
	tempChar = UARTCharGet(UART5_BASE);
	UARTCharPutNonBlocking(UART5_BASE, tempChar);
	if (!(tempChar-13))
	{
		//Do the commands
		for(i = 0; i < 13; i++)
		{
			if(arr_cmd[i].Command[0] == instructions[0] && arr_cmd[i].Command[1] == instructions[1])
			{
				arr_cmd[i].act();
			}
		}
	}
	else
	{
		instructions[0] = instructions[1];
		instructions[1] = tempChar;
	}
}
