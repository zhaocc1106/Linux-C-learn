#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

int main(void){
	mode_t mode = 0777;
	int retval;
	unlink("fifo");
	if ((retval = mkfifo("fifo", mode)) != 0) {
		perror("mkfifo failed");
		exit(1);
	}
	pid_t pid;
	if ((pid = fork()) < 0) {
		perror("fork failed");
		exit(1);
	} else if (pid == 0) {
		int pipefd;
		printf("child open...\n");
		if ((pipefd = open("fifo", O_RDONLY)) < 0) {
			perror("open failed");
			exit(1);
		}
		printf("child open successful fd = %d\n", pipefd);
		char buf[BUFSIZ] = {0};
		if (read(pipefd, buf, BUFSIZ) < 0) {
			perror("read failed");
			exit(1);
		}
		printf("read: %s\n", buf);
		close(pipefd);
		exit(0);
	} else {
		int pipefd;
		printf("parent open...\n");
		if ((pipefd = open("fifo", O_WRONLY)) < 0) {
			perror("open failed");
			exit(1);
		}
		printf("parent open successful fd = %d\n", pipefd);
		char buf[] = "I'm parent";
		if (write(pipefd, buf, sizeof(buf)) < 0) {
			perror("write failed");
			exit(1);
		}
	}
	return 0;
}
