/*
 *  ======= main ========
 *
 *  Created on: Sep 29, 2021
 *  Author:     MechaGodzilla
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
#include "driverlib/pwm.h"
#include <driverlib/timer.h>


#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include "main.h"
#include "movement.h"
#include "sensing.h"
#include "controlLED.h"

// Set the baud rate
#define UART_BAUDRATE 115200

#define PWM_TICKS_IN_PERIOD 3000 // defines full duty cycle period for pwm (set at 3000 to be same scale as ADC)
#define SETPOINT 2500
#define P_MULT 0.1
#define D_MULT 0.05

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
        {"JX",slowSpeed},       //movement.c
        {"KX",mediumSpeed},     //movement.c
        {"LX",fastSpeed},       //movement.c
        {"MX",goForward},       //movement.c
        {"NX",turnRight},       //movement.c
        {"OX",uturn},           //movement.c
        {"PX",stop}             //movement.c
};

uint32_t val_load, pwm_clk, adcVal;

// *** Board Initialization Function ***
void Board_Init() {
    //Enable for LEDs
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //Enable for UART pins
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    //Enable for PWM pins
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    //Enable for UART controller
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
    //Set LED pins to output
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    //Set PE4 to RX, PE5 to TX
    GPIOPinConfigure(GPIO_PE4_U5RX);
    GPIOPinConfigure(GPIO_PE5_U5TX);
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    //Set PWM for motor control (PB4 is right and PB5 is left)
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);
    GPIOPinConfigure(GPIO_PB5_M0PWM3);
    GPIOPinConfigure(GPIO_PB4_M0PWM2);

    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    //SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
    UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), UART_BAUDRATE,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTEnable(UART5_BASE);
    UARTIntEnable(UART5_BASE, UART_INT_RX | UART_INT_RT);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

    //Clock divider for PWM
    pwm_clk = SysCtlClockGet() / 64;
    val_load = (pwm_clk / 100) - 1;

    //Configures PWM base and generator
    PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, val_load);

    //Enables PWM base and generator
    PWMGenEnable(PWM0_BASE, PWM_GEN_1);

    //Sets PWM state to off by default
    PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, false);
    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, false);

    //*******************Config For ADC*********************************************
    // Enable ADC0 module
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    // configure PE3 (front) and PE2 for input (right)
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
    // Configure sample sequencer
    ADCSequenceDisable(ADC0_BASE, 3);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);

    // Enable ADC1 module
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
    // configure PE3 (front) and PE2 for input (right)
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);
    // Configure sample sequencer
    ADCSequenceDisable(ADC1_BASE, 3);
    ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC1_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC1_BASE, 3);

    //*******************Config For Timer*****************************************
    //Enable Timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerEnable(TIMER0_BASE,TIMER_A);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/20);
   // TimerIntRegister(TIMER0_BASE, TIMER_A, PID);
    TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
    TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
    IntEnable(INT_TIMER0A);
    TimerEnable(TIMER0_BASE, TIMER_A);

}

// ======== main ========

char instructions[2];
int main(void)
{
    Board_Init();

    BIOS_start();
    return(0);
}
void UART_Read()
{
    char tempChar;
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART5_BASE, true);
    //char BlinkingAt[12] = "Blinking at ";
    //char hertz[6] = " hertz";
    UARTIntClear(UART5_BASE, ui32Status);
    tempChar = UARTCharGet(UART5_BASE);
    UARTCharPutNonBlocking(UART5_BASE, tempChar);
    if (!(tempChar-13))
    {
        //Do the commands
        InputFunction();
    }
    else
    {
        instructions[0] = instructions[1];
        instructions[1] = tempChar;
    }
}
bool toggle = false;
void PID(void)
{
    TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
//    IRDistanceCollect(2);
//
//    convertedADCVal = PWM_TICKS_IN_PERIOD - ui32ADCAvg; // Do this bc PWM goes up, while ADC reads go down (voltage decreases as LED gets brighter)
//    errorCurr = (SETPOINT - ui32ADCAvg);
//
//    P = P_MULT * errorCurr;
//
//    diff = errorCurr - errorPrev;
//    errorPrev = errorCurr;
//    D = D_MULT * diff;  // Note, not using time as a simplification since it should be consistent
//
//    pulseWidth = (uint16_t)(P + D);
//
//    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, pulseWidth);         // Set duty cycle to pulseWidth/400



    if (toggle){
        rLED();
        toggle = false;
    }
    else{
        toggle = true;
        offLED();
    }
}

void InputFunction(void)
{
    int i;

    for(i = 0; i < sizeof(arr_cmd)/sizeof(UserCommand_t); i++)
    {
        if(arr_cmd[i].Command[0] == instructions[0] && arr_cmd[i].Command[1] == instructions[1])
        {
            arr_cmd[i].act();
            break;
        }
    }
}


