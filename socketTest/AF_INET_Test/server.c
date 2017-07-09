#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<string.h>
#include<sys/un.h>
#include<netinet/in.h>
#include<stddef.h>
#include<sys/stat.h>
#include<errno.h>
#include<arpa/inet.h>
#define SERV_IP "10.101.2.166"
#define SERV_PORT 8000

int serv_listen();
int serv_accept(int listenfd);

int main(void) {
	int fd, cifd;
	fd = serv_listen();
	if (fd != -1) {
		cifd = serv_accept(fd);
		if (fd == -1)
			return 1;
		while (1) {
			char buf[BUFSIZ];
			ssize_t n;
			if ((n = read(cifd, buf, sizeof(buf))) < 0) {
				perror("recv failed");
				exit(1);
			} else if (n == 0) {
				printf("n = 0 close the fd\n");
				close(cifd);
				close(fd);
				exit(0);
			}
			printf("read:%s\n", buf);
		}
	}
	return 0;
}

int serv_listen() {
	int serverfd;
	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	int n = 1;
	setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(n));
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(struct sockaddr_in));
	sin.sin_family = AF_INET;
	struct in_addr serv_addr;
	serv_addr.s_addr = inet_addr(SERV_IP);
	sin.sin_addr = serv_addr;
	sin.sin_port = SERV_PORT;
	if (((bind(serverfd, (struct sockaddr*) &sin, sizeof(sin))))
			< 0) {
		perror("bind failed");
		return -1;
	}
	if ((listen(serverfd, 1024)) < 0) {
		perror("listen failed");
		return -1;
	}
	return serverfd;
}

int serv_accept(int listenfd) {
	int confd;
	struct sockaddr_in client_sin;
	int un_len = sizeof(struct sockaddr_in);
	bzero(&client_sin, un_len);
	printf("accepting...\n");
	if ((confd = accept(listenfd, (struct sockaddr*) &client_sin, &un_len))
			< 0) {
		perror("accept failed");
		return -1;
	}
	printf("accepted, address:%s\n", client_sin.sin_addr);
	return confd;
}
