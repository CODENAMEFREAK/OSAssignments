/*
 * FreakSched.c
 *
 *  Created on: 30-Mar-2017
 *      Author: codenamefreak
 */


#include "FreakSched.h"

#include <stdio.h>
#include <stdlib.h>


static task_leaf_t * currentTaskLeaf = NULL;
static utask_t *mainTaskContext = NULL;

static int flagAllEnd =-1;
void validateLinks()
{
	for(int i=0;i<10;i++)
	{
		printf("Checking Level %d.\n",i+1);
		task_leaf_t * head =schedbook[i].ready_queue_level;
		if(head!=NULL)
		{
		while(head->next!=NULL)
		{
			head=head->next;
		}
		printf("\tForward Verified...\n");
		while(head!=schedbook[i].ready_queue_level)
				head=head->prev;
		printf("\tBackward Verified...\n");
		}
	}
}
int initializeScheduler()
{
	schedbook[0].priority=ONE;
	schedbook[1].priority=TWO;
	schedbook[2].priority=THREE;
	schedbook[3].priority=FOUR;
	schedbook[4].priority=FIVE;
	schedbook[5].priority=SIX;
	schedbook[6].priority=SEVEN;
	schedbook[7].priority=EIGHT;
	schedbook[8].priority=NINE;
	schedbook[9].priority=TEN;
	for(int i=MIN_PRIORITY_LEVEL;i<NUM_PRIORITY_LEVELS;i++)
		{
		//priority_node_t[0].priority=i;
		schedbook[i].ready_queue_level=NULL;
		}
}


static task_leaf_t * addToEnd(task_leaf_t* taskQueueHead,task_leaf_t * task)
{
	task_leaf_t * temp = taskQueueHead;
	if(temp==NULL)
	{
		taskQueueHead = temp = task;
	}
	else{
		//add to the end
		while(temp->next!=NULL)
		{
			temp=temp->next;
		}
		//at this point temp points to the last added node
		task->prev = temp;
		temp->next=task;
	}
	return taskQueueHead;
}
/**
 * Adds taskleaf into the scheduler book.
 *
 * */
int addNewUserTask(utask_t * newTask)
{
	task_leaf_t *taskLeaf =(task_leaf_t *)malloc(sizeof(taskLeaf));
	taskLeaf->task=newTask;
	taskLeaf->next=NULL;
	taskLeaf->prev=NULL;

	task_leaf_t *temp = NULL;
	switch(newTask->priority)
	{
		case ONE:
		{
			schedbook[0].ready_queue_level = addToEnd(schedbook[0].ready_queue_level, taskLeaf);
			return 1;
		}
		case TWO:
		{
			schedbook[1].ready_queue_level = addToEnd(schedbook[1].ready_queue_level, taskLeaf);
			return 2;
		}
		case THREE:
		{
			schedbook[2].ready_queue_level = addToEnd(schedbook[2].ready_queue_level, taskLeaf);
			return 3;
		}
		case FOUR:
		{
			schedbook[3].ready_queue_level = addToEnd(schedbook[3].ready_queue_level, taskLeaf);
			return 4;
		}
		case FIVE:
		{
			schedbook[4].ready_queue_level = addToEnd(schedbook[4].ready_queue_level, taskLeaf);
			return 5;
		}
		case SIX:
		{
			schedbook[5].ready_queue_level = addToEnd(schedbook[5].ready_queue_level, taskLeaf);
			return 6;
		}
		case SEVEN:
		{
			schedbook[6].ready_queue_level = addToEnd(schedbook[6].ready_queue_level, taskLeaf);
			return 7;
		}
		case EIGHT:
		{
			schedbook[7].ready_queue_level = addToEnd(schedbook[7].ready_queue_level, taskLeaf);
			return 8;
		}
		case NINE:
		{
			schedbook[8].ready_queue_level = addToEnd(schedbook[8].ready_queue_level, taskLeaf);
			return 9;
		}
		case TEN:
		{
			schedbook[9].ready_queue_level = addToEnd(schedbook[9].ready_queue_level, taskLeaf);
			return 10;
		}
		default:
		{
			return -1;
		}
	}
}

