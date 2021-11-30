/*
 * sensing.c
 *
 *  Created on: Oct 19, 2021
 *      Author: Mechagodzilla
 */

#include "main.h"
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
char temp[40];
int counter = -1;
bool firstFlag = false;
bool pingPong = false;
bool everyOther = false;

extern bool startCollecting;
extern bool uturnFlag;

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
    uturnFlag = false;
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
    int temp;

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
            counter = (counter >= 9) ? 0 : (counter+1);
            pingPong = (counter==0) ? (!pingPong) : pingPong;
            if(pingPong){
                temp = fabs(errorCurr);
                ping[2*counter] = temp/256;
                ping[2*counter + 1] = temp%256;
            }
            else{
                temp = fabs(errorCurr);
                pong[2*counter] = temp/256;
                pong[2*counter + 1] = temp%256;
            }
        if(counter == 9)
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

    sendData();
    bLED();
    for(i = 0; i < 3; ++i)
    {
        UARTCharPut(UART5_BASE, header[i]);
    }

    if(pingPong)
    {
        //write ping(counter)
        for(i = 0; i < 20; ++i)
        {
            temp[2*i] = ping[i]/16;
            temp[2*i+1] = ping[i]%16;
            if(temp[2*i] < 10)
            {
                temp[2*i] += 48;
            }
            else
            {
                temp[2*i] += 55;
            }
            if(temp[2*i+1] < 10)
            {
                temp[2*i+1] += 48;
            }
            else
            {
                temp[2*i+1] += 55;
            }
            UARTCharPut(UART5_BASE, temp[2*i]);
            UARTCharPut(UART5_BASE, temp[2*i+1]);
            //UARTCharPut(UART5_BASE, i+65);
        }
    }
    else
    {
        for(i = 0; i < 20; ++i)
        {
            //write pong(counter)
            temp[2*i] = pong[i]/16;
            temp[2*i+1] = pong[i]%16;
            if(temp[2*i] < 10)
            {
                temp[2*i] += 48;
            }
            else
            {
                temp[2*i] += 55;
            }
            if(temp[2*i+1] < 10)
            {
                temp[2*i+1] += 48;
            }
            else
            {
                temp[2*i+1] += 55;
            }
            UARTCharPut(UART5_BASE, temp[2*i]);
            UARTCharPut(UART5_BASE, temp[2*i+1]);
            //UARTCharPut(UART5_BASE, i+65);
        }
    }

    for (i = 0; i < 4; ++i)
    {
        UARTCharPut(UART5_BASE, footer[i]);
    }
    //endSend();
    offLED();
}

void Uturn(void)
{
    while(1)
    {
        double frontDist, rightDist;
        frontDist = IRDistanceCollect(ADC1_BASE);
        rightDist = IRDistanceCollect(ADC0_BASE);
        if(!(frontDist < 900 && rightDist > 1800))
        {
            uturnFlag = true;
            Semaphore_post(semaphore0);
            //gLED();
            fastSpeed();
            frontDist = IRDistanceCollect(ADC1_BASE);
            rightDist = IRDistanceCollect(ADC0_BASE);
            LightTimerReload();
        }
        else
        {
            uturnFlag = false;
            offLED();
        }
    }
}
