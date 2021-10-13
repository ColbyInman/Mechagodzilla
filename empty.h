/*
 * empty.h
 *
 *  Created on: Oct 3, 2021
 *      Author: Mechagodzilla
 */

#ifndef EMPTY_H_
#define EMPTY_H_

typedef void (*Actions)(void);

typedef struct UserCommand
{
   char *Command;
   Actions act;
}UserCommand_t;

//basic function definitions
void set(void){}
void dist(void){}
void rLED(void);
void gLED(void);
void bLED(void);
void wLED(void);
void offLED(void);
void frontDistPrnt(void){}
void rightDistPrnt(void){}


#endif /* EMPTY_H_ */
