/*
 * FreakMonitor.h
 *
 *  Created on: 15-Apr-2017
 *      Author: codenamefreak
 */

#ifndef FREAKMONITOR_H_
#define FREAKMONITOR_H_
#include "FreakLock.h"
#include "FreakBoundedBuffer.h"
#include "FreakConditionVariable.h"
#include <iostream>
#include "TimeLogger.h"
using namespace std;

class FreakMonitor_BUSY_WAIT{
private:
	BoundedBuffer *monitoredBoundedBuffer;
	FreakLock_BUSY_WAIT freaklock_t;
	FreakConditionVariable cond_buff_Not_Full, cond_buff_Not_Empty;
public:
 int produceToBuffer(int item)
 {
	 //cout<<"I try";
	// cout<<"Lock Produce "<<&freaklock_t;
	 	 freaklock_t.acquireLock();
	 	cout<<getCurrentTimeString()<<"Producer Lock Acquired\t"<<pthread_self()<<endl;
	 	 	 int returnValue;
	 	 	 while(monitoredBoundedBuffer->produceItem(item)==-1)
	 	 	//if(monitoredBoundedBuffer->produceItem(1)==-1)
	 	 	 	 {
	 	 	 		 //i.e. Buffer is full.
	 	 	 		 cond_buff_Not_Full.wait(&freaklock_t);//i.e. wait until condition "buffer is not full" satisfies.
	 	 	 	 }

	 	 	 	 monitoredBoundedBuffer->displayBuffer();
	 	 	 	 //while(1);
	 	 freaklock_t.releaseLock();
	 	cout<<getCurrentTimeString()<<"Producer Lock Released\t"<<pthread_self()<<endl;
	 	 cond_buff_Not_Empty.notify();//i.e. condition "buff is not empty" is satisfied.
	 	 return returnValue;
 }

 int consumeFromBuffer()
 {
	 	 freaklock_t.acquireLock();
	 	cout<<getCurrentTimeString()<<"Consumer Lock Acquired\t"<<pthread_self()<<endl;
	 	 	 int returnValue;
	 	 	 while(monitoredBoundedBuffer->consumeItem()==-1)
	 	 	//	if(monitoredBoundedBuffer->consumeItem()==-1)
	 	 	 {
	 	 		 //i.e. buffer is empty
	 	 		cond_buff_Not_Empty.wait(&freaklock_t);//i.e. wait until condition "buffer is non empty" satisfies.
	 	 	 }
	 	 	monitoredBoundedBuffer->displayBuffer();
	 	 freaklock_t.releaseLock();
	 	cout<<getCurrentTimeString()<<"Consumer Lock Released\t"<<pthread_self()<<endl;
	 	 cond_buff_Not_Full.notify();//i.e. condition "buffer not full" satisfies.

	 	 return returnValue;
 }

 FreakMonitor_BUSY_WAIT(BoundedBuffer *buff)
 {
	 cout<<getCurrentTimeString()<<"FreakMonitor_BUSY_WAIT initialized."<<endl;
	 /**
	  * Inititalize one Monitor for all threads.
	  *  */
	 monitoredBoundedBuffer =buff;
 }

};

class FreakMonitor_NON_BUSY_WAIT{
private:
	BoundedBuffer *monitoredBoundedBuffer;
	FreakLock_NON_BUSY_WAIT freaklock_t;
	FreakConditionVariable_Non_Busy_Wait cond_buff_Not_Full, cond_buff_Not_Empty;
public:
 int produceToBuffer(int item)
 {
	 //cout<<"I try";
	// cout<<"Lock Produce "<<&freaklock_t;
	 	 freaklock_t.acquireLock();
	 	cout<<getCurrentTimeString()<<"Producer Lock Acquired\t"<<pthread_self()<<endl;
	 	 	 int returnValue;
	 	 	 while(monitoredBoundedBuffer->produceItem(item)==-1)
	 	 	//if(monitoredBoundedBuffer->produceItem(1)==-1)
	 	 	 	 {
	 	 	 		 //i.e. Buffer is full.
	 	 	 		 cond_buff_Not_Full.wait(&freaklock_t);//i.e. wait until condition "buffer is not full" satisfies.
	 	 	 	 }

	 	 	 	 monitoredBoundedBuffer->displayBuffer();
	 	 	 	 //while(1);
	 	 freaklock_t.releaseLock();
	 	cout<<getCurrentTimeString()<<"Producer Lock Released\t"<<pthread_self()<<endl;
	 	 cond_buff_Not_Empty.notify();//i.e. condition "buff is not empty" is satisfied.
	 	 return returnValue;
 }

 int consumeFromBuffer()
 {
 	 freaklock_t.acquireLock();
 	cout<<getCurrentTimeString()<<"Consumer Lock Acquired\t"<<pthread_self()<<endl;
 	 	 int returnValue;
 	 	 while(monitoredBoundedBuffer->consumeItem()==-1)
 	 	 {
 	 		 //i.e. buffer is empty
 	 		cond_buff_Not_Empty.wait(&freaklock_t);//i.e. wait until condition "buffer is non empty" satisfies.
 	 	 }
 	 	monitoredBoundedBuffer->displayBuffer();
 	 freaklock_t.releaseLock();
 	cout<<getCurrentTimeString()<<"Consumer Lock Released\t"<<pthread_self()<<endl;
 	 cond_buff_Not_Full.notify();//i.e. condition "buffer not full" satisfies.

 	 return returnValue;
}

 FreakMonitor_NON_BUSY_WAIT(BoundedBuffer *buff)
 {
	 cout<<getCurrentTimeString()<<"FreakMonitor_NON_BUSY_WAIT initialized."<<endl;
	 /**
	  * Inititalize one Monitor for all threads.
	  *  */
	 monitoredBoundedBuffer = buff;
 }
};

#endif /* FREAKMONITOR_H_ */
