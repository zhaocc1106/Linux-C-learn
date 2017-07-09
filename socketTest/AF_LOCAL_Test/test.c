#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/un.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#define filename "test.socket"

void setnonblocking(int fd)
{
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void *client_func(void *arg)
{
  sleep(3);
  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  setnonblocking(fd);
  struct sockaddr_un un;
  memset(&un, 0, sizeof(un));
  un.sun_family = AF_UNIX;
  sprintf(un.sun_path, "file_%d.socket", (int)getpid());
  unlink(un.sun_path);
  socklen_t len = sizeof(un);
  bind(fd, (struct sockaddr *)&un, sizeof(un));
  strcpy(un.sun_path, filename);
  int ret = connect(fd, (struct sockaddr *)&un, len);
  if (ret == -1)
  {
    printf("connect server failed...\n");
    close(fd);
    return NULL;
  }
  char buf[256];
  memset(buf, 0, sizeof(buf));
  strcpy(buf, "hello world");
  int n = send(fd, buf, strlen(buf)+1, 0);
  printf("send data, %d bytes..\n", n);
  sleep(5);
  close(fd);

  return NULL;
}

int main()
{
  unlink(filename);
  signal(SIGPIPE, SIG_IGN);
  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  int yes = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  setnonblocking(fd);
  struct sockaddr_un un;
  memset(&un, 0, sizeof(un));
  un.sun_family = AF_UNIX;
  strcpy(un.sun_path, filename);
  bind(fd, (struct sockaddr *)&un, sizeof(un));
  listen(fd, 100);

  pthread_t pid;
  pthread_create(&pid, NULL, client_func, NULL);

  struct sockaddr_un uu;
  socklen_t len = sizeof(uu);
  while (1)
  {
    memset(&uu, 0, len);
    int newfd = accept(fd, (struct sockaddr *)&uu, &len);
    if (newfd != -1)
    {
      setnonblocking(newfd);
      printf("newfd = %d, path = %s\n", newfd, uu.sun_path);
      char buf[512];
      memset(buf, 0, sizeof(buf));
      int n = recv(newfd, buf, 512,0);
      printf("recv %d bytes, contents is %s\n", n, buf);
    }
    usleep(100000);
  }

  close(fd);

  return 0;
}
