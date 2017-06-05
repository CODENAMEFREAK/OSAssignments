/*
 * FreakThreadInterface.h
 *
 *  Created on: 30-Mar-2017
 *      Author: codenamefreak
 */

#ifndef FREAKTHREADINTERFACE_H_
#define FREAKTHREADINTERFACE_H_

#include "FreakSched.h"
static int NEXT_USER_THREAD_ID =1; //initially this value is set as 1.
/**
 * Create a USER-LEVEL THREAD that executes the function passed as an argument
 * */
int create_freak_thread(void(*freakTask)(void *),void * arg, TASK_PRIORITY taskPriority, int period);


/**
 * Display the info about the threads per Priority Level basis.
 * */
void display_freak_threads();

/**
 * Remove thread from the thread directory given threadID.
 *  */
void destroy_freak_thread(int threadID);

#endif /* FREAKTHREADINTERFACE_H_ */
