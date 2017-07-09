#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<string.h>
#include<sys/un.h>
#include<stddef.h>
#include<sys/stat.h>
#include<errno.h>
#include<sys/time.h>
#include<sys/select.h>
#define SERV_PATH "/home/zhaocc/sourceCode/linux c/socketTest/AF_LOCAL_Test/server.sock"
#define FDS_LEN 128

int serv_listen(const char * filepath);
void serv_select(int listenfd);
int serv_accept(int listenfd);
int serv_read(int acceptfd);

int main(void) {
	int fd;
	fd = serv_listen(SERV_PATH);
	serv_select(fd);
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

void serv_select(int listenfd) {

	int fds[FDS_LEN];
	fd_set rfds;
	int maxfd = -1;
	int i = 0;
	for (i = 0; i < FDS_LEN; i++) {
		fds[i] = -1;
	}

	fds[0] = listenfd;
	while(1) {
		FD_ZERO(&rfds);
		for (i = 0; i < FDS_LEN; i++) {
			if (fds[i] != -1) {
				FD_SET(fds[i], &rfds);
				maxfd = fds[i] > maxfd ? fds[i]:maxfd;
			}
		}
		printf("selecting...\n");
		struct timeval timeout = {5, 0};
		switch(select(maxfd + 1, &rfds, NULL, NULL, &timeout)) {
		case 0:
			printf("timeout\n");
			break;
		case -1:
			perror("select failed");
			exit(1);
			break;
		default:
			if (FD_ISSET(listenfd, &rfds)) {
				int acceptfd = serv_accept(listenfd);
				if (-1 != acceptfd) {
					for (i = 0; i < FDS_LEN; i++) {
						if (fds[i] == -1) {
							fds[i] = acceptfd;
							printf("accpet fd:%d\n", acceptfd);
							break;
						}
					}
					if (i == FDS_LEN) {
						close(acceptfd);
						printf("accpet fd full!\n");
					}
				}
			} else {
				for (i = 0; i < FDS_LEN; i++) {
					if ((fds[i] != -1) && FD_ISSET(fds[i], &rfds)) {
						if (-1 == serv_read(fds[i])) {
							fds[i] = -1;
						}
					}
				}
			}
		}
	}
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

int serv_read(int acceptfd) {
	char buf[BUFSIZ];
	ssize_t n;
	if ((n = recv(acceptfd, buf, sizeof(buf), 0)) < 0) {
		perror("recv failed");
		unlink(SERV_PATH);
		exit(1);
	} else if (n == 0) {
		close(acceptfd);
		printf("client %d is closed\n", acceptfd);
		return -1;
	}
	printf("read:%s\n", buf);
	return 0;
}
