
#include "FreakThread.h"

static task_u * READY_QUEUE = NULL;
static int NEXT_USER_THREAD_ID =1;		//initially this value is set as 1.
static task_u * current = NULL;	//Points to the last added task.

static int remaining_main_method_visits;

task_u * getNextReadyTask()
{

		//task_u * nextReadyTask = READY_QUEUE;


		//i.e. there are atleast two tasks in ready queue
		return READY_QUEUE->next;//FIFO as of now can change this.
}

void scheduleNext()
{

	getTimeStamp();
	printf("Schedule Next Invoked.\n");
	if(READY_QUEUE==current){
		if(READY_QUEUE->user_task_id==1)//i.e. it is the maintask
		{
			if(!(READY_QUEUE->num_visits<remaining_main_method_visits))
				{
					exit(0); //i.e. if main has already been visited for maximum allowed times.
				}
		}
		READY_QUEUE->num_visits++;
		getTimeStamp();
		printf("Visiting Thread %d , %d time.\n",READY_QUEUE->user_task_id,READY_QUEUE->num_visits);
		showList();
		//printf("Last Time here\n");
		return;//i.e. only one task
	}
	else{
		//Need to make logic for executing first ready task
		task_u * nextTask = getNextReadyTask();

		if(nextTask->user_task_id==1)//i.e. it is the maintask
				{
					if(!(nextTask->num_visits<remaining_main_method_visits))
						{
							exit(0); //i.e. if main has already been visited for maximum allowed times.
						}
				}

					current->next=READY_QUEUE; //added Front to the end.
					READY_QUEUE->prev = current;

					READY_QUEUE = nextTask;
					nextTask->prev->next =NULL;
					nextTask->prev=NULL;
					current=current->next;
					current->status=READY;
					//printf("Selecting %d \n",READY_QUEUE->user_task_id);
					READY_QUEUE->status=RUNNING;
					READY_QUEUE->num_visits++;
					getTimeStamp();
					printf("Visiting Thread %d , %d time.\n",READY_QUEUE->user_task_id,READY_QUEUE->num_visits);
					showList();
					swapcontext(current->context,READY_QUEUE->context);



			}
}


char * getTaskStaus(TASK_STATUS stat)
{
	switch (stat) {
		case READY:
		{
			return "READY";
		}
		case RUNNING:
		{
			return "RUNNING";
		}
		case EXIT:
		{
			return "EXIT";
		}
		default:
			break;
	}
	return  "";
}



/**
 * To check the links in list
 * */
void showList()
{
	printf("\n###############################################################################################\n\n");
	printf("\t\t\t\tDisplaying READY QUEUE\t\t\t\n\n");
	printf("###############################################################################################\n\n");
	printf("THREAD_ID\tNUM_VISITS\tTASK_STATUS\n");

	task_u * temp =READY_QUEUE;

	while(temp!=NULL)
	{
		printf("   %d\t\t   %d\t\t%s\n",temp->user_task_id,temp->num_visits, getTaskStaus(temp->status));
		temp=temp->next;
	}
	printf("\n\n");
}


static int _initThreadEngine(){

	NEXT_USER_THREAD_ID++; //Give Next Thread ID 2 , as 1 is reserved for MainTask



	/**
	 * Add other initialization tasks here.
	 * */
	return 0;
}

static void removeFreakContext(task_u * completedTask)
{
	//Since it just finished execution so this was at the front of the queue.
	READY_QUEUE=completedTask->next;
	READY_QUEUE->prev=NULL;
	printf("Removing %d\n.",completedTask->user_task_id);
	free(completedTask->context);
	free(completedTask);
	showList();
	if(READY_QUEUE->user_task_id==1)//i.e. it is the maintask
			{
				if(!(READY_QUEUE->num_visits<remaining_main_method_visits))
					{
						exit(0); //i.e. if main has already been visited for maximum allowed times.
					}
			}
	printf("Visiting Thread %d , %d time.",READY_QUEUE->user_task_id,READY_QUEUE->num_visits);
	setcontext(READY_QUEUE->context);
}


static void playFreakTask(void *task)
{
	task_u  * playTask = (task_u *) task;
	playTask->status=RUNNING;	 //First set the status as running
	playTask->userTask(playTask->args);				  //Then start execution
	playTask->status=EXIT;	  //Once execution is completed set the status as EXIT
	removeFreakContext(playTask);	//Then remove the context from the Ready Queue.
}



static int addNewUserTask(void(*freakTask)(void *),void *arg)
{


	task_u * newTask = (task_u *)malloc(sizeof(task_u));
	newTask->next=NULL;
	newTask->prev=current;
	newTask->user_task_id=NEXT_USER_THREAD_ID++;
	newTask->num_visits =0;
	newTask->userTask=freakTask;
	newTask->args=arg;
	if(current==NULL)
	{
		//i.e. adding first task
		READY_QUEUE=newTask;
	}
	else{
		current->next=newTask;
	}
	current=newTask;
	//Upto this point have created and added Task in the ready Queue
	newTask->status = READY;	//1) Set status as READY

	newTask->context =(ucontext_t *)malloc(sizeof(ucontext_t));
	getcontext(newTask->context);

	newTask->context->uc_link =NULL;	//TODO:See the effect of this.
	newTask-> stack = malloc(STACK_SIZE); //stack is pointed to by stack pointer in task.
	newTask->context->uc_stack.ss_sp=newTask->stack; //pointer to stack is set in context.
	newTask->context->uc_stack.ss_size=STACK_SIZE;
	newTask->context->uc_stack.ss_flags=0;//TODO:See the effect of this.
	makecontext(newTask->context, playFreakTask,1, newTask);//Invoke playFreakTask with newTask that has method to be invoked and arguments that are to be passed to it.
	return current->user_task_id;
	}



int create_freak_thread(void(*freakTask)(void *),void * arg)
{

	if(current==NULL)	//For the first time READY_QUEUE will be empty
	{
		if(_initThreadEngine()==-1)
		{
			return -1;
		}
	}
	//printf("My %d\n",NEXT_USER_THREAD_ID);
	//showList();
	return addNewUserTask(freakTask,arg);	//Once READY_QUEUE initialized add the new task.
}

/**
 * Main application after creating all the threads invokes play method to begin scheduling.
 * */
void play(int numTimesMainThreadCalled)
{
	// 1)Add the main context to the ready queue.
	task_u * mainTaskContext = (task_u *)malloc(sizeof(task_u));
	mainTaskContext->next=READY_QUEUE;
	mainTaskContext->prev=NULL;
	mainTaskContext->status=READY;
	mainTaskContext->userTask =NULL;
	mainTaskContext->user_task_id=1;
	mainTaskContext->num_visits=1;
	mainTaskContext->context=(ucontext_t *)malloc(sizeof(ucontext_t));
	getcontext(mainTaskContext->context);
	READY_QUEUE->prev = mainTaskContext;
	READY_QUEUE =mainTaskContext;

	showList();
	// 2)And schedule.
	remaining_main_method_visits = numTimesMainThreadCalled;
	scheduleNext();

	while(READY_QUEUE->num_visits<remaining_main_method_visits)//So that return only when visited specified number of times
	{
		getTimeStamp();
		printf("Main Thread Scheduled.\n");
		sleep(1);
		scheduleNext();
	}


}