/*task_leaf_t * fetchNextTopLeaf(priority_node_t node)
{
	printf("Scanning Priority Level: %d \n",node.priority);
	task_leaf_t * temp=node.ready_queue_level;
	while(temp!=NULL)
	{
		utask_t nextTask = temp->task;
		if(nextTask->status==READY)
		{
			return temp;//i.e. as soon as you see any task with status READY.
		}
		temp=temp->next;
	}

	return temp;
	}*/



void scheduleNext()
{
	display_freak_threads();
	if(currentTaskLeaf!=NULL)
		{
	//First of all move currently running task to the end of that queue
	int currentPriorityLevel = currentTaskLeaf->task->priority;
	priority_node_t node = schedbook[currentPriorityLevel];
	task_leaf_t *headtask = node.ready_queue_level;

	if(headtask->task->sched_info.remaining_timeslice<=0)
	{

		//i.e. checking if the time has already expired
		if(headtask->next!=NULL)
		{
			schedbook[currentPriorityLevel].ready_queue_level= headtask->next;
			headtask->next->prev=NULL;
		}
		else{
			schedbook[currentPriorityLevel].ready_queue_level=NULL;
		}

//exit(0);
		//Now Free headtask
		//free(headtask->task->context);
		//free(headtask->task->stack);
		//free(headtask);
	currentTaskLeaf=NULL;//So no need to save the context of current
	}
	else{
		//i.e. the time has not expired for the task
		if(headtask->next!=NULL)
		{

			//i.e. need to move to the end of this queue
			task_leaf_t * temp= headtask;
			while(temp->next!=NULL)
			{
				//printf("\n%d",temp->task->user_task_id);
				temp=temp->next;
			}
			//printf("\n%d",temp->task->user_task_id);
			schedbook[currentPriorityLevel].ready_queue_level=headtask->next;
			headtask->next->prev=NULL;
			temp->next=headtask;
			headtask->prev=temp;
			headtask->next=NULL;
		}
		headtask->task->status=READY;
		}



flagAllEnd=1;
		for(int i=0;i<10;i++)
		{
			if(schedbook[i].ready_queue_level!=NULL)
			{

				flagAllEnd =0;
				//i.e.this Priority level has Tasks to be scheduled.
				task_leaf_t * nextTask = schedbook[i].ready_queue_level;
				nextTask->task->status=RUNNING;

				if(currentTaskLeaf==NULL)
				{
					currentTaskLeaf = nextTask;
					currentTaskLeaf->task->sched_info.num_visits++;
					getTimeStamp();
					printf("Scheduler selected to run task %d , %d times.\n",currentTaskLeaf->task->user_task_id,currentTaskLeaf->task->sched_info.num_visits);
					setcontext(currentTaskLeaf->task->context);
					return;
				}
				else{
					//printf("LEVEL %d",i);
					task_leaf_t* old = currentTaskLeaf;
					currentTaskLeaf = nextTask;
				//	printf("OLD Task ID = %d\t NEW Task ID = %d\n",old->task->user_task_id,nextTask->task->user_task_id);
					currentTaskLeaf->task->sched_info.num_visits++;
					getTimeStamp();
					printf("Scheduler selected to run task %d , %d times.\n",currentTaskLeaf->task->user_task_id,currentTaskLeaf->task->sched_info.num_visits);
					swapcontext(old->task->context,currentTaskLeaf->task->context);
					return;
				}

			}
		}
		if(flagAllEnd==1)
		{


			//i.e if nothing to schedule switch to Main Context.
			setcontext(mainTaskContext->context);
		}
	}else{
		if(mainTaskContext!=NULL)
		{
			//If main context is saved
			for(int i=0;i<10;i++)
			{
				if(schedbook[i].ready_queue_level!=NULL)
				{
					task_leaf_t * nextTask = schedbook[i].ready_queue_level;
					nextTask->task->status=RUNNING;
					currentTaskLeaf=nextTask;
					currentTaskLeaf->task->sched_info.num_visits++;
					getTimeStamp();
					printf("Scheduler selected to run task %d , %d times.\n",currentTaskLeaf->task->user_task_id,currentTaskLeaf->task->sched_info.num_visits);
					setcontext(currentTaskLeaf->task->context);
				}
			}
		}
		else{
			return;
		}
	}
	//validateLinks();
}

