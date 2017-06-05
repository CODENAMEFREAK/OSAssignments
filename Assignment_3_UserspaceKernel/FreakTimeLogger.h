/*
 * FreakTimeLogger.h
 *
 *  Created on: 21-Apr-2017
 *      Author: codenamefreak
 */

#ifndef FREAKTIMELOGGER_H_
#define FREAKTIMELOGGER_H_

#include <time.h>
#include <stdio.h>

void getTimeStamp()
{
	time_t logtime =time(NULL);
	 char * ch =asctime( localtime(&logtime));
	 while((*ch)!='\n')
		 {
		 printf("%c",*ch);
		 ch++;
		 }
	printf("\t");
}

void displayDelayedCharacter(char ch, int numTimes, long nanoseconds)
{
	struct timespec t1,t2;
	t1.tv_sec=0;
	t1.tv_nsec = nanoseconds;
	for(int i=1;i<=numTimes;i++)
	{
		printf("%c",ch);
		fflush(stdout);
		nanosleep(&t1,&t2);
	}
	printf("\n");
}



#endif /* FREAKTIMELOGGER_H_ */
