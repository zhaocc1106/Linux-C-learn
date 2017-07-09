#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(void) {
	pid_t pid;
	if ((pid = fork()) < 0) {
		perror("fork failed");
		exit(1);
	} else if (pid == 0) {
		//char *envp[] = {"PATH=$PATH:/home/zhaocc/sourceCode/linux_c/execTest", NULL};
		setenv("PATH", "/home/zhaocc/sourceCode/linux c/execTest", 1);
		char *argv[] = {"test.sh", NULL};
		//char *argv2[] = {"echo", "\"$PATH\"", NULL};
		//if (-1 == execvp("echo", argv2)) {
		//	perror("exec faild");
		//}
		system("test.sh");
		printf("PATH = %s\n", getenv("PATH"));
		if (-1 == execlp("test.sh", "test.sh", NULL)) {
			perror("execvp failed");
			exit(1);
		}
		exit(0);
	} else {
		int retval;
		wait(&retval);
		printf("ret = %d\n", retval);
	}
	return 0;
}
