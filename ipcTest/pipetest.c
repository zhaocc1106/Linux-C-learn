#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<fcntl.h>
#include<signal.h>

void signal_handler_fun(int signal_num) {
	printf("signal : %d\n", signal_num);	
}

int main(void) {
	pid_t pid;
	int fd_pipe[2] = {0};
	if (pipe(fd_pipe) < 0 ) {
		perror("pipe failed");
		exit(1);
	}
	if ((pid = fork()) < 0) {
		perror("fork failed");
		exit(1);
	} else if (pid == 0) {
		/*
		   gin_name
		close(fd_pipe[0]);
		char buf[] = "I'm child";
		int len;
		while(1) {
			int retval;
			fcntl(fd_pipe[1], F_SETFL, O_NONBLOCK);
			if ((retval = write(fd_pipe[1], buf, sizeof(buf))) < 0) {
				perror("write failed");
				exit(1);
			}
			len += retval;
			printf("retval = %d len = %d\n", retval, len);
		}
		close(fd_pipe[1]);*/
		exit(0);
	} else {
		signal(SIGPIPE, SIG_IGN);
		wait(NULL);
	/*	close(fd_pipe[1]);
		char buf[128] = {0};
		fcntl(fd_pipe[0], F_SETFL, O_NONBLOCK);
		printf("reading...\n");
		close(fd_pipe[0]);
		if (read(fd_pipe[0], buf, 128) < 0) {
			perror("read failed");
			exit(1);
		} else {
			printf("read:%s\n", buf);
			close(fd_pipe[0]);
			exit(0);
		} */
		close(fd_pipe[0]);
		char buf[] = "I'm parent";
		int len;
		while(1) {
			int retval;
			if ((retval = write(fd_pipe[1], buf, sizeof(buf))) < 0) {
				perror("write failed");
				exit(1);
			}
			len += retval;
			printf("retval = %d len = %d\n", retval, len);
		}
	}
	return 0;
}
