#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>

int main(void)
{
	int sockfd[2];
	pid_t pid;
	if ((socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd) < 0))
	{
		perror("socketpair failed");
		exit(1);
	}
	if (((pid = fork()) == -1))
	{
		perror("fork failed ");
		exit(1);
	}
	else if (pid == 0)
	{
		char s[BUFSIZ];
		ssize_t n;
		close(sockfd[1]);
		if ((n = read(sockfd[0], s, sizeof(s))) < 0)
		{
			perror("read error");
			exit(1);
		}
		printf("read:%s\n", s);
		close(sockfd[0]);
		exit(0);
	}
	else if (pid > 0)
	{
		char buf[] = "hello socketpair!";
		ssize_t n;
		close(sockfd[0]);
		if ((n = write(sockfd[1], buf, sizeof(buf))) < 0)
		{
			perror("write failed");
			exit(1);
		}
		close(sockfd[1]);
		wait(NULL);
	}
	return 0;
}

