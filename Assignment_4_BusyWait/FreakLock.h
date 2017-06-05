/*
 * FreakLock.h
 *
 *  Created on: 14-Apr-2017
 *      Author: codenamefreak
 */

#ifndef FREAKLOCK_H_
#define FREAKLOCK_H_
#include "FreakSemaphore.h"
#include <pthread.h>
#include "TimeLogger.h"

class FreakLock_BUSY_WAIT{
private:
	 pthread_t threadHolder;

	 Freak_Mutex_Semaphore_BUSY_WAIT semaphore;

	 Freak_Mutex_Semaphore_BUSY_WAIT sem_allow_multi_calls;
public:
	 void acquireLock()
	 {
		 //cout<<getCurrentTimeString()<<"acquireLock()\t"<<pthread_self()<<endl;
		// cout<<"I try"<<endl;
		 sem_allow_multi_calls.atomic_P();

		 if(pthread_equal(threadHolder,pthread_self()))
		 {
			 //cout<<"Same thread entering other protected method with thread ID\t"<<pthread_self()<<endl;//add more info
			 //i.e the same thread is calling other method in the Monitor.
			 //cout<<getCurrentTimeString()<<"acquireLock() multi-call\t"<<pthread_self()<<endl;
			 sem_allow_multi_calls.atomic_V();
			 return;
		 }
		 else{
			 sem_allow_multi_calls.atomic_V();//And in next line it will block
		 }
		// cout<<"I try"<<endl;
		 semaphore.atomic_P();//First try to get hold of the lock.
		 //cout<<getCurrentTimeString()<<"acquireLock() successful\t"<<pthread_self()<<endl;
		 //cout<<"I try"<<endl;
		 threadHolder = pthread_self();//Update the current thread value
		 //cout<<"Lock acquired by thread with thread ID\t"<<pthread_self()<<endl; //Display information by thread getting the lock
	 }

	 void releaseLock()
	 {
		// cout<<getCurrentTimeString()<<"releaseLock()\t"<<pthread_self()<<endl;
		 //cout<<"Lock released by thread with thread ID\t"<<pthread_self()<<endl; //Display information by thread releasing the lock.
		 //First Print information
		 semaphore.atomic_V();//Then release the lock
	 }

};

class FreakLock_NON_BUSY_WAIT{
private:
	pthread_t threadHolder;

	Freak_Mutex_Semaphore_NON_BUSY_WAIT semaphore;

	Freak_Mutex_Semaphore_NON_BUSY_WAIT sem_allow_multi_calls;

public:
	void acquireLock()
	{
		//cout<<"First\n";
		sem_allow_multi_calls.atomic_P();
		//cout<<"First End\n";
		//cout<<"Hello World2";

    	 if(pthread_equal(threadHolder,pthread_self()))
		 {
				// cout<<"Same thread entering other protected method"<<endl;//add more info
				 //i.e the same thread is calling other method in the Monitor.
			 sem_allow_multi_calls.atomic_V();
				 return;
		 }
		 else{
			 //cout<<"Hello World 3";
				 sem_allow_multi_calls.atomic_V();//And in next line it will block
		}
    	 //cout<<"Hello World";
		semaphore.atomic_P();//First try to get hold of the lock.
		//cout<<"Lock acquired by thread with thread ID\t"<<pthread_self()<<endl; //Display information by thread getting the lock}
		//threadHolder = pthread_self();//Update the current thread value
    	//cout<<"Lock acquired by thread with thread ID\t"<<pthread_self()<<endl; //Display information by thread getting the lock}

	}
	void releaseLock()
	{
		 //cout<<"Lock released by thread with thread ID\t"<<pthread_self()<<endl; //Display information by thread releasing the lock.
		 //First Print information
		 semaphore.atomic_V();//Then release the lock
	}
};


#endif /* FREAKLOCK_H_ */
