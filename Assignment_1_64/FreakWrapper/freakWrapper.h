#include<string.h>
#include<stdio.h>
extern int freakPrint(int len,const char * message);

void freakPrintMe(const char * freakMessage)
{
	if(freakMessage!=NULL)
	{
		int length = strlen(freakMessage);
		length++;

		 freakPrint(length,freakMessage);
	}
}

void displayReturnValue(int value)
{
	printf("Return Value is %d.\n",value);
}
