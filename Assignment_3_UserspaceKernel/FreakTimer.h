/*
 * FreakTimer.h
 *
 *  Created on: 30-Mar-2017
 *      Author: codenamefreak
 */

#ifndef FREAKTIMER_H_
#define FREAKTIMER_H_

#include<signal.h>
#define FREAK_MIN_TIME_SLICE_MS 10000 //Timeslice after which the Kernel will receive SIGVTALRM.
#define FREAK_GLOBAL_TIME_QUANTUM 2 //Multiple in terms of FREAK_MIN_TIME_SLICE_MS
pid_t SCHED_PROCESS_ID;

#define SIGRTSCHED  (SIGRTMIN +1) //Real Signal for scheduling.
#define SIGRTUPDTE	(SIGRTSCHED +1)//RealSignal for timeupdate.

int initializeTimer(void (*freakRealSignalHandlerForScheduling)(int,siginfo_t *,void *),void (*freakRealSignalHandlerForTimeUpdate)(int,siginfo_t *,void *));

#endif /* FREAKTIMER_H_ */
