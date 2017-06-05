/*
 * UserTask.h
 *
 *  Created on: 30-Mar-2017
 *      Author: codenamefreak
 */

#ifndef USERTASK_H_
#define USERTASK_H_


#include <ucontext.h>
#include <unistd.h>
#define STACK_SIZE getpagesize()



typedef enum {READY,RUNNING,EXIT} TASK_STATUS;

typedef enum {ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,TEN} TASK_PRIORITY;

typedef struct user_sched{
	int num_visits;
	int current_timeslice;
	int remaining_timeslice;
	int defaultPeriod;
}u_se_t;


/**
 *
 * utask_t is thread descriptor.
 *
 * */
typedef struct usertask{
	int user_task_id;	//Locally generated ID.
	ucontext_t * context; //to keep track of machine context.
	void * stack; //to keep track of stack.
	u_se_t sched_info;//to keep sched info
	//int num_visits;
	TASK_STATUS status;
	TASK_PRIORITY priority;
	void(*userTask)(void *); //To userTask Function.
	void *args; //To user args.
}utask_t;




#endif /* USERTASK_H_ */
