/*
 * UserTask.h
 *
 *  Created on: 23-Mar-2017
 *      Author: codenamefreak
 */

#ifndef USERTASK_H_
#define USERTASK_H_

#include <ucontext.h>
#include <unistd.h>
#define STACK_SIZE getpagesize()
typedef enum {READY,RUNNING,EXIT} TASK_STATUS;

typedef struct usertask{
	int user_task_id;	//Locally generated ID.
	ucontext_t * context; //to keep track of machine context.
	void * stack; //to keep track of stack.
	int num_visits;
	TASK_STATUS status;
	void(*userTask)(void *); //To userTask Function.
	void *args; //To user args.
	struct usertask * prev;//To prev Node.
	struct usertask * next;//To next Node.
}task_u;

#endif /* USERTASK_H_ */
