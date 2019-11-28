#include <stdio.h>
extern char name[];
int add(int a,int b)
{
	printf("calling add \n");
	printf("Hello,%s!\n",name);
	return a+b;
}
