/*
 * movement.c
 *
 *  Created on: Oct 13, 2021
 *      Author: Mechagodzilla
 */

#include "movement.h"

extern uint32_t val_load;

void setSpeed(int duty){
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, duty*val_load/100);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, duty*val_load/100);
}

void slowSpeed(void)
{
    setSpeed(25);
}
void mediumSpeed(void)
{
    setSpeed(50);
}
void fastSpeed(void)
{
    setSpeed(75);
}
void goForward(void){
    PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);
}
void turnRight(void){
    PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, false);
    //figure out a control timing to force a full right turn
}
void uturn(void){
    turnRight();
    //figure out a control timing to force a full u-turn
}
void stop(void){
    PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, false);
    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, false);
}

