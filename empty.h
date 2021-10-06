/*
 * empty.h
 *
 *  Created on: Oct 3, 2021
 *      Author: colby
 */

#ifndef EMPTY_H_
#define EMPTY_H_

typedef void (*Actions)(void);

struct UserCommand
{
   char *Command;
   Actions act;
};

void set(void){}
void dist(void){}
void rLED(void);
void gLED(void);
void bLED(void);
void wLED(void);
void offLED(void);
void frontDistPrnt(void){}
void rightDistPrnt(void){}
void run(void){}
void reverse(void){}
void uturn(void){}
void stop(void){}

#endif /* EMPTY_H_ */
