/*
 * FreakConditionVariable.h
 *
 *  Created on: 15-Apr-2017
 *      Author: codenamefreak
 */

#ifndef FREAKCONDITIONVARIABLE_H_
#define FREAKCONDITIONVARIABLE_H_

#include "TimeLogger.h"
class BlockedThreadQueue{
private:
	pthread_t parkedThread;
public:

	BlockedThreadQueue * nextNode;

	pthread_t getParkedThread()
	{
		return parkedThread;
	}
	BlockedThreadQueue(pthread_t threadToPark)
	{
		parkedThread = threadToPark;
		nextNode = NULL;
	}
};

class FreakConditionVariable
{
	private:
		Freak_Mutex_Semaphore_BUSY_WAIT condSem;


	public:
	void wait(FreakLock_BUSY_WAIT * currentLock)
	{
		cout<<getCurrentTimeString()<<"Lock Released on wait().\t"<<pthread_self()<<endl;
		currentLock->releaseLock();//Can release lock as not update any shared variable
		//cout<<"Released Lock inside Wait"<<endl;
		//cout<<"Lock "<<currentLock;
		//pthread_yield();
		condSem.atomic_P();
		//cout<<"Released Lock inside Wait 2"<<endl;
		currentLock->acquireLock();//To acquire lock after waking up by notify.
		cout<<getCurrentTimeString()<<"Lock Acquired on notify().\t"<<pthread_self()<<endl;
	}
	void notify()
	{
		cout<<getCurrentTimeString()<<"notify() invoked.\t"<<pthread_self()<<endl;
		//cout<<"Notify Invoked";
		condSem.atomic_V();//Will just this work???????
	}
	FreakConditionVariable()
	{
		condSem.atomic_P();//This is done as I have just used semaphore by setting initially as 1. So it will be 0 when wait is invoked.
		//front=rear=NULL;
	}

};

class FreakConditionVariable_Non_Busy_Wait
{
	private:
		Freak_Mutex_Semaphore_NON_BUSY_WAIT condSem;


	public:
	void wait(FreakLock_NON_BUSY_WAIT * currentLock)
	{
		currentLock->releaseLock();//Can release lock as not update any shared variable
		//cout<<"Released Lock inside Wait"<<endl;
		//cout<<"Lock "<<currentLock;
		//pthread_yield();
		condSem.atomic_P();
		//cout<<"Released Lock inside Wait 2"<<endl;
		currentLock->acquireLock();//To acquire lock after waking up by notify.
	}
	void notify()
	{
		//cout<<"Notify Invoked";
		condSem.atomic_V();//Will just this work???????
	}
	FreakConditionVariable_Non_Busy_Wait()
	{
		condSem.atomic_P();//This is done as I have just used semaphore by setting initially as 1. So it will be 0 when wait is invoked.
		//front=rear=NULL;
	}

};

#endif /* FREAKCONDITIONVARIABLE_H_ */
