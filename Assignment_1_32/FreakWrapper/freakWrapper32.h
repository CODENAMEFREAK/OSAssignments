#include<string.h>
#include<stdio.h>
extern int _print32(int length,const char * msg);
void freakPrint32(const char * freakMessage)
{
	if(freakMessage!=NULL)
	{
		int length = strlen(freakMessage);
		length++;
		_print32(length,freakMessage);
	}
}

void displayReturnValue32(int value)
{
	printf("Return Value is %d.\n",value);
}
