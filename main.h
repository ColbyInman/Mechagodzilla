/*
 * empty.h
 *
 *  Created on: Oct 3, 2021
 *      Author: Mechagodzilla
 */

#ifndef MAIN_H_
#define MAIN_H_

typedef void (*Actions)(void);

typedef struct UserCommand
{
   char *Command;
   Actions act;
}UserCommand_t;

//basic function definitions
void set(void){}
void dist(void){}
void InputFunction(void);
void PID();

#endif /* MAIN_H_ */
