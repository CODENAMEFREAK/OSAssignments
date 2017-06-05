/*
 ============================================================================
 Name        : SampleClone.c
 Author      : Anubhav Guleria
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int freakThreadFunction(void * ptr)
{
	char * message = (char*)ptr;
	printf("%d \t%s \n",getpid(),message);
	sleep(25);
	printf("Thread Over");
	return EXIT_SUCCESS;
	}

int test()
{
	printf("Test invoked\n");
	sleep(10);
return EXIT_SUCCESS;
}
int main(void) {
	printf("Parent Initiated\n");
	printf("Parent Pid\t %d\n",getpid());
	sleep(10);
	int STACK_SIZE = 1* getpagesize(); //allocationg n * getpagesize of stack to a thread.
	for(int i=0;i<10;i++)
	{
		void * freakStack = malloc(STACK_SIZE);
		void * freakStackTop = freakStack + STACK_SIZE;
		char * evenMessage ="Message for Even Thread";
			char * oddMessage ="Message for Odd Thread";
		clone(freakThreadFunction, freakStackTop,CLONE_VM|CLONE_FILES|CLONE_SIGHAND|CLONE_FS,(i%2==0)? evenMessage:oddMessage);
		sleep(2);
	}
	test();
sleep(100);
	return EXIT_SUCCESS;
}
