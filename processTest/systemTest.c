#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
int main(void)
{
	int status;
	if ((status = system(NULL) < 0))
	{
		printf("don't support system()\n");
		exit(1);
	}
	printf("exit status = %d\n", status);
	if ((status = system("date")) < 0)
	{
		printf("system error!\n");
		exit(1);
	}
	printf("exit status = %d\n", status);
	return 0;
}
