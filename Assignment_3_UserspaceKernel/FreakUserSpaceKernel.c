/*
 ============================================================================
 Name        : FreakUserSpaceKernel.c
 Author      : Anubhav Guleria
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "FreakKernel.h"
#include "Tasks.h"


/*void timeExpire(void)
{
	printf("10 millisecond Timer Expired.....\n");
	//Take actions after getting 10 ms Real Signal.
	//
	//
	//
	triggerTimerExpiration();
}*/


static void freakRealSignalHandlerForScheduling(int sig,siginfo_t * siginfo, void * context)
{
	//printf("20ms\n");
	/**
	 * Add 10 ms and Preempt the currently running.
	 *  */
	triggerTimerExpiration();
	//printf("Hey");
	scheduleNext();


}

static void freakRealSignalHandlerForTimeUpdate(int sig,siginfo_t * siginfo, void * context)
{
	//printf("10ms\n");
	/**
	 * Add 10ms to currently running task.
	 * */
	triggerTimerExpiration();
	//display_freak_threads();
}


int main(void) {
	getTimeStamp();
	printf("Freak Kernel Started");
	displayDelayedCharacter('.',10 , 500000000L);
	getTimeStamp();
	printf("Kernel will receive Real Signal SIGRTUPDTE (to update current timeslice) after every: %d ms.\n",FREAK_MIN_TIME_SLICE_MS/1000);
	getTimeStamp();
	printf("Kernel will receive Real Signal SIGRTSCHED (to schedule next priority task) after every: %d ms.\n",(FREAK_MIN_TIME_SLICE_MS/1000)*FREAK_GLOBAL_TIME_QUANTUM);
	/**
	 *
	 *
	 * Kernel Started.
	 * 1) Timer Initialized. (initializes a 10 ms Real Signal received after every 10 ms timer expiry.)
	 * 2) Scheduler Initialized.
	 * 3) Tasks Initialized.
	 *
	 * 							''''PLAY''''
	 *
	 * */


	getTimeStamp();
	printf("Initializing Scheduler");
	displayDelayedCharacter('.',10 , 500000000L);
	initializeScheduler();
	getTimeStamp();
	printf("Scheduler Initialized.\n");

	////////////////////////////////////	First Thread Created	///////////////////////////////////////////
	getTimeStamp();
	printf("Creating First thread.");
	displayDelayedCharacter('.',4 , 500000000L);
	create_freak_thread(freakTask_1, NULL, ONE, 40);
	display_freak_threads();
	displayDelayedCharacter(' ',6 , 500000000L);
	////////////////////////////////////	First Thread Created	///////////////////////////////////////////

	////////////////////////////////////	Second Thread Created	///////////////////////////////////////////
	getTimeStamp();
	printf("Creating Second thread.");
	displayDelayedCharacter('.',4 , 500000000L);
	create_freak_thread(freakTask_2, NULL, ONE, 60);
	display_freak_threads();
	displayDelayedCharacter(' ',6 , 500000000L);
	////////////////////////////////////	Second Thread Created	///////////////////////////////////////////


	////////////////////////////////////	Third Thread Created	///////////////////////////////////////////
	getTimeStamp();
	printf("Creating Third thread.");
	displayDelayedCharacter('.',4 , 500000000L);
	create_freak_thread(freakTask_3, NULL, ONE, 80);
	display_freak_threads();
	displayDelayedCharacter(' ',6 , 500000000L);
	////////////////////////////////////	Third Thread Created	///////////////////////////////////////////



	////////////////////////////////////	Fourth Thread Created	///////////////////////////////////////////
	getTimeStamp();
	printf("Creating Fourth thread.");
	displayDelayedCharacter('.',4 , 500000000L);
	create_freak_thread(freakTask_4, NULL, ONE, 40);
	display_freak_threads();
	displayDelayedCharacter(' ',6 , 500000000L);
	////////////////////////////////////	Fourth Thread Created	///////////////////////////////////////////

	////////////////////////////////////	Fifth Thread Created	///////////////////////////////////////////
	getTimeStamp();
	printf("Creating Fifth thread.");
	displayDelayedCharacter('.',4 , 500000000L);
	create_freak_thread(freakTask_5, NULL, FIVE, 40);
	display_freak_threads();
	displayDelayedCharacter(' ',6 , 500000000L);
	////////////////////////////////////	Fifth Thread Created	///////////////////////////////////////////

	////////////////////////////////////	Sixth Thread Created	///////////////////////////////////////////
	getTimeStamp();
	printf("Creating Sixth thread.");
	displayDelayedCharacter('.',4 , 500000000L);
	create_freak_thread(freakTask_6, NULL, TWO, 20);
	display_freak_threads();
	displayDelayedCharacter(' ',6 , 500000000L);
	////////////////////////////////////	Sixth Thread Created	///////////////////////////////////////////

	////////////////////////////////////	Seventh Thread Created	///////////////////////////////////////////
	getTimeStamp();
	printf("Creating Seventh thread.");
	displayDelayedCharacter('.',4 , 500000000L);
	create_freak_thread(freakTask_7, NULL, NINE, 40);
	display_freak_threads();
	displayDelayedCharacter(' ',6 , 500000000L);
	////////////////////////////////////	Seventh Thread Created	///////////////////////////////////////////

	////////////////////////////////////	Eighth Thread Created	///////////////////////////////////////////
	getTimeStamp();
	printf("Creating Seventh thread.");
	displayDelayedCharacter('.',4 , 500000000L);
	create_freak_thread(freakTask_8, NULL, TEN, 100);
	display_freak_threads();
	displayDelayedCharacter(' ',6 , 500000000L);
	////////////////////////////////////	Eighth Thread Created	///////////////////////////////////////////

	getTimeStamp();
	printf("Destroying Third thread.\n");
	destroy_freak_thread(3);
	displayDelayedCharacter('.',4 , 500000000L);
	display_freak_threads();
	char ch;
	getTimeStamp();
	printf("Enter any character to begin scheduling.\n");

	scanf("%c",&ch);

	getTimeStamp();
	printf("Initializing Timer");
	displayDelayedCharacter('.',10 , 500000000L);
	initializeTimer(freakRealSignalHandlerForScheduling,freakRealSignalHandlerForTimeUpdate);
	getTimeStamp();
	printf("Timer Initialized.\n");
	printf("\n\n");
	getTimeStamp();
	printf("Initiating Scheduling.\n\n");

	playTasks();



	//while(1);

	/**
	 * Scheduling will be done here.
	 * */
	getTimeStamp();
	printf("Terminating User Kernel\n");
	displayDelayedCharacter('.',10 , 500000000L);
	return EXIT_SUCCESS;
}
