/*
 * FreakSemaphore.h
 *
 *  Created on: 14-Apr-2017
 *      Author: codenamefreak
 */

#ifndef FREAKSEMAPHORE_H_
#define FREAKSEMAPHORE_H_
#include<iostream>
#include <cstdint>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#define SIGRT_SEM_WAKEUP  (SIGRTMIN +2) //Real Signal for waking up any waiting thread.
using namespace std;

extern "C" void _spinlock( uint64_t * mutex);//For BUSYWAIT
extern "C" uint64_t* _spin_unlock( uint64_t* mutex);//FOR BUSYWAIT
extern "C" uint64_t _lock( uint64_t * mutex);//FOR NON-BUSY-WAIT
extern "C" uint64_t* _unlock( uint64_t* mutex);//FOR NON-BUSY-WAIT
//extern "C" unsigned int _test(unsigned int *pointer);
class Freak_Mutex_Semaphore_BUSY_WAIT{

private:
	uint64_t mutex ;
public:
	Freak_Mutex_Semaphore_BUSY_WAIT()
{
		//			_spin_unlock(&mutex);
		mutex =1;
}
	void atomic_P()
	{
		////cout<<"Called atomic_P() "<<endl;
		_spinlock(&mutex);
		////cout<<"Got Lock"<<endl;
	}

	void atomic_V()
	{
		_spin_unlock(&mutex);
	}
};

class BlockedThread{
private:
	pthread_t blockedThread;

public:
	BlockedThread * next;

	pthread_t getThread()
	{
		return blockedThread;
	}

	BlockedThread(pthread_t thread)
	{
		blockedThread = thread;
		next =NULL;
	}
};

class Freak_Mutex_Semaphore_NON_BUSY_WAIT
{

private:
	Freak_Mutex_Semaphore_BUSY_WAIT finiteBusyWait;//Used while updating Blocked Queue. Asmany threads can block at same time.
	uint64_t mutex;
	BlockedThread * blockedThreadQueueFront;
	BlockedThread * blockedThreadQueueRear;


	void addThreadToBlockedQueue()
	{//Need to make this method synchronized: Can use Busy Wait as wait is deterministic
		BlockedThread * newBlockedThread = new BlockedThread(pthread_self());
		finiteBusyWait.atomic_P();
		if(blockedThreadQueueFront == NULL)
		{

			blockedThreadQueueFront=blockedThreadQueueRear = newBlockedThread;
			//cout<<"\nAdded\t"<<blockedThreadQueueFront->getThread()<<" and "<<&blockedThreadQueueFront<<endl;
		}
		else{

			blockedThreadQueueRear->next = newBlockedThread;
			blockedThreadQueueRear=newBlockedThread;
		}
		finiteBusyWait.atomic_V();
	}

	BlockedThread * unblockFrontThread()
	{
		finiteBusyWait.atomic_P();
		//Need not to synchronize this as only single thread will at any time will call this.
		//cout<<"Unblocking invoked";
		BlockedThread * toReturn;
		if(blockedThreadQueueFront==NULL)
		{
			//cout<<"Unblocking invoked NULL\t"<<&blockedThreadQueueFront;
			toReturn = NULL;
		}
		else if(blockedThreadQueueFront == blockedThreadQueueRear)
		{

			//cout<<"Unblocking setting NULL";
			toReturn =blockedThreadQueueFront;
			toReturn->next=NULL;
			blockedThreadQueueFront=blockedThreadQueueRear=NULL;
		}
		else{
			toReturn =blockedThreadQueueFront;
			blockedThreadQueueFront=blockedThreadQueueFront->next;
			toReturn->next=NULL;
		}
		finiteBusyWait.atomic_V();
		return toReturn;
	}
	void setFreakWaitForRTSignal()
	{
		sigset_t mysigmask;
		siginfo_t siginfo;

		sigemptyset(&mysigmask);
		sigaddset(&mysigmask,SIGRT_SEM_WAKEUP);
		pthread_sigmask(SIG_BLOCK, &mysigmask, NULL); //Blocked for that thread

		addThreadToBlockedQueue(); //O(1) operation
		//cout<<"Added to blocked Queue"<<endl;
		sigwaitinfo(&mysigmask,&siginfo);
		//cout<<"After wakeup you called me so can here set mutex to 1";
		_unlock(&mutex);//unlocking here since will execute after the pthread_sigqueue from atomic_V
	}
public:
	void atomic_P()
	{

		if(_lock(&mutex))//if assembly returns lock is already acquired
		{
			////cout<<"Lock Taken"<<endl;
		}
		else{
			//get
			//cout<<"Blocking thread\t"<<pthread_self()<<endl;
			setFreakWaitForRTSignal();
		}

	}

	void atomic_V()
	{
		////cout<<"Calling V";
		union sigval sval;
		BlockedThread *nextToUnblock =unblockFrontThread();
		if(nextToUnblock!=NULL){
			////cout<<"Hello World 3";
			//cout<<"sending to"<<nextToUnblock->getThread();
			pthread_sigqueue(nextToUnblock->getThread(), SIGRT_SEM_WAKEUP, sval);
		}
		else{
			_unlock(&mutex);
		}

	}
	Freak_Mutex_Semaphore_NON_BUSY_WAIT()
	{
		/**
		 * Register for SIGRT_SEM_WAKEUP
		 * */
		////cout<<"Invoked Constructor\n"<<i;
		////cout<<"Front Address: "<<&blockedThreadQueueFront<<endl;
		mutex =1;
		blockedThreadQueueFront =NULL;//Initialize the Blocked Thread Queue waiting on this Semaphore.
		blockedThreadQueueRear =NULL;
	}
};


#endif /* FREAKSEMAPHORE_H_ */
