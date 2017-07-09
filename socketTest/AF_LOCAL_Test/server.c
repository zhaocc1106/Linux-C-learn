#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<string.h>
#include<sys/un.h>
#include<stddef.h>
#include<sys/stat.h>
#include<errno.h>
#define SERV_PATH "/home/zhaocc/sourceCode/linux c/socketTest/AF_LOCAL_Test/server.sock"
int serv_listen(const char * filepath);
int serv_accept(int listenfd);

int main(void) {
	int fd;
	fd = serv_listen(SERV_PATH);
	if (fd != -1) {
		fd = serv_accept(fd);
		if (fd == -1)
			return 1;
		while (1) {
			char buf[BUFSIZ];
			ssize_t n;
			if ((n = read(fd, buf, sizeof(buf))) < 0) {
				perror("recv failed");
				unlink(SERV_PATH);
				exit(1);
			} else if (n == 0){
				printf("n = 0, close fd\n");
				close(fd);
				unlink(SERV_PATH);
				exit(0);
			}
			printf("read:%s n = %d\n", buf, n);
		}
	}
	unlink(SERV_PATH);
	return 0;
}

int serv_listen(const char *filepath) {
	int serverfd;
	serverfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	int n = 1;
	setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(n));
	struct sockaddr_un sun;
	memset(&sun, 0, sizeof(struct sockaddr_un));
	sun.sun_family = AF_UNIX;
	strcpy(sun.sun_path, filepath);
	unlink(filepath);
	if (((bind(serverfd, (struct sockaddr*) &sun,
			1 + offsetof(struct sockaddr_un, sun_path) + strlen(filepath))))
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
	struct sockaddr_un client_sun;
	int un_len = sizeof(struct sockaddr_un);
	bzero(&client_sun, un_len);
	printf("accepting...\n");
	if ((confd = accept(listenfd, (struct sockaddr*) &client_sun, &un_len))
			< 0) {
		perror("accept failed");
		return -1;
	}
	struct stat statbuf;
	if (stat(client_sun.sun_path, &statbuf) < 0) {
		perror("stat failed");
		return -1;
	}
	if (S_ISSOCK(statbuf.st_mode == 0)) {
		perror("not a socket");
		return -1;
	}
	printf("accepted, path:%s\n", client_sun.sun_path);
	return confd;
}
