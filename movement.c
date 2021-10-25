/*
 * movement.c
 *
 *  Created on: Oct 13, 2021
 *      Author: Mechagodzilla
 */

#include "movement.h"

extern uint32_t val_load;

void setSpeed(int duty){//Sets the speed based on the function sent to it
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
void goForward(void){//Turns on both PWM signals
    PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);
}
void turnRight(void){//turns off PB4 and activates PB5, turning right
    PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, false);
    //figure out a control timing to force a full right turn
}
void uturn(void){//Calls turnRight in order to perform a u-turn
    turnRight();
    //figure out a control timing to force a full u-turn
}
void stop(void){//Turns off both PWM signals
    PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, false);
    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, false);
}

void CalculateSpeed(uint16_t PID, uint16_t R_Error) {
    if (PID < SETPOINT) { // If too close to wall
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, (100*val_load/100) * 1 - R_Error);
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, 100*val_load/100);
    }
    if (PID > SETPOINT) { // If too close to wall
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, (100*val_load/100) - (100*val_load/100)* R_Error);
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, 100*val_load/100);
    }
    else {
        fastSpeed();
    }
}
