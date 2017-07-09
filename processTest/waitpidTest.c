#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
int main(void)
{
	int status;
	pid_t pc, pr;
	if ((pc = fork()) < 0)
	{
		printf("fork failed!\n");
		exit(1);
	}
	else if (pc == 0)
	{
		printf("this is the child process, pid = %d ppid = %d\n", getpid(), getppid());
		exit(0);
	}
	else
	{
		//pr = waitpid(-1, &status, 0);
		pr = wait(&status);
		if (WIFEXITED(status))
		{
			printf("the child process %d exit normarlly\n", pr);
			printf("the return code id %d \n", WEXITSTATUS(status));
		}
		else
		{
			printf("the child exit abnormally\n");
		}
	}
	return 0;
}
