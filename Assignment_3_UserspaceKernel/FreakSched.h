/*
 * FreakSched.h
 *
 *  Created on: 30-Mar-2017
 *      Author: codenamefreak
 */

#ifndef FREAKSCHED_H_
#define FREAKSCHED_H_

#include "UserTask.h"
#define MIN_PRIORITY_LEVEL 0
#define NUM_PRIORITY_LEVELS 10

static utask_t * READY_QUEUE = NULL;
static utask_t * current = NULL;	//Points to the last added task.

typedef struct taskKeeper{
	utask_t * task;
	struct taskKeeper * prev;
	struct taskKeeper * next;
}task_leaf_t;

typedef struct priority_node{
	TASK_PRIORITY priority;
	task_leaf_t * ready_queue_level;
}priority_node_t;



static priority_node_t schedbook[10];

/**
 * Initializes Scheduler i.e. will initialize the priority queue book.
 * */
int initializeScheduler();

int addNewUserTask(utask_t * newTask); //Adds new task to the queue.

/**
 *
 * void triggerTimerExpiration();
 * Updates the value of current timeslice expiry in currently executing job.
 * */
void triggerTimerExpiration();

/**
 * Displays all current tasks.
 * */
void showAllTasks();

/**
 * Saves mainContext and initiates scheduling.
 * */
void playTasks();

void removeThreadWithID(int threadID);

void validateLinks();
#endif /* FREAKSCHED_H_ */