void triggerTimerExpiration()
{
	if(currentTaskLeaf!=NULL)
	{
		utask_t *current = currentTaskLeaf->task;
		current->sched_info.remaining_timeslice =current->sched_info.remaining_timeslice - 10 ;//Decrement Remaining timeslice by 10ms.
		current->sched_info.current_timeslice=current->sched_info.current_timeslice + 10; //Increment current used timeslice by 10ms.
		printf("\n");
		getTimeStamp();
		printf("Task %d consumed another 10 ms, total consumed %d ms and remaining is %d ms.\n",current->user_task_id, current->sched_info.current_timeslice, current->sched_info.remaining_timeslice);
		//printf("%d\n",currentTaskLeaf->task->sched_info.remaining_timeslice);
		//printf("Currently Running %d\n",current->user_task_id);
	}else{
		return;
	}
	//printf("Hey");
}

void getPriorityString(TASK_PRIORITY pri)
{
	switch (pri) {
		case ONE:
		{
			printf("\tONE");
			break;
		}
		case TWO:
		{
			printf("\tTWO");
			break;
		}
		case THREE:
		{
			printf("\tTHREE");
			break;
		}
		case FOUR:
		{
			printf("\tFOUR");
			break;
		}
		case FIVE:
		{
			printf("\tFIVE");
			break;
		}
		case SIX:
		{
			printf("\tSIX");
			break;
		}
		case SEVEN:
		{
			printf("\tSEVEN");
			break;
		}
		case EIGHT:
		{
			printf("\tEIGHT");
			break;
		}
		case NINE:
		{
			printf("\tNINE");
			break;
		}
		case TEN:
		{
			printf("\tTEN");
			break;
		}

		default:
			break;
	}
}
void printThreadQueue(task_leaf_t * head , TASK_PRIORITY prio)
{

	if(head==NULL){
		getPriorityString(prio); //Prints Priority string
			printf("\t");//indentation
		printf("-\t\t");//Prints Task id and indentation
		printf("-\t\t\t\t\t");//current time slice and indentation
		printf("-\t\t\t\t\t");//remaining time slice and indentation
		printf("-");//default Period
		printf("\n\n");

	}

	else{
		int counter =1;
		utask_t *temp;
		while(head!=NULL)
		{
			getPriorityString(prio); //Prints Priority string
			printf("\t");//indentation
			temp= head->task;
			printf("%d\t\t",temp->user_task_id);//Prints Task id and indentation
			printf("%d\t\t\t\t\t",temp->sched_info.current_timeslice);//current time slice and indentation
			printf("%d\t\t\t\t\t",temp->sched_info.remaining_timeslice);//remaining time slice and indentation
			printf("%d",temp->sched_info.defaultPeriod);//default Period
			printf("\n\n");
			/////////////////////
			//printf("%d)",counter);
			//temp = head->task;
		//	printf("TASK_ID = %d\n",temp->user_task_id);
			//printf("\tCurrent Tileslice = %d\n",temp->sched_info.current_timeslice);
			//printf("\tRemaining Tileslice = %d\n",temp->sched_info.remaining_timeslice);
			//printf("\tPeriod = %d\n",temp->sched_info.defaultPeriod);
			/**
			 * TODO: Add more info.
			 * */
			counter++;
			head= head->next;
		}
	}

}
void showAllTasks()
{
	printf("\n###############################################################################################\n\n");
	printf("\t\t\t\tDisplaying Task Info\t\t\t\n\n");
	printf("###############################################################################################\n\n");
	printf("PRIORITY_LEVEL\tTHREAD_ID\tCurrent_TimeSlice\t\tRemaining_TimeSlice\\ttPeriod\n");

	//printf("\n---------------------------------Displaying Task Info---------------------------------\n");
	//printf("\nPriority Level ONE:\n");
	printThreadQueue(schedbook[0].ready_queue_level , ONE);
	//printf("\nPriority Level TWO:\n");
	printThreadQueue(schedbook[1].ready_queue_level , TWO);
	//printf("\nPriority Level THREE:\n");
	printThreadQueue(schedbook[2].ready_queue_level , THREE);
	//printf("\nPriority Level FOUR:\n");
	printThreadQueue(schedbook[3].ready_queue_level , FOUR);
	//printf("\nPriority Level FIVE:\n");
	printThreadQueue(schedbook[4].ready_queue_level , FIVE);
	//printf("\nPriority Level SIX:\n");
	printThreadQueue(schedbook[5].ready_queue_level , SIX);
	//printf("\nPriority Level SEVEN:\n");
	printThreadQueue(schedbook[6].ready_queue_level, SEVEN);
	//printf("\nPriority Level EIGHT:\n");
	printThreadQueue(schedbook[7].ready_queue_level , EIGHT);
	//printf("\nPriority Level NINE:\n");
	printThreadQueue(schedbook[8].ready_queue_level , NINE);
	//printf("\nPriority Level TEN:\n");
	printThreadQueue(schedbook[9].ready_queue_level , TEN);
	//printf("\n---------------------------------------------------------------------------------------\n");

}

