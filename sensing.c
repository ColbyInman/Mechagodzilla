/*
 * sensing.c
 *
 *  Created on: Oct 19, 2021
 *      Author: Mechagodzilla
 */
#include "sensing.h"

void frontDistPrnt(void)
{
    IRDistanceCollect(3);
}
void rightDistPrnt(void)
{
    IRDistanceCollect(2);//find actual needed pin number for this
}

void IRDistanceCollect(int pin)
{
    extern uint32_t adcVal;
    double VoltageRead; // to display voltage
    // clear ADC interrupt
    //ADCIntClear(ADC0_BASE, 3);
    // trigger ADC sampling
    ADCProcessorTrigger(ADC0_BASE, pin);
    // read voltage
    ADCSequenceDataGet(ADC0_BASE, pin, &adcVal);
    VoltageRead = adcVal * 3.3 / 4095;
    double F_Distance = -1.7117*(VoltageRead - 9.1733949)/(VoltageRead + 0.0773);
    IRDistanceDisplay(F_Distance);

}

void IRDistanceDisplay(double distance)
{
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART5_BASE, true);
    UARTIntClear(UART5_BASE, ui32Status);
    double Num = 2.2;
    char DistanceActual [25];
    sprintf(DistanceActual,"IR Distance: %lf", distance);
    int len = strlen(DistanceActual);
    int i;
    for(i=0;i<len;i++){
        UARTCharPutNonBlocking(UART5_BASE, DistanceActual[i]);
    }
    UARTCharPutNonBlocking(UART5_BASE, '\r');
    UARTCharPutNonBlocking(UART5_BASE, '\n');


}
