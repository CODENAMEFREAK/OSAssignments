/*
 ==================================================================================================
 Name        : SamplePThreads.c
 Author      : Anubhav Guleria
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style. Compile with 'gcc -pthread SamplePThreads.c'
 	 	 	   In other console run 'watch -n 1 pstree -c PID output on starting of this process.'
 ==================================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


void * evenThreadFunction(void * ptr)
{
	char * message = (char*)ptr;
	printf("Thread ID:%d \t %s \n",pthread_self(),message);
	sleep(25);
	printf("Thread Over\n");
	return EXIT_SUCCESS;
	}


int main(void) {
	printf("Parent Initiated\n");
	printf("Parent Pid\t %d\n",getpid());
	sleep(10);
	pthread_t threadPool[10]; //Thread pool for 10 threads.
	char * evenMessage ="Message for Even Thread";
	char * oddMessage ="Message for Odd Thread";
	for(int i=0;i<10;i++)
	{
		pthread_create(&threadPool[i] , NULL, evenThreadFunction, (i%2==0)? evenMessage:oddMessage);
		sleep(2);
	}
	sleep(100);
	return EXIT_SUCCESS;
}
