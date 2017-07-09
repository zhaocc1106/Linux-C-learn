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
#include<sys/epoll.h>
#include<sys/eventfd.h>
#define SERV_PATH "/home/zhaocc/sourceCode/linux c/socketTest/AF_LOCAL_Test/server.sock"
#define FDS_LEN 128

int serv_listen(const char * filepath);
void serv_epoll(int listenfd);
int serv_accept(int listenfd);
int serv_read(int acceptfd);

int main(void) {
	int fd;
	fd = serv_listen(SERV_PATH);
	serv_epoll(fd);
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

void serv_epoll(int listenfd) {
	int epfd = epoll_create(FDS_LEN);
	int len = 0;
	struct epoll_event event;
	event.data.fd = listenfd;
	event.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &event);
	len++;
	int i;
	while(1) {
		printf("epoll...\n");
		struct epoll_event events[FDS_LEN];
		int num_events = epoll_wait(epfd, events, FDS_LEN, 5000);
		switch(num_events) {
		case 0:
			printf("timeout\n");
			break;
		case -1:
			perror("poll failed");
			exit(1);
			break;
		default:
			for (i = 0; i < num_events; i++) {
				if ((int)events[i].data.fd == listenfd && events[i].events & EPOLLIN) {
					int acceptfd = serv_accept(listenfd);
					if (-1 != acceptfd) {
						if (len < FDS_LEN) {
							event.data.fd = acceptfd;
							event.events = EPOLLIN;
							epoll_ctl(epfd, EPOLL_CTL_ADD, acceptfd, &event);
							printf("accept fd = %d\n", event.data.fd);
						} else {
							close(acceptfd);
							printf("accpet fd full!\n");
						}
					}
				} else if ((int)events[i].data.fd != -1 && events[i].events & EPOLLIN) {
					if (-1 == serv_read(events[i].data.fd)) {
						epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
						printf("delete fd = %d\n", events[i].data.fd);
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
