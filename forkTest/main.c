#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stddef.h>

int gloval = 6;
void main(void)
{
	int autoval = 80;
	pid_t pid;

	printf("befor vfork\n");
	if ((pid = vfork()) < 0) {
		perror("fork error");
	} else if (pid == 0 ) {
		gloval++;
		autoval++;
		_exit(0);
	}

	printf("gloval:%d, autoval:%d\n", gloval, autoval);
	exit(0);
}
