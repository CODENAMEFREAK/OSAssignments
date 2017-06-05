/*
 * MainEngine.cpp
 *
 *  Created on: 10-Apr-2017
 *      Author: codenamefreak
 */


//#define _GNU_SOURCE
#include "FreakMonitor.h"
#include "FreakBoundedBuffer.h"
#include <pthread.h>
#include <thread>
#include <stdlib.h>
#include "TimeLogger.h"
using namespace std;


BoundedBuffer * sharedBoundedBuffer ;//= new BoundedBuffer(10);
FreakMonitor_BUSY_WAIT * busywaitMonitor;// =new FreakMonitor_BUSY_WAIT(sharedBoundedBuffer);


cpu_set_t producercpuset;
cpu_set_t consumercpuset;
unsigned numThreadsSupported;
int bufferSize;
int numProducers;
int numConsumers;
bool beginAll=false;

void * produce(void * item)
{
	while(!beginAll);
	cout<<getCurrentTimeString()<<"Starting Thread to Produce.\t"<<pthread_self()<<endl;
	int* i =(int*) item;
	srand(time((NULL)));
	int randomSleepTime = (rand()%10);
	while(1){
		i =(int*) item;
		*i=((*i)+randomSleepTime)%10;
		busywaitMonitor->produceToBuffer(*i);
		srand(time((NULL)));
		randomSleepTime = (rand()%10);
		cout<<getCurrentTimeString()<<"Producer Thread sleeping for "<<randomSleepTime<<" seconds.\t"<<pthread_self()<<endl;
		sleep(randomSleepTime);
		cout<<getCurrentTimeString()<<"Producer Thread up after "<<randomSleepTime<<".\t seconds"<<pthread_self()<<endl;
	}
	cout<<getCurrentTimeString()<<"Ending Thread Produced.\t"<<pthread_self()<<endl;
	}
void * consume(void * item)
{
	while(!beginAll);
	cout<<getCurrentTimeString()<<"Starting Thread to Consume.\t"<<pthread_self()<<endl;
	srand(time((NULL)));
	int randomSleepTime = (rand()%10);
	while(1)
	{
		busywaitMonitor->consumeFromBuffer();
		srand(time((NULL)));
		randomSleepTime = (rand()%10);
		cout<<getCurrentTimeString()<<"Consumer Thread sleeping for "<<randomSleepTime<<" seconds.\t"<<pthread_self()<<endl;
		sleep(randomSleepTime);
		cout<<getCurrentTimeString()<<"Consumer Thread up after "<<randomSleepTime<<".\t seconds"<<pthread_self()<<endl;
	}

	cout<<getCurrentTimeString()<<"Ending Thread Consumed.\t"<<pthread_self()<<endl;
}
void ignition()
{

	CPU_ZERO(&producercpuset);
	CPU_ZERO(&consumercpuset);
	numThreadsSupported =std::thread::hardware_concurrency();

	for(int i=0;i<numThreadsSupported;i++)
	{
		if(i%2==0){
			CPU_SET(i,&producercpuset);
		}
		else{
			CPU_SET(i,&consumercpuset);
		}
	}
	bufferSize=numThreadsSupported*numThreadsSupported;
	numProducers=numConsumers=bufferSize/2;
	cout<<"======================================================================================="<<endl;
	cout<<getCurrentTimeString()<<"Number of concurrent threads supported "<<numThreadsSupported<<endl;
	cout<<getCurrentTimeString()<<"Number of cores in Producer CPU set are: "<<CPU_COUNT(&producercpuset)<<endl;
	cout<<getCurrentTimeString()<<"Number of cores in Consumer CPU set are: "<<CPU_COUNT(&consumercpuset)<<endl;
	cout<<getCurrentTimeString()<<"Buffer Size: "<<bufferSize<<endl;
	cout<<getCurrentTimeString()<<"Number of Producer Threads: "<<numProducers<<endl;
	cout<<getCurrentTimeString()<<"Number of Consumer Threads: "<<numConsumers<<endl;
	cout<<"======================================================================================="<<endl<<endl;
	sleep(1);
	}
int main()
{
	ignition();

	sharedBoundedBuffer = new BoundedBuffer(bufferSize);
	pthread_t producerThreads[numProducers];
	pthread_t consumerThreads[numConsumers];

	busywaitMonitor =new FreakMonitor_BUSY_WAIT(sharedBoundedBuffer);

	cout<<"======================================================================================="<<endl<<endl;
	cout<<"\t\t\tThread Affinity Table\t\t\t"<<endl<<endl;
	cout<<"======================================================================================="<<endl<<endl;
	cout<<"##TYPE##\t\t##CPU_SET_SIZE##\t\t##Thread ID##"<<endl<<endl;
	cout<<"---------------------------------------------------------------------------------------"<<endl;
	for(int i=0;i<numConsumers;i++)
	{

		//cout<<consumerThreads[i]<<"\t"<<CPU_COUNT(&consumercpuset)<<"\t"<<&consumercpuset<<endl;
		//pthread_setaffinity_np(consumerThreads[i],CPU_COUNT(&consumercpuset), &consumercpuset);
		pthread_create((consumerThreads+i), NULL, consume, NULL);
		if(pthread_setaffinity_np(consumerThreads[i],CPU_COUNT(&consumercpuset), &consumercpuset)!=0)
		{
			cout<<"Can't set affinity"<<endl;
		}
		cout<<"CONSUMER\t\t\t"<<CPU_COUNT(&consumercpuset)<<"\t\t\t"<<consumerThreads[i]<<endl<<endl;
	}
	int * argVal ;
	for(int i=0;i<numProducers;i++)
	{

		argVal=new int;
		*argVal=i;
			//pthread_setaffinity_np(producerThreads[i],CPU_COUNT(&producercpuset), &producercpuset);
		pthread_create((producerThreads+i), NULL, produce, argVal);
		if(pthread_setaffinity_np(producerThreads[i],CPU_COUNT(&producercpuset), &producercpuset)!=0)
			cout<<"Can't set affinity"<<endl;
		cout<<"PRODUCER\t\t\t"<<CPU_COUNT(&producercpuset)<<"\t\t\t"<<producerThreads[i]<<endl<<endl;
		}

	char ch;
	cout<<getCurrentTimeString()<<"Press any Key to begin."<<endl;
	cin>>ch;
	cout<<getCurrentTimeString()<<"Initiating threads"<<endl;

	beginAll=true;


	for(int i=0;i<numConsumers;i++)
	{
		pthread_join(consumerThreads[i], NULL);
	}

	for(int i=0;i<numProducers;i++)
	{
		pthread_join(producerThreads[i], NULL);
	}


	cout<<getCurrentTimeString()<<"Main over"<<endl;

}

