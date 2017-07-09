#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
	int count = 0;
	pid_t pid;
	pid = fork();
	if (pid < 0)
	{
		printf("error in fork!");
		exit(1);
	}
	else if (pid == 0)
	{
		printf("this is child process count = %d\n", ++count);
		exit(0);
	}
	else
	{
		printf("this is the father process count = %d\n", ++count);
		exit(0);
	}
	return 0;
}
