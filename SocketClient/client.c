#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<string.h>
#include<sys/un.h>
#include<stddef.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#define SERV_IP "10.101.2.166"
#define SERV_PORT 8000

int client_connect();

int main(void) {
	int fd;
	fd = client_connect();
	if (fd != -1) {
		char buf[] = "I'm client";
		ssize_t n;
		if ((n = send(fd, buf, sizeof(buf), 0)) < 0) {
			perror("write failed");
			exit(1);
		}
		printf("write successful n = %d\n", n);
	}
	close(fd);
	while(1);
	return 0;
}

int client_connect() {
	int clientfd;
	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(struct sockaddr_in));
	sin.sin_family = AF_INET;
	struct in_addr serv_addr;
	serv_addr.s_addr = inet_addr(SERV_IP);
	sin.sin_addr = serv_addr;
	sin.sin_port = SERV_PORT;
	if ((connect(clientfd, (struct sockaddr*) &sin, sizeof(struct sockaddr_in)))
			< 0) {
		perror("connect failed");
		return -1;
	}
	return clientfd;
}

