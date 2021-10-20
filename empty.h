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


#endif /* EMPTY_H_ */