void playTasks()
{
	//1) Add Main Context to the schedbook.

	mainTaskContext = (utask_t *)malloc(sizeof(utask_t));
	mainTaskContext->user_task_id =0;//Main Task is given task id 0.
	mainTaskContext->priority=ONE;//Main Task is given priority ONE.
	mainTaskContext->status=READY;

	mainTaskContext->sched_info.num_visits=1;
	mainTaskContext->sched_info.current_timeslice=-1;
	mainTaskContext->sched_info.defaultPeriod =-1;
	mainTaskContext->sched_info.remaining_timeslice=-1;

	mainTaskContext->context = (ucontext_t *)malloc(sizeof(ucontext_t));
	getcontext(mainTaskContext->context);
	if(flagAllEnd==1)
	{
		//i.e. when all tasks have ended.
		return;
	}
	scheduleNext();

}

void removeThreadWithID(int threadID)
{

	task_leaf_t * temp;
	for(int i=0;i<10;i++)
	{
		if(schedbook[i].ready_queue_level!=NULL)
		{

			temp = schedbook[i].ready_queue_level;
			while(temp!=NULL  )
			{
				if(temp->task->user_task_id==threadID)
					break;
				else
					temp=temp->next;
			}
			if(temp!=NULL)
			{
				getTimeStamp();
				printf("Removing thread with ID: %d from Priority Level %d",threadID,(i+1));
				//i.e. this task is to be removed.
				if(temp->next==NULL && temp->prev==NULL)
				{
					//only task in list
					schedbook[i].ready_queue_level = NULL;
				}
				else if(temp->next==NULL)
				{
					//last task in list
					temp->prev->next=NULL;
				}
				else if(temp->prev==NULL)
				{
					//first task in list
					schedbook[i].ready_queue_level = temp->next;
					temp->next->prev=NULL;
				}
				else{
					//Task in middle of list
					temp->prev->next = temp->next;
					temp->next->prev=temp->prev;
				}
			}
		}
	}
}
