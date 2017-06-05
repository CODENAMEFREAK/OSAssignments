/*
 * FreakThread.c
 *
 *  Created on: 30-Mar-2017
 *      Author: codenamefreak
 */



#include "FreakThreadInterface.h"
#include<stdlib.h>






static void playFreakTask(void *task)
{
	utask_t  * playTask = (utask_t *) task;
	playTask->status=RUNNING;	 //First set the status as running
		playTask->userTask(playTask->args);				  //Then start execution
	playTask->status=EXIT;	  //Once execution is completed set the status as EXIT
	//removeFreakContext(playTask);	//Then remove the context from the Ready Queue.
}

/**
 * Creates new User Task  and adds it to the Ready Queue.
 * */
int create_freak_thread(void(*freakTask)(void *),void * arg,TASK_PRIORITY taskPriority,int period)
{
	utask_t * newTask = (utask_t *)malloc(sizeof(utask_t));
	newTask->user_task_id=NEXT_USER_THREAD_ID++;
	newTask->userTask=freakTask;
	newTask->args=arg;
	newTask->status = READY;	//1) Set status as READY
	newTask->context =(ucontext_t *)malloc(sizeof(ucontext_t));
	newTask->priority=taskPriority;
	newTask->sched_info.num_visits=0;
	newTask->sched_info.current_timeslice=0;
	newTask->sched_info.remaining_timeslice = newTask->sched_info.defaultPeriod = period;
	getcontext(newTask->context);  // 2)Get the current context

	newTask->context->uc_link =NULL;	//This is pointer to next context structure which is used if the context defined by current structure returns. Setting it as NULL.
	newTask-> stack = malloc(4 * STACK_SIZE); //stack is pointed to by stack pointer in task. TODO: Change this. Unsafe
	newTask->context->uc_stack.ss_sp=newTask->stack; //pointer to stack is set in context.
	newTask->context->uc_stack.ss_size=(4*STACK_SIZE);
	newTask->context->uc_stack.ss_flags=0;//TODO:See the effect of this.
	/**
	 * Here I called playFreakTask in the new Context.So as to keep track when this task completes and set it's STATUS as EXIT. */
	makecontext(newTask->context, playFreakTask,1, newTask);//Invoke playFreakTask with newTask that has method to be invoked and arguments that are to be passed to it.

	return addNewUserTask(newTask);
}


void display_freak_threads()
{
	//validateLinks();
	showAllTasks();
}

void destroy_freak_thread(int threadID)
{
	removeThreadWithID(threadID);
}

