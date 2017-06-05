/*
 * FreakThread.h
 *
 *  Created on: 23-Mar-2017
 *      Author: codenamefreak
 */

#ifndef FREAKTHREAD_H_
#define FREAKTHREAD_H_
#include <stdio.h>
#include <stdlib.h>
#include "UserTask.h"
#include "FreakTimeLogger.h"

#define NUM_TIMES_LOOP 10

/**
 * Create a USER-LEVEL THREAD that executes the function passed as an argument
 * */
int create_freak_thread(void(*freakTask)(void *),void * arg);
void scheduleNext(void);

void play();
void showList();

#endif /* FREAKTHREAD_H_ */
