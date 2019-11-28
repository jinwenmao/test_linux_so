#include <stdio.h>
#include <dlfcn.h>
int add(int a,int b);
char name[100];
int main(){
	int a =10 ,b =20;
int c=0;
strcpy(name,"NHN XDBMS");
	c= add(a,b);
printf("%d + %d = %d \n",a,b,c);
return 0;
}
