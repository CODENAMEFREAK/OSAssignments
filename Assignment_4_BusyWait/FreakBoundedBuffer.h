/*
 * FreakBoundedBuffer.h
 *
 *  Created on: 14-Apr-2017
 *      Author: codenamefreak
 */

#ifndef FREAKBOUNDEDBUFFER_H_
#define FREAKBOUNDEDBUFFER_H_
#include<iostream>
#include "TimeLogger.h"

using namespace std;
class BoundedBuffer{

private:
	int testParam;
	int BUFF_SIZE;
	int  *CIRCULAR_BUFFER;
	int front,rear;
	bool isEmpty()
	{
		/*if(testParam==0)
			return true;
		else
			return false;*/
		//cout<<"In isempty()\t"<<front<<"\t"<<rear<<endl;
		if(front==rear && front==-1)
			return true;
		else
			return false;
	}

	bool isFull()
	{
		/*if(testParam==BUFF_SIZE)
			return true;
		else
			return false;*/

		if(((rear+1)%BUFF_SIZE)==(front%BUFF_SIZE)&&(front!=rear))
			return true;
		else
			return false;
	}
public:
	/**
	 * returns -1 if the buffer is FULL and 0 otherwise.
	 * */
	int produceItem(int item)
	{
		if(isFull())
		{
			cout<<getCurrentTimeString()<<"Buffer Full.\t"<<pthread_self()<<endl;
			return -1;
		}
		else{
	/*		testParam++;
			cout<<"Test Parameter produced "<<testParam<<endl;
	*/
			if(isEmpty())
			{
				front=0;
				//rear will be updated in next line after if;
			}
			//cout<<"Producing at\t"<<(rear+1)%BUFF_SIZE<<endl;
			CIRCULAR_BUFFER[(rear+1)%BUFF_SIZE]= item;

			rear++;
			return 0;
		}
	}
	/**
		 * returns -1 if the buffer is FULL and 0 otherwise.
		 * */
	int consumeItem()
	{
		if(isEmpty())
		{
			cout<<getCurrentTimeString()<<"Buffer Empty.\t"<<pthread_self()<<endl;
		 	return -1;
		}
		else{
	/*		testParam--;
			cout<<"Test Parameter consumed "<<testParam<<endl;
	*/
			int itemToReturn = CIRCULAR_BUFFER[front%BUFF_SIZE];
			if((front%BUFF_SIZE) == (rear%BUFF_SIZE))
			{
				front = rear = -1;
			}
			else{
				front++;
			}
			return 0;
		}
	}
	void displayBuffer()
	{

		if(isEmpty())
		{
			cout<<getCurrentTimeString()<<"Nothing to display.\t"<<pthread_self()<<endl;
		}
		else{
			//cout<<"Test Parameter is "<<testParam<<endl;
			int temp=front;
			while(temp<rear)
			{
				cout<<CIRCULAR_BUFFER[temp%BUFF_SIZE]<<" , ";
				temp++;
			}
			cout<<CIRCULAR_BUFFER[temp%BUFF_SIZE]<<endl;
			cout<<endl;
		}
	}
	BoundedBuffer(int buffersize)
	{
		BUFF_SIZE = buffersize;
		testParam =0;
		CIRCULAR_BUFFER =new int[BUFF_SIZE];
		front=rear=-1;
	}


};



#endif /* FREAKBOUNDEDBUFFER_H_ */
