#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
	int count = 0;
	pid_t pid;
	pid = vfork();
	if (pid < 0)
	{
		printf("error in fork!");
		exit(1);
	}
	else if (pid == 0)
	{
		printf("this is child process count = %d pid = %d ppid = %d\n", ++count, getpid(), getppid());
		exit(0);
	}
	else
	{
		printf("this is father process count = %d pid = %d ppid = %d\n", ++count, getpid(), getppid());
		exit(0);
	}
	return 0;
}
