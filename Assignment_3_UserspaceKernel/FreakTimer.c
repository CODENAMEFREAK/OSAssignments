/*
 * FreakTimer.c
 *
 *  Created on: 30-Mar-2017
 *      Author: codenamefreak
 */


#include "FreakTimer.h"
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>


void (*kernelTimerHandlerForScheduling)(int,siginfo_t *,void *) = NULL;
void (*kernelTimerHandlerForTimeUpdate)(int,siginfo_t *,void *) = NULL;


int num_sig_skipped=0;


/**
 * Sets the disposition for Real Signal generated for 10ms  timer expiration.
 * */
static void setScheduleDisposition()
{
	SCHED_PROCESS_ID = getpid();//Initializes PID so that sigqueue can send real signal.
	struct sigaction * actRTHandler = (struct sigaction *)malloc(sizeof(struct sigaction));


	actRTHandler->sa_flags=SA_SIGINFO | SA_RESTART;
	actRTHandler->sa_handler= kernelTimerHandlerForScheduling;

	if(sigaction(SIGRTSCHED,actRTHandler,NULL)==-1)
	{
		printf("Failed to set Real Signal\n");
	}
	else{
		getTimeStamp();
		printf("Real Signal handler set for SIGRTSCHED (Updates the time consumed by current thread by 10 ms and schedules the next thread after %d ms).\n",(FREAK_MIN_TIME_SLICE_MS/1000)*FREAK_GLOBAL_TIME_QUANTUM);
	}


	actRTHandler = (struct sigaction *)malloc(sizeof(struct sigaction));
	actRTHandler->sa_flags=SA_SIGINFO | SA_RESTART;
	actRTHandler->sa_handler= kernelTimerHandlerForTimeUpdate;
	if(sigaction(SIGRTUPDTE,actRTHandler,NULL)==-1)
		{
			printf("Failed to set Real Signal\n");
		}
		else{
			getTimeStamp();
			printf("Real Signal handler set for SIGRTUPDTE (Updates the time consumed by current thread by %d ms).\n",FREAK_MIN_TIME_SLICE_MS/1000);
		}
}
/**
 * Handler for 10ms Times that generates a Real Signal for the process to invoke Scheduling.*/
static void handler(int sig,siginfo_t * siginfo,void * context)
{
	num_sig_skipped++;
	if(num_sig_skipped<FREAK_GLOBAL_TIME_QUANTUM){
		//printf("Skipping 10 ms signal\n");
		union sigval sigvalue;

		sigqueue(SCHED_PROCESS_ID,SIGRTUPDTE, sigvalue);//Real Signal Generated for Kernel.
	}
	else{
		num_sig_skipped=0;
		/**
				 * send a Real signal to invoke scheduler.
				 * */
		union sigval sigvalue;
		sigqueue(SCHED_PROCESS_ID,SIGRTSCHED, sigvalue);//Real Signal Generated for Kernel.
	}



}

static struct sigaction * initializeActSIGVT()
{
	struct sigaction * actHandler = (struct sigaction *)malloc(sizeof(struct sigaction));

	actHandler->sa_handler=handler;
	actHandler->sa_flags=SA_SIGINFO | SA_RESTART; //SA_SIGINFO to call handler with extra info, and SA_RESTART to restart system calls interrupted by signal handler.
	/**
	 * Also the signal that will invoke the signal handler will be automatically added to the signal mask and thus handler will not */
	return actHandler;
}



/**
 * Sets 10 ms timer and establishes its Disposition.
 * */

static void setTimer()
{
	struct itimerval * oldVal=NULL;
	struct itimerval * newVal=NULL;



	/**
	 * Initialize the value for timer.
	 * */
	newVal = (struct itimerval *)malloc(sizeof(struct itimerval));

	/**
	 * it_interval is the period between the timer expiration.
	 * it_value is the phase from current moment.
	 * */
	newVal->it_interval.tv_sec=0;
	newVal->it_interval.tv_usec=FREAK_MIN_TIME_SLICE_MS;//10 milliseconds = 10000 microseconds

	newVal->it_value.tv_sec = 0;
	newVal->it_value.tv_usec = FREAK_MIN_TIME_SLICE_MS;
	struct sigaction * actSIGVTALARM = initializeActSIGVT();
	if(sigaction(SIGVTALRM,actSIGVTALARM,NULL)==-1)
		{
		printf("Failed to set disposition.\n");
	}else{
		getTimeStamp();
		printf("Signal handler set for SIGVTALRM.\n");
	}

	/**
	 * ITIMER_VIRTUAL: Create a timer that counts down in Process-VirtualTime i.e. User mode CPU time.
	 * 				   When this timer expires SIGVTALARM signal is generated for the process.
	 * */
	if(setitimer(ITIMER_VIRTUAL,newVal,oldVal)==-1)
		{
			printf("Failed to set Timer\n");
		}
	else
		{
		getTimeStamp();
		printf("Timer installation successful to raise SIGVTALRM at 10ms.\n");
		//printf("Wait for it :) \n");
		}

}







/**
 * Initializing a 10 ms periodic timer.
 * Also set a disposition for SIGVTALARM , that signals the UserKernel by a Real Signal.
 * */
int initializeTimer(void (*freakRealSignalHandlerForScheduling)(int,siginfo_t *,void *), void (*freakRealSignalHandlerForTimeUpdate)(int,siginfo_t *,void *))
{
	kernelTimerHandlerForScheduling = freakRealSignalHandlerForScheduling;
	kernelTimerHandlerForTimeUpdate = freakRealSignalHandlerForTimeUpdate;


	/**
		 * Set disposition for Real Signal  to invoke scheduling.
		 * */
	getTimeStamp();
	printf("Setting schedule disposition for Real Signals.");
	displayDelayedCharacter('.', 5, 500000000L);
	setScheduleDisposition();

	getTimeStamp();
	printf("Setting handler for SIGVTALRM to get signal after every 10 ms.");
	setTimer();

	return 0;



}
