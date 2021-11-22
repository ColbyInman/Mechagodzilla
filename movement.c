/*
 * movement.c
 *
 *  Created on: Oct 13, 2021
 *      Author: Mechagodzilla
 */

#include "movement.h"
#include "controlLED.h"
#include "LightTimer.h"

int val_load, pwm_clk;
extern bool startCollecting;
extern int overallTimer;

void Movement_Init(void)
{
    //Enable for PWM pins
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

    //Set PWM for motor control (PB4 is right and PB5 is left)
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);
    GPIOPinConfigure(GPIO_PB5_M0PWM3);
    GPIOPinConfigure(GPIO_PB4_M0PWM2);

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
}

void setSpeed(int duty)
{//Sets the speed based on the function sent to it
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, duty*val_load/100);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, duty*val_load/100);
}

void slowSpeed(void)
{//calls setSpeed with a small value
    setSpeed(50);
}

void mediumSpeed(void)
{//calls setSpeed with a medium value
    setSpeed(75);
}

void fastSpeed(void)
{//calls setSpeed with a large value
    setSpeed(100);
}

void goForward(void)
{//Turns on both PWM signals
    PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);
    TimerEnable(TIMER1_BASE, TIMER_A);
    TimerEnable(TIMER1_BASE, TIMER_B); //Timer for total racetime
    overallTimer = 0;
}

void turnRight(void)
{//turns off PB4 and activates PB5, turning right
    PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, false);
    //figure out a control timing to force a full right turn
}

void uturn(void)
{//Calls turnRight in order to perform a u-turn
    turnRight();
    //figure out a control timing to force a full u-turn
}

void stop(void)
{//Turns off both PWM signals
    PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, false);
    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, false);
    TimerDisable(TIMER1_BASE, TIMER_A);
    startCollecting = 0;
    float completionTime = overallTimer / 20;
    printTotalTime(completionTime);
}

void CalculateSpeed(uint16_t PID, double R_Error) {

    if (PID < SETPOINT)
    { // If far from the wall
        int PWMleft = val_load;
        int PWMright = val_load - val_load * R_Error/100;
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, PWMright);
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, PWMleft);
    }

    else if (PID > SETPOINT)
    { // If too close to the wall
        int PWMright = val_load;
        int PWMleft = val_load - val_load * R_Error/100;
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, PWMleft);
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, PWMright);
    }

    else {
        fastSpeed();
    }
}

void printTotalTime(float time) {
    char timeStr[24];
    int length = sprintf(timeStr, "Overall Time: %0.2f", time);
    int i;
    for(i = 0; i < length; ++i)
        {
            UARTCharPut(UART5_BASE, timeStr[i]);
        }
}
