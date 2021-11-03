/*
 * sensing.c
 *
 *  Created on: Oct 19, 2021
 *      Author: Mechagodzilla
 */
#include "sensing.h"
#include "movement.h"
#include "controlLED.h"

void Sensing_Init(void)
{
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
    ADCSequenceStepConfigure(ADC1_BASE, 3, 0, ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC1_BASE, 3);
}

void frontDistPrnt(void)
{
    IRDistanceCollect(ADC1_BASE);
}
void rightDistPrnt(void)
{
    IRDistanceCollect(ADC0_BASE);
}

int IRDistanceCollect(int base)
{
    extern uint32_t adcVal;
    //double VoltageRead; // to display voltage
    // clear ADC interrupt
    //ADCIntClear(ADC0_BASE, 3);
    // trigger ADC sampling
    ADCProcessorTrigger(base, 3);
    // read voltage
    ADCSequenceDataGet(base, 3, &adcVal);
    //VoltageRead = adcVal * 3.3 / 4095;
    //double F_Distance = -1.7117*(VoltageRead - 9.1733949)/(VoltageRead + 0.0773);
    //IRDistanceDisplay(adcVal);
    return adcVal;

    //No longer calculates distance in cm, only as a % of 4095
}

void IRDistanceDisplay(int distance)
{
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART5_BASE, true);
    UARTIntClear(UART5_BASE, ui32Status);
    char DistanceActual [25];
    sprintf(DistanceActual,"IR Distance: %d ", distance);
    int len = strlen(DistanceActual);
    int i;
    for(i=0;i<len;i++)
    {
        UARTCharPutNonBlocking(UART5_BASE, DistanceActual[i]);
    }
    UARTCharPutNonBlocking(UART5_BASE, '\r');
    UARTCharPutNonBlocking(UART5_BASE, '\n');
}

void Uturn(void)
{
    double frontDist, rightDist;
    frontDist = IRDistanceCollect(ADC1_BASE);
    rightDist = IRDistanceCollect(ADC0_BASE);
    while(!(frontDist < 900 && rightDist > 1800))
    {
        gLED();
        fastSpeed();
        frontDist = IRDistanceCollect(ADC1_BASE);
        rightDist = IRDistanceCollect(ADC0_BASE);
    }
    offLED();
}
