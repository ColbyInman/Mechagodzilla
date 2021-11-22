/*
 * sensing.c
 *
 *  Created on: Oct 19, 2021
 *      Author: Mechagodzilla
 */

#include "sensing.h"
#include "movement.h"
#include "LightTimer.h"
#include "controlLED.h"
#define SETPOINT 2500
#define P_MULT 0.08
#define D_MULT 0.05

char instructions[2];
//int16_t totalSummation = 0;
int16_t diff;
int errorPrev = 0;
int errorCurr;
int count = 0;

char ping[20];
char pong[20];
int counter = -1;
bool firstFlag = false;
bool pingPong = false;
bool everyOther = false;

extern bool startCollecting;

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

void PID(void)
{
    count++;
    int P, D;
    int IRdist, frontDist;
    int CorrectionError;

    IRdist = IRDistanceCollect(ADC0_BASE);
    frontDist = IRDistanceCollect(ADC1_BASE);
    if (frontDist > 2000)
    {
        Uturn();
    }

    errorCurr = fabs(SETPOINT - IRdist);

    P = P_MULT * errorCurr;

    diff = errorCurr - errorPrev;
    errorPrev = errorCurr;
    D = D_MULT * diff;  // Note, not using time as a simplification since it should be consistent

    CorrectionError = P+D;

    if (CorrectionError > 60)
    {
        CorrectionError = 60;
        rLED();
    }
    else
    {
        offLED();
    }
    CalculateSpeed(IRdist, CorrectionError);

    if(everyOther){
            counter = (counter >= 19) ? 0 : (counter+1);
            pingPong = (counter==0) ? (!pingPong) : pingPong;
            if(pingPong){
                ping[counter] = fabs(errorCurr);
            }
            else{
                pong[counter] = fabs(errorCurr);
            }
        if(counter == 19)
        {
            IRDistanceDisplay();
        }
    }
    everyOther = (startCollecting)? !everyOther : false;
    //everyOther = !everyOther;
    if(startCollecting)
    {
        dataCollect();
    }
    else
    {
        endCollect();
    }
}

void IRDistanceDisplay(void)
{
    char header[3] = ":24";
    char footer[4] = "24\r\n";
    int i = 0;

    for(i = 0; i < 3; ++i)
    {
        UARTCharPut(UART5_BASE, header[i]);
    }

    if(pingPong)
    {
        //write ping(counter)
        for(i = 0; i < 20; ++i)
        {
            UARTCharPutNonBlocking(UART5_BASE, ping[i]);
            sendData();
        }
        endSend();
    }
    else
    {
        //write pong(counter)
        for(i = 0; i < 20; ++i)
        {
            UARTCharPutNonBlocking(UART5_BASE, pong[i]);
            sendData();
        }
        endSend();
    }

    for (i = 0; i < 4; ++i)
    {
        UARTCharPut(UART5_BASE, footer[i]);
    }
}

void Uturn(void)
{
    double frontDist, rightDist;
    frontDist = IRDistanceCollect(ADC1_BASE);
    rightDist = IRDistanceCollect(ADC0_BASE);
    while(!(frontDist < 900 && rightDist > 1800))
    {
        if (frontDist < 900) { //Left Turn
            break;
        }
        gLED();
        fastSpeed();
        frontDist = IRDistanceCollect(ADC1_BASE);
        rightDist = IRDistanceCollect(ADC0_BASE);
        LightTimerReload();
    }
    offLED();
}
