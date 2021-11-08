/*
 *  ======= main ========
 *
 *  Created on: Sep 29, 2021
 *  Author:     MechaGodzilla
 *
 */

#include "main.h"

// Set the baud rate

#define SETPOINT 2500
#define P_MULT 0.08
#define D_MULT 0.05

struct UserCommand arr_cmd[] =
{
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

uint32_t adcVal;

// *** Board Initialization Function ***
void Board_Init() {

    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    //SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
    LED_Init();
    UART_Init();
    Movement_Init();
    Sensing_Init();
    LightTimer_Init();

    //SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    //*******************Config For Timer*****************************************
    //Enable Timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerEnable(TIMER0_BASE,TIMER_A);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/20);
    TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
    TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
    IntEnable(INT_TIMER0A);
    TimerEnable(TIMER0_BASE, TIMER_A);

}

// ======== main ========

char instructions[2];
int16_t totalSummation = 0;
int16_t diff;
double errorPrev = 0;
int main(void)
{
    Board_Init();

    BIOS_start();
    return(0);
}

void Timeout_50MS() {
    identify_color();
    PID();
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
