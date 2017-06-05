/*
 * TimeLogger.h
 *
 *  Created on: 16-Apr-2017
 *      Author: codenamefreak
 */

#ifndef TIMELOGGER_H_
#define TIMELOGGER_H_

#include <chrono>
#include <ctime>
#include <string.h>
using namespace std;


Freak_Mutex_Semaphore_BUSY_WAIT loggerSem;
char getCurrentTimeString()
{
loggerSem.atomic_P();
	 std::time_t result = std::time(nullptr);
	    char * out=  std::ctime(&result);
	    while(*out!='\n'){
	    	cout<<*out;
	    	out++;
	    }
	    cout<<"\t";
	   /* int length = strlen(out)+1;
	    char *timestamp = new char[length];
	    for(int i=0;i<length-2;i++)
	    {
	    	timestamp[i] = *(out+i);
	    }
	    timestamp[length-2]='\t';

	    return timestamp;
*
*/
	    loggerSem.atomic_V();
return ' ';

}





#endif /* TIMELOGGER_H_ */
