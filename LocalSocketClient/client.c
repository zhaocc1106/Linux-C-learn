#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<string.h>
#include<sys/un.h>
#include<stddef.h>
#include<sys/stat.h>
#include<thread.h>

int client_connect(const char * filepath);

int main(void) {
	int fd;
	fd = client_connect(
			"/home/zhaocc/sourceCode/linux c/socketTest/AF_LOCAL_Test/");
	if (fd != -1) {
		char buf[] = "I'm client";
		ssize_t n;
		if ((n = send(fd, buf, sizeof(buf), 0)) < 0) {
			perror("write failed");
			exit(1);
		}
		printf("write successful n = %d\n", n);
	}
	return 0;
}

int client_connect(const char *filepath) {
	int clientfd;
	clientfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	int n = 1;
	setsockopt(clientfd, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(n));
	struct sockaddr_un sun;
	memset(&sun, 0, sizeof(struct sockaddr_un));
	sun.sun_family = AF_UNIX;
	sprintf(sun.sun_path, "%s%d.sock", filepath, getpid());
	printf("client_sun_path:%s\n", sun.sun_path);
	unlink(sun.sun_path);
	if (((bind(clientfd, (struct sockaddr*) &sun,
			1 + offsetof(struct sockaddr_un, sun_path) + strlen(sun.sun_path))))
			< 0) {
		perror("bind failed");
		return -1;
	}
	struct sockaddr_un serv_sun;
	memset(&sun, 0, sizeof(struct sockaddr_un));
	serv_sun.sun_family = AF_UNIX;
	strcpy(serv_sun.sun_path,
			"/home/zhaocc/sourceCode/linux c/socketTest/AF_LOCAL_Test/server.sock");
	if ((connect(clientfd, (struct sockaddr*) &serv_sun,
			1 + offsetof(struct sockaddr_un, sun_path) + strlen(serv_sun.sun_path)))
			< 0) {
		perror("connect failed");
		return -1;
	}
	return clientfd;
}

